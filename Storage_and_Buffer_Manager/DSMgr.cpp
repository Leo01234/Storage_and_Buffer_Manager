#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include "DSMgr.h"
#include "iocount.h"

using std::cout;
using std::endl;
using std::exit;

DSMgr::DSMgr() = default;
int DSMgr::OpenFile(string filename) {
    currFile = fopen(filename.c_str(), "br+");
    if (currFile == nullptr) {
        // on error
        return errno;
    }
    else {
        // success
        return 0;
    }
}
int DSMgr::CloseFile() {
    return fclose(currFile);
}
bFrame DSMgr::ReadPage(int page_id) {
    // io count
    g_iocount++;

    if (Seek(FRAMESIZE * page_id, SEEK_SET)) {
        cout << "seek fail" << endl;
        exit(EXIT_FAILURE);
    }
    bFrame frm;
    if (fread(frm.field, sizeof(Char), FRAMESIZE, currFile) < FRAMESIZE) {
        cout << "read fail" << endl;
        exit(EXIT_FAILURE);
    }
    return frm;
}
int DSMgr::WritePage(int page_id, bFrame frm) {
    // io count
    g_iocount++;

    if (Seek(FRAMESIZE * page_id, SEEK_SET)) {
        cout << "seek fail" << endl;
        exit(EXIT_FAILURE);
    }
    return fwrite(frm.field, sizeof(Char), FRAMESIZE, currFile);
}
int DSMgr::Seek(int offset, int pos) {
    return fseek(currFile, offset, pos);
}
FILE* DSMgr::GetFile() {
    return currFile;
}
void DSMgr::IncNumPages() {
    numPages++;
}
int DSMgr::GetNumPages() {
    return numPages;
}
void DSMgr::SetUse(int page_id, int use_bit) {
    pages[page_id] = use_bit;
}
int DSMgr::GetUse(int page_id) {
    return pages[page_id];
}

DSMgr g_DSMgr;
