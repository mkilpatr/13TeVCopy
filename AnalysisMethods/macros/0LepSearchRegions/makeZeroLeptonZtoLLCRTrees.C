#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class ZtoLLCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    ZtoLLCRAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, isMCTree, pars) {}

    // booking
    size i_njets30 = 0;
    size i_nbjets30 = 0;
    size i_ptzll = 0;

    bool passZtoLLSel = false;
    float ptzll = 0;

    void book() {
      ZeroLeptonAnalyzer::book();

      i_njets30           = data.add<int>("","njets30","I",0);
      i_nbjets30          = data.add<int>("","nbjets30","I",0);
      i_ptzll             = data.add<float>("","ptzll","F",0);
    }

    void processVariables(){
      ZeroLeptonAnalyzer::processVariables();

      // selections: exactly two same flavor leptons, inv mass in (80, 100)
      passZtoLLSel = false;
      if(selectedLeptons.size() != 2)
        return;

      auto lep0 = selectedLeptons.at(0);
      auto lep1 = selectedLeptons.at(1);

      if (lep0->pt()<20)
        return;

      if (lep0->iselectron() == lep1->iselectron() && lep0->q()*lep1->q() < 0){
        auto z_p4 = lep0->p4() + lep1->p4();
        if (z_p4.mass()>80 && z_p4.mass()<100){
          passZtoLLSel = true;
          ptzll = z_p4.pt();
        }
      }

    }


    bool fillEvent() {
      if(!passZtoLLSel)                     return false;
      if(!goodvertex)                       return false;
      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);

      data.fill<int>(i_njets30, std::count_if(jets.cbegin(), jets.cend(), [](RecoJetF* j){return j->pt()>30;}) );
      data.fill<int>(i_nbjets30, std::count_if(bJets.cbegin(), bJets.cend(), [](RecoJetF* j){return j->pt()>30;}) );
      data.fill<float>(i_ptzll, ptzll);
      return true;
    }


};

void makeInclusiveZtoLLCRTrees(TString sname = "dyjetstoll_cr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/vdutta/13TeV/150715/74X/merged/dyjetstoll_1_ntuple_wgtxsec.root",
                                 const double xsec = 1.0,
                                 const TString outputdir = "trees",
                                 const TString fileprefix = "root://eoscms//eos/cms",
                                 const TString json="")
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;
  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  cfgSet::ConfigSet pars = pars0lep(json);
  pars = cfgSet::zl_dilepton_set;
//  pars.corrections.jetAndMETCorrections |= JetAndMETCorrectionSet::METSCALE | JetAndMETCorrectionSet::METRESOLUTION;
//  pars.corrections.eventCorrections |= ucsbsusy::EventCorrectionSet::NORM;

  ZtoLLCRAnalyzer a(fullname, "Events", outfilename, isMC, &pars);
  a.analyze(100000);
//  a.analyze(100000, 10000);


}
