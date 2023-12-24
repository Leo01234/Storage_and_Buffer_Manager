#pragma once

#include "BCB.h"
#include "buf.h"

struct NewPage
{
    int page_id;
    int frame_id;
    NewPage(int page_id, int frame_id):
        page_id(page_id), frame_id(frame_id) {}
};

class BMgr
{
public:
    BMgr();
    ~BMgr();

    // Interface functions

    // there is no page_latch in this lab,
    // so the parameter "prot" is of no use here
    //int FixPage(int page_id, int prot);
    int FixPage(int page_id);
    NewPage FixNewPage();
    int UnfixPage(int page_id);
    int NumFreeFrames();

    // Internal Functions

    // the function is called only when the buffer is full
    int SelectVictim();
    int Hash(int page_id);
    void RemoveBCB(BCB *ptr, int page_id);
    // the function is called when
    // a dirty frame needs to be written back to disk
    void RemoveLRUEle(int frid);
    void SetDirty(int frame_id);
    void UnsetDirty(int frame_id);
    void WriteDirtys();
    void PrintFrame(int frame_id);

    // operation on LRU linked list

    // only reassign pointers, do not delete BCB
    // should only be called when there is more than one BCB in LRU list
    void RemoveFromLRUList(BCB *ptr);
    void AddToMRU(BCB *ptr);

private:
    // Hash Table
    // use dynamic array to fit to the buffer size that the user defined by the input parameter
    int *ftop = new int[g_bufsize]; // do not need to initialize to 0
    BCB **ptof = new BCB *[g_bufsize](); // initialize to nullptr

    // LRU linked list
    BCB *MRU = nullptr;
    BCB *LRU = nullptr;

    int min_free_frame_id = 0;
};

extern BMgr g_BMgr;
