#ifndef EXTRAVARIABLES_HH
#define EXTRAVARIABLES_HH

#include "AnalysisTools/DataFormats/interface/CMSTop.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

using namespace ucsbsusy;

struct ExtraVarsFiller {

  ExtraVarsFiller() {}

  // Test vars -- Add here first for testing, and move to other categories or BasicVarsFiller later!
  size i_test_genwpt;

  // Syst. studies
  size i_systweights;
  size i_wpolWeightUp;
  size i_wpolWeightDn;
  size i_costhetastar;
  size i_lp;

  // JetMET extra
  size i_njets30   ;
  size i_nbjets30  ;
  size i_ht30      ;
  size i_j1eta     ;
  size i_j2eta     ;
  size i_j3pt      ;
  size i_j3eta     ;
  size i_csvj1eta  ;
  size i_csvj2pt   ;
  size i_csvj2eta  ;
  size i_csv1      ;
  size i_csv2      ;
  size i_dphicsv1met;
  size i_dphicsv2met;
  size i_dphicsv12met;
  size i_meff;
  size i_dphitopmet;
  size i_toppt;
  size i_httwoleadfatjet;

  size i_j2lpt;
  size i_dphicsv1csv2;
  size i_drcsv1csv2;
  size i_dphij2lmet;
  size i_dphij1lj2l;
  size i_detaj1lj2l;
  size i_drj1lj2l;

  // Lepton extra
  size i_absdphilepmet;
  size i_absdphilepw;
  size i_htalonglep;
  size i_annulus   ;
  size i_leptonmatchtrigmu;
  size i_leptonmatchtrige;
  size i_lepton2pt ;
  size i_lepton2eta;
  size i_lepton2pdgid;
  size i_chhpt     ;
  size i_chheta    ;
  size i_chhdz     ;
  size i_chhmt     ;
  size i_chhdphimet;
  size i_chhtaudisc;
  size i_mupt      ;
  size i_mueta     ;
  size i_muhtalong ;
  size i_muannulus ;
  size i_elept     ;
  size i_eleeta    ;
  size i_elehtalong;
  size i_eleannulus;
  size i_trackpt   ;
  size i_tracketa  ;
  size i_trackhtalong;
  size i_taupt     ;
  size i_taueta    ;
  size i_tauhtalong;

  // gen-level
  size i_distoppt ;
  size i_ditoppt  ;
  size i_ngenjets ;
  size i_ngenbjets;
  size i_gentop1pt;
  size i_gentop2pt;
  size i_genb1pt;
  size i_genb2pt;
  size i_genb1eta;
  size i_genb2eta;
  size i_dphigenb1genb2;
  size i_drgenb1genb2;
  size i_ngenmu;
  size i_ngenel;
  size i_ngentau;
  size i_ngenlep;
  size i_genlepq ;
  size i_genwpt;

  void bookTest(TreeWriterData* data){
    i_test_genwpt         = data->add<float>("","test_genwpt","F",-1);
  }

  void bookSyst(TreeWriterData* data){
    i_systweights       = data->addMulti<float>("","systweights",0);
    i_wpolWeightUp      = data->add<float>("","wpolWeightUp","F",1);
    i_wpolWeightDn      = data->add<float>("","wpolWeightDn","F",1);
    i_costhetastar      = data->add<float>("","costhetastar","F",-1);
    i_lp                = data->add<float>("","lp","F",-9);
  }

