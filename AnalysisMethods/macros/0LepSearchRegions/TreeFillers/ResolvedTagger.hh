/*
  Resolved top/W tagger

    ResolvedFiller
    fillResolvedInfo
    resolved top = resTops[i]
    resolved w = resWs[i]

    collections filled are resTops and resWs
      use res.j123 for object MomentumF
          res.m3jet for total mass
          res.j1 and j2 for W components
          res.bjet and res.bjetcsv (is res.j3) for highest csv subjet of tops

    reshardnsubjets = resolved, hardest resolved object in event, nsubjets 
    resmatchedX = resolved, matched to gen, X

*/

#ifndef RESOLVEDTAGGER_HH
#define RESOLVEDTAGGER_HH

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/DataFormats/interface/CMSTop.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include <vector>
#include "Math/LorentzVector.h"
#include "Math/VectorUtil.h"
//#include "AnalysisTools/KinematicVariables/interface/mt2w.h"
//#include "AnalysisTools/KinematicVariables/interface/Mt2Helper.h"

using namespace ucsbsusy;

struct resolved {
  resolved() : 
              rjet1(0), rjet2(0), rjet3(0),
              bjetcsv(-1.),
              m1(-1.),  m2(-1.),  m3(-1.),
              m12(-1.), m13(-1.), m23(-1.), m3jet(-1.),
              isW(false)
              {}
  enum NSubjets { MONOJET = 1, DIJET, TRIJET };
  MomentumF jet1, jet2, jet3;      // subjet momenta. might not be pt-ordered. see individual algo.
  MomentumF jet123;                // object momentum, the sum of subjets' p4
  MomentumF bjet;                  // subjet with highest csv
  float     bjetcsv;               //
  bool      hasmbjet;               // does csv > defaults::medium
  RecoJetF *rjet1, *rjet2, *rjet3; // ptrs to reco subjets' full info
  float     m1, m2, m3, m12, m13, m23, m3jet; // subjet masses, pairwise subjet masses, total object mass
  NSubjets  nSubjets;              // see enum. 1 to 3. indicates which subjet properties will be filled.
  bool      isW;                   // example of usage: dijet could mean top=(b,W) or W=(w1,w2)
};
std::vector<resolved > resTops;
std::vector<resolved > resWs;

struct ResTreeFiller {

  ResTreeFiller() {}

  // matched
  size i_resnmatchedrecotops;
  size i_reshardmatchedisw;
  size i_reshardmatchednsubjets;
  size i_reshardmatchedgentoppt;
  size i_reshardmatchedgentopeta;
  size i_reshardmatchedrecopt;
  size i_reshardmatchedrecoeta;
  size i_reshardmatchedrecom3jet;
  size i_reshardmatchedrecom12;
  size i_reshardmatchedrecom13;
  size i_reshardmatchedrecom23;
  size i_reshardmatchedrecom1;
  size i_reshardmatchedrecom2;
  size i_reshardmatchedrecom3;
  size i_reshardmatchedrecoj1pt;
  size i_reshardmatchedrecoj1eta;
  size i_reshardmatchedrecoj2pt;
  size i_reshardmatchedrecoj2eta;
  size i_reshardmatchedrecoj3pt;
  size i_reshardmatchedrecoj3eta;

  // unmatched
  size i_resnrecotops;
  size i_reshardisw;
  size i_reshardnsubjets;
  size i_reshardgentoppt;
  size i_reshardgentopeta;
  size i_reshardrecopt;
  size i_reshardrecoeta;
  size i_reshardrecom3jet;
  size i_reshardrecom12;
  size i_reshardrecom13;
  size i_reshardrecom23;
  size i_reshardrecom1;
  size i_reshardrecom2;
  size i_reshardrecom3;
  size i_reshardrecoj1pt;
  size i_reshardrecoj1eta;
  size i_reshardrecoj2pt;
  size i_reshardrecoj2eta;
  size i_reshardrecoj3pt;
  size i_reshardrecoj3eta;

