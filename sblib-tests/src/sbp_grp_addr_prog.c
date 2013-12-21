// automatically generated from jung2138-gruppenadressen.trx

#include "CUnit/Basic.h"

#include "sb_proto_test.h"
#include "sb_bus.h"
#include "sb_proto.h"
#include "sb_const.h"
#include "sb_eeprom.h"

static void check_connect(void)
{
  CU_ASSERT(sbConnectedAddr == 0x0001);
}

static void check_disconnect(void)
{
  CU_ASSERT(sbConnectedAddr == 0x0000);
}

static void tc_setup(void)
{
  sbOwnPhysicalAddr = 0x1112; // set own address to 1.1.18
  eeprom[0x04] = 0x04;
  eeprom[0x05] = 0x20;
  eeprom[0x06] = 0x60;
  eeprom[0x07] = 0x01;
  eeprom[0x11] = 0x19;
  eeprom[0x19] = 0x14;
  eeprom[0x1A] = 0xFE;
  eeprom[0x1B] = 0x00;
  eeprom[0x1C] = 0xFE;
  eeprom[0x1D] = 0x01;
  eeprom[0x1E] = 0xFE;
  eeprom[0x1F] = 0x02;
  eeprom[0x20] = 0xFE;
  eeprom[0x21] = 0x03;
  eeprom[0x22] = 0xFE;
  eeprom[0x23] = 0x04;
  eeprom[0x24] = 0xFE;
  eeprom[0x25] = 0x05;
  eeprom[0x26] = 0xFE;
  eeprom[0x27] = 0x06;
  eeprom[0x28] = 0xFE;
  eeprom[0x29] = 0x07;
  eeprom[0x2A] = 0xFE;
  eeprom[0x2B] = 0x08;
  eeprom[0x2C] = 0xFE;
  eeprom[0x2D] = 0x09;
  eeprom[0x2E] = 0xFE;
  eeprom[0x2F] = 0x0A;
  eeprom[0x30] = 0xFE;
  eeprom[0x31] = 0x0B;
  eeprom[0x32] = 0xFE;
  eeprom[0x33] = 0x0C;
  eeprom[0x34] = 0xFE;
  eeprom[0x35] = 0x0D;
  eeprom[0x36] = 0xFE;
  eeprom[0x37] = 0x0E;
  eeprom[0x38] = 0xFE;
  eeprom[0x39] = 0x0F;
  eeprom[0x3A] = 0xFE;
  eeprom[0x3B] = 0x10;
  eeprom[0x3C] = 0xFE;
  eeprom[0x3D] = 0x11;
  eeprom[0x3E] = 0xFE;
  eeprom[0x3F] = 0x12;
  eeprom[0x40] = 0xFE;
  eeprom[0x41] = 0x13;
  eeprom[0x42] = 0x01;
  eeprom[0x43] = 0x00;
}

