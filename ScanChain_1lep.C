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

  TH1F *h_loose_tag_mupt  = new TH1F("h_loose_tag_mupt",  "Tag Muon pt",  90, 0, 150);
  TH1F *h_loose_tag_mueta = new TH1F("h_loose_tag_mueta", "Tag Muon eta", 90, -3, 3);
  TH1F *h_loose_tag_muphi = new TH1F("h_loose_tag_muphi", "Tag Muon phi", 90, -3.5, 3.5);

  TH1F *h_muonCount = new TH1F("h_muonCount", "Number of Muons in this event", 90, 0, 5);

  TH1F *h_test_tag_muphi1 = new TH1F("h_test_tag_muphi1", "Tag Muon phi", 90, -3.5, 3.5);
  TH1F *h_test_tag_muphi2 = new TH1F("h_test_tag_muphi2", "Tag Muon phi", 90, -3.5, 3.5);
  TH1F *h_test_tag_muphi3 = new TH1F("h_test_tag_muphi3", "Tag Muon phi", 90, -3.5, 3.5);

  TH1F *h_loose_invM    = new TH1F("h_loose_invM",   "InvM between tag & probe",  90, 0, 180);
  TH1F *h_loose_dilepM  = new TH1F("h_loose_dilepM", "InvM of the dilepton",      90, 0, 180);

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

  TH1F *h_tag_mupt  = new TH1F("h_tag_mupt",  "Tag Muon pt",  90, 0, 150);
  TH1F *h_tag_mueta = new TH1F("h_tag_mueta", "Tag Muon eta", 60, -3, 3);
  TH1F *h_tag_muphi = new TH1F("h_tag_muphi", "Tag Muon phi", 70, -3.5, 3.5);

  TH1F *h_tag_trig_mupt  = new TH1F("h_tag_trig_mupt",  "Tag Muon pt",  90, 0, 150);
  TH1F *h_tag_trig_mueta = new TH1F("h_tag_trig_mueta", "Tag Muon eta", 60, -3, 3);
  TH1F *h_tag_trig_muphi = new TH1F("h_tag_trig_muphi", "Tag Muon phi", 70, -3.5, 3.5);

  TH1F *h_loose_tag_trig_mupt  = new TH1F("h_loose_tag_trig_mupt",  "Tag Muon pt",  90, 0, 150);
  TH1F *h_loose_tag_trig_mueta = new TH1F("h_loose_tag_trig_mueta", "Tag Muon eta", 60, -3, 3);
  TH1F *h_loose_tag_trig_muphi = new TH1F("h_loose_tag_trig_muphi", "Tag Muon phi", 70, -3.5, 3.5);

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
    int passLooseID  = false;
    int passMediumID = false;
    int passTightID  = false;
    
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
      if (debug && event > 200) break;          // debug

      int nevt = evt_event();
      if (nevt != evt_num) {
        evt_num = nevt;
        Fill1F(h_muonCount, nMuonCount);

        if(nMuonCount == 1) {
          if (passLooseID) {
            Fill1F(h_loose_mupt, p4mu.pt());
            Fill1F(h_loose_mueta, p4mu.eta());
            Fill1F(h_loose_muphi, p4mu.phi());
            Fill1F(h_loose_tag_mupt, tag_p4mu.pt());
            Fill1F(h_loose_tag_mueta, tag_p4mu.eta());
            Fill1F(h_loose_tag_muphi, tag_p4mu.phi());
            Fill1F(h_loose_invM, (p4mu+tag_p4mu).M());
            if (isTriggerMuon == 1) {
              Fill1F(h_loose_trig_mupt, p4mu.pt());
              Fill1F(h_loose_trig_mueta, p4mu.eta());
              Fill1F(h_loose_trig_muphi, p4mu.phi());
            }
          } 
          if (passMediumID) {
            Fill1F(h_med_mupt, p4mu.pt());
            Fill1F(h_med_mueta, p4mu.eta());
            Fill1F(h_med_muphi, p4mu.phi());
            if (isTriggerMuon == 1) {
              Fill1F(h_med_trig_mupt, p4mu.pt());
              Fill1F(h_med_trig_mueta, p4mu.eta());
              Fill1F(h_med_trig_muphi, p4mu.phi());
            }
          } 
          if (passTightID) {
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

      // if (event > 34 && event < 45 )
      //   cout << "[Debug] This is the " << event << "th event, with evt_event() " << evt_event()
      //        << ", isTriggerMuon = " << isTriggerMuon << ", and nMuonCount = " << nMuonCount << ",\n"
      //        << "    while mupt = " << p4().pt() << "    while mueta = " << p4().eta() << "    while muphi = " << p4().phi()
      //        << "\n    RelIso03EA = " <<  RelIso03EA() << ", passes_POG_looseID = " << passes_POG_looseID() 
      //        << "\n    tag_mupt = " << tag_p4().pt() << "    while mueta = " << tag_p4().eta() << "    while muphi = " << tag_p4().phi()
      //        << "\n    HLT_IsoMu20() = " << HLT_IsoMu20() << ", dilep_pt = " << dilep_p4().pt() << ", dilep_mass = " << dilep_mass() << endl << endl;


      Fill1F(h_tag_mupt, tag_p4().pt());
      Fill1F(h_tag_mueta, tag_p4().eta());
      Fill1F(h_tag_muphi, tag_p4().phi());
      if (HLT_IsoMu20() > 0) {
        Fill1F(h_tag_trig_mupt, tag_p4().pt());
        Fill1F(h_tag_trig_mueta, tag_p4().eta());
        Fill1F(h_tag_trig_muphi, tag_p4().phi());
      }

      if (abs(id()) == 13 && p4().pt() > 20.0 && fabs(p4().eta()) < 2.4 && RelIso03EA() < 0.2 && passes_POG_looseID() > 0){
        nMuonCount++;

        if (nMuonCount == 2) h_test_tag_muphi1->Fill(p4mu.phi());

        if (probe_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_LeadingLeg > 0) isTriggerMuon++;
        passLooseID  = passes_POG_looseID();
        passMediumID = passes_POG_mediumID();
        passTightID  = passes_POG_tightID();

        p4mu = p4();
        tag_p4mu = tag_p4();

        if (nMuonCount == 2) h_test_tag_muphi2->Fill(p4mu.phi());
        else if (nMuonCount > 2) h_test_tag_muphi3->Fill(p4mu.phi());

        if (debug)
          cout << "[Debug] This is the " << event << "th event, with evt_event() " << evt_event()
               << ", isTriggerMuon = " << isTriggerMuon << ", and nMuonCount = " << nMuonCount << ",\n"
               << "    while mupt = " << p4().pt() << ", tag_mupt = " << tag_p4().pt() << ", and HLT_IsoMu20() = " << HLT_IsoMu20() << endl << endl;
        // << ", HLT_Mu17() = " << HLT_Mu17() << ", HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL() = " << HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL() << endl;
      }

      // start filling if last event
      if (event == nEventsTree-1 && nMuonCount == 1) {
        if (passLooseID) {
          Fill1F(h_loose_mupt, p4mu.pt());
          Fill1F(h_loose_mueta, p4mu.eta());
          Fill1F(h_loose_muphi, p4mu.phi());
          Fill1F(h_loose_tag_mupt, tag_p4mu.pt());
          Fill1F(h_loose_tag_mueta, tag_p4mu.eta());
          Fill1F(h_loose_tag_muphi, tag_p4mu.phi());
          Fill1F(h_loose_invM, (p4mu+tag_p4mu).M());
          if (isTriggerMuon == 1) {
            Fill1F(h_loose_trig_mupt, p4mu.pt());
            Fill1F(h_loose_trig_mueta, p4mu.eta());
            Fill1F(h_loose_trig_muphi, p4mu.phi());
          }
        } 
        if (passMediumID) {
          Fill1F(h_med_mupt, p4mu.pt());
          Fill1F(h_med_mueta, p4mu.eta());
          Fill1F(h_med_muphi, p4mu.phi());
          if (isTriggerMuon == 1) {
            Fill1F(h_med_trig_mupt, p4mu.pt());
            Fill1F(h_med_trig_mueta, p4mu.eta());
            Fill1F(h_med_trig_muphi, p4mu.phi());
          }
        } 
        if (passTightID) {
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

  h_loose_tag_mupt->Write();
  h_loose_tag_mueta->Write();
  h_loose_tag_muphi->Write();

  h_test_tag_muphi1->Write();
  h_test_tag_muphi2->Write();
  h_test_tag_muphi3->Write();

  h_muonCount->Write();
  h_loose_invM->Write();

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

  h_tag_mupt->Write(); 
  h_tag_mueta->Write();
  h_tag_muphi->Write();
  h_tag_trig_mupt->Write(); 
  h_tag_trig_mueta->Write();
  h_tag_trig_muphi->Write();
  // h_loose_tag_trig_mupt->Write();
  // h_loose_tag_trig_mueta->Write();
  // h_loose_tag_trig_muphi->Write();

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
