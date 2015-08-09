//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 11 14:51:51 2015 by ROOT version 5.34/07
// from TTree vf48/decode vf48 adcs
// found on file: vf48_decode.root
//////////////////////////////////////////////////////////

#ifndef read_tree_h
#define read_tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
using namespace std;
// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class read_tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          runNo;
   UInt_t          eventNo;
   Bool_t          isBad;
   UInt_t          nChannel;
   UInt_t          nameModule[16*48];   //[nChannel]
   UInt_t          indexChannel[16*48];   //[nChannel]
   ULong64_t       timeStamp[16*48];   //[nChannel]
   UInt_t          timeCFD[16*48];   //[nChannel]
   ULong64_t       charge[16*48];   //[nChannel]
   UInt_t       nSample[16*48];   //[nChannel]
   vector<vector<unsigned short> > *data;

   // List of branches
   TBranch        *b_runNo;
   TBranch        *b_eventNo;   //!
   TBranch        *b_isBad;
   TBranch        *b_nChannel;   //!
   TBranch        *b_nameModule;   //!
   TBranch        *b_indexChannel;   //!
   TBranch        *b_timeStamp;   //!
   TBranch        *b_timeCFD;   //!
   TBranch        *b_charge;   //!
   TBranch        *b_nSample;   //!
   TBranch        *b_data;   //!

   read_tree(TTree *tree=0);
   virtual ~read_tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef read_tree_cxx
read_tree::read_tree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("hdw_vf48.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("hdw_vf48.root");
      }
      f->GetObject("vf48_hdw",tree);

   }
   Init(tree);
}

read_tree::~read_tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t read_tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t read_tree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void read_tree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   data = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);
   fChain->SetBranchAddress("runNo", &runNo, &b_runNo);
   fChain->SetBranchAddress("eventNo", &eventNo, &b_eventNo);
   fChain->SetBranchAddress("isBad", &isBad, &b_isBad);
   fChain->SetBranchAddress("nChannel", &nChannel, &b_nChannel);
   fChain->SetBranchAddress("nameModule", nameModule, &b_nameModule);
   fChain->SetBranchAddress("indexChannel", indexChannel, &b_indexChannel);
   fChain->SetBranchAddress("timeStamp", timeStamp, &b_timeStamp);
   fChain->SetBranchAddress("timeCFD", timeCFD, &b_timeCFD);
   fChain->SetBranchAddress("charge", charge, &b_charge);
   fChain->SetBranchAddress("nSample", nSample, &b_nSample);
   fChain->SetBranchAddress("data", &data, &b_data);
   Notify();
}

Bool_t read_tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void read_tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t read_tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef read_tree_cxx
