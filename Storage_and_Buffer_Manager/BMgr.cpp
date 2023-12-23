#include "BMgr.h"

BMgr::BMgr() = default;
BMgr::~BMgr() {
    delete [] ptof;
    delete [] ftop;
}