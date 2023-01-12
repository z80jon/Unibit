#ifndef PARSER_TESTS_H_
#define PARSER_TESTS_H_

#include <munit.h>


extern int getNumberOfTokens(char* text);

const char** parserInputStrings = {
    "1",
    "1+",
    "1+2",
    "1 + 2",
    "1+3*2",
    "1*3*(12+2)",
    "parser[foo]"
};
const int* getNumberOfTokensOutputs = {
    1,
    2,
    3,
    3,
    5,
    9,
    4
};


static MunitParameterEnum test_params[] = {
  { (char*) "inputs", parserInputStrings },
  { (char*) "getNumberOfTokensOutput", getNumberOfTokensOutputs },
  { NULL, NULL },
};

#endif /* PARSER_TESTS_H_ */
