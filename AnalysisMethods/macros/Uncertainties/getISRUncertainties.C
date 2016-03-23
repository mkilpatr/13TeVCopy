#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "TTree.h"
#endif

// Procedure: The recommendation for ISR uncertainties is 0% for pT < 400 GeV, 15% for pT 400-600 GeV, 30% for pT > 600 GeV where pT is the recoil system pT
//

void getISRUncertainties(TString sname = "T2tb_300_100",
                         const int fileindex = -1,
                         const bool isMC = true,
                         const TString fname = "/eos/uscms/store/user/vdutta/13TeV/trees/031216/T2tb_300_100_tree.root",
                         const TString outputdir = "plots_bkgest/sysvars",
                         const TString fileprefix = "root://cmseos:1094/",
                         const TString json = "")
{

  const bool    dolepcr = false;
  const TString lumistr = "2.318";

  gSystem->mkdir(outputdir,true);

  TString basewgt = dolepcr ? lumistr + "*weight*truePUWeight*btagWeight*btagFastSimWeight*leptnpweight" : lumistr + "*weight*truePUWeight*btagWeight*btagFastSimWeight*leptnpweight*lepvetoweight";

  map<TString,TString> sel;

  sel["trig"]         = "passjson && passdijetmet && j2pt>75 && met>250 && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["vetoes"]       = " && nvetolep==0 && (nvetotau==0 || (ismc && npromptgentau>0))";
  sel["njets"]        = " && njets>=5 && nbjets>=1 && nlbjets>=2";
  sel["dphij123"]     = " && dphij12met>0.5 && dphij3met>0.5 && dphij4met>0.5";
  sel["lepcrsel"]     = " && nvetolep>0 && mtlepmet<100";
  sel["sr"]           = sel["trig"] + sel["vetoes"]   + sel["njets"]    + sel["dphij123"];
  sel["lepcr"]        = sel["trig"] + sel["lepcrsel"] + sel["njets"]    + sel["dphij123"];
  sel["lowmt"]        = " && mtcsv12met<=175";
  sel["highmt"]       = " && mtcsv12met>175";
  sel["mednj"]        = " && njets>=5 && njets<7";
  sel["highnj"]       = " && njets>=7";
  sel["nb1"]          = " && nbjets==1";
  sel["nb2"]          = " && nbjets>=2";
  sel["nt0"]          = " && ncttstd==0";
  sel["nt1"]          = " && ncttstd>0";
  sel["lowmt_mednj"]        = sel["lepcr"] + sel["lowmt"] + sel["mednj"];
  sel["lowmt_highnj"]       = sel["lepcr"] + sel["lowmt"] + sel["highnj"];
  sel["highmt_mednj_nt0"]   = sel["lepcr"] + sel["highmt"] + sel["mednj"] + sel["nt0"];
  sel["highmt_highnj_nt0"]  = sel["lepcr"] + sel["highmt"] + sel["highnj"] + sel["nt0"];
  sel["highmt_nt1"]         = sel["lepcr"] + sel["highmt"] + sel["nt1"];
  sel["lowmt_mednj_nb1"]        = sel["sr"] + sel["lowmt"] + sel["mednj"] + sel["nb1"];
  sel["lowmt_mednj_nb2"]        = sel["sr"] + sel["lowmt"] + sel["mednj"] + sel["nb2"];
  sel["lowmt_highnj_nb1"]       = sel["sr"] + sel["lowmt"] + sel["highnj"] + sel["nb1"];
  sel["lowmt_highnj_nb2"]       = sel["sr"] + sel["lowmt"] + sel["highnj"] + sel["nb2"];
  sel["highmt_mednj_nt0_nb1"]   = sel["sr"] + sel["highmt"] + sel["mednj"] + sel["nt0"] + sel["nb1"];
  sel["highmt_mednj_nt0_nb2"]   = sel["sr"] + sel["highmt"] + sel["mednj"] + sel["nt0"] + sel["nb2"];
  sel["highmt_highnj_nt0_nb1"]  = sel["sr"] + sel["highmt"] + sel["highnj"] + sel["nt0"] + sel["nb1"];
  sel["highmt_highnj_nt0_nb2"]  = sel["sr"] + sel["highmt"] + sel["highnj"] + sel["nt0"] + sel["nb2"];
  sel["highmt_nt1_nb1"]         = sel["sr"] + sel["highmt"] + sel["nt1"] + sel["nb1"];
  sel["highmt_nt1_nb2"]         = sel["sr"] + sel["highmt"] + sel["nt1"] + sel["nb2"];

  vector<TString> srbins = {"lowmt_mednj_nb1", "lowmt_mednj_nb2", "lowmt_highnj_nb1", "lowmt_highnj_nb2", "highmt_mednj_nt0_nb1", "highmt_mednj_nt0_nb2", "highmt_highnj_nt0_nb1", "highmt_highnj_nt0_nb2", "highmt_nt1_nb1", "highmt_nt1_nb2"};
  if(dolepcr)
    srbins = {"lowmt_mednj", "lowmt_highnj", "highmt_mednj_nt0", "highmt_highnj_nt0", "highmt_nt1"};

  const int nbins = 5;
  double metbins[] = {250.,300.,400.,500.,600.,1000.};

  vector<TString> binlabels = {"5_1_0_0", "5_2_0_0", "7_1_0_0", "7_2_0_0", "5_1_0_175", "5_2_0_175", "7_1_0_175", "7_2_0_175", "5_1_1_175", "5_2_1_175"};
  if(dolepcr)
    binlabels = {"5_1_0_0", "7_1_0_0", "5_1_0_175", "7_1_0_175", "5_1_1_175"};

  TFile* outfile = 0;
  if(dolepcr) outfile = new TFile(outputdir + "/" + sname + "_onelepcr_isrvars.root","RECREATE");
  else        outfile = new TFile(outputdir + "/" + sname + "_isrvars.root","RECREATE");

  FILE * txtfile = 0;
  if(dolepcr) txtfile = fopen(outputdir + "/" + sname + "_onelepcr_isruncs.txt","w");
  else        txtfile = fopen(outputdir + "/" + sname + "_isruncs.txt","w");

  TString fullname = fileprefix+fname;
  printf("Opening %s\n",fullname.Data());
  TFile* infile = TFile::Open(fullname,"READ");
  assert(infile);
  TTree* intree = (TTree*)infile->Get("Events");
  assert(intree);

  int isrbin = 0;
  for(auto srbin : srbins) {
    TString hname = "nominal_"+srbin+"_"+sname;
    TString hnameup = "isrup_"+srbin+"_"+sname;
    TString hnamedown = "isrdown_"+srbin+"_"+sname;

    TH1F *h=0, *hvarup=0, *hvardown=0;

    if(srbin == "highmt_nt1") {
      h = new TH1F(hname,"",1,250.,1000.);
      hvarup = new TH1F(hnameup,"",1,250.,1000.);
      hvardown = new TH1F(hnamedown,"",1,250.,1000.);
    } else {
      h = new TH1F(hname,"",nbins,metbins);
      hvarup = new TH1F(hnameup,"",nbins,metbins);
      hvardown = new TH1F(hnamedown,"",nbins,metbins);
    }

    h->Sumw2();
    hvarup->Sumw2();
    hvardown->Sumw2();

    TString basecutstr = basewgt+"*("+sel[srbin]+")";
    TString upcutstr = basewgt+"*("+sel[srbin]+" && distoppt<400) + " + basewgt+"*1.15*("+sel[srbin]+" && distoppt>400 && distoppt<600) + " + basewgt+"*1.30*("+sel[srbin]+" && distoppt>600)";
    TString downcutstr = basewgt+"*("+sel[srbin]+" && distoppt<400) + " + basewgt+"*0.85*("+sel[srbin]+" && distoppt>400 && distoppt<600) + " + basewgt+"*0.70*("+sel[srbin]+" && distoppt>600)";

    intree->Project(hname,"met",basecutstr);
    Plot::addOverFlow(h);

    intree->Project(hnameup,"met",upcutstr);
    Plot::addOverFlow(hvarup);

    intree->Project(hnamedown,"met",downcutstr);
    Plot::addOverFlow(hvardown);

    outfile->cd();
    h->Write();
    hvarup->Write();
    hvardown->Write();

    TGraphAsymmErrors* gr = new TGraphAsymmErrors(h);

    for(int ibin = 0; ibin < gr->GetN(); ++ibin) {
      double varup = hvarup->GetBinContent(ibin+1)-h->GetBinContent(ibin+1);
      double vardown = hvardown->GetBinContent(ibin+1)-h->GetBinContent(ibin+1);
      printf("bin_%s%d_%s\t%5.3f\t%5.3f\t(%5.3f)\n", dolepcr ? "onelepcr_" : "", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), varup, vardown, h->GetBinContent(ibin+1));
      gr->SetPointEYhigh(ibin, varup);
      gr->SetPointEYlow(ibin, -1.0*vardown);

      double maxvar = 1.0 + (max(fabs(varup),fabs(vardown)))/h->GetBinContent(ibin+1);

      printf("bin_%s%d_%s\tisrunc\t%s\t%4.3f\n", dolepcr ? "onelepcr_" : "", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), sname.Data(), maxvar);
      fprintf(txtfile, "bin_%s%d_%s\tisrunc\t%s\t%4.3f\n", dolepcr ? "onelepcr_" : "", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), sname.Data(), maxvar);
    }

    outfile->cd();
    gr->Write(TString(h->GetName()).Append("_isrupdown"));

    isrbin++;
  }

  fclose(txtfile);
  outfile->Close();

}
