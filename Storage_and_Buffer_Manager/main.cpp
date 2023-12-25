#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <cstdio>
#include "buf.h"
#include "DSMgr.h"
#include "BMgr.h"
#include "iocount.h"

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

        cout << "database file do not exist, generating..." << endl;

        // create empty file
        FILE *db_file = fopen(DBF_NAME, "wb");
        if (db_file == nullptr) {
            cout << "create file fail" << endl;
            exit(EXIT_FAILURE);
        }

        if (fclose(db_file)) {
            cout << "close file fail" << endl;
            exit(EXIT_FAILURE);
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

        cout << "database file materialized, please run again to begin the lab" << endl;
        exit(EXIT_SUCCESS);

    }

    // database file should already be materialized here

    // open trace file for reading
    FILE *trace_file = fopen(TRACE_FILE, "r");
    if (trace_file == nullptr) {
        cout << "open file fail" << endl;
        exit(EXIT_FAILURE);
    }

    // deal with trace file
    int is_write;
    char s_page_num[6];
    while (fscanf(trace_file, "%1d,%5s", &is_write, s_page_num) != EOF) { // not reach the end of file
        int page_num = atoi(s_page_num);

        if (!is_write) { // read
            g_BMgr.FixPage(page_num - 1); // page_id starts at 0

            // here is some data reading things...

            if (g_BMgr.UnfixPage(page_num - 1) < 0) {
                cout << "page not found in buffer" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else { // write
            int frame_id = g_BMgr.FixPage(page_num - 1); // page_id starts at 0

            g_BMgr.SetDirty(frame_id);
            // here is some data writing things...

            if (g_BMgr.UnfixPage(page_num - 1) < 0) {
                cout << "page not found in buffer" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    // output io count
    cout << "total I/Os between memory and disk: " << g_iocount << endl;

    if (fclose(trace_file)) {
        cout << "close file fail" << endl;
        exit(EXIT_FAILURE);
    }

    // free space

    // free dynamic arrays
    g_BMgr.DestoryDynamicArrays();
    // free buffer
    delete [] g_buf;
}
