#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class ZtoLLCRAnalyzer : public ZeroLeptonAnalyzer {

  public :

    ZtoLLCRAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, isMCTree, pars), passZtoLLSel(false) {}

    bool passZtoLLSel;
    const float low_metcut_ = 100;

    void processVariables(){
      BaseTreeAnalyzer::processVariables();

      // nVetoedLeptons should be zero since it is not configured.
      assert(nVetoedLeptons==0);

      // selections: exactly two same flavor leptons, inv mass in (80, 100)
      passZtoLLSel = false;
      if(selectedLeptons.size() == 2){
        auto lep0 = selectedLeptons.at(0);
        auto lep1 = selectedLeptons.at(1);
        if (lep0->q() != lep1->q()  &&  lep0->iselectron() == lep1->iselectron()){
          auto z_p4 = lep0->p4() + lep1->p4();
          if (z_p4.mass()>80 && z_p4.mass()<100){
            passZtoLLSel = true;
            met->setP4(met->p4() + z_p4); // add leptons to met

            // clean vetoedTaus vs selectedLeptons
            vector<bool> isOverlapTrack(vetoedTracks.size(),false);
            vector<PFCandidateF*> tmpTracks;
            for(const auto* lep : selectedLeptons) {
              double nearDR = 0;
              int near = PhysicsUtilities::findNearestDRDeref(*lep,vetoedTracks,nearDR,0.4,-1,0,vetoedTracks.size());
              if(near >= 0){
                isOverlapTrack[near] = true;
              }
            }
            for (unsigned int iT = 0; iT < vetoedTracks.size(); ++iT){
              if (!isOverlapTrack.at(iT))
                tmpTracks.push_back(vetoedTracks.at(iT));
            }
            vetoedTracks = tmpTracks;
            nVetoedTracks = vetoedTracks.size();
          }
        }
      }

    }


    bool fillEvent() {
      if(!passZtoLLSel)                     return false;
      if(nVetoedTracks > 0)                 return false;
      if(met->pt() < low_metcut_)           return false;
      if(!goodvertex)                       return false;
      filler.fillEventInfo(&data, this, datatype_);
      filler.fillJetInfo  (&data, jets, bJets, met);
      return true;
    }


};

void makeZeroLeptonZtoLLCRTrees(TString sname = "dyjetstoll_cr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/vdutta/13TeV/150715/74X/merged/dyjetstoll_1_ntuple_wgtxsec.root",
                                 const double xsec = 1.0,
                                 const TString outputdir = "trees",
                                 const TString fileprefix = "root://eoscms//eos/cms")
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;
  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  cfgSet::ConfigSet pars = pars0lep();
  pars = cfgSet::zl_dilepton_set;

  ZtoLLCRAnalyzer a(fullname, "Events", outfilename, isMC, &pars);
  a.analyze(100000);
//  a.analyze(100000, 10000);


}
