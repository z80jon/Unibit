#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>

bool bIsMathTokenDelimiter(char c);

bool bIsWhitespaceDelimiter(char c);

bool bIsStartOfComment(char* text, int index);

int findEndOfComment(char* text, int index);

#endif /* UTILS_H_ */