#include "vf48_tree_hdw.h"
#include "iostream"
using namespace std;
void VF48TreeHdw::addChannel(const UInt_t& name, const UInt_t& index,const UInt_t& nmCsI, const UInt_t& iCsI,const ULong64_t& stamp, const UInt_t& time, const ULong64_t& chg,const vector<UShort_t>& adc){
  nameModule[nChannel]=name;
  indexChannel[nChannel]=index;
  nameCsI[nChannel]=nmCsI;
  indexCsI[nChannel]=iCsI;
  timeStamp[nChannel]=stamp;
  timeCFD[nChannel]=time;
  charge[nChannel]=chg;
  nSample[nChannel]=adc.size();
  data.push_back(adc);
  nChannel++;
}
