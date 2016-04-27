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

// LepTree
#include "LepTree.cc"

using namespace std;
using namespace lepTree;

int ScanChain( TChain* chain, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Example Histograms
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
  // samplehisto->SetDirectory(rootdir);
  TH1F *h_loose_mupt  = new TH1F("h_loose_mupt",  "Muon pt",  80, 0, 0);
  TH1F *h_loose_mueta = new TH1F("h_loose_mueta", "Muon eta", 80, -3, 3);
  TH1F *h_loose_muphi = new TH1F("h_loose_muphi", "Muon phi", 80, -3.5, 3.5);

  TH1F *h_med_mupt  = new TH1F("h_med_mupt",  "Muon pt",  80, 0, 0);
  TH1F *h_med_mueta = new TH1F("h_med_mueta", "Muon eta", 80, -3, 3);
  TH1F *h_med_muphi = new TH1F("h_med_muphi", "Muon phi", 80, -3.5, 3.5);

  TH1F *h_tight_mupt  = new TH1F("h_tight_mupt",  "Muon pt",  80, 0, 0);
  TH1F *h_tight_mueta = new TH1F("h_tight_mueta", "Muon eta", 80, -3, 3);
  TH1F *h_tight_muphi = new TH1F("h_tight_muphi", "Muon phi", 80, -3.5, 3.5);

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
      // for(unsigned int i=0; i<p4().size(); i++){
      if( abs(id()) != 13 ) continue;
      if (p4().pt() < 20.0) continue;
      if (fabs(p4().eta()) > 2.4) continue;
      if (RelIso03EA() > 0.2) continue;

      if (passes_POG_looseID()){
        h_loose_mupt->Fill(p4().pt());
        h_loose_mueta->Fill(p4().eta());
        h_loose_muphi->Fill(p4().phi());
      } 
      else if (passes_POG_mediumID()){
        h_med_mupt->Fill(p4().pt());
        h_med_mueta->Fill(p4().eta());
        h_med_muphi->Fill(p4().phi());
      } 
      else if (passes_POG_tightID()){
        h_tight_mupt->Fill(p4().pt());
        h_tight_mueta->Fill(p4().eta());
        h_tight_muphi->Fill(p4().phi());
      } 
        
      // }

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
