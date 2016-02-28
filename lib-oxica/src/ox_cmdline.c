/**
 * @brief Oxica command line parser
 *
 * (c) Oxica [2016]
 *
 * This code is the property of Oxica.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ox_cmdline.h"

static struct ox_cmdline_t* __cmdline = NULL;
static size_t __cmdline_cnt = 0;

void ox_cmdline_init(const struct ox_cmdline_t* cmdline)
{
	const struct ox_cmdline_t* ptr = cmdline;
	__cmdline_cnt = 0;

	// Count the number of entries
	while (ptr && ptr->long_opt) {
		__cmdline_cnt++;
		ptr++;
	}

	// Allocate the internal structure
	__cmdline = malloc(sizeof(struct ox_cmdline_t) * (__cmdline_cnt + 1));
	memset(__cmdline, 0, sizeof(struct ox_cmdline_t) * (__cmdline_cnt + 1));

	for (size_t i = 0; i < (__cmdline_cnt + 1); ++i) {
		__cmdline[i].long_opt = cmdline[i].long_opt ? strdup(cmdline[i].long_opt) : NULL;
		__cmdline[i].short_opt = cmdline[i].short_opt;
		__cmdline[i].has_arg = cmdline[i].has_arg;
		__cmdline[i].fn = cmdline[i].fn;
		__cmdline[i].desc = cmdline[i].desc ? strdup(cmdline[i].desc) : NULL;
	}
}

void ox_cmdline_parse(int argc, char** argv)
{
	// Build the list of short options
	// The idea is that we can potentially have as many short options
	// as there are long options, so that is set as the minimum length.
	// Additionally each option can have a required argument (extra ':')
	// or optional argument (extra "::"), so look for these and add them
	// to the option list size

	size_t short_opt_len = __cmdline_cnt;
	for (size_t i = 0; i < __cmdline_cnt; ++i) {
		if (__cmdline[i].has_arg == required_argument)
			short_opt_len++;
		if (__cmdline[i].has_arg == optional_argument)
			short_opt_len += 2;
	}

	char short_options[(short_opt_len + 1)];
	char* short_opt_ptr = short_options;
	memset(short_options, 0, (short_opt_len + 1));

	int flag = 0;
	int long_idx = 0;
	struct option long_options[(__cmdline_cnt + 1)];
	memset(long_options, 0, (sizeof(struct option) * (__cmdline_cnt + 1)));

	for (size_t i = 0; i < __cmdline_cnt; ++i) {
		long_options[i].name = __cmdline[i].long_opt;
		long_options[i].has_arg = __cmdline[i].has_arg;
		long_options[i].flag = &flag;
		long_options[i].val = 0;

		if (__cmdline[i].short_opt) {
			*short_opt_ptr = __cmdline[i].short_opt;
			short_opt_ptr++;

			if ((__cmdline[i].has_arg == required_argument) || (__cmdline[i].has_arg == optional_argument)) {
				*short_opt_ptr = ':';
				short_opt_ptr++;
			}

			if (__cmdline[i].has_arg == optional_argument) {
				*short_opt_ptr = ':';
				short_opt_ptr++;
			}
		}
	}

	int c;
	while ((c = getopt_long(argc, argv, short_options, long_options, &long_idx)) >= 0) {
		if (c == '?') {
			// getopt_long() would have printed an error message
			// Add to that the usage output
			ox_cmdline_usage(argv[0]);
		}

		int idx = -1;

		if (c == 0) {
			// Long option was found, simply use the returned index
			idx = long_idx;
		} else {
			// Short option was found, look for the appropriate index
			for (size_t i = 0; i < __cmdline_cnt; ++i) {
				if (c == __cmdline[i].short_opt) {
					idx = i;
					break;
				}
			}
		}

		if ((idx < 0) || (idx >= __cmdline_cnt)) {
			fprintf(stderr, "\nError while parsing command line parameters\n\n");
			ox_cmdline_usage(argv[0]);
		}

		// Optional arguments have some quirks with regard to how closely they
		// are supposed to follow the option parameter:
		// * Long options require the option and the parameter to be separated
		//   by a space
		// * Short options require that there is no separation between the
		//   option and the parameter
		// To allow for a normal space separation, as is allowed for required
		// arguments, check the optind variable, and verify that it is not NULL,
		// empty, or another comment. The true argument is then argv[optind]
		const char* true_optarg = NULL;
		if (optarg)
			true_optarg = optarg;
		else {
			if (optind < argc // optind is still withing the argv range
					&& NULL != argv[optind] // not a null string
					&& '\0' != argv[optind][0] // not an empty string
					&& '-' != argv[optind][0] // not another option
				) {
				true_optarg = argv[optind];
			}
		}

		// Call the appropriate function if needed
		// Make sure we always include a valid character pointer, just in case
		// the function being called will fail with a NULL value
		if (__cmdline[idx].fn) {
			if (true_optarg)
				__cmdline[idx].fn(true_optarg);
			else
				__cmdline[idx].fn(argv[0]);
		}
	}
}

void ox_cmdline_usage(const char* argv0)
{
	if (__cmdline_cnt == 0) {
		fprintf(stderr, "USAGE: %s\n\n", argv0);
	}

	if (__cmdline[__cmdline_cnt].desc) {
		fprintf(stderr, "USAGE: %s [options] %s\n", argv0, __cmdline[__cmdline_cnt].desc);
	} else {
		fprintf(stderr, "USAGE: %s [options]\n", argv0);
	}

	for (size_t i = 0; i < __cmdline_cnt; ++i) {
		fprintf(stderr, "  %c%c  --%-20s %s %s\n",
				(__cmdline[i].short_opt != 0) ? '-' : ' ',
				(__cmdline[i].short_opt != 0) ? __cmdline[i].short_opt : ' ',
				__cmdline[i].long_opt,
				__cmdline[i].has_arg ? "<arg>" : "     ",
				__cmdline[i].desc ? __cmdline[i].desc : " ");
	}

	fprintf(stderr, "\n");

	// Terminate the program
	ox_cmdline_done();
	exit(1);
}

void ox_cmdline_done(void)
{
	if (__cmdline) {
		for (size_t i = 0; i < __cmdline_cnt; ++i) {
			if (__cmdline[i].long_opt)
				free((void*)__cmdline[i].long_opt);

			if (__cmdline[i].desc)
				free((void*)__cmdline[i].desc);
		}

		free(__cmdline);
		__cmdline = NULL;
		__cmdline_cnt = 0;
	}
}


