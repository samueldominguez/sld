/*
 * sof.c
 * Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include "sof.h"

int get_symbols(FILE *in)
{
	char line[MAX_SYMBOL_LINE_SIZE];
	char symbol[MAX_SYMBOL_SIZE];
	char value[MAX_VALUE_SIZE];
	int i, j;
	
	rewind(in);
	
	if (strcmp(SOF_SYMBOL_START, fgets(line, MAX_SYMBOL_LINE_SIZE, in)) != 0) {
		error("corrupt SOF file format");
		return 1;
	}
	while (fgets(line, MAX_SYMBOL_LINE_SIZE, in) != NULL) {
		if (strcmp(line, SOF_BINARY_START) == 0) break;
		else if (strcmp(line, SOF_END) == 0) break;
		for (i = 0; line[i] != ' '; ++i) ;
		strncpy(symbol, line, i);
		symbol[i] = '\0';
		i++;
		for (j = i; line[j] != '\n'; ++j) ;
		strncpy(value, line + i, j - i);
		add_label(symbol, (int) strtol(value, NULL, 0));
	}
	return 0;
}

int write_to_ram(FILE *in)
{
	char line[MAX_SYMBOL_LINE_SIZE];
	char c;
	fpos_t filepos;
	fpos_t tmpfilepos;
	int i;
	int length;
	int error_flag;
	int value;
	char errorstring[100];

	fgetpos(in, &filepos);
	fseek(in, -1, SEEK_END);
	c = (char) fgetc(in);
	if (c != 'd') {
		error("corrupt file format");
		return 1;
	}
	fsetpos(in, &filepos);
	fseek(in, -1, SEEK_CUR);
	c = (char) fgetc(in);
	if (c == 'd') {
		/* its a library object file, nothing to do here */
		return 0;
	} else {
		fsetpos(in, &filepos);
		while (1) {
			i = filepos.__pos;
			while ((c = (char) fgetc(in)) != ' ' && c != '\n') ;
			fseek(in, 1, SEEK_CUR);
			if (c == '\n') break;
			fseek(in, -1, SEEK_CUR);
			fgetpos(in, &filepos);
			length = (int) filepos.__pos - i;
			fgetpos(in, &tmpfilepos);
			tmpfilepos.__pos = i;
			fsetpos(in, &tmpfilepos);
			fgets(line, length, in);
			line[length] = '\0';

			if (currw + 1 == DISK_SIZE) {
				error("binary generation exceeds floppy disk size");
			}
			if (line[0] == '0') {
				ram[currw] = (u16) strtol(line, NULL, 0); 
				currw++;
			} else {
				value = get_symbol_value(line, &error_flag);
				if (error_flag) {
					sprintf(errorstring, "undefined symbol '%s'", line);
					error(errorstring);
					return 1;
				}
				ram[currw] = (u16) value;
				currw++;
			}
			fgetc(in);
			fgetpos(in, &filepos);
		}

	} 
	return 0;
}
