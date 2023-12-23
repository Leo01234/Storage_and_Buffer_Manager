#pragma once

struct BCB
{
    BCB();
    int page_id;
    int frame_id;
    int count;
    int dirty;
    BCB * next;
};
