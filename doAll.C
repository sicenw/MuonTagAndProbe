{

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *ch = new TChain("t"); 
  // ch->Add("/nfs-7/userdata/leptonTree/v1.03TP/2015DSingleMuon.root");
  // ch->Add("/nfs-7/userdata/leptonTree/v1.03TP/2015DSingleMuonV4.root");
  // ch->Add("/nfs-7/userdata/leptonTree/v1.07TP/2015DSingleMuonV4.root");
  // ch->Add("/nfs-7/userdata/leptonTree/v2.00TP/DY_madgraph80X.root");
  // ch->Add("/nfs-7/userdata/leptonTree/v2.04TP/2016SingleEl.root");
  // ch->Add("/nfs-7/userdata/leptonTree/v5.01/2015DDoubleMuon.root");
  // ch->Add("/nfs-7/userdata/namin/forSicheng/2016BSingleMuon_80XLeptonTree.root");
  // ch->Add("/nfs-7/userdata/leptonTree/v2.00TP/2016SingleMu273590.root");
  // ch->Add("/nfs-7/userdata/leptonTree/v1.09/v1.09FR_80X/2016DoubleMuon.root");
  ch->Add("/home/users/sicheng/working/leptree/2016SingleMu.root");

  ScanChain(ch);
}
