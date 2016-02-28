/**
 * @brief Oxica financial interest related library
 *
 * (c) Oxica [2016]
 *
 * This code is the property of Oxica.
 */

#ifndef __FINANCE_H__
#define __FINANCE_H__

#include <sys/cdefs.h>

__BEGIN_DECLS

/**
 * @brief Calculate the estimated repayment value on a loan/financing option
 * @param principle				The principle debt amount
 * @param interest_rate			The interest rate for the loan
 * @param period				The period (in months) that the loan will be active before being settled
 * @param estimated_repayment	The estimated montly payment required
 * @return Zero for success. Less than zero for error.
 */
int finance_estimate_loan_repayment(float principle, float interest_rate, unsigned int period, float* estimated_repayment);

__END_DECLS

#endif
