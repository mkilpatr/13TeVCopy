#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/T2bWTreeAnalyzer.h"

#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"

using namespace ucsbsusy;

class Analyze : public T2bWTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  TString                         prefix;


  Analyze(TString fname, string treeName, bool isMCTree) : T2bWTreeAnalyzer(fname,treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter){}

  virtual void loadVariables(){
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(GENPARTICLES);
  }

  void processVariables(){
      boson = 0;
      radPartons.clear();
      genParts.clear();
      genParts.reserve(genParticleReader.genParticles.size());
      for(auto& p : genParticleReader.genParticles){
        genParts.push_back(&p);
        if(ParticleInfo::isEWKBoson(p.pdgId())) boson = &p;
        if(ParticleInfo::isQuarkOrGluon(p.pdgId())) radPartons.push_back(&p);
      }
    assert(boson);


    jets.clear();
    jets.reserve(ak4Reader.recoJets.size());
    for(auto& jet : ak4Reader.recoJets){
      if(JetKinematics::passCuts(jet,30,9999) && PhysicsUtilities::deltaR2(*boson,jet) >= .16  )
        jets.push_back(&jet);
    }

    genJets.clear();
    genJets.reserve(ak4Reader.genJets.size());
    for(auto& jet : ak4Reader.genJets){
      if(JetKinematics::passCuts(jet,30,9999) && PhysicsUtilities::deltaR2(*boson,jet) >= .16  )
        genJets.push_back(&jet);
    }

    met = boson;
    weight = evtInfoReader.weight;
    process = evtInfoReader.process;
    prefix = defaults::PROCESS_NAMES[process];
    prefix += "_";

    fillSearchVars();
    if(loadedT2BW && passPreselction)
      computeT2BWDiscriminators();
  }

  void plot(double weight2, TString prefix2){
    eventPlots.rewind();

    eventPlots("",false);
    eventPlots("nrad_qg_eq0to1__",radPartons.size() <= 1);
    eventPlots("nrad_qg_eq2__",radPartons.size() == 2);
    eventPlots("nrad_qg_eq3__",radPartons.size() == 3);
    eventPlots("nrad_qg_eq4__",radPartons.size() == 4);
    eventPlots("nrad_qg_geq5__",radPartons.size() >= 5);

    double ht = JetKinematics::ht(jets);
    eventPlots("ht_eq140to250__",ht < 250);
    eventPlots("ht_eq250to350__",ht >= 250 && ht < 350);
    eventPlots("ht_eq350to500__",ht >= 350 && ht < 500);
    eventPlots("ht_geq500__"    ,ht >= 500);


    eventPlots("nGenJets_eq2to3__",genJets.size() <= 3                       );
    eventPlots("nGenJets_eq4to5__",genJets.size() == 4 || genJets.size() == 5);
    eventPlots("nGenJets_eq6to7__",genJets.size() == 6 || genJets.size() == 7);
    eventPlots("nGenJets_geq8__"  ,genJets.size() >= 8);

    eventPlots("nRecoJets_eq2to3__",genJets.size() <= 3);
    eventPlots("nRecoJets_eq4to5__",genJets.size() == 4 || genJets.size() == 5);
    eventPlots("nRecoJets_eq6to7__",genJets.size() == 6 || genJets.size() == 7);
    eventPlots("nRecoJets_geq8__"  ,genJets.size() >= 8);


    int nBs = 0;
    for(const auto* j : jets){
      if(TMath::Abs(j->flavor()) == 5) nBs++;
    }

    eventPlots("nBs_incl__",true);
    eventPlots("nBs_eq0__" ,nBs==0);
    eventPlots("nBs_eq1__" ,nBs == 1);
    eventPlots("nBs_geq2__",nBs >= 2);


    eventPlots("nCross_incl__"    ,true);

    eventPlots("nCross_nGenJets_eq2to3__nBs_eq0__" ,genJets.size() <= 3 && nBs==  0);
    eventPlots("nCross_nGenJets_eq2to3__nBs_eq1__" ,genJets.size() <= 3 && nBs == 1);
    eventPlots("nCross_nGenJets_eq2to3__nBs_geq2__",genJets.size() <= 3 && nBs >= 2);

    eventPlots("nCross_nGenJets_eq4to5__nBs_eq0__" ,(genJets.size() == 4 || genJets.size() == 5) && nBs==  0);
    eventPlots("nCross_nGenJets_eq4to5__nBs_eq1__" ,(genJets.size() == 4 || genJets.size() == 5) && nBs == 1);
    eventPlots("nCross_nGenJets_eq4to5__nBs_geq2__",(genJets.size() == 4 || genJets.size() == 5) && nBs >= 2);

    eventPlots("nCross_nGenJets_eq6to7__nBs_eq0__" ,(genJets.size() == 6 || genJets.size() == 7) && nBs==  0);
    eventPlots("nCross_nGenJets_eq6to7__nBs_eq1__" ,(genJets.size() == 6 || genJets.size() == 7) && nBs == 1);
    eventPlots("nCross_nGenJets_eq6to7__nBs_geq2__",(genJets.size() == 6 || genJets.size() == 7) && nBs >= 2);

    eventPlots("nCross_nGenJets_geq8__nBs_eq0__" ,genJets.size() >= 8 && nBs==  0);
    eventPlots("nCross_nGenJets_geq8__nBs_eq1__" ,genJets.size() >= 8 && nBs == 1);
    eventPlots("nCross_nGenJets_geq8__nBs_geq2__",genJets.size() >= 8 && nBs >= 2);



    eventPlots.fill(boson->pt(), weight2, "boson_pt"    , ";boson p_{T}"     ,90 , 100,  1000, prefix2);


    eventPlots.rewind();
    eventPlots("",true);

    ++eventPlots;
    eventPlots("j70_geq2__met_geq175__"              , nJ70 >= 2 && met_pt >= 175);
    eventPlots("T2bW_preselection__"                 , passPreselction);
    eventPlots.checkPoint();

    const double            eventQLikeli          = (prodQL < 0 ? -1 : +1) * TMath::Log(TMath::Abs(prodQL));


    eventPlots.fill(met_pt             , weight2, "met"                , ";#slash{E}_{T}"                            , 33,  175,  405,prefix2);
    eventPlots.fill(nJ30               , weight2, "num_j30"            , ";N. p_{T} #geq 30 jets"                    , 11,  1.5, 12.5,prefix2);
    eventPlots.fill(nJ70               , weight2, "num_j70"            , ";N. p_{T} #geq 70 jets"                    , 11,  1.5, 12.5,prefix2);
    eventPlots.fill(nTightBTags        , weight2, "num_tight_btags"    , ";N. tight b-tagged p_{T} #geq 30 jets"     , 5 , -0.5,  4.5,prefix2);
    eventPlots.fill(dPhiMET12          , weight2, "min_dphi_j12_met"   , ";min |#Delta#phi(#slash{E}_{T},j1-2)|"     , 21,  0  , 3.15,prefix2);
    eventPlots.fill(dPhiMET3           , weight2, "dphi_j3_met"        , ";|#Delta#phi(#slash{E}_{T},j3)|"           , 21,  0  , 3.15,prefix2);
    eventPlots.fill(nMedBTags          , weight2, "num_medium_btags"   , "N. medium b-tagged p_{T} #geq 30 jets"    , 5 , -0.5,  4.5, prefix2);
    eventPlots.fill(secLeadQL          , weight2, "q2_likeli"          , "2nd-max L_{q}"                            , 22, -0.1,    1, prefix2);
    eventPlots.fill(leadLeadQL         , weight2, "q1_likeli"          , "max L_{q}"                                , 22, -0.1,    1, prefix2);
    eventPlots.fill(eventQLikeli       , weight2, "quark_likeli"       , "ln[ #prod L_{q} ]"                        , 25, -24 ,  1  , prefix2);
    eventPlots.fill(htAlongAway        , weight2, "ht_along_over_away" , "H_{T}^{along} / H_{T}^{away}"             , 24,  0  ,  1.2, prefix2);
    eventPlots.fill(rmsJetPT           , weight2, "rms_pt"             , "RMS[ p_{T}(jet) ]"                        , 25,  0  ,  250, prefix2);
    eventPlots.fill(rmsJetDphiMET      , weight2, "rms_dphi"           , "RMS[ #Delta#phi(jet,#slash{E}_{T}) ]"     , 25,  0  ,  2.5, prefix2);
    eventPlots.fill(bInvMass           , weight2, "bb_mass"            , "m(b1 + b2)"                               , 24,  0  ,  600, prefix2);
    eventPlots.fill(bTransverseMass    , weight2, "mTb"                , "m_{T}(nearest b to #slash{E}_{T})"        , 20,  0  ,  400, prefix2);
    eventPlots.fill(rmsBEta            , weight2, "deta_b_rms"         , "RMS[ #Delta#eta(b,other jets) ]"          , 25,  0  ,  2.5, prefix2);
    eventPlots.fill(wInvMass           , weight2, "leading_jj_mass"    , "m(max-p_{T} jj)"                          , 24,  0  ,  600, prefix2);



    eventPlots.fill(getT2BWDiscriminator(T2BW_LX)    , weight2, "T2bw_lowX"          , ";D_{T2bw}(lowX)"                          , 100,  -1, 1, prefix2);
    eventPlots.fill(getT2BWDiscriminator(T2BW_LM)    , weight2, "T2bw_lowMass"       , ";D_{T2bw}(lowMass)"                       , 100,  -1, 1, prefix2);
    eventPlots.fill(getT2BWDiscriminator(T2BW_MXHM)  , weight2, "T2bw_medXHighMass"  , ";D_{T2bw}(mediumX, highMass)"             , 100,  -1, 1, prefix2);
    eventPlots.fill(getT2BWDiscriminator(T2BW_HXHM)  , weight2, "T2bw_highXHighMass" , ";D_{T2bw}(highX, highMass)"               , 100,  -1, 1, prefix2);
    eventPlots.fill(getT2BWDiscriminator(T2BW_VHM)   , weight2, "T2bw_veryHighMass"  , ";D_{T2bw}(veryHighMass)"                  , 100,  -1, 1, prefix2);

    for(unsigned int iS = 0; iS < NUM_T2BW_SEARCHREGIONS; ++iS)
      if(getT2BWDiscriminator( (T2BWSearchRegions)(iS) ) >= T2BWDiscCuts[iS] )
        eventPlots.fill(iS    , weight2, "srCounts"          , ";SRCounts", 5,  -.5, 4.5, prefix2);
  }

  void runEvent(){
    plot(weight,prefix);
    if(process == defaults::SINGLE_G){

      int nBs = 0;
      for(const auto* j : jets){
        if(TMath::Abs(j->flavor()) == 5) nBs++;
      }
      int nJ = genJets.size();

      double x = std::min(float(1000.0),boson->pt());
      double extraWeight = 1;
//      if(nBs >= 2) extraWeight = 0.39582;
//      else if(nBs == 1) extraWeight = 0.33589;
//      else
      extraWeight = -0.184294+0.00425285 *x -1.52337e-05*x*x + 2.7087e-08*x*x*x -2.32439e-11*x*x*x*x + 7.69601e-15*x*x*x*x*x;

      if(nBs == 0){
        if(nJ <= 3)      extraWeight *= 1.06;
        else if (nJ <= 5)extraWeight *= .9699;
        else if (nJ <= 7)extraWeight *= .9476;
        else             extraWeight *= .9155;
      } else if (nBs ==1){
        if(nJ <= 3)      extraWeight *= 1.386;
        else if (nJ <= 5)extraWeight *= 1.156;
        else if (nJ <= 7)extraWeight *= 1.045;
        else             extraWeight *= .9481;
      }else{
        if(nJ <= 3)      extraWeight *= 1.633;
        else if (nJ <= 5)extraWeight *= 1.352;
        else if (nJ <= 7)extraWeight *= 1.244;
        else             extraWeight *= 1.202;
      }



      plot(weight*extraWeight,prefix+"timesSF_");
    }
  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }

  float weight;
  defaults::Process process;
  vector<GenJetF*> genJets;
  GenParticleF * boson;
  vector<GenParticleF*> radPartons;

};


#endif


void testPhotonZApplicability(string fname = "evttree.root", string treeName = "Events") {

  Analyze a(fname, treeName, true);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.analyze();
  a.out(TString::Format("%s_plots.root",name.Data()));
}
