{

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *ch = new TChain("t"); 
  // ch->Add("/nfs-7/userdata/leptonTree/v1.03TP/2015DSingleMuon.root");
  // ch->Add("/nfs-7/userdata/leptonTree/v1.03TP/2015DSingleMuonV4.root");
  ch->Add("/nfs-7/userdata/leptonTree/v1.07TP/2015DSingleMuonV4.root");
  // ch->Add("/nfs-7/userdata/leptonTree/v1.07TP/DY_madgraph.root");
  // ch->Add("/nfs-7/userdata/leptonTree/v5.01/2015DDoubleMuon.root");
  ScanChain(ch); 
}
