/**
 * @author Felix Stegmaier
 */

#ifndef PRINTF_H
#define PRINTF_H

#include <cstdarg>

#include "debug.h"

char* Printf(char* dst, const void* end, const char* fmt, ...);

#endif
