#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotTauCands(const TString conffile="runtau.conf",
                  const TString inputdir="run/plots",
                  const TString outputdir="run/plots")
{

  PlotStuff* myPlots1 = new PlotStuff(conffile, inputdir, outputdir);
  myPlots1->setPlotSource(PlotStuff::TREES);
  myPlots1->setPlotType(PlotStuff::DATAMC);
  myPlots1->setSigScale(-1);
  myPlots1->setTree("Candidates");
  myPlots1->setWgtVar("1.0");
  myPlots1->setWriteHists();
  myPlots1->setHeaderText("","","");
  myPlots1->setColor("realtaus_tt1tau",StyleTools::color_ttbar1l);
  myPlots1->setColor("realtaus_tt2tau",StyleTools::color_ttbar2l);
  myPlots1->setColor("faketaus_T2tt_500_325",StyleTools::color_comp4);
  myPlots1->setColor("faketaus_T2tt_650_325",StyleTools::color_comp1);
  myPlots1->setColor("faketaus_T2tt_850_100",StyleTools::color_comp2);
  myPlots1->setLegend(0.25, 0.7, 0.9, 0.9);
  myPlots1->setYTitle("Candidates");
  myPlots1->setTreeFileSuffix("_addtaumva.root");
  myPlots1->setAddSigScaleTxt(false);
  //myPlots1->setLogy();

  myPlots1->addTreeVar("mt",        "mt",        "1==1","m_{T} [GeV]",                   250,0.0,500.0);
  myPlots1->addTreeVar("mtnophoton","mtnophoton","1==1","m_{T} [GeV]",                   250,0.0,500.0);
  myPlots1->addTreeVar("dphimet",   "dphimet",   "1==1","|#Delta#phi(h, #slash{E}_{T})|",252,0.0,3.15);
  myPlots1->addTreeVar("dphiw",     "dphiw",     "1==1","|#Delta#phi(h, W)|"            ,252,0.0,3.15);

  myPlots1->addTreeVar("mt_baseline",        "mt",        "njets>3 && misset>150.0","m_{T} [GeV]",                   250,0.0,500.0);
  myPlots1->addTreeVar("mtnophoton_baseline","mtnophoton","njets>3 && misset>150.0","m_{T} [GeV]",                   250,0.0,500.0);
  myPlots1->addTreeVar("dphimet_baseline",   "dphimet",   "njets>3 && misset>150.0","|#Delta#phi(h, #slash{E}_{T})|",252,0.0,3.15);
  myPlots1->addTreeVar("dphiw_baseline",     "dphiw",     "njets>3 && misset>150.0","|#Delta#phi(h, W)|",252,0.0,3.15);

  myPlots1->addTreeVar("mva_mtpresel",  "taumva_mtpresel",  "njets>3 && misset>150.0 && mt<100.0 && absdz<0.2",    "Discriminator",200,-1.0,1.0);
  myPlots1->addTreeVar("mva_dphipresel","taumva_dphipresel","njets>3 && misset>150.0 && dphimet<1.34 && absdz<0.2","Discriminator",200,-1.0,1.0);

  myPlots1->plot(); 

  TFile* infile = new TFile(myPlots1->outfileName());
  PlotStuff* myPlots2 = new PlotStuff(infile, outputdir);
  myPlots2->setPlotSource(PlotStuff::HISTSSINGLEFILE);
  myPlots2->setPlotType(PlotStuff::COMP);
  myPlots2->setUnitScale();
  myPlots2->setHeaderText("","","");
  myPlots2->setLegend(0.3,0.65,0.9,0.9);
  myPlots2->setColor("comp3",StyleTools::color_comp5);
  myPlots2->setColor("comp4",StyleTools::color_comp6);
  myPlots2->setYTitle("Candidates (normalized to 1)");

  vector<TString> mtnames1  = {"mtnophoton_realtaus_tt1tau", "mt_realtaus_tt1tau", "mtnophoton_faketaus_T2tt_500_325", "mt_faketaus_T2tt_500_325"};
  vector<TString> mtlabels1 = {"t#bar{t} gen-matched #tau_{had} cands (without #gamma)", "t#bar{t} gen-matched #tau_{had} cands (with #gamma)", "T2tt(500,325) non-matched cands (without #gamma)", "T2tt(500,325) non-matched cands (with #gamma)"};
  vector<TString> mtnames2  = {"mtnophoton_realtaus_tt1tau", "mt_realtaus_tt1tau", "mtnophoton_faketaus_T2tt_650_325", "mt_faketaus_T2tt_650_325"};
  vector<TString> mtlabels2 = {"t#bar{t} gen-matched #tau_{had} cands (without #gamma)", "t#bar{t} gen-matched #tau_{had} cands (with #gamma)", "T2tt(650,325) non-matched cands (without #gamma)", "T2tt(650,325) non-matched cands (with #gamma)"};
  vector<TString> mtnames3  = {"mtnophoton_realtaus_tt1tau", "mt_realtaus_tt1tau", "mtnophoton_faketaus_T2tt_850_100", "mt_faketaus_T2tt_850_100"};
  vector<TString> mtlabels3 = {"t#bar{t} gen-matched #tau_{had} cands (without #gamma)", "t#bar{t} gen-matched #tau_{had} cands (with #gamma)", "T2tt(850,100) non-matched cands (without #gamma)", "T2tt(850,100) non-matched cands (with #gamma)"};

  myPlots2->addCompSet("mtcomparison_lowdm",mtnames1,mtlabels1);
  myPlots2->addCompSet("mtcomparison_meddm",mtnames2,mtlabels2);
  myPlots2->addCompSet("mtcomparison_highdm",mtnames3,mtlabels3);

  myPlots2->plot();

  delete myPlots1;
  delete myPlots2;

}
