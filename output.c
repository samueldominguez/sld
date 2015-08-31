/*
 * Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "output.h"

void write_binary(FILE *out)
{
	/* fwrite follows the system's endian, little endian.
	 * we reverse the words so the final result will be a
	 * big endian binary file */
	reverse_words();
	fwrite(ram, 2, currw, out);
}

void reverse_words()
{
	int i;
	for (i = 0; i < currw; ++i) ram[i] = htons(ram[i]);
}
