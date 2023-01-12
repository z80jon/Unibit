#include "parser_tests.h"
#include <munit.h>



parser_runTests() {

}

// test getNumberOfTokens function
static MunitResult
parser_getNumberOfTokens(const MunitParameter params[], void* data) {
    char* input = munit_parameters_get(params, "inputs");
    int output =  munit_parameters_get(params, "getNumberOfTokensOutput");

    return munit_assert_int(getNumberOfTokens(input), ==, output);
}


static MunitTest parser_tests[] = {
    {
        (char*) "/parser/getNumberOfTokens",
        parser_getNumberOfTokens,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    }
}


static const MunitSuite test_suite = {
  /* This string will be prepended to all test names in this suite;
   * for example, "/example/rand" will become "/µnit/example/rand".
   * Note that, while it doesn't really matter for the top-level
   * suite, NULL signal the end of an array of tests; you should use
   * an empty string ("") instead. */
  (char*) "",
  /* The first parameter is the array of test suites. */
  parser_tests,
  /* In addition to containing test cases, suites can contain other
   * test suites.  This isn't necessary in this example, but it can be
   * a great help to projects with lots of tests by making it easier
   * to spread the tests across many files.  This is where you would
   * put "other_suites" (which is commented out above). */
  NULL,
  /* An interesting feature of µnit is that it supports automatically
   * running multiple iterations of the tests.  This is usually only
   * interesting if you make use of the PRNG to randomize your tests
   * cases a bit, or if you are doing performance testing and want to
   * average multiple runs.  0 is an alias for 1. */
  1,
  /* Just like MUNIT_TEST_OPTION_NONE, you can provide
   * MUNIT_SUITE_OPTION_NONE or 0 to use the default settings. */
  MUNIT_SUITE_OPTION_NONE
};


