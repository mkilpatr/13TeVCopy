#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotSearchRegions(const TString conffile="runPhotonZ.conf",
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
    {"METinc", "MET>-1"}, {"MET200to300", "MET>=200 && MET<300"}, {"MET300to400", "MET>=300 && MET<400"},{"MET400to500", "MET>=400 && MET<500"},{"MET500to600", "MET>=500 && MET<600"},{"MET600toInf", "MET>=600"}
  };


  {
    PlotStuff* myPlots = new PlotStuff(conffile, inputdir, outputdir);
    myPlots->setPlotSource(PlotStuff::TREES);
    myPlots->setPlotType(PlotStuff::COMP);
    myPlots->setFormat("pdf");
//    myPlots->setUnitScale();
    myPlots->setWgtVar("ScaleFactor");
    myPlots->setDrawOption("histE");

    myPlots->addTreeVar("baseline_genBosonPT", "genBosonPT", baseline, "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("baseline_genMET", "GenMET", baseline, "genMET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("baseline_numGenJet", "NGenJets", baseline, "N(gen ak4 Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("baseline_numGenBJets", "NGenBJets", baseline, "N(gen ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("baseline_MET", "MET", baseline, "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("baseline_numJets", "NJets", baseline, "N(ak4Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("baseline_numBJets", "NBJets", baseline, "N(ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("baseline_dPhiMET12","dPhiMET12", baseline, "dPhiMET12", 30, 0, 3);
    myPlots->addTreeVar("baseline_dPhiMET3","dPhiMET3", baseline, "dPhiMET3", 30, 0, 3);
    myPlots->addTreeVar("baseline_mtB1MET", "mtB1MET", baseline, "m_{T}(B1,MET) [GeV]", 20, 0, 1000);
    myPlots->addTreeVar("baseline_mtB2MET", "mtB2MET", baseline, "m_{T}(B2,MET) [GeV]", 20, 0, 1000);
    myPlots->addTreeVar("baseline_NCTTstd", "NCTTstd", baseline, "NCTTstd", 5, -0.5, 4.5);

    for (auto nb : NBbins){
      for (auto nt : NCTTbins){
        for (auto met : METbins){
          TString prefix = nb.first+"_"+nt.first+"_"+met.first+"_";
          TString cut = nb.second + " && " + nt.second + " && " + met.second + " && ";
          cout << prefix << "::" << cut+baseline << endl;

          myPlots->addTreeVar(prefix+"genBosonPT", "genBosonPT", cut+baseline, "boson PT [GeV]", 18, 100, 1000);
          myPlots->addTreeVar(prefix+"genMET", "GenMET", cut+baseline, "genMET [GeV]", 18, 100, 1000);
          myPlots->addTreeVar(prefix+"numGenJet", "NGenJets", cut+baseline, "N(gen ak4 Jets)", 10, -0.5, 9.5);
          myPlots->addTreeVar(prefix+"numGenBJets", "NGenBJets", cut+baseline, "N(gen ak4 BJets)", 5, -0.5, 4.5);
          myPlots->addTreeVar(prefix+"MET", "MET", cut+baseline, "MET [GeV]", 18, 100, 1000);
          myPlots->addTreeVar(prefix+"numJets", "NJets", cut+baseline, "N(ak4Jets)", 10, -0.5, 9.5);
          myPlots->addTreeVar(prefix+"numBJets", "NBJets", cut+baseline, "N(ak4 BJets)", 5, -0.5, 4.5);
          myPlots->addTreeVar(prefix+"dPhiMET12","dPhiMET12", cut+baseline, "dPhiMET12", 30, 0, 3);
          myPlots->addTreeVar(prefix+"dPhiMET3","dPhiMET3", cut+baseline, "dPhiMET3", 30, 0, 3);
          myPlots->addTreeVar(prefix+"mtB1MET", "mtB1MET", cut+baseline, "m_{T}(B1,MET) [GeV]", 20, 0, 1000);
          myPlots->addTreeVar(prefix+"mtB2MET", "mtB2MET", cut+baseline, "m_{T}(B2,MET) [GeV]", 20, 0, 1000);
          myPlots->addTreeVar(prefix+"NCTTstd", "NCTTstd", cut+baseline, "NCTTstd", 5, -0.5, 4.5);

        }
      }
    }

    myPlots->plot();
    delete myPlots;
  }

  {
    PlotStuff* myPlots = new PlotStuff(conffile, inputdir, outputdir);
    myPlots->setPlotSource(PlotStuff::TREES);
    myPlots->setPlotType(PlotStuff::NORMCOMP);
    myPlots->setFormat("pdf");
    myPlots->setWgtVar("ScaleFactor");
    myPlots->setDrawOption("histE");

    myPlots->addTreeVar("baseline_genBosonPT", "genBosonPT", baseline, "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("baseline_genMET", "GenMET", baseline, "genMET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("baseline_numGenJet", "NGenJets", baseline, "N(gen ak4 Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("baseline_numGenBJets", "NGenBJets", baseline, "N(gen ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("baseline_MET", "MET", baseline, "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("baseline_numJets", "NJets", baseline, "N(ak4Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("baseline_numBJets", "NBJets", baseline, "N(ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("baseline_dPhiMET12","dPhiMET12", baseline, "dPhiMET12", 30, 0, 3);
    myPlots->addTreeVar("baseline_dPhiMET3","dPhiMET3", baseline, "dPhiMET3", 30, 0, 3);
    myPlots->addTreeVar("baseline_mtB1MET", "mtB1MET", baseline, "m_{T}(B1,MET) [GeV]", 20, 0, 1000);
    myPlots->addTreeVar("baseline_mtB2MET", "mtB2MET", baseline, "m_{T}(B2,MET) [GeV]", 20, 0, 1000);
    myPlots->addTreeVar("baseline_NCTTstd", "NCTTstd", baseline, "NCTTstd", 5, -0.5, 4.5);

    for (auto nb : NBbins){
      for (auto nt : NCTTbins){
        for (auto met : METbins){
          TString prefix = nb.first+"_"+nt.first+"_"+met.first+"_";
          TString cut = nb.second + " && " + nt.second + " && " + met.second + " && ";

          myPlots->addTreeVar(prefix+"genBosonPT", "genBosonPT", cut+baseline, "boson PT [GeV]", 18, 100, 1000);
          myPlots->addTreeVar(prefix+"genMET", "GenMET", cut+baseline, "genMET [GeV]", 18, 100, 1000);
          myPlots->addTreeVar(prefix+"numGenJet", "NGenJets", cut+baseline, "N(gen ak4 Jets)", 10, -0.5, 9.5);
          myPlots->addTreeVar(prefix+"numGenBJets", "NGenBJets", cut+baseline, "N(gen ak4 BJets)", 5, -0.5, 4.5);
          myPlots->addTreeVar(prefix+"MET", "MET", cut+baseline, "MET [GeV]", 18, 100, 1000);
          myPlots->addTreeVar(prefix+"numJets", "NJets", cut+baseline, "N(ak4Jets)", 10, -0.5, 9.5);
          myPlots->addTreeVar(prefix+"numBJets", "NBJets", cut+baseline, "N(ak4 BJets)", 5, -0.5, 4.5);
          myPlots->addTreeVar(prefix+"dPhiMET12","dPhiMET12", cut+baseline, "dPhiMET12", 30, 0, 3);
          myPlots->addTreeVar(prefix+"dPhiMET3","dPhiMET3", cut+baseline, "dPhiMET3", 30, 0, 3);
          myPlots->addTreeVar(prefix+"mtB1MET", "mtB1MET", cut+baseline, "m_{T}(B1,MET) [GeV]", 20, 0, 1000);
          myPlots->addTreeVar(prefix+"mtB2MET", "mtB2MET", cut+baseline, "m_{T}(B2,MET) [GeV]", 20, 0, 1000);
          myPlots->addTreeVar(prefix+"NCTTstd", "NCTTstd", cut+baseline, "NCTTstd", 5, -0.5, 4.5);

        }
      }
    }

    myPlots->plot();
    delete myPlots;
  }

}
