#include "parser_tests.h"
#include "parser.h"
#include <munit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//===== PARSER TESTS =====//
extern int getNumberOfTokens(char* text);
extern int createArrayofTokens(char* text);
extern char** tokens;
//extern int getValueOfToken(char* text, uint16_t* returnValue);

#define NUM_PARSER_INPUT_STRINGS 17
static char* parserInputStrings[NUM_PARSER_INPUT_STRINGS + 1] = {
    "1",
    "1+",
    "1+2 ",
    "1 + 2 ",
    "1+3*2",
    "1*3*(12+2)",
    "parser[foo]",
    "1[2]",         
    "(2)",
    "2*((7+3)*2)+1", //41
    "92+(7 *(3 ) )", //113
    "172[12*2]",     //196
    "+1",
    "-2",
    "(f",
    "[f",
    "0x7cF",
    NULL
};

static char* createArrayofTokensOutputs[NUM_PARSER_INPUT_STRINGS + 1][25] = {
    {"1", NULL},
    {"1","+"},
    {"1","+","2"},
    {"1","+","2"},
    {"1","+","3","*","2"},
    {"1","*","3","*","(","12","+","2",")"},
    {"parser","[","foo","]"},
    {"1","[","2","]"},
    {"(","2",")"},                      //2
    {"2","*","(","(","7","+","3",")","*","2",")","+","1"}, //41
    {"92","+","(","7","*","(","3",")",")"},   //113
    {"172","[","12","*","2","]"},       //196
    {"+","1"},
    {"-","2"},
    {"(","f"},
    {"[","f"},
    {"0x7cF"},
    {NULL}
};

static int getNumberOfTokensOutputs[] = {
    1,
    2,
    3,
    3,
    5,
    9,
    4,
    4,
    3,
    13,
    9,
    6,
    2,
    2,
    2,
    2,
    1
};


static int parserOutputs[] = {
    1,
    9999,
    3,
    3,
    7,
    42,
    9999,
    3,
    2,
    41,
    113,
    196,
    9999,
    9999,
    9999,
    9999,
    1999
};

static MunitParameterEnum testParams__parser[] = {
  { (char*) "inputs", parserInputStrings },
  { NULL, NULL },
};


static MunitResult parser_getNumberOfTokens(const MunitParameter params[], void* data) {
    char* input = munit_parameters_get(params, "inputs");
    int i;
    for(i = 0; strcmp(parserInputStrings[i], input) != 0; i++);
    int output = getNumberOfTokensOutputs[i];

    munit_assert_int(getNumberOfTokens(input), ==, output);

    if(getNumberOfTokens(input) != output) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}
static MunitResult parser_createArrayOfTokens(const MunitParameter params[], void* data) {
    char* input = munit_parameters_get(params, "inputs");
    int i;
    for(i = 0; strcmp(parserInputStrings[i], input) != 0; i++);
    int tokensExpected = getNumberOfTokensOutputs[i];
    MunitResult passFail = MUNIT_OK;

    int tokensActual = createArrayofTokens(input);

    for(int j=0; j<tokensExpected && j<tokensActual; j++) {
        if(strcmp(tokens[j], createArrayofTokensOutputs[i][j]) != 0) {
            munit_assert_string_equal(tokens[j],createArrayofTokensOutputs[i][j]);
            passFail = MUNIT_FAIL;
            break;
        }
    }

    //Free up memory
    for(int j = 0; j < tokensActual; j++) {
        free(tokens[j]);
    }

    return passFail;
    
}
static MunitResult parser_parser_begin(const MunitParameter params[], void* data) {
    char* input = munit_parameters_get(params, "inputs");
    int i;
    for(i = 0; strcmp(parserInputStrings[i], input) != 0; i++);
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


MunitTest parser_tests[] = {
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


