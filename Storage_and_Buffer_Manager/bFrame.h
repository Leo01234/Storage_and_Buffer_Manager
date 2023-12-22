#pragma once

#define FRAMESIZE 4096
using Char = unsigned char;
struct bFrame
{
    Char field[FRAMESIZE] = { 0 };
};
