#ifndef __VF48_TREE_HDW_H__
#define __VF48_TREE_HDW_H__
#define MaxChannel 16*48
#define MaxData 1000
#include "Rtypes.h"
#include "TTree.h"
#include <vector>
#include <iostream>
using namespace std;
class VF48TreeHdw{
 private:
  TTree *tree;
  UInt_t runNo;
  UInt_t eventNo;
  Int_t isBad;
  UInt_t nChannel;
  UInt_t nameModule[MaxChannel];
  UInt_t indexChannel[MaxChannel];
  UInt_t nameCsI[MaxChannel];
  UInt_t indexCsI[MaxChannel];
  ULong64_t timeStamp[MaxChannel];
  UInt_t timeCFD[MaxChannel];
  ULong64_t charge[MaxChannel];
  UInt_t nSample[MaxChannel];

  vector<vector<UShort_t> > data;
 public:
 VF48TreeHdw():tree(0),runNo(0),eventNo(0),isBad(1),nChannel(0){
    tree=new TTree("vf48_hdw","decoded vf48 adcs of hdw"); 
    tree->Branch("runNo",&runNo,"runNo/i");
    tree->Branch("eventNo",&eventNo,"eventNo/i");
    tree->Branch("isBad",&isBad,"isBad/I");
    tree->Branch("nChannel",&nChannel,"nChannel/i");
    tree->Branch("nameModule",nameModule,"nameModule[nChannel]/i");
    tree->Branch("indexChannel",indexChannel,"indexChannel[nChannel]/i");
    tree->Branch("nameCsI",nameCsI,"nameCsI[nChannel]/i");
    tree->Branch("indexCsI",indexCsI,"indexCsI[nChannel]/i");
    tree->Branch("timeStamp",timeStamp,"timeStamp[nChannel]/l");
    tree->Branch("timeCFD",timeCFD,"timeCFD[nChannel]/i");
    tree->Branch("charge",charge,"charge[nChannel]/l");
    tree->Branch("nSample",nSample,"nSample[nChannel]/i");
    tree->Branch("data",&data);
  }
 VF48TreeHdw(UInt_t run):tree(0),runNo(run),eventNo(0),isBad(1),nChannel(0){
    tree=new TTree("vf48","decoded vf48 adcs"); 
    tree->Branch("runNo",&runNo,"runNo/i");
    tree->Branch("eventNo",&eventNo,"eventNo/i");
    tree->Branch("isBad",&isBad,"isBad/I");
    tree->Branch("nChannel",&nChannel,"nChannel/i");
    tree->Branch("nameModule",nameModule,"nameModule[nChannel]/i");
    tree->Branch("indexChannel",indexChannel,"indexChannel[nChannel]/i");
    tree->Branch("nameCsI",nameCsI,"nameCsI[nChannel]/i");
    tree->Branch("indexCsI",indexCsI,"indexCsI[nChannel]/i");
    tree->Branch("timeStamp",timeStamp,"timeStamp[nChannel]/l");
    tree->Branch("timeCFD",timeCFD,"timeCFD[nChannel]/i");
    tree->Branch("charge",charge,"charge[nChannel]/l");
    tree->Branch("nSample",nSample,"nSample[nChannel]/i");
    tree->Branch("data",&data);
  }

  ~VF48TreeHdw(){
  }
  void setEventNo(const UInt_t& evt){
    eventNo=evt;
  }
  void setRunNo(const UInt_t& evt){
    runNo=evt;
  }
  void setBad(){
    isBad=-1;
  }
  Int_t badBit(){
    return isBad;
  }
  void addChannel(const UInt_t& name, const UInt_t& index,const UInt_t& nmCsI, const UInt_t& iCsI, const ULong64_t& stamp, const UInt_t& time, const ULong64_t& charge,const vector<UShort_t>& adc);
  void fill(){
    tree->Fill();
  }
  void reset(){
    data.clear();
    nChannel=0;
    eventNo=0;
    isBad=1;
  }
};


#endif
