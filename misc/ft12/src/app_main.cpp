/*
 *  app_main.cpp - The ft12 application's main.
 *
 *  Copyright (c) 2015 Stefan Taferner <stefan.taferner@gmx.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#include <sblib/digital_pin.h>
#include <sblib/io_pin_names.h>
#include <sblib/timeout.h>
#include <sblib/eib/knx_lpdu.h>
#include <sblib/eib/apci.h>
#include <sblib/serial.h>
#include <sblib/bits.h>
#include <sblib/version.h>
#include <cstring>
#include "config.h"
#include "bcu_ft12.h"
#include "ft12_protocol.h"
#include "debug_handler.h"


APP_VERSION("SBft12  ", "0", "03")  // Don't forget to also change the build-variable sw_version

auto bcuFt12 = BcuFt12();  //!< Bus coupling unit Mask version 0x0012 of the ft12 module

/** ft12 bit timeout converted in milliseconds */
constexpr uint32_t ft12ExchangeTimeoutMs = 2 * ((FT12_EXCHANGE_TIMEOUT_BITS * 1000/FT12_BAUD_RATE) + 1);
/** ft12 line idle timeout converted in milliseconds */
constexpr uint32_t ft12LineIdleTimeoutMs = 2 * ((FT12_LINE_IDLE_TIMEOUT_BITS * 1000/FT12_BAUD_RATE) + 1);

constexpr uint16_t FT12_MASK_VERSION = 0x0012;

uint8_t ftFrameIn[FT_FRAME_SIZE] = {};        //!< Buffer for incoming FT1.2 frames
uint8_t ftFrameInLen = 0;                     //!< Length of the data in ftFrameIn
uint8_t ftFrameOut[FT_FRAME_SIZE] = {};       //!< Buffer for preparing FT1.2 frames to send to serial port
uint8_t ftFrameOutBuffer[FT_FRAME_SIZE] = {}; //!< Buffer for outgoing FT1.2 frames which are waiting an ACK
uint8_t ftFrameOutBufferLength = 0;           //!< Length of the data in ftFrameOutBuffer

int16_t repeatCounter = 0;                  //! Decrement on every repeat until its zero, initialized with @ref FT12_REPEAT_LIMIT

uint32_t lastSerialRecvTime = 0;
uint32_t lastSerialSendTime = 0;

bool sendFrameCountBit = true;
bool rcvFrameCountBit = true;

int16_t lastCheckSum = InvalidCheckSum;
Timeout ft12AckTimeout;     //!< waiting for ft12 ACK timeout
Timeout knxRxTimeout;       //!< KNX-Rx LED blinking timeout

FtFrameType ftFrameType = FT_NONE;

bool ackPending()
{
    const auto stopped = ft12AckTimeout.stopped();
    const auto expired = ft12AckTimeout.expired();
    return !stopped && !expired;
}

void resetTx()
{
    ft12AckTimeout.stop();
    ftFrameOutBufferLength = 0;
    lastSerialSendTime = 0;
    repeatCounter = FT12_REPEAT_LIMIT;
}

void resetRx()
{
    ftFrameInLen = 0;
    ftFrameType = FT_NONE;
    lastSerialRecvTime = 0;
    lastCheckSum = InvalidCheckSum;
}

/**
 * Reset the buffers.
 */
void reset()
{
    serial.clearBuffers();
    sendFrameCountBit = true;
    rcvFrameCountBit = true;
    resetTx();
    resetRx();
}

uint8_t * getFtFrameOut()
{
    ///\todo This can't work, because right now there are three ways to land here
    ///      loop()->processVariableFrame->processDataConnectedRequest
    ///      loop()->processVariableFrame
    ///      loop()->processTelegram
    ///      All of them start in loop(), which would reset a pending ACK, but
    ///      calling loop() here would end in a recursion, so DON'T do it.
    //while (ackPending())
    //{
    //    debugFatal();
    //}
    memset(ftFrameOut, 0, sizeof(ftFrameOut)/sizeof(ftFrameOut[0]));
    return ftFrameOut;
}

