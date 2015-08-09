//
// test_decode.h
//
//
#include <fstream>
#include <iostream>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;
#include "vf48_unpack.h"
#include <sstream>
string NameFilelist;
UInt_t RunNo=0;
UInt_t MaxEvent=0;
string NameRoot="hdw_vf48.root";
vector<string> listFile;
extern string NameMap;
void printUsage(const string& name){
  cout<<"Usage:"<<name<<" <options> [raw files]"<<endl;
  cout<<"raw files:     list of raw files to be converted, but  Ignored if -f option specified!!"<<endl;
  cout<<"               The name is supposed to be like /path/to/data/run#.dat."<<endl;
  cout<<"               Run number is extracted from the file name unless -r flags specified."<<endl;
  cout<<"options:"<<endl;
  cout<<"-f filelist:   string filelist contains a list of raw files with full path or relative path to current working directory"<<endl;
  cout<<"               one file in each line and file name is supposed to be like /path/to/data/run#.dat."<<endl;
  cout<<"               Run number will be extracted from each file name unless -r flags specified."<<endl;
  cout<<"-r #run:       non-negative #run is the run number overriding the filename."<<endl;
  cout<<"-n #number:    non-negative #number is the maximum number of events to be processed. It is unlimited if this options is missing."<<endl;
  cout<<"-o out.root:   string out.root is the name of the output root file. By default, it is hdw_vf48.root"<<endl;
  cout<<"-m map:        name of the map file. By default, it is vf48_map.txt"<<endl;
  cout<<"-h:            Print this and parsed parameters."<<endl;
}
void findRunNo(string name){
  auto posBeg=name.rfind("/run");
  if(posBeg!=string::npos){
    auto posEnd=name.find(".dat",posBeg);
    if(posEnd!=string::npos){
      string strRun=name.substr(posBeg+4,posEnd-posBeg-4);
      RunNo=stoul(strRun);
    }
    else
      RunNo=0;
  }
  else
    RunNo=0;
}
void buildFilelist(){
  if(NameFilelist.empty()) return;
  listFile.clear();
  ifstream inFilelist(NameFilelist.c_str());
  while(inFilelist.good()){
    string buff;
    getline(inFilelist,buff);
    if(inFilelist.good()){
      listFile.push_back(buff);
    }
  }
}
bool parseArgs(int argc,char** argv){
  if (argc==1) return false;
  bool writeHelp=false;
  for(int i=1;i<argc;i++){
    if(argv[i][0]=='-'){
      switch (argv[i][1]){
      case 'h':
	writeHelp=true;
	break;
      case 'f'://filelist
	{
	  if(i+1<argc && argv[i+1][0]!='-'){
	    NameFilelist=argv[i+1];
	    i++;
	  }
	  else
	    return false;
	}
	break;
      case 'm'://filelist
	{
	  if(i+1<argc && argv[i+1][0]!='-'){
	    NameMap=argv[i+1];
	    i++;
	  }
	  else
	    return false;
	}
	break;
      case 'r'://run number
	{
	  if(i+1<argc && argv[i+1][0]!='-'){
	    RunNo=stoul(argv[i+1]);
	    i++;
	  }
	  else
	    return false;
	}
	break;
      case 'n'://Max number of events to be processed
	{
	  if(i+1<argc && argv[i+1][0]!='-'){
	    MaxEvent=stoul(argv[i+1]);
	    i++;
	  }
	  else
	    return false;
	}
	break;
      case 'o'://root file name
	{
	  if(i+1<argc && argv[i+1][0]!='-'){
	    NameRoot=argv[i+1];
	    i++;
	  }
	  else
	    return false;
	}
	break;
      }//end of switch
    }//options
    else{
      listFile.push_back(argv[i]);
    }
  }//loop over all args
  buildFilelist();
  if(writeHelp){
    if(RunNo>0){
      cout<<"Run number:"<<RunNo<<endl;
    }
    if(MaxEvent>0){
      cout<<"Max number of events:"<<MaxEvent<<endl;
    }
    if(!NameFilelist.empty()){
      cout<<"Name of filelist:"<<NameFilelist<<endl;
    }
    if(listFile.size()!=0){
      UInt_t nFile=listFile.size();
      cout<<"number of files:"<<nFile<<endl;
      for(UInt_t iFile=0;iFile<nFile;iFile++){
	cout<<iFile<<":"<<listFile[iFile]<<endl;
      }

    }
    return false;
  }

  return true;
}
