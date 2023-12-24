#include "BCB.h"

BCB::BCB(int page_id, int frame_id, int dirty, BCB *LRU_prev):
    page_id(page_id), frame_id(frame_id), dirty(dirty), LRU_prev(LRU_prev) {}