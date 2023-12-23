//#include <iostream>

#include <cstdlib>
#include "buf.h"

//using std::cout;

using std::atoi;

int main(int argc, char* argv[]) {

    // create buffer
    if (argc > 1) {
        g_bufsize = atoi(argv[1]);
    }
    else {
        g_bufsize = DEFBUFSIZE;
    }
    g_buf = new bFrame[g_bufsize];

    //cout << g_bufsize;

    delete [] g_buf;
}
