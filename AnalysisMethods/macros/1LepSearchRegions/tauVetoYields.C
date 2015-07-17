#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void tauVetoYields(const TString conffile="run0lep.conf",
                     const TString inputdir="run/plots_pf_fixed",
                     const TString outputdir=".")
{

  PlotStuff* tablemaker = new PlotStuff(conffile, inputdir, outputdir);

  tablemaker->setPlotSource(PlotStuff::TREES);
  tablemaker->setWgtVar("ScaleFactor");

  TString base1 = "MT>150 && MET>200 && min(jetphi1,jetphi2)>0.8 && mt2w>200 && hadronic_top_chi2<5";
  TString base2 = base1 + "&& nVetoed5GeVLeptons < 2";
  TString base3 = base2 + "&& nNewVetoCHTracks==0";

  tablemaker->addSelection("selection baseline is $MT>150$,$MET>200$,min(jetphi1,jetphi2)$>0.8$,mt2w$>200$,hadchi2$<5$",base1);
  tablemaker->addSelection("sel' base' + 5 GeV add' lep veto",base2);
  tablemaker->addSelection("sel' base' + 5 GeV add' lep veto + new CH track veto",base3);

// first set of comparisons

  tablemaker->addSelection("Old track veto on baseline1", "nOldVetoElTracks==0 && nOldVetoMuTracks==0 && "+base1);
  tablemaker->addSelection("New track veto on baseline1","nNewVetoElTracks==0 && nNewVetoMuTracks==0 && "+base1);
  tablemaker->addSelection("5 GeV additional lep on baseline1","nVetoed5GeVLeptons<2 && "+base1);
  tablemaker->addSelection("10 GeV additional lep on baseline1","nVetoed10GeVLeptons<2 && "+base1);

// second set of comparisons

  tablemaker->addSelection("Old e/mu track veto on baseline2","nOldVetoElTracks==0 && nOldVetoMuTracks==0 && "+base2);
  tablemaker->addSelection("New e/mu track veto on baseline2","nNewVetoElTracks==0 && nNewVetoMuTracks==0 && "+base2);

// third set of comparisons
/*
  tablemaker->addSelection("charged hadron new track veto on baseline2","nNewVetoCHTracks==0 && "+base2);
  tablemaker->addSelection("charged hadron old track veto on baseline2","nOldVetoCHTracks==0 && "+base2);
  tablemaker->addSelection("MVA tau veto on baseline2","nVetoMVATaus==0 && "+base2);
  tablemaker->addSelection("cut based tau veto (rel iso cut 0.1) on baseline2","nVetoCutTausISO1 == 0 && "+base2);
  tablemaker->addSelection("cut	based tau veto (rel iso	cut 0.2) on baseline2","nVetoCutTausISO2 == 0 && "+base2);
  tablemaker->addSelection("HPS tau veto on baseline2","nVetoFNALCutTaus==0 && "+base2);

// fourth set of comparisons

  tablemaker->addSelection("cut based tau veto (rel iso cut 0.1) on baseline3","nVetoCutTausISO1 == 0 && "+base3);
  tablemaker->addSelection("cut based tau veto (rel iso cut 0.2) on baseline3","nVetoCutTausISO2 == 0 && "+base3);
  tablemaker->addSelection("MVA tau veto on baseline3","nVetoMVATaus==0 && "+base3);
  tablemaker->addSelection("HPS tau veto on baseline3","nVetoFNALCutTaus==0 && "+base3);
*/

  tablemaker->tabulate();

}
