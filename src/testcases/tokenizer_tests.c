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
    "jumpIfZero"
}

static enum eProgTokenType getTokenTypeOutputs[] = {

}