/**
 * Sends a @ref FT_ACK
 */
void sendFt12Ack()
{
    serial.write(FT_ACK);
    serial.flush(); /// Don't buffer the ACK, send it right now
    digitalWrite(LED_SERIAL_RX, LED_OFF);
    dump(softUART.println("<-A"));
}

/**
 * Sends a ft12 frame and starts the ack-timeout timer
 * @param frame     ft12 frame to send
 * @param frameSize size of the frame
 */
void sendFt12withAckWaiting(uint8_t* frame, const int32_t frameSize)
{
    constexpr uint8_t CONTROL_FIELD_POSITION = 4;
    if (frameSize < CONTROL_FIELD_POSITION + 1)
    {
        debugFatal();
        return;
    }

    // ReSharper disable once CppDFAArrayIndexOutOfBounds
    FtControlField cf  = controlFieldFromByte(frame[CONTROL_FIELD_POSITION]);
    if (cf.frameCountBitValid && ackPending())
    {
        ///\todo This happens, when calimero sends a non-blocking frame, followed by another frame
        // E.g. non-blocking TL4 T_ACK followed by blocking T_DataConnected within
        // #68#09#09#68 #73#11#00#00#00#FF#C0#60#C2#65#16 (calimero log: sending FT1.2 frame, non-blocking, attempt 1)
        // #68#17#17#68 #53#11#0C#00#00#FF#C0#6E#46#F8#BF#3D#90#02#13#49#84#5E#AF#67#AB#F5#5D#BA#16 (calimero log: sending FT1.2 frame, blocking, attempt 1)
        // Maybe this is a bug in calimero related to https://github.com/calimero-project/calimero-core/issues/91

        //debugFatal();
        uint8_t operand;
        if (cf.frameCountBit) // we flip 5.bit, so calculate new checksum
        {
            operand = -(1 << 5);
        }
        else
        {
            operand = +(1 << 5);
        }
        frame[frameSize - 2] = (frame[frameSize - 2] + operand) % 256; // set checksum for flipped frameCountBit
        cf.frameCountBit = !cf.frameCountBit; // flip frameCountBit
        // ReSharper disable once CppDFAArrayIndexOutOfBounds
        frame[CONTROL_FIELD_POSITION] = controlFieldToByte(cf);
    }
    resetTx();

    ftFrameOutBufferLength = frameSize;
    memcpy(ftFrameOutBuffer, frame, ftFrameOutBufferLength);
    serial.write(frame, ftFrameOutBufferLength);
    lastSerialSendTime = millis();
    ft12AckTimeout.start(ft12ExchangeTimeoutMs);
    digitalWrite(LED_SERIAL_RX, LED_ON);
    dump(softUART.print("<-Tx ", ftFrameOutBufferLength);)
}

void sendFt12RepeatedFrame()
{
    if (ftFrameOutBufferLength == 0)
    {
        resetTx();
        return;
    }

    if (repeatCounter <= 0)
    {
        resetTx();
        return;
    }

    serial.write(ftFrameOutBuffer, ftFrameOutBufferLength);
    ft12AckTimeout.start(ft12ExchangeTimeoutMs);
    repeatCounter--;
    digitalWrite(LED_SERIAL_RX, LED_ON);
    dump(softUART.println("ERROR Tx ", ftFrameOutBufferLength);)
}

/**
 * \brief Send the status response as a fixed frame.
 * 
 * The status response is sent in response to an @ref FC_REQ_STATUS function code and contains the data flow control bit
 * which indicates if the BCU can receive more frames or not.
 * \param bufferFull  The data flow control bit to send in the status response, true if the ft12 part is busy, false otherwise
 * \note KNX Spec. 3.0 3/6/2 6.4.4.3 p.26 Control Field from secondary station
 * \warning This function is UNTESTED, because I have no idea who would trigger it.
 */
