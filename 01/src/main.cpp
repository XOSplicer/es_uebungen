/**
 * @author Felix Stegmaier
 */

#include "OptParser.h"

int main(int argc, char* argv[]) {
    CmdLineOptParser cmd;
    return cmd.Parse(argc,argv) ? 0 : 1;
}
