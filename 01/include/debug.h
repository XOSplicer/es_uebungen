/**
 * @author Felix Stegmaier
 */

//#define ENABLE_DEBUG

#ifndef DEBUG_H
#define DEBUG_H

#ifdef ENABLE_DEBUG
#include <iostream>
#define DEBUG(x) std::cerr << "DEBUG: " << x << std::endl
#else
#define DEBUG(x)
#endif

#endif
