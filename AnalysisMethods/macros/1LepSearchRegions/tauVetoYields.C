#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void tauVetoYields(const TString conffile="run0lep.conf",
                     const TString inputdir="run/plots",
                     const TString outputdir=".")
{

  PlotStuff* tablemaker = new PlotStuff(conffile, inputdir, outputdir);

  tablemaker->setPlotSource(PlotStuff::TREES);
  tablemaker->setWgtVar("ScaleFactor");

  tablemaker->addSelection("10 GeV 2nd lep veto","nVetoed10GeVLeptons<2 && MT>120 && MET>200 && min(jetphi1,jetphi2)>0.8 && hadronic_top_chi2 < 5");
  tablemaker->addSelection("5 GeV 2nd lep veto","nVetoed5GeVLeptons<2 && MT>120 && MET>200 && min(jetphi1,jetphi2)>0.8 && hadronic_top_chi2 < 5");
  tablemaker->addSelection("old track veto","nOldVetoTrackss==0 && MT>120 && MET>200 && min(jetphi1,jetphi2)>0.8 && hadronic_top_chi2 < 5");
  tablemaker->addSelection("new track veto","nNewVetoTracks==0 && MT>120 && MET>200 && min(jetphi1,jetphi2)>0.8 && hadronic_top_chi2 < 5");
  tablemaker->addSelection("old track veto and 5 GeV 2nd lep veto","nOldVetoTrackss==0 && nVetoed5GeVLeptons<2 && MT>120 && MET>200 && min(jetphi1,jetphi2)>0.8 && hadronic_top_chi2 < 5");
  tablemaker->addSelection("new track veto and 5 GeV 2nd lep veto","nNewVetoTracks==0 && nVetoed5GeVLeptons<2 && MT>120 && MET>200 && min(jetphi1,jetphi2)>0.8 && hadronic_top_chi2 < 5");
  tablemaker->addSelection("5GeV 2nd lep veto and mva tau veto","nVetoed5GeVLeptons<2 && nVetoMVATaus==0 && MT>120 && MET>200 && min(jetphi1,jetphi2)>0.8 && hadronic_top_chi2 < 5");
  tablemaker->addSelection("5GeV 2nd lep veto and reliso=0.1 cut tau veto","nVetoed5GeVLeptons<2 && nVetoCutTausISO1==0 && MT>120 && MET>200 && min(jetphi1,jetphi2)>0.8 && hadronic_top_chi2 < 5");
  tablemaker->addSelection("5GeV 2nd lep veto and reliso=0.2 cut tau veto","nVetoed5GeVLeptons<2 && nVetoCutTausISO2==0 && MT>120 && MET>200 && min(jetphi1,jetphi2)>0.8 && hadronic_top_chi2 < 5");

  tablemaker->tabulate();

}
