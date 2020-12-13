#ifndef __TEST_H__
#define __TEST_H__

#include "module.h"

typedef enum Module_
{
	Relay = 0,
	CO2_flux,
	Water,
	Approach_Switch,
	stoste,
	stepping,
	cardreader,
	gprs,
}module_e;

void TestModule(module_e m);
void Test_stoste(uint16_t Period);

#endif
