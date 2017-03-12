#include "OptParser.h"

/**
 * [CmdLineOptParser::Parse description]
 * @param  argc number of command line arguments, i.e. len(argv)
 * @param  argv array of command line arguments
 * @return      returns true, if all arguments are successfully parsed
 */
bool CmdLineOptParser::Parse(int argc, char* argv[]) {
  if (1 > argc
      || !argv) {
    return false;
  }
  /* check if any string is empty */
  for (int argn = 0; argn < argc; argn++) {
    if (!argv[argn]) {
      DEBUG("null string in argv[" << argn << "]");
      return false;
    }
    DEBUG("argv["<< argn << "]=" << argv[argn] << " (" << std::strlen(argv[argn]) << ")");
  }
  /* parse all the argument */
  /*
    possible types:
      (1) -x
      (2) -xvalue
      (3) -x=value
      (4) -x value
   */
  for (int argn = 1; argn < argc; argn++) {
    if (!('-' == argv[argn][0])
        || 2 > std::strlen(argv[argn])) {
      DEBUG("bad arg in argv[" << argn << "]");
      return false;
    }
    char option_c = 0;
    char const * option_info = NULL;
    if (2 == std::strlen(argv[argn])) { /* type (1) or (4) */
      if (argc-1 == argn /* last arg */
          || '-' == argv[argn+1][0]) /* next no value */ { /* type (1) */
          option_c = argv[argn][1];
          option_info = "";
      } else { /* type (4) */
        option_c = argv[argn][1];
        option_info = argv[argn+1];
        /* skip one arg b/c its taken as value */
        argn++;
      }
    } else { /* type (2) or (3), strlen > 2 */
      if ('=' == argv[argn][2]) { /* type (3) */
        if(!( 3 < std::strlen(argv[argn]))) { /* only valid with -x=value */
          DEBUG("arg to short with '=' in argv[" << argn << "]");
          return false;
        }
        option_c = argv[argn][1];
        option_info = &(argv[argn][3]);
      } else { /* type (2) */
        option_c = argv[argn][1];
        option_info = &(argv[argn][2]);
      }
    }
    if (!CmdLineOptParser::Option(option_c, option_info)) {
      DEBUG("not a valid option in argv[" << argn << "] : '" << option_c << "' '" << option_info << "'");
      return false;
    }
    DEBUG("found option: '" << option_c << "' '" << option_info << "'");
  }

  return true;
}

/**
 * Can be overwritten to turn option into saved flag
 * @param  c    the character of the option flag
 * @param  info the string value behind it
 * @return      returns true, if option was successfully parsed
 */
bool CmdLineOptParser::Option(const char c, const char* info) {
  if (!c
      || !info) {
    return false;
  }
  return true;
}
