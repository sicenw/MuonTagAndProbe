{

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *ch = new TChain("t"); 
  ch->Add("/nfs-7/userdata/leptonTree/v1.03TP/2015DSingleMuon.root");
  ScanChain(ch); 
}