/*
 *  label.c
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "label.h"

void init_label_table()
{
	label_count = 0;
}

void add_label(char *s, int ram_address)
{
	strcpy(label_table[label_count].name, s);
	label_table[label_count].ram_address = ram_address;
	label_count++;
}

int get_symbol_value(char *s, int *error)
{
	int i;
	
	for (i = 0; i < label_count; ++i) {
		if (strcmp(label_table[i].name, s) == 0) {
			*error = 0;
			return label_table[i].ram_address;
		}
	}
	*error = 1;
	return 0;
}
