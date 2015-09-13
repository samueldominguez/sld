/*
 * Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "output.h"

extern struct opt_t opts;

void write_binary(FILE *out)
{
	int i;
	u16 reloc_header[] = {reloc.magic_number, reloc.version_number,
	reloc.entry_point, reloc.table_size};
	/* fwrite follows the system's endian, little endian.
	 * we reverse the words so the final result will be a
	 * big endian binary file */
	if (opts.reloc_tables) {
		for (i = 0; i < reloc.i; ++i) {
			ram[reloc.entries[i]] += reloc.table_size;
			reloc.entries[i] += reloc.table_size;
		}
		reloc_header[2] += reloc.table_size;
		
		/* first write the relocation tables header */
		reverse_words(reloc_header, ARRAY_SIZE(reloc_header));
		fwrite(reloc_header, 2, ARRAY_SIZE(reloc_header), out);
		/* now write the relocation tables entries */
		reverse_words(reloc.entries, reloc.i);
		fwrite(reloc.entries, 2, reloc.i, out);
	}
	reverse_words(ram, currw);
	fwrite(ram, 2, currw, out);
}

void reverse_words(u16 *arr, int size)
{
	int i;
	for (i = 0; i < size; ++i) arr[i] = htons(arr[i]);
}
