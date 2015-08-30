/*
 *  output.h
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#ifndef OUTPUT_H
#define OUTPUT_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sld.h"

#define MAX_SYMBOL_LINE_SIZE		73
#define MAX_SYMBOL_SIZE			64
#define MAX_VALUE_SIZE			 7

u16 ram[DISK_SIZE];
u16 currw;
u16 start_write_address;

/* expects out file to be an open stream, writes in big endian from ram */
void write_binary(FILE *out);

#endif /* OUTPUT_H */