/*
 *  sld.c
 *  Copyright (C) 2015 Samuel Dominguez Lorenzo
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "opt.h"
#include "sld.h"
#include "output.h"
#include "label.h"
#include "sof.h"

struct opt_t opts; /* this is where all the specified options are stored */
char defout[] = "a.bin"; /* default object file name for output */
int sld_error = 0; /* if there is an error parsing the file, this is 1 */
int lines = 0; /* global line count */

static void display_usage(void)
{
	fprintf(stderr,
	"sld - simple dcpu-16 linker for SOF object files; version %.2f\n", VERS);
	fprintf(stderr, "usage: sld [options] [objfiles]\n");
	fprintf(stderr, "options:\n");
	fprintf(stderr,
	"  -o outfile		write binary to outfile, default name is a.bin\n");
	fprintf(stderr,
	"  --org <address>	binary expects to be loaded at <address>\n");
	fprintf(stderr,
	"  --relocation		generates relocation tables\n");
	fprintf(stderr,
	"  --version		display version number and exit\n");
	fprintf(stderr,
	"  -h, --help		display this help and exit\n");
}

static void display_version(void)
{
	fprintf(stderr, "sld version: %.2f\n", VERS);
}

static void suggest_help(void)
{
	fprintf(stderr, "sld: type `sld -h' for help\n");
}

void error(char *s)
{
	fprintf(stderr, "sld: error: %s\n", s);
}

static void opts_free(void)
{
	opts.obj_fcount--;
	for (; opts.obj_fcount >= 0; opts.obj_fcount--) {
		if (opts.obj_fname[opts.obj_fcount] != NULL) {
			free(opts.obj_fname[opts.obj_fcount]);
			opts.obj_fname[opts.obj_fcount] = NULL;
		}
	}
	free(opts.obj_fname);
}

static void close_files(FILE **objfiles, int n)
{
	for (--n; n >= 0; --n) {
		if (objfiles[n]) fclose(objfiles[n]);
	}
}

static void handle_args(int argc, char **argv)
{
	char opt = 0;
	int longind = 0;
	int outcount = 0;
	char **hdata = NULL;
	char *obj_file = NULL;
	static const char *optstring = "ho:";
	static const struct option longopts[] = {
		{"help", no_argument, NULL, 0},
		{"version", no_argument, NULL, 0},
		{"org", required_argument, NULL, 0},
		{"relocation", no_argument, NULL, 0},
	};

	opts.obj_fname = NULL;
	opts.bin_fname = NULL;
	opts.bin_name_spec = 0;
	opts.obj_fcount = 0;
	opts.org = 0;
	opts.reloc_tables = 0;

	opt = getopt_long(argc, argv, optstring, longopts, &longind);

	while (opt != -1) {
		switch (opt) {
		case 'o':
			if (outcount == 0) {
				opts.bin_name_spec = 1;
				opts.bin_fname = optarg;
				outcount++;
			} else {
				error("output file name already defined");
				exit(EXIT_FAILURE);
			}
			break;
		case 'h':
			display_usage();
			exit(EXIT_FAILURE);
			break;
		case '?':
			suggest_help();
			exit(EXIT_FAILURE);
		case 0:
			if (strcmp("help", longopts[longind].name) == 0) {
				display_usage();
				exit(EXIT_FAILURE);
			} else if (strcmp("version",
					longopts[longind].name) == 0) {
				display_version();
				exit(EXIT_FAILURE);
			} else if (strcmp("org",
					longopts[longind].name) == 0) {
				opts.org = strtol(optarg, NULL, 0);
			} else if (strcmp("relocation",
					longopts[longind].name) == 0) {
				opts.reloc_tables = 1;
			}
			break;
		}
		opt = getopt_long(argc, argv, optstring, longopts, &longind);
	}

	if (optind == argc) {
		error("no input files specified");
		suggest_help();
		exit(EXIT_FAILURE);
	} else {
		while (optind < argc) {
			hdata = realloc(opts.obj_fname, sizeof (char*) * (opts.obj_fcount + 1));
			if (hdata == NULL) {
				error("memory error.");
				opts_free();
				exit(EXIT_FAILURE);
			}
			opts.obj_fname = hdata;
			opts.obj_fname[opts.obj_fcount] = malloc(sizeof (char) * strlen(argv[optind]) + 1);
			if (opts.obj_fname[opts.obj_fcount] == NULL) {
				error("memory error.");
				opts_free();
				exit(EXIT_FAILURE);
			}
			strcpy(opts.obj_fname[opts.obj_fcount], argv[optind]);
			opts.obj_fcount++;
			optind++;
		}
	}
	if (opts.bin_name_spec == 0) {
		opts.bin_fname = defout;
	}
}
 
int main(int argc, char **argv)
{
	char errstr[BUFSIZ];
	char line[BUFSIZ];
	FILE* objfiles[BUFSIZ];
	FILE *binfile = NULL;
	int i;

	currw = 0x0000;
	start_write_address = 0x0000;

	handle_args(argc, argv);
	memset(ram, 0x0000, RAM_SIZE);

	for (i = 0; i < opts.obj_fcount; ++i) {
		printf("obj file[%d]: '%s'\n", i + 1, opts.obj_fname[i]);
		objfiles[i] = fopen(opts.obj_fname[i], "r");
		if (objfiles[i] == NULL) {
			sprintf(errstr, "couldn't open '%s'", opts.obj_fname[i]);
			error(errstr);
			error("perhaps it doesn't exist?");
			opts.obj_fcount = i;
			goto exit;
		}
	}
	if (opts.bin_name_spec) printf("bin file: '%s'\n", opts.bin_fname);
	else printf("bin file: '%s'\n", defout);

	init_label_table();
	init_reloc();

	lines = 0;
	for (i = 0; i < opts.obj_fcount; ++i) {
		get_symbols(objfiles[i]);
	}
	for (i = 0; i < opts.obj_fcount; ++i) {
		write_to_ram(objfiles[i]);
	}

	binfile = fopen(opts.bin_fname, "wb");
	if (binfile == NULL) {
		sprintf(errstr, "couldn't open '%s' for output", opts.bin_fname);
		error(errstr);
	} else {
		write_binary(binfile);
		fclose(binfile);
	}
	exit:
	close_files(objfiles, opts.obj_fcount);
	opts_free();
	exit(EXIT_SUCCESS);
}
