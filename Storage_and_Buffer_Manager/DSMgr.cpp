#include <cstdio>
#include <cerrno>
#include "DSMgr.h"

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

}
int DSMgr::WritePage(int frame_id, bFrame frm) {

}
int DSMgr::Seek(int offset, int pos) {

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