void sendFt12StatusResponse(const bool bufferFull)
{
    dump(softUART.println("FC_RESPOND_STATUS ", bufferFull);)
    constexpr uint8_t DIR = 1 << 7; // BAU (bus access unit) to external station
    constexpr uint8_t PRM = 0 << 6; // Message from responding station
    constexpr uint8_t RES = 0 << 5; // reserved
    const uint8_t DFC = bufferFull << 4; // Data flow control

    uint8_t frame[FIXED_FRAME_LENGTH];
    frame[0] = FT_FIXED_START;
    frame[1] = DIR | PRM | RES | DFC | FC_RESPOND_STATUS;
    frame[2] = frame[1]; // checksum modulo 256 of one byte control field is the same as the control field
    frame[3] = FT_END;
    serial.write(frame, FIXED_FRAME_LENGTH);
}

/**
 * Initialize the application.
 */
BcuBase* setup()
{
    // led init and test
    pinMode(LED_KNX_RX, OUTPUT);    // KNX-Rx LED
    digitalWrite(LED_KNX_RX, LED_ON);
    delay(LED_TEST_MS);
    pinMode(LED_SERIAL_RX, OUTPUT); // Serial-Rx LED
    digitalWrite(LED_SERIAL_RX, LED_ON);
    delay(LED_TEST_MS);
    digitalWrite(LED_KNX_RX, LED_OFF);
    delay(LED_TEST_MS);
    digitalWrite(LED_SERIAL_RX, LED_OFF);

    dump(
        softUART.begin();
        softUART.println("Selfbus ft12");
        softUART.println("-> = Rx (Receive)");
        softUART.println("<- = Tx (Transmit)");
        softUART.println("A  = ACK (EMI)");


        softUART.println("------------------");

    )

    bcuFt12.begin(); // bcu.userRam->status is set in BcuFt12::begin()
    bcuFt12.setOwnAddress(FT_OWN_KNX_ADDRESS);
    serial.setTxPin(PIN_FT_SERIAL_TX);
    pinMode(PIN_FT_SERIAL_RX, SERIAL_RXD | PULL_UP | HYSTERESIS);
    serial.begin(FT12_BAUD_RATE, SERIAL_8E1, RxTriggerLevel::CHAR_1); /// ACKs are just one byte, and we need to be fast
    reset();
    return &bcuFt12;
}


/**
 * This is a VERY, VERY dirty hack to replace the KNX sender address 0.0.0 with 15.15.255 for @ref APCI_INDIVIDUAL_ADDRESS_RESPONSE_PDU
 * Older ARM Selfbus devices with clean flash have the default KNX-address 0.0.0 which is not allowed in KNX specification so e.g. knxd ignores their response.
 * Even KNX-address programming with a broadcasted @ref APCI_INDIVIDUAL_ADDRESS_READ_PDU will fail.
 *
 * @warning This is a VERY, VERY dirty hack and not even close to KNX spec!
 *
 * @param frame          The buffer that contains the frame
 * @param funcCode       The function code, e.g. FC_SEND_UDAT
 * @param emi            The @ref EmiCode to send
 * @param userDataLength The length of the frame's payload
 *
 * @return true if sender address was replaced with 15.15.255, otherwise false
 */
bool dirtyCheckAndReplaceInvalidDefaultSenderAddress(uint8_t* frame, const FtFunctionCode& funcCode, const EmiCode& emi, const uint8_t& userDataLength)
{
    // invalid frame example:
    // # 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
    //  68 0A 0A 68 D3 29 B0 00 00 00 00 E1 01 40 CE 16

    // user data indication with length 10?
    if ((emi != L_Data_Ind) || (userDataLength != 0x0A) || ((funcCode & 0x0f) != FC_SEND_UDAT))
    {
        return false;
    }

    // check 0.0.0 for KNX sender and destination address
    if ((frame[7] != 0) || (frame[8] != 0) || (frame[9] != 0) || (frame[10] != 0))
    {
        return false;
    }

    // is apci command = APCI_INDIVIDUAL_ADDRESS_RESPONSE_PDU ??
    if (makeWord(frame[12], frame[13]) != APCI_INDIVIDUAL_ADDRESS_RESPONSE_PDU)
    {
        return false;
    }

    // is response length = 1??
    if ((frame[11] & 0x0f) != 1) // response length in low nibble
    {
        return false;
    }

    // okay seems like we had a bad day
    // -> inject sender address 15.15.255
    frame[7] = 0xff;
    frame[8] = 0xff;
    // Not really an error, more of a warning, but makes it easier to search in log
    dump(softUART.println("ERROR new 0.0.0->15.15.255"))
    return true;
}

