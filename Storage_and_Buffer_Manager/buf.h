#pragma once

#include "bFrame.h"

#define DEFBUFSIZE 1024
extern int g_bufsize;

// use dynamic array to create buffer of the size that the user defined by the input parameter
extern bFrame *g_buf;
