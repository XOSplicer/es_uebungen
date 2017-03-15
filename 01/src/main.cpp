/**
 * @author Felix Stegmaier
 */

#include "OptParser.h"

int main(int argc, char* argv[]) {
    int ret = 0;
    CmdLineOptParser cmd;
    ret |= cmd.Parse(argc, argv) ? 0 : 1; /* with bin */
    ret |= cmd.Parse(argc-1,argv+1) ? 0 : 1; /* without bin */
    return ret;
}