/**
 * \brief Send a FT frame of variable length.
 *
 * The frame buffer must have enough space, so that the checksum and end byte are added.
 * @param frame          The buffer that contains the frame
 * @param funcCode       The function code, e.g. FC_SEND_UDAT
 * @param emi            The @ref EmiCode to send
 * @param userDataLength The length of the frame's payload, including the control field and EMI
 */
void sendVariableFrame(uint8_t* frame, const FtFunctionCode& funcCode, const EmiCode& emi, const uint8_t& userDataLength)
{
    if (funcCode != FC_SEND_UDAT)
    {
        // only FC_SEND_UDAT allowed as variable length frame
        debugFatal();
        return;
    }

    if (userDataLength < VARIABLE_FRAME_USER_DATA_LENGTH_MIN)
    {
        debugFatal();
        return;
    }
    if (userDataLength > VARIABLE_FRAME_USER_DATA_LENGTH_MAX)
    {
        debugFatal();
        return;
    }

    // This is a VERY, VERY dirty hack for older Selfbus devices with default knx address 0.0.0
    dirtyCheckAndReplaceInvalidDefaultSenderAddress(frame, funcCode, emi, userDataLength);

    const FtControlField cf { true, true, sendFrameCountBit, true, funcCode };
    frame[0] = FT_VARIABLE_START;
    frame[1] = userDataLength;
    frame[2] = userDataLength;
    frame[3] = FT_VARIABLE_START;
    frame[4] = controlFieldToByte(cf);
    frame[5] = emi;

    frame[4 + userDataLength] = calcCheckSum(frame, userDataLength);
    frame[5 + userDataLength] = FT_END;
    sendFt12withAckWaiting(frame, userDataLength + VARIABLE_FRAME_HEADER_LENGTH);
}

/**
 * Process a fixed length FT frame
 *
 * @param frame - 4 byte Buffer containing the fixed length frame to process
 * @note KNX Spec. 2.1 3/6/2 6.4.3.2 p.23ff
 */
bool processFixedFrame(const uint8_t* frame)
{
    dump(softUART.print("->Rx ");)
    const FtControlField cf  = controlFieldFromByte(frame[1]);

    if (!cf.isRequest)
    {
        debugFatal();
        return false;
    }

    switch (cf.functionCode)
    {
        case FC_SEND_RESET:
            dump(softUART.println("FC_SEND_RESET");)
            reset();
            return true;

        case FC_REQ_STATUS:
            dump(softUART.println("FC_REQ_STATUS");)
            sendFt12StatusResponse(ackPending());
            return true;

        default:
            dump(softUART.println("ERROR FC unknown");)
            debugFatal();
            return false;
    }

}

/**
 * Process an L_DataConnected request in frame[]
 */
