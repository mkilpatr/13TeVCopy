#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"


using namespace std;
using namespace ucsbsusy;
enum TYPE { NONE, DIRTY, SINGLE, DOUBLE, TRIPLE, MULTI};

struct splitSet {
  splitSet() {};
  splitSet( MomentumF superJet_, MomentumF subJet1_, MomentumF subJet2 ) : superJet(superJet_) , subJet1(subJet1_), subJet2(subJet2) {};
  float pt() const { return superJet.pt();}
  float eta()const { return superJet.eta();}
  float phi()const { return superJet.phi();}

  MomentumF superJet;
  MomentumF subJet1;
  MomentumF subJet2;
  float tau1                 ;
  float tau2                 ;
  float highest_peak         ;
  float lowest_peak          ;
  float minimum_value        ;
  float lowest_peak_location ;
  float highest_peak_location;
  float minimum_location     ;
  float splitDisc            ;
  int   shouldSplit          ;
  float subJetDR             ;

  TYPE type;
};

struct SplitVars{
  SplitVars() {
    superJet_pt          = new vector<float>;
    superJet_eta         = new vector<float>;
    superJet_phi         = new vector<float>;
    superJet_mass        = new vector<float>;
    tau1                 = new vector<float>;
    tau2                 = new vector<float>;
    subJet_1_pt          = new vector<float>;
    subJet_1_eta         = new vector<float>;
    subJet_1_phi         = new vector<float>;
    subJet_1_mass        = new vector<float>;
    subJet_2_pt          = new vector<float>;
    subJet_2_eta         = new vector<float>;
    subJet_2_phi         = new vector<float>;
    subJet_2_mass        = new vector<float>;
    subJet_1_pu_E        = new vector<float>;
    subJet_2_pu_E        = new vector<float>;
    highest_peak         = new vector<float>;
    lowest_peak          = new vector<float>;
    minimum_value        = new vector<float>;
    center_value         = new vector<float>;
    lowest_peak_location = new vector<float>;
    highest_peak_location= new vector<float>;
    minimum_location     = new vector<float>;
    splitDisc            = new vector<float>;
    shouldSplit          = new vector<int  >;
  }
  void load(TreeReader * treeReader, string branchName){
    treeReader->setBranchAddress(branchName,"superJet_pt"          ,&superJet_pt          ,true);
    treeReader->setBranchAddress(branchName,"superJet_eta"         ,&superJet_eta         ,true);
    treeReader->setBranchAddress(branchName,"superJet_phi"         ,&superJet_phi         ,true);
    treeReader->setBranchAddress(branchName,"superJet_mass"        ,&superJet_mass        ,true);
    treeReader->setBranchAddress(branchName,"tau1"                 ,&tau1                 ,true);
    treeReader->setBranchAddress(branchName,"tau2"                 ,&tau2                 ,true);
    treeReader->setBranchAddress(branchName,"subJet_1_pt"          ,&subJet_1_pt          ,true);
    treeReader->setBranchAddress(branchName,"subJet_1_eta"         ,&subJet_1_eta         ,true);
    treeReader->setBranchAddress(branchName,"subJet_1_phi"         ,&subJet_1_phi         ,true);
    treeReader->setBranchAddress(branchName,"subJet_1_mass"        ,&subJet_1_mass        ,true);
    treeReader->setBranchAddress(branchName,"subJet_2_pt"          ,&subJet_2_pt          ,true);
    treeReader->setBranchAddress(branchName,"subJet_2_eta"         ,&subJet_2_eta         ,true);
    treeReader->setBranchAddress(branchName,"subJet_2_phi"         ,&subJet_2_phi         ,true);
    treeReader->setBranchAddress(branchName,"subJet_2_mass"        ,&subJet_2_mass        ,true);
    treeReader->setBranchAddress(branchName,"subJet_1_pu_E"        ,&subJet_1_pu_E        ,true);
    treeReader->setBranchAddress(branchName,"subJet_2_pu_E"        ,&subJet_2_pu_E        ,true);
    treeReader->setBranchAddress(branchName,"highest_peak"         ,&highest_peak         ,true);
    treeReader->setBranchAddress(branchName,"lowest_peak"          ,&lowest_peak          ,true);
    treeReader->setBranchAddress(branchName,"minimum_value"        ,&minimum_value        ,true);
    treeReader->setBranchAddress(branchName,"center_value"         ,&center_value         ,true);
    treeReader->setBranchAddress(branchName,"lowest_peak_location" ,&lowest_peak_location ,true);
    treeReader->setBranchAddress(branchName,"highest_peak_location",&highest_peak_location,true);
    treeReader->setBranchAddress(branchName,"minimum_location"     ,&minimum_location     ,true);
    treeReader->setBranchAddress(branchName,"splitDisc"            ,&splitDisc            ,true);
    treeReader->setBranchAddress(branchName,"shouldSplit"          ,&shouldSplit          ,true);
  }
  void refresh(vector<splitSet> * set){
    set->clear();
    set->reserve(superJet_pt->size());
    for(unsigned int iJ = 0; iJ < superJet_pt->size(); ++iJ){

      set->emplace_back(MomentumF(CylLorentzVectorF(superJet_pt->at(iJ),superJet_eta->at(iJ),superJet_phi->at(iJ),superJet_mass->at(iJ))),
          subJet_1_pt > 0 ? MomentumF(CylLorentzVectorF(subJet_1_pt->at(iJ),subJet_1_eta->at(iJ),subJet_1_phi->at(iJ),subJet_1_mass->at(iJ))) : MomentumF(),
              subJet_2_pt > 0 ? MomentumF(CylLorentzVectorF(subJet_2_pt->at(iJ),subJet_2_eta->at(iJ),subJet_2_phi->at(iJ),subJet_2_mass->at(iJ))) : MomentumF()
      );

      set->back().tau1                  = tau1                   ->at(iJ);
      set->back().tau2                  = tau2                   ->at(iJ);
      set->back().highest_peak          = highest_peak           ->at(iJ);
      set->back().lowest_peak           = lowest_peak            ->at(iJ);
      set->back().minimum_value         = minimum_value          ->at(iJ);
      set->back().lowest_peak_location  = lowest_peak_location   ->at(iJ);
      set->back().highest_peak_location = highest_peak_location  ->at(iJ);
      set->back().minimum_location      = minimum_location       ->at(iJ);
      set->back().splitDisc             = splitDisc              ->at(iJ);
      set->back().shouldSplit           = shouldSplit            ->at(iJ);
      set->back().subJetDR              = PhysicsUtilities::deltaR(set->back().subJet1,set->back().subJet2);
    }
  }

