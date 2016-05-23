// Usage:
// > root -b doAll.C

// C++
#include <iostream>
#include <vector>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TH1F.h"
#include "TH2F.h"

// LepTree
#include "LepTree.cc"

using namespace std;
using namespace lepTree;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

void Fill1F(TH1F *&hist, double x, double w = 1)
{
  x = min(hist->GetXaxis()->GetBinCenter(hist->GetXaxis()->GetLast()) , x);
  x = max(hist->GetXaxis()->GetBinCenter(hist->GetXaxis()->GetFirst()), x);
  hist->Fill(x, w);
}

/*
enum histType {tag_mupt, tag_mueta, tag_muphi, den_mupt, den_mueta, den_muphi, num_mupt, num_mueta, num_muphi, dilep_mass};

vector<map<histType,TH1F*>> creatMuonHists(vector<string> tiggerNames){

  vector<map<histType,TH1F*>> triggerHists;

  for(int i=0; i < triggerNames.size(); i++){

    map<histType, TH1F*> muonHists;

    muonHists[tag_mupt]   = new TH1F(Form("h_tag_%s_mupt",   triggerNames[i].c_str()), "Muon pt",  90, 0, 150);
    muonHists[tag_mueta]  = new TH1F(Form("h_tag_%s_mueta",  triggerNames[i].c_str()), "Muon eta", 60, -3, 3);
    muonHists[tag_muphi]  = new TH1F(Form("h_tag_%s_muphi",  triggerNames[i].c_str()), "Muon phi", 70, -3.5, 3.5);

    muonHists[den_mupt]   = new TH1F(Form("h_den_%s_mupt",   triggerNames[i].c_str()), "Muon pt",  90, 0, 150);
    muonHists[den_mueta]  = new TH1F(Form("h_den_%s_mueta",  triggerNames[i].c_str()), "Muon eta", 60, -3, 3);
    muonHists[den_muphi]  = new TH1F(Form("h_den_%s_muphi",  triggerNames[i].c_str()), "Muon phi", 70, -3.5, 3.5);

    muonHists[num_mupt]   = new TH1F(Form("h_num_%s_mupt",   triggerNames[i].c_str()), "Muon pt",  90, 0, 150);
    muonHists[num_mueta]  = new TH1F(Form("h_num_%s_mueta",  triggerNames[i].c_str()), "Muon eta", 60, -3, 3);
    muonHists[num_muphi]  = new TH1F(Form("h_num_%s_muphi",  triggerNames[i].c_str()), "Muon phi", 70, -3.5, 3.5);

    muonHists[dilep_mass] = new TH1F(Form("h_dilep_%s_mass", triggerNames[i].c_str()), "InvM of the dilepton", 90, 0, 180);

    triggerHists.push_back(muonHists);
  }

  return triggerHists;
}

void fillMuonHists(map<histType, TH1F*>& histmap){


}



// inline int getTriggerValue(string trigName, TTree* tree, int event){

//   TBranch* trig_branch;
//   trig_branch = tree->GetBranch(trigName.c_str());
//   if (trig_branch == 0) {cerr << "Error: Cannot find branch under name: " << trigName << endl; return 0;}

//   int trig_value = 0;
//   trig_branch->SetAddress(&trig_value);
//   trig_branch->GetEntry(event);

//   return trig_value;
// }


vector<TBranch*>& setupTriggerBranches(vector<string> triggerNames, TTree* tree, int event){

  vector<TBranch*> trigBranches;

  for(int i=0; i<triggerNames.size(); i++){

    TBranch* trig_branch;
    trig_branch = tree->GetBranch(triggerNames[i].c_str());
    if (trig_branch == 0) {cerr << "Error: Cannot find branch under name: " << triggerNames[i] << endl; return 0;}

    int trig_value = 0;
    trig_branch->SetAddress(&trig_value);
  }
  return trigBranches;
}

vector<TBranch*>& setupTagTriggerBranches(vector<string> triggerNames, TTree* tree, int event){

  vector<TBranch*> trigBranches;

  for(int i=0; i<triggerNames.size(); i++){

    TBranch* tag_trig_branch;
    tag_trig_branch = tree->GetBranch(Form("tag_%s", triggerNames[i].c_str()));
    if (tag_trig_branch == 0) {cerr << "Error: Cannot find branch under name: tag_" << triggerNames[i] << endl; return 0;}
    tag_trig_branch->SetAddress(&trig_value);
  }
  return trigBranches;
}
*/


