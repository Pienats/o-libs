/**
 * @brief Oxica command line parser
 *
 * (c) Oxica [2016]
 *
 * This code is the property of Oxica.
 */

#ifndef __OX_CMDLINE_H__
#define __OX_CMDLINE_H__

#include <sys/cdefs.h>
#include <stddef.h>
#include <getopt.h>

__BEGIN_DECLS
// Function call type
typedef void (*ox_cmdline_fx)(const char* arg);

struct ox_cmdline_t {
	const char* long_opt;		///< Long option name
	char short_opt;				///< Short option name
	int has_arg;				///< no_argument, required_argument, optional_argument
	ox_cmdline_fx fn;			///< Function to call if the option is found
	const char* desc;			///< Option description
};

/**
 * @brief Initializes the command line structure
 * @param cmdline Command line descriptor. Last structure item should set @c long_opt to @c NULL
 */
void ox_cmdline_init(const struct ox_cmdline_t* cmdline);

/**
 * @brief Parse a command line
 * @param argc Argument count as provided to main()
 * @param argv Argument vector as provided to main()
 */
void ox_cmdline_parse(int argc, char** argv);

/**
 * @brief Prints out program usage
 * @param argv0 Argument vector[0] as provided to main()
 */
void ox_cmdline_usage(const char* argv0);

/**
 * @brief Cleans up command line parsing structures
 */
void ox_cmdline_done(void);

__END_DECLS

#endif