  static void getParts(vector<GenParticleF>& inParts, vector<GenParticleF*>& goodParts, vector<GenParticleF*>& vetoParts, vector<GenParticleF*>& lepParts   ){
    int firstTopInd = -1;
    for(unsigned int iP = 0; iP < inParts.size(); ++iP){
      int pdgID = TMath::Abs(inParts[iP].pdgId());
      int status = inParts[iP].status();
      if(status != 22 && status != 23) continue; //Only status ~3 particles
      if(pdgID > 1000000) continue; //No SUSY particles;
      if(pdgID == 24) continue; //No Ws
      if(pdgID == 6 && firstTopInd < 0) firstTopInd = iP; //Mark ISR
      if(pdgID == 6) continue; //no Tops

      if(pdgID >= 11 && pdgID <= 16)lepParts.push_back(&inParts[iP]);
      else if(firstTopInd >= 0) goodParts.push_back(&inParts[iP]);
      else vetoParts.push_back(&inParts[iP]);
    }
  }

  static void categorize(vector<splitSet>& set, vector<GenParticleF*>& goodParts, vector<GenParticleF*>& vetoParts){

    vector<int> numGood(set.size(),0);
    vector<int> numVeto(set.size(),0);


    for(unsigned int iP = 0; iP < goodParts.size(); ++iP){
      double nearDR = 999;
      int index = PhysicsUtilities::findNearestDR(*goodParts[iP],set,nearDR,1);
      if(index < 0) continue;
      if(goodParts[iP]->pt() > 10) numGood[index]++;
//      else numGood[index]++;
    }

    for(unsigned int iP = 0; iP < vetoParts.size(); ++iP){
      double nearDR = 999;
      int index = PhysicsUtilities::findNearestDR(*vetoParts[iP],set,nearDR,1);
      if(index < 0) continue;
      if(goodParts[iP]->pt() > 10) numGood[index]++;
//      else numGood[index]++;

//      if(index >= 0) numGood[index]++;
    }


    for(unsigned int iJ = 0; iJ < set.size(); ++iJ){
//      int numGood = 0;
//      int numVeto = 0;
//
//      for(unsigned int iP = 0; iP < goodParts.size(); ++iP)
//        if(PhysicsUtilities::deltaR(*goodParts[iP],set[iJ].superJet ) < 1 ) numGood++;
//
//      for(unsigned int iP = 0; iP < vetoParts.size(); ++iP)
//        if(PhysicsUtilities::deltaR(*vetoParts[iP],set[iJ].superJet ) < 1 ) numVeto++;

      if(numGood[iJ] + numVeto[iJ] == 0) set[iJ].type = NONE;
//      else if(numVeto[iJ] > 0) set[iJ].type = DIRTY;
      else if(numGood[iJ] == 1) set[iJ].type = SINGLE;
      else if(numGood[iJ] > 1) set[iJ].type = MULTI;
//      else if(numGood[iJ] == 2) set[iJ].type = DOUBLE;
//      else if(numGood[iJ] == 3) set[iJ].type = TRIPLE;
//      else if(numGood[iJ] >  3) set[iJ].type = MULTI;
    }
  }


  vector<float> * superJet_pt           ;
  vector<float> * superJet_eta          ;
  vector<float> * superJet_phi          ;
  vector<float> * superJet_mass         ;
  vector<float> * tau1                  ;
  vector<float> * tau2                  ;
  vector<float> * subJet_1_pt           ;
  vector<float> * subJet_1_eta          ;
  vector<float> * subJet_1_phi          ;
  vector<float> * subJet_1_mass         ;
  vector<float> * subJet_2_pt           ;
  vector<float> * subJet_2_eta          ;
  vector<float> * subJet_2_phi          ;
  vector<float> * subJet_2_mass         ;
  vector<float> * subJet_1_pu_E         ;
  vector<float> * subJet_2_pu_E         ;
  vector<float> * highest_peak          ;
  vector<float> * lowest_peak           ;
  vector<float> * minimum_value         ;
  vector<float> * center_value          ;
  vector<float> * lowest_peak_location  ;
  vector<float> * highest_peak_location ;
  vector<float> * minimum_location      ;
  vector<float> * splitDisc             ;
  vector<int  > * shouldSplit           ;
};


class PickyReader : public BaseReader {
public:
  PickyReader(){


  };
  ~PickyReader() {};
  void load(TreeReader *treeReader, int options, std::string branchName){
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") jets with: ";
    genVars.load(treeReader,branchName_+ "_gen");
    recoVars.load(treeReader,branchName_+ "_reco");

  }
  void refresh(){
    genVars.refresh(&genSplits);
    recoVars.refresh(&recoSplits);
  }

  SplitVars genVars;
  SplitVars recoVars;

  vector<splitSet> genSplits;
  vector<splitSet> recoSplits;
};
#endif
