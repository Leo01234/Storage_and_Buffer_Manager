#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <cstdio>
#include "buf.h"
#include "DSMgr.h"
#include "BMgr.h"

#define DBF_NAME "data.dbf"

using std::atoi;
using std::cout;
using std::endl;
using std::exit;
using std::filesystem::exists;

int main(int argc, char* argv[]) {

    // create buffer
    if (argc > 1) {
        g_bufsize = atoi(argv[1]);
    }
    else {
        g_bufsize = DEFBUFSIZE;
    }
    g_buf = new bFrame[g_bufsize];

    // check whether the database file exists
    // if not, create one with given pages and exit
    if (!exists(DBF_NAME)) {

        // create empty file
        FILE *file = fopen(DBF_NAME, "w");
        if (file == nullptr) {
            cout << "create file fail" << endl;
            exit(EXIT_FAILURE);
        }
        else {
            if (fclose(file)) {
                cout << "close file fail" << endl;
                exit(EXIT_FAILURE);
            }
        }
        
        // materialize the file
        if (g_DSMgr.OpenFile(DBF_NAME)) {
            cout << "open file fail" << endl;
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < MAXPAGES; i++) {
            g_BMgr.FixNewPage();
        }
        g_BMgr.WriteDirtys();

        if (g_DSMgr.CloseFile()) {
            cout << "close file fail" << endl;
            exit(EXIT_FAILURE);
        }

        cout << "database file materialized" << endl;
        exit(EXIT_SUCCESS);

    }

    // free buffer
    delete [] g_buf;
}
