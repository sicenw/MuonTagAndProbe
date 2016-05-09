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

int ScanChain( TChain* chain, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Example Histograms
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
  // samplehisto->SetDirectory(rootdir);
  TH1F *h_loose_mupt  = new TH1F("h_loose_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_loose_mueta = new TH1F("h_loose_mueta", "Muon eta", 90, -3, 3);
  TH1F *h_loose_muphi = new TH1F("h_loose_muphi", "Muon phi", 90, -3.5, 3.5);

  TH1F *h_med_mupt  = new TH1F("h_med_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_med_mueta = new TH1F("h_med_mueta", "Muon eta", 90, -3, 3);
  TH1F *h_med_muphi = new TH1F("h_med_muphi", "Muon phi", 90, -3.5, 3.5);

  TH1F *h_tight_mupt  = new TH1F("h_tight_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_tight_mueta = new TH1F("h_tight_mueta", "Muon eta", 90, -3, 3);
  TH1F *h_tight_muphi = new TH1F("h_tight_muphi", "Muon phi", 90, -3.5, 3.5);

  TH1F *h_loose_trig_mupt  = new TH1F("h_loose_trig_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_loose_trig_mueta = new TH1F("h_loose_trig_mueta", "Muon eta", 90, -3, 3);
  TH1F *h_loose_trig_muphi = new TH1F("h_loose_trig_muphi", "Muon phi", 90, -3.5, 3.5);

  TH1F *h_med_trig_mupt  = new TH1F("h_med_trig_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_med_trig_mueta = new TH1F("h_med_trig_mueta", "Muon eta", 90, -3, 3);
  TH1F *h_med_trig_muphi = new TH1F("h_med_trig_muphi", "Muon phi", 90, -3.5, 3.5);

  TH1F *h_tight_trig_mupt  = new TH1F("h_tight_trig_mupt",  "Muon pt",  90, 0, 150);
  TH1F *h_tight_trig_mueta = new TH1F("h_tight_trig_mueta", "Muon eta", 90, -3, 3);
  TH1F *h_tight_trig_muphi = new TH1F("h_tight_trig_muphi", "Muon phi", 90, -3.5, 3.5);

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
      
      // if (event > 200) break;          // debug

      int nevt = evt_event();
      if (nevt != evt_num) {
        evt_num = nevt;
        if(nMuonCount == 1) {
          if (passes_POG_looseID()) {
            Fill1F(h_loose_mupt, p4mu.pt());
            Fill1F(h_loose_mueta, p4mu.eta());
            Fill1F(h_loose_muphi, p4mu.phi());
            if (isTriggerMuon == 1) {
              Fill1F(h_loose_trig_mupt, p4mu.pt());
              Fill1F(h_loose_trig_mueta, p4mu.eta());
              Fill1F(h_loose_trig_muphi, p4mu.phi());
            }
          } 
          if (passes_POG_mediumID()) {
            Fill1F(h_med_mupt, p4mu.pt());
            Fill1F(h_med_mueta, p4mu.eta());
            Fill1F(h_med_muphi, p4mu.phi());
            if (isTriggerMuon == 1) {
              Fill1F(h_med_trig_mupt, p4mu.pt());
              Fill1F(h_med_trig_mueta, p4mu.eta());
              Fill1F(h_med_trig_muphi, p4mu.phi());
            }
          } 
          if (passes_POG_tightID()) {
            Fill1F(h_tight_mupt, p4mu.pt());
            Fill1F(h_tight_mueta, p4mu.eta());
            Fill1F(h_tight_muphi, p4mu.phi());
            if (isTriggerMuon == 1) {
              Fill1F(h_tight_trig_mupt, p4mu.pt());
              Fill1F(h_tight_trig_mueta, p4mu.eta());
              Fill1F(h_tight_trig_muphi, p4mu.phi());
            }
          }
        }
        // if(isTriggerMuon > 1) cout << "Error: More than one trigger muon!!\n";
        nMuonCount = 0;
        isTriggerMuon = 0;
      }

      if (abs(id()) == 13 && p4().pt() > 20.0 && fabs(p4().eta()) < 2.4 && RelIso03EA() < 0.2 ){
        nMuonCount++;
        if (HLT_IsoMu20() > 0) isTriggerMuon++;

        p4mu = p4();
        // cout << "[Debug] This is the " << event << "th event, with evt_event() " << evt_event()
        //      << ", isTriggerMuon = " << isTriggerMuon << ", and nMuonCount = " << nMuonCount << ",\n"
        //      << "   while mupt = " << p4mu.pt() << ", and HLT_IsoMu20() = " << HLT_IsoMu20()
        //      << ", HLT_Mu17() = " << HLT_Mu17() << ", HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL() = " << HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL() << endl;
      }

      // start filling if last event
      if (event == nEventsTree-1 && nMuonCount == 1) {
        if (passes_POG_looseID()) {
          Fill1F(h_loose_mupt, p4mu.pt());
          Fill1F(h_loose_mueta, p4mu.eta());
          Fill1F(h_loose_muphi, p4mu.phi());
          if (isTriggerMuon == 1) {
            Fill1F(h_loose_trig_mupt, p4mu.pt());
            Fill1F(h_loose_trig_mueta, p4mu.eta());
            Fill1F(h_loose_trig_muphi, p4mu.phi());
          }
        } 
        if (passes_POG_mediumID()) {
          Fill1F(h_med_mupt, p4mu.pt());
          Fill1F(h_med_mueta, p4mu.eta());
          Fill1F(h_med_muphi, p4mu.phi());
          if (isTriggerMuon == 1) {
            Fill1F(h_med_trig_mupt, p4mu.pt());
            Fill1F(h_med_trig_mueta, p4mu.eta());
            Fill1F(h_med_trig_muphi, p4mu.phi());
          }
        } 
        if (passes_POG_tightID()) {
          Fill1F(h_tight_mupt, p4mu.pt());
          Fill1F(h_tight_mueta, p4mu.eta());
          Fill1F(h_tight_muphi, p4mu.phi());
          if (isTriggerMuon == 1) {
            Fill1F(h_tight_trig_mupt, p4mu.pt());
            Fill1F(h_tight_trig_mueta, p4mu.eta());
            Fill1F(h_tight_trig_muphi, p4mu.phi());
          }
        }
      }
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
  h_loose_mupt->Write();
  h_loose_mueta->Write();
  h_loose_muphi->Write();

  h_med_mupt->Write();
  h_med_mueta->Write();
  h_med_muphi->Write();

  h_tight_mupt->Write();
  h_tight_mueta->Write();
  h_tight_muphi->Write();

  h_loose_trig_mupt->Write();
  h_loose_trig_mueta->Write();
  h_loose_trig_muphi->Write();

  h_med_trig_mupt->Write();
  h_med_trig_mueta->Write();
  h_med_trig_muphi->Write();

  h_tight_trig_mupt->Write();
  h_tight_trig_mueta->Write();
  h_tight_trig_muphi->Write();

  file->Close();

  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;
  return 0;
}
