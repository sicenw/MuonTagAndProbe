// Usage:
// > root -b doAll.C

// C++
#include <iostream>
#include <vector>
#include <map>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TEfficiency.h"

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

enum histType {tag_mupt, tag_mueta, tag_muphi, den_mupt, den_mueta, den_muphi, num_mupt, num_mueta, num_muphi, den_invm, num_invm};

vector< map< histType,TH1F*> > creatMuonHists(vector<string> triggerNames, string suffix = ""){

  vector< map< histType,TH1F*> > triggerHists;

  for(unsigned int i=0; i < triggerNames.size(); i++){

    map<histType, TH1F*> muonHists;

    muonHists[tag_mupt]   = new TH1F(Form("h_tag_%s_mupt%s",   triggerNames[i].c_str(), suffix.c_str()), Form("Muon pt in tag_%s",  triggerNames[i].c_str()), 90, 0, 250);
    muonHists[tag_mueta]  = new TH1F(Form("h_tag_%s_mueta%s",  triggerNames[i].c_str(), suffix.c_str()), Form("Muon eta in tag_%s", triggerNames[i].c_str()), 60, -3, 3);
    muonHists[tag_muphi]  = new TH1F(Form("h_tag_%s_muphi%s",  triggerNames[i].c_str(), suffix.c_str()), Form("Muon phi in tag_%s", triggerNames[i].c_str()), 70, -3.5, 3.5);

    muonHists[den_mupt]   = new TH1F(Form("h_den_%s_mupt%s",   triggerNames[i].c_str(), suffix.c_str()), Form("Muon pt in %s",  triggerNames[i].c_str()), 90, 0, 250);
    muonHists[den_mueta]  = new TH1F(Form("h_den_%s_mueta%s",  triggerNames[i].c_str(), suffix.c_str()), Form("Muon eta in %s", triggerNames[i].c_str()), 60, -3, 3);
    muonHists[den_muphi]  = new TH1F(Form("h_den_%s_muphi%s",  triggerNames[i].c_str(), suffix.c_str()), Form("Muon phi in %s", triggerNames[i].c_str()), 70, -3.5, 3.5);

    muonHists[num_mupt]   = new TH1F(Form("h_num_%s_mupt%s",   triggerNames[i].c_str(), suffix.c_str()), Form("Muon pt in %s",  triggerNames[i].c_str()), 90, 0, 250);
    muonHists[num_mueta]  = new TH1F(Form("h_num_%s_mueta%s",  triggerNames[i].c_str(), suffix.c_str()), Form("Muon eta in %s", triggerNames[i].c_str()), 60, -3, 3);
    muonHists[num_muphi]  = new TH1F(Form("h_num_%s_muphi%s",  triggerNames[i].c_str(), suffix.c_str()), Form("Muon phi in %s", triggerNames[i].c_str()), 70, -3.5, 3.5);

    muonHists[den_invm] = new TH1F(Form("h_den_%s_invm%s", triggerNames[i].c_str(), suffix.c_str()), Form("InvM of the dilepton in %s", triggerNames[i].c_str()), 90, 0, 180);
    muonHists[num_invm] = new TH1F(Form("h_num_%s_invm%s", triggerNames[i].c_str(), suffix.c_str()), Form("InvM of the dilepton in %s", triggerNames[i].c_str()), 90, 0, 180);

    triggerHists.push_back(muonHists);
  }

  return triggerHists;
}

