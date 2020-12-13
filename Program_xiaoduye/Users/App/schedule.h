#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include "production.h"
#include "client.h"

typedef enum state_
{
	STOP = 0,
	PREPARED = 1,
	PRODUCTION_NORMAL = 2,
	PRODUCTION_WAIT = 3,
}state_t;

extern state_t production_state;

void Schedule_StateMchine(void);
void Schedule_Scan(void);

#endif
