#include "vf48_unpack.h"
#include <iostream>
#include <vector>
#include <map>
#include "hdw2det_map.h"
using namespace std;
UInt_t VF48Channel::addData(const UShort_t& d){
  listData.push_back(d);
  return (UInt_t)listData.size();
}
void VF48Channel::print(){
  UInt_t nSample=listData.size();
  cout<<"Time "<<mTime<<" Charge "<<mCharge<<" with "<<nSample<<" data"<<endl;
  if(nSample>0){
    for(UInt_t i=0;i<nSample;i++){
      cout<<listData[i]<<"\t";
    }
    cout<<endl;
  }
}
void VF48Group::print(){
  cout<<"Trigger "<<mTrigger<<" Timestamp "<<mTimeStamp<<endl;
  for(int i=0;i<8;i++){
    cout<<"Channel "<<i<<":";
    pChannel[i]->print();
  }
}
void VF48Module::setName(const char* nm){
  mName=0;
  char* pName=(char*)&mName;
  for(int i=0;i<4;i++){
    pName[i]=nm[i];
  }
}
void VF48Module::print(){
  cout<<"Module ";
  cout<<hex;
  cout<<mName;
  cout<<dec;
  cout<<" "<<endl;
  for(int i=0;i<6;i++){
    cout<<"Group "<<i<<":";
    pGroup[i]->print();
  }
}
void VF48Event::print(){
  if(nModule==0){ cout<<"No module installed!!"<<endl;}
  else{
    cout<<"Event "<<mEventNo<<endl;
    for(int i=0;i<nModule;i++){
      cout<<"Module "<<i<<" ";
      pModule[i]->print();
    }
    cout<<"End of event"<<endl;
  }
}
void VF48Event::printModule(){
  if(nModule==0){ cout<<"No module installed!!"<<endl;}
  else{
    cout<<nModule<<" modules installed."<<endl;
    for(int i=0;i<nModule;i++){
      pModule[i]->print();
    }
  }
  
}
void VF48Event::addModule(const char* name){
  pModule[nModule]=new VF48Module();
  pModule[nModule]->setName(name);
  nModule++;
}
void VF48Event::addModule(const UInt_t& name){
  pModule[nModule]=new VF48Module();
  pModule[nModule]->setName(name);
  nModule++;
}

VF48Module* VF48Event::getModule(const UInt_t& name){
  int index=-1;
  for(int i=0;i<nModule;i++){
    if(pModule[i]->name()==name){
      index=i;
      break;
    }
  }
  if(index==-1) return NULL;
  return pModule[index];
}
VF48Module* VF48Event::getModule(const char* name){
  UInt_t word=0;
  for(int i=0;i<4;i++){
    word=word<<8;
    word+=name[i];
  }
  return getModule(word);
}