Telegram group_address_programming_t[] =
{ {TEL_RX,  7, 0, check_connect       , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0x80}} //   1
, {TEL_RX,  8, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x61, 0x43, 0x00}} //   2
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC2}} //   3
, {TEL_TX, 10, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x63, 0x43, 0x40, 0x00, 0x12}} //   4
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xC2}} //   5
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x46, 0x01, 0x01, 0x04}} //   6
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC6}} //   7
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x46, 0x41, 0x01, 0x04, 0x04}} //   8
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xC6}} //   9
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x4A, 0x04, 0x01, 0x04}} //  10
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xCA}} //  11
, {TEL_TX, 14, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x67, 0x4A, 0x44, 0x01, 0x04, 0x04, 0x20, 0x60, 0x01}} //  12
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xCA}} //  13
, {TEL_RX,  7, 0, check_disconnect    , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0x81}} //  14
, {TEL_RX,  7, 0, check_connect       , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0x80}} //  15
, {TEL_RX,  8, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x61, 0x43, 0x00}} //  16
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC2}} //  17
, {TEL_TX, 10, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x63, 0x43, 0x40, 0x00, 0x12}} //  18
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xC2}} //  19
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x46, 0x81, 0x01, 0x0D, 0x00}} //  20
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC6}} //  21
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x4A, 0x01, 0x01, 0x0D}} //  22
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xCA}} //  23
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x46, 0x41, 0x01, 0x0D, 0x00}} //  24
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xC6}} //  25
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x4E, 0x81, 0x01, 0x16, 0x01}} //  26
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xCE}} //  27
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x52, 0x01, 0x01, 0x16}} //  28
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xD2}} //  29
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x4A, 0x41, 0x01, 0x16, 0x01}} //  30
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xCA}} //  31
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x56, 0x01, 0x01, 0x11}} //  32
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xD6}} //  33
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x4E, 0x41, 0x01, 0x11, 0x19}} //  34
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xCE}} //  35
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x5A, 0x81, 0x01, 0x11, 0x1B}} //  36
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xDA}} //  37
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x5E, 0x01, 0x01, 0x11}} //  38
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xDE}} //  39
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x52, 0x41, 0x01, 0x11, 0x1B}} //  40
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xD2}} //  41
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x62, 0x0C, 0x01, 0x19}} //  42
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xE2}} //  43
, {TEL_TX, 22, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x6F, 0x56, 0x4C, 0x01, 0x19, 0x14, 0xFE, 0x00, 0xFE, 0x01, 0xFE, 0x02, 0xFE, 0x03, 0xFE, 0x04, 0xFE}} //  44
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xD6}} //  45
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x66, 0x0C, 0x01, 0x25}} //  46
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xE6}} //  47
, {TEL_TX, 22, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x6F, 0x5A, 0x4C, 0x01, 0x25, 0x05, 0xFE, 0x06, 0xFE, 0x07, 0xFE, 0x08, 0xFE, 0x09, 0xFE, 0x0A, 0xFE}} //  48
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xDA}} //  49
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x6A, 0x0C, 0x01, 0x31}} //  50
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xEA}} //  51
, {TEL_TX, 22, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x6F, 0x5E, 0x4C, 0x01, 0x31, 0x0B, 0xFE, 0x0C, 0xFE, 0x0D, 0xFE, 0x0E, 0xFE, 0x0F, 0xFE, 0x10, 0xFE}} //  52
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xDE}} //  53
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x6E, 0x07, 0x01, 0x3D}} //  54
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xEE}} //  55
, {TEL_TX, 17, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x6A, 0x62, 0x47, 0x01, 0x3D, 0x11, 0xFE, 0x12, 0xFE, 0x13, 0x01, 0x00}} //  56
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xE2}} //  57
, {TEL_RX, 15, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x68, 0x72, 0x85, 0x01, 0x19, 0x00, 0x01, 0x14, 0x01, 0x00}} //  58
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xF2}} //  59
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x76, 0x05, 0x01, 0x19}} //  60
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xF6}} //  61
, {TEL_TX, 15, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x68, 0x66, 0x45, 0x01, 0x19, 0x00, 0x01, 0x14, 0x01, 0x00}} //  62
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xE6}} //  63
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x7A, 0x81, 0x01, 0x1F, 0x01}} //  64
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xFA}} //  65
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x7E, 0x01, 0x01, 0x1F}} //  66
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xFE}} //  67
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x6A, 0x41, 0x01, 0x1F, 0x01}} //  68
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xEA}} //  69
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x42, 0x81, 0x01, 0x21, 0x02}} //  70
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC2}} //  71
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x46, 0x01, 0x01, 0x21}} //  72
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC6}} //  73
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x6E, 0x41, 0x01, 0x21, 0x02}} //  74
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xEE}} //  75
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x4A, 0x81, 0x01, 0x23, 0x03}} //  76
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xCA}} //  77
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x4E, 0x01, 0x01, 0x23}} //  78
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xCE}} //  79
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x72, 0x41, 0x01, 0x23, 0x03}} //  80
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xF2}} //  81
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x52, 0x81, 0x01, 0x25, 0x04}} //  82
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xD2}} //  83
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x56, 0x01, 0x01, 0x25}} //  84
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xD6}} //  85
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x76, 0x41, 0x01, 0x25, 0x04}} //  86
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xF6}} //  87
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x5A, 0x81, 0x01, 0x27, 0x05}} //  88
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xDA}} //  89
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x5E, 0x01, 0x01, 0x27}} //  90
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xDE}} //  91
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x7A, 0x41, 0x01, 0x27, 0x05}} //  92
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xFA}} //  93
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x62, 0x81, 0x01, 0x29, 0x06}} //  94
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xE2}} //  95
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x66, 0x01, 0x01, 0x29}} //  96
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xE6}} //  97
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x7E, 0x41, 0x01, 0x29, 0x06}} //  98
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xFE}} //  99
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x6A, 0x81, 0x01, 0x2B, 0x07}} // 100
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xEA}} // 101
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x6E, 0x01, 0x01, 0x2B}} // 102
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xEE}} // 103
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x42, 0x41, 0x01, 0x2B, 0x07}} // 104
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xC2}} // 105
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x72, 0x81, 0x01, 0x2D, 0x08}} // 106
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xF2}} // 107
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x76, 0x01, 0x01, 0x2D}} // 108
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xF6}} // 109
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x46, 0x41, 0x01, 0x2D, 0x08}} // 110
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xC6}} // 111
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x7A, 0x81, 0x01, 0x2F, 0x09}} // 112
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xFA}} // 113
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x7E, 0x01, 0x01, 0x2F}} // 114
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xFE}} // 115
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x4A, 0x41, 0x01, 0x2F, 0x09}} // 116
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xCA}} // 117
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x42, 0x81, 0x01, 0x31, 0x0A}} // 118
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC2}} // 119
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x46, 0x01, 0x01, 0x31}} // 120
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC6}} // 121
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x4E, 0x41, 0x01, 0x31, 0x0A}} // 122
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xCE}} // 123
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x4A, 0x81, 0x01, 0x33, 0x0B}} // 124
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xCA}} // 125
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x4E, 0x01, 0x01, 0x33}} // 126
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xCE}} // 127
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x52, 0x41, 0x01, 0x33, 0x0B}} // 128
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xD2}} // 129
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x52, 0x81, 0x01, 0x35, 0x0C}} // 130
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xD2}} // 131
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x56, 0x01, 0x01, 0x35}} // 132
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xD6}} // 133
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x56, 0x41, 0x01, 0x35, 0x0C}} // 134
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xD6}} // 135
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x5A, 0x81, 0x01, 0x37, 0x0D}} // 136
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xDA}} // 137
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x5E, 0x01, 0x01, 0x37}} // 138
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xDE}} // 139
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x5A, 0x41, 0x01, 0x37, 0x0D}} // 140
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xDA}} // 141
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x62, 0x81, 0x01, 0x39, 0x0E}} // 142
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xE2}} // 143
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x66, 0x01, 0x01, 0x39}} // 144
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xE6}} // 145
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x5E, 0x41, 0x01, 0x39, 0x0E}} // 146
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xDE}} // 147
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x6A, 0x81, 0x01, 0x3B, 0x0F}} // 148
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xEA}} // 149
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x6E, 0x01, 0x01, 0x3B}} // 150
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xEE}} // 151
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x62, 0x41, 0x01, 0x3B, 0x0F}} // 152
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xE2}} // 153
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x72, 0x81, 0x01, 0x3D, 0x10}} // 154
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xF2}} // 155
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x76, 0x01, 0x01, 0x3D}} // 156
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xF6}} // 157
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x66, 0x41, 0x01, 0x3D, 0x10}} // 158
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xE6}} // 159
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x7A, 0x81, 0x01, 0x3F, 0x11}} // 160
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xFA}} // 161
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x7E, 0x01, 0x01, 0x3F}} // 162
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xFE}} // 163
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x6A, 0x41, 0x01, 0x3F, 0x11}} // 164
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xEA}} // 165
, {TEL_RX, 13, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x66, 0x42, 0x83, 0x01, 0x41, 0x12, 0xFE, 0x13}} // 166
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC2}} // 167
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x46, 0x03, 0x01, 0x41}} // 168
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC6}} // 169
, {TEL_TX, 13, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x66, 0x6E, 0x43, 0x01, 0x41, 0x12, 0xFE, 0x13}} // 170
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xEE}} // 171
, {TEL_RX, 22, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x6F, 0x4A, 0x8C, 0x01, 0x9A, 0x14, 0xD6, 0x54, 0x97, 0x00, 0x55, 0x93, 0x00, 0x56, 0x93, 0x00, 0x57}} // 172
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xCA}} // 173
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x4E, 0x0C, 0x01, 0x9A}} // 174
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xCE}} // 175
, {TEL_TX, 22, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x6F, 0x72, 0x4C, 0x01, 0x9A, 0x14, 0xD6, 0x54, 0x97, 0x00, 0x55, 0x93, 0x00, 0x56, 0x93, 0x00, 0x57}} // 176
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xF2}} // 177
, {TEL_RX, 22, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x6F, 0x52, 0x8C, 0x01, 0xA6, 0x93, 0x00, 0x58, 0x93, 0x00, 0x59, 0x93, 0x00, 0x5A, 0x93, 0x00, 0x5B}} // 178
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xD2}} // 179
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x56, 0x0C, 0x01, 0xA6}} // 180
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xD6}} // 181
, {TEL_TX, 22, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x6F, 0x76, 0x4C, 0x01, 0xA6, 0x93, 0x00, 0x58, 0x93, 0x00, 0x59, 0x93, 0x00, 0x5A, 0x93, 0x00, 0x5B}} // 182
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xF6}} // 183
, {TEL_RX, 22, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x6F, 0x5A, 0x8C, 0x01, 0xB2, 0x93, 0x00, 0x5C, 0x93, 0x01, 0x5D, 0x93, 0x01, 0x5E, 0x93, 0x01, 0x5F}} // 184
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xDA}} // 185
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x5E, 0x0C, 0x01, 0xB2}} // 186
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xDE}} // 187
, {TEL_TX, 22, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x6F, 0x7A, 0x4C, 0x01, 0xB2, 0x93, 0x00, 0x5C, 0x93, 0x01, 0x5D, 0x93, 0x01, 0x5E, 0x93, 0x01, 0x5F}} // 188
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xFA}} // 189
, {TEL_RX, 22, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x6F, 0x62, 0x8C, 0x01, 0xBE, 0x93, 0x01, 0x60, 0xC3, 0x00, 0x61, 0xC3, 0x00, 0x62, 0xC3, 0x00, 0x63}} // 190
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xE2}} // 191
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x66, 0x0C, 0x01, 0xBE}} // 192
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xE6}} // 193
, {TEL_TX, 22, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x6F, 0x7E, 0x4C, 0x01, 0xBE, 0x93, 0x01, 0x60, 0xC3, 0x00, 0x61, 0xC3, 0x00, 0x62, 0xC3, 0x00, 0x63}} // 194
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xFE}} // 195
, {TEL_RX, 22, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x6F, 0x6A, 0x8C, 0x01, 0xCA, 0xC3, 0x00, 0x64, 0xC3, 0x00, 0x65, 0xC3, 0x00, 0x66, 0xC3, 0x00, 0x67}} // 196
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xEA}} // 197
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x6E, 0x0C, 0x01, 0xCA}} // 198
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xEE}} // 199
, {TEL_TX, 22, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x6F, 0x42, 0x4C, 0x01, 0xCA, 0xC3, 0x00, 0x64, 0xC3, 0x00, 0x65, 0xC3, 0x00, 0x66, 0xC3, 0x00, 0x67}} // 200
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xC2}} // 201
, {TEL_RX, 12, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x65, 0x72, 0x82, 0x01, 0xD6, 0xC3, 0x00}} // 202
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xF2}} // 203
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x76, 0x02, 0x01, 0xD6}} // 204
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xF6}} // 205
, {TEL_TX, 12, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x65, 0x46, 0x42, 0x01, 0xD6, 0xC3, 0x00}} // 206
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xC6}} // 207
, {TEL_RX, 19, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x6C, 0x7A, 0x89, 0x00, 0xCE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}} // 208
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xFA}} // 209
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x7E, 0x09, 0x00, 0xCE}} // 210
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xFE}} // 211
, {TEL_TX, 19, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x6C, 0x4A, 0x49, 0x00, 0xCE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}} // 212
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xCA}} // 213
, {TEL_RX, 19, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x6C, 0x42, 0x89, 0x00, 0xD7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}} // 214
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC2}} // 215
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x46, 0x09, 0x00, 0xD7}} // 216
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xC6}} // 217
, {TEL_TX, 19, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x6C, 0x4E, 0x49, 0x00, 0xD7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}} // 218
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xCE}} // 219
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x4A, 0x01, 0x01, 0x16}} // 220
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xCA}} // 221
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x52, 0x41, 0x01, 0x16, 0x01}} // 222
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xD2}} // 223
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x4E, 0x81, 0x01, 0x16, 0x02}} // 224
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xCE}} // 225
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x52, 0x01, 0x01, 0x16}} // 226
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xD2}} // 227
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x56, 0x41, 0x01, 0x16, 0x02}} // 228
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xD6}} // 229
, {TEL_RX, 11, 1, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x64, 0x56, 0x81, 0x01, 0x0D, 0xFF}} // 230
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xD6}} // 231
, {TEL_RX, 10, 2, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x63, 0x5A, 0x01, 0x01, 0x0D}} // 232
, {TEL_TX,  7, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x60, 0xDA}} // 233
, {TEL_TX, 11, 0, NULL                , {0xB0, 0x11, 0x12, 0x00, 0x01, 0x64, 0x5A, 0x41, 0x01, 0x0D, 0xFF}} // 234
, {TEL_RX,  7, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0xDA}} // 235
, {TEL_RX,  8, 0, NULL                , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x61, 0x5F, 0x80}} // 236
, {TEL_RX,  7, 0, check_disconnect    , {0xB0, 0x00, 0x01, 0x11, 0x12, 0x60, 0x81}} // 237
, {END}
};

Test_Case group_address_programming =
{ "Group address programming"
, 0x00   // sbState
, tc_setup // init function
, group_address_programming_t   // telegrams
};