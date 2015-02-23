/*
 * mapping.h
 *
 *  Created on: 24 nov. 2014
 *      Author: yolivave
 */

#ifndef MAPPING_H_
#define MAPPING_H_

#include "types.h"

typedef enum {
	true,
	false,
}boolean;

void init_mappings();
void dynamicMapping(int nbProcessors);
u8 changeMapping(u32* prevNbProc, u32* currNbProc);

#endif /* MAPPING_H_ */
