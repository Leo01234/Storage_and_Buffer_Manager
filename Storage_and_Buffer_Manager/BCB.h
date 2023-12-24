#pragma once

struct BCB
{
    BCB(int page_id, int frame_id, int dirty);
    int page_id;
    int frame_id;
    int count = 1;
    int dirty;
    BCB *next = nullptr;

    // pointers for the LRU list
    BCB *LRU_next = nullptr;
    BCB *LRU_prev = nullptr;
};
