/*
 *  sld.h
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#ifndef SLD_H
#define SLD_H

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

typedef signed short s16;
typedef unsigned short u16;
typedef uint8_t u8;

#define VERS	0.2

#define ARRAY_SIZE(a) (sizeof (a) / sizeof (*a))

#define RAM_SIZE	0xffff
#define DISK_SIZE	0x12034 /* mackpar media m35fd floppy disk size */

#endif /* SLD_H */
