#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

// Procedure: The first 9 entries in the systweights vector correspond to various permutations of varying mu_R / mu_F up/down by a factor of 2. We take the envelope of these variations to get the scale uncertainty. In order to factor out the effects on the cross section, yields should be normalized to the inclusive yield. To be done properly, input trees should have no pre-selection
//

void fillSysVars(TH1F* hvar, TH1F* hsysup, TH1F* hsysdown) {

  for(int ibin = 1; ibin < hvar->GetNbinsX()+1; ++ibin) {
    if(hvar->GetBinContent(ibin) > hsysup->GetBinContent(ibin)) hsysup->SetBinContent(ibin, hvar->GetBinContent(ibin));
    if(hvar->GetBinContent(ibin) < hsysdown->GetBinContent(ibin)) hsysdown->SetBinContent(ibin, hvar->GetBinContent(ibin));
  }

}

void getScaleUncertaintiesTTZ(const TString inputdir = "/uscms_data/d3/hqu/trees/050316/sr",
                              const TString outputdir = "plots_bkgest/sysvars",
                              const TString lumistr = "2.318",
                              const TString format = "pdf",
                              vector<TString> samples = {"ttZ_only","tZq"},
                              const bool    combinesamples = true,
                              const bool    plotlog = true)
{

  gSystem->mkdir(outputdir,true);

  //TString basewgt = lumistr + "*weight*truePUWeight*btagWeight*leptnpweight*lepvetoweight";
  TString basewgt = lumistr + "*weight"; // leave out extra weights to avoid some weird statistical effects

  map<TString,TString> sel;

  sel["trig"]         = "passjson && passdijetmet && j2pt>75 && met>250 && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["vetoes"]       = " && nvetolep==0 && (nvetotau==0 || (ismc && npromptgentau>0))";
  sel["njets"]        = " && njets>=5 && nbjets>=1 && nlbjets>=2";
  sel["dphij123"]     = " && dphij12met>0.5 && dphij3met>0.5 && dphij4met>0.5";
  sel["sr"]           = sel["trig"] + sel["vetoes"]   + sel["njets"]    + sel["dphij123"];
  sel["lowmt"]        = " && mtcsv12met<=175";
  sel["highmt"]       = " && mtcsv12met>175";
  sel["mednj"]        = " && njets>=5 && njets<7";
  sel["highnj"]       = " && njets>=7";
  sel["nb1"]          = " && nbjets==1";
  sel["nb2"]          = " && nbjets>=2";
  sel["nt0"]          = " && ncttstd==0";
  sel["nt1"]          = " && ncttstd>0";
  sel["lowmt_mednj"]        = sel["sr"] + sel["lowmt"] + sel["mednj"];
  sel["lowmt_highnj"]       = sel["sr"] + sel["lowmt"] + sel["highnj"];
  sel["highmt_mednj_nt0"]   = sel["sr"] + sel["highmt"] + sel["mednj"] + sel["nt0"];
  sel["highmt_highnj_nt0"]  = sel["sr"] + sel["highmt"] + sel["highnj"] + sel["nt0"];
  sel["highmt_nt1"]         = sel["sr"] + sel["highmt"] + sel["nt1"];
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

  const int nbins = 5;
  double metbins[nbins+1] = {250.,300.,400.,500.,600.,1000.};

  vector<TString> binlabels = {"5_1_0_0", "5_2_0_0", "7_1_0_0", "7_2_0_0", "5_1_0_175", "5_2_0_175", "7_1_0_175", "7_2_0_175", "5_1_1_175", "5_2_1_175"};

  TFile* infile = 0;
  TTree* intree = 0;

  TFile* outfile = new TFile(outputdir + "/scalevars.root","RECREATE");

  FILE * txtfile = fopen(outputdir + "/scaleuncs.txt","w");

  for(auto sample : samples) {
    infile = TFile::Open(inputdir + "/" + sample + "_tree.root");
    assert(infile);
    intree = (TTree*)infile->Get("Events");
    assert(intree);
    TH1F* hbase = new TH1F("base_"+sample,"",1,0,1000);
    hbase->Sumw2();
    intree->Project("base_"+sample,"met","weight");
    double baseint = hbase->Integral(0,2);
    cout << sample.Data() << "\n\tBase Norm = " << baseint << endl;
    vector<double> varnorms;
    for(int iwgt = 0; iwgt < 9; ++iwgt) {
      TH1F* hvarbase = new TH1F(TString::Format("base_var%d_%s", iwgt, sample.Data()),"",1,0,1000);
      hvarbase->Sumw2();
      intree->Project(hvarbase->GetName(),"met",TString::Format("weight*systweights[%d]",iwgt));
      double varint = hvarbase->Integral(0,2);
      cout << "\tVar " << iwgt << " Norm = " << varint << endl;
      varnorms.push_back(baseint/varint);
    }
    int isrbin = 0;
    for(auto srbin : srbins) {
      TString hname = "nominal_"+srbin+"_"+sample;
      TH1F* h = new TH1F(hname,"",5,metbins);
      h->Sumw2();
      intree->Project(hname,"met",basewgt+"*("+sel[srbin]+")");
      Plot::addOverFlow(h);
      outfile->cd();
      h->Write();
      TH1F* hsysup = (TH1F*)h->Clone(TString(h->GetName()).Append("_varup"));
      TH1F* hsysdown = (TH1F*)h->Clone(TString(h->GetName()).Append("_vardown"));
      for(int iwgt = 0; iwgt < 9; ++iwgt) {
        TString cutstr = TString::Format("%s*systweights[%d]*(%s)", basewgt.Data(), iwgt, sel[srbin].Data());
        TString hvarname = TString::Format("var%d_%s_%s", iwgt, srbin.Data(), sample.Data());
        TH1F* hvar = new TH1F(hvarname,"",5,metbins);
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
        printf("bin_%d_%s\tscaleunc\t%s\t%4.3f\t%4.3f\t(%4.3f)\n", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), sample.Data(), hsysup->GetBinContent(ibin+1)/h->GetBinContent(ibin+1), hsysdown->GetBinContent(ibin+1)/h->GetBinContent(ibin+1), h->GetBinContent(ibin+1));
        double varup = hsysup->GetBinContent(ibin+1) - h->GetBinContent(ibin+1);
        double vardown = h->GetBinContent(ibin+1) - hsysdown->GetBinContent(ibin+1);
        double maxsys = 1.0 + (max(varup,vardown))/h->GetBinContent(ibin+1);
        printf("bin_%d_%s\tscaleunc\t%s\t%4.3f\n", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), sample.Data(), maxsys);
        fprintf(txtfile, "bin_%d_%s\tscaleunc\t%s\t%4.3f\n", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), sample.Data(), maxsys);
      }
      outfile->cd();
      gr->Write(TString(h->GetName()).Append("_varenvelope"));
      isrbin++;
    }
  }

  if(combinesamples) {
    cout << "Combined" << endl;
    int isrbin = 0;
    for(auto srbin : srbins) {
      TH1F *hnom = 0, *hup = 0, *hdown = 0;
      for(auto sample : samples) {
        if(!hnom)  hnom     = (TH1F*)outfile->Get("nominal_"+srbin+"_"+sample)->Clone("combined_"+srbin);
        else       hnom ->Add((TH1F*)outfile->Get("nominal_"+srbin+"_"+sample));
        if(!hup)   hup      = (TH1F*)outfile->Get("nominal_"+srbin+"_"+sample+"_varup")->Clone("combined_"+srbin+"_varup");
        else       hup  ->Add((TH1F*)outfile->Get("nominal_"+srbin+"_"+sample+"_varup"));
        if(!hdown) hdown    = (TH1F*)outfile->Get("nominal_"+srbin+"_"+sample+"_vardown")->Clone("combined_"+srbin+"_vardown");
        else       hdown->Add((TH1F*)outfile->Get("nominal_"+srbin+"_"+sample+"_vardown"));
      }
      hnom->Write();
      hup->Write();
      hdown->Write();
      TGraphAsymmErrors* gr = new TGraphAsymmErrors(hnom);
      for(int ibin = 0; ibin < gr->GetN(); ++ibin) {
        gr->SetPointEYhigh(ibin, hup->GetBinContent(ibin+1)-hnom->GetBinContent(ibin+1));
        gr->SetPointEYlow(ibin, hnom->GetBinContent(ibin+1)-hdown->GetBinContent(ibin+1));
        printf("bin_%d_%s\tscaleunc\tcombined\t%4.3f\t%4.3f\t(%4.3f)\n", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), hup->GetBinContent(ibin+1)/hnom->GetBinContent(ibin+1), hdown->GetBinContent(ibin+1)/hnom->GetBinContent(ibin+1), hnom->GetBinContent(ibin+1));
        double varup = hup->GetBinContent(ibin+1) - hnom->GetBinContent(ibin+1);
        double vardown = hnom->GetBinContent(ibin+1) - hdown->GetBinContent(ibin+1);
        double maxsys = 1.0 + (max(varup,vardown))/hnom->GetBinContent(ibin+1);
        printf("bin_%d_%s\tscaleunc\tcombined\t%4.3f\n", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), maxsys);
        fprintf(txtfile, "bin_%d_%s\tscaleunc\tcombined\t%4.3f\n", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), maxsys);
      }
      outfile->cd();
      gr->Write(TString(hnom->GetName()).Append("_varenvelope"));
      isrbin++;
    }
  }

  fclose(txtfile);

  PlotStuff* plots = new PlotStuff(outfile, outputdir);
  plots->setPlotSource(PlotStuff::HISTSSINGLEFILE);
  plots->setPlotType(PlotStuff::COMP);
  plots->setHeaderText("#sqrt{s} = 13 TeV",TString::Format("%4.1f fb^{-1}",stof(string(lumistr.Data()))),"");
  plots->setYTitle("Events");
  if(plotlog) plots->setLogy();
  plots->setFormat(format);
  plots->setGraphDrawOption("P2");

  for(auto sample : samples) {
    for(auto srbin : srbins) {
      plots->addCompSet(sample+"_"+srbin+"_scalevar", {"nominal_"+srbin+"_"+sample+"_varenvelope"},{"Max. #mu_{R}/#mu_{F} variation"},-1.,PlotStuff::PlotComp::GRAPHCOMP);
    }
  }
  if(combinesamples) {
    for(auto srbin : srbins) {
      plots->addCompSet("combined_"+srbin+"_scalevar", {"combined_"+srbin+"_varenvelope"},{"Max. #mu_{R}/#mu_{F} variation"},-1.,PlotStuff::PlotComp::GRAPHCOMP);
    }
  }
  plots->plot();

}