  void bookJetMET(TreeWriterData* data){
    i_njets30        = data->add<int>("","njets30","I",0);
    i_nbjets30       = data->add<int>("","nbjets30","I",0);
    i_ht30           = data->add<float>("","ht30","F",0);
    i_j1eta          = data->add<float>("","j1eta","F",0);
    i_j2eta          = data->add<float>("","j2eta","F",0);
    i_j3pt           = data->add<float>("","j3pt","F",0);
    i_j3eta          = data->add<float>("","j3eta","F",0);
    i_csvj1eta       = data->add<float>("","csvj1eta","F",0);
    i_csvj2pt        = data->add<float>("","csvj2pt","F",0);
    i_csvj2eta       = data->add<float>("","csvj2eta","F",0);
    i_csv1           = data->add<float>("","csv1","F",0);
    i_csv2           = data->add<float>("","csv2","F",0);
    i_dphicsv1met    = data->add<float>("","dphicsv1met","F",0);
    i_dphicsv2met    = data->add<float>("","dphicsv2met","F",0);
    i_dphicsv12met   = data->add<float>("","dphicsv12met","F",0);
    i_meff           = data->add<float>("","meff","F",0);
    i_dphitopmet     = data->add<float>("","dphitopmet","F",0);
    i_toppt          = data->add<float>("","toppt","F",0);
    i_httwoleadfatjet = data->add<float>("","httwoleadfatjet","F",0);

    i_j2lpt          = data->add<float>("","j2lpt","F",0);
    i_dphij2lmet     = data->add<float>("","dphij2lmet","F",0);
    i_dphij1lj2l     = data->add<float>("","dphij1lj2l","F",0);
    i_detaj1lj2l     = data->add<float>("","detaj1lj2l","F",0);
    i_drj1lj2l       = data->add<float>("","drj1lj2l","F",0);
    i_dphicsv1csv2   = data->add<float>("","dphicsv1csv2","F",0);
    i_drcsv1csv2     = data->add<float>("","drcsv1csv2","F",0);
  }

  void bookLepton(TreeWriterData* data){
    i_absdphilepmet  = data->add<float>("","absdphilepmet","F",-1);
    i_absdphilepw    = data->add<float>("","absdphilepw","F",0);
    i_htalonglep     = data->add<float>("","htalonglep","F",0);
    i_annulus        = data->add<float>("","annulus","F",0);
    i_leptonmatchtrigmu  = data->add<bool>("","leptonmatchtrigmu","O",0);
    i_leptonmatchtrige   = data->add<bool>("","leptonmatchtrige","O",0);
    i_lepton2pt      = data->add<float>("","lepton2pt","F",0);
    i_lepton2eta     = data->add<float>("","lepton2eta","F",0);
    i_lepton2pdgid   = data->add<int>("","lepton2pdgid","I",0);
    i_chhpt          = data->addMulti<float>("","chhpt",0);
    i_chheta         = data->addMulti<float>("","chheta",0);
    i_chhdz          = data->addMulti<float>("","chhdz",0);
    i_chhmt          = data->addMulti<float>("","chhmt",0);
    i_chhdphimet     = data->addMulti<float>("","chhdphimet",0);
    i_chhtaudisc     = data->addMulti<float>("","chhtaudisc",0);
    i_mupt           = data->addMulti<float>("","mupt",0);
    i_mueta          = data->addMulti<float>("","mueta",0);
    i_muhtalong      = data->addMulti<float>("","muhtalong",0);
    i_muannulus      = data->addMulti<float>("","muannulus",0);
    i_elept          = data->addMulti<float>("","elept",0);
    i_eleeta         = data->addMulti<float>("","eleeta",0);
    i_elehtalong     = data->addMulti<float>("","elehtalong",0);
    i_eleannulus     = data->addMulti<float>("","eleannulus",0);
    i_trackpt        = data->addMulti<float>("","trackpt",0);
    i_tracketa       = data->addMulti<float>("","tracketa",0);
    i_trackhtalong   = data->addMulti<float>("","trackhtalong",0);
    i_taupt          = data->addMulti<float>("","taupt",0);
    i_taueta         = data->addMulti<float>("","taueta",0);
    i_tauhtalong     = data->addMulti<float>("","tauhtalong",0);
  }

