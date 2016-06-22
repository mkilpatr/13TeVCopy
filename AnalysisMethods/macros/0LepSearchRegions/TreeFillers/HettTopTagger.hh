/*
  Order of HETT top cand discovery and cleaning according to PAS and AN (and sometimes having to read between the lines...):
    
    Discovery order: the best FRTQ ("best" by |m-mtop|)
                     more unordered FRTQs
                     additional trijets
                     monojets
                     dijet+jets
 
    Cleaning order: Clean all trijets except the best FRTQ for overlaps, keeping cand with smallest |m-mtop|
                    Clean all monojets & dijet+jets. Do not use trijets' subets to discover these. 

    "Fully reconstructed top quarks" (FRTQs) (ANv9 p11) are trijet cands having a bjet in rsys. 
      One with best |m-mtop| is "the" FRTQ (ANv9 p13). It's protected from cleaning wrt any other cands.
      Events with nTops=1 are rejected (hettrejectevent) if no FRTQ exists. Helps the MT2 calculation. ANv9 p7/8.
    "Remaining system" (rsys) of a top object is all AK4 jets minus this object's jets. If nTops=1 then no ambiguity.

    Method used here (no diff from AN/PAS I can find):
      Discover FRTQs, putting best (|m-mtop|) at first position. ANv9 p13. Reason: it gets cleaning priority.
      Discover additional trijets (not requiring bjet in rsys).
      Clean trijets, never bumping the best FRTQ, keeping cand with smallest |m-mtop|.
      Discover monojets using AK4 jets not already used in trijets.
      Discover dijet+jet cands using AK4 jets not already used in trijets. Thus mono/dijets can't bump trijets during cleaning.
        From PASv6 p18 (monojets implied):
         "Jets which are already included in a top-tagged jet triplet, as described above, are excluded from being considered in dijet+jet candidates."        
      Clean monojets and dijet+jets, keeping cand with smallest |m-mtop|.
      Reject events with nTops=1 with no FRTQ. ANv9 p7.
      Calculate MT2 using top cands.
*/

#ifndef HETTTOPTAGGER_HH
#define HETTTOPTAGGER_HH

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
#include "AnalysisTools/KinematicVariables/interface/mt2w.h"
#include "AnalysisTools/KinematicVariables/interface/Mt2Helper.h"

using namespace ucsbsusy;

struct hettTop {
  hettTop() : rsysbjetcsv(0.), rjet1(0), rjet2(0), rjet3(0),
              m1(-1.),  m2(-1.),  m3(-1.),
              m12(-1.), m13(-1.), m23(-1.), m3jet(-1.), 
              isFRTQ(false) {}
  enum NSubjets { MONOJET = 1, DIJET, TRIJET };
  MomentumF jet1, jet2, jet3;      // pt-ordered subjet momenta
  MomentumF jet123;                // top object momentum, the sum of subjets' p4
  MomentumF rsysbjet;              // best bjet (CSV) in rsys for FRTQs
  float     rsysbjetcsv;           // csv of rsysbjet
  RecoJetF *rjet1, *rjet2, *rjet3; // pt-ordered original AK4 subjets
  float     m1, m2, m3;            // subjet masses
  float     m12, m13, m23;         // pairwise subjet masses
  float     m3jet;                 // top object total mass
  NSubjets  nSubjets;              // 1 to 3 AK4 subjets. indicates which of above are expected to be filled
  bool      isFRTQ;                // FRTQ = trijet with an mbjet in the rsys (AK4 jets minus this trijet)
};
std::vector<hettTop > hettTops;

struct HettTreeFiller {

  HettTreeFiller() {}

