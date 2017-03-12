#ifndef OPTPARSER_H
#define OPTPARSER_H

#include <cstring>

#include "debug.h"

class CmdLineOptParser
{
  public:
    // returns true, if all arguments are successfully parsed.
    bool Parse(int argc, char* argv[]);
  protected:
    // returns true, if option was successfully parsed.
    virtual bool Option(const char c, const char* info);
};

#endif
