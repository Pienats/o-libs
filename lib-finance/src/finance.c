/**
 * @brief Oxica financial interest related library
 *
 * (c) Oxica [2016]
 *
 * This code is the property of Oxica.
 */

#include <stdio.h>
#include <math.h>

#include "finance.h"

int finance_estimate_loan_repayment(float principle, float interest_rate, unsigned int period, float* estimated_repayment)
{
	if (principle < 0) {
		fprintf(stderr, "The principle debt must be a positive value");
		return -1;
	}

	if (interest_rate < 0) {
		fprintf(stderr, "The interest rate must be a positive value");
		return -2;
	}

	if (period < 0) {
		fprintf(stderr, "The loan period must be a positive value");
		return -3;
	}

	float _int_rate = interest_rate/1200;
	*estimated_repayment = ((_int_rate + (_int_rate/(pow((1+_int_rate),period)-1))) * principle);

	return 0;
}
