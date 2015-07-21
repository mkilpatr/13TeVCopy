#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisMethods/PlotUtils/interface/EffPlotTools.hh"
#endif

void plotSubjetCorrections(const TString conffile="jets.conf",
                           const TString inputdir="trees",
                           const TString outputdir="plots/subjetcorrs")
{

  gSystem->mkdir(outputdir, true);

  vector<float> corrections = {0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45};
  vector<float> ptbins = {20.0, 30.0, 40.0, 50.0, 60.0, 80.0, 100.0, 150.0, 200.0, 300.0, 500.0, 1000.0};
  vector<float> etabins = {0.0, 1.5, 2.4};

  vector<TString> labels;
  for(auto corr: corrections) {
    labels.push_back(TString::Format("corr%4.2f", corr));
  }

  vector<TString> binlabels;
  for(unsigned int iptbin = 0; iptbin < ptbins.size() - 1; iptbin++) {
    for(unsigned int ietabin = 0; ietabin < etabins.size() - 1; ietabin++) {
      binlabels.push_back(TString::Format("_pt%4.2fto%4.2f_eta%4.2fto%4.2f", ptbins[iptbin], ptbins[iptbin+1], etabins[ietabin], etabins[ietabin+1]));
    }
  }

  PlotStuff* plots = new PlotStuff(conffile, inputdir, outputdir);

  plots->setPlotSource(PlotStuff::TREES);
  plots->setPlotType(PlotStuff::COMP);
  plots->setTree("Events");
  plots->setWgtVar("1.0");
  plots->setWriteHists();
  plots->setHeaderText("","","");
  plots->setDrawOption("hist");

  for(auto label : labels) {
    TString prefix = "reco_" + label + "_nj";
    plots->addTreeVar(prefix+"20",prefix+"20","1==1","Number of jets (p_{T} > 20 GeV)", 15, -0.5, 14.5);
    plots->addTreeVar(prefix+"30",prefix+"30","1==1","Number of jets (p_{T} > 30 GeV)", 15, -0.5, 14.5);
    plots->addTreeVar(prefix+"40",prefix+"40","1==1","Number of jets (p_{T} > 40 GeV)", 15, -0.5, 14.5);
    plots->addTreeVar(prefix+"50",prefix+"50","1==1","Number of jets (p_{T} > 50 GeV)", 15, -0.5, 14.5);
    plots->addTreeVar(prefix+"60",prefix+"60","1==1","Number of jets (p_{T} > 60 GeV)", 15, -0.5, 14.5);
    for(auto binlabel : binlabels) {
      TString binprefix = prefix + binlabel;
      plots->addTreeVar(binprefix,binprefix,"1==1","Number of jets", 15, -0.5, 14.5);
    }
  }
  plots->addTreeVar("gen_nj20","gen_nj20","1==1","Number of jets (p_{T} > 20 GeV)", 15, -0.5, 14.5);
  plots->addTreeVar("gen_nj30","gen_nj30","1==1","Number of jets (p_{T} > 30 GeV)", 15, -0.5, 14.5);
  plots->addTreeVar("gen_nj40","gen_nj40","1==1","Number of jets (p_{T} > 40 GeV)", 15, -0.5, 14.5);
  plots->addTreeVar("gen_nj50","gen_nj50","1==1","Number of jets (p_{T} > 50 GeV)", 15, -0.5, 14.5);
  plots->addTreeVar("gen_nj60","gen_nj60","1==1","Number of jets (p_{T} > 60 GeV)", 15, -0.5, 14.5);
  for(auto binlabel : binlabels) {
    TString binprefix = "gen_nj" + binlabel;
    plots->addTreeVar(binprefix,binprefix,"1==1","Number of jets", 15, -0.5, 14.5);
  }

  plots->plot();

  TFile* infile = new TFile(plots->outfileName(), "UPDATE");

  vector<Sample*> samples;
  PlotTools::initSamples(conffile, samples);

  for(auto* sample : samples) {
    TString name = sample->name;
    TH1F* den_20 = (TH1F*)infile->Get("gen_nj20_"+name);
    TH1F* den_30 = (TH1F*)infile->Get("gen_nj30_"+name);
    TH1F* den_40 = (TH1F*)infile->Get("gen_nj40_"+name);
    TH1F* den_50 = (TH1F*)infile->Get("gen_nj50_"+name);
    TH1F* den_60 = (TH1F*)infile->Get("gen_nj60_"+name);
    for(auto label : labels) {
      TString prefix = "reco_" + label + "_nj";
      TH1F* num_20 = (TH1F*)infile->Get(prefix+"20_"+name);
      TH1F* num_30 = (TH1F*)infile->Get(prefix+"30_"+name);
      TH1F* num_40 = (TH1F*)infile->Get(prefix+"40_"+name);
      TH1F* num_50 = (TH1F*)infile->Get(prefix+"50_"+name);
      TH1F* num_60 = (TH1F*)infile->Get(prefix+"60_"+name);
      num_20->Divide(den_20);
      num_20->Write("ratio_20_"+label+"_"+name);
      num_30->Divide(den_30);
      num_30->Write("ratio_30_"+label+"_"+name);
      num_40->Divide(den_40);
      num_40->Write("ratio_40_"+label+"_"+name);
      num_50->Divide(den_50);
      num_50->Write("ratio_50_"+label+"_"+name);
      num_60->Divide(den_60);
      num_60->Write("ratio_60_"+label+"_"+name);
    }
    for(auto binlabel : binlabels) {
      TString binprefix = "gen_nj" + binlabel;
      TH1F* den = (TH1F*)infile->Get("gen_nj"+binlabel+"_"+name);
      for(auto label : labels) {
        TString prefix = "reco_" + label + "_nj" + binlabel + "_";
        TH1F* num = (TH1F*)infile->Get(prefix+name);
        num->Divide(den);
        num->Write("ratio_"+label+binlabel+"_"+name);
      }
    }
  }

  infile->Write();
  infile->Close();

  infile = new TFile(plots->outfileName());

  plots = new PlotStuff(infile, outputdir);

  plots->setPlotSource(PlotStuff::HISTSSINGLEFILE);
  plots->setPlotType(PlotStuff::COMP);
  plots->setHeaderText("","","");
  plots->setYTitle("Ratio");

  vector<TString> ratio20_names;
  vector<TString> ratio30_names;
  vector<TString> ratio40_names;
  vector<TString> ratio50_names;
  vector<TString> ratio60_names;
  vector<TString> ratioptetabins_names;

  for(auto* sample : samples) {
    ratio20_names.clear();
    ratio30_names.clear();
    ratio40_names.clear();
    ratio50_names.clear();
    ratio60_names.clear();
    TString name = sample->name;
    for(auto label : labels) {
      ratio20_names.push_back("ratio_20_"+label+"_"+name);
      ratio30_names.push_back("ratio_30_"+label+"_"+name);
      ratio40_names.push_back("ratio_40_"+label+"_"+name);
      ratio50_names.push_back("ratio_50_"+label+"_"+name);
      ratio60_names.push_back("ratio_60_"+label+"_"+name);
    }
    for(auto binlabel : binlabels) {
      ratioptetabins_names.clear();
      for(auto label : labels) {
        ratioptetabins_names.push_back("ratio_"+label+binlabel+"_"+name);
      }
      plots->addCompSet("njet_ratio"+binlabel+"_"+name, ratioptetabins_names, labels);
    }

    plots->addCompSet("njet20_ratio_"+name, ratio20_names, labels);
    plots->addCompSet("njet30_ratio_"+name, ratio30_names, labels);
    plots->addCompSet("njet40_ratio_"+name, ratio40_names, labels);
    plots->addCompSet("njet50_ratio_"+name, ratio50_names, labels);
    plots->addCompSet("njet60_ratio_"+name, ratio60_names, labels);
  }

  plots->plot();

  delete plots;

}
