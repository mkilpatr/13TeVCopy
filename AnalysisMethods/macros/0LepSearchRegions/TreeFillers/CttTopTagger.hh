#ifndef CTTTOPTAGGER_HH
#define CTTTOPTAGGER_HH

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

using namespace ucsbsusy;

struct CttTreeFiller {

  CttTreeFiller() {}

  // below is for top / w tagging and SF extraction
  size i_sfbclose2lep;
  size i_sfncttcand  ;
  size i_sfcttcandpt ;
  size i_sfcttcandeta;
  size i_sfcttcandallsubjetmass;
  size i_sfcttcandcmstoptagmass;
  size i_sfcttcandnsubjets;
  size i_sfcttcandminmass;
  size i_sfncttpass  ;
  size i_sfcttpasspt ;
  size i_sfcttpasseta;
  size i_sfnfjcand  ;
  size i_sffjcandpt ;
  size i_sffjcandeta;
  size i_sfntopsdpass  ;
  size i_sftopsdpasspt ;
  size i_sftopsdpasseta;
  size i_sfnwsdpass  ;
  size i_sfwsdpasspt ;
  size i_sfwsdpasseta;
  size i_wcandmass;
  size i_wcandpt;
  size i_wcandeta;
  size i_wpasspt;
  size i_wpasseta;
  size i_sfcttcandptdr1;
  size i_sfcttcandptdr2;
  size i_sfcttcandptdr3;
  size i_sfcttcandetadr1;
  size i_sfcttcandetadr2;
  size i_sfcttcandetadr3;
  size i_sfcttpassptdr1;
  size i_sfcttpassptdr2;
  size i_sfcttpassptdr3;
  size i_sfcttpassetadr1;
  size i_sfcttpassetadr2;
  size i_sfcttpassetadr3;
  size i_nhadronicgentops;
  size i_sfcttcandptdr6;
  size i_sfcttpassptdr6;
  size i_sfcttcandetadr6;
  size i_sfcttpassetadr6;
  size i_sfcttcandptdr6hard;
  size i_sfcttcandetadr6hard;
  size i_sfcttpassptdr6hard;
  size i_sfcttpassetadr6hard;