int ScanChain( TChain* chain, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Example Histograms
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
  // samplehisto->SetDirectory(rootdir);

  TH1F *h_tag_IsoMu24_mupt  = new TH1F("h_tag_IsoMu24_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_tag_IsoMu24_mueta = new TH1F("h_tag_IsoMu24_mueta", "Muon eta", 60, -3, 3);
  TH1F *h_tag_IsoMu24_muphi = new TH1F("h_tag_IsoMu24_muphi", "Muon phi", 70, -3.5, 3.5);

  TH1F *h_den_IsoMu24_mupt  = new TH1F("h_den_IsoMu24_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_den_IsoMu24_mueta = new TH1F("h_den_IsoMu24_mueta", "Muon eta", 60, -3, 3);
  TH1F *h_den_IsoMu24_muphi = new TH1F("h_den_IsoMu24_muphi", "Muon phi", 70, -3.5, 3.5);

  TH1F *h_num_IsoMu24_mupt  = new TH1F("h_num_IsoMu24_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_num_IsoMu24_mueta = new TH1F("h_num_IsoMu24_mueta", "Muon eta", 60, -3, 3);
  TH1F *h_num_IsoMu24_muphi = new TH1F("h_num_IsoMu24_muphi", "Muon phi", 70, -3.5, 3.5);

  TH1F *h_dilepM_IsoMu24  = new TH1F("h_dilepM_IsoMu24", "InvM of the dilepton", 90, 0, 180);

  TH1F *h_tag_IsoTkMu24_mupt  = new TH1F("h_tag_IsoTkMu24_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_tag_IsoTkMu24_mueta = new TH1F("h_tag_IsoTkMu24_mueta", "Muon eta", 60, -3, 3);
  TH1F *h_tag_IsoTkMu24_muphi = new TH1F("h_tag_IsoTkMu24_muphi", "Muon phi", 70, -3.5, 3.5);

  TH1F *h_den_IsoTkMu24_mupt  = new TH1F("h_den_IsoTkMu24_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_den_IsoTkMu24_mueta = new TH1F("h_den_IsoTkMu24_mueta", "Muon eta", 60, -3, 3);
  TH1F *h_den_IsoTkMu24_muphi = new TH1F("h_den_IsoTkMu24_muphi", "Muon phi", 70, -3.5, 3.5);

  TH1F *h_num_IsoTkMu24_mupt  = new TH1F("h_num_IsoTkMu24_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_num_IsoTkMu24_mueta = new TH1F("h_num_IsoTkMu24_mueta", "Muon eta", 60, -3, 3);
  TH1F *h_num_IsoTkMu24_muphi = new TH1F("h_num_IsoTkMu24_muphi", "Muon phi", 70, -3.5, 3.5);

  TH1F *h_dilepM_IsoTkMu24  = new TH1F("h_dilepM_IsoTkMu24", "InvM of the dilepton", 90, 0, 180);

  TH1F *h_tag_IsoMu20_mupt  = new TH1F("h_tag_IsoMu20_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_tag_IsoMu20_mueta = new TH1F("h_tag_IsoMu20_mueta", "Muon eta", 60, -3, 3);
  TH1F *h_tag_IsoMu20_muphi = new TH1F("h_tag_IsoMu20_muphi", "Muon phi", 70, -3.5, 3.5);

  TH1F *h_den_IsoMu20_mupt  = new TH1F("h_den_IsoMu20_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_den_IsoMu20_mueta = new TH1F("h_den_IsoMu20_mueta", "Muon eta", 60, -3, 3);
  TH1F *h_den_IsoMu20_muphi = new TH1F("h_den_IsoMu20_muphi", "Muon phi", 70, -3.5, 3.5);

  TH1F *h_num_IsoMu20_mupt  = new TH1F("h_num_IsoMu20_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_num_IsoMu20_mueta = new TH1F("h_num_IsoMu20_mueta", "Muon eta", 60, -3, 3);
  TH1F *h_num_IsoMu20_muphi = new TH1F("h_num_IsoMu20_muphi", "Muon phi", 70, -3.5, 3.5);

  TH1F *h_dilepM_IsoMu20  = new TH1F("h_dilepM_IsoMu20", "InvM of the dilepton", 90, 0, 180);

  TH1F *h_muonCount = new TH1F("h_muonCount", "Number of Muons in this event", 90, 0, 5);

  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if( nEvents >= 0 ) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile file( currentFile->GetTitle() );
    TTree *tree = (TTree*)file.Get("t");
    if(fast) TTreeCache::SetLearnEntries(10);
    if(fast) tree->SetCacheSize(128*1024*1024);
    t.Init(tree);

    int evt_num = -1;
    int nMuonCount = 0;
    int isTriggerMuon = 0;
    LorentzVector p4mu;
    LorentzVector tag_p4mu;

    // Loop over Events in current file
    if( nEventsTotal >= nEventsChain ) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {

      // Get Event Content
      if( nEventsTotal >= nEventsChain ) continue;
      if(fast) tree->LoadTree(event);
      t.GetEntry(event);
      ++nEventsTotal;

      // Progress
      LepTree::progress( nEventsTotal, nEventsChain );

      // Analysis Code

      // bool debug = true;
      bool debug = false;
      if (debug && event > 200) break;     // debug

      if (evt_run() < 273423) continue;    // Get runs after fixing the L1 interface problem

      int nevt = evt_event();
      if (nevt != evt_num) {
        h_muonCount->Fill(nMuonCount);
        nMuonCount = 0;
        isTriggerMuon = 0;
      }

      // --- Tag & Probe ---
      // if( abs(id()) == 13 && fabs(tag_p4().eta()) < 2.4 && tag_RelIso03EA() < 0.2){
      if( abs(id()) != 13 ) continue;
      if (tag_HLT_IsoMu24() > 0) {
        Fill1F(h_tag_IsoMu24_mupt, tag_p4().pt());
        Fill1F(h_tag_IsoMu24_mueta, tag_p4().eta());
        Fill1F(h_tag_IsoMu24_muphi, tag_p4().phi());
        if (fabs(p4().eta()) < 2.4 && RelIso03EA() < 0.15 && passes_POG_mediumID()) {
          Fill1F(h_dilepM_IsoMu24, dilep_mass());
          Fill1F(h_den_IsoMu24_mupt,  p4().pt());
          Fill1F(h_den_IsoMu24_mueta, p4().eta());
          Fill1F(h_den_IsoMu24_muphi, p4().phi());
          if (HLT_IsoMu24() > 0){
            Fill1F(h_num_IsoMu24_mupt,  p4().pt());
            Fill1F(h_num_IsoMu24_mueta, p4().eta());
            Fill1F(h_num_IsoMu24_muphi, p4().phi());
          }
        }
      }

      if (tag_HLT_IsoTkMu24() > 0) {
        Fill1F(h_tag_IsoTkMu24_mupt, tag_p4().pt());
        Fill1F(h_tag_IsoTkMu24_mueta, tag_p4().eta());
        Fill1F(h_tag_IsoTkMu24_muphi, tag_p4().phi());
        if (fabs(p4().eta()) < 2.4 && RelIso03EA() < 0.15 && passes_POG_mediumID()) {
          Fill1F(h_dilepM_IsoTkMu24, dilep_mass());
          Fill1F(h_den_IsoTkMu24_mupt,  p4().pt());
          Fill1F(h_den_IsoTkMu24_mueta, p4().eta());
          Fill1F(h_den_IsoTkMu24_muphi, p4().phi());
          if (HLT_IsoTkMu24() > 0){
            Fill1F(h_num_IsoTkMu24_mupt,  p4().pt());
            Fill1F(h_num_IsoTkMu24_mueta, p4().eta());
            Fill1F(h_num_IsoTkMu24_muphi, p4().phi());
          }
        }
      }

      if (tag_HLT_IsoMu20() > 0) {
        Fill1F(h_tag_IsoMu20_mupt, tag_p4().pt());
        Fill1F(h_tag_IsoMu20_mueta, tag_p4().eta());
        Fill1F(h_tag_IsoMu20_muphi, tag_p4().phi());
        if (fabs(p4().eta()) < 2.4 && RelIso03EA() < 0.15 && passes_POG_mediumID()) {
          Fill1F(h_dilepM_IsoMu20, dilep_mass());
          Fill1F(h_den_IsoMu20_mupt,  p4().pt());
          Fill1F(h_den_IsoMu20_mueta, p4().eta());
          Fill1F(h_den_IsoMu20_muphi, p4().phi());
          if (HLT_IsoMu20() > 0){
            Fill1F(h_num_IsoMu20_mupt,  p4().pt());
            Fill1F(h_num_IsoMu20_mueta, p4().eta());
            Fill1F(h_num_IsoMu20_muphi, p4().phi());
          }
        }
      }

      if (debug)
        cout << "[Debug] This is the " << event << "th event, with evt_event() " << evt_event()
             << ", isTriggerMuon = " << isTriggerMuon << ", and nMuonCount = " << nMuonCount << ",\n"
             << "    while mupt = " << p4().pt() << ", tag_mupt = " << tag_p4().pt() << ", and HLT_IsoMu20() = " << HLT_IsoMu20() << endl << endl;
      // << ", HLT_Mu17() = " << HLT_Mu17() << ", HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL() = " << HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL() << endl;
    }

    // Clean Up
    delete tree;
    file.Close();
  }
  if ( nEventsChain != nEventsTotal ) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }

  // return
  bmark->Stop("benchmark");

  TFile* file = new TFile("hists.root", "RECREATE");

  h_tag_IsoMu24_mupt->Write();
  h_tag_IsoMu24_mueta->Write();
  h_tag_IsoMu24_muphi->Write();

  h_den_IsoMu24_mupt->Write();
  h_den_IsoMu24_mueta->Write();
  h_den_IsoMu24_muphi->Write();

  h_num_IsoMu24_mupt->Write();
  h_num_IsoMu24_mueta->Write();
  h_num_IsoMu24_muphi->Write();

  h_dilepM_IsoMu24->Write();

  h_tag_IsoTkMu24_mupt->Write();
  h_tag_IsoTkMu24_mueta->Write();
  h_tag_IsoTkMu24_muphi->Write();

  h_den_IsoTkMu24_mupt->Write();
  h_den_IsoTkMu24_mueta->Write();
  h_den_IsoTkMu24_muphi->Write();

  h_num_IsoTkMu24_mupt->Write();
  h_num_IsoTkMu24_mueta->Write();
  h_num_IsoTkMu24_muphi->Write();

  h_dilepM_IsoTkMu24->Write();

  h_tag_IsoMu20_mupt->Write();
  h_tag_IsoMu20_mueta->Write();
  h_tag_IsoMu20_muphi->Write();

  h_den_IsoMu20_mupt->Write();
  h_den_IsoMu20_mueta->Write();
  h_den_IsoMu20_muphi->Write();

  h_num_IsoMu20_mupt->Write();
  h_num_IsoMu20_mueta->Write();
  h_num_IsoMu20_muphi->Write();

  h_dilepM_IsoMu20->Write();

  TH1F *h_eff_IsoMu24_mupt  = (TH1F*) h_num_IsoMu24_mupt->Clone();
  TH1F *h_eff_IsoMu24_mueta = (TH1F*) h_num_IsoMu24_mueta->Clone();
  TH1F *h_eff_IsoMu24_muphi = (TH1F*) h_num_IsoMu24_muphi->Clone();

  h_eff_IsoMu24_mupt->SetName("h_eff_IsoMu24_mupt");
  h_eff_IsoMu24_mueta->SetName("h_eff_IsoMu24_mueta");
  h_eff_IsoMu24_muphi->SetName("h_eff_IsoMu24_muphi");

  h_eff_IsoMu24_mupt->Divide(h_den_IsoMu24_mupt);
  h_eff_IsoMu24_mueta->Divide(h_den_IsoMu24_mueta);
  h_eff_IsoMu24_muphi->Divide(h_den_IsoMu24_muphi);

  h_eff_IsoMu24_mupt->Write();
  h_eff_IsoMu24_mueta->Write();
  h_eff_IsoMu24_muphi->Write();

  TH1F *h_eff_IsoMu20_mupt  = (TH1F*) h_num_IsoMu20_mupt->Clone();
  TH1F *h_eff_IsoMu20_mueta = (TH1F*) h_num_IsoMu20_mueta->Clone();
  TH1F *h_eff_IsoMu20_muphi = (TH1F*) h_num_IsoMu20_muphi->Clone();

  h_eff_IsoMu20_mupt->SetName("h_eff_IsoMu20_mupt");
  h_eff_IsoMu20_mueta->SetName("h_eff_IsoMu20_mueta");
  h_eff_IsoMu20_muphi->SetName("h_eff_IsoMu20_muphi");

  h_eff_IsoMu20_mupt->Divide(h_den_IsoMu20_mupt);
  h_eff_IsoMu20_mueta->Divide(h_den_IsoMu20_mueta);
  h_eff_IsoMu20_muphi->Divide(h_den_IsoMu20_muphi);

  h_eff_IsoMu20_mupt->Write();
  h_eff_IsoMu20_mueta->Write();
  h_eff_IsoMu20_muphi->Write();

  h_muonCount->Write();
  // h_dilepM->Write();

  file->Close();
  // cout << "\n------------------------------\n"
  //      << "moreThan2Muons: " << moreThan2Muons
  //      << "\n------------------------------\n";

  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;
  return 0;
}
