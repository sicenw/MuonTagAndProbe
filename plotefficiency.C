#include "TStyle.h"
#include "TFile.h"

vector<TH1F*> getEfficiencyPlot(TFile* file, string triggerName, bool drawPlots = true) {
  
  TH1F* den_mupt  = (TH1F*) file->Get(Form("h_den_%s_mupt", triggerName.c_str()));
  TH1F* den_mueta = (TH1F*) file->Get(Form("h_den_%s_mueta", triggerName.c_str()));
  TH1F* den_muphi = (TH1F*) file->Get(Form("h_den_%s_muphi", triggerName.c_str()));
  TH1F* num_mupt  = (TH1F*) file->Get(Form("h_num_%s_mupt", triggerName.c_str()));
  TH1F* num_mueta = (TH1F*) file->Get(Form("h_num_%s_mueta", triggerName.c_str()));
  TH1F* num_muphi = (TH1F*) file->Get(Form("h_num_%s_muphi", triggerName.c_str()));

  TH1F* eff_mupt  = (TH1F*) num_mupt->Clone();
  TH1F* eff_mueta = (TH1F*) num_mueta->Clone();
  TH1F* eff_muphi = (TH1F*) num_muphi->Clone();

  eff_mupt ->SetName("eff_mupt");
  eff_mueta->SetName("eff_mueta");
  eff_muphi->SetName("eff_muphi");

  eff_mupt ->Divide(den_mupt);
  eff_mueta->Divide(den_mueta);
  eff_muphi->Divide(den_muphi);

  for (int i=1; i <= eff_mupt->GetNbinsX(); i++){
    float p = eff_mupt->GetBinContent(i);
    if (den_mupt->GetBinContent(i) == 0) eff_mupt->SetBinError(i, 0);
    else 
      eff_mupt->SetBinError(i, sqrt(p*(1-p)/den_mupt->GetBinContent(i)));
  }
  for (int i=1; i <= eff_mueta->GetNbinsX(); i++){
    float p = eff_mueta->GetBinContent(i);
    if (den_mueta->GetBinContent(i) == 0) eff_mueta->SetBinError(i, 0);
    else
      eff_mueta->SetBinError(i, sqrt(p*(1-p)/den_mueta->GetBinContent(i)));
  }
  for (int i=1; i <= eff_muphi->GetNbinsX(); i++){
    float p = eff_muphi->GetBinContent(i);
    if (den_muphi->GetBinContent(i) == 0) eff_muphi->SetBinError(i, 0);
    else
      eff_muphi->SetBinError(i, sqrt(p*(1-p)/den_muphi->GetBinContent(i)));
  }

  if (drawPlots) {
    TCanvas* c1 = new TCanvas;
    gStyle->SetOptStat(0);

    eff_mupt->SetTitle(Form("%s efficiency in muon p_{T}", triggerName.c_str()));
    eff_mupt->GetXaxis()->SetTitle("p_{T}");
    eff_mupt->Draw("PE");
    c1->SaveAs(Form("%s_eff_mupt.png", triggerName.c_str()));
    c1->Clear();
    eff_mueta->SetTitle(Form("%s efficiency in muon eta", triggerName.c_str()));
    eff_mueta->GetXaxis()->SetTitle("eta");
    eff_mueta->Draw("PE");
    c1->SaveAs(Form("%s_eff_mueta.png", triggerName.c_str()));
    c1->Clear();
    eff_muphi->SetTitle(Form("%s efficiency in muon phi", triggerName.c_str()));
    eff_muphi->GetXaxis()->SetTitle("phi");
    eff_muphi->Draw("PE");
    c1->SaveAs(Form("%s_eff_muphi.png", triggerName.c_str()));
    c1->Close();
  }

  vector<TH1F*> effplots;
  effplots.push_back(eff_mupt);
  effplots.push_back(eff_mueta);
  effplots.push_back(eff_muphi);

  return effplots;
}

int plotefficiency()
{
  TFile* file = new TFile("hists.root");

  getEfficiencyPlot(file, "IsoMu24");
  getEfficiencyPlot(file, "IsoTkMu24");

  return 0;
}
