#include "hdw2det_map.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
string NameMap="vf48_map.txt";
mapCsI TreasureMap;
void dumpTreasureMap(){
  for(auto it=TreasureMap.begin();it!=TreasureMap.end();it++){
    char * name=(char*)&(it->second.first);
    cout<<"("<<hex<<it->first.first<<dec<<","<<it->first.second<<") =>"<<"("<<name[3]<<name[2]<<name[1]<<name[0]<<","<<it->second.second<<")"<<endl;
  }
}

UInt_t readMap(UInt_t run){

  ifstream inMap(NameMap);
  if(!inMap.good()){
    cerr<<"Error opening map file:"<<NameMap<<endl;
  }
  bool findRange=false;
  UInt_t lowRun,highRun;
  TreasureMap.clear();
  while(inMap.good()){
    string buff;
    getline(inMap,buff);
    if(!inMap.good()) break;
    if(buff.size()==0) continue;

    if(!findRange){//search for keyword run
      if(strcmp(buff.substr(0,3).c_str(),"run")==0){
        auto pos=buff.find("=");
        if(pos!=string::npos){
          stringstream par(buff.substr(pos+1,string::npos));
          par>>lowRun>>highRun;
          if(run>=lowRun && (run<=highRun || highRun==0)){
            findRange=true;

          }
        }
      }
    }
    else if(buff[0]>='0' && buff[0]<='9'){

      stringstream line(buff);
      UInt_t nameModule;
      string nameG[3];
      UInt_t name[3];

      line>>nameModule>>nameG[0]>>nameG[1]>>nameG[2];
      nameModule=0x30304b54+((nameModule/10)<<16)+((nameModule%10)<<24);

      for(int iG=0;iG<3;iG++){
	if(nameG[iG].size()==3){
	  name[iG]=0x30000000+(nameG[iG][0]<<16)+(nameG[iG][1]<<8)+nameG[iG][2];
	}
	else{
	  name[iG]=(nameG[iG][0]<<24)+(nameG[iG][1]<<16)+(nameG[iG][2]<<8)+nameG[iG][3];
	}
        for(int iC=0;iC<16;iC++){
          pairVf48 hdw(nameModule,iG*16+iC);
          pairCsI det(name[iG],iC+1);
          TreasureMap[hdw]=det;
        }
      }
    }
    else if(strcmp(buff.substr(0,3).c_str(),"run")==0){
      break;
    }
  }
  return TreasureMap.size();
}