  void bookGen(TreeWriterData* data){
    i_distoppt       = data->add<float>("","distoppt","F",0);
    i_ditoppt        = data->add<float>("","ditoppt","F",-1.);
    i_ngenjets       = data->add<int>("","ngenjets","I",0);
    i_ngenbjets      = data->add<int>("","ngenbjets","I",0);
    i_gentop1pt      = data->add<float>("","gentoppt1","F",0);
    i_gentop2pt      = data->add<float>("","gentoppt2","F",0);
    i_genb1pt        = data->add<float>("","genb1pt","F",0);
    i_genb2pt        = data->add<float>("","genb2pt","F",0);
    i_genb1eta       = data->add<float>("","genb1eta","F",0);
    i_genb2eta       = data->add<float>("","genb2eta","F",0);
    i_dphigenb1genb2 = data->add<float>("","dphigenb1genb2","F",0);
    i_drgenb1genb2   = data->add<float>("","drgenb1genb2","F",0);
    i_genlepq        = data->add<float>("","genlepq","F",0);
    i_ngenmu         = data->add<int>("","ngenmu","I",0);
    i_ngenel         = data->add<int>("","ngenel","I",0);
    i_ngentau        = data->add<int>("","ngentau","I",0);
    i_ngenlep        = data->add<int>("","ngenlep","I",0);
    i_genwpt         = data->add<float>("","genwpt","F",-1);
  }


  void fillTestVars(TreeWriterData* data, const BaseTreeAnalyzer* ana){
    if (ana->isMC()){
      for (const auto *p : ana->genParts){
        if (ParticleInfo::isA(ParticleInfo::p_Wplus, p)){
          data->fill<float>(i_test_genwpt, p->pt());
          break;
        }
      }
    }
  }

  void fillSystInfo(TreeWriterData* data, const BaseTreeAnalyzer* ana){
    for(auto wgt : *ana->evtInfoReader.systweights) {
      data->fillMulti<float>(i_systweights, wgt/ana->evtInfoReader.lhecentralweight);
    }
    data->fill<float>(i_wpolWeightUp, ana->wpolCorrections.getWpolWeightUp());
    data->fill<float>(i_wpolWeightDn, ana->wpolCorrections.getWpolWeightDn());
    data->fill<float>(i_costhetastar, ana->wpolCorrections.getCosThetaStar());

    float lp_ = -9.;
    if (ana->selectedLepton){
      const auto *lep = ana->selectedLepton;
      auto WP4 = lep->p4() + ana->met->p4();
      lp_ = ( lep->px()*WP4.px() + lep->py()*WP4.py() ) / ( WP4.pt()*WP4.pt() );
    }
    data->fill<float>(i_lp,lp_);
  }

