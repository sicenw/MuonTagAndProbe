#include "TStyle.h"
#include "TFile.h"

vector<TH1F*> getEfficiencyPlot(TFile* file1, TFile* file2, string triggerName, string dirName, bool drawPlots = true) {
  vector<TH1F*> effplots;
  string prefix = triggerName + "/" + dirName + "/h_eff_" + triggerName;
  
  TH1F* num_mupt  = (TH1F*) file1->Get(Form("%s_mupt", prefix.c_str()));
  TH1F* num_mueta = (TH1F*) file1->Get(Form("%s_mueta", prefix.c_str()));
  TH1F* num_muphi = (TH1F*) file1->Get(Form("%s_muphi", prefix.c_str()));

  TH1F* den_mupt  = (TH1F*) file2->Get(Form("%s_mupt", prefix.c_str()));
  TH1F* den_mueta = (TH1F*) file2->Get(Form("%s_mueta", prefix.c_str()));
  TH1F* den_muphi = (TH1F*) file2->Get(Form("%s_muphi", prefix.c_str()));

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

    string titleTemp = dirName + " eff scale in " + triggerName;
    float tot_scale = num_mupt->Integral()/den_mupt->Integral();
    eff_mupt->SetTitle(Form("%s (tot. %.3f) in p_{T}", titleTemp.c_str(), tot_scale));
    eff_mupt->GetXaxis()->SetTitle("p_{T}");
    eff_mupt->GetYaxis()->SetRangeUser(0, 1.1);
    eff_mupt->Draw("PE");
    c1->SaveAs(Form("plot/%s_scale_mupt.png", triggerName.c_str()));
    c1->Clear();
    tot_scale = num_mueta->Integral()/den_mueta->Integral();
    eff_mueta->SetTitle(Form("%s (tot. %.3f) in eta", titleTemp.c_str(), tot_scale));
    eff_mueta->GetXaxis()->SetTitle("eta");
    eff_mueta->GetYaxis()->SetRangeUser(0, 1.1);
    eff_mueta->Draw("PE");
    c1->SaveAs(Form("plot/%s_scale_mueta.png", triggerName.c_str()));
    c1->Clear();
    tot_scale = num_muphi->Integral()/den_muphi->Integral();
    eff_muphi->SetTitle(Form("%s (tot. %.3f) in phi", titleTemp.c_str(), tot_scale));
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

  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoMu20", "trigeff");
  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoTkMu20", "trigeff");

  return 0;
}