  void book(TreeWriterData* data) {
    //matched
    i_resnmatchedrecotops        = data->add<int  >("","resnmatchedrecotops","I",0);
    i_reshardmatchedisw          = data->add<bool >("","reshardmatchedisw","O",0);
    i_reshardmatchednsubjets     = data->add<int  >("","reshardmatchednsubjets","I",0);
    i_reshardmatchedgentoppt     = data->add<float>("","reshardmatchedgentoppt","F",0);
    i_reshardmatchedgentopeta    = data->add<float>("","reshardmatchedgentopeta","F",0);
    i_reshardmatchedrecopt       = data->add<float>("","reshardmatchedrecopt","F",0);
    i_reshardmatchedrecoeta      = data->add<float>("","reshardmatchedrecoeta","F",0);
    i_reshardmatchedrecom3jet    = data->add<float>("","reshardmatchedrecom3jet","F",0);
    i_reshardmatchedrecom12      = data->add<float>("","reshardmatchedrecom12","F",0);
    i_reshardmatchedrecom13      = data->add<float>("","reshardmatchedrecom13","F",0);
    i_reshardmatchedrecom23      = data->add<float>("","reshardmatchedrecom23","F",0);
    i_reshardmatchedrecom1       = data->add<float>("","reshardmatchedrecom1","F",0);
    i_reshardmatchedrecom2       = data->add<float>("","reshardmatchedrecom2","F",0);
    i_reshardmatchedrecom3       = data->add<float>("","reshardmatchedrecom3","F",0);
    i_reshardmatchedrecoj1pt     = data->add<float>("","reshardmatchedrecoj1pt","F",0);
    i_reshardmatchedrecoj1eta    = data->add<float>("","reshardmatchedrecoj1eta","F",0);
    i_reshardmatchedrecoj2pt     = data->add<float>("","reshardmatchedrecoj2pt","F",0);
    i_reshardmatchedrecoj2eta    = data->add<float>("","reshardmatchedrecoj2eta","F",0);
    i_reshardmatchedrecoj3pt     = data->add<float>("","reshardmatchedrecoj3pt","F",0);
    i_reshardmatchedrecoj3eta    = data->add<float>("","reshardmatchedrecoj3eta","F",0);
    //unmatched
    i_resnrecotops               = data->add<int  >("","resnrecotops","I",0);
    i_reshardisw                 = data->add<bool >("","reshardisw","O",0);
    i_reshardnsubjets            = data->add<int  >("","reshardnsubjets","I",0);
    i_reshardgentoppt            = data->add<float>("","reshardgentoppt","F",0);
    i_reshardgentopeta           = data->add<float>("","reshardgentopeta","F",0);
    i_reshardrecopt              = data->add<float>("","reshardrecopt","F",0);
    i_reshardrecoeta             = data->add<float>("","reshardrecoeta","F",0);
    i_reshardrecom3jet           = data->add<float>("","reshardrecom3jet","F",0);
    i_reshardrecom12             = data->add<float>("","reshardrecom12","F",0);
    i_reshardrecom13             = data->add<float>("","reshardrecom13","F",0);
    i_reshardrecom23             = data->add<float>("","reshardrecom23","F",0);
    i_reshardrecom1              = data->add<float>("","reshardrecom1","F",0);
    i_reshardrecom2              = data->add<float>("","reshardrecom2","F",0);
    i_reshardrecom3              = data->add<float>("","reshardrecom3","F",0);
    i_reshardrecoj1pt            = data->add<float>("","reshardrecoj1pt","F",0);
    i_reshardrecoj1eta           = data->add<float>("","reshardrecoj1eta","F",0);
    i_reshardrecoj2pt            = data->add<float>("","reshardrecoj2pt","F",0);
    i_reshardrecoj2eta           = data->add<float>("","reshardrecoj2eta","F",0);
    i_reshardrecoj3pt            = data->add<float>("","reshardrecoj3pt","F",0);
    i_reshardrecoj3eta           = data->add<float>("","reshardrecoj3eta","F",0);
  }