  void book(TreeWriterData* data) {
    // ctt
    i_sfbclose2lep     = data->add<bool>("","sfbclose2lep","O",0);
    i_sfncttcand       = data->add<unsigned int>("","sfncttcand","i",0);
    i_sfcttcandpt      = data->add<float>("","sfcttcandpt","F",0);
    i_sfcttcandeta     = data->add<float>("","sfcttcandeta","F",0);
    i_sfcttcandallsubjetmass       = data->add<float>("","sfcttcandallsubjetmass","F",0);
    i_sfcttcandcmstoptagmass = data->add<float>("","sfcttcandcmstoptagmass","F",0);
    i_sfcttcandnsubjets = data->add<int>("","sfcttcandnsubjets","I",0);
    i_sfcttcandminmass = data->add<float>("","sfcttcandminmass","F",0);
    i_sfncttpass       = data->add<unsigned int>("","sfncttpass","i",0);
    i_sfcttpasspt      = data->add<float>("","sfcttpasspt","F",0);
    i_sfcttpasseta     = data->add<float>("","sfcttpasseta","F",0);
    i_sfnfjcand       = data->add<unsigned int>("","sfnfjcand","i",0);
    i_sffjcandpt      = data->add<float>("","sffjcandpt","F",0);
    i_sffjcandeta     = data->add<float>("","sffjcandeta","F",0);
    i_sfntopsdpass       = data->add<unsigned int>("","sfntopsdpass","i",0);
    i_sftopsdpasspt      = data->add<float>("","sftopsdpasspt","F",0);
    i_sftopsdpasseta     = data->add<float>("","sftopsdpasseta","F",0);
    i_sfnwsdpass       = data->add<unsigned int>("","sfnwsdpass","i",0);
    i_sfwsdpasspt      = data->add<float>("","sfwsdpasspt","F",0);
    i_sfwsdpasseta     = data->add<float>("","sfwsdpasseta","F",0);
    i_wcandmass = data->addMulti<float>("","wcandmass",0);
    i_wcandpt   = data->addMulti<float>("","wcandpt",0);
    i_wcandeta  = data->addMulti<float>("","wcandeta",0);
    i_wpasspt   = data->addMulti<float>("","wpasspt",0);
    i_wpasseta  = data->addMulti<float>("","wpasseta",0);
    i_sfcttcandptdr1 = data->addMulti<float>("","sfcttcandptdr1",0);
    i_sfcttcandptdr2= data->addMulti<float>("","sfcttcandptdr2",0);
    i_sfcttcandptdr3= data->addMulti<float>("","sfcttcandptdr3",0);
    i_sfcttcandetadr1= data->addMulti<float>("","sfcttcandetadr1",0);
    i_sfcttcandetadr2= data->addMulti<float>("","sfcttcandetadr2",0);
    i_sfcttcandetadr3= data->addMulti<float>("","sfcttcandetadr3",0);
    i_sfcttpassptdr1= data->addMulti<float>("","sfcttpassptdr1",0);
    i_sfcttpassptdr2= data->addMulti<float>("","sfcttpassptdr2",0);
    i_sfcttpassptdr3= data->addMulti<float>("","sfcttpassptdr3",0);
    i_sfcttpassetadr1= data->addMulti<float>("","sfcttpassetadr1",0);
    i_sfcttpassetadr2= data->addMulti<float>("","sfcttpassetadr2",0);
    i_sfcttpassetadr3= data->addMulti<float>("","sfcttpassetadr3",0);
    i_sfcttcandptdr6hard = data->addMulti<float>("","sfcttcandptdr6hard",0);
    i_sfcttcandetadr6hard = data->addMulti<float>("","sfcttcandetadr6hard",0);
    i_sfcttpassptdr6hard = data->addMulti<float>("","sfcttpassptdr6hard",0);
    i_sfcttpassetadr6hard =data->addMulti<float>("","sfcttpassetadr6hard",0);
    i_nhadronicgentops = data->add<int>("","nhadronicgentops","i",0);
    i_sfcttcandptdr6 = data->addMulti<float>("","sfcttcandptdr6",0);
    i_sfcttcandetadr6= data->addMulti<float>("","sfcttcandetadr6",0);
    i_sfcttpassptdr6= data->addMulti<float>("","sfcttpassptdr6",0);
    i_sfcttpassetadr6= data->addMulti<float>("","sfcttpassetadr6",0);
  }

  bool passSoftDropTaggerFJ(const FatJetF* fj,float minMass,float maxMass) {
    return ( (fj->fjSoftDropMass() > minMass) && (fj->fjSoftDropMass() < maxMass) && fabs(fj->p4().eta())<=2.4);
  }

  bool passPrunedTaggerFJ(const FatJetF* fj,float minMass,float maxMass) {
    return ( (fj->fjPrunedMass() > minMass) && (fj->fjPrunedMass() < maxMass) && fabs(fj->p4().eta())<=2.4 );
  }

