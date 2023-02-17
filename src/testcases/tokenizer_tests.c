#include "tokenizer_tests.h"
#include "tokenizer.h"
#include "datastructures.h"
#include <munit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static char* getTokenTypeInputStrings[] = {
    "label:",
    "l1bel:",
    "labe:l",
    "labe l:",
    "Negate:",
    "n",
    "NEGATe",
    "inVERT",
    "S",
    "St",
    "Store",
    "j",
    "jIz",
    "jumpIfZero",
    NULL
};

// static enum eProgTokenType getTokenTypeOutputs[] = {

// };


static char* getNextTokenInputs[] = {
    "load 1;",
    "load 1",
    "\nstore 7;",
    "\n jumpifzero 4;",
    "\n/*Yes\n*/negate;",

};

static int getNextToken_output_lineNo[] = {
    1,
    1,
    2,
    2,
    3
};

static char* getNextToken_output_text[] = {
    "load 1",
    "load 1", //TBD error handling method
    "store 7",
    "jumpifzero 4",
    "negate"
};

static MunitParameterEnum testParams__getNextToken[] = {
  { (char*) "inputs", getNextTokenInputs },
  { NULL, NULL },
};


static MunitResult test_getNextToken(const MunitParameter params[], void* data) {
    char* input = munit_parameters_get(params, "inputs");
    int i;
    for(i = 0; strcmp(getNextTokenInputs[i], input) != 0; i++);
    int expectedLineNo = getNextToken_output_lineNo[i];
    char* outputText = getNextToken_output_text[i];
    int actualLineNo;

    munit_assert_string_equal(getNextToken(input, &actualLineNo), outputText);
    munit_assert_int(actualLineNo, ==, expectedLineNo);

    if(strcmp(getNextToken(input, &actualLineNo), outputText) != 0 ||
                    actualLineNo != expectedLineNo) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}

MunitTest tokenizer_tests[] = {
    {
        (char*) "/getNextToken",
        test_getNextToken,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        testParams__getNextToken
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};



