#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

//root -l -b -q "../CMSSW_7_4_11/src/AnalysisMethods/macros/TnP/plotTnP.C+"
void plotTnP( const TString indir     = "trees/151006_newNtuples2015d"
            , const TString outdir    = "fitResults/plots/2015d"
            , const TString format    = "png"
            )
{

  TString lumi   = "0.2092*";
  TString wtVar  = "weight";
  TString cuts   = "pass";

  bool logy = false;

  gSystem->mkdir(outdir, true);

  //*
  PlotStuff* plotMu = new PlotStuff("plotTnPmu.conf", indir, outdir);
  plotMu->setPlotSource(PlotStuff::TREES);
  plotMu->setPlotType(PlotStuff::DATAMC);
  plotMu->setTree("Probes");
  plotMu->setDataName("singlemu");
  plotMu->setHeaderText("#sqrt{s} = 13 TeV", "209.2 pb^{-1}", "");
  plotMu->setHeaderPosition(0.5,0.92);
  plotMu->setFormat(format);
  plotMu->setWgtVar(lumi+wtVar);
  plotMu->setPlotOverflow(true);
  plotMu->setRatioPlot(true);
  if(logy) plotMu->setLogy();

  plotMu->addTreeVar("mu_htalong"   , "htalong" , cuts   , "probe htalong"     ,  20,  0,1000   );

  if(!logy){
    plotMu->addTreeVar("mu_pt"        , "pt"      , cuts   , "probe pt"          ,  20,  0, 200    );
    plotMu->addTreeVar("mu_eta"       , "abseta"  , cuts   , "probe eta"         ,  31,  0,   3.14 );
    plotMu->addTreeVar("mu_htalong"   , "htalong" , cuts   , "probe htalong"     ,  20,  0,1000   );
    plotMu->addTreeVar("mu_annulus"   , "annulus" , cuts   , "probe annulus"     ,  20,  0,  10  );
    plotMu->addTreeVar("mu_mass"      , "mass"    , cuts   , "dilep mass"        ,  30, 60, 120    ); // +"&& pt>27"
    plotMu->addTreeVar("mu_mass_first", "mass"    , cuts+" && matchnum==1"   , "dilep mass (first pair)",  30, 60, 120    );
    plotMu->addTreeVar("mu_matchnum"  , "matchnum", "pass" , "number of matches" ,   5, .5,  5.5   );
    plotMu->addTreeVar("mu_nopass_pt"        , "pt"      , "!pass" , "probe pt"      ,  20,  0, 200    );
    plotMu->addTreeVar("mu_all_pt"        , "pt"      , "1==1" , "probe pt"      ,  20,  0, 200    );
    //plotMu->addTreeVar("nopass_eta"       , "abseta"  , "1==1" , "probe eta"     ,  31,  0,   3.14 );
    //plotMu->addTreeVar("nopass_htalong"   , "htalong" , "1==1" , "probe htalong" ,  30,  0, 300    );
    //plotMu->addTreeVar("nopass_mass"      , "mass"    , "1==1" , "dilep mass"    ,  30, 60, 120    );
    //plotMu->addTreeVar("cr2l_pt"        , "pt"      , "pass && pt>10 && abseta<2.1" , "probe pt"      ,  20,  0, 200    );
  }

  plotMu->plot();
  delete plotMu;

  // */
  PlotStuff* plotEl = new PlotStuff("plotTnPel.conf", indir, outdir);
  plotEl->setPlotSource(PlotStuff::TREES);
  plotEl->setPlotType(PlotStuff::DATAMC);
  plotEl->setTree("Probes");
  plotEl->setDataName("singleel");
  plotEl->setHeaderText("#sqrt{s} = 13 TeV", "209.2 pb^{-1}", "");
  plotEl->setHeaderPosition(0.5,0.92);
  plotEl->setFormat(format);
  plotEl->setWgtVar(lumi+wtVar);
  plotEl->setPlotOverflow(true);
  plotEl->setRatioPlot(true);
  if(logy) plotEl->setLogy();

  plotEl->addTreeVar("el_htalong"   , "htalong" , cuts   , "probe htalong"     ,  20,  0,1000   );

  if(!logy){
    plotEl->addTreeVar("el_pt"        , "pt"      , cuts   , "probe pt"          ,  20,  0, 200   );
    plotEl->addTreeVar("el_eta"       , "abseta"  , cuts   , "probe eta"         ,  31,  0,   3.14);
    plotEl->addTreeVar("el_annulus"   , "annulus" , cuts   , "probe annulus"     ,  20,  0,  10  );
    plotEl->addTreeVar("el_mass"      , "mass"    , cuts   , "dilep mass"        ,  30, 60, 120   ); // +" && pt>35"
    plotEl->addTreeVar("el_mass_first", "mass"    , cuts+" && matchnum==1"   , "dilep mass (first pair)",  30, 60, 120   ); // +" && pt>35"
    plotEl->addTreeVar("el_matchnum"  , "matchnum", "pass" , "number of matches" ,   5, .5,  5.5   );
    plotEl->addTreeVar("el_nopass_pt"        , "pt"      , "!pass" , "probe pt"      ,  20,  0, 200   );
    plotEl->addTreeVar("el_all_pt"           , "pt"      , "1==1" , "probe pt"      ,  20,  0, 200   );
    //plotEl->addTreeVar("nopass_eta"       , "abseta"  , "1==1" , "probe eta"     ,  31,  0,   3.14);
    //plotEl->addTreeVar("nopass_htalong"   , "htalong" , "1==1" , "probe htalong" ,  30,  0, 300   );
    //plotEl->addTreeVar("nopass_mass"      , "mass"    , "1==1" , "dilep mass"    ,  30, 60, 120   );
    //plotEl->addTreeVar("cr2l_pt"        , "pt"      , "pass && pt>10 && abseta<2.1" , "probe pt"      ,  20,  0, 200    );
  }

  plotEl->plot();
  delete plotEl;

}
