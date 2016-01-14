//--------------------------------------------------------------------------------------------------
// 
// BaseTreeAnalyzer
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_TREEANALYZER_T2BWTREEANALYZER_H
#define ANALYSISBASE_TREEANALYZER_T2BWTREEANALYZER_H

class Panvariate;
template<typename Object> class Paramatrix;
typedef   Paramatrix<Panvariate>  ParamatrixMVA;


#include <string>
#include <vector>
#include <assert.h>
#include <TString.h>

#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

using namespace std;

namespace ucsbsusy {

  class T2bWTreeAnalyzer : public BaseTreeAnalyzer {

  public:

    T2bWTreeAnalyzer(TString fileName, TString treeName, bool isMCTree = false);
    virtual ~T2bWTreeAnalyzer() {};

//    virtual void	load(VarType type, int options = -1, string branchName = "" );
    virtual void  processVariables();
    virtual void runEvent() = 0;
    void  filterJets(vector<RecoJetF*>& newJets, const double minPT, const double maxETA);
    static bool  isMediumBTaggedJet (const RecoJetF& jet) {return jet.csv() > .679;}
    static bool  isTightBTaggedJet(const RecoJetF& jet) {return jet.csv() > .898;}
    void  fillSearchVars();
    void computeT2BWDiscriminators();


    enum T2BWSearchRegions {T2BW_LX, T2BW_LM, T2BW_MXHM,T2BW_HXHM,T2BW_VHM, NUM_T2BW_SEARCHREGIONS};
    static const double T2BWDiscCuts[NUM_T2BW_SEARCHREGIONS];

    double getT2BWDiscriminator(T2BWSearchRegions SR) const{
      assert(loadedT2BW);
      assert (SR < NUM_T2BW_SEARCHREGIONS);
      return T2BWDiscValues[SR];
    }
    bool passT2BWDiscriminator(T2BWSearchRegions SR) const{
      assert(loadedT2BW);
      assert (SR < NUM_T2BW_SEARCHREGIONS);
      return T2BWDiscValues[SR] >= T2BWDiscCuts[SR];
    }

    const double minPT;
    const double maxETA;

    //T2BW Discriminator inputs
    const vector<const ParamatrixMVA*> T2BWParams;
    const vector<TString>              T2BWParNames;
    const bool loadedT2BW;

    vector<RecoJetF*> jets;
    MomentumF * met;

    //Number of leptons
    int   nMuons;
    int   nElectrons;

    //common variables for the search region def
    double met_pt;
    int    nJ70;
    int    nJ30;
    int    nTightBTags;
    double dPhiMET12;
    double dPhiMET3;
    bool   passPreselction;
    int    nMedBTags;
    double secLeadQL;
    double leadLeadQL;
    double prodQL;
    double htAlongAway;
    double rmsJetPT;
    double rmsJetDphiMET;
    double bInvMass;
    double bTransverseMass;
    double rmsBEta;
    double wInvMass;

    vector<double>               T2BWDiscValues;

  };
}


#endif