void handleDataConnectedRequest(const uint8_t * frame, const uint8_t frameLength)
{
    if (frameLength < 14)
    {
        debugFatal();
        return;
    }

    const uint16_t apci = makeWord(frame[12], frame[13]);
    if (apci == APCI_DEVICEDESCRIPTOR_READ_PDU)
    {
        ///\todo Who needs this response to work?
        ///      It's here at least since commit a3dafc9 09.06.2015 12:29:07
        uint8_t * buffer = getFtFrameOut();
        buffer[11] = 0x60 | 3; // routing count in high nibble + response length in low nibble
        buffer[12] = HIGH_BYTE(APCI_DEVICEDESCRIPTOR_RESPONSE_PDU);
        buffer[13] = lowByte(APCI_DEVICEDESCRIPTOR_RESPONSE_PDU);
        buffer[14] = HIGH_BYTE(FT12_MASK_VERSION);
        buffer[15] = lowByte(FT12_MASK_VERSION);
        ///\todo shouldn't userDataLength be 8 (buffer[10..17] (including the control field and EMI) instead of 12?
        sendVariableFrame(buffer, FC_SEND_UDAT, T_Data_Connected_Con, 12);
    }
    else
    {
        dump(softUART.println("ERROR apci unknown 0x", apci);)
        debugFatal();
    }
}

/**
 * Process a variable length FT frame
 */
bool processVariableFrame(const uint8_t* emiFrame, const uint8_t emiFrameLength)
{
    dump(softUART.print("->Rx ");)
    const FtControlField cf  = controlFieldFromByte(emiFrame[4]);

    if (cf.functionCode != FC_SEND_UDAT)
    {
        debugFatal();
        return false;
    }

    const uint8_t checkSum = emiFrame[emiFrameLength - 2];
    if (!cf.frameCountBitValid)
    {
        debugFatal();
        return false;
    }

    // Compare cf.frameCountBit and checksum with last received once
    if (cf.frameCountBit == rcvFrameCountBit)
    {
        if ((checkSum == lastCheckSum) && (lastCheckSum != InvalidCheckSum))
        {
            // Same cf.frameCountBit and checksum => ignore already received repeated frame
            ///\todo maybe we need to send our last L_Data_Con?
            return true;
        }
    }
    rcvFrameCountBit = cf.frameCountBit;
    lastCheckSum = checkSum;

    uint8_t * emiSendBuffer;
    auto emiCode = static_cast<EmiCode>(emiFrame[5]);
    switch (emiCode)
    {
    case PEI_Identify_Req: // KNX Spec. 3.0 3/6/3 3.3.9.5 p.56, 57
        /// Build PEI_Identify.con
        dump(softUART.println("Id");)
        emiSendBuffer = getFtFrameOut();
        emiSendBuffer[6]  = HIGH_BYTE(bcuFt12.ownAddress());
        emiSendBuffer[7]  = lowByte(bcuFt12.ownAddress());
        emiSendBuffer[8]  = 0x00; // 6 bytes KNX serial number
        emiSendBuffer[9]  = 0x01;
        emiSendBuffer[10] = 0x00;
        emiSendBuffer[11] = 0x01;
        emiSendBuffer[12] = 0xE4;
        emiSendBuffer[13] = 0x5A; // 6. byte
        emiSendBuffer[14] = 0; ///\todo A byte to much set, which is not transmitted
        sendVariableFrame(emiSendBuffer, FC_SEND_UDAT, PEI_Identify_Con, 10);
        break;

    case PEI_Switch_Req:
        bcuFt12.handlePeiSwitchRequest(&emiFrame[6], emiFrameLength - 8);
        /// Reset has its own EMI code, so don't call reset() here.
        break;

    case T_Connect_Req:
        /// Build T_Connect.con message, KNX Spec 3.0 3/6/3 3.3.6.3 p.34
        dump(softUART.println("Connect");)
        emiSendBuffer = getFtFrameOut();
        emiSendBuffer[6]  = 0; // control (unused)
        emiSendBuffer[7]  = emiFrame[9];  // destination address
        emiSendBuffer[8]  = emiFrame[10]; // destination address
        emiSendBuffer[9]  = 0; // XX
        emiSendBuffer[10] = 0; // 0x00
        emiSendBuffer[11] = 0; ///\todo Again a byte to much set, which is not transmitted
        sendVariableFrame(emiSendBuffer, FC_SEND_UDAT, T_Connect_Con, 7);
        break;

    case T_Data_Connected_Req:
        dump(softUART.println("DataConn ", emiFrameLength);)
        handleDataConnectedRequest(emiFrame, emiFrameLength);
        break;

    case L_Data_Req: // KNX Spec. 3.0 3/6/3 3.3.4.2 p.21
    {
        const uint8_t userDataLength = emiFrame[1];
        dump(
            /// Include header length, for easier log interpretation
            softUART.print("DataReq ", userDataLength + VARIABLE_FRAME_HEADER_LENGTH);
        )
        const uint8_t emiControl = emiFrame[VARIABLE_FRAME_HEADER_LENGTH];
        // No need to evaluate ack_request(a) (bit 1 of L_Data.req control byte),
        // because L_Data service on KNX TP1 is always acknowledge driven (KNX Spec. 3.0 3/2/2 2.4.2 p.38)
        // but we need to read requested priority to prepare the control byte of the KNX telegram
        const uint8_t emiPriority = emiControl & 0x0c;

        emiSendBuffer = getFtFrameOut();
        memcpy(emiSendBuffer, emiFrame, emiFrameLength - 2);
        // set requested priority and positive ACK flag (last bit 0)
        emiSendBuffer[VARIABLE_FRAME_HEADER_LENGTH] = emiPriority & 0xfe;
        sendVariableFrame(emiSendBuffer, FC_SEND_UDAT, L_Data_Con, userDataLength);

        const uint8_t knxTelegramLength = userDataLength - 2;
        // Wait till bcuFt12 has sent our previous KNX telegram
        uint8_t * knxSendBuffer = bcuFt12.acquireSendBuffer();

        // copy emiFrame userdata to knxSendBuffer
        memcpy(knxSendBuffer, &emiFrame[VARIABLE_FRAME_HEADER_LENGTH], knxTelegramLength);

        initLpdu(knxSendBuffer, static_cast<KNXPriority>(emiPriority >> 2), false, FRAME_STANDARD);
        dump(softUART.println("Kou ", knxTelegramLength);)
        bcuFt12.bus->sendTelegram(knxSendBuffer, knxTelegramLength);
        break;
    }

    default:
        dump(softUART.println("ERROR EMI unknown");)
        debugFatal();
        return false;
    }
    return true;
}

