#include "TStyle.h"
#include "TFile.h"

TH1F* getEffPlot(TH1F* h_num, TH1F* h_den, float xbins[], int nbins, string histName) {
  TH1F *s_num = new TH1F(string(h_num->GetName()).replace(0, 1, "s").c_str(), h_num->GetTitle(), nbins, xbins);
  TH1F *s_den = new TH1F(string(h_den->GetName()).replace(0, 1, "s").c_str(), h_den->GetTitle(), nbins, xbins);

  for (int i = 1; i <= h_num->GetNbinsX(); ++i) {
    s_num->Fill(fabs(h_num->GetBinCenter(i)), h_num->GetBinContent(i));
    s_den->Fill(fabs(h_den->GetBinCenter(i)), h_den->GetBinContent(i));
  }

  TH1F* s_eff = (TH1F*) s_num->Clone(histName.c_str());
  s_eff->Divide(s_den);

  for (int i=1; i <= s_eff->GetNbinsX(); ++i){
    float p = s_eff->GetBinContent(i);
    if (s_den->GetBinContent(i) == 0) s_eff->SetBinError(i, 0);
    else
      s_eff->SetBinError(i, sqrt(p*(1-p)/s_den->GetBinContent(i)));
  }

  delete s_num;
  delete s_den;
  return s_eff;
}

TH1F* getScalePlot(TFile* f_data, TFile* f_mcdy, string triggerName, string dirName, string type, string suffix, float xbins[], int nbins, bool drawPlots = true) {
  string prefix_num = triggerName + "/" + dirName + "/h_num_" + triggerName;
  string prefix_den = triggerName + "/" + dirName + "/h_den_" + triggerName;
  string prefix_eff = "s_eff_" + triggerName;
  suffix = "mu" + type + suffix;

  TH1F* num_num = (TH1F*) f_data->Get(Form("%s_%s", prefix_num.c_str(), suffix.c_str()));
  TH1F* num_den = (TH1F*) f_data->Get(Form("%s_%s", prefix_den.c_str(), suffix.c_str()));
  TH1F* num_eff = getEffPlot(num_num, num_den, xbins, nbins, prefix_eff + "_data_" + suffix);
  
  TH1F* den_num = (TH1F*) f_mcdy->Get(Form("%s_%s", prefix_num.c_str(), suffix.c_str()));
  TH1F* den_den = (TH1F*) f_mcdy->Get(Form("%s_%s", prefix_den.c_str(), suffix.c_str()));
  TH1F* den_eff = getEffPlot(den_num, den_den, xbins, nbins, prefix_eff + "_mcdy_" + suffix);

  TH1F* eff_eff = (TH1F*) num_eff->Clone((prefix_eff + "_scale_" + suffix).c_str());
  eff_eff->Divide(den_eff);
  for (int i=1; i <= eff_eff->GetNbinsX(); i++)
    eff_eff->SetBinError(i, num_eff->GetBinError(i));

  if (drawPlots) {
    TCanvas* c1 = new TCanvas;
    TLegend* leg = new TLegend(0.55,0.17,0.75,0.37);
    gStyle->SetOptStat(0);
    eff_eff->SetLineColor(kGreen);
    eff_eff->SetMarkerColor(kGreen);
    num_eff->SetLineColor(kBlue);
    num_eff->SetMarkerColor(kBlue);
    den_eff->SetLineColor(kRed);
    den_eff->SetMarkerColor(kRed);
    eff_eff->SetMarkerStyle(20);
    eff_eff->SetMarkerSize(0.5);
    num_eff->SetMarkerStyle(20);
    num_eff->SetMarkerSize(0.5);
    den_eff->SetMarkerStyle(20);
    den_eff->SetMarkerSize(0.5);
    eff_eff->GetXaxis()->SetTitle(type.c_str());
    eff_eff->GetYaxis()->SetRangeUser(0, 1.1);
    leg->AddEntry(eff_eff, "data/MC");
    leg->AddEntry(num_eff, "data");
    leg->AddEntry(den_eff, "MC");
    eff_eff->Draw("PE");
    num_eff->Draw("PEsame");
    den_eff->Draw("PEsame");
    leg->Draw("same");
    c1->SetGrid();
    c1->SaveAs(Form("plot/%s_%s_scale_mu%s.pdf", triggerName.c_str(), dirName.c_str(), type.c_str()));
    c1->Close();
  }

  return eff_eff;
}

