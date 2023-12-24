#include "BCB.h"

BCB::BCB(int page_id, int frame_id, int dirty):
    page_id(page_id), frame_id(frame_id), dirty(dirty) {}