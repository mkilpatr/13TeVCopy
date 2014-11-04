#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/T2bWTreeAnalyzer.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"

using namespace ucsbsusy;

class Analyze : public T2bWTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  TString                         prefix;
  Analyze(TString fname, string treeName, bool isMCTree) : T2bWTreeAnalyzer(fname,treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter)  {
    load(EVTINFO);
    load(AK4JETS);
  }

  void run(){

    double weight = 1;

    eventPlots.rewind();
    eventPlots("j70_geq2__met_geq175__"              , nJ70 >= 2 && met_pt >= 175);
    eventPlots("T2bW_preselection__"                 , passPreselction);
    eventPlots.checkPoint();

    const double            eventQLikeli          = (prodQL < 0 ? -1 : +1) * TMath::Log(TMath::Abs(prodQL));


    eventPlots.fill(met_pt             , weight, "met"                , ";#slash{E}_{T}"                            , 33,  175,  405, prefix);
    eventPlots.fill(nJ30               , weight, "num_j30"            , ";N. p_{T} #geq 30 jets"                    , 11,  1.5, 12.5, prefix);
    eventPlots.fill(nJ70               , weight, "num_j70"            , ";N. p_{T} #geq 70 jets"                    , 11,  1.5, 12.5, prefix);
    eventPlots.fill(nTightBTags        , weight, "num_tight_btags"    , ";N. tight b-tagged p_{T} #geq 30 jets"     , 5 , -0.5,  4.5, prefix);
    eventPlots.fill(dPhiMET12          , weight, "min_dphi_j12_met"   , ";min |#Delta#phi(#slash{E}_{T},j1-2)|"     , 21,  0  , 3.15, prefix);
    eventPlots.fill(dPhiMET3           , weight, "dphi_j3_met"        , ";|#Delta#phi(#slash{E}_{T},j3)|"           , 21,  0  , 3.15, prefix);
    eventPlots.fill(nMedBTags          , weight, "num_medium_btags"   , "N. medium b-tagged p_{T} #geq 30 jets"    , 5 , -0.5,  4.5, prefix);
    eventPlots.fill(secLeadQL          , weight, "q2_likeli"          , "2nd-max L_{q}"                            , 22, -0.1,    1, prefix);
    eventPlots.fill(leadLeadQL         , weight, "q1_likeli"          , "max L_{q}"                                , 22, -0.1,    1, prefix);
    eventPlots.fill(eventQLikeli       , weight, "quark_likeli"       , "ln[ #prod L_{q} ]"                        , 25, -24 ,  1  , prefix);
    eventPlots.fill(htAlongAway        , weight, "ht_along_over_away" , "H_{T}^{along} / H_{T}^{away}"             , 24,  0  ,  1.2, prefix);
    eventPlots.fill(rmsJetPT           , weight, "rms_pt"             , "RMS[ p_{T}(jet) ]"                        , 25,  0  ,  250, prefix);
    eventPlots.fill(rmsJetDphiMET      , weight, "rms_dphi"           , "RMS[ #Delta#phi(jet,#slash{E}_{T}) ]"     , 25,  0  ,  2.5, prefix);
    eventPlots.fill(bInvMass           , weight, "bb_mass"            , "m(b1 + b2)"                               , 24,  0  ,  600, prefix);
    eventPlots.fill(bTransverseMass    , weight, "mTb"                , "m_{T}(nearest b to #slash{E}_{T})"        , 20,  0  ,  400, prefix);
    eventPlots.fill(rmsBEta            , weight, "deta_b_rms"         , "RMS[ #Delta#eta(b,other jets) ]"          , 25,  0  ,  2.5, prefix);
    eventPlots.fill(wInvMass           , weight, "leading_jj_mass"    , "m(max-p_{T} jj)"                          , 24,  0  ,  600, prefix);



    eventPlots.fill(getT2BWDiscriminator(T2BW_LX)    , weight, "T2bw_lowX"          , ";D_{T2bw}(lowX)"                          , 100,  -1, 1, prefix);
    eventPlots.fill(getT2BWDiscriminator(T2BW_LM)    , weight, "T2bw_lowMass"       , ";D_{T2bw}(lowMass)"                       , 100,  -1, 1, prefix);
    eventPlots.fill(getT2BWDiscriminator(T2BW_MXHM)  , weight, "T2bw_medXHighMass"  , ";D_{T2bw}(mediumX, highMass)"             , 100,  -1, 1, prefix);
    eventPlots.fill(getT2BWDiscriminator(T2BW_HXHM)  , weight, "T2bw_highXHighMass" , ";D_{T2bw}(highX, highMass)"               , 100,  -1, 1, prefix);
    eventPlots.fill(getT2BWDiscriminator(T2BW_VHM)   , weight, "T2bw_veryHighMass"  , ";D_{T2bw}(veryHighMass)"                  , 100,  -1, 1, prefix);



    for(const auto * jet : jets){
      eventPlots.revert();
      ++eventPlots;
      int flv = TMath::Abs(ak4Reader.genjetflavor_->at(jet->index()));
      eventPlots("udsc__", flv >= 1 && flv <= 4 );
      eventPlots("b__", flv == 5);
      eventPlots("g__", flv == 21);
      eventPlots.fill(ak4Reader.jetqgl_->at(jet->index()), weight, "jet_qgl"    , "Q/G Likelihood"                       , 100,  0  ,  1, prefix);
    }


  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }
};


#endif



void processT2bWSearchRegions(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", bool isMCTree = true) {

  Analyze a(fname, treeName, isMCTree);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  while(a.nextEvent()) a.run();
  a.out(TString::Format("%s_plots.root",name.Data()));
}