vector<TH1F*> getScalePlots(TFile* file1, TFile* file2, string triggerName, string dirName, string suffix = "", bool drawPlots = true) {
  vector<TH1F*> scalePlots;
  string prefix_num = triggerName + "/" + dirName + "/h_num_" + triggerName;
  string prefix_den = triggerName + "/" + dirName + "/h_den_" + triggerName;

  float ptbins[5] = {20, 30, 40, 50, 250};
  float etabins[6] = {0, 0.8, 1.4, 1.6, 2, 2.5};

  TH1F* scale_mupt = getScalePlot(file1, file2, triggerName, dirName, "pt", suffix, ptbins, 4, drawPlots);
  TH1F* scale_mueta = getScalePlot(file1, file2, triggerName, dirName, "eta", suffix, etabins, 5, drawPlots);
  
  scalePlots.push_back(scale_mupt);
  scalePlots.push_back(scale_mueta);
  // scalePlots.push_back(scale_muphi);
  return scalePlots;
}


vector<TH1F*> getEfficiencyPlots(TFile* file1, TFile* file2, string triggerName, string dirName, string suffix = "", bool drawPlots = true) {
  vector<TH1F*> effPlots;
  string prefix = triggerName + "/" + dirName + "/h_eff_" + triggerName;
  
  TH1F* num_mupt  = (TH1F*) file1->Get(Form("%s_mupt%s", prefix.c_str(), suffix.c_str()));
  TH1F* num_mueta = (TH1F*) file1->Get(Form("%s_mueta%s", prefix.c_str(), suffix.c_str()));
  TH1F* num_muphi = (TH1F*) file1->Get(Form("%s_muphi%s", prefix.c_str(), suffix.c_str()));

  TH1F* den_mupt  = (TH1F*) file2->Get(Form("%s_mupt%s", prefix.c_str(), suffix.c_str()));
  TH1F* den_mueta = (TH1F*) file2->Get(Form("%s_mueta%s", prefix.c_str(), suffix.c_str()));
  TH1F* den_muphi = (TH1F*) file2->Get(Form("%s_muphi%s", prefix.c_str(), suffix.c_str()));

  if (num_mupt == 0) {
    cout << "What?!! " << prefix << endl;
    return effPlots;
  }

  TH1F* eff_mupt  = (TH1F*) num_mupt->Clone();
  TH1F* eff_mueta = (TH1F*) num_mueta->Clone();
  TH1F* eff_muphi = (TH1F*) num_muphi->Clone();

  eff_mupt ->SetName(Form("%s_scale_mupt", dirName.c_str()));
  eff_mueta->SetName(Form("%s_scale_mueta", dirName.c_str()));
  eff_muphi->SetName(Form("%s_scale_muphi", dirName.c_str()));

  eff_mupt ->Divide(den_mupt);
  eff_mueta->Divide(den_mueta);
  eff_muphi->Divide(den_muphi);

  for (int i=1; i <= eff_mupt->GetNbinsX(); i++)
    eff_mupt->SetBinError(i, num_mupt->GetBinError(i));

  for (int i=1; i <= eff_mueta->GetNbinsX(); i++)
    eff_mueta->SetBinError(i, num_mueta->GetBinError(i));

  for (int i=1; i <= eff_muphi->GetNbinsX(); i++)
    eff_muphi->SetBinError(i, num_muphi->GetBinError(i));

  string titleTemp = dirName + " eff scale in " + triggerName;
  float tot_scale = num_mupt->Integral()/den_mupt->Integral();
  eff_mupt->SetTitle(Form("%s (tot. %.3f) in p_{T}", titleTemp.c_str(), tot_scale));
  eff_mupt->GetXaxis()->SetTitle("p_{T}");
  eff_mupt->GetYaxis()->SetRangeUser(0, 1.1);
  tot_scale = num_mueta->Integral()/den_mueta->Integral();
  eff_mueta->SetTitle(Form("%s (tot. %.3f) in eta", titleTemp.c_str(), tot_scale));
  eff_mueta->GetXaxis()->SetTitle("eta");
  eff_mueta->GetYaxis()->SetRangeUser(0, 1.1);
  tot_scale = num_muphi->Integral()/den_muphi->Integral();
  eff_muphi->SetTitle(Form("%s (tot. %.3f) in phi", titleTemp.c_str(), tot_scale));
  eff_muphi->GetXaxis()->SetTitle("phi");
  eff_muphi->GetYaxis()->SetRangeUser(0, 1.1);

  if (drawPlots) {
    TCanvas* c1 = new TCanvas;
    gStyle->SetOptStat(0);

    TLegend* leg = new TLegend(0.55,0.17,0.75,0.37);
    eff_mupt->SetLineColor(kGreen);
    eff_mupt->SetMarkerColor(kGreen);
    num_mupt->SetLineColor(kBlue);
    num_mupt->SetMarkerColor(kBlue);
    den_mupt->SetLineColor(kRed);
    den_mupt->SetMarkerColor(kRed);
    leg->AddEntry(eff_mupt, "data/MC");
    leg->AddEntry(num_mupt, "data");
    leg->AddEntry(den_mupt, "MC");
    eff_mupt->Draw("PE");
    num_mupt->Draw("PEsame");
    den_mupt->Draw("PEsame");
    leg->Draw("same");
    c1->SaveAs(Form("plot/%s_%s_scale_mupt.pdf", triggerName.c_str(), dirName.c_str()));
    c1->Clear();
    eff_mueta->SetLineColor(kGreen);
    eff_mueta->SetMarkerColor(kGreen);
    num_mueta->SetLineColor(kBlue);
    num_mueta->SetMarkerColor(kBlue);
    den_mueta->SetLineColor(kRed);
    den_mueta->SetMarkerColor(kRed);
    leg->Clear();
    leg->AddEntry(eff_mueta, "data/MC");
    leg->AddEntry(num_mueta, "data");
    leg->AddEntry(den_mueta, "MC");
    eff_mueta->Draw("PE");
    num_mueta->Draw("PEsame");
    den_mueta->Draw("PEsame");
    leg->Draw("same");
    c1->SaveAs(Form("plot/%s_%s_scale_mueta.pdf", triggerName.c_str(), dirName.c_str()));
    c1->Clear();
    // eff_muphi->SetLineColor(kGreen);
    // eff_muphi->SetMarkerColor(kGreen);
    // num_muphi->SetLineColor(kBlue);
    // num_muphi->SetMarkerColor(kBlue);
    // den_muphi->SetLineColor(kRed);
    // den_muphi->SetMarkerColor(kRed);
    // leg->Clear();
    // leg->AddEntry(eff_muphi, "data/MC");
    // leg->AddEntry(num_muphi, "data");
    // leg->AddEntry(den_muphi, "MC");
    // eff_muphi->Draw("PE");
    // num_muphi->Draw("PEsame");
    // den_muphi->Draw("PEsame");
    // leg->Draw("same");
    // c1->SaveAs(Form("plot/%s_%s_scale_muphi.pdf", triggerName.c_str(), dirName.c_str()));
    c1->Close();
  }

  effPlots.push_back(eff_mupt);
  effPlots.push_back(eff_mueta);
  effPlots.push_back(eff_muphi);

  return effPlots;
}

