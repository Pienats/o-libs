/**
 * @brief Unit test for Oxica command line parser
 *
 * (c) Oxica [2016]
 *
 * This code is the property of Oxica.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h>

#include <locale.h>

#include "ox_cmdline.h"
#include "finance.h"

static bool calc_loan_repay = false;
static float principle = 0.0;
static float interest_rate = 10.5;
static unsigned int period = 0;

static void ox_cmdline_loan_repay(const char* arg) { calc_loan_repay = true; }
static void ox_cmdline_principle(const char* arg) { errno = 0; principle = strtof(arg, NULL); if (errno) { principle = 0.0; }; }
static void ox_cmdline_interest_rate(const char* arg) { errno = 0; interest_rate = strtof(arg, NULL); if (errno) { interest_rate = 0.0; }; }
static void ox_cmdline_period(const char* arg) { errno = 0; period = strtoul(arg, NULL, 10); if (errno) { period = 0; }; }

static struct ox_cmdline_t cmdline[] = {
	{ "help", 'h', no_argument, ox_cmdline_usage, "Print this message" },
	{ "repayment", 0, no_argument, ox_cmdline_loan_repay, "Calculate estimated loan repayments" },
	{ "principle", 'p', required_argument, ox_cmdline_principle, "Principle loan amount" },
	{ "interest_rate", 'r', required_argument, ox_cmdline_interest_rate, "The interest rate to use in calculations" },
	{ "period", 't', required_argument, ox_cmdline_period, "The loan period in months" },
	{ 0, 0, 0, 0, 0},
};

int main(int argc, char** argv)
{
	int r = 0;
	ox_cmdline_init(cmdline);
	ox_cmdline_parse(argc, argv);

	if (!calc_loan_repay) {
		fprintf(stderr, "Please specify an action to perform\n");
		ox_cmdline_usage(argv[0]);
	}

	setlocale(LC_NUMERIC, "");

	if (calc_loan_repay) {
		float monthly_repayment = 0.0;

		if (principle == 0.0) {
			fprintf(stderr, "No valid loan principle value specified\n");
			ox_cmdline_usage(argv[0]);
		}

		if (period == 0) {
			fprintf(stderr, "No valid loan period specified\n");
			ox_cmdline_usage(argv[0]);
		}

		if (interest_rate == 0.0) {
			printf("Interest rate used is 0.0%%\n");
			monthly_repayment = principle / period;
		} else {
			r = finance_estimate_loan_repayment(principle, interest_rate, period, &monthly_repayment);
			if (r) {
				fprintf(stderr, "Error calculating the loan repayment value; r=%d\n", r);
				goto error;
			}
		}

		printf("Estimated monthly repayment of a loan to the value of R%'.2f\nover %d months\nat %.2f%% interest per-anum\nis R%'.2f\n\n", principle, period, interest_rate, monthly_repayment);
	}

	goto exit;

error:
exit:
	ox_cmdline_done();
	return 0;
}


