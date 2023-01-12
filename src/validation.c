#include "validation.h"
#include "parser.h"
#include <munit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//===== PARSER TESTS =====//
extern int getNumberOfTokens(char* text);
extern int createArrayofTokens(char* text, char*** array);
//extern int getValueOfToken(char* text, uint16_t* returnValue);

#define NUM_PARSER_INPUT_STRINGS 7
static char* parserInputStrings[NUM_PARSER_INPUT_STRINGS + 1] = {
    "1",
    "1+",
    "1+2 ",
    "1 + 2",
    "1+3*2",
    "1*3*(12+2)",
    "parser[foo]",
    NULL
};
// static char* getValueOfTokenInputStrings[] = {
//     "1",
//     "2",
//     "0x70",
//     "3",
//     "0x01",
//     "0x77"
//     NULL
// };
static char* createArrayofTokensOutputs[NUM_PARSER_INPUT_STRINGS + 1][9] = {
    {"1", NULL},
    {"1","+"},
    {"1","+","2"},
    {"1","+","2"},
    {"1","+","3","*","2"},
    {"1","*","3","*","(","12","+","2",")"},
    {"parser","[","foo","]"},
    {NULL}
};
static int getNumberOfTokensOutputs[] = {
    1,
    2,
    3,
    3,
    5,
    9,
    4
};
static int parserOutputs[] = {
    1,
    9999,
    3,
    3,
    7,
    42,
    9999,
};

static MunitParameterEnum testParams__parser[] = {
  { (char*) "inputs", parserInputStrings },
  { NULL, NULL },
};

static MunitResult parser_getNumberOfTokens(const MunitParameter params[], void* data) {
    char* input = munit_parameters_get(params, "inputs");
    int i;
    for(i = 0; i < NUM_PARSER_INPUT_STRINGS; i++)
        if(strcmp(parserInputStrings[i], input) == 0)
            break;
    int output = getNumberOfTokensOutputs[i];//atoi(munit_parameters_get(params, "getNumberOfTokensOutput"));

    munit_assert_int(getNumberOfTokens(input), ==, output);

    if(getNumberOfTokens(input) != output) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}
static MunitResult parser_createArrayOfTokens(const MunitParameter params[], void* data) {
    char* input = munit_parameters_get(params, "inputs");
    int i;
    for(i = 0; i < NUM_PARSER_INPUT_STRINGS; i++)
        if(strcmp(parserInputStrings[i], input) == 0)
            break;
    char** solnArray;
    int tokensExpected = getNumberOfTokensOutputs[i];
    MunitResult passFail = MUNIT_OK;

    int tokensActual = createArrayofTokens(input, &solnArray);

    for(int j=0; j<tokensExpected && j<tokensActual; j++) {
        if(strcmp(solnArray[j], createArrayofTokensOutputs[i][j]) != 0) {
            munit_assert_string_equal(solnArray[j],createArrayofTokensOutputs[i][j]);
            passFail = MUNIT_FAIL;
            break;
        }
    }

    //Free up memory
    for(int j = 0; j < tokensActual; j++) {
        free(solnArray[j]);
    }

    return passFail;
    
}
static MunitResult parser_parser_begin(const MunitParameter params[], void* data) {
    char* input = munit_parameters_get(params, "inputs");
    int i;
    for(i = 0; i < NUM_PARSER_INPUT_STRINGS; i++)
        if(strcmp(parserInputStrings[i], input) == 0)
            break;
    int expectedOutput = parserOutputs[i];//atoi(munit_parameters_get(params, "getNumberOfTokensOutput"));
    uint16_t solution;

    int retVal = parser_begin(input, &solution);

    //Edge case: Invalid input, error expected
    if(expectedOutput == 9999 && solution == 0 && retVal != 0) {
        munit_assert_int(0,==,0);
        return MUNIT_OK;
    }

    munit_assert_int(solution, ==, expectedOutput);
    if(expectedOutput == solution)
        return MUNIT_OK;
    return MUNIT_FAIL;

    
}
static MunitTest parser_tests[] = {
    {
        (char*) "/getNumberOfTokens",
        parser_getNumberOfTokens,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        testParams__parser
    },
    {
        (char*) "/createArrayOfTokens",
        parser_createArrayOfTokens,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        testParams__parser
    },
    {
        (char*) "/parser_begin",
        parser_parser_begin,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        testParams__parser
    }
};


//===== SOME OTHER STUFF =====//

static const MunitSuite testSuite__parser = {
  /* This string will be prepended to all test names in this suite;
   * for example, "/example/rand" will become "/µnit/example/rand".
   * Note that, while it doesn't really matter for the top-level
   * suite, NULL signal the end of an array of tests; you should use
   * an empty string ("") instead. */
  (char*) "/parser",
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


int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
    
    return munit_suite_main(&testSuite__parser, (void*) "µnit", argc, argv);
}