#include "utils_tests.h"
#include "utils.h"
#include "datastructures.h"
#include <munit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* findEndOfComment_testParams[] = {
    "//",
    "// ",
    "/* Test */",
    "/* Foo //*/ ",
    "//*Foo*///",
    "/*\n\r*/ ",
    "//\nf",
    NULL
};

static MunitParameterEnum testParams_findEndOfComment[] = {
  { (char*) "inputs", findEndOfComment_testParams },
  { NULL, NULL },
};

static int findEndOfCommentTestOutputs[] = {
    -1,
    -1,
    -1,
    11,
    -1,
    6,
    3
};

static MunitResult test_findEndOfComment(const MunitParameter params[], void* data) {
    char* input = munit_parameters_get(params, "inputs");
    int i;
    for(i = 0; strcmp(findEndOfComment_testParams[i], input) != 0; i++);
    int output = findEndOfCommentTestOutputs[i];

    munit_assert_int(findEndOfComment(input, 0), ==, output);

    if(findEndOfComment(input, 0) != output) {
        return MUNIT_FAIL;
    }
    return MUNIT_OK;
}


MunitTest utils_tests[] = {
    {
        (char*) "/findEndOfComment",
        test_findEndOfComment,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        testParams_findEndOfComment
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};


