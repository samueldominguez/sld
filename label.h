/*
 *  label.h
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#ifndef LABEL_H
#define LABEL_H

#include <string.h>

/* this max value was taken from the C standard, interesting fact, or not? */
#define LABEL_MAX_LENGTH	63
/* a rough estimate, if you ever run out of labels just modify this and
 * recompile */
#define LABEL_MAX_NUMBER	3000

struct label {
	char name[LABEL_MAX_LENGTH];
	int ram_address;
};

struct label label_table[LABEL_MAX_NUMBER];
int label_count;

void init_label_table();
void add_label(char *s, int ram_address);

/* returns the value on success and error sets to 0
 * failure is noted by returning 0 AND error sets to 1 */
int get_symbol_value(char *s, int *error);

#endif /* LABEL_H */