  size i_mt2;
  size i_hettrejectevent;
  // matched
  size i_hettnmatchedrecotops;
  size i_hetthardmatchednsubjets;
  size i_hetthardmatchedgentoppt;
  size i_hetthardmatchedgentopeta;
  size i_hetthardmatchedrecopt;
  size i_hetthardmatchedrecoeta;
  size i_hetthardmatchedrecom3jet;
  size i_hetthardmatchedrecom12;
  size i_hetthardmatchedrecom13;
  size i_hetthardmatchedrecom23;
  size i_hetthardmatchedrecom1;
  size i_hetthardmatchedrecom2;
  size i_hetthardmatchedrecom3;
  size i_hetthardmatchedrecoj1pt;
  size i_hetthardmatchedrecoj1eta;
  size i_hetthardmatchedrecoj2pt;
  size i_hetthardmatchedrecoj2eta;
  size i_hetthardmatchedrecoj3pt;
  size i_hetthardmatchedrecoj3eta;
  // unmatched
  size i_hettnrecotops;
  size i_hetthardnsubjets;
  size i_hetthardgentoppt;
  size i_hetthardgentopeta;
  size i_hetthardrecopt;
  size i_hetthardrecoeta;
  size i_hetthardrecom3jet;
  size i_hetthardrecom12;
  size i_hetthardrecom13;
  size i_hetthardrecom23;
  size i_hetthardrecom1;
  size i_hetthardrecom2;
  size i_hetthardrecom3;
  size i_hetthardrecoj1pt;
  size i_hetthardrecoj1eta;
  size i_hetthardrecoj2pt;
  size i_hetthardrecoj2eta;
  size i_hetthardrecoj3pt;
  size i_hetthardrecoj3eta;

  void book(TreeWriterData* data) {
    i_mt2                         = data->add<float>("","mt2","F",0);
    i_hettrejectevent             = data->add<bool >("","hettrejectevent","O",0);
    //matched
    i_hettnmatchedrecotops        = data->add<int  >("","hettnmatchedrecotops","I",0);
    i_hetthardmatchednsubjets     = data->add<int  >("","hetthardmatchednsubjets","I",0);
    i_hetthardmatchedgentoppt     = data->add<float>("","hetthardmatchedgentoppt","F",0);
    i_hetthardmatchedgentopeta    = data->add<float>("","hetthardmatchedgentopeta","F",0);
    i_hetthardmatchedrecopt       = data->add<float>("","hetthardmatchedrecopt","F",0);
    i_hetthardmatchedrecoeta      = data->add<float>("","hetthardmatchedrecoeta","F",0);
    i_hetthardmatchedrecom3jet    = data->add<float>("","hetthardmatchedrecom3jet","F",0);
    i_hetthardmatchedrecom12      = data->add<float>("","hetthardmatchedrecom12","F",0);
    i_hetthardmatchedrecom13      = data->add<float>("","hetthardmatchedrecom13","F",0);
    i_hetthardmatchedrecom23      = data->add<float>("","hetthardmatchedrecom23","F",0);
    i_hetthardmatchedrecom1       = data->add<float>("","hetthardmatchedrecom1","F",0);
    i_hetthardmatchedrecom2       = data->add<float>("","hetthardmatchedrecom2","F",0);
    i_hetthardmatchedrecom3       = data->add<float>("","hetthardmatchedrecom3","F",0);
    i_hetthardmatchedrecoj1pt     = data->add<float>("","hetthardmatchedrecoj1pt","F",0);
    i_hetthardmatchedrecoj1eta    = data->add<float>("","hetthardmatchedrecoj1eta","F",0);
    i_hetthardmatchedrecoj2pt     = data->add<float>("","hetthardmatchedrecoj2pt","F",0);
    i_hetthardmatchedrecoj2eta    = data->add<float>("","hetthardmatchedrecoj2eta","F",0);
    i_hetthardmatchedrecoj3pt     = data->add<float>("","hetthardmatchedrecoj3pt","F",0);
    i_hetthardmatchedrecoj3eta    = data->add<float>("","hetthardmatchedrecoj3eta","F",0);
    //unmatched
    i_hettnrecotops               = data->add<int  >("","hettnrecotops","I",0);
    i_hetthardnsubjets            = data->add<int  >("","hetthardnsubjets","I",0);
    i_hetthardgentoppt            = data->add<float>("","hetthardgentoppt","F",0);
    i_hetthardgentopeta           = data->add<float>("","hetthardgentopeta","F",0);
    i_hetthardrecopt              = data->add<float>("","hetthardrecopt","F",0);
    i_hetthardrecoeta             = data->add<float>("","hetthardrecoeta","F",0);
    i_hetthardrecom3jet           = data->add<float>("","hetthardrecom3jet","F",0);
    i_hetthardrecom12             = data->add<float>("","hetthardrecom12","F",0);
    i_hetthardrecom13             = data->add<float>("","hetthardrecom13","F",0);
    i_hetthardrecom23             = data->add<float>("","hetthardrecom23","F",0);
    i_hetthardrecom1              = data->add<float>("","hetthardrecom1","F",0);
    i_hetthardrecom2              = data->add<float>("","hetthardrecom2","F",0);
    i_hetthardrecom3              = data->add<float>("","hetthardrecom3","F",0);
    i_hetthardrecoj1pt            = data->add<float>("","hetthardrecoj1pt","F",0);
    i_hetthardrecoj1eta           = data->add<float>("","hetthardrecoj1eta","F",0);
    i_hetthardrecoj2pt            = data->add<float>("","hetthardrecoj2pt","F",0);
    i_hetthardrecoj2eta           = data->add<float>("","hetthardrecoj2eta","F",0);
    i_hetthardrecoj3pt            = data->add<float>("","hetthardrecoj3pt","F",0);
    i_hetthardrecoj3eta           = data->add<float>("","hetthardrecoj3eta","F",0);
  }

