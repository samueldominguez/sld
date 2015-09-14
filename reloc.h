/*
 * Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#ifndef RELOC_H
#define RELOC_H

#include "output.h"

#define MAGIC_NUMBER		0x0127
#define VERSION_NUMBER		0x9819
#define MAX_RELOC_ENTRIES	0x7ffe /* dcpu ram divided by two minus 4 */

struct reloc_table {
	u16 magic_number;
	u16 version_number;
	u16 entry_point;
	u16 table_size;
	u16 i;
	u16 entries[MAX_RELOC_ENTRIES];
};

struct reloc_table reloc;

void init_reloc();
void add_reloc(u16 entry);

#endif /* RELOC_H */
