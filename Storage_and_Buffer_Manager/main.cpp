//#include <iostream>

#include <cstdlib>
#include "buf.h"

//using std::cout;

using std::atoi;

int main(int argc, char* argv[]) {

    // create buf
    if (argc > 1) {
        bufsize = atoi(argv[1]);
    }
    else {
        bufsize = DEFBUFSIZE;
    }
    buf = new bFrame[bufsize];

    //cout << bufsize;

    delete [] buf;
}