  // fill HETT branches
  void fillHettInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, vector<RecoJetF*> jets, vector<hettTop> hettTops) {
    float mW   =  80.4;
    float mTop = 173.4;
    float rMin = 0.85*mW/mTop;
    float rMax = 1.25*mW/mTop;

    // use only pt > 30 jets in hett. shouldn't see 'jets[' or ': jets(' after this.
    vector<RecoJetF*> jets30;
    for(auto* jet : jets) {
      if(jet->p4().pt() > 30.) jets30.push_back(jet);
    }

    MomentumF jet1, jet2, jet3, jet123;
    int nJets = jets30.size();

    // discover trijet cands using unique ak4 triplets
    for (int i = 0 ; i < nJets-2 ; ++i) {
      for(int j = i+1 ; j < nJets-1 ; ++j) {
        for(int k = j+1 ; k < nJets ; ++k) {
          jet1 = jets30[i]->p4(); jet2 = jets30[j]->p4(); jet3 = jets30[k]->p4();
          jet123 = jet1.p4() + jet2.p4() + jet3.p4();
          float m12  = (jet1.p4() + jet2.p4()).mass();
          float m13  = (jet1.p4() + jet3.p4()).mass();
          float m23  = (jet2.p4() + jet3.p4()).mass();
          float m3jet = jet123.mass(); // note m123 ~ m3jet for p_i^2 ~ 0.

          // condition (i)
          if((PhysicsUtilities::deltaR(jet123,jet1) > 1.5) ||
             (PhysicsUtilities::deltaR(jet123,jet2) > 1.5) ||
             (PhysicsUtilities::deltaR(jet123,jet3) > 1.5)) continue;

          // condition (ii)
          if((m3jet > 250.) || (m3jet < 100.)) continue;

          // condition (iii) has subconditions A1 A2 B C
          bool condA1 = (0.2 < atan(m13/m12)) &
                        (1.3 > atan(m13/m12));

          bool condA2 = (rMin < m23/m3jet) &
                        (rMax > m23/m3jet);
          bool condA = condA1 & condA2;

          // condition B
          bool condB = (pow(rMin,2)*(1+pow(m13/m12,2)) < 1-pow(m23/m3jet,2)) &
                       (pow(rMax,2)*(1+pow(m13/m12,2)) > 1-pow(m23/m3jet,2)) &
                       (m23/m3jet > 0.35);

          // condition C is condition B with jet 2 <-> 3
          bool condC = (pow(rMin,2)*(1+pow(m12/m13,2)) < 1-pow(m23/m3jet,2)) &
                       (pow(rMax,2)*(1+pow(m12/m13,2)) > 1-pow(m23/m3jet,2)) &
                       (m23/m3jet > 0.35);

          // enforce either A or B or C
          if (!(condA || condB || condC)) continue;

          hettTop cand; // fill and submit this hett candidate
          cand.nSubjets = hettTop::TRIJET;
          cand.jet1  = jet1;                cand.jet2  = jet2;               cand.jet3  = jet3; cand.jet123 = jet123;
          cand.rjet1 = jets30[i];           cand.rjet2 = jets30[j];          cand.rjet3 = jets30[k];
          cand.m1    = (jet1.p4()).mass();  cand.m2    = (jet2.p4()).mass(); cand.m3    = (jet3.p4()).mass();
          cand.m12   = m12; cand.m23 = m23; cand.m13   = m13;                cand.m3jet = m3jet;
 
          // is trijet a FRTQ?
          for(int i = 0 ; i < nJets ; ++i) {
            bool isUsed  =    (jets30[i]->p4() == cand.jet1.p4()) |
                              (jets30[i]->p4() == cand.jet2.p4()) |
                              (jets30[i]->p4() == cand.jet3.p4());
            if(isUsed) continue; // find rsys jets
            if(jets30[i]->csv() > defaults::CSV_MEDIUM) {
              cand.isFRTQ = true;
              if(jets30[i]->csv() > cand.rsysbjetcsv) {
                cand.rsysbjet    = jets30[i]->p4();
                cand.rsysbjetcsv = jets30[i]->csv();
              }
            }
          }
          // trijet cand is either best FRTQ (prepend to hettTops) or not (append -- unordered)
          if( (cand.isFRTQ) && (hettTops.size()>0) ) {
            if( hettTops[0].isFRTQ && (abs(cand.m3jet - mTop) < abs(hettTops[0].m3jet - mTop)) ) {
              hettTops.insert(hettTops.begin()+0,cand); // insert new best FRTQ at index 0
            } else {
              hettTops.push_back(cand);
            }
          } else {
              hettTops.push_back(cand);
          }
        }//for jet
      }//for jet
    }//for jet

    /* 
       clean trijets wrt each other. if two trijets share a jet, keep trijet with smallest |m-mtop|.
       if first jet is a FRTQ it is protected from cleaning
    */
    for(int i = 0 ; i < nJets ; ++i) {
      int prev = -1, toErase = -1;
      for(int j = 0 ; j < hettTops.size() ; ++j) {
        bool isPresent = (jets30[i]->p4() == hettTops[j].jet1.p4()) |
			 (jets30[i]->p4() == hettTops[j].jet2.p4()) |
                         (jets30[i]->p4() == hettTops[j].jet3.p4());
        if(isPresent && prev > -1 ) { // this jet is in two hettTops
          toErase = (abs(hettTops[prev].m3jet - mTop) >
                     abs(hettTops[j   ].m3jet - mTop)) ? prev : j; // erase one with largest gap from mTop
          if( (prev == 0) && (hettTops[prev].isFRTQ) ) {
            toErase = j; // FRTQ position trumps other trijets
          }
        }
        if(isPresent) prev = j;
      }
      if(toErase > -1) {
        hettTops.erase(hettTops.begin() + toErase);
        i = 0; // iterate procedure for common case of > 2 trijets sharing a jet
      }
    }

    // find dijets and monojets (boosted scenarios)
    for(auto jet : jets30) {
      // ignore any jets already used in a trijet
      bool jetUsed = false;
      for(auto t : hettTops) {
        if(t.nSubjets != hettTop::TRIJET) continue; // only use jets not already used by trijets
        jetUsed |= ((jet->p4() == t.jet1.p4()) |
                    (jet->p4() == t.jet2.p4()) |
                    (jet->p4() == t.jet3.p4()));
      }
      if(jetUsed) continue;

      // boosted top "monojet", which stands alone
      if ((jet->mass() > 110.) && (jet->mass() < 220.)) {
        hettTop cand; // fill and submit this hett candidate
        cand.nSubjets = hettTop::MONOJET;
        cand.jet1  = jet->p4(); cand.jet123 = cand.jet1;
        cand.rjet1 = jet;
        cand.m1    = jet->mass(); cand.m3jet = cand.m1;
        hettTops.push_back(cand);
      }

      // boosted W "dijet", which requires an auxiliary jet
      else if ((jet->mass() > 70.) && (jet->mass() < 110.)) {
        int iCandAux = -1;
        for(int i = 0 ; i < nJets ; ++i) { // find an auxiliary jet for the W
          if(jets30[i]->p4() == jet->p4()) continue; // ignore self

          bool isUsed = false; // only use jets not already in a trijet
          for(auto t : hettTops) {
            if(t.nSubjets != hettTop::TRIJET) continue; 
            isUsed |= ((jets30[i]->p4() == t.jet1.p4()) |
                       (jets30[i]->p4() == t.jet2.p4()) |
                       (jets30[i]->p4() == t.jet3.p4()));
          }
          if(isUsed) continue;

          // two conditions on dijet + jet system
          float massDijet        = jet->mass();
          float massDijetPlusJet = (jet->p4()+jets30[i]->p4()).mass();
          bool condA = (rMin < massDijet/massDijetPlusJet) &
                       (rMax > massDijet/massDijetPlusJet);
          bool condB = (100. < massDijetPlusJet) &
                       (250. > massDijetPlusJet);
          if(condA && condB) {
            iCandAux = i; // found a suitable aux jet

            hettTop cand ; // fill and submit this hett top candidate
            cand.nSubjets = hettTop::DIJET;
            cand.jet1  = jet->p4(); cand.jet2 = jets30[iCandAux]->p4(); cand.jet123 = cand.jet1.p4()+cand.jet2.p4();
            cand.rjet1 = jet; cand.rjet2 = jets30[iCandAux];
            cand.m1    = (jet->p4()).mass();
            cand.m2    = (jets30[iCandAux]->p4()).mass();
            cand.m12   = (jet->p4() + jets30[iCandAux]->p4()).mass();
            cand.m3jet = (jet->p4() + jets30[iCandAux]->p4()).mass();
            hettTops.push_back(cand);
          }//conditions
        }//aux
      }//ifW
    }//mono/dijets

    // clean dijets/monojets. can effect by cleaning all cands (except FRTQ), since dijets/monojets already orthogonal to trijets.
    for(int i = 0 ; i < nJets ; ++i) {
      int prev = -1, toErase = -1;
      for(int j = 0 ; j < hettTops.size() ; ++j) {
        bool isPresent = (jets30[i]->p4() == hettTops[j].jet1.p4()) |
                         (jets30[i]->p4() == hettTops[j].jet2.p4()) |
                         (jets30[i]->p4() == hettTops[j].jet3.p4());
        if(isPresent && prev > -1 ) { // this jet is in two hettTops
          toErase = (abs(hettTops[prev].m3jet - mTop) >
                     abs(hettTops[j   ].m3jet - mTop)) ? prev : j; // erase one with largest gap from mTop
          if( (prev == 0) && (hettTops[prev].isFRTQ) ) {
            toErase = j; // FRTQ position trumps other trijets
          }
        }
        if(isPresent) prev = j;
      }
      if(toErase > -1) {
        hettTops.erase(hettTops.begin() + toErase);
        i = 0; // iterate procedure for case of > 2 objects sharing a jet
      }
    }

    // reco branches and mc efficiency branches
    int   hettnrecotops_       = hettTops.size();
    // properties of hardest (unmatched) hett top (per-jet basis)
    int   hetthardnsubjets_    = -1;
    float hetthardgentoppt_    = -9.;
    float hetthardgentopeta_   = -9.;
    float hetthardrecopt_      = -9.;
    float hetthardrecoeta_     = -9.;
    float hetthardrecom3jet_   = -9.;
    float hetthardrecom12_     = -9.;
    float hetthardrecom13_     = -9.;
    float hetthardrecom23_     = -9.;
    float hetthardrecom1_      = -9.;
    float hetthardrecom2_      = -9.;
    float hetthardrecom3_      = -9.;
    float hetthardrecoj1pt_    = -9.;
    float hetthardrecoj1eta_   = -9.;
    float hetthardrecoj2pt_    = -9.;
    float hetthardrecoj2eta_   = -9.;
    float hetthardrecoj3pt_    = -9.;
    float hetthardrecoj3eta_   = -9.;

    // properties of hardest gen-matched hett top (per-jet basis)
    int   hettnmatchedrecotops_     = -1; // number of reco tops matched to hardest gen top
    int   hetthardmatchednsubjets_  = -1;
    float hetthardmatchedgentoppt_  = -9.;
    float hetthardmatchedgentopeta_ = -9.;
    float hetthardmatchedrecopt_    = -9.;
    float hetthardmatchedrecoeta_   = -9.;
    float hetthardmatchedrecom3jet_ = -9.;
    float hetthardmatchedrecom12_   = -9.;
    float hetthardmatchedrecom13_   = -9.;
    float hetthardmatchedrecom23_   = -9.;
    float hetthardmatchedrecom1_    = -9.;
    float hetthardmatchedrecom2_    = -9.;
    float hetthardmatchedrecom3_    = -9.;
    float hetthardmatchedrecoj1pt_  = -9.;
    float hetthardmatchedrecoj1eta_ = -9.;
    float hetthardmatchedrecoj2pt_  = -9.;
    float hetthardmatchedrecoj2eta_ = -9.;
    float hetthardmatchedrecoj3pt_  = -9.;
    float hetthardmatchedrecoj3eta_ = -9.;

    // unmatched properties
    // find hardest reco top
    int iHardestHett_ = -1;
    float hardhettpt_ = -99.; // the running hardest reco top pt
    for(int i = 0 ; i < hettTops.size() ; ++i) {
      hettTop htop = hettTops[i];
      float htoppt_ = htop.jet123.p4().pt();
      if(htoppt_ < hardhettpt_) continue; // we've already found harder reco tops
      hardhettpt_ = htoppt_; // update hardest reco top pt
      iHardestHett_ = i;
    }
    // fill properties of hardest unmatched reco top
    if(iHardestHett_ > -1) {
      hettTop hardHett_  = hettTops[iHardestHett_];
      hetthardrecopt_    = hardHett_.jet123.p4().pt();
      hetthardrecoeta_   = hardHett_.jet123.p4().eta();
      hetthardrecom3jet_ = hardHett_.m3jet;
      hetthardrecom12_   = hardHett_.m12;
      hetthardrecom13_   = hardHett_.m13;
      hetthardrecom23_   = hardHett_.m23;
      hetthardrecom1_    = hardHett_.m1;
      hetthardrecom2_    = hardHett_.m2;
      hetthardrecom3_    = hardHett_.m3;
      hetthardrecoj1pt_  = hardHett_.jet1.p4().pt();
      hetthardrecoj1eta_ = hardHett_.jet1.p4().eta();
      hetthardrecoj2pt_  = hardHett_.jet2.p4().pt();
      hetthardrecoj2eta_ = hardHett_.jet2.p4().eta();
      hetthardrecoj3pt_  = hardHett_.jet3.p4().pt();
      hetthardrecoj3eta_ = hardHett_.jet3.p4().eta();
      hetthardnsubjets_  = hardHett_.nSubjets;
    }//if hardHett

    if(ana->isMC()) {

      // gen-matched properties
      // find hardest gen top
      GenParticleF* hardGenTop_ = 0;
      float hardgentoppt_ = -99.; // the running hardest gen top pt
      for(auto* p : ana->genParts) {
        if ((abs(p->pdgId()) == ParticleInfo::p_t) 
            && ParticleInfo::isGenTopHadronic(p) 
            && ParticleInfo::isLastInChain(p)) { // only want hadronic gen tops last in chain
          float gentoppt_ = p->p4().pt();
          if (gentoppt_ < hardgentoppt_) continue; // we've already found harder gen tops
          hardgentoppt_ = gentoppt_; // update hardest gen top pt
          hardGenTop_ = p;
        }
      }

      if(hardGenTop_) {
        hetthardgentoppt_ = hardGenTop_->p4().pt();
        hetthardgentopeta_ = hardGenTop_->p4().eta();
        // find hardest reco top which gen-matches our hardest gen top
        int iHardestReco_ = -1;
        float hardmatchedrecotoppt_ = -99.; // the running hardest reco top pt
        hettnmatchedrecotops_ = 0; // must restart count at zero here (default above is -1 meaning N/A)
        for(int i = 0 ; i < hettTops.size() ; ++i) {
          hettTop recotop = hettTops[i];
          if (PhysicsUtilities::deltaR(recotop.jet123, *hardGenTop_) < 0.4) { // only want matched reco tops
            ++hettnmatchedrecotops_;
            float matchedrecotoppt_ = recotop.jet123.p4().pt();
            if(matchedrecotoppt_ < hardmatchedrecotoppt_) continue; // we've already found harder matched reco tops
            hardmatchedrecotoppt_     = matchedrecotoppt_; // update hardest matched reco top pt
            hetthardmatchedgentoppt_  = hardGenTop_->p4().pt();  // must fill pt/eta here, or later do = (hettnmatchedrecotops_>0) ? hardGenTop_->p4().pt() : -99.;
            hetthardmatchedgentopeta_ = hardGenTop_->p4().eta(); //
            iHardestReco_ = i;
          }
        }
        if(iHardestReco_ > -1) {
          hettTop* hardRecoTop_ = &hettTops[iHardestReco_];
          hetthardmatchedrecopt_    = hardRecoTop_->jet123.p4().pt();
          hetthardmatchedrecoeta_   = hardRecoTop_->jet123.p4().eta();
          hetthardmatchedrecom3jet_ = hardRecoTop_->m3jet;
          hetthardmatchedrecom12_   = hardRecoTop_->m12;
          hetthardmatchedrecom13_   = hardRecoTop_->m13;
          hetthardmatchedrecom23_   = hardRecoTop_->m23;
          hetthardmatchedrecom1_    = hardRecoTop_->m1;
          hetthardmatchedrecom2_    = hardRecoTop_->m2;
          hetthardmatchedrecom3_    = hardRecoTop_->m3;
          hetthardmatchedrecoj1pt_  = hardRecoTop_->jet1.p4().pt();
          hetthardmatchedrecoj1eta_ = hardRecoTop_->jet1.p4().eta();
          hetthardmatchedrecoj2pt_  = hardRecoTop_->jet2.p4().pt();
          hetthardmatchedrecoj2eta_ = hardRecoTop_->jet2.p4().eta();
          hetthardmatchedrecoj3pt_  = hardRecoTop_->jet3.p4().pt();
          hetthardmatchedrecoj3eta_ = hardRecoTop_->jet3.p4().eta();
          if(hardRecoTop_->m13 < 0) { // either dijet or monojet
            if(hardRecoTop_->m12 < 0) { // monojet
              hetthardmatchednsubjets_ = 1;
            } else { // dijet
              hetthardmatchednsubjets_ = 2;
            }
          } else { // trijet
            hetthardmatchednsubjets_ = 3;
          }

        }//if hardRecoTop
      }//if hardGenTop
    }//isMC

    // reject event?
    bool hettrejectevent_ = (hettnrecotops_ == 1) && !(hettTops[0].isFRTQ); // for nTops=1, need a bjet in the rsys for MT2 calc
    /* 
       MT2 calculation
       takes two visible sides, (1) and (2), and the invisible MET (combo of two invisible sides)
       if nTops >= 2, take min of mt2 using all combos (1)=topi, (2)=topj
       if nTops == 1, take (1) = top, (2) = best bjet in rsys + aux jet
         take closest aux jet to the bjet giving m_bj in [50,mTop]
         if one can't be found (2) = that bjet
       the invisible mass is zero per AN and PAS, since this MT2 targets ttbar (mNu = 0) reconstruction, not t2tt (mInv = mLSP). 
         PAS typo? It talks about MT2 using Inv = LSP and having kinematic endpoint at mSTOP, then says mInv=0 is used.
         ** If mInv = 0 is used, still has endpoint at mSTOP, just duller.
    */
    MomentumF ptmiss(ana->met->p4());
    MomentumF visibleA, visibleB;
    visibleA = ptmiss; visibleB = ptmiss; // debug purposes 
    float mt2_ = -9.;
    int nTops = hettTops.size(); 
    Mt2Helper mt2helper; // can use repeatedly

    if(!hettrejectevent_) {
      if(nTops>= 2) { 
        // take all combos of top cands per PAS. I expected to use any FRTQs specially here...
        for (int i = 0 ; i < nTops-1 ; ++i) {
          for(int j = i+1 ; j < nTops ; ++j) {
            visibleA = hettTops[i].jet123.p4(); visibleB = hettTops[j].jet123.p4();
            float thismt2 = mt2helper.CalcMT2(&visibleA, &visibleB, &ptmiss);
            mt2_ = (mt2_ > 0) ? min(mt2_, thismt2) : thismt2;
          }
        }
      } else if (nTops == 1) {
        // find closest jet in rsys to the rsys bjet s.t. m_bj in [50,mTop]
        visibleA = hettTops[0].jet123.p4(); // is a FRTQ by the hettrejectevent rule
        assert(hettTops[0].isFRTQ);
        MomentumF auxjet;
        float auxjetdr = -1.; // running dr of aux jet to bjet (minimize)
        float mbj = -1.;
        for(auto jet : jets30) {
          if( (jet->p4() == hettTops[0].jet1.p4()) ||
              (jet->p4() == hettTops[0].jet2.p4()) ||
              (jet->p4() == hettTops[0].jet3.p4()) ||
              (jet->p4() == hettTops[0].rsysbjet.p4()) ) continue;
          float thisauxjetdr = PhysicsUtilities::deltaR(*jet, hettTops[0].rsysbjet);
          float mbj = (hettTops[0].rsysbjet.p4() + jet->p4()).mass();
          if( ((thisauxjetdr < auxjetdr) || (auxjetdr < 0)) // best or first seen
                     && ((mbj > 50.) && (mbj < mTop)) ){
            auxjet = jet->p4(); auxjetdr = thisauxjetdr;
          }
        }
        visibleB = (auxjetdr > 0) ? auxjet : hettTops[0].rsysbjet.p4();
        mt2_ = mt2helper.CalcMT2(&visibleA, &visibleB, &ptmiss);
      }//ntops1
    }//!rejectevent

    // fill branches
    data->fill<float>(i_mt2,mt2_);
    data->fill<bool >(i_hettrejectevent     ,hettrejectevent_   );
    data->fill<int  >(i_hettnrecotops       ,hettnrecotops_     );
    data->fill<int  >(i_hetthardnsubjets    ,hetthardnsubjets_  );
    data->fill<float>(i_hetthardgentoppt    ,hetthardgentoppt_  );
    data->fill<float>(i_hetthardgentopeta   ,hetthardgentopeta_ );
    data->fill<float>(i_hetthardrecopt      ,hetthardrecopt_    );
    data->fill<float>(i_hetthardrecoeta     ,hetthardrecoeta_   );
    data->fill<float>(i_hetthardrecom3jet   ,hetthardrecom3jet_ );
    data->fill<float>(i_hetthardrecom12     ,hetthardrecom12_   );
    data->fill<float>(i_hetthardrecom13     ,hetthardrecom13_   );
    data->fill<float>(i_hetthardrecom23     ,hetthardrecom23_   );
    data->fill<float>(i_hetthardrecom1      ,hetthardrecom1_    );
    data->fill<float>(i_hetthardrecom2      ,hetthardrecom2_    );
    data->fill<float>(i_hetthardrecom3      ,hetthardrecom3_    );
    data->fill<float>(i_hetthardrecoj1pt    ,hetthardrecoj1pt_  );
    data->fill<float>(i_hetthardrecoj1eta   ,hetthardrecoj1eta_ );
    data->fill<float>(i_hetthardrecoj2pt    ,hetthardrecoj2pt_  );
    data->fill<float>(i_hetthardrecoj2eta   ,hetthardrecoj2eta_ );
    data->fill<float>(i_hetthardrecoj3pt    ,hetthardrecoj3pt_  );
    data->fill<float>(i_hetthardrecoj3eta   ,hetthardrecoj3eta_ );
    data->fill<int  >(i_hettnmatchedrecotops       ,hettnmatchedrecotops_     );
    data->fill<int  >(i_hetthardmatchednsubjets    ,hetthardmatchednsubjets_  );
    data->fill<float>(i_hetthardmatchedgentoppt    ,hetthardmatchedgentoppt_  );
    data->fill<float>(i_hetthardmatchedgentopeta   ,hetthardmatchedgentopeta_ );
    data->fill<float>(i_hetthardmatchedrecopt      ,hetthardmatchedrecopt_    );
    data->fill<float>(i_hetthardmatchedrecoeta     ,hetthardmatchedrecoeta_   );
    data->fill<float>(i_hetthardmatchedrecom3jet   ,hetthardmatchedrecom3jet_ );
    data->fill<float>(i_hetthardmatchedrecom12     ,hetthardmatchedrecom12_   );
    data->fill<float>(i_hetthardmatchedrecom13     ,hetthardmatchedrecom13_   );
    data->fill<float>(i_hetthardmatchedrecom23     ,hetthardmatchedrecom23_   );
    data->fill<float>(i_hetthardmatchedrecom1      ,hetthardmatchedrecom1_    );
    data->fill<float>(i_hetthardmatchedrecom2      ,hetthardmatchedrecom2_    );
    data->fill<float>(i_hetthardmatchedrecom3      ,hetthardmatchedrecom3_    );
    data->fill<float>(i_hetthardmatchedrecoj1pt    ,hetthardmatchedrecoj1pt_  );
    data->fill<float>(i_hetthardmatchedrecoj1eta   ,hetthardmatchedrecoj1eta_ );
    data->fill<float>(i_hetthardmatchedrecoj2pt    ,hetthardmatchedrecoj2pt_  );
    data->fill<float>(i_hetthardmatchedrecoj2eta   ,hetthardmatchedrecoj2eta_ );
    data->fill<float>(i_hetthardmatchedrecoj3pt    ,hetthardmatchedrecoj3pt_  );
    data->fill<float>(i_hetthardmatchedrecoj3eta   ,hetthardmatchedrecoj3eta_ );

    return;
  }//fillHettInfo

};//hettTreeFiller

#endif
