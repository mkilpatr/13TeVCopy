#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "TTree.h"
#endif

// Procedure: The first 9 entries in the systweights vector correspond to various permutations of varying mu_R / mu_F up/down by a factor of 2. We take the envelope of these variations to get the scale uncertainty. In order to factor out the effects on the cross section, yields should be normalized to the inclusive yield. To be done properly, input trees should have no pre-selection
//

void fillSysVars(TH1F* hvar, TH1F* hsysup, TH1F* hsysdown) {

  for(int ibin = 1; ibin < hvar->GetNbinsX()+1; ++ibin) {
    if(hvar->GetBinContent(ibin) > hsysup->GetBinContent(ibin)) hsysup->SetBinContent(ibin, hvar->GetBinContent(ibin));
    if(hvar->GetBinContent(ibin) < hsysdown->GetBinContent(ibin)) hsysdown->SetBinContent(ibin, hvar->GetBinContent(ibin));
  }

}

void getScaleUncertainties(TString sname = "T2tb_300_100",
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
  double metbins[nbins+1] = {250.,300.,400.,500.,600.,1000.};

  vector<TString> binlabels = {"5_1_0_0", "5_2_0_0", "7_1_0_0", "7_2_0_0", "5_1_0_175", "5_2_0_175", "7_1_0_175", "7_2_0_175", "5_1_1_175", "5_2_1_175"};
  if(dolepcr)
    binlabels = {"5_1_0_0", "7_1_0_0", "5_1_0_175", "7_1_0_175", "5_1_1_175"};

  TFile* outfile = 0;
  if(dolepcr) outfile = new TFile(outputdir + "/" + sname + "_onelepcr_scalevars.root","RECREATE");
  else        outfile = new TFile(outputdir + "/" + sname + "_scalevars.root","RECREATE");

  FILE * txtfile = 0;
  if(dolepcr) txtfile = fopen(outputdir + "/" + sname + "_onelepcr_scaleuncs.txt","w");
  else        txtfile = fopen(outputdir + "/" + sname + "_scaleuncs.txt","w");

  TString fullname = fileprefix+fname;
  printf("Opening %s\n",fullname.Data());
  TFile* infile = TFile::Open(fullname,"READ");
  assert(infile);
  TTree* intree = (TTree*)infile->Get("Events");
  assert(intree);

  TH1F* hbase = new TH1F("base_"+sname,"",1,0,1000);
  hbase->Sumw2();
  intree->Project("base_"+sname,"met","weight");

  double baseint = hbase->Integral(0,2);
  cout << sname.Data() << "\n\tBase Norm = " << baseint << endl;

  vector<double> varnorms;
  for(int iwgt = 0; iwgt < 9; ++iwgt) {
    TH1F* hvarbase = new TH1F(TString::Format("base_var%d_%s", iwgt, sname.Data()),"",1,0,1000);
    hvarbase->Sumw2();
    intree->Project(hvarbase->GetName(),"met",TString::Format("weight*systweights[%d]",iwgt));
    double varint = hvarbase->Integral(0,2);
    cout << "\tVar " << iwgt << " Norm = " << varint << endl;
    varnorms.push_back(baseint/varint);
  }

  int isrbin = 0;
  for(auto srbin : srbins) {
    TString hname = "nominal_"+srbin+"_"+sname;
    TH1F* h = 0;
    if(srbin == "highmt_nt1") h = new TH1F(hname,"",1,250.,1000.);
    else                      h = new TH1F(hname,"",nbins,metbins);
    h->Sumw2();

    intree->Project(hname,"met",basewgt+"*("+sel[srbin]+")");
    Plot::addOverFlow(h);

    outfile->cd();
    h->Write();

    TH1F* hsysup = (TH1F*)h->Clone(TString(h->GetName()).Append("_varup"));
    TH1F* hsysdown = (TH1F*)h->Clone(TString(h->GetName()).Append("_vardown"));

    for(int iwgt = 0; iwgt < 9; ++iwgt) {
      TString cutstr = TString::Format("%s*systweights[%d]*(%s)", basewgt.Data(), iwgt, sel[srbin].Data());
      TString hvarname = TString::Format("var%d_%s_%s", iwgt, srbin.Data(), sname.Data());
      TH1F* hvar = 0;
      if(srbin == "highmt_nt1") hvar = new TH1F(hvarname,"",1,250.,1000.);
      else                      hvar = new TH1F(hvarname,"",nbins,metbins);
      hvar->Sumw2();

      intree->Project(hvarname,"met",cutstr);
      hvar->Scale(varnorms[iwgt]);
      Plot::addOverFlow(hvar);

      outfile->cd();
      hvar->Write();

      fillSysVars(hvar,hsysup,hsysdown);
    }

    outfile->cd();
    hsysup->Write();
    hsysdown->Write();

    TGraphAsymmErrors* gr = new TGraphAsymmErrors(h);

    for(int ibin = 0; ibin < gr->GetN(); ++ibin) {
      gr->SetPointEYhigh(ibin, hsysup->GetBinContent(ibin+1)-h->GetBinContent(ibin+1));
      gr->SetPointEYlow(ibin, h->GetBinContent(ibin+1)-hsysdown->GetBinContent(ibin+1));

      printf("bin_%s%d_%s\tscaleunc\t%s\t%4.3f\t%4.3f\t(%4.3f)\n", dolepcr ? "onelepcr_" : "", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), sname.Data(), hsysup->GetBinContent(ibin+1)/h->GetBinContent(ibin+1), hsysdown->GetBinContent(ibin+1)/h->GetBinContent(ibin+1), h->GetBinContent(ibin+1));

      double varup = hsysup->GetBinContent(ibin+1) - h->GetBinContent(ibin+1);
      double vardown = h->GetBinContent(ibin+1) - hsysdown->GetBinContent(ibin+1);
      double maxsys = 1.0 + (max(varup,vardown))/h->GetBinContent(ibin+1);

      printf("bin_%s%d_%s\tscaleunc\t%s\t%4.3f\n", dolepcr ? "onelepcr_" : "", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), sname.Data(), maxsys);
      fprintf(txtfile, "bin_%s%d_%s\tscaleunc\t%s\t%4.3f\n", dolepcr ? "onelepcr_" : "", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), sname.Data(), maxsys);
    }

    outfile->cd();
    gr->Write(TString(h->GetName()).Append("_varenvelope"));

    isrbin++;
  }

  fclose(txtfile);
  outfile->Close();

}
