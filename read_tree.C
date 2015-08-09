#define read_tree_cxx
#include "read_tree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void read_tree::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L read_tree.C
//      Root > read_tree t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
   TH2D* h2=new TH2D("waveform","waveform for all channels",250,0.5,250.5,1000,0.0,1000.0);
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     for(int iCh=0;iCh<nChannel;iCh++){
       if(nameModule[iCh]==0x37334b54 && indexChannel[iCh]==31){
	 for(int iSample=0;iSample<nSample[iCh];iSample++){
	   h2->Fill(iSample+1,(*data)[iCh][iSample]);
	 }
       }
     }
      // if (Cut(ientry) < 0) continue;
   }
   gStyle->SetOptStat(0);
   TCanvas* c1=new TCanvas("c1","c1",800,518);
   h2->Draw("colz");
 c1->SaveAs("waveform.png");
 
}
