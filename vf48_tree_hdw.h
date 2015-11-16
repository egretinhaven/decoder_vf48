#ifndef __VF48_TREE_HDW_H__
#define __VF48_TREE_HDW_H__
#define MaxChannel 16*48
#define MaxData 1000
#include "Rtypes.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <TObject.h>

using namespace std;
/**
 * Base class for all components. We may use this for function prototypes.
 * copied from cooker
 */

class CRTBase: public TObject
{
 public:
  CRTBase();
  virtual ~CRTBase();
  ClassDef(CRTBase,1);
};

class CRTRawCsI:public CRTBase
{
 public:
  UInt_t runNo;
  UInt_t eventNo;
  Int_t isBad;
  UInt_t nChannel;
  vector<UInt_t> nameModule;
  vector<UInt_t> indexChannel;
  vector<UInt_t> nameCsI;
  vector<UInt_t> indexCsI;
  vector<ULong64_t> timeStamp;
  vector<UInt_t> timeCFD;
  vector<ULong64_t> charge;
  vector<UInt_t> nSample;
  vector<vector<UShort_t> > data;  
  CRTRawCsI();
  virtual ~CRTRawCsI();
  ClassDef(CRTRawCsI,1);
};
class VF48TreeHdw{
 private:
  TTree *tree;
  CRTRawCsI* pData;
 public:
 VF48TreeHdw():tree(0){
    tree=new TTree("vf48_hdw","decoded vf48 adcs of hdw"); 
    pData=new CRTRawCsI();
    pData->runNo=0;
    pData->eventNo=0;
    pData->isBad=1;
    pData->nChannel=0;
    tree->Branch("vf48",&pData,16000,0); 
  }
 VF48TreeHdw(UInt_t run):tree(0){
    tree=new TTree("vf48_hdw","decoded vf48 adcs of hdw"); 
    pData=new CRTRawCsI();
    pData->runNo=run;
    pData->eventNo=0;
    pData->isBad=1;
    pData->nChannel=0;
    tree->Branch("vf48",&pData,32000,0); 
  }

  ~VF48TreeHdw(){
  }
  void setEventNo(const UInt_t& evt){
    pData->eventNo=evt;
  }
  void setRunNo(const UInt_t& evt){
    pData->runNo=evt;
  }
  void setBad(){
    pData->isBad=-1;
  }
  Int_t badBit(){
    return pData->isBad;
  }
  void addChannel(const UInt_t& name, const UInt_t& index,const UInt_t& nmCsI, const UInt_t& iCsI, const ULong64_t& stamp, const UInt_t& time, const ULong64_t& charge,const vector<UShort_t>& adc);
  void fill(){
    tree->Fill();
  }
  void reset(){
    pData->nameModule.clear();
    pData->indexChannel.clear();
    pData->nameCsI.clear();
    pData->indexCsI.clear();
    pData->timeStamp.clear();
    pData->timeCFD.clear();
    pData->charge.clear();
    pData->nSample.clear();

    pData->data.clear();
    pData->nChannel=0;
    pData->eventNo=0;
    pData->isBad=1;
  }
};


#endif
