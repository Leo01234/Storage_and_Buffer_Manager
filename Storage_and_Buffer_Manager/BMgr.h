#pragma once

#include "BCB.h"
#include "buf.h"

struct NewPage
{
    int page_id;
    int frame_id;
    NewPage(int page_id, int frame_id) : page_id(page_id), frame_id(frame_id) {}
};

class BMgr
{
public:
    BMgr();
    ~BMgr();
    // Interface functions
    int FixPage(int page_id, int prot);
    NewPage FixNewPage();
    int UnfixPage(int page_id);
    int NumFreeFrames();
    // Internal Functions
    int SelectVictim();
    int Hash(int page_id);
    void RemoveBCB(BCB *ptr, int page_id);
    void RemoveLRUEle(int frid);
    void SetDirty(int frame_id);
    void UnsetDirty(int frame_id);
    void WriteDirtys();
    void PrintFrame(int frame_id);
private:
    // Hash Table
    // use dynamic array to fit to the buffer size that the user defined by the input parameter
    int *ftop = new int[g_bufsize];
    BCB **ptof = new BCB *[g_bufsize];
};
