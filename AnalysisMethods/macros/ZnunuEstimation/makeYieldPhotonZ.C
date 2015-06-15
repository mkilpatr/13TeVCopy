#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void makeYieldPhotonZ(const TString conffile="runPhotonZ.conf",
    const TString inputdir="../run/plots/",
    const TString outputdir="../run/plots/")
{

  TString etaCut       = "genBosonEta < 2.4 && genBosonEta>-2.4 && passLoosePhotons==1";
  TString all          = etaCut;
  TString preselection = all + " && NVetoLeps==0 && NVetoTaus==0 && NJets>=5 && NBJets>=1 && MET>200";
  TString baseline     = preselection + " && dPhiMET12>1 && dPhiMET3>0.5 && mtB1MET>175 && mtB2MET>175";

  map<TString, TString> NBbins{
    {"NB1", "NBJets==1"}, {"NB2", "NBJets>=2"}
  };
  map<TString, TString> NCTTbins{
    {"NCTT0", "NCTTstd==0"}, {"NCTT1", "NCTTstd>=1"}
  };
  map<TString, TString> METbins{
    {"MET200to300", "MET>=200 && MET<300"}, {"MET300to400", "MET>=300 && MET<400"},{"MET400to500", "MET>=400 && MET<500"},{"MET500to600", "MET>=500 && MET<600"},{"MET600toInf", "MET>=600"}
  };


  {
    PlotStuff* myPlots = new PlotStuff(conffile, inputdir, outputdir);
    myPlots->setPlotSource(PlotStuff::TREES);
    myPlots->setWgtVar("ScaleFactor");

    for (auto nb : NBbins){
      for (auto nt : NCTTbins){
        for (auto met : METbins){
          TString prefix = nb.first+"_"+nt.first+"_"+met.first+"_";
          TString cut = nb.second + " && " + nt.second + " && " + met.second + " && ";
          cout << prefix << "::" << cut+baseline << endl;
          myPlots->addSelection(prefix, cut+baseline);
        }
      }
    }

    myPlots->tabulate();
    delete myPlots;
  }


}
