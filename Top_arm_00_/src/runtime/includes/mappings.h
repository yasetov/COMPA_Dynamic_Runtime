/*
 * mapping.h
 *
 *  Created on: 24 nov. 2014
 *      Author: yolivave
 */

#ifndef MAPPING_H_
#define MAPPING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "types.h"

//typedef enum {
//	true,
//	false,
//}bool;

void initMapping();
void initMappingEx(u32 currNbProc);
void dynamicMapping(u32 prevNbProc, u32 currNbProc, u64 execTimes[NB_PROCESSORS][NB_ACTORS + 1]);
u8 changeMapping(u32* prevNbProc, u32* currNbProc);

#ifdef __cplusplus
}
#endif

#endif /* MAPPING_H_ */
