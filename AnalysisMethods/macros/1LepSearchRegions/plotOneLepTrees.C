#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotOneLepTrees(const TString conffile="../run1leptrees.conf",
                     const TString inputdir="../run/trees",
                     const TString outputdir="../run/plots")
{

  PlotStuff* myPlots1 = new PlotStuff(conffile, inputdir, outputdir);
  myPlots1->setPlotSource(PlotStuff::TREES);
  myPlots1->setPlotType(PlotStuff::DATAMC);
  myPlots1->setSigScale(100);
  myPlots1->setWgtVar("ScaleFactor");

  myPlots1->addTreeVar("dphi_baseline","DphiLepW","1==1","|#Delta#phi(W, l)|",126,0,3.15);
  myPlots1->addTreeVar("topness_baseline","minTopness","1==1","Topness",60,-10,20);
  myPlots1->addTreeVar("topness_dphicut","minTopness","abs(DphiLepW)>1","Topness",60,-10,20);

  myPlots1->plot(); 

  PlotStuff* myPlots2 = new PlotStuff(conffile, inputdir, outputdir);
  myPlots2->setPlotSource(PlotStuff::TREES);
  myPlots2->setPlotType(PlotStuff::DATAMC);
  myPlots2->setSigScale(-1);    // scale to sum(bkg)
  myPlots2->setWgtVar("ScaleFactor");
  myPlots2->setLegend(0.51, 0.65, 0.95, 0.9);

  myPlots2->addTreeVar("dphi_baseline","DphiLepW","1==1","|#Delta#phi(W, l)|",126,0,3.15);
  myPlots2->addTreeVar("topness_baseline","minTopness","1==1","Topness",60,-10,20);
  myPlots2->addTreeVar("topness_dphicut","minTopness","abs(DphiLepW)>1","Topness",60,-10,20);

  myPlots2->plot();

  PlotStuff* myPlots3 = new PlotStuff(conffile, inputdir, outputdir);
  myPlots3->setPlotSource(PlotStuff::TREES);
  myPlots3->setPlotType(PlotStuff::DATAMC);
  myPlots3->setWgtVar("ScaleFactor");
  myPlots3->setMaxScale(1.6);

  myPlots3->addTreeVar("topness_met150to250_nj4to7","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && MET>150 && MET<=250","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met150to250_nj4to7_nb1","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets == 1 && MET>150 && MET<=250","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met150to250_nj4to7_nb2","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets == 2 && MET>150 && MET<=250","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met150to250_nj4to7_nbgeq3","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets >= 3 && MET>150 && MET<=250","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met150to250_njgeq7","minTopness","abs(DphiLepW)>1 && NJets >= 7 && MET>150 && MET<=250","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met150to250_njgeq7_nb1","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets == 1 && MET>150 && MET<=250","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met150to250_njgeq7_nb2","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets == 2 && MET>150 && MET<=250","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met150to250_njgeq7_nbgeq3","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets >= 3 && MET>150 && MET<=250","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met250to350_nj4to7","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && MET>250 && MET<=350","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met250to350_nj4to7_nb1","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets == 1 && MET>250 && MET<=350","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met250to350_nj4to7_nb2","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets == 2 && MET>250 && MET<=350","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met250to350_nj4to7_nbgeq3","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets >= 3 && MET>250 && MET<=350","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met250to350_njgeq7","minTopness","abs(DphiLepW)>1 && NJets >= 7 && MET>250 && MET<=350","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met250to350_njgeq7_nb1","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets == 1 && MET>250 && MET<=350","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met250to350_njgeq7_nb2","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets == 2 && MET>250 && MET<=350","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met250to350_njgeq7_nbgeq3","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets >= 3 && MET>250 && MET<=350","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met350to450_nj4to7","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && MET>350 && MET<=450","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met350to450_nj4to7_nb1","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets == 1 && MET>350 && MET<=450","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met350to450_nj4to7_nb2","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets == 2 && MET>350 && MET<=450","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met350to450_nj4to7_nbgeq3","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets >= 3 && MET>350 && MET<=450","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met350to450_njgeq7","minTopness","abs(DphiLepW)>1 && NJets >= 7 && MET>350 && MET<=450","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met350to450_njgeq7_nb1","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets == 1 && MET>350 && MET<=450","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met350to450_njgeq7_nb2","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets == 2 && MET>350 && MET<=450","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met350to450_njgeq7_nbgeq3","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets >= 3 && MET>350 && MET<=450","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met450to550_nj4to7","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && MET>450 && MET<=550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met450to550_nj4to7_nb1","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets == 1 && MET>450 && MET<=550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met450to550_nj4to7_nb2","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets == 2 && MET>450 && MET<=550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met450to550_nj4to7_nbgeq3","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets >= 3 && MET>450 && MET<=550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met450to550_njgeq7","minTopness","abs(DphiLepW)>1 && NJets >= 7 && MET>450 && MET<=550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met450to550_njgeq7_nb1","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets == 1 && MET>450 && MET<=550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met450to550_njgeq7_nb2","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets == 2 && MET>450 && MET<=550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met450to550_njgeq7_nbgeq3","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets >= 3 && MET>450 && MET<=550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met550_nj4to7","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && MET>550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met550_nj4to7_nb1","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets == 1 && MET>550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met550_nj4to7_nb2","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets == 2 && MET>550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met550_nj4to7_nbgeq3","minTopness","abs(DphiLepW)>1 && NJets >= 4 && NJets < 7 && NBJets >= 3 && MET>550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met550_njgeq7","minTopness","abs(DphiLepW)>1 && NJets >= 7 && MET>550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met550_njgeq7_nb1","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets == 1 && MET>550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met550_njgeq7_nb2","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets == 2 && MET>550","Topness",15,-10,20);
  myPlots3->addTreeVar("topness_met550_njgeq7_nbgeq3","minTopness","abs(DphiLepW)>1 && NJets >= 7 && NBJets >= 3 && MET>550","Topness",15,-10,20);

  myPlots3->plot();

  delete myPlots1;
  delete myPlots2;
  delete myPlots3;

}
