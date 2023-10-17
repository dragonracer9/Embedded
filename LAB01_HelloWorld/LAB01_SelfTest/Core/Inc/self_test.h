/*
 * @file self_test.h
 * @brief PBLShield Selftest.
 * @author Philipp Schilk 2023
 */
#ifndef SELF_TEST_H_
#define SELF_TEST_H_

#include <stdbool.h>
#include "main.h"

void run_self_test();

struct test {
	char* name;
	bool (*run_test)();
};

#endif /* SELF_TEST_H_ */
