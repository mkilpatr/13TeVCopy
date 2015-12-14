#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"

TH1F* getHist(TTree* tree, TString sel, TString name, int nbins, double* bins, TString wgt = "1.0", TString var = "met")
{
  TH1F* hist = new TH1F(name,"",nbins,bins);
  hist->Sumw2();
  tree->Project(name.Data(),var,wgt+"*("+sel+")");
  return hist;
}

vector<TH1F*> getRatios(vector<TH1F*> num_hists, vector<TH1F*> den_hists, bool correlated = false, TString debugname = "")
{
  bool debug = debugname != "";
  if(debug) cout << "\n\nPrinting info for " << debugname << ":" << endl;
  vector<TH1F*> ratios;
  for(unsigned int i = 0; i < num_hists.size(); ++i) {
    TH1F* h1 = num_hists[i];
    TH1F* h2 = den_hists[i];
    if(debug) cout << "\nDividing " << h1->GetName() << " (" << h1->Integral(0, h1->GetNbinsX()+1) << ") by " << h2->GetName() << " (" << h2->Integral(0, h2->GetNbinsX()+1) << ")" << endl;
    if(correlated) {
      TH1F* ratio = (TH1F*)h1->Clone(TString(h1->GetName()).Append("_").Append(h2->GetName()).Append("_ratio"));
      ratio->Divide(h1, h2, 1, 1, "B");
      ratios.push_back(ratio);
      if(debug) cout << "\nCalculating ratio with correlated errors" << endl;
      if(debug) cout << "Ratio: " << ratio->Integral(0, ratio->GetNbinsX()+1) << endl;
      if(debug) cout << "saving " << ratio->GetName() << endl;
    }
    else {
      TH1F* ratio = (TH1F*)h1->Clone(TString(h1->GetName()).Append("_").Append(h2->GetName()).Append("_ratio"));
      ratio->Divide(h1, h2);
      ratios.push_back(ratio);
      if(debug) cout << "\nCalculating ratio with uncorrelated errors" << endl;
      if(debug) cout << "Ratio: " << ratio->Integral(0, ratio->GetNbinsX()+1) << endl;
      if(debug) cout << "saving " << ratio->GetName() << endl;
    }
  }
  return ratios;
}

