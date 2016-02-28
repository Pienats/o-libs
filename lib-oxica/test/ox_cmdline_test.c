/**
 * @brief Unit test for Oxica command line parser
 *
 * (c) Oxica [2016]
 *
 * This code is the property of Oxica.
 */

#include <stdio.h>

#include "ox_cmdline.h"

static void ox_cmdline_req(const char* arg) { printf("In ox_cmdline_req()\nArgument: %s\n\n", arg); }
static void ox_cmdline_opt(const char* arg) { printf("In ox_cmdline_opt()\nArgument: %s\n\n", arg); }
static void ox_cmdline_another_opt(const char* arg) { printf("In ox_cmdline_another_opt()\nArgument: %s\n\n", arg); }
static void ox_cmdline_no_short(const char* arg) { printf("In ox_cmdline_no_short()\n"); }

static struct ox_cmdline_t cmdline[] = {
	{ "help", 'h', no_argument, ox_cmdline_usage, "Print this message" },
	{ "required_arg", 'r', required_argument, ox_cmdline_req, "Required argument option" },
	{ "optional_arg", 'o', optional_argument, ox_cmdline_opt, "Optional argument option" },
	{ "another_optional_arg", 'a', optional_argument, ox_cmdline_another_opt, "Another optional argument option" },
	{ "no_short", 0, no_argument, ox_cmdline_no_short, "No short option" },
	{ 0, 0, 0, 0, 0},
};

int main(int argc, char** argv)
{
	ox_cmdline_init(cmdline);
	ox_cmdline_parse(argc, argv);
	ox_cmdline_done();
	return 0;
}

