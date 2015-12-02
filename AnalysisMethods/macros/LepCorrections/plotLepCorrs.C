#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

//root -l -b -q "../CMSSW_7_4_11/src/AnalysisMethods/macros/LepCorrections/plotLepCorrs.C+"
void plotLepCorrs( const TString inputdir="trees/151008_lepactivity"
                 , const TString outputdir="plots"
                 , const TString format = "png")
{


  TString subdir = outputdir + "/8tev";
  gSystem->mkdir(subdir, true);
  TString lumi     = "0.2092*";
  TString trigger  = "passdijetmet && passjson && passcscflt && passeebadscflt && "; // passhbhefixflt   passhbheflt
  TString baseline = "met>200 && njets60>=2 && j1pt>75 && j2pt>75 && njets>=5 && nlbjets>=2 && nbjets>=1 && dphij12met>0.4 && dphij3met>0.4";
  TString srcuts   = "met>200 && njets60>=2 && j1pt>75 && j2pt>75 && njets>=5 && nlbjets>=2 && nbjets>=1 && dphij12met>1   && dphij3met>0.5 && mtcsv12met>175";
  TString muVeto   = " && nvetomu==0";
  TString muEVeto  = " && nvetomu==0 && nvetolele==0";
  TString wtVar    = "weight*puWeight";


  //TString subdirE = outputdir + "/crZcheck";
  //gSystem->mkdir(subdirE, true);
  /*
  PlotStuff* plotE = new PlotStuff("run_Zmass.conf", "trees/151007_CRZcheck", subdirE);
  plotE->setPlotSource(PlotStuff::TREES);
  plotE->setPlotType(PlotStuff::DATAMC);
  plotE->setTree("Events");
  plotE->setDataName("edata");
  plotE->setHeaderText("#sqrt{s} = 13 TeV", "209.2 pb^{-1}", "");
  plotE->setHeaderPosition(0.5,0.92);
  plotE->setFormat(format);
  plotE->setWgtVar(lumi+wtVar);
  plotE->setPlotOverflow(true);
  //plotE->setLogy();
  //plotE->setScaleToData();
  //plotE->setSigScale(-1);
  plotE->setRatioPlot(true);
  //plotE->setColor("ttV-tW-25ns-0LSR", 797);
  //plotE->setColor("ttbar-powheg-50ns-0LSR_tau3p", 4); // blue
  //plotE->setColor("ttbar-powheg-25ns-0LSR_tau3p", 4); // blue
  //plotE->setColor("ttbar-powheg-50ns-0LSR_taufake", 7); // cyan
  //plotE->setColor("ttbar-powheg-25ns-0LSR_taufake", 7); // cyan
  //plotE->setColor("T2tt_650_325", 2); // red
  //plotE->setColor("T2tt_500_325", 6); // magenta
  TString ebase = "weight>0 && passtrige && abs(leptonpdgid)==11 && abs(lepton2pdgid)==11 && leptonpt>35 && abs(leptoneta)<2.1 && lepton2pt>35 && abs(lepton2eta)<2.1 && nvetolele>=2 && nvetomu==0";
  plotE->addTreeVar("lepton2pt"          , "lepton2pt"      , ebase                                    , "lepton2pt (full baseline)" ,  20, 0,  200 );
  plotE->addTreeVar("lepton2pt_0_zmass"  , "lepton2pt"      , ebase+" && dilepmass>60 && dilepmass<120", "lepton2pt (no z mass)"     ,  20, 0,  200 );
  plotE->addTreeVar("leptonpt"           , "leptonpt"       , ebase                                    , "leptonpt (full baseline)"  ,  20, 0,  200 );
  plotE->addTreeVar("leptonpt_0_zmass"   , "leptonpt"       , ebase+" && dilepmass>60 && dilepmass<120", "leptonpt (no z mass)"      ,  20, 0,  200 );
  plotE->addTreeVar("lepton2eta"         , "abs(lepton2eta)", ebase                                    , "lepton2eta (full baseline)",  31,  0,   3.14 );
  plotE->addTreeVar("lepton2eta_0_zmass" , "abs(lepton2eta)", ebase+" && dilepmass>60 && dilepmass<120", "lepton2eta (no z mass)"    ,  31,  0,   3.14 );
  plotE->addTreeVar("leptoneta"          , "abs(leptoneta)" , ebase                                    , "leptoneta (full baseline)" ,  31,  0,   3.14 );
  plotE->addTreeVar("leptoneta_0_zmass"  , "abs(leptoneta)" , ebase+" && dilepmass>60 && dilepmass<120", "leptoneta (no z mass)"     ,  31,  0,   3.14 );
  //plotE->addTreeVar("htalonglep"         , "htalonglep"     , ebase                                    , "htalonglep (full baseline)",  30,  0, 300    );
  //plotE->addTreeVar("htalonglep_0_zmass" , "htalonglep"     , ebase+" && dilepmass>60 && dilepmass<120", "htalonglep (no z mass)"    ,  30,  0, 300    );
  plotE->addTreeVar("dilepmass"          , "dilepmass"      , ebase                                    , "dilepmass"                 ,  30, 60, 120 );
  plotE->plot();
  delete plotE;
  // */

  /*
  PlotStuff* plotMu = new PlotStuff("run_Zmass.conf", "trees/151007_CRZcheck", subdirE);
  plotMu->setPlotSource(PlotStuff::TREES);
  plotMu->setPlotType(PlotStuff::DATAMC);
  plotMu->setTree("Events");
  plotMu->setDataName("mudata");
  plotMu->setHeaderText("#sqrt{s} = 13 TeV", "209.2 pb^{-1}", "");
  plotMu->setHeaderPosition(0.5,0.92);
  plotMu->setFormat(format);
  plotMu->setWgtVar(lumi+wtVar);
  plotMu->setPlotOverflow(true);
  //plotE->setLogy();
  //plotE->setScaleToData();
  //plotE->setSigScale(-1);
  plotMu->setRatioPlot(true);
  //plotE->setColor("ttV-tW-25ns-0LSR", 797);
  //plotE->setColor("ttbar-powheg-50ns-0LSR_tau3p", 4); // blue
  //plotE->setColor("ttbar-powheg-25ns-0LSR_tau3p", 4); // blue
  //plotE->setColor("ttbar-powheg-50ns-0LSR_taufake", 7); // cyan
  //plotE->setColor("ttbar-powheg-25ns-0LSR_taufake", 7); // cyan
  //plotE->setColor("T2tt_650_325", 2); // red
  //plotE->setColor("T2tt_500_325", 6); // magenta
  TString mubase = "passtrigmu && leptonmatchtrigmu && abs(leptonpdgid)==13 && abs(lepton2pdgid)==13 && leptonpt>27 && abs(leptoneta)<2.1 && lepton2pt>27 && abs(lepton2eta)<2.1 && nvetomu>=2 && nvetolele==0";
  plotMu->addTreeVar("mu_lepton2pt"          , "lepton2pt"      , mubase                                    , "lepton2pt (full baseline)" ,  20, 0,  200 );
  plotMu->addTreeVar("mu_lepton2pt_0_zmass"  , "lepton2pt"      , mubase+" && dilepmass>60 && dilepmass<120", "lepton2pt (no z mass)"     ,  20, 0,  200 );
  plotMu->addTreeVar("mu_leptonpt"           , "leptonpt"       , mubase                                    , "leptonpt (full baseline)"  ,  20, 0,  200 );
  plotMu->addTreeVar("mu_leptonpt_0_zmass"   , "leptonpt"       , mubase+" && dilepmass>60 && dilepmass<120", "leptonpt (no z mass)"      ,  20, 0,  200 );
  plotMu->addTreeVar("mu_lepton2eta"         , "abs(lepton2eta)", mubase                                    , "lepton2eta (full baseline)",  31,  0,   3.14 );
  plotMu->addTreeVar("mu_lepton2eta_0_zmass" , "abs(lepton2eta)", mubase+" && dilepmass>60 && dilepmass<120", "lepton2eta (no z mass)"    ,  31,  0,   3.14 );
  plotMu->addTreeVar("mu_leptoneta"          , "abs(leptoneta)" , mubase                                    , "leptoneta (full baseline)" ,  31,  0,   3.14 );
  plotMu->addTreeVar("mu_leptoneta_0_zmass"  , "abs(leptoneta)" , mubase+" && dilepmass>60 && dilepmass<120", "leptoneta (no z mass)"     ,  31,  0,   3.14 );
  //plotMu->addTreeVar("mu_htalonglep"         , "htalonglep"     , mubase                                    , "htalonglep (full baseline)",  30,  0, 300    );
  //plotMu->addTreeVar("mu_htalonglep_0_zmass" , "htalonglep"     , mubase+" && dilepmass>60 && dilepmass<120", "htalonglep (no z mass)"    ,  30,  0, 300    );
  plotMu->addTreeVar("mu_dilepmass"          , "dilepmass"      , mubase                                    , "dilepmass"                 ,  30, 60, 120 );
  plotMu->plot();
  delete plotMu;
  // */

  /*
  PlotStuff* tablemaker = new PlotStuff("run_50ns.conf", inputdir, subdir);
  tablemaker->setPlotSource(PlotStuff::TREES);
  tablemaker->setWgtVar(lumi50+wtVar50);
  tablemaker->addSelection("mu inc" ,trigger+baseline+" && ngoodgenmu==1");
  tablemaker->addSelection("mu 0"   ,trigger+baseline+" && ngoodgenmu==1 && nvetomu==0");
  tablemaker->addSelection("mu 1+"  ,trigger+baseline+" && ngoodgenmu==1 && nvetomu>=1");
  tablemaker->addSelection("el inc" ,trigger+baseline+" && ngoodgenele==1");
  tablemaker->addSelection("el 0"   ,trigger+baseline+" && ngoodgenele==1 && nvetolele==0");
  tablemaker->addSelection("el 1+"  ,trigger+baseline+" && ngoodgenele==1 && nvetolele>=1");
  tablemaker->tabulate();
  delete tablemaker;
  return;
  // */

  //*
  PlotStuff* plots50ns = new PlotStuff("plotLepCor_8tev.conf", inputdir, subdir);
  plots50ns->setPlotSource(PlotStuff::TREES);
  plots50ns->setPlotType(PlotStuff::DATAMC);
  plots50ns->setTree("Events");
  plots50ns->setDataName("data");
  plots50ns->setHeaderText("#sqrt{s} = 13 TeV", "209.2 pb^{-1}", "");
  plots50ns->setHeaderPosition(0.5,0.92);
  plots50ns->setFormat(format);
  plots50ns->setWgtVar(lumi+wtVar);
  plots50ns->setPlotOverflow(true);
  //plots50ns->setLogy();
  //plots50ns->setScaleToData();
  //plots50ns->setSigScale(-1);
  plots50ns->setRatioPlot(true);
  plots50ns->setColor("ttV-tW-0LSR", 797);
  plots50ns->setColor("ttbar-powheg-50ns-0LSR_tau3p", 4); // blue
  plots50ns->setColor("ttbar-powheg-25ns-0LSR_tau3p", 4); // blue
  plots50ns->setColor("ttbar-powheg-50ns-0LSR_taufake", 7); // cyan
  plots50ns->setColor("ttbar-powheg-25ns-0LSR_taufake", 7); // cyan
  //plots50ns->setColor("T2tt_650_325", 2); // red
  //plots50ns->setColor("T2tt_500_325", 6); // magenta

  plots50ns->addTreeVar("nvetomu_base"    , "nvetomu"  , trigger+baseline        , "nvetomu (baseline)"    ,  2, -0.5,   1.5);
  plots50ns->addTreeVar("nvetolele_base"  , "nvetolele", trigger+baseline        , "nvetolele (baseline)"  ,  2, -0.5,   1.5);
  plots50ns->addTreeVar("nvetotau_base"   , "nvetotau" , trigger+baseline        , "nvetotau (baseline)"   ,  2, -0.5,   1.5);
  plots50ns->addTreeVar("nvetolele_lepsel", "nvetolele", trigger+baseline+muVeto , "nvetolele (nMu=0)"     ,  2, -0.5,   1.5);
  plots50ns->addTreeVar("nvetotau_lepsel" , "nvetotau" , trigger+baseline+muEVeto, "nvetotau (nMu=0, nE=0)",  2, -0.5,   1.5);
  plots50ns->addTreeVar("nvetohpstaus_base"   , "nvetohpstaus", trigger+baseline        , "nvetohpstaus (baseline)"   ,  2, -0.5,   1.5);
  plots50ns->addTreeVar("nvetohpstaus_muEVeto", "nvetohpstaus", trigger+baseline+muEVeto, "nvetohpstaus (nMu=0, nE=0)",  2, -0.5,   1.5);

  //plots50ns->addTreeVar("ngoodgenmu_base"    , "ngoodgenmu"   , trigger+baseline , "ngoodgenmu (baseline)"   ,  2, -0.5,   1.5);
  //plots50ns->addTreeVar("ngoodgenele_base"   , "ngoodgenele"  , trigger+baseline , "ngoodgenele (baseline)"  ,  2, -0.5,   1.5);
  //plots50ns->addTreeVar("npromptgentau_base" , "npromptgentau", trigger+baseline , "npromptgentau (baseline)",  2, -0.5,   1.5);

  plots50ns->addTreeVar("leptonpt_mu", "leptonpt" , trigger+baseline+" && nvetomu>=1"                , "lepton pt (muon)"     ,  20, 0, 200);
  plots50ns->addTreeVar("leptonpt_e" , "leptonpt" , trigger+baseline+" && nvetolele>=1 && nvetomu==0", "lepton pt (electron)" ,  20, 0, 200);
  plots50ns->addTreeVar("leptonpt_sr_mu", "leptonpt" , trigger+srcuts+" && nvetomu>=1"                , "lepton pt (sr, muon)"     ,  20, 0, 200);
  plots50ns->addTreeVar("leptonpt_sr_e" , "leptonpt" , trigger+srcuts+" && nvetolele>=1 && nvetomu==0", "lepton pt (sr, electron)" ,  20, 0, 200);
  plots50ns->addTreeVar("leptonpt_inc", "leptonpt" , trigger+baseline, "lepton pt (inc, muon)"     ,  20, 0, 200);

  plots50ns->addTreeVar("leptoneta_mu", "abs(leptoneta)" , trigger+baseline+" && nvetomu>=1"                , "lepton eta (muon)"     ,  31, 0, 3.14);
  plots50ns->addTreeVar("leptoneta_e" , "abs(leptoneta)" , trigger+baseline+" && nvetolele>=1 && nvetomu==0", "lepton eta (electron)" ,  31, 0, 3.14);
  plots50ns->addTreeVar("leptoneta_sr_mu", "abs(leptoneta)" , trigger+srcuts+" && nvetomu>=1"                , "lepton eta (sr, muon)"     ,  31, 0, 3.14);
  plots50ns->addTreeVar("leptoneta_sr_e" , "abs(leptoneta)" , trigger+srcuts+" && nvetolele>=1 && nvetomu==0", "lepton eta (sr, electron)" ,  31, 0, 3.14);
  plots50ns->addTreeVar("leptoneta_inc", "abs(leptoneta)" , trigger+srcuts+" && (nvetomu>=1 || nvetolele>=1)", "lepton eta (inc, muon)"     ,  31, 0, 3.14);

  plots50ns->addTreeVar("htalong_mu", "htalonglep" , trigger+baseline+" && nvetomu>=1"                , "htalong (muon)"     ,  20, 0, 1000);
  plots50ns->addTreeVar("htalong_e" , "htalonglep" , trigger+baseline+" && nvetolele>=1 && nvetomu==0", "htalong (electron)" ,  20, 0, 1000);
  plots50ns->addTreeVar("htalong_sr_mu", "htalonglep" , trigger+srcuts+" && nvetomu>=1"                , "htalong (sr, muon)"     ,  20, 0, 1000);
  plots50ns->addTreeVar("htalong_sr_e" , "htalonglep" , trigger+srcuts+" && nvetolele>=1 && nvetomu==0", "htalong (sr, electron)" ,  20, 0, 1000);
  plots50ns->addTreeVar("htalong_inc" , "htalonglep" , trigger+srcuts, "htalong (inc, electron)" ,  20, 0, 1000);

  plots50ns->addTreeVar("annulus_mu", "annulus" , trigger+baseline+" && nvetomu>=1"                , "annulus (muon)"     ,  20, 0, 10);
  plots50ns->addTreeVar("annulus_e" , "annulus" , trigger+baseline+" && nvetolele>=1 && nvetomu==0", "annulus (electron)" ,  20, 0, 10);
  plots50ns->addTreeVar("annulus_sr_mu", "annulus" , trigger+srcuts+" && nvetomu>=1"                , "annulus (sr, muon)"     ,  20, 0, 10);
  plots50ns->addTreeVar("annulus_sr_e" , "annulus" , trigger+srcuts+" && nvetolele>=1 && nvetomu==0", "annulus (sr, electron)" ,  20, 0, 10);
  plots50ns->addTreeVar("annulus_inc" , "annulus" , trigger+srcuts, "annulus (inc, electron)" ,  20, 0, 10);

  plots50ns->plot();
  delete plots50ns;
  // */

  /*
  plots50ns->addTreeVar("chhpt_base"   , "chhpt", trigger+baseline        , "chhpt (baseline)"   ,  20, 10, 50);
  plots50ns->addTreeVar("chhpt_muEVeto", "chhpt", trigger+baseline+muEVeto, "chhpt (nMu=0, nE=0)",  20, 10, 50);
  plots50ns->addTreeVar("chheta_base"   , "abs(chheta)", trigger+baseline        , "chheta (baseline)"   ,  24, 0, 2.4);
  plots50ns->addTreeVar("chheta_muEVeto", "abs(chheta)", trigger+baseline+muEVeto, "chheta (nMu=0, nE=0)",  24, 0, 2.4);
  plots50ns->addTreeVar("chhdz_base"   , "abs(chhdz)", trigger+baseline        , "chhdz (baseline)"   ,  20, 0, 0.1);
  plots50ns->addTreeVar("chhdz_muEVeto", "abs(chhdz)", trigger+baseline+muEVeto, "chhdz (nMu=0, nE=0)",  20, 0, 0.1);
  plots50ns->addTreeVar("chhmt_base"   , "chhmt", trigger+baseline        , "chhmt (baseline)"   ,  20, 0, 500);
  plots50ns->addTreeVar("chhmt_muEVeto", "chhmt", trigger+baseline+muEVeto, "chhmt (nMu=0, nE=0)",  20, 0, 500);
  plots50ns->addTreeVar("chhmt_mva55_ase"   , "chhmt", trigger+"chhtaudisc>.55 && "+baseline        , "chhmt (baseline)"   ,  20, 0, 500);
  plots50ns->addTreeVar("chhmt_mva55_uEVeto", "chhmt", trigger+"chhtaudisc>.55 && "+baseline+muEVeto, "chhmt (nMu=0, nE=0)",  20, 0, 500);
  plots50ns->addTreeVar("chhdphimet_base"   , "abs(chhdphimet)", trigger+baseline        , "chhdphimet (baseline)"   ,  31, 0, 3.14);
  plots50ns->addTreeVar("chhdphimet_muEVeto", "abs(chhdphimet)", trigger+baseline+muEVeto, "chhdphimet (nMu=0, nE=0)",  31, 0, 3.14);
  plots50ns->addTreeVar("chhtaudisc_base"   , "chhtaudisc", trigger+baseline        , "chhtaudisc (baseline)"   ,  20, -1, 1);
  plots50ns->addTreeVar("chhtaudisc_muEVeto", "chhtaudisc", trigger+baseline+muEVeto, "chhtaudisc (nMu=0, nE=0)",  20, -1, 1);
  plots50ns->addTreeVar("chhtaudisc_mtdz_base"   , "chhtaudisc", trigger+"abs(chhdz)<.2 && chhmt<100 && "+baseline        , "chhtaudisc (mtdx, baseline)"   ,  20, -1, 1);
  plots50ns->addTreeVar("chhtaudisc_mtdz_muEVeto", "chhtaudisc", trigger+"abs(chhdz)<.2 && chhmt<100 && "+baseline+muEVeto, "chhtaudisc (mtdx, nMu=0, nE=0)",  20, -1, 1);

  plots50ns->addTreeVar("chiso0p1_base"   , "chiso0p1", trigger+baseline        , "chiso0p1 (baseline)"   ,  20, 0, 500);
  plots50ns->addTreeVar("chiso0p1_muEVeto", "chiso0p1", trigger+baseline+muEVeto, "chiso0p1 (nMu=0, nE=0)",  20, 0, 500);
  plots50ns->addTreeVar("chiso0p2_base"   , "chiso0p2", trigger+baseline        , "chiso0p2 (baseline)"   ,  20, 0, 500);
  plots50ns->addTreeVar("chiso0p2_muEVeto", "chiso0p2", trigger+baseline+muEVeto, "chiso0p2 (nMu=0, nE=0)",  20, 0, 500);
  plots50ns->addTreeVar("chiso0p3_base"   , "chiso0p3", trigger+baseline        , "chiso0p3 (baseline)"   ,  20, 0, 500);
  plots50ns->addTreeVar("chiso0p3_muEVeto", "chiso0p3", trigger+baseline+muEVeto, "chiso0p3 (nMu=0, nE=0)",  20, 0, 500);
  plots50ns->addTreeVar("chiso0p4_base"   , "chiso0p4", trigger+baseline        , "chiso0p4 (baseline)"   ,  20, 0, 500);
  plots50ns->addTreeVar("chiso0p4_muEVeto", "chiso0p4", trigger+baseline+muEVeto, "chiso0p4 (nMu=0, nE=0)",  20, 0, 500);
  plots50ns->addTreeVar("totiso0p1_base"   , "totiso0p1", trigger+baseline        , "totiso0p1 (baseline)"   ,  20, 0, 800);
  plots50ns->addTreeVar("totiso0p1_muEVeto", "totiso0p1", trigger+baseline+muEVeto, "totiso0p1 (nMu=0, nE=0)",  20, 0, 800);
  plots50ns->addTreeVar("totiso0p2_base"   , "totiso0p2", trigger+baseline        , "totiso0p2 (baseline)"   ,  20, 0, 800);
  plots50ns->addTreeVar("totiso0p2_muEVeto", "totiso0p2", trigger+baseline+muEVeto, "totiso0p2 (nMu=0, nE=0)",  20, 0, 800);
  plots50ns->addTreeVar("totiso0p3_base"   , "totiso0p3", trigger+baseline        , "totiso0p3 (baseline)"   ,  20, 0, 800);
  plots50ns->addTreeVar("totiso0p3_muEVeto", "totiso0p3", trigger+baseline+muEVeto, "totiso0p3 (nMu=0, nE=0)",  20, 0, 800);
  plots50ns->addTreeVar("totiso0p4_base"   , "totiso0p4", trigger+baseline        , "totiso0p4 (baseline)"   ,  20, 0, 800);
  plots50ns->addTreeVar("totiso0p4_muEVeto", "totiso0p4", trigger+baseline+muEVeto, "totiso0p4 (nMu=0, nE=0)",  20, 0, 800);
  plots50ns->addTreeVar("neartrkdr_base"   , "abs(neartrkdr)", trigger+baseline        , "neartrkdr (baseline)"   ,  20, 0, .2);
  plots50ns->addTreeVar("neartrkdr_muEVeto", "abs(neartrkdr)", trigger+baseline+muEVeto, "neartrkdr (nMu=0, nE=0)",  20, 0, .2);
  plots50ns->addTreeVar("contjetdr_base"   , "abs(contjetdr)", trigger+baseline        , "contjetdr (baseline)"   ,  20, 0, .4);
  plots50ns->addTreeVar("contjetdr_muEVeto", "abs(contjetdr)", trigger+baseline+muEVeto, "contjetdr (nMu=0, nE=0)",  20, 0, .4);
  plots50ns->addTreeVar("contjetcsv_base"   , "contjetcsv", trigger+baseline        , "contjetcsv (baseline)"   ,  20, 0, 1);
  plots50ns->addTreeVar("contjetcsv_muEVeto", "contjetcsv", trigger+baseline+muEVeto, "contjetcsv (nMu=0, nE=0)",  20, 0, 1);

  plots50ns->addTreeVar("csvj1pt_base"   , "csvj1pt", trigger+baseline        , "csvj1pt (baseline)"   ,  20, 0, 500);
  plots50ns->addTreeVar("csvj1pt_muEVeto", "csvj1pt", trigger+baseline+muEVeto, "csvj1pt (nMu=0, nE=0)",  20, 0, 500);
  plots50ns->addTreeVar("csvj2pt_base"   , "csvj2pt", trigger+baseline        , "csvj2pt (baseline)"   ,  20, 0, 500);
  plots50ns->addTreeVar("csvj2pt_muEVeto", "csvj2pt", trigger+baseline+muEVeto, "csvj2pt (nMu=0, nE=0)",  20, 0, 500);
  plots50ns->addTreeVar("csv1_base"   , "csv1", trigger+baseline        , "csv1 (baseline)"   ,  20, .8, 1);
  plots50ns->addTreeVar("csv1_muEVeto", "csv1", trigger+baseline+muEVeto, "csv1 (nMu=0, nE=0)",  20, .8, 1);
  plots50ns->addTreeVar("csv2_base"   , "csv2", trigger+baseline        , "csv2 (baseline)"   ,  20, .5, 1);
  plots50ns->addTreeVar("csv2_muEVeto", "csv2", trigger+baseline+muEVeto, "csv2 (nMu=0, nE=0)",  20, .5, 1);
  // */

}
