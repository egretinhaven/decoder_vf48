#ifndef __VF48_UNPACK_H__
#define __VF48_UNPACK_H__
#include <cstdint>
#include <cstring>
#define MaxModule 20
#include <iostream>
#include <fstream>
#include "Rtypes.h"
#include "vf48_tree_hdw.h"
using namespace std;
class VF48Channel{
 private:
  UInt_t mTime;//CFD time
  ULong64_t mCharge;//integrated charge
  vector<UShort_t> listData;
 public:
  void reset(){
    mTime=0;
    mCharge=0;
    listData.clear();
  }
  UInt_t time() const{
    return mTime;
  }
  ULong64_t charge() const{
    return mCharge;
  }
  UShort_t nSamples() const{
    return listData.size();
  }
  UShort_t data(int i) const{
    if(i<0) return 0;
    size_t mySize=listData.size(); 
    if((size_t)i>=mySize) return 0;
    return listData[i];
  }
  const vector<UShort_t>& data(){
    return listData;
  }
 public:
  void setTime(const UInt_t& t){
    mTime=t;
  }
  void setCharge(const ULong64_t& c){
    mCharge=c;
  }
 public:
 VF48Channel():mTime(0),mCharge(0),listData(){
  }
  ~VF48Channel(){
    //    cout<<"Channel destroyed"<<endl;
  }
 public:
  UInt_t addData(const UShort_t& d);
  void print();
};
class VF48Group{
 private:
  UInt_t mTrigger;
  ULong64_t mTimeStamp;
  VF48Channel* pChannel[8];
 public:
  void reset(){
    mTrigger=0;
    mTimeStamp=0;
    for(int i=0;i<8;i++){
      pChannel[i]->reset();
    }
  }
  UInt_t trigger() const{
    return mTrigger;
  }
  ULong64_t timeStamp() const{
    return mTimeStamp;
  }
  VF48Channel* channel(int i) const{
    if(i<0) return 0;
    if(i>7) return 0;
    return pChannel[i]; 
  }
 public:
  void setTrigger(UInt_t t){
    mTrigger=t;
  }
  void setTimeStamp(UInt_t low,UInt_t high){
    mTimeStamp=high;
    mTimeStamp=mTimeStamp<<32;
    mTimeStamp+=low;
  }
 public:
 VF48Group():mTrigger(0),mTimeStamp(0){
    for(UShort_t iCh=0;iCh<8;iCh++){
      pChannel[iCh]=new VF48Channel();
    }
  }

  ~VF48Group(){
    for(int i=0;i<8;i++){
      //      cout<<"destroying channel "<<i<<endl;
      delete pChannel[i];
    }
  }
 public:
  void print();
};
class VF48Module{
 private:
  UInt_t mName;
  VF48Group* pGroup[6];
 public:
  void reset(){
    for(int i=0;i<6;i++){
      pGroup[i]->reset();
    }
  }
  UInt_t name() const{
    return mName;
  }
  VF48Group* group(int i) const{
    if(i>=6) return 0;
    return pGroup[i];
  }
 public:
  void setName(const UInt_t & nm){
    mName=nm;
  }
 public:
 VF48Module(){
    mName=0;
    for(UShort_t iGr=0;iGr<6;iGr++){
      pGroup[iGr]=new VF48Group();
    }
  }
  ~VF48Module(){
    for(int i=0;i<6;i++){
      //      cout<<"destroying group "<<i<<endl;
      delete pGroup[i];
    }
  }
 public:
  void setName(const char * nm);
  void print();
};

class VF48Event{
 private:
  VF48Module* pModule[MaxModule];
  UInt_t mEventNo;
  UInt_t mRunNo;
  Int_t isBad;
 public:
  int nModule;
 public:
  void reset(){
    mEventNo=0;
    isBad=1;
    for(int i=0;i<nModule;i++){
      pModule[i]->reset();
    }
  }
  VF48Module* module(const int& i){
    if(i<0 || i > nModule-1) return 0;
    return pModule[i];
  }

  UInt_t eventNo() const{
    return mEventNo;
  }
  UInt_t runNo() const{
    return mRunNo;
  }
 public:
  void setEventNo(const int& ev){
    mEventNo=ev;
  }
  void setRunNo(const int& ev){
    mRunNo=ev;
  }
  void setBad(){
    isBad=-1;
  }
 public:
 VF48Event():mEventNo(0),mRunNo(0),isBad(1),nModule(0){
    for(int iM=0;iM<MaxModule;iM++){
      pModule[iM]=0;
    }
  }
 VF48Event(UInt_t run):mEventNo(0),mRunNo(run),isBad(1),nModule(0){
    for(int iM=0;iM<MaxModule;iM++){
      pModule[iM]=0;
    }
  }

  ~VF48Event(){
    for(int iModule=0;iModule<nModule;iModule++){
      //      cout<<"destroying module "<<iModule<<endl;
      delete pModule[iModule];
    }
  }
 public:
  VF48Module* getModule(const char* name);
  VF48Module* getModule(const UInt_t& name);
  void addModule(const char* name);
  void addModule(const UInt_t& name);
  void printModule();
  VF48Module* module(char* name) const;
  void print();
  int decode(UInt_t* buffer,UInt_t size);
  bool fillTree(VF48TreeHdw* tree);
};
#endif
