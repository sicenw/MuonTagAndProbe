#include "TStyle.h"
#include "TFile.h"
// #include "../../tas/Software/dataMCplotMaker/dataMCplotMaker.h"
#include "TH2F.h"

int plotefficiency()
{
  TFile* file = new TFile("hists.root");

  TH1F* IsoMu24_den_mupt  = (TH1F*) file->Get("h_den_IsoMu24_mupt");
  TH1F* IsoMu24_den_mueta = (TH1F*) file->Get("h_den_IsoMu24_mueta");
  TH1F* IsoMu24_den_muphi = (TH1F*) file->Get("h_den_IsoMu24_muphi");
  TH1F* IsoMu24_num_mupt  = (TH1F*) file->Get("h_num_IsoMu24_mupt");
  TH1F* IsoMu24_num_mueta = (TH1F*) file->Get("h_num_IsoMu24_mueta");
  TH1F* IsoMu24_num_muphi = (TH1F*) file->Get("h_num_IsoMu24_muphi");

  TH1F *IsoMu24_eff_mupt  = (TH1F*) IsoMu24_num_mupt->Clone();
  TH1F *IsoMu24_eff_mueta = (TH1F*) IsoMu24_num_mueta->Clone();
  TH1F *IsoMu24_eff_muphi = (TH1F*) IsoMu24_num_muphi->Clone();

  IsoMu24_eff_mupt ->SetName("IsoMu24_eff_mupt");
  IsoMu24_eff_mueta->SetName("IsoMu24_eff_mueta");
  IsoMu24_eff_muphi->SetName("IsoMu24_eff_muphi");

  IsoMu24_eff_mupt ->Divide(IsoMu24_den_mupt);
  IsoMu24_eff_mueta->Divide(IsoMu24_den_mueta);
  IsoMu24_eff_muphi->Divide(IsoMu24_den_muphi);

  for (int i=1; i <= IsoMu24_eff_mupt->GetNbinsX(); i++){
    float p = IsoMu24_eff_mupt->GetBinContent(i);
    if (IsoMu24_den_mupt->GetBinContent(i) == 0) IsoMu24_eff_mupt->SetBinError(i, 0);
    else 
      IsoMu24_eff_mupt->SetBinError(i, sqrt(p*(1-p)/IsoMu24_den_mupt->GetBinContent(i)));
    // // debug
    // cout << "This is " << i << "th bin, with p = " << p << ", and the error: " 
    //      << sqrt(p*(1-p)/IsoMu24_den_mupt->GetBinContent(i)) << ", num = " << IsoMu24_num_mupt->GetBinContent(i)
    //      << ", den = " << IsoMu24_den_mupt->GetBinContent(i) << endl;
  }
  for (int i=1; i <= IsoMu24_eff_mueta->GetNbinsX(); i++){
    float p = IsoMu24_eff_mueta->GetBinContent(i);
    if (IsoMu24_den_mueta->GetBinContent(i) == 0) IsoMu24_eff_mueta->SetBinError(i, 0);
    else
      IsoMu24_eff_mueta->SetBinError(i, sqrt(p*(1-p)/IsoMu24_den_mueta->GetBinContent(i)));
  }
  for (int i=1; i <= IsoMu24_eff_muphi->GetNbinsX(); i++){
    float p = IsoMu24_eff_muphi->GetBinContent(i);
    if (IsoMu24_den_muphi->GetBinContent(i) == 0) IsoMu24_eff_muphi->SetBinError(i, 0);
    else
      IsoMu24_eff_muphi->SetBinError(i, sqrt(p*(1-p)/IsoMu24_den_muphi->GetBinContent(i)));
  }

  // IsoMu24_eff_mupt->SetMarkerStyle(20);
  TCanvas* c1 = new TCanvas;
  gStyle->SetOptStat(0);

  IsoMu24_eff_mupt->SetTitle("Muon p_{T} efficiency");
  IsoMu24_eff_mupt->GetXaxis()->SetTitle("p_{T}");
  IsoMu24_eff_mupt->Draw("PE");
  c1->SaveAs("IsoMu24_eff_mupt.png");
  c1->Clear();
  IsoMu24_eff_mueta->SetTitle("Muon eta efficiency");
  IsoMu24_eff_mueta->GetXaxis()->SetTitle("eta");
  IsoMu24_eff_mueta->Draw("PE");
  c1->SaveAs("IsoMu24_eff_mueta.png");
  c1->Clear();
  IsoMu24_eff_muphi->SetTitle("Muon phi efficiency");
  IsoMu24_eff_muphi->GetXaxis()->SetTitle("phi");
  IsoMu24_eff_muphi->Draw("PE");
  c1->SaveAs("IsoMu24_eff_muphi.png");
  c1->Clear();

  return 0;
}
