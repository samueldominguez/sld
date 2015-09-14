/*
 * Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "reloc.h"

extern void sld_error(char *s);

void init_reloc()
{
	reloc.magic_number = MAGIC_NUMBER;
	reloc.version_number = VERSION_NUMBER;
	reloc.entry_point = 0x0000;
	reloc.table_size = 4;
	reloc.i = 0;
}

void add_reloc(u16 entry)
{
	if (reloc.table_size == MAX_RELOC_ENTRIES) {
		sld_error("relocation table cannot grow bigger");
		return;
	}
	reloc.table_size++;
	reloc.entries[reloc.i] = entry;
	reloc.i++;
}