  void fillJetMETInfo(TreeWriterData* data, const BaseTreeAnalyzer* ana, bool useModifiedMET = false, MomentumF* metn = 0){

    const auto &jets = ana->jets;
    const MomentumF *met = useModifiedMET ? metn : ana->met;

    int njets30 = 0, nbjets30 = 0;
    for(const auto *j : jets) {
      if(j->pt() > 30.0) njets30++;
      if(j->csv() > defaults::CSV_MEDIUM && j->pt() > 30.0) nbjets30++;
    }
    data->fill<int>(i_njets30,  njets30);
    data->fill<int>(i_nbjets30, nbjets30);
    data->fill<float>(i_ht30, JetKinematics::ht(jets, 30.0, 2.4));
    data->fill<float>(i_meff, JetKinematics::ht(jets, 20.0, 2.4)+(met->pt()));
    if(jets.size() > 0) {
      data->fill<float>(i_j1eta, jets[0]->eta());
    }
    if(jets.size() > 1) {
      data->fill<float>(i_j2eta, jets[1]->eta());
    }
    if(jets.size() > 2) {
      data->fill<float>(i_j3pt, jets[2]->pt());
      data->fill<float>(i_j3eta, jets[2]->eta());
    }

    vector<RecoJetF*> jetsCSVranked(jets);
    cfgSet::sortByCSV(jetsCSVranked);

    double dphicsv1met = 0, dphicsv2met = 0, dphicsv12met = 0;
    if(jetsCSVranked.size() > 0) {
      dphicsv1met = fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[0], *met));
      dphicsv12met = dphicsv1met;
      data->fill<float>(i_dphicsv1met, dphicsv1met);
      data->fill<float>(i_csvj1eta, jetsCSVranked[0]->eta());
      data->fill<float>(i_csv1, jetsCSVranked[0]->csv());
      if(jetsCSVranked.size() > 1){
        dphicsv2met = fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[1], *met));
        dphicsv12met = min(dphicsv1met, dphicsv2met);
        data->fill<float>(i_dphicsv2met, dphicsv2met);
        data->fill<float>(i_csvj2pt, jetsCSVranked[1]->pt());
        data->fill<float>(i_csvj2eta, jetsCSVranked[1]->eta());
        data->fill<float>(i_csv2, jetsCSVranked[1]->csv());
        data->fill<float>(i_dphicsv1csv2,fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[0], *jetsCSVranked[1])));
        data->fill<float>(i_drcsv1csv2,  fabs(PhysicsUtilities::deltaR(*jetsCSVranked[0], *jetsCSVranked[1])));

      }
    }
    data->fill<float>(i_dphicsv12met,dphicsv12met);

    if (ana->nSelCTTTops > 0) {
      data->fill<float>(i_toppt,ana->selectedCTTTops[0]->pt());
      data->fill<float>(i_dphitopmet,fabs(PhysicsUtilities::deltaPhi(*ana->selectedCTTTops[0], *ana->met)));
    }
    if (ana->cttTops.size() > 1) {
      data->fill<float>(i_httwoleadfatjet,(ana->cttTops[0]->pt()+ana->cttTops[1]->pt()));
    }

    if (ana->isrJets.size() > 1) {
      data->fill<float>(i_j2lpt,ana->isrJets[1]->pt());
      data->fill<float>(i_dphij2lmet,fabs(PhysicsUtilities::deltaPhi(*ana->isrJets[1],*met)));
      data->fill<float>(i_dphij1lj2l,fabs(PhysicsUtilities::deltaPhi(*ana->isrJets[0],*ana->isrJets[1])));
      data->fill<float>(i_detaj1lj2l,fabs(PhysicsUtilities::deltaEta(*ana->isrJets[0],*ana->isrJets[1])));
      data->fill<float>(i_drj1lj2l,fabs(PhysicsUtilities::deltaR(*ana->isrJets[0],*ana->isrJets[1])));
    }

  }

  void fillLeptonInfo(TreeWriterData* data, const BaseTreeAnalyzer* ana){

    if (ana->selectedLepton){
      const auto * lep = ana->selectedLepton;
      auto WP4 = lep->p4() + ana->met->p4();
      data->fill<float>(i_absdphilepmet, fabs(PhysicsUtilities::deltaPhi(*lep, *ana->met)));
      data->fill<float>(i_absdphilepw,   fabs(PhysicsUtilities::deltaPhi(*lep, WP4)) );

      bool cleanHt = false;
      float htalonglep = 0;
      for(const auto* jet : ana->jets){
        if (PhysicsUtilities::absDeltaPhi(*jet,*lep) < TMath::PiOver2()) {
          htalonglep += jet->pt();
          if(PhysicsUtilities::deltaR(*jet,*lep) < .4) cleanHt = true;
        }
      } // jets
      if(cleanHt) htalonglep -= lep->pt();
      data->fill<float>(i_htalonglep, htalonglep);
      data->fill<float>(i_annulus,    lep->pt() *lep->annulusactivity());

      bool matchtrigmu = false, matchtrige = false;
      for(auto* to : ana->triggerObjects) {
        if((to->filterflags() & kSingleIsoMu22) && (to->pathflags() & kHLT_IsoMu22)) {
          if(PhysicsUtilities::deltaR(*lep, *to) < 0.05) {
            matchtrigmu = true;
          }
        }
        if((to->filterflags() & kSingleEle22) && (to->pathflags() & kHLT_Ele22_eta2p1_WPLoose_Gsf)) {
          if(PhysicsUtilities::deltaR(*lep, *to) < 0.05) {
            matchtrige = true;
          }
        }
      }
      data->fill<bool >(i_leptonmatchtrigmu, matchtrigmu);
      data->fill<bool >(i_leptonmatchtrige, matchtrige);

      if(ana->nSelLeptons > 1) {
        const LeptonF * lep2 = 0;
        for(const auto * tLep : ana->selectedLeptons){
          if(tLep->index() == lep->index() && tLep->ismuon() == lep->ismuon()) continue;
          lep2 = tLep;
        }
        if(lep2==0)throw std::range_error("ZeroLeptonTreeHelper::fillEventInfo: You say you have at least two leptons but I only find one!");
        data->fill<float>(i_lepton2pt, lep2->pt());
        data->fill<float>(i_lepton2eta, lep2->eta());
        data->fill<int  >(i_lepton2pdgid, lep2->pdgid());
      }
    }

    for(auto& pfc : ana->pfcandReader.pfcands) {
      if(!pfc.ischargedhadron() || pfc.pt() < 10.0 || fabs(pfc.eta()) > 2.4) continue;
      data->fillMulti<float>(i_chhpt, pfc.pt());
      data->fillMulti<float>(i_chheta, pfc.eta());
      data->fillMulti<float>(i_chhdz, pfc.dz());
      data->fillMulti<float>(i_chhmt, pfc.mt());
      data->fillMulti<float>(i_chhdphimet, fabs(pfc.dphimet()));
      data->fillMulti<float>(i_chhtaudisc, pfc.taudisc());
    }

    if(ana->nSelLeptons > 0) {
      for(auto* l : ana->selectedLeptons) {
        bool cleanHt = false;
        float htalong = 0;
        for(const auto* jet : ana->jets) {
          if(PhysicsUtilities::absDeltaPhi(*jet,*l) < TMath::PiOver2()) {
            htalong += jet->pt();
            if(PhysicsUtilities::deltaR(*jet,*l) < 0.4) cleanHt = true;
          }
        }
        if(cleanHt) htalong -= l->pt();
        if(l->ismuon()) {
          data->fillMulti<float>(i_mupt, l->pt());
          data->fillMulti<float>(i_mueta, l->eta());
          data->fillMulti<float>(i_muhtalong, htalong);
          data->fillMulti<float>(i_muannulus, l->pt()*l->annulusactivity());
        } else {
          data->fillMulti<float>(i_elept, l->pt());
          data->fillMulti<float>(i_eleeta, l->eta());
          data->fillMulti<float>(i_elehtalong, htalong);
          data->fillMulti<float>(i_eleannulus, l->pt()*l->annulusactivity());
        }
      }
    }

    if(ana->nVetoedTracks > 0) {
      for(const auto* tr : ana->vetoedTracks) {
        data->fillMulti<float>(i_trackpt, tr->pt());
        data->fillMulti<float>(i_tracketa, tr->eta());
        bool cleanHt = false;
        float htalong = 0;
        for(const auto* jet : ana->jets) {
          if(PhysicsUtilities::absDeltaPhi(*jet,*tr) < TMath::PiOver2()) {
            htalong += jet->pt();
            if(PhysicsUtilities::deltaR(*jet,*tr) < 0.4) cleanHt = true;
          }
        }
        if(cleanHt) htalong -= tr->pt();
        data->fillMulti<float>(i_trackhtalong, htalong);
      }
    }

    if(ana->nVetoHPSTaus > 0) {
      for(const auto* tau : ana->vetoedTaus) {
        data->fillMulti<float>(i_taupt, tau->pt());
        data->fillMulti<float>(i_taueta, tau->eta());
        bool cleanHt = false;
        float htalong = 0;
        for(const auto* jet : ana->jets) {
          if(PhysicsUtilities::absDeltaPhi(*jet,*tau) < TMath::PiOver2()) {
            htalong += jet->pt();
            if(PhysicsUtilities::deltaR(*jet,*tau) < 0.4) cleanHt = true;
          }
        }
        if(cleanHt) htalong -= tau->pt();
        data->fillMulti<float>(i_tauhtalong, htalong);
      }
    }

  }

  void fillGenInfo(TreeWriterData* data, const BaseTreeAnalyzer* ana){
    if(!ana->isMC()) return;

    int ngenmu_ = 0; int ngenel_ = 0; int ngentau_ = 0; int ngenlep_ = 0; int genlepq_ = -9.;
    GenParticleF *stop1 = nullptr, *stop2 = nullptr;
    std::vector<GenParticleF*> genb, gent, genw;
    for(auto* p : ana->genParts) {
      if(ParticleInfo::isA(ParticleInfo::p_stop1, p)){
        if (!stop1) stop1 = p;
        else if (!stop2) stop2 = p;
      }
      else if (abs(p->pdgId())==5) { genb.push_back(p); }
      else if (abs(p->pdgId())==6) { gent.push_back(p); }
      else if (abs(p->pdgId())==24){ genw.push_back(p); }

      if (p->numberOfMothers()==0) continue;

      const GenParticleF *genPartMom = p->mother(0);
      if ( (abs(p->pdgId())==11) && (abs(genPartMom->pdgId())==24) ) { ++ngenel_;  ++ngenlep_; genlepq_=p->pdgId()>0 ? -1 : 1; }
      if ( (abs(p->pdgId())==13) && (abs(genPartMom->pdgId())==24) ) { ++ngenmu_;  ++ngenlep_; genlepq_=p->pdgId()>0 ? -1 : 1; }
      if ( (abs(p->pdgId())==15) && (abs(genPartMom->pdgId())==24) ) { ++ngentau_; ++ngenlep_; genlepq_=p->pdgId()>0 ? -1 : 1; }

    }

    data->fill<int>(i_ngenel,ngenel_);
    data->fill<int>(i_ngenmu,ngenmu_);
    data->fill<int>(i_ngentau,ngentau_);
    data->fill<int>(i_ngenlep,ngenlep_);
    data->fill<int>(i_genlepq,genlepq_);

    if(stop1 && stop2) {
      data->fill<float>(i_distoppt, (stop1->p4() + stop2->p4()).pt());
    }

    if (gent.size()>0) {
      data->fill<float>(i_gentop1pt, gent[0]->pt());
    }
    if (gent.size()>1) {
      data->fill<float>(i_gentop2pt, gent[1]->pt());
      data->fill<float>(i_ditoppt, (gent[0]->p4()+gent[1]->p4()).pt());
    }

    if (genw.size()>0) { data->fill<float>(i_genwpt, genw[0]->pt()); }

    if (genb.size()>0) {
      data->fill<float>(i_genb1pt, genb[0]->pt());
      data->fill<float>(i_genb1eta, genb[0]->eta());
    }
    if (genb.size()>1) {
      data->fill<float>(i_genb2pt, genb[1]->pt());
      data->fill<float>(i_genb2eta, genb[1]->eta());
      data->fill<float>(i_dphigenb1genb2,fabs(PhysicsUtilities::deltaPhi(*genb[0],*genb[1])) );
      data->fill<float>(i_drgenb1genb2,  fabs(PhysicsUtilities::deltaR(*genb[0],*genb[1])) );
    }

    int ngenjets = 0, ngenbjets = 0;
    for(const auto &j : ana->defaultJets->genJets) {
      ngenjets++;
      if(fabs(j.flavor()) == JetFlavorInfo::b_jet) {
        ngenbjets++;
      }
    }
    data->fill<int  >(i_ngenjets, ngenjets);
    data->fill<int  >(i_ngenbjets, ngenbjets);

  }

};

#endif