  // fill HPTT (CTT) branches
  void fillTopTagInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, vector<RecoJetF*> jets) {

    bool sfbclose2lep_ = false;
    unsigned int sfncttcand_      = 0;
    float sfcttcandpt_            = -9.;
    float sfcttcandeta_           = -9.;
    float sfcttcandallsubjetmass_ = -9.;
    float sfcttcandcmstoptagmass_ = -9.;
    int sfcttcandnsubjets_        = -1;
    float sfcttcandminmass_       = -9.;

    unsigned int sfncttpass_ = 0;
    float sfcttpasspt_       = -9.;
    float sfcttpasseta_      = -9.;

    unsigned int sfnfjcand_ = 0;
    float sffjcandpt_       = -9.;
    float sffjcandeta_      = -9.;

    unsigned int sfntopsdpass_ = 0;
    float sftopsdpasspt_       = -9.;
    float sftopsdpasseta_      = -9.;

    unsigned int sfnwsdpass_ = 0;
    float sfwsdpasspt_       = -9.;
    float sfwsdpasseta_      = -9.;

    int nhadronicgentops_ = 0;

    // ctt selection efficiency
    // strategy: for each gen top, loop thru reco cttTops, match one or multi with dR cone, record pT spectra for efficiency
    if(ana->isMC()) {

      //indices of hardest cands for each matching cone size
      // 1,2,3 are placeholders, 6 is default dr=0.6
      unsigned int indxctt = 99;
      float maxcttpt_ = -1.;
      unsigned int indxctt1 = 99;
      float maxcttpt1_ = -1.;
      unsigned int indxctt2 = 99;
      float maxcttpt2_ = -1.;
      unsigned int indxctt3 = 99;
      float maxcttpt3_ = -1.;
      unsigned int indxcttpass1 = 99;
      float maxcttpasspt1_ = -1.;
      unsigned int indxcttpass2 = 99;
      float maxcttpasspt2_ = -1.;
      unsigned int indxcttpass3 = 99;
      float maxcttpasspt3_ = -1.;

      // to be sorted for duplicates later
      vector<float> canddr1pt, canddr1eta, canddr2pt, canddr2eta, canddr3pt, canddr3eta;
      vector<float> passdr1pt, passdr1eta, passdr2pt, passdr2eta, passdr3pt, passdr3eta;
      vector<float> canddr6pt, canddr6eta, passdr6pt, passdr6eta;

      for(auto* p : ana->genParts) {
        if ((abs(p->pdgId()) == ParticleInfo::p_t)
            && ParticleInfo::isGenTopHadronic(p)
            && ParticleInfo::isLastInChain(p)) {
          nhadronicgentops_++;
          unsigned int countctttags = 0;
          for (auto* ctt : ana->cttTops) {
            float cttpt_ = ctt->p4().pt();

            // for variable dr cones, want matching efficiency (cand = passed ctt selection; pass = then passed dR matching)
            if (cfgSet::isSelTaggedTop(*ctt)) {

              // cone 1 (dR 0.2)
              if (cttpt_>maxcttpt1_) { indxctt1 = countctttags; maxcttpt1_ = cttpt_; } // index of hardest cand
              canddr1pt.push_back(ctt->pt()); canddr1eta.push_back(ctt->p4().eta());
              if(PhysicsUtilities::deltaR(*ctt, *p) < 0.2) {
                if (cttpt_>maxcttpasspt1_) { indxcttpass1 = countctttags; maxcttpasspt1_ = cttpt_; } // index of hardest pass
                passdr1pt.push_back(ctt->pt()); passdr1eta.push_back(ctt->p4().eta());
              }

              // cone 2 (dR 0.5)
              if (cttpt_>maxcttpt2_) { indxctt2 = countctttags; maxcttpt2_ = cttpt_; } // index of hardest cand
              canddr2pt.push_back(ctt->pt()); canddr2eta.push_back(ctt->p4().eta());
              if(PhysicsUtilities::deltaR(*ctt, *p) < 0.5) {
                if (cttpt_>maxcttpasspt2_) { indxcttpass2 = countctttags; maxcttpasspt2_ = cttpt_; } // index of hardest pass
                passdr2pt.push_back(ctt->pt()); passdr2eta.push_back(ctt->p4().eta());
              }

              // cone 3 (dR 0.8)
              if (cttpt_>maxcttpt3_) { indxctt3 = countctttags; maxcttpt3_ = cttpt_; } // index of hardest cand
              canddr3pt.push_back(ctt->pt()); canddr3eta.push_back(ctt->p4().eta());
              if(PhysicsUtilities::deltaR(*ctt, *p) < 0.8) {
                if (cttpt_>maxcttpasspt3_) { indxcttpass3 = countctttags; maxcttpasspt3_ = cttpt_; } // index of hardest pass
                passdr3pt.push_back(ctt->pt()); passdr3eta.push_back(ctt->p4().eta());
              }

            }//if ctt selected

            // default cone (dR 0.6)
            // not matching eff here. rather want eff of ctt selection after matching.
            // multiple jets per event!
            if(PhysicsUtilities::deltaR(*ctt, *p) < 0.6) {
              if (cttpt_>maxcttpt_) { indxctt = countctttags; maxcttpt_ = cttpt_; } // index of hardest 0.6 matched ctt jet
              canddr6pt.push_back(ctt->p4().pt()); canddr6eta.push_back(ctt->p4().eta());
              if(cfgSet::isSelTaggedTop(*ctt)) {passdr6pt.push_back(ctt->p4().pt()); passdr6eta.push_back(ctt->p4().eta());}
            }

            ++countctttags;
          }//for ctt tops
        }//if hadronic top
      }//for gen particles

      // vectors to be de-duplicate
      vector<vector<float> > v_undup;
      v_undup.push_back(canddr1pt); v_undup.push_back(canddr1eta);
      v_undup.push_back(canddr2pt); v_undup.push_back(canddr2eta);
      v_undup.push_back(canddr3pt); v_undup.push_back(canddr3eta);
      v_undup.push_back(passdr1pt); v_undup.push_back(passdr1eta);
      v_undup.push_back(passdr2pt); v_undup.push_back(passdr2eta);
      v_undup.push_back(passdr3pt); v_undup.push_back(passdr3eta);
      v_undup.push_back(canddr6pt); v_undup.push_back(canddr6eta);
      v_undup.push_back(passdr6pt); v_undup.push_back(passdr6eta);

      for(int ii = 0; ii < v_undup.size(); ii++) {
        set<float> s_undup(v_undup[ii].begin(),v_undup[ii].end());
        v_undup[ii].assign(s_undup.begin(),s_undup.end());
      }

      // push dr cone variables
/*
      for(unsigned int i = 0; i < v_undup[0].size(); ++i) {data->fillMulti<float>(i_sfcttcandptdr1,v_undup[0][i]); }
      for(unsigned int i = 0; i < v_undup[1].size(); ++i) {data->fillMulti<float>(i_sfcttcandetadr1,v_undup[1][i]); }
      for(unsigned int i = 0; i < v_undup[2].size(); ++i) {data->fillMulti<float>(i_sfcttcandptdr2,v_undup[2][i]); }
      for(unsigned int i = 0; i < v_undup[3].size(); ++i) {data->fillMulti<float>(i_sfcttcandetadr2,v_undup[3][i]); }
      for(unsigned int i = 0; i < v_undup[4].size(); ++i) {data->fillMulti<float>(i_sfcttcandptdr3,v_undup[4][i]); }
      for(unsigned int i = 0; i < v_undup[5].size(); ++i) {data->fillMulti<float>(i_sfcttcandetadr3,v_undup[5][i]); }

      for(unsigned int i = 0; i < v_undup[6].size(); ++i) {data->fillMulti<float>(i_sfcttpassptdr1,v_undup[6][i]); }
      for(unsigned int i = 0; i < v_undup[7].size(); ++i) {data->fillMulti<float>(i_sfcttpassetadr1,v_undup[7][i]); }
      for(unsigned int i = 0; i < v_undup[8].size(); ++i) {data->fillMulti<float>(i_sfcttpassptdr2,v_undup[8][i]); }
      for(unsigned int i = 0; i < v_undup[9].size(); ++i) {data->fillMulti<float>(i_sfcttpassetadr2,v_undup[9][i]); }
      for(unsigned int i = 0; i < v_undup[10].size(); ++i) {data->fillMulti<float>(i_sfcttpassptdr3,v_undup[10][i]); }
      for(unsigned int i = 0; i < v_undup[11].size(); ++i) {data->fillMulti<float>(i_sfcttpassetadr3,v_undup[11][i]); }
*/

      for(unsigned int i = 0; i < v_undup[12].size(); ++i) {data->fillMulti<float>(i_sfcttcandptdr6,v_undup[12][i]); }
      for(unsigned int i = 0; i < v_undup[13].size(); ++i) {data->fillMulti<float>(i_sfcttcandetadr6,v_undup[13][i]); }
      for(unsigned int i = 0; i < v_undup[14].size(); ++i) {data->fillMulti<float>(i_sfcttpassptdr6,v_undup[14][i]); }
      for(unsigned int i = 0; i < v_undup[15].size(); ++i) {data->fillMulti<float>(i_sfcttpassetadr6,v_undup[15][i]); }

      // hardest gen matched to 0.6
      if (indxctt<99) {
        data->fillMulti<float>(i_sfcttcandptdr6hard        , ana->cttTops[indxctt]->p4().pt());
        data->fillMulti<float>(i_sfcttcandetadr6hard       , ana->cttTops[indxctt]->p4().eta());
        if(cfgSet::isSelTaggedTop(*ana->cttTops[indxctt])) {
          data->fillMulti<float>(i_sfcttpassptdr6hard      , ana->cttTops[indxctt]->p4().pt());
          data->fillMulti<float>(i_sfcttpassetadr6hard      , ana->cttTops[indxctt]->p4().eta());
        }
      }

      // hardest gen matched other cones
      // cone 1
      if (indxctt1<99) {
        auto *ctt = ana->cttTops[indxctt1];
        data->fillMulti<float>(i_sfcttcandptdr1        , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttcandetadr1       , ctt->p4().eta());
      }
      if (indxcttpass1<99) {
        auto *ctt = ana->cttTops[indxcttpass1];
        data->fillMulti<float>(i_sfcttpassptdr1      , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttpassetadr1      , ctt->p4().eta());
      }
      // cone 2
      if (indxctt2<99) {
        auto *ctt = ana->cttTops[indxctt2];
        data->fillMulti<float>(i_sfcttcandptdr2        , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttcandetadr2       , ctt->p4().eta());
      }
      if (indxcttpass2<99) {
        auto *ctt = ana->cttTops[indxcttpass2];
        data->fillMulti<float>(i_sfcttpassptdr2      , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttpassetadr2      , ctt->p4().eta());
      }
      // cone 3
      if (indxctt3<99) {
        auto *ctt = ana->cttTops[indxctt3];
        data->fillMulti<float>(i_sfcttcandptdr3        , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttcandetadr3       , ctt->p4().eta());
      }
      if (indxcttpass3<99) {
        auto *ctt = ana->cttTops[indxcttpass3];
        data->fillMulti<float>(i_sfcttpassptdr3      , ctt->p4().pt());
        data->fillMulti<float>(i_sfcttpassetadr3      , ctt->p4().eta());
      }

    }//if MC

    vector<LorentzVector> csvmjets;
    for(auto* j : jets) {

      if(j->csv() > defaults::CSV_MEDIUM) {
	LorentzVector tmpVecCSVMJets; tmpVecCSVMJets = j->p4();
	csvmjets.push_back(tmpVecCSVMJets); }
    }


    if (ana->nSelLeptons == 0) {

      // do it for ctt fatjets
      unsigned int countctttags = 0;
      unsigned int indxctt = 99;
      float maxcttpt_ = -1.;
      for (auto* ctt : ana->cttTops) {

	float cttpt_ = ctt->p4().pt();
	if (cttpt_>maxcttpt_) { indxctt = countctttags; maxcttpt_ = cttpt_;}

	++countctttags;
      }

      if (indxctt<99) {
	sfncttcand_             = 1;
	sfcttcandpt_            = ana->cttTops[indxctt]->p4().pt();
	sfcttcandeta_           = ana->cttTops[indxctt]->p4().eta();
	sfcttcandallsubjetmass_ = ana->cttTops[indxctt]->topAllSubjetMass();
	sfcttcandcmstoptagmass_ = ana->cttTops[indxctt]->topCmsTopTagMass();
	sfcttcandnsubjets_      = ana->cttTops[indxctt]->topNsubJets();
	sfcttcandminmass_       = ana->cttTops[indxctt]->topMinMass();

	if (cfgSet::isSelTaggedTop(*ana->cttTops[indxctt])) {
	  sfncttpass_   = 1;
	  sfcttpasspt_  = ana->cttTops[indxctt]->p4().pt();
	  sfcttpasseta_ = ana->cttTops[indxctt]->p4().eta(); }
      }


      // do it for ak8 fatjets
      unsigned int countfjtags = 0;
      unsigned int indxfj = 99;
      float maxfjpt_ = -1.;
      for (auto* fj : ana->fatJets) {

	float fjpt_ = fj->p4().pt();
	if (fjpt_>maxfjpt_) { indxfj = countfjtags; maxfjpt_ = fjpt_;}

	++countfjtags;
      }

      if (indxfj<99) {
	sfnfjcand_   = 1;
	sffjcandpt_  = ana->fatJets[indxfj]->p4().pt();
	sffjcandeta_ = ana->fatJets[indxfj]->p4().eta();

	if (passSoftDropTaggerFJ(ana->fatJets[indxfj],120.,220.)) {
	  sfntopsdpass_   = 1;
	  sftopsdpasspt_  = ana->fatJets[indxfj]->p4().pt();
	  sftopsdpasseta_ = ana->fatJets[indxfj]->p4().eta(); }

	if (passSoftDropTaggerFJ(ana->fatJets[indxfj],60.,100000.)) {
	  sfnwsdpass_   = 1;
	  sfwsdpasspt_  = ana->fatJets[indxfj]->p4().pt();
	  sfwsdpasseta_ = ana->fatJets[indxfj]->p4().eta(); }

      }


    } // end of zero leptons


    if (ana->nSelLeptons > 0) {
      MomentumF lepf(ana->selectedLeptons.at(0)->p4());
      MomentumF* lep = &lepf;
      for (unsigned int i0=0; i0<csvmjets.size(); ++i0) {
	float drlepbjet_ = PhysicsUtilities::deltaR(*lep,csvmjets[i0]);
	if (drlepbjet_<(3.14/2.)) { sfbclose2lep_ = true; }
      }

      // do it for ctt tags
      unsigned int countctttags = 0;
      unsigned int indxctt = 99;
      float maxcttpt_ = -1.;
      for (auto* ctt : ana->cttTops) {

	float drlepctt_ = PhysicsUtilities::deltaR(*lep,ctt->p4());

	if (drlepctt_>=(3.14/2.)) {
	  float cttpt_ = ctt->p4().pt();
	  if (cttpt_>maxcttpt_) { indxctt = countctttags; maxcttpt_ = cttpt_;}
	}

	++countctttags;
      }

      if (indxctt<99) {
	sfncttcand_             = 1;
	sfcttcandpt_            = ana->cttTops[indxctt]->p4().pt();
	sfcttcandeta_           = ana->cttTops[indxctt]->p4().eta();
	sfcttcandallsubjetmass_ = ana->cttTops[indxctt]->topAllSubjetMass();
	sfcttcandcmstoptagmass_ = ana->cttTops[indxctt]->topCmsTopTagMass();
	sfcttcandnsubjets_      = ana->cttTops[indxctt]->topNsubJets();
	sfcttcandminmass_       = ana->cttTops[indxctt]->topMinMass();

	if (cfgSet::isSelTaggedTop(*ana->cttTops[indxctt])) {
	  sfncttpass_     = 1;
	  sfcttpasspt_  = ana->cttTops[indxctt]->p4().pt();
	  sfcttpasseta_ = ana->cttTops[indxctt]->p4().eta(); }

      }

      // do it for ak8 fatjets
      unsigned int countfjtags = 0;
      unsigned int indxfj = 99;
      float maxfjpt_ = -1.;
      for (auto* fj : ana->fatJets) {

	float drlepfj_ = PhysicsUtilities::deltaR(*lep,fj->p4());

	if (drlepfj_>=(3.14/2.)) {
	  float fjpt_ = fj->p4().pt();
	  if (fjpt_>maxfjpt_) { indxfj = countfjtags; maxfjpt_ = fjpt_;}
	}

	++countfjtags;
      }

      if (indxfj<99) {
	sfnfjcand_   = 1;
	sffjcandpt_  = ana->fatJets[indxfj]->p4().pt();
	sffjcandeta_ = ana->fatJets[indxfj]->p4().eta();

	if (passSoftDropTaggerFJ(ana->fatJets[indxfj],120.,220.)) {
	  sfntopsdpass_   = 1;
	  sftopsdpasspt_  = ana->fatJets[indxfj]->p4().pt();
	  sftopsdpasseta_ = ana->fatJets[indxfj]->p4().eta(); }

	if (passSoftDropTaggerFJ(ana->fatJets[indxfj],60.,100000.)) {
	  sfnwsdpass_   = 1;
	  sfwsdpasspt_  = ana->fatJets[indxfj]->p4().pt();
	  sfwsdpasseta_ = ana->fatJets[indxfj]->p4().eta(); }

      }


    } // end of at least one lepton


    data->fill<bool>(i_sfbclose2lep           , sfbclose2lep_);
    data->fill<unsigned int>(i_sfncttcand     , sfncttcand_);
    data->fill<float>(i_sfcttcandpt           , sfcttcandpt_);
    data->fill<float>(i_sfcttcandeta          , sfcttcandeta_);
    data->fill<float>(i_sfcttcandallsubjetmass, sfcttcandallsubjetmass_);
    data->fill<float>(i_sfcttcandcmstoptagmass, sfcttcandcmstoptagmass_);
    data->fill<int>(i_sfcttcandnsubjets       , sfcttcandnsubjets_);
    data->fill<float>(i_sfcttcandminmass      , sfcttcandminmass_);
    data->fill<unsigned int>(i_sfncttpass     , sfncttpass_);
    data->fill<float>(i_sfcttpasspt           , sfcttpasspt_);
    data->fill<float>(i_sfcttpasseta          , sfcttpasseta_);
    data->fill<unsigned int>(i_sfnfjcand     , sfnfjcand_);
    data->fill<float>(i_sffjcandpt           , sffjcandpt_);
    data->fill<float>(i_sffjcandeta          , sffjcandeta_);
    data->fill<unsigned int>(i_sfntopsdpass  , sfntopsdpass_);
    data->fill<float>(i_sftopsdpasspt        , sftopsdpasspt_);
    data->fill<float>(i_sftopsdpasseta       , sftopsdpasseta_);
    data->fill<unsigned int>(i_sfnwsdpass    , sfnwsdpass_);
    data->fill<float>(i_sfwsdpasspt          , sfwsdpasspt_);
    data->fill<float>(i_sfwsdpasseta         , sfwsdpasseta_);
    data->fill<int  >(i_nhadronicgentops      , nhadronicgentops_);

    for(auto* fj : ana->fatJets) {

      data->fillMulti<float>(i_wcandmass, fj->fjSoftDropMass());
      data->fillMulti<float>(i_wcandpt  , fj->p4().pt());
      data->fillMulti<float>(i_wcandeta , fj->p4().eta());

      if (passSoftDropTaggerFJ(fj,60.,100000.)) {

        data->fillMulti<float>(i_wpasspt  , fj->p4().pt());
        data->fillMulti<float>(i_wpasseta , fj->p4().eta());
      }

    }

  } // end of fillTopTagInfo

};//cttTreeFiller

#endif
