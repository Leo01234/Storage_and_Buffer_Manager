#pragma once

#include <string>
#include "bFrame.h"

using std::string;

#define MAXPAGES 50000

class DSMgr
{
public:
    DSMgr();
    int OpenFile(string filename);
    int CloseFile();
    bFrame ReadPage(int page_id);
    // I think the first argument here should be page_id instead of frame_id
    //int WritePage(int frame_id, bFrame frm);
    int WritePage(int page_id, bFrame frm);
    int Seek(int offset, int pos);
    FILE *GetFile();
    void IncNumPages();
    int GetNumPages();
    void SetUse(int index, int use_bit);
    int GetUse(int index);
private:
    FILE *currFile = nullptr;
    int numPages = 0;
    int pages[MAXPAGES] = {0};
};

extern DSMgr g_DSMgr;
