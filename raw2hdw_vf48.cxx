#include "raw2hdw_vf48.h"
#include "vf48_tree_hdw.h"
#include "hdw2det_map.h"
#include "TFile.h"

UInt_t iEvent=0;
UInt_t buff[1024*1024*25];//100M

void raw2hdw(ifstream& inData,VF48TreeHdw* pTree,UInt_t runNo){
  auto timeBegin=time(0);
  cout<<"Reading map for run "<<runNo<<":";
  UInt_t nElement=readMap(runNo);
  if(nElement==0) return;
  if(nElement!=768){
    cout<<"The number read from the map file is incorrect. It should be 768 and the actual number is "<<nElement<<endl;
    exit(-1);
  }
  int total=0;
  bool beginVf48=false;
  while(inData.good()){
    inData.read((char*)&buff[total],4);
    if(buff[total]==0x45564e54){
      if(!beginVf48){
	inData.read((char*)&buff[total],4*8);
	if(buff[total+7]==0xd0000000){//start of vf48 in raw data
	  beginVf48=true;
	  buff[0]=0xd0000000;
	  total=1;
	}
      }
      else{
	inData.read((char*)&buff[total],4*8);
	if(buff[total+7]==0xd0000000){//start of vf48 in raw data
	  buff[total]=0xd0000000;
	  total++;
	}
	else{
	  beginVf48=false;
	  VF48Event thisEvent(runNo);
	  thisEvent.decode(buff,total);
	  //	  thisEvent.print();
	  thisEvent.fillTree(pTree);
	  /*
	    if(!thisEvent.fillTree(pTree)){
	    for(int i=0;i<total;i++)
	    cout<<i<<":"<<hex<<buff[i]<<endl;
	    }*/
	  iEvent++;
	  if(iEvent%1000==0){
	    auto timeNow=time(0);
	    if(timeNow-timeBegin>10){
	      cout<<iEvent<<" events processed"<<endl;
	      timeBegin=timeNow;
	    }
	  }
	  total=0;
	  if(MaxEvent!=0 && iEvent>=MaxEvent) break;
	}//not vf48 anymore
      }
    }
    else{
      if(beginVf48){//inside vf48, simply keep
	total++;
      }
      else{
      }
    }
  }
  if(total>0 && iEvent<MaxEvent){
    VF48Event thisEvent(runNo);
    thisEvent.decode(buff,total);
    thisEvent.fillTree(pTree);
    iEvent++;
  }
}
int main(int argc,char** argv){
  if(!parseArgs(argc,argv)){
    printUsage("raw2hdw_vf48");  
    return 0;
  }
  auto timeBegin=time(0);

  TFile* fileOut=new TFile(NameRoot.c_str(),"RECREATE");
  VF48TreeHdw *pTree=new VF48TreeHdw;
  bool extractRunNo=true;
  if(RunNo!=0){
    extractRunNo=false;
    if(!readMap(RunNo)){
      cerr<<"Error reading map"<<endl;
      return -1;
    }
  }
  for(int iFile=0,nFile=listFile.size();iFile<nFile;iFile++){
    string file=listFile[iFile];
    cout<<"*************************************"<<endl;
    cout<<"processing file "<<file<<endl;
    auto timeFile=time(0);
    if(extractRunNo)
      findRunNo(file);
    ifstream inRaw(file);
    UInt_t currentEvent=iEvent;
    if(!inRaw.good()){
      cerr<<"Error opening raw file "<<file<<endl;
    }
    raw2hdw(inRaw,pTree,RunNo);
    if(MaxEvent>0 && iEvent>=MaxEvent){
      break;
    }
    inRaw.close();
    auto timeNow=time(0);
    cout<<"Processed "<<iEvent-currentEvent<<" events in "<<timeNow-timeFile<<" seconds, total "<<iEvent<<" events in "<<timeNow-timeFile<<" seconds"<<endl;
  }
  fileOut->Write();
  fileOut->Close();
  auto timeEnd=time(0);
  cout<<"============================================="<<endl;
  cout<<"Summary:"<<iEvent<<" events processed in "<<timeEnd-timeBegin<<" seconds"<<endl;

}