/**
 * Send a telegram from the KNX bus as variable frame to the User.
 */
void processTelegram()
{
    uint8_t * buffer = getFtFrameOut();
    const int32_t knxTelegramLength = bcuFt12.bus->telegramLen - 1; // -1 to exclude the KNX telegram checksum
    dump(softUART.print("Kin ", knxTelegramLength);)

    if (knxTelegramLength < (VARIABLE_FRAME_USER_DATA_LENGTH_MIN - 2)) // -2 exclude control field and EMI
    {
        debugFatal();
        return;
    }
    if (knxTelegramLength > (VARIABLE_FRAME_USER_DATA_LENGTH_MAX - 2)) // -2 exclude control field and EMI
    {
        debugFatal();
        return;
    }
    memcpy(buffer + VARIABLE_FRAME_HEADER_LENGTH, bcuFt12.bus->telegram, knxTelegramLength);
    sendVariableFrame(buffer, FC_SEND_UDAT, L_Data_Ind, knxTelegramLength + 2); // +2 control field and EMI
    dump(softUART.println();)
}

/**
 * The main processing loop.
 */
void loop()
{
    if (knxRxTimeout.expired())
    {
        digitalWrite(LED_KNX_RX, LED_OFF);
    }

    int32_t rxByte;
    while ((rxByte = serial.read()) > -1) ///\todo read all in once into buffer of size FT_FRAME_SIZE
    {
        lastSerialRecvTime = millis();
        // start byte / frame detection, fixed or variable frame or just an ack
        if (ftFrameType == FT_NONE)
        {
            switch (rxByte)
            {
                case FT_ACK:
                {
                    dump(
                        softUART.println("->A");
                        if (!ackPending())
                        {
                            ///\todo been here with knxd, but it should never happen
                            softUART.println("ERROR FT_NONE:ACK");
                        }
                    )
                    resetTx();
                    sendFrameCountBit = !sendFrameCountBit;
                    digitalWrite(LED_SERIAL_RX, LED_OFF);
                    continue;
                }
                case FT_FIXED_START:
                    ftFrameType = FT_FIXED_START;
                    break;
                case FT_VARIABLE_START:
                    ftFrameType = FT_VARIABLE_START;
                    break;
                case 0xA0: ///\todo Can't find 0xA0 in KNX Spec 3.0 3/6/2 6.4.3 Transmission Frame Format p.22ff
                    dump(softUART.println("ERROR UNKNOWN rxByte 0x", rxByte, HEX);)
                    reset();
                    continue;
                default:
                    /// We may land here on lost bytes, receiving was corrupted, other side closed its serial port...
                    ftFrameType = FT_NONE;
                    continue;
            }
        }

        // buffer overflow prevention
        if (ftFrameInLen >= FT_FRAME_SIZE)
        {
            debugFatal();
            reset();
            continue;
        }

        ftFrameIn[ftFrameInLen++] = rxByte;

        if (rxByte != FT_END)
        {
            continue;
        }

        if (ftFrameType == FT_FIXED_START)
        {
            const FtError ftError = isValidFixedFrameHeader(&ftFrameIn[0], ftFrameInLen);
            switch (ftError)
            {
                case FtError::FT_TOO_SHORT:
                    continue;

                case FtError::FT_TOO_LONG:
                case FtError::FT_INVALID_START:
                case FtError::FT_INVALID_END:
                case FtError::FT_INVALID_CHECKSUM:
                    resetRx();
                    debugFatal();
                    break;

                case FtError::FT_NO_ERROR:
                    sendFt12Ack();
                    dump(
                        if (ackPending())
                        {
                            /// This may happen, see sendFt12withAckWaiting(..) for details
                            softUART.println("ERROR FT_FIXED_START: ACK pending");
                        }
                    );

                    if (!processFixedFrame(&ftFrameIn[0]))
                    {
                        debugFatal();
                    }
                    resetRx();
                    break;

                default:
                    debugFatal();
                    resetRx();
                    break;
            }
        }
        else if (ftFrameType == FT_VARIABLE_START)
        {
            const FtError ftError = isValidVariableFrameHeader(&ftFrameIn[0], ftFrameInLen);
            switch (ftError)
            {
                case FtError::FT_TOO_SHORT:
                    continue;

                case FtError::FT_TOO_LONG:
                case FtError::FT_INVALID_START:
                case FtError::FT_INVALID_END:
                case FtError::FT_INVALID_CHECKSUM:
                case FtError::FT_INVALID_LENGTH:
                    resetRx();
                    dump(softUART.print("ERROR invalid frame ");)
                    break;

                case FtError::FT_NO_ERROR:
                    sendFt12Ack();
                    dump(
                        if (ackPending())
                        {
                            /// This may happen, see sendFt12withAckWaiting(..) for details
                            softUART.println("ERROR FT_VARIABLE_START: ACK pending");
                        }
                    );
                    if (!processVariableFrame(&ftFrameIn[0], ftFrameInLen))
                    {
                        debugFatal();
                    }
                    resetRx();
                    break;

                default:
                    debugFatal();
                    resetRx();
                    break;
            }
        }
    }

    if (ft12AckTimeout.expired())
    {
        sendFt12RepeatedFrame();
    }

    if (bcuFt12.bus->telegramReceived() && !ackPending())
    {
        digitalWrite(LED_KNX_RX, LED_ON);
        knxRxTimeout.start(LED_KNX_RX_BLINK_TIME_MS);
        if (ftFrameOutBufferLength == 0)
        {
            processTelegram();
            bcuFt12.bus->discardReceivedTelegram();
        }
    }

    if (ftFrameType != FT_NONE && elapsed(lastSerialRecvTime) > ft12ExchangeTimeoutMs)
    {
        resetRx();
    }
}

/**
 * The processing loop while no KNX-application is loaded
 */
void loop_noapp()
{
    loop();
}
