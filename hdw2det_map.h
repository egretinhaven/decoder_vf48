#ifndef __HDW_2_DET_MAP_H__
#define __HDW_2_DET_MAP_H__

#include <utility>
#include <map>
#include "Rtypes.h"
using namespace std;
typedef pair<UInt_t,UInt_t> pairVf48;
typedef pair<UInt_t,UInt_t> pairCsI;
typedef map<pairVf48,pairCsI> mapCsI;
extern mapCsI TreasureMap;
void dumpTreasureMap();
UInt_t readMap(UInt_t run);

#endif
