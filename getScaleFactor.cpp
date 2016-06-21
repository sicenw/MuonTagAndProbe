#include "TStyle.h"
#include "TFile.h"

vector<TH1F*> getEfficiencyPlot(TFile* file1, TFile* file2, string triggerName, string dirName = "", bool drawPlots = true) {
  vector<TH1F*> effplots;
  
  TH1F* num_mupt  = (TH1F*) file1->Get(Form("%sh_eff_%s_mupt", dirName.c_str(), triggerName.c_str()));
  TH1F* num_mueta = (TH1F*) file1->Get(Form("%sh_eff_%s_mueta", dirName.c_str(), triggerName.c_str()));
  TH1F* num_muphi = (TH1F*) file1->Get(Form("%sh_eff_%s_muphi", dirName.c_str(), triggerName.c_str()));

  TH1F* den_mupt  = (TH1F*) file2->Get(Form("%sh_eff_%s_mupt", dirName.c_str(), triggerName.c_str()));
  TH1F* den_mueta = (TH1F*) file2->Get(Form("%sh_eff_%s_mueta", dirName.c_str(), triggerName.c_str()));
  TH1F* den_muphi = (TH1F*) file2->Get(Form("%sh_eff_%s_muphi", dirName.c_str(), triggerName.c_str()));

  if (num_muphi == 0) {
    cout << "What?!! " << Form("%sh_eff_%s_mupt", dirName.c_str(), triggerName.c_str()) << endl; 
    return effplots;
  }

  TH1F* eff_mupt  = (TH1F*) num_mupt->Clone();
  TH1F* eff_mueta = (TH1F*) num_mueta->Clone();
  TH1F* eff_muphi = (TH1F*) num_muphi->Clone();

  eff_mupt ->SetName("scale_mupt");
  eff_mueta->SetName("scale_mueta");
  eff_muphi->SetName("scale_muphi");

  eff_mupt ->Divide(den_mupt);
  eff_mueta->Divide(den_mueta);
  eff_muphi->Divide(den_muphi);

  for (int i=1; i <= eff_mupt->GetNbinsX(); i++)
    eff_mupt->SetBinError(i, num_mupt->GetBinError(i));

  for (int i=1; i <= eff_mueta->GetNbinsX(); i++)
    eff_mueta->SetBinError(i, num_mueta->GetBinError(i));

  for (int i=1; i <= eff_muphi->GetNbinsX(); i++)
    eff_muphi->SetBinError(i, num_muphi->GetBinError(i));


  if (drawPlots) {
    TCanvas* c1 = new TCanvas;
    gStyle->SetOptStat(0);

    eff_mupt->SetTitle(Form("%s scale of muon eff in p_{T}", triggerName.c_str()));
    eff_mupt->GetXaxis()->SetTitle("p_{T}");
    eff_mupt->GetYaxis()->SetRangeUser(0, 1.1);
    eff_mupt->Draw("PE");
    c1->SaveAs(Form("plot/%s_scale_mupt.png", triggerName.c_str()));
    c1->Clear();
    eff_mueta->SetTitle(Form("%s scale of muon eff in eta", triggerName.c_str()));
    eff_mueta->GetXaxis()->SetTitle("eta");
    eff_mueta->GetYaxis()->SetRangeUser(0, 1.1);
    eff_mueta->Draw("PE");
    c1->SaveAs(Form("plot/%s_scale_mueta.png", triggerName.c_str()));
    c1->Clear();
    eff_muphi->SetTitle(Form("%s scale of muon eff in phi", triggerName.c_str()));
    eff_muphi->GetXaxis()->SetTitle("phi");
    eff_muphi->GetYaxis()->SetRangeUser(0, 1.1);
    eff_muphi->Draw("PE");
    c1->SaveAs(Form("plot/%s_scale_muphi.png", triggerName.c_str()));
    c1->Close();
  }

  effplots.push_back(eff_mupt);
  effplots.push_back(eff_mueta);
  effplots.push_back(eff_muphi);

  return effplots;
}

int getScaleFactor()
{
  TFile* f_data = new TFile("hists/hists_idiso_2fb.root");
  TFile* f_mcdy = new TFile("hists/hists_idiso_DY2.00TP.root");

  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoMu20", "HLT_IsoMu20/trigeff/");
  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoTkMu20", "HLT_IsoTkMu20/trigeff/");

  return 0;
}
