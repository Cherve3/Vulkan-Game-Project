#ifndef __COOLDOWNS_H__
#define __COOLDOWNS_H__

#include "gfc_types.h"

typedef struct
{
	Uint32 timer;
	Uint32 old_time;
}Cooldown;

#endif