inline void writeEfficiencyPlots(map< histType,TH1F*>& histmap, string triggerName, TFile* file){
  for(unsigned int j=0; j<histmap.size(); j++)
    histmap[(histType) j]->Write();

  TH1F *h_eff_mupt  = (TH1F*) histmap[num_mupt] ->Clone();
  TH1F *h_eff_mueta = (TH1F*) histmap[num_mueta]->Clone();
  TH1F *h_eff_muphi = (TH1F*) histmap[num_muphi]->Clone();

  h_eff_mupt ->SetName(string(h_eff_mupt ->GetName()).replace(2, 3, "eff").c_str());
  h_eff_mueta->SetName(string(h_eff_mueta->GetName()).replace(2, 3, "eff").c_str());
  h_eff_muphi->SetName(string(h_eff_muphi->GetName()).replace(2, 3, "eff").c_str());

  h_eff_mupt ->Divide(histmap[den_mupt]);
  h_eff_mueta->Divide(histmap[den_mueta]);
  h_eff_muphi->Divide(histmap[den_muphi]);

  for (int i=1; i <= h_eff_mupt->GetNbinsX(); i++){
    float p = h_eff_mupt->GetBinContent(i);
    if (histmap[den_mupt]->GetBinContent(i) == 0) h_eff_mupt->SetBinError(i, 0);
    else
      h_eff_mupt->SetBinError(i, sqrt(p*(1-p)/histmap[den_mupt]->GetBinContent(i)));
  }
  for (int i=1; i <= h_eff_mueta->GetNbinsX(); i++){
    float p = h_eff_mueta->GetBinContent(i);
    if (histmap[den_mueta]->GetBinContent(i) == 0) h_eff_mueta->SetBinError(i, 0);
    else
      h_eff_mueta->SetBinError(i, sqrt(p*(1-p)/histmap[den_mueta]->GetBinContent(i)));
  }
  for (int i=1; i <= h_eff_muphi->GetNbinsX(); i++){
    float p = h_eff_muphi->GetBinContent(i);
    if (histmap[den_muphi]->GetBinContent(i) == 0) h_eff_muphi->SetBinError(i, 0);
    else
      h_eff_muphi->SetBinError(i, sqrt(p*(1-p)/histmap[den_muphi]->GetBinContent(i)));
  }

  h_eff_mupt->SetTitle(Form("%s efficiency in muon p_{T}", triggerName.c_str()));
  h_eff_mupt->GetXaxis()->SetTitle("p_{T}");
  h_eff_mupt->GetYaxis()->SetRangeUser(0, 1.1);

  h_eff_mueta->SetTitle(Form("%s efficiency in muon eta", triggerName.c_str()));
  h_eff_mueta->GetXaxis()->SetTitle("eta");
  h_eff_mueta->GetYaxis()->SetRangeUser(0, 1.1);

  h_eff_muphi->SetTitle(Form("%s efficiency in muon phi", triggerName.c_str()));
  h_eff_muphi->GetXaxis()->SetTitle("phi");
  h_eff_muphi->GetYaxis()->SetRangeUser(0, 1.1);

  h_eff_mupt ->Write();
  h_eff_mueta->Write();
  h_eff_muphi->Write();

  // Try the TEfficiency class
  TEfficiency* eff_mupt  = new TEfficiency(*histmap[num_mupt] , *histmap[den_mupt] );
  TEfficiency* eff_mueta = new TEfficiency(*histmap[num_mueta], *histmap[den_mueta]);
  TEfficiency* eff_muphi = new TEfficiency(*histmap[num_muphi], *histmap[den_muphi]);

  eff_mupt->SetTitle(Form("%s efficiency in muon p_{T}", triggerName.c_str()));
  eff_mupt->SetLineColor(kAzure+7);
  eff_mupt->SetMarkerColor(kAzure+7);

  eff_mueta->SetTitle(Form("%s efficiency in muon eta", triggerName.c_str()));
  eff_mueta->SetLineColor(kAzure+7);
  eff_mueta->SetMarkerColor(kAzure+7);

  eff_muphi->SetTitle(Form("%s efficiency in muon phi", triggerName.c_str()));
  eff_muphi->SetLineColor(kAzure+7);
  eff_muphi->SetMarkerColor(kAzure+7);

  eff_mupt ->Write();
  eff_mueta->Write();
  eff_muphi->Write();
}

vector<TBranch*> setupTriggerBranches(vector<string> triggerNames, TTree* tree){

  vector<TBranch*> trigBranches;

  for(unsigned int i=0; i<triggerNames.size(); i++){
    TBranch* trig_branch;
    trig_branch = tree->GetBranch(triggerNames[i].c_str());
    if (trig_branch == 0) {cerr << "Error: Cannot find branch under name: " << triggerNames[i] << endl; exit(-1);}

    trigBranches.push_back(trig_branch);
  }
  return trigBranches;
}

vector<TBranch*> setupTagTriggerBranches(vector<string> triggerNames, TTree* tree){

  vector<TBranch*> tagTrigBranches;

  for(unsigned int i=0; i<triggerNames.size(); i++){
    TBranch* tag_trig_branch;
    tag_trig_branch = tree->GetBranch(Form("tag_%s", triggerNames[i].c_str()));
    if (tag_trig_branch == 0) {cerr << "Error: Cannot find branch under name: tag_" << triggerNames[i] << endl; exit(-1);}

    tagTrigBranches.push_back(tag_trig_branch);
  }
  return tagTrigBranches;
}

inline int getTriggerValue(TBranch* trig_branch, int event){

  if (trig_branch == 0) {cerr << "Error: getTriggerValue: TBranch* is empty!" << endl; return 0;}

  int trig_value = 0;
  trig_branch->SetAddress(&trig_value);
  trig_branch->GetEntry(event);

  return trig_value;
}

inline void fillTagMuonHists(map< histType,TH1F* >& histmap, float ptcut = 30){
  Fill1F(histmap[tag_mupt], tag_p4().pt());
  if (tag_p4().pt() > ptcut){
    Fill1F(histmap[tag_mueta], tag_p4().eta());
    Fill1F(histmap[tag_muphi], tag_p4().phi());
  }
}

inline void fillDenMuonHists(map< histType,TH1F* >& histmap, float ptcut = 30){
  Fill1F(histmap[den_mupt], p4().pt());
  if (p4().pt() > ptcut){
    Fill1F(histmap[den_mueta], p4().eta());
    Fill1F(histmap[den_muphi], p4().phi());
    Fill1F(histmap[den_invm], dilep_mass());
  }
}

inline void fillNumMuonHists(map< histType,TH1F* >& histmap, float ptcut = 30){
  Fill1F(histmap[num_mupt], p4().pt());
  if (p4().pt() > ptcut){
    Fill1F(histmap[num_mueta], p4().eta());
    Fill1F(histmap[num_muphi], p4().phi());
    Fill1F(histmap[num_invm], dilep_mass());
  }
}

inline void fillProbeMuonHists(map< histType,TH1F* >& histmap, TBranch* trig_branch, int event, float ptcut = 30){
  fillDenMuonHists(histmap, ptcut);
  if (getTriggerValue(trig_branch, event) > 0)
    fillNumMuonHists(histmap, ptcut);
}

int ScanChain( TChain* chain, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Example Histograms
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
  // samplehisto->SetDirectory(rootdir);

  TH1F *h_muonCount = new TH1F("h_muonCount", "Number of Muons in this event", 90, 0, 5);

  // MuonTagAndProbe: define the trigger to check here
  vector<string> triggerNames;
  triggerNames.push_back("HLT_IsoMu24");
  triggerNames.push_back("HLT_IsoTkMu24");
  triggerNames.push_back("HLT_IsoMu20");
  triggerNames.push_back("HLT_IsoTkMu20");
  // triggerNames.push_back("HLT_Ele27_eta2p1_WPLoose_Gsf");

  // Separate vector to store the pt cut to be used for eta and phi
  vector<float> triggerPtCuts(triggerNames.size(), 30);
  triggerPtCuts[2] = 25;
  triggerPtCuts[3] = 25;

  vector< map< histType,TH1F*> > muonHists = creatMuonHists(triggerNames);
  vector< map< histType,TH1F*> > muonHists1 = creatMuonHists(triggerNames, "_1");
  vector< map< histType,TH1F*> > muonHists2 = creatMuonHists(triggerNames, "_2");
  vector< map< histType,TH1F*> > muonHists3 = creatMuonHists(triggerNames, "_3");

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

    vector<TBranch*> trigBranches = setupTriggerBranches(triggerNames, tree);
    vector<TBranch*> tagTrigBranches = setupTagTriggerBranches(triggerNames, tree);

    // Loop over Events in current file
    if (nEventsTotal >= nEventsChain) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for (unsigned int event = 0; event < nEventsTree; ++event) {

      // Get Event Content
      if( nEventsTotal >= nEventsChain ) continue;
      if(fast) tree->LoadTree(event);
      t.GetEntry(event);
      ++nEventsTotal;

      // Progress
      LepTree::progress( nEventsTotal, nEventsChain );

      // Analysis Code

      if (evt_run() < 273423) continue;    // Get runs after fixing the L1 interface problem

      int nevt = evt_event();
      if (nevt != evt_num) {
        h_muonCount->Fill(nMuonCount);
        nMuonCount = 0;
        isTriggerMuon = 0;
      }

      // --- New Tag & Probe ---
      if (abs(id()) != 13) continue;
      if (p4().pt() < 15) continue;
      if (fabs(p4().eta()) > 2.4 ) continue;
      if (charge()*tag_charge() > 0) continue;
      if (tag_p4().pt() < 25) continue;
      if (tag_RelIso03EA() > 0.1) continue;
      if (dilep_mass() < 75 || dilep_mass() > 105) continue;

      ++nMuonCount;
      // if (nMuonCount > 2) continue;

      for (unsigned int i=0; i<triggerNames.size(); i++){
        if (getTriggerValue(tagTrigBranches[i], event) <= 0) continue;

        fillTagMuonHists(muonHists[i],  triggerPtCuts[i]);
        if (RelIso03EA() < 0.15 && passes_POG_tightID())
          fillProbeMuonHists(muonHists[i], trigBranches[i], event, triggerPtCuts[i]);

        fillTagMuonHists(muonHists1[i], triggerPtCuts[i]);
        fillTagMuonHists(muonHists2[i], triggerPtCuts[i]);
        fillTagMuonHists(muonHists3[i], triggerPtCuts[i]);

        fillDenMuonHists(muonHists1[i], triggerPtCuts[i]);
        if (RelIso03EA() < 0.1 && passes_POG_tightID())
          fillNumMuonHists(muonHists1[i], triggerPtCuts[i]);

        if (RelIso03EA() < 0.1) {
          fillDenMuonHists(muonHists2[i], triggerPtCuts[i]);
          if (passes_POG_tightID())
            fillNumMuonHists(muonHists2[i], triggerPtCuts[i]);
        }

        if (passes_POG_tightID()) {
          fillDenMuonHists(muonHists3[i], triggerPtCuts[i]);
          if (RelIso03EA() < 0.1)
            fillNumMuonHists(muonHists3[i], triggerPtCuts[i]);
        }
      }

      // End of Analysis Code
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

  TFile* outfile = new TFile("hists.root", "RECREATE");

  for (unsigned int i=0; i<triggerNames.size(); i++) {
    TDirectory * dir = (TDirectory*) outfile->mkdir(triggerNames[i].c_str());
    dir->cd();
    TDirectory * dir2 = (TDirectory*) dir->mkdir("trigeff");
    dir2->cd();
    writeEfficiencyPlots(muonHists[i], triggerNames[i], outfile);
    dir2 = (TDirectory*) dir->mkdir("1.ID+ISO");
    dir2->cd();
    writeEfficiencyPlots(muonHists1[i], triggerNames[i], outfile);
    dir2 = (TDirectory*) dir->mkdir("2.ID");
    dir2->cd();
    writeEfficiencyPlots(muonHists2[i], triggerNames[i], outfile);
    dir2 = (TDirectory*) dir->mkdir("3.ISO");
    dir2->cd();
    writeEfficiencyPlots(muonHists3[i], triggerNames[i], outfile);
  }

  h_muonCount->Write();

  outfile->Close();

  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;
  return 0;
}
