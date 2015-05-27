#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisMethods/PlotUtils/interface/EffPlotTools.hh"
#endif

void plotSubjetsPerf(const TString conffile="plottops.conf",
                     const TString inputdir="trees",
                     const TString outputdir="plots/subjeteffs")
{
  gSystem->mkdir(outputdir, true);

  vector<TString> labels;

  vector<double> mcuts = {20.0, 30.0, 40.0, 50.0};
  vector<double> rmins = {0.10}; //{0.05, 0.10, 0.15, 0.20};
  vector<double> ycuts = {0.05}; //{0.05, 0.10, 0.15, 0.20};

  for(auto mcut : mcuts)
    for(auto rmin : rmins)
      for(auto ycut : ycuts)
        labels.push_back(TString::Format("mcut%2.0f_rmin%4.2f_ycut%4.2f", mcut, rmin, ycut));

  PlotStuff* plots1 = new PlotStuff(conffile, inputdir, outputdir);

  plots1->setPlotSource(PlotStuff::TREES);
  plots1->setPlotType(PlotStuff::COMP);
  plots1->setTree("Events");
  plots1->setWgtVar("1.0");
  plots1->setWriteHists();
  plots1->setHeaderText("","","");
  plots1->setDrawOption("hist");

  for(auto label : labels) {
    TString prefix = "cuts_"+label;

    plots1->addTreeVar(label+"_ngoodpartons",		prefix+"_ngoodpartons",		"1==1",	"Number of good partons",     14, -0.5, 13.5);
    plots1->addTreeVar(label+"_nassocpartons",		prefix+"_nassocpartons",	"1==1",	"Number of assoc partons",    14, -0.5, 13.5);
    plots1->addTreeVar(label+"_nsplitpartons",		prefix+"_nsplitpartons",	"1==1",	"Number of split partons",    14, -0.5, 13.5);
    plots1->addTreeVar(label+"_nresolvedpartons",	prefix+"_nresolvedpartons",	"1==1",	"Number of resolved partons", 14, -0.5, 13.5);
    plots1->addTreeVar(label+"_nmergedpartons",		prefix+"_nmergedpartons",	"1==1",	"Number of merged partons",   14, -0.5, 13.5);
    plots1->addTreeVar(label+"_alltoppt",		prefix+"_top_pt",		"1==1",	"top p_{T} [GeV]", 15, 0, 1200);
    plots1->addTreeVar(label+"_top1pt",			prefix+"_top_pt[0]",		"1==1",	"top p_{T} [GeV]", 60, 0, 1200);
    plots1->addTreeVar(label+"_top2pt",			prefix+"_top_pt[1]",		"1==1",	"top p_{T} [GeV]", 60, 0, 1200);
    plots1->addTreeVar(label+"_goodtoppt",		prefix+"_goodtop_pt",		"1==1",	"top p_{T} [GeV]", 15, 0, 1200);
    plots1->addTreeVar(label+"_assoctoppt",		prefix+"_assoctop_pt",		"1==1",	"top p_{T} [GeV]", 15, 0, 1200);
    plots1->addTreeVar(label+"_notassoctoppt",		prefix+"_notassoctop_pt",	"1==1",	"top p_{T} [GeV]", 15, 0, 1200);
    plots1->addTreeVar(label+"_nosplittoppt",		prefix+"_nosplittop_pt",	"1==1",	"top p_{T} [GeV]", 15, 0, 1200);
    plots1->addTreeVar(label+"_splittoppt",		prefix+"_splittop_pt",		"1==1",	"top p_{T} [GeV]", 15, 0, 1200);
    plots1->addTreeVar(label+"_mergedtoppt",		prefix+"_mergedtop_pt",		"1==1",	"top p_{T} [GeV]", 15, 0, 1200);
    plots1->addTreeVar(label+"_contaminatedtoppt",	prefix+"_contaminatedtop_pt",	"1==1",	"top p_{T} [GeV]", 15, 0, 1200);
    plots1->addTreeVar(label+"_resolvedtoppt",		prefix+"_resolvedtop_pt",	"1==1",	"top p_{T} [GeV]", 15, 0, 1200);
    plots1->addTreeVar(label+"_avg2goodtoppt",		"0.5*("+prefix+"_goodtop_pt[0]+"+prefix+"_goodtop_pt[1])",		prefix+"_top_isgood[0] && "+prefix+"_top_isgood[1]",		"average top p_{T} [GeV]", 15, 0, 1200);
    plots1->addTreeVar(label+"_avg2resolvedtoppt",	"0.5*("+prefix+"_resolvedtop_pt[0]+"+prefix+"_resolvedtop_pt[1])",	prefix+"_top_isresolved[0] && "+prefix+"_top_isresolved[1]",	"average top p_{T} [GeV]", 15, 0, 1200);
    plots1->addTreeVar(label+"_top1ptvstop2pt",		prefix+"_top_pt[1]:"+prefix+"_top_pt[0]",	"1==1",	"top 1 p_{T} [GeV]; top 2 p_{T} [GeV]", 60, 0, 1200, 60, 0, 1200);
  }

  plots1->plot();

  TFile* infile = new TFile(plots1->outfileName(),"UPDATE");

  vector<Sample*> samples;
  PlotTools::initSamples(conffile, samples);

  for(auto* sample : samples) {
    TString name = sample->name;
    for(auto label : labels) {
      TH1F* denominator = (TH1F*)infile->Get(label+"_goodtoppt_"+name);
      assert(denominator);

      TH1F* denominator_alltop = name.Contains("T2tt") || name.Contains("ttbar") ? (TH1F*)infile->Get(label+"_avg2goodtoppt_"+name) : (TH1F*)infile->Get(label+"_avg4goodtoppt_"+name);
      assert(denominator_alltop);

      TH1F* num_assoc = (TH1F*)infile->Get(label+"_assoctoppt_"+name);
      assert(num_assoc);

      TH1F* num_notassoc = (TH1F*)infile->Get(label+"_notassoctoppt_"+name);
      assert(num_notassoc);

      TH1F* num_nosplit = (TH1F*)infile->Get(label+"_nosplittoppt_"+name);
      assert(num_nosplit);

      TH1F* num_split = (TH1F*)infile->Get(label+"_splittoppt_"+name);
      assert(num_split);

      TH1F* num_merged = (TH1F*)infile->Get(label+"_mergedtoppt_"+name);
      assert(num_merged);

      TH1F* num_contaminated = (TH1F*)infile->Get(label+"_contaminatedtoppt_"+name);
      assert(num_contaminated);

      TH1F* num_resolved = (TH1F*)infile->Get(label+"_resolvedtoppt_"+name);
      assert(num_resolved);

      TH1F* num_resolved_alltop = name.Contains("T2tt") || name.Contains("ttbar") ? (TH1F*)infile->Get(label+"_avg2resolvedtoppt_"+name) : (TH1F*)infile->Get(label+"_avg4resolvedtoppt_"+name);
      assert(num_resolved_alltop);

      TH1F* denominator_unresolved = (TH1F*)num_notassoc->Clone(label+"_notresolvedtoppt_"+name);
      denominator_unresolved->Add(num_split);
      denominator_unresolved->Add(num_merged);
      denominator_unresolved->Add(num_contaminated);

      TGraphAsymmErrors* eff_assoc = EffPlotTools::computeEffGraph(num_assoc, denominator);
      eff_assoc->Write(label+"_effassoc_"+name);

      TGraphAsymmErrors* eff_notassoc = EffPlotTools::computeEffGraph(num_notassoc, denominator);
      eff_notassoc->Write(label+"_effnotassoc_"+name);

      TGraphAsymmErrors* relineff_notassoc = EffPlotTools::computeEffGraph(num_notassoc, denominator_unresolved);
      relineff_notassoc->Write(label+"_relineffnotassoc_"+name);

      TGraphAsymmErrors* eff_nosplit = EffPlotTools::computeEffGraph(num_nosplit, denominator);
      eff_nosplit->Write(label+"_effnosplit_"+name);

      TGraphAsymmErrors* eff_split = EffPlotTools::computeEffGraph(num_split, denominator);
      eff_split->Write(label+"_effsplit_"+name);

      TGraphAsymmErrors* relineff_split = EffPlotTools::computeEffGraph(num_split, denominator_unresolved);
      relineff_split->Write(label+"_relineffsplit_"+name);

      TGraphAsymmErrors* eff_merged = EffPlotTools::computeEffGraph(num_merged, denominator);
      eff_merged->Write(label+"_effmerged_"+name);

      TGraphAsymmErrors* relineff_merged = EffPlotTools::computeEffGraph(num_merged, denominator_unresolved);
      relineff_merged->Write(label+"_relineffmerged_"+name);

      TGraphAsymmErrors* eff_contaminated = EffPlotTools::computeEffGraph(num_contaminated, denominator);
      eff_contaminated->Write(label+"_effcontaminated_"+name);

      TGraphAsymmErrors* relineff_contaminated = EffPlotTools::computeEffGraph(num_contaminated, denominator_unresolved);
      relineff_contaminated->Write(label+"_relineffcontaminated_"+name);

      TGraphAsymmErrors* eff_resolved = EffPlotTools::computeEffGraph(num_resolved, denominator);
      eff_resolved->Write(label+"_effresolved_"+name);

      TGraphAsymmErrors* eff_resolved_alltop = EffPlotTools::computeEffGraph(num_resolved_alltop, denominator_alltop);
      eff_resolved_alltop->Write(label+"_effresolved_alltop_"+name);
    }
  }

  infile->Write();
  infile->Close();

  infile = new TFile(plots1->outfileName());

  PlotStuff* plots2 = new PlotStuff(infile, outputdir);

  plots2->setPlotSource(PlotStuff::HISTSSINGLEFILE);
  plots2->setPlotType(PlotStuff::COMP);
  plots2->setHeaderText("","","");
  plots2->setYTitle("Events");
  plots2->setHeaderText("","","");

  vector<TString> ngoodpartonnames, nresolvedpartonnames, nmergedpartonnames;
  vector<TString> effassocnames, effnosplitnames, effresolvednames, effresolvedallnames;
  vector<TString> effnotassocnames, effsplitnames, effmergednames, effcontaminatednames;
  vector<TString> samplelabels;
  vector<TString> topptnames   = {labels[0]+"_top1pt_"+samples[0]->name,labels[0]+"_top2pt_"+samples[0]->name};
  vector<TString> topptlabels  = {"leading top", "subleading top"};
  vector<TString> toppt2dname  = {labels[0]+"_top1ptvstop2pt_"+samples[0]->name};
  vector<TString> toppt2dlabel = {""};

  for(auto* sample : samples) {
    samplelabels.push_back(sample->label);
    ngoodpartonnames.clear();
    nresolvedpartonnames.clear();
    nmergedpartonnames.clear();
    effassocnames.clear();
    effnotassocnames.clear();
    effnosplitnames.clear();
    effsplitnames.clear();
    effmergednames.clear();
    effcontaminatednames.clear();
    effresolvednames.clear();
    effresolvedallnames.clear();

    for(auto label : labels) {
      ngoodpartonnames.push_back(label+"_ngoodpartons_"+sample->name);
      nresolvedpartonnames.push_back(label+"_nresolvedpartons_"+sample->name);
      nmergedpartonnames.push_back(label+"_nmergedpartons_"+sample->name);
      effassocnames.push_back(label+"_effassoc_"+sample->name);
      effnotassocnames.push_back(label+"_effnotassoc_"+sample->name);
      effnosplitnames.push_back(label+"_effnosplit_"+sample->name);
      effsplitnames.push_back(label+"_effsplit_"+sample->name);
      effmergednames.push_back(label+"_effmerged_"+sample->name);
      effcontaminatednames.push_back(label+"_effcontaminated_"+sample->name);
      effresolvednames.push_back(label+"_effresolved_"+sample->name);
      effresolvedallnames.push_back(label+"_effresolved_alltop_"+sample->name);
      vector<TString> ineffnames    = {label+"_effnotassoc_"+sample->name, label+"_effsplit_"+sample->name, label+"_effmerged_"+sample->name, label+"_effcontaminated_"+sample->name};
      vector<TString> relineffnames = {label+"_relineffnotassoc_"+sample->name, label+"_relineffsplit_"+sample->name, label+"_relineffmerged_"+sample->name, label+"_relineffcontaminated_"+sample->name};
      vector<TString> inefflabels   = {"fails jet association", "split partons", "merged partons", "contaminated jet"};
      plots2->addCompSet(label+"_ineffcomp_"+sample->name,	ineffnames,		inefflabels, 0.5, PlotStuff::PlotComp::GRAPHCOMP);
      plots2->addCompSet(label+"_relineffcomp_"+sample->name,	relineffnames,		inefflabels, 1.1, PlotStuff::PlotComp::GRAPHCOMP);
    }

    plots2->addCompSet("ngoodpartonscomp_"+sample->name,	ngoodpartonnames,	labels);
    plots2->addCompSet("nresolvedpartonscomp_"+sample->name,	nresolvedpartonnames,	labels);
    plots2->addCompSet("nmergedpartonscomp_"+sample->name,	nmergedpartonnames,	labels);
    plots2->addCompSet("effassoc_"+sample->name,		effassocnames,		labels, 1.1, PlotStuff::PlotComp::GRAPHCOMP);
    plots2->addCompSet("effnotassoc_"+sample->name,		effnotassocnames,	labels, 1.1, PlotStuff::PlotComp::GRAPHCOMP);
    plots2->addCompSet("effnosplit_"+sample->name,		effnosplitnames,	labels, 1.1, PlotStuff::PlotComp::GRAPHCOMP);
    plots2->addCompSet("effsplit_"+sample->name,		effsplitnames,		labels, 1.1, PlotStuff::PlotComp::GRAPHCOMP);
    plots2->addCompSet("effmerged_"+sample->name,		effmergednames,		labels, 1.1, PlotStuff::PlotComp::GRAPHCOMP);
    plots2->addCompSet("effcontaminated_"+sample->name,		effcontaminatednames,	labels, 1.1, PlotStuff::PlotComp::GRAPHCOMP);
    plots2->addCompSet("effresolved_"+sample->name,		effresolvednames,	labels, 1.1, PlotStuff::PlotComp::GRAPHCOMP);
    plots2->addCompSet("effresolved_alltop_"+sample->name,	effresolvedallnames,	labels, 1.1, PlotStuff::PlotComp::GRAPHCOMP);
  }

  plots2->addCompSet("topptcomp",  topptnames, topptlabels);
  plots2->addCompSet("toppt2dcomp",toppt2dname,toppt2dlabel,100.0,PlotStuff::PlotComp::HIST2DCOMP);

  plots2->plot();

  delete plots1;
  delete plots2;

}
