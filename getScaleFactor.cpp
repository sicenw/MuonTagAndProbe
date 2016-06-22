#include "TStyle.h"
#include "TFile.h"

vector<TH1F*> getEfficiencyPlot(TFile* file1, TFile* file2, string triggerName, string dirName, string suffix = "", bool drawPlots = true) {
  vector<TH1F*> effplots;
  string prefix = triggerName + "/" + dirName + "/h_eff_" + triggerName;
  
  TH1F* num_mupt  = (TH1F*) file1->Get(Form("%s_mupt%s", prefix.c_str(), suffix.c_str()));
  TH1F* num_mueta = (TH1F*) file1->Get(Form("%s_mueta%s", prefix.c_str(), suffix.c_str()));
  TH1F* num_muphi = (TH1F*) file1->Get(Form("%s_muphi%s", prefix.c_str(), suffix.c_str()));

  TH1F* den_mupt  = (TH1F*) file2->Get(Form("%s_mupt%s", prefix.c_str(), suffix.c_str()));
  TH1F* den_mueta = (TH1F*) file2->Get(Form("%s_mueta%s", prefix.c_str(), suffix.c_str()));
  TH1F* den_muphi = (TH1F*) file2->Get(Form("%s_muphi%s", prefix.c_str(), suffix.c_str()));

  if (num_mupt == 0) {
    cout << "What?!! " << prefix << endl;
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

    string titleTemp = dirName + " eff scale in " + triggerName;
    float tot_scale = num_mupt->Integral()/den_mupt->Integral();
    eff_mupt->SetTitle(Form("%s (tot. %.3f) in p_{T}", titleTemp.c_str(), tot_scale));
    eff_mupt->GetXaxis()->SetTitle("p_{T}");
    eff_mupt->GetYaxis()->SetRangeUser(0, 1.1);
    eff_mupt->Draw("PE");
    c1->SaveAs(Form("plot/%s_%s_scale_mupt.pdf", triggerName.c_str(), dirName.c_str()));
    c1->Clear();
    tot_scale = num_mueta->Integral()/den_mueta->Integral();
    eff_mueta->SetTitle(Form("%s (tot. %.3f) in eta", titleTemp.c_str(), tot_scale));
    eff_mueta->GetXaxis()->SetTitle("eta");
    eff_mueta->GetYaxis()->SetRangeUser(0, 1.1);
    eff_mueta->Draw("PE");
    c1->SaveAs(Form("plot/%s_%s_scale_mueta.pdf", triggerName.c_str(), dirName.c_str()));
    c1->Clear();
    tot_scale = num_muphi->Integral()/den_muphi->Integral();
    eff_muphi->SetTitle(Form("%s (tot. %.3f) in phi", titleTemp.c_str(), tot_scale));
    eff_muphi->GetXaxis()->SetTitle("phi");
    eff_muphi->GetYaxis()->SetRangeUser(0, 1.1);
    eff_muphi->Draw("PE");
    c1->SaveAs(Form("plot/%s_%s_scale_muphi.pdf", triggerName.c_str(), dirName.c_str()));
    c1->Close();
  }

  effplots.push_back(eff_mupt);
  effplots.push_back(eff_mueta);
  effplots.push_back(eff_muphi);

  return effplots;
}

int getScaleFactor()
{
  TFile* f_data = new TFile("hists/hists_2fb.root");
  TFile* f_mcdy = new TFile("hists/hists_DY2.00TP.root");

  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoMu20",   "trigeff");
  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoTkMu20", "trigeff");

  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoMu20",   "ID+ISO", "_1");
  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoTkMu20", "ID+ISO", "_1");

  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoMu20",   "ID", "_2");
  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoTkMu20", "ID", "_2");

  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoMu20",   "ISO", "_3");
  getEfficiencyPlot(f_data, f_mcdy, "HLT_IsoTkMu20", "ISO", "_3");

  return 0;
}