int VF48Event::decode(UInt_t* buffer,UInt_t sizeBuffer){
  map<UInt_t,UInt_t> listTrigger;
  for(UInt_t iBuffer=0;iBuffer<sizeBuffer;iBuffer++){
    if((buffer[iBuffer]&0xF0000000)==0x80000000){//trigger number
      UInt_t trigNo =buffer[iBuffer]& 0x00FFFFFF;
      listTrigger[trigNo]++;
    }
  }
  UInt_t myTrigger=0;
  UInt_t myTimes=0;
  for(auto thisNode=listTrigger.begin();thisNode!=listTrigger.end();thisNode++){
    if(thisNode->second>myTimes){//there should be only one trigger number repeated 6 times
      myTimes=thisNode->second;
      myTrigger=thisNode->first;
    }
  }
  if(myTrigger==0) return -1;
  mEventNo=myTrigger;
  VF48Group* pGroup=0;
  VF48Channel* pChannel=0;
  VF48Module* pModule=0;
  int iGroup=-1;
  int iChannel=-1;
  bool doDump=false;
  //  if(myTrigger>0x1dea) doDump=true;
  for(UInt_t iBuffer=0;iBuffer<sizeBuffer;iBuffer++){
    if(doDump)
      printf("%u,%x\n",iBuffer,buffer[iBuffer]);
    switch(buffer[iBuffer]&0xF0000000){

    default:
      {
	if(buffer[iBuffer]!=0xbad0bad0){
	  cout<<"warning: unexpected data "<<hex<<buffer[iBuffer]<<endl;
	  cout<<dec;
	}
	else{
	  cout<<"event slipping"<<endl;
	  setBad();
	}
	break;
      }
    case 0xE0000000://end of trigger of this group, skip everything of this group
      {

	int nextGroup=iGroup+1;
	UInt_t expectGroup=0xfffffff0+nextGroup;
	bool doSkipping=false;
	while(iBuffer+1<sizeBuffer && (buffer[iBuffer+1]!=expectGroup  && (buffer[iBuffer+1]&0xff000000)!=0xd0000000)){
	  //	  cout<<"skipping "<<hex<<buffer[iBuffer]<<dec<<endl;
	  iBuffer++;
	  //doSkipping=true;
	}
	if(doSkipping){
	  if(iBuffer+1<sizeBuffer)
	    cout<<hex<<"stop at "<<buffer[iBuffer+1]<<dec<<endl;
	  else
	    cout<<"stop at the end of stream"<<endl;
	}
	iChannel=-1;

	break;
      }
    case 0xD0000000://index of modules
      {
	//	cout<<":control sequence"<<endl;
	if(buffer[iBuffer]==0xdeaddead) break;
	if(buffer[iBuffer]==0xdeafdeaf){
	  cout<<"event slip"<<endl;
	  setBad();
	  break;
	}
	iBuffer++;
	//cout<<hex<<buffer[iBuffer-1]<<" reading module "<<buffer[iBuffer]<<dec<<endl;
	//	cout<<"around:"<<hex<<buffer[iBuffer-1]<<" "<<buffer[iBuffer]<<" "<<buffer[iBuffer+1]<<buffer[iBuffer+2]<<" "<<buffer[iBuffer+3]<<" "<<buffer[iBuffer+4]<<endl;
	//    printf("%u,%x\n",iBuffer,buffer[iBuffer]);
	addModule(buffer[iBuffer]);
	pModule=module(nModule-1);
	iGroup=-1;
	iChannel=-1;
	break;
      }
    case 0xF0000000://group number
      {
	//	cout<<":group"<<endl;
	iGroup=buffer[iBuffer]&0x0000000F;
	pGroup=pModule->group(iGroup);
	break;
      }
    case 0x80000000://trigger, begin of a group
      {
	//	cout<<":trigger"<<endl;
	while(buffer[iBuffer]!=(0x80000000+myTrigger)){
	  iBuffer++;
	  //    printf("%u,%x\n",iBuffer,buffer[iBuffer]);
	}
	if((buffer[iBuffer-1]&0xF0000000)!=0xF0000000){//no group specified
	  iGroup++;
	  pGroup=pModule->group(iGroup);
	}
	UInt_t trigNo =buffer[iBuffer]& 0x00FFFFFF; 
	pGroup->setTrigger(trigNo);
	iChannel=-1;
	//	pChannel=pGroup->channel(iChannel);
	break;
      }
    case 0xA0000000: // time stamp
      {
	//	cout<<":timestamp"<<endl;
	UInt_t t1 = buffer[iBuffer]&0x00FFFFFF;
	iBuffer++;
	//    printf("%u,%x\n",iBuffer,buffer[iBuffer]);
	UInt_t t2=buffer[iBuffer]&0x00FFFFFF;
	pGroup->setTimeStamp(t1,t2);
	break;
      }
    case 0xC0000000://channel
      {
	//	cout<<":channel"<<endl;
	iChannel=buffer[iBuffer]&0x0000000F;
	pChannel=pGroup->channel(iChannel);
	break;
      }
    case 0x00000000://data
      {
	//	cout<<":data"<<endl;
	if (buffer[iBuffer]==0) // 64-bit padding
	  {
	    break;
	  }
	if(iChannel==-1) break;
	UShort_t sample1 =buffer[iBuffer]& 0x3FFF;
	UShort_t sample2 = (buffer[iBuffer]>>16) & 0x3FFF;
	pChannel->addData(sample1);
	pChannel->addData(sample2);
	break;
      }
    case 0x50000000://charge
      {
	//	cout<<":charge"<<endl;
	ULong64_t c0 =buffer[iBuffer]& 0x00FFFFFF;
	ULong64_t c1=0;
	ULong64_t c2=0;
	if((buffer[iBuffer+1]&0xF0000000)==0x50000000){
	  iBuffer++;
	  //    printf("%u,%x\n",iBuffer,buffer[iBuffer]);
	  c1=buffer[iBuffer]& 0x00FFFFFF;
	  if((buffer[iBuffer+1]&0xF0000000)==0x50000000){
	    iBuffer++;
	    //    printf("%u,%x\n",iBuffer,buffer[iBuffer]);
	    c2=buffer[iBuffer]& 0x00FFFFFF;
	  }//c2
	}//c1
	ULong64_t charge=c0+(c1<<24)+(c2<<48);
	pChannel->setCharge(charge);
	break;
      }
    case 0x40000000://cfd time
      {
	//	cout<<":time"<<endl;
	UInt_t t =buffer[iBuffer]& 0x00FFFFFF;
	pChannel->setTime(t);
	break;
      }
    }//end of switch
    
  }//end of looping buffer
  
  return 0;
}
bool VF48Event::fillTree(VF48TreeHdw* tree){
  bool goodTree=true;
  tree->reset();
  tree->setRunNo(mRunNo);
  tree->setEventNo(mEventNo);
  for(int iModule=0;iModule<nModule;iModule++){
    for(int iGroup=0;iGroup<6;iGroup++){
      for(int iChannel=0;iChannel<8;iChannel++){
	UInt_t name=module(iModule)->name();
	ULong64_t stamp=module(iModule)->group(iGroup)->timeStamp();
	VF48Channel* pChannel=module(iModule)->group(iGroup)->channel(iChannel);
	UInt_t time=pChannel->time();
	ULong64_t charge=pChannel->charge();
	UInt_t nSample=pChannel->nSamples();
	UInt_t index=iGroup*8+iChannel;
	pairVf48 hdw(name,index);
	pairCsI det=TreasureMap[hdw];
	if(nSample>0){
	  if(nSample<200 && isBad==1){
	    //	    cout<<"Warning:number of sample of channel "<<iChannel<<" in group "<<iGroup<<" of module "<<hex<<name<<dec<<" is "<<nSample<<endl;
	    goodTree=false;
	  }
	  tree->addChannel(name,index,det.first,det.second,stamp,time,charge,pChannel->data());
	} 
      }
    }
  }
  if(isBad==-1){
    tree->setBad();
    cout<<"filling slipped event "<<tree->badBit()<<endl;
  }
  tree->fill();
  return goodTree;
}
