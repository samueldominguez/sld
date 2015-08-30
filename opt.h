/*
 *  opt.h
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#ifndef OPT_H
#define OPT_H

struct opt_t {
	char **obj_fname; /* name of the object files */
	char *bin_fname; /* name of the binary file */
	int bin_name_spec; /* did the user specify a binary name? */
	int obj_fcount; /* how many object files? */
};

#endif /* OPT_H */
