#include <iostream>
#include <cstdlib>
#include "BMgr.h"
#include "DSMgr.h"

using std::cout;
using std::endl;
using std::exit;

BMgr::BMgr() = default;
int BMgr::FixPage(int page_id) {
    BCB *p = ptof[Hash(page_id)];
    while (p != nullptr) {
        if (p->page_id == page_id) { // page already in buffer
            p->count++;
            if (MRU != p) {
                RemoveFromLRUList(p);
                AddToMRU(p);
            }
            return p->frame_id;
        }
        p = p->next;
    }

    int frame_id;
    if (min_free_frame_id < g_bufsize) { // has free frame
        frame_id = min_free_frame_id;
        min_free_frame_id++;
    }
    else { // no free frame
        frame_id = SelectVictim();
    }

    // read in the page
    g_buf[frame_id] = g_DSMgr.ReadPage(page_id);

    BCB *new_BCB = new BCB(page_id, frame_id, 0);

    // update ftop
    ftop[frame_id] = page_id;

    // add to ptof
    BCB **bucket = ptof + Hash(page_id);
    new_BCB->next = *bucket;
    *bucket = new_BCB;

    // add to LRU list
    AddToMRU(new_BCB);

    return frame_id;
}
NewPage BMgr::FixNewPage() {

    // checks the pages array for a use_bit of zero.
    // If one is found, the page is reused.
    // If not, a new page is allocated.

    bool has_reusable_page = false;
    int page_id = g_DSMgr.GetNumPages();
    for (int i = 0; i < g_DSMgr.GetNumPages(); i++) {
        if (!g_DSMgr.GetUse(i)) { // use_bit is 0
            page_id = i;
            g_DSMgr.SetUse(i, 1);
            has_reusable_page = true;
            break;
        }
    }
    if (!has_reusable_page) {
        g_DSMgr.IncNumPages();
        g_DSMgr.SetUse(page_id, 1);
    }

    // then do things the same as FixPage
    // without checking the buffer for the page
    // and without reading in the page
    // BCB dirty should be initialized to 1

    int frame_id;
    if (min_free_frame_id < g_bufsize) { // has free frame
        frame_id = min_free_frame_id;
        min_free_frame_id++;
    }
    else { // no free frame
        frame_id = SelectVictim();
    }

    BCB *new_BCB = new BCB(page_id, frame_id, 1);

    // update ftop
    ftop[frame_id] = page_id;

    // add to ptof
    BCB **bucket = ptof + Hash(page_id);
    new_BCB->next = *bucket;
    *bucket = new_BCB;

    // add to LRU list
    AddToMRU(new_BCB);

    return NewPage(page_id, frame_id);

}
int BMgr::UnfixPage(int page_id) {
    BCB *p = ptof[Hash(page_id)];
    while (p != nullptr) {
        if (p->page_id == page_id) {
            p->count--;
            return p->frame_id;
        }
        p = p->next;
    }
    return -1;
}
int BMgr::NumFreeFrames() {
    return min_free_frame_id;
}
int BMgr::SelectVictim() {
    BCB *victim = LRU;
    while (victim->count != 0) {
        victim = victim->LRU_next;
    }
    int frame_id = victim->frame_id;
    if (victim->dirty) {
        RemoveLRUEle(frame_id);
    }
    RemoveBCB(victim, victim->page_id);
    return frame_id;
}
int BMgr::Hash(int page_id) {
    return page_id % g_bufsize;
}
void BMgr::RemoveBCB(BCB *ptr, int page_id) {
    RemoveFromLRUList(ptr);
    BCB **bucket = ptof + Hash(page_id);
    if (*bucket == ptr) {
        *bucket = ptr->next;
        delete ptr;
        return;
    }
    BCB *p = *bucket;
    while (p != nullptr) {
        if (p->next == ptr) {
            p->next = ptr->next;
            delete ptr;
            return;
        }
        p = p->next;
    }
}
void BMgr::RemoveLRUEle(int frid) {
    if (g_DSMgr.WritePage(ftop[frid], g_buf[frid]) < FRAMESIZE) {
        cout << "write fail" << endl;
        exit(EXIT_FAILURE);
    }
}
void BMgr::SetDirty(int frame_id) {
    BCB *p = ptof[Hash(ftop[frame_id])];
    while (p != nullptr) {
        if (p->frame_id == frame_id) {
            p->dirty = 1;
            return;
        }
        p = p->next;
    }
}
void BMgr::UnsetDirty(int frame_id) {
    BCB *p = ptof[Hash(ftop[frame_id])];
    while (p != nullptr) {
        if (p->frame_id == frame_id) {
            p->dirty = 0;
            return;
        }
        p = p->next;
    }
}
void BMgr::WriteDirtys() {
    for (int i = 0; i < g_bufsize; i++) {
        BCB *p = ptof[i];
        while (p != nullptr) {
            if (p->dirty) { // if dirty is 1
                RemoveLRUEle(p->frame_id);
            }
            p = p->next;
        }
    }
}
void BMgr::PrintFrame(int frame_id) {
    Char *field = g_buf[frame_id].field;
    for (int i = 0; i < FRAMESIZE; i++) {
        cout << field[i];
    }
    cout << endl;
}
void BMgr::RemoveFromLRUList(BCB* ptr) {
    if (LRU == ptr && MRU == ptr) { // there is only one element in the list
        LRU = nullptr;
        MRU = nullptr;
    }
    else if (LRU == ptr) {
        ptr->LRU_next->LRU_prev = nullptr;
        LRU = ptr->LRU_next;
    }
    else if (MRU == ptr) {
        ptr->LRU_prev->LRU_next = nullptr;
        MRU = ptr->LRU_prev;
    }
    else {
        ptr->LRU_next->LRU_prev = ptr->LRU_prev;
        ptr->LRU_prev->LRU_next = ptr->LRU_next;
    }
}
void BMgr::AddToMRU(BCB* ptr) {
    if (MRU == nullptr) {
        MRU = ptr;
        LRU = ptr;
        ptr->LRU_next = nullptr;
        ptr->LRU_prev = nullptr;
    }
    else {
        MRU->LRU_next = ptr;
        ptr->LRU_prev = MRU;
        ptr->LRU_next = nullptr;
        MRU = ptr;
    }
}
void BMgr::InitDynamicArrays() {
    ftop = new int[g_bufsize]; // do not need to initialize to 0
    ptof = new BCB *[g_bufsize](); // initialize to nullptr
}
void BMgr::DestoryDynamicArrays() {
    // free all BCBs
    for (int i = 0; i < g_bufsize; i++) {
        BCB *p = ptof[i];
        while (p != nullptr) {
            BCB *q = p;
            p = p->next;
            delete q;
        }
    }

    delete [] ptof;
    delete [] ftop;
}

BMgr g_BMgr;
