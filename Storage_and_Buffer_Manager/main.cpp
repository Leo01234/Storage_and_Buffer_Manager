#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <cstdio>
#include "buf.h"
#include "DSMgr.h"
#include "BMgr.h"

#define DBF_NAME "data.dbf"
#define TRACE_FILE "data-5w-50w-zipf.txt"

using std::atoi;
using std::cout;
using std::endl;
using std::exit;
using std::filesystem::exists;

int main(int argc, char* argv[]) {

    // determine g_bufsize
    if (argc > 1) {
        g_bufsize = atoi(argv[1]);
    }
    else {
        g_bufsize = DEFBUFSIZE;
    }
    // create buffer
    g_buf = new bFrame[g_bufsize];
    // initialize after g_bufsize is determined
    g_BMgr.InitDynamicArrays();

    // check whether the database file exists
    // if not, create one with given pages and exit
    if (!exists(DBF_NAME)) {

        // create empty file
        FILE *file = fopen(DBF_NAME, "wb");
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
            NewPage new_page_info = g_BMgr.FixNewPage();
            //cout << "new page: frame_id(" << new_page_info.frame_id
            //    << "), page_id(" << new_page_info.page_id << ")"
            //    << endl;

            // here is some data insert things...

            if (g_BMgr.UnfixPage(new_page_info.page_id) < 0) {
                cout << "page not found in buffer" << endl;
                exit(EXIT_FAILURE);
            }
        }
        g_BMgr.WriteDirtys();

        if (g_DSMgr.CloseFile()) {
            cout << "close file fail" << endl;
            exit(EXIT_FAILURE);
        }

        cout << "database file materialized" << endl;
        exit(EXIT_SUCCESS);

    }

    // free space

    // free dynamic arrays
    g_BMgr.DestoryDynamicArrays();
    // free buffer
    delete [] g_buf;
}