  // simple method for tops:
  // step 1) highest csv jet is the b
  // step 2) locate two nearby jets (same hemi) with mass ~ W and total mass ~ top
  // step 3) locate nearby jet with mass ~ W
  void fillResolvedInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, vector<RecoJetF*> jets, vector<resolved> resTops, vector<resolved> resWs) {
    std::cout << std::endl << std::endl <<  "***********************filling resolved info" << std::endl;
    bool dbg = false;

    float mW   =  80.4;
    float mTop = 173.4;
    float mWmin = 50., mWmax = 110.; // tunable windows
    float mTopMin = 110., mTopMax = 220.;

    int nJets = jets.size();
    if(dbg) std::cout << "njets " << nJets << std::endl;

    // step 1 - bjet is max csv jet
    int   indMaxCsv = -1;
    float maxCsv = -1.;
    for(int i = 0 ; i < nJets ; ++i){
      float thisCsv = jets[i]->csv();
      if(thisCsv > maxCsv) {maxCsv = thisCsv; indMaxCsv = i;}
    }
    assert(indMaxCsv > -1);
    MomentumF bjet = jets[indMaxCsv]->p4();
    float bjetcsv  = jets[indMaxCsv]->csv();
    if(dbg) std::cout << "bjet index, csv, p4 " << indMaxCsv << " " << bjetcsv << " " << bjet << std::endl;

    // look for two jets to accomplish top = (b,j1,j2) or W = (j1,j2)
    // the b is from earlier. in both cases W is j1 and j2.
    MomentumF jet1, jet2, jet3, jet12, jet123;
    jet3 = bjet; // use variables with third index only for top stuff
    for(int i = 0 ; i < nJets-1 ; ++i){
      for(int j = i+1 ; j < nJets ; ++j){
        if( (i == indMaxCsv) || (j == indMaxCsv) ) continue; // skip b jet
        jet1 = jets[i]->p4(); jet2 = jets[j]->p4();
        jet12 = jet1.p4() + jet2.p4(); // this is the w
        jet123 = jet1.p4() + jet2.p4() + jet3.p4();
        float m12  = (jet1.p4() + jet2.p4()).mass();
        float m13  = (jet1.p4() + jet3.p4()).mass();
        float m23  = (jet2.p4() + jet3.p4()).mass();
        float m3jet = jet123.mass();
        if(dbg) std::cout << "indx i,j " << i << " " << j << std::endl;
        if(dbg) std::cout << "jet1, jet2, jet3 " << jet1 << " " << jet2 << " " << jet3 << std::endl;
        if(dbg) std::cout << "m12, m13, m23, m3jet " << m12 << " " << m13 << " " << m23 << " " << m3jet << std::endl;

        // want jets to be in same hemisphere as total jet axis
        /*
        if((PhysicsUtilities::deltaR(jet123,jet1) > 1.5) || // use jet12 if W
           (PhysicsUtilities::deltaR(jet123,jet2) > 1.5) ||
           (PhysicsUtilities::deltaR(jet123,jet3) > 1.5)) continue;
        */

        // w mass window
        if( (m12 > mWmax) || (m12 < mWmin) ) continue;
        if(dbg) std::cout << "m12 passed w window " << std::endl;

        // softdrop W conditions
        // todo

        // top mass window - here we use the b
        bool isW = true;
        if( (m3jet < mTopMax) && (m3jet > mTopMin) ) isW = false; // if passes top conditions, call it a top
        if(dbg) std::cout << "m3jet result, isW: " << isW << std::endl;

        // softdrop top conditions
        // todo

        // fill and submit this resolved candidate
        resolved cand;
        cand.nSubjets = isW ? resolved::DIJET : resolved::TRIJET;
        cand.isW = isW;
        cand.jet1  = jet1; cand.jet2 = jet2;
        cand.rjet1 = jets[i]; cand.rjet2 = jets[j];
        cand.m1    = (jet1.p4()).mass();  cand.m2    = (jet2.p4()).mass();
        cand.m12   = m12;
        if(!isW) { // top, much ado about bjets
          cand.jet3 = bjet;
          cand.jet123 = jet123;
          cand.bjet = bjet; cand.bjetcsv = bjetcsv; cand.hasmbjet = (cand.bjetcsv>defaults::CSV_MEDIUM);
          cand.rjet3 = jets[indMaxCsv];
          cand.m3    = (jet3.p4()).mass();
          cand.m23   = m23;
          cand.m13   = m13;
          cand.m3jet = m3jet;
          resTops.push_back(cand);
        }else{ // W
          cand.jet123 = jet12;
          cand.m3jet = jet12.mass();
          resWs.push_back(cand);
        }

      }//for jet
    }//for jet

    if(dbg) std::cout << "******* top object dump *********" << std::endl;
    if(dbg){
    for(int i = 0 ; i < resTops.size() ; ++i){
      resolved rj = resTops[i];
      std::cout << rj.jet1 << " " << rj.jet2 << " " << rj.jet3 << std::endl;
      std::cout << rj.jet123 << std::endl;
      std::cout << rj.bjet << " " << rj.bjetcsv << " " << rj.hasmbjet << std::endl;
      std::cout << rj.rjet1 << " " << rj.rjet2 << " " << rj.rjet3 << std::endl;
      std::cout << rj.m1 << " " << rj.m2 << " " << rj.m3 << " " << rj.m12 << " " << rj.m23 << " " << rj.m13 << " " << rj.m3jet << std::endl;
      std::cout << rj.nSubjets << " " << rj.isW << std::endl;
    }
    }
    if(dbg) std::cout << "******* w object dump *********" << std::endl;
    if(dbg){
    for(int i = 0 ; i < resWs.size() ; ++i){
      resolved rj = resWs[i];
      std::cout << rj.jet1 << " " << rj.jet2 << " " << rj.jet3 << std::endl;
      std::cout << rj.jet123 << std::endl;
      std::cout << rj.bjet << " " << rj.bjetcsv << " " << rj.hasmbjet << std::endl;
      std::cout << rj.rjet1 << " " << rj.rjet2 << " " << rj.rjet3 << std::endl;
      std::cout << rj.m1 << " " << rj.m2 << " " << rj.m3 << " " << rj.m12 << " " << rj.m23 << " " << rj.m13 << " " << rj.m3jet << std::endl;
      std::cout << rj.nSubjets << " " << rj.isW << std::endl;
    }
    }

    /*
      clean resolved objects wrt each other
      keep one with mass nearest shell
      for now, ignore overlaps between tops and Ws - treat as independent collections
    */
    // clean resTops
    for(int i = 0 ; i < nJets ; ++i) { // find ak4 jets shared by multiple tops
      int prev = -1, toErase = -1;
      for(int j = 0 ; j < resTops.size() ; ++j) {
        bool isPresent = (jets[i]->p4() == resTops[j].jet1.p4()) |
			 (jets[i]->p4() == resTops[j].jet2.p4()) |
                         (jets[i]->p4() == resTops[j].jet3.p4());
        if(isPresent && prev > -1 ) { // this jet is in two Resolved
          toErase = (abs(resTops[prev].m3jet - mTop) >
                     abs(resTops[j   ].m3jet - mTop)) ? prev : j; // erase one with largest gap from mTop
          // keep this - might be useful later
          //if( (prev == 0) && (resTops[prev].isFRTQ) ) {
          //  toErase = j; // FRTQ position trumps other trijets
          //}
        }
        if(isPresent) prev = j;
      }
      if(toErase > -1) {
        resTops.erase(resTops.begin() + toErase);
        i = 0; // iterate procedure for common case of > 2 trijets sharing a jet
      }
    }

    // clean resWs
    for(int i = 0 ; i < nJets ; ++i) { // find ak4 jets shared by multiple Ws
      int prev = -1, toErase = -1;
      for(int j = 0 ; j < resWs.size() ; ++j) {
        bool isPresent = (jets[i]->p4() == resWs[j].jet1.p4()) |
			 (jets[i]->p4() == resWs[j].jet2.p4());
        if(isPresent && prev > -1 ) { // this jet is in two resolved
          toErase = (abs(resWs[prev].m3jet - mW) >
                     abs(resWs[j   ].m3jet - mW)) ? prev : j; // erase one most offshell
          // keep this - might be useful later
          //if( (prev == 0) && (resWs[prev].isFRTQ) ) {
          //  toErase = j; // FRTQ position trumps other trijets
          //}
        }
        if(isPresent) prev = j;
      }
      if(toErase > -1) {
        resWs.erase(resWs.begin() + toErase);
        i = 0; // iterate
      }
    }

    if(dbg) std::cout << "******* top object dump *********" << std::endl;
    if(dbg){
    for(int i = 0 ; i < resTops.size() ; ++i){
      resolved rj = resTops[i];
      std::cout << rj.jet1 << " " << rj.jet2 << " " << rj.jet3 << std::endl;
      std::cout << rj.jet123 << std::endl;
      std::cout << rj.bjet << " " << rj.bjetcsv << " " << rj.hasmbjet << std::endl;
      std::cout << rj.rjet1 << " " << rj.rjet2 << " " << rj.rjet3 << std::endl;
      std::cout << rj.m1 << " " << rj.m2 << " " << rj.m3 << " " << rj.m12 << " " << rj.m23 << " " << rj.m13 << " " << rj.m3jet << std::endl;
      std::cout << rj.nSubjets << " " << rj.isW << std::endl;
    }
    }
    if(dbg) std::cout << "******* w object dump *********" << std::endl;
    if(dbg){
    for(int i = 0 ; i < resWs.size() ; ++i){
      resolved rj = resWs[i];
      std::cout << rj.jet1 << " " << rj.jet2 << " " << rj.jet3 << std::endl;
      std::cout << rj.jet123 << std::endl;
      std::cout << rj.bjet << " " << rj.bjetcsv << " " << rj.hasmbjet << std::endl;
      std::cout << rj.rjet1 << " " << rj.rjet2 << " " << rj.rjet3 << std::endl;
      std::cout << rj.m1 << " " << rj.m2 << " " << rj.m3 << " " << rj.m12 << " " << rj.m23 << " " << rj.m13 << " " << rj.m3jet << std::endl;
      std::cout << rj.nSubjets << " " << rj.isW << std::endl;
    }
    }

    if(dbg) std::cout << "***** event had tops and ws: " << resTops.size() << " " << resWs.size() << std::endl;

    // default reco and gen vars
    int   resnrecotops_       = resTops.size();
    int   resnrecows_         = resWs.size();
    // properties of hardest res object in event
    bool  reshardisw_         = false;
    int   reshardnsubjets_    = -1;
    float reshardgentoppt_    = -9.;
    float reshardgentopeta_   = -9.;
    float reshardrecopt_      = -9.;
    float reshardrecoeta_     = -9.;
    float reshardrecom3jet_   = -9.;
    float reshardrecom12_     = -9.;
    float reshardrecom13_     = -9.;
    float reshardrecom23_     = -9.;
    float reshardrecom1_      = -9.;
    float reshardrecom2_      = -9.;
    float reshardrecom3_      = -9.;
    float reshardrecoj1pt_    = -9.;
    float reshardrecoj1eta_   = -9.;
    float reshardrecoj2pt_    = -9.;
    float reshardrecoj2eta_   = -9.;
    float reshardrecoj3pt_    = -9.;
    float reshardrecoj3eta_   = -9.;

    // properties of hardest matched res object
    int   resnmatchedrecotops_     = -1; // number of reco tops matched to hardest gen top
    bool  reshardmatchedisw_       = false;
    int   reshardmatchednsubjets_  = -1;
    float reshardmatchedgentoppt_  = -9.;
    float reshardmatchedgentopeta_ = -9.;
    float reshardmatchedrecopt_    = -9.;
    float reshardmatchedrecoeta_   = -9.;
    float reshardmatchedrecom3jet_ = -9.;
    float reshardmatchedrecom12_   = -9.;
    float reshardmatchedrecom13_   = -9.;
    float reshardmatchedrecom23_   = -9.;
    float reshardmatchedrecom1_    = -9.;
    float reshardmatchedrecom2_    = -9.;
    float reshardmatchedrecom3_    = -9.;
    float reshardmatchedrecoj1pt_  = -9.;
    float reshardmatchedrecoj1eta_ = -9.;
    float reshardmatchedrecoj2pt_  = -9.;
    float reshardmatchedrecoj2eta_ = -9.;
    float reshardmatchedrecoj3pt_  = -9.;
    float reshardmatchedrecoj3eta_ = -9.;

    // find hardest top
    int ihardtop_ = -1;
    float hardrespt_ = -99.; // the running hardest
    for(int i = 0 ; i < resTops.size() ; ++i) {
      resolved res = resTops[i];
      float respt_ = res.jet123.p4().pt();
      if(respt_ < hardrespt_) continue; // we've found harder
      hardrespt_ = respt_; // update hardest
      ihardtop_ = i;
    }
    if(dbg) std::cout << "hardest reco top index: " << ihardtop_ << std::endl;

    // fill properties of hardest top
    if(ihardtop_ > -1) {
      resolved hardtop_ = resTops[ihardtop_];
      reshardrecopt_    = hardtop_.jet123.p4().pt();
      reshardrecoeta_   = hardtop_.jet123.p4().eta();
      reshardrecom3jet_ = hardtop_.m3jet;
      reshardrecom12_   = hardtop_.m12;
      reshardrecom13_   = hardtop_.m13;
      reshardrecom23_   = hardtop_.m23;
      reshardrecom1_    = hardtop_.m1;
      reshardrecom2_    = hardtop_.m2;
      reshardrecom3_    = hardtop_.m3;
      reshardrecoj1pt_  = hardtop_.jet1.p4().pt();
      reshardrecoj1eta_ = hardtop_.jet1.p4().eta();
      reshardrecoj2pt_  = hardtop_.jet2.p4().pt();
      reshardrecoj2eta_ = hardtop_.jet2.p4().eta();
      reshardrecoj3pt_  = hardtop_.jet3.p4().pt();
      reshardrecoj3eta_ = hardtop_.jet3.p4().eta();
      reshardnsubjets_  = hardtop_.nSubjets;
    }

    // for mc, gen matching vars
    if(ana->isMC()) {
      // find hardest gen top
      GenParticleF* hardGenTop_ = 0;
      float hardgentoppt_ = -99.;
      for(auto* p : ana->genParts) {
        if ((abs(p->pdgId()) == ParticleInfo::p_t) 
            && ParticleInfo::isGenTopHadronic(p) 
            && ParticleInfo::isLastInChain(p)) {
          float gentoppt_ = p->p4().pt();
          if (gentoppt_ < hardgentoppt_) continue;
          hardgentoppt_ = gentoppt_;
          hardGenTop_ = p;
        }
      }
      if(dbg) std::cout << "hardest gen top: " << hardgentoppt_ << hardGenTop_->p4() << std::endl;

      // find hardest gen W
      GenParticleF* hardGenW_ = 0;
      float hardgenwpt_ = -99.;
      for(auto* p : ana->genParts) {
        if ((abs(p->pdgId()) == ParticleInfo::p_Wplus) 
            && ParticleInfo::isGenWHadronic(p) 
            && ParticleInfo::isLastInChain(p)) {
          float genwpt_ = p->p4().pt();
          if (genwpt_ < hardgenwpt_) continue;
          hardgenwpt_ = genwpt_;
          hardGenW_ = p;
        }
      }
      if(dbg) std::cout << "hardest gen W: " << hardgenwpt_ << hardGenW_->p4() << std::endl;

      // find hardest reco top which gen-matches our hardest gen top
      if(hardGenTop_) {
        reshardgentoppt_ = hardGenTop_->p4().pt();
        reshardgentopeta_ = hardGenTop_->p4().eta();
        int iHardestReco_ = -1;
        float hardmatchedrecotoppt_ = -99.; // the running hardest reco top pt
        resnmatchedrecotops_ = 0; // must restart count at zero here (default above is -1 meaning N/A)
        for(int i = 0 ; i < resTops.size() ; ++i) {
          resolved recotop = resTops[i];
          if (PhysicsUtilities::deltaR(recotop.jet123, *hardGenTop_) < 0.4) { // only want matched reco tops
            ++resnmatchedrecotops_;
            float matchedrecotoppt_ = recotop.jet123.p4().pt();
            if(matchedrecotoppt_ < hardmatchedrecotoppt_) continue; // we've already found harder matched reco tops
            hardmatchedrecotoppt_     = matchedrecotoppt_; // update hardest matched reco top pt
            reshardmatchedgentoppt_  = hardGenTop_->p4().pt();  // must fill pt/eta here, or later do = (resnmatchedrecotops_>0) ? hardGenTop_->p4().pt() : -99.;
            reshardmatchedgentopeta_ = hardGenTop_->p4().eta(); //
            iHardestReco_ = i;
          }
        }

        if(iHardestReco_ > -1) {
          resolved* hardRecoTop_     = &resTops[iHardestReco_];
          reshardmatchedrecopt_    = hardRecoTop_->jet123.p4().pt();
          std::cout << "n matched to hardest gen top: " << resnmatchedrecotops_ << reshardmatchedrecopt_ << std::endl;
          reshardmatchedrecoeta_   = hardRecoTop_->jet123.p4().eta();
          reshardmatchedrecom3jet_ = hardRecoTop_->m3jet;
          reshardmatchedrecom12_   = hardRecoTop_->m12;
          reshardmatchedrecom13_   = hardRecoTop_->m13;
          reshardmatchedrecom23_   = hardRecoTop_->m23;
          reshardmatchedrecom1_    = hardRecoTop_->m1;
          reshardmatchedrecom2_    = hardRecoTop_->m2;
          reshardmatchedrecom3_    = hardRecoTop_->m3;
          reshardmatchedrecoj1pt_  = hardRecoTop_->jet1.p4().pt();
          reshardmatchedrecoj1eta_ = hardRecoTop_->jet1.p4().eta();
          reshardmatchedrecoj2pt_  = hardRecoTop_->jet2.p4().pt();
          reshardmatchedrecoj2eta_ = hardRecoTop_->jet2.p4().eta();
          reshardmatchedrecoj3pt_  = hardRecoTop_->jet3.p4().pt();
          reshardmatchedrecoj3eta_ = hardRecoTop_->jet3.p4().eta();
          reshardmatchedisw_       = hardRecoTop_->isW;
        }//if hardRecoTop
      }//if hardGenTop
    }//isMC

    // fill branches
    data->fill<int  >(i_resnrecotops       ,resnrecotops_     );
    data->fill<bool >(i_reshardisw         ,reshardisw_       );
    data->fill<int  >(i_reshardnsubjets    ,reshardnsubjets_  );
    data->fill<float>(i_reshardgentoppt    ,reshardgentoppt_  );
    data->fill<float>(i_reshardgentopeta   ,reshardgentopeta_ );
    data->fill<float>(i_reshardrecopt      ,reshardrecopt_    );
    data->fill<float>(i_reshardrecoeta     ,reshardrecoeta_   );
    data->fill<float>(i_reshardrecom3jet   ,reshardrecom3jet_ );
    data->fill<float>(i_reshardrecom12     ,reshardrecom12_   );
    data->fill<float>(i_reshardrecom13     ,reshardrecom13_   );
    data->fill<float>(i_reshardrecom23     ,reshardrecom23_   );
    data->fill<float>(i_reshardrecom1      ,reshardrecom1_    );
    data->fill<float>(i_reshardrecom2      ,reshardrecom2_    );
    data->fill<float>(i_reshardrecom3      ,reshardrecom3_    );
    data->fill<float>(i_reshardrecoj1pt    ,reshardrecoj1pt_  );
    data->fill<float>(i_reshardrecoj1eta   ,reshardrecoj1eta_ );
    data->fill<float>(i_reshardrecoj2pt    ,reshardrecoj2pt_  );
    data->fill<float>(i_reshardrecoj2eta   ,reshardrecoj2eta_ );
    data->fill<float>(i_reshardrecoj3pt    ,reshardrecoj3pt_  );
    data->fill<float>(i_reshardrecoj3eta   ,reshardrecoj3eta_ );
    data->fill<int  >(i_resnmatchedrecotops       ,resnmatchedrecotops_     );
    data->fill<bool >(i_reshardmatchedisw         ,reshardmatchedisw_       );
    data->fill<int  >(i_reshardmatchednsubjets    ,reshardmatchednsubjets_  );
    data->fill<float>(i_reshardmatchedgentoppt    ,reshardmatchedgentoppt_  );
    data->fill<float>(i_reshardmatchedgentopeta   ,reshardmatchedgentopeta_ );
    data->fill<float>(i_reshardmatchedrecopt      ,reshardmatchedrecopt_    );
    data->fill<float>(i_reshardmatchedrecoeta     ,reshardmatchedrecoeta_   );
    data->fill<float>(i_reshardmatchedrecom3jet   ,reshardmatchedrecom3jet_ );
    data->fill<float>(i_reshardmatchedrecom12     ,reshardmatchedrecom12_   );
    data->fill<float>(i_reshardmatchedrecom13     ,reshardmatchedrecom13_   );
    data->fill<float>(i_reshardmatchedrecom23     ,reshardmatchedrecom23_   );
    data->fill<float>(i_reshardmatchedrecom1      ,reshardmatchedrecom1_    );
    data->fill<float>(i_reshardmatchedrecom2      ,reshardmatchedrecom2_    );
    data->fill<float>(i_reshardmatchedrecom3      ,reshardmatchedrecom3_    );
    data->fill<float>(i_reshardmatchedrecoj1pt    ,reshardmatchedrecoj1pt_  );
    data->fill<float>(i_reshardmatchedrecoj1eta   ,reshardmatchedrecoj1eta_ );
    data->fill<float>(i_reshardmatchedrecoj2pt    ,reshardmatchedrecoj2pt_  );
    data->fill<float>(i_reshardmatchedrecoj2eta   ,reshardmatchedrecoj2eta_ );
    data->fill<float>(i_reshardmatchedrecoj3pt    ,reshardmatchedrecoj3pt_  );
    data->fill<float>(i_reshardmatchedrecoj3eta   ,reshardmatchedrecoj3eta_ );

    return;
  }//fillResInfo

};//ResTreeFiller

#endif