void getLepCorrections(const TString inputdir = "../0LepSearchRegions/trees_0lep_1211",
                       const TString outputdir = "plots_lepeffs_1211/lepcorrs",
                       const TString lumistr = "2137",
                       const bool    plotlog = false,
                       const TString format = "png")
{

  gSystem->mkdir(outputdir, true);

  map<TString,TString> sel;
  sel["trig"]       = "passjson && passdijetmet && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["base"]       = sel["trig"] + " && met>200 && j2pt>75 && njets>=2 && nlbjets>=2 && nbjets>=1";
  sel["inclcr"]     = sel["base"] + " && dphij12met>0.4 && dphij3met>0.4";
  sel["inclcrnomu"] = sel["inclcr"] + " && nvetomu==0";
  sel["inclcrnoemu"]= sel["inclcrnomu"] + " && nvetolele==0";
  sel["genele"]     = " && ngoodgenele>0";
  sel["genmu"]      = " && ngoodgenmu>0";
  sel["gentau"]     = " && npromptgentau>0";
  sel["nogenele"]   = " && ngoodgenele==0";
  sel["nogenmu"]    = " && ngoodgenmu==0";
  sel["nogentau"]   = " && npromptgentau==0";
  sel["mucr"]       = sel["inclcr"] + " && nvetomu>0";
  sel["elecr"]      = sel["inclcr"] + " && nvetomu==0 && nvetolele>0";
  sel["taucr"]      = sel["inclcr"] + " && nvetomu==0 && nvetolele==0 && nvetotau>0";
  sel["hpstaucr"]   = sel["inclcr"] + " && nvetomu==0 && nvetolele==0 && nvetohpstaus>0";

  TFile* datafile = new TFile(inputdir+"/data_tree.root");
  TTree* datatree = (TTree*)datafile->Get("Events");
  assert(datatree);

  TFile* mcfile = new TFile(inputdir+"/mc_tree.root");
  TTree* mctree = (TTree*)mcfile->Get("Events");
  assert(mctree);

  TString wgtexpr = lumistr + "*0.001*weight*truePUWeight";

  vector<TH1F*> data_incl, data_sel, data_sel_nobins;
  vector<TH1F*> mc_incl, mc_sel;
  vector<TH1F*> mc_genlep_incl, mc_genlep_sel, mc_genlep_sel_nobins;
  vector<TH1F*> mc_nogenlep_sel, mc_nogenlep_sel_nobins;

  double ptbins[]     = {0., 20., 1000.};
  double hpsptbins[]  = {20., 1000.};
  double muetabins[]  = {0., 0.8, 1.6, 2.4};
  double eletabins[]  = {0., 0.8, 1.5, 2.4};
  double tauetabins[] = {0., 0.8, 1.5, 2.4};
  double actbins[]    = {0., 1., 3., 5., 10., 20.};
  double dummybins[]  = {0., 100.};

  int nbins = 2, nhpsbins = 1;

  data_incl.push_back       (getHist(datatree, sel["inclcr"],      "data_incl_mu", 1, dummybins));
  data_incl.push_back       (getHist(datatree, sel["inclcrnomu"],  "data_incl_ele", 1, dummybins));
  data_incl.push_back       (getHist(datatree, sel["inclcrnoemu"], "data_incl_tau", 1, dummybins));
  data_incl.push_back       (getHist(datatree, sel["inclcrnoemu"], "data_incl_hpstau", 1, dummybins));
  data_sel.push_back        (getHist(datatree, sel["mucr"],     "data_sel_mu", nbins, ptbins, "1.0", "mupt[0]"));
  data_sel.push_back        (getHist(datatree, sel["elecr"],    "data_sel_ele", nbins, ptbins, "1.0", "elept[0]"));
  data_sel.push_back        (getHist(datatree, sel["taucr"],    "data_sel_tau", nbins, ptbins, "1.0", "trackpt[0]"));
  data_sel.push_back        (getHist(datatree, sel["hpstaucr"], "data_sel_hpstau", nhpsbins, hpsptbins, "1.0", "taupt[0]"));
  data_sel_nobins.push_back (getHist(datatree, sel["mucr"],     "data_sel_nobins_mu", 1, dummybins));
  data_sel_nobins.push_back (getHist(datatree, sel["elecr"],    "data_sel_nobins_ele", 1, dummybins));
  data_sel_nobins.push_back (getHist(datatree, sel["taucr"],    "data_sel_nobins_tau", 1, dummybins));
  data_sel_nobins.push_back (getHist(datatree, sel["hpstaucr"], "data_sel_nobins_hpstau", 1, dummybins));
  mc_incl.push_back         (getHist(mctree, sel["inclcr"],      "mc_incl_mu", 1, dummybins, wgtexpr));
  mc_incl.push_back         (getHist(mctree, sel["inclcrnomu"],  "mc_incl_ele", 1, dummybins, wgtexpr));
  mc_incl.push_back         (getHist(mctree, sel["inclcrnoemu"], "mc_incl_tau", 1, dummybins, wgtexpr));
  mc_incl.push_back         (getHist(mctree, sel["inclcrnoemu"], "mc_incl_hpstau", 1, dummybins, wgtexpr));
  mc_sel.push_back          (getHist(mctree, sel["mucr"],     "mc_sel_mu", nbins, ptbins, wgtexpr, "mupt[0]"));
  mc_sel.push_back          (getHist(mctree, sel["elecr"],    "mc_sel_ele", nbins, ptbins, wgtexpr, "elept[0]"));
  mc_sel.push_back          (getHist(mctree, sel["taucr"],    "mc_sel_tau", nbins, ptbins, wgtexpr, "trackpt[0]"));
  mc_sel.push_back          (getHist(mctree, sel["hpstaucr"], "mc_sel_hpstau", nhpsbins, hpsptbins, wgtexpr, "taupt[0]"));
  mc_genlep_incl.push_back  (getHist(mctree, sel["inclcr"] + sel["genmu"],       "mc_genlep_incl_mu", 1, dummybins, wgtexpr));
  mc_genlep_incl.push_back  (getHist(mctree, sel["inclcrnomu"] + sel["genele"],  "mc_genlep_incl_ele", 1, dummybins, wgtexpr));
  mc_genlep_incl.push_back  (getHist(mctree, sel["inclcrnoemu"] + sel["gentau"], "mc_genlep_incl_tau", 1, dummybins, wgtexpr));
  mc_genlep_incl.push_back  (getHist(mctree, sel["inclcrnoemu"] + sel["gentau"], "mc_genlep_incl_hpstau", 1, dummybins, wgtexpr));
  mc_genlep_sel.push_back   (getHist(mctree, sel["mucr"] + sel["genmu"],      "mc_genlep_sel_mu", nbins, ptbins, wgtexpr, "mupt[0]"));
  mc_genlep_sel.push_back   (getHist(mctree, sel["elecr"] + sel["genele"],    "mc_genlep_sel_ele", nbins, ptbins, wgtexpr, "elept[0]"));
  mc_genlep_sel.push_back   (getHist(mctree, sel["taucr"] + sel["gentau"],    "mc_genlep_sel_tau", nbins, ptbins, wgtexpr, "trackpt[0]"));
  mc_genlep_sel.push_back   (getHist(mctree, sel["hpstaucr"] + sel["gentau"], "mc_genlep_sel_hpstau", nhpsbins, hpsptbins, wgtexpr, "taupt[0]"));
  mc_genlep_sel_nobins.push_back   (getHist(mctree, sel["mucr"] + sel["genmu"],      "mc_genlep_sel_nobins_mu", 1, dummybins, wgtexpr));
  mc_genlep_sel_nobins.push_back   (getHist(mctree, sel["elecr"] + sel["genele"],    "mc_genlep_sel_nobins_ele", 1, dummybins, wgtexpr));
  mc_genlep_sel_nobins.push_back   (getHist(mctree, sel["taucr"] + sel["gentau"],    "mc_genlep_sel_nobins_tau", 1, dummybins, wgtexpr));
  mc_genlep_sel_nobins.push_back   (getHist(mctree, sel["hpstaucr"] + sel["gentau"], "mc_genlep_sel_nobins_hpstau", 1, dummybins, wgtexpr));
  mc_nogenlep_sel.push_back        (getHist(mctree, sel["mucr"] + sel["nogenmu"],      "mc_nogenlep_sel_mu", nbins, ptbins, wgtexpr, "mupt[0]"));
  mc_nogenlep_sel.push_back        (getHist(mctree, sel["elecr"] + sel["nogenele"],    "mc_nogenlep_sel_ele", nbins, ptbins, wgtexpr, "elept[0]"));
  mc_nogenlep_sel.push_back        (getHist(mctree, sel["taucr"] + sel["nogentau"],    "mc_nogenlep_sel_tau", nbins, ptbins, wgtexpr, "trackpt[0]"));
  mc_nogenlep_sel.push_back        (getHist(mctree, sel["hpstaucr"] + sel["nogentau"], "mc_nogenlep_sel_hpstau", nhpsbins, hpsptbins, wgtexpr, "taupt[0]"));
  mc_nogenlep_sel_nobins.push_back (getHist(mctree, sel["mucr"] + sel["nogenmu"],      "mc_nogenlep_sel_nobins_mu", 1, dummybins, wgtexpr));
  mc_nogenlep_sel_nobins.push_back (getHist(mctree, sel["elecr"] + sel["nogenele"],    "mc_nogenlep_sel_nobins_ele", 1, dummybins, wgtexpr));
  mc_nogenlep_sel_nobins.push_back (getHist(mctree, sel["taucr"] + sel["nogentau"],    "mc_nogenlep_sel_nobins_tau", 1, dummybins, wgtexpr));
  mc_nogenlep_sel_nobins.push_back (getHist(mctree, sel["hpstaucr"] + sel["nogentau"], "mc_nogenlep_sel_nobins_hpstau", 1, dummybins, wgtexpr));

  vector<TH1F*> sf = getRatios(data_incl, mc_incl, false, "SF");
  vector<TH1F*> data_eff = getRatios(data_sel_nobins, data_incl, true, "DATA_EFF");
  vector<TH1F*> mc_eff = getRatios(mc_genlep_sel_nobins, mc_incl, true, "MC_EFF");
  vector<TH1F*> mc_fake = getRatios(mc_nogenlep_sel_nobins, mc_genlep_sel_nobins, false, "MC_FAKE");
  vector<TH1F*> mc_geneff = getRatios(mc_genlep_sel_nobins, mc_genlep_incl, true, "MC_GEN_EFF");
  vector<TH1F*> eff_term = getRatios(data_sel, mc_genlep_sel, false, "EFF_TERM");
  vector<TH1F*> fake_term = getRatios(mc_nogenlep_sel, mc_genlep_sel, "FAKE_TERM");

  TFile* outfile = new TFile(outputdir+"/lepcorrs.root","RECREATE");

  vector<TH1F*> corr;
  TH1F* lepCor = new TH1F("LEP", "LEP", 9, 0, 9); lepCor->Sumw2();
  lepCor->SetBinContent(1,0); /// default bin
  lepCor->SetBinError(1,0);
  int nlepcorbin = 2;

  for(unsigned int i = 0; i < sf.size(); ++i) {
    TH1F* h = (TH1F*)eff_term[i]->Clone(TString::Format("corr_%d",i));
    h->Scale(1.0/sf[0]->Integral(0, sf[0]->GetNbinsX()+1));
    h->Add(fake_term[i], -1);
    corr.push_back(h);
    for(int ibin = 1; ibin < h->GetNbinsX()+1; ++ibin) {
      cout << "\nCorrection in bin " << ibin << ": " << h->GetBinContent(ibin) << " +/- " << h->GetBinError(ibin) << endl;
      lepCor->SetBinContent(nlepcorbin, h->GetBinContent(ibin));
      lepCor->SetBinError(nlepcorbin, h->GetBinError(ibin));
      nlepcorbin++;
    }
    outfile->cd();
    sf[i]->Write();
    eff_term[i]->Write();
    fake_term[i]->Write();
    data_incl[i]->Write();
    mc_incl[i]->Write();
    mc_genlep_incl[i]->Write();
    data_sel[i]->Write();
    data_sel_nobins[i]->Write();
    mc_genlep_sel[i]->Write();
    mc_genlep_sel_nobins[i]->Write();
    mc_nogenlep_sel[i]->Write();
    mc_nogenlep_sel_nobins[i]->Write();

    // Cross check overall efficiencies/SFs
    // NB: this assumes the normalization SF from the specific CRl for each lepton flavor. So this number will be different from the numbers we save for the actual correction. But it's a good cross-check to get an idea of the actual data/MC efficiency SF
    TH1F* h2 = (TH1F*)data_eff[i]->Clone(TString::Format("corr2_%d",i));
    h2->Divide(mc_eff[i]);
    h2->Add(mc_fake[i], -1);
    corr.push_back(h2);
    double mcefferr = 0, correrr = 0;
    double mceff = mc_geneff[i]->IntegralAndError(0, mc_geneff[i]->GetNbinsX()+1, mcefferr);
    double corrl = h2->IntegralAndError(0, h2->GetNbinsX()+1, correrr);
    cout << "\n------------------------------------" << endl;
    cout << "MC eff (integrated): " << mceff << " +/- " << mcefferr << endl;
    cout << "Correction (integrated): " << corrl << " +/- " << correrr << endl;
  }

  lepCor->SetBinContent(nlepcorbin,1); /// fake bin
  lepCor->SetBinError(nlepcorbin,0);

  TFile* lepcorfile = new TFile("lepCorr.root","RECREATE");
  lepcorfile->cd();
  lepCor->Write("LEP");
  lepcorfile->Close();

  outfile->Close();

}
