/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */

#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_

#include <sblib/eib/com_objects.h>

class HelperFunctions {
public:
	static void setComObjPtr(ComObjects* bcu, int objno, ComType type, uint16_t& addr);
	static void setFlagsTablePtr(ComObjects* bcu, uint16_t flagsAddr);
};

#endif /* HELPERFUNCTIONS_H_ */
