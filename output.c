/*
 * Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "output.h"

void write_binary(FILE *out)
{
	fwrite(ram, 2, currw, out);
}
