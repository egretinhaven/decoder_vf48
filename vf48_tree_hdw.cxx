#include "vf48_tree_hdw.h"
#include "iostream"
using namespace std;
CRTBase::CRTBase() {};
CRTBase::~CRTBase() {};
ClassImp(CRTBase);
CRTVF48::CRTVF48() {};
CRTVF48::~CRTVF48() {};
ClassImp(CRTVF48);

void VF48TreeHdw::addChannel(const UInt_t& name, const UInt_t& index,const UInt_t& nmCsI, const UInt_t& iCsI,const ULong64_t& stamp, const UInt_t& time, const ULong64_t& chg,const vector<UShort_t>& adc){
  pData->nameModule.push_back(name);
  pData->indexChannel.push_back(index);
  pData->nameCsI.push_back(nmCsI);
  pData->indexCsI.push_back(iCsI);
  pData->timeStamp.push_back(stamp);
  pData->timeCFD.push_back(time);
  pData->charge.push_back(chg);
  pData->nSample.push_back(adc.size());
  pData->data.push_back(adc);
  pData->nChannel++;
}
