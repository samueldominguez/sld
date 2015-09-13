/*
 * sof.h
 * Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#ifndef SOF_H
#define SOF_H

#include "output.h"
#include "label.h"
#include "opt.h"

extern void error(char *s);

#define SOF_SYMBOL_START        "symbols:\n"
#define SOF_BINARY_START        "binary:\n"
#define SOF_END                 "end"
#define SOF_ENTRY_POINT		"__main"

/* expects in to be an open stream in READ mode 
 * returns 0 on success, 1 on failure */
int get_symbols(FILE *in);

/* expects in to be an open stream in READ mode
 * returns 0 on success, 1 on failure */
int write_to_ram(FILE *in);

#endif /* SOF_H */