int getScaleFactor()
{
  TFile* f_data = new TFile("hists/hists_2fb.root");
  TFile* f_mcdy = new TFile("hists/hists_DY2.00TP.root");

  // vector<string> triggerNames{"HLT_IsoMu20", "HLT_IsoTkMu20"};
  // vector<string> dirNames{"trigeff", "ID+ISO", "ID", "ISO"};
  // vector<string> suffixs{"", "_1", "_2", "_3"};
  vector<string> triggerNames{"HLT_IsoMu20"};
  vector<string> dirNames{"trigeff"};
  vector<string> suffixs{""};

  TFile* outfile; 
  bool outputHists = false;
  if (outputHists) outfile = new TFile("hists/eff_scalings.root", "RECREATE");

  for (unsigned int i = 0; i < triggerNames.size(); ++i) {
    for (unsigned int j = 0; j < dirNames.size(); ++j) {
      // vector<TH1F*> hScales = getEfficiencyPlots(f_data, f_mcdy, triggerNames[i], dirNames[j], suffixs[j], true);
      vector<TH1F*> hScales = getScalePlots(f_data, f_mcdy, triggerNames[i], dirNames[j], suffixs[j], true);

      if (outputHists) {
        TDirectory* dir = (TDirectory*) outfile->Get(triggerNames[i].c_str());
        if (dir == 0) dir = (TDirectory*) outfile->mkdir(triggerNames[i].c_str());
        dir->cd();
        for (auto it = hScales.begin(); it != hScales.end(); ++it)
          (*it)->Write();
      }
    }
  }

  return 0;
}
