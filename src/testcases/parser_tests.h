#ifndef PARSER_TESTS_H_
#define PARSER_TESTS_H_

#include <munit.h>


extern int getNumberOfTokens(char* text);

const char** inputStrings = {
    "1",
    "1+",
    "1+2",
    "1 + 2",
    "parser[foo]",
    "1+3*2",
    "1*3*(12+2)"
};
const int* getNumberOfTokensOutputs = {
    1,
    2,
    3,
    3,
    4,
    5,
    9
};


static MunitParameterEnum test_params[] = {
  { (char*) "inputs", inputStrings },
  { (char*) "getNumberOfTokensOutput", getNumberOfTokensOutputs },
  { NULL, NULL },
};

#endif /* PARSER_TESTS_H_ */
