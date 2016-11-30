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

  // Histograms
  TH1D* hsys = nullptr;
  TH1D* hpartonht = nullptr;

  // Test vars -- Add here first for testing, and move to other categories or BasicVarsFiller later!

  // Syst. studies
  size i_systweights;
  size i_wpolWeightUp;
  size i_wpolWeightDn;
  size i_costhetastar;

  // JetMET extra
  size i_njets30   ;
  size i_nbjets30  ;
  size i_ht30      ;
  size i_j1eta     ;
  size i_j2eta     ;
  size i_j3pt      ;
  size i_j3eta     ;
  size i_csvj1eta  ;
  size i_csvj2eta  ;
  size i_csv1      ;
  size i_csv2      ;
  size i_dphicsv1met;
  size i_dphicsv2met;
  size i_dphicsv12met;
  size i_meff;
  //size i_dphitopmet;
  //size i_toppt;
  //size i_httwoleadfatjet;

  size i_j2lpt;
  size i_dphicsv1csv2;
  size i_drcsv1csv2;
  size i_dphij2lmet;
  size i_dphij1lj2l;
  size i_detaj1lj2l;
  size i_drj1lj2l;

  size i_j1chhadn2;
  size i_j1chhadn4;
  size i_j1chhadn6;

  // Lepton extra
  size i_absdphilepmet;
  size i_absdphilepw;
  size i_htalonglep;
  size i_annulus   ;
  size i_nvetoele  ;
  size i_nvetomu   ;
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
  size i_genelpt;
  size i_geneleta;
  size i_genmupt;
  size i_genmueta;
  size i_gentaupt;
  size i_gentaueta;

  //w-tag
  size i_sfbclose2lep;
  size i_ak8candmass;
  size i_ak8candmasstau21;
  size i_ak8candmasstau32;
  size i_ak8candpt;
  size i_ak8candpttau21;
  size i_ak8candpttau32;
  size i_ak8wpassmass;
  size i_ak8wpasspt;
  size i_ak8toppassmass;
  size i_ak8toppasspt;
  size i_ak8pt;
  size i_ak8eta;
  size i_ak8phi;
  size i_ak8rawmass;
  size i_ak8prunmass;
  size i_ak8sdmass;
  size i_ak8tau21;
  size i_ak8tau31;
  size i_ak8tau32;
  size i_ak8fromgenhadw;
  size i_ak8fromgenhadt;
  size i_nak8jets;

  // TopFrameTagger
  size i_fatjetmass;
  size i_fatjetenergy;
  size i_fatjetp;
  size i_fatjetpt;
  size i_fatjetpl;
  size i_fatjettau1;
  size i_fatjettau2;
  size i_fatjettau3;
  size i_subjetwmass;
  size i_subjetwenergy;
  size i_subjetwp;
  size i_subjetwpt;
  size i_subjetwpl;
  size i_subjetweta;
  size i_subjetwphi;
  size i_subjettopframewmass;
  size i_subjettopframewenergy;
  size i_subjettopframewp;
  size i_subjettopframewpt;
  size i_subjettopframewpl;
  size i_subjettopframeweta;
  size i_subjettopframewphi;
  size i_subjetbmass;
  size i_subjetbenergy;
  size i_subjetbp;
  size i_subjetbpt;
  size i_subjetbpl;
  size i_subjetbeta;
  size i_subjetbphi;
  size i_subjettopframebmass;
  size i_subjettopframebenergy;
  size i_subjettopframebp;
  size i_subjettopframebpt;
  size i_subjettopframebpl;
  size i_subjettopframebeta;
  size i_subjettopframebphi;

  void bookHist(TFile *outFile){
    outFile->cd();
    hsys = new TH1D("hsys", "syst weights", 1000, 0.5, 1000.5);
    hpartonht = new TH1D("hpartonht", ";parton HT;Events", 300, 0, 3000);
  }

  void bookTest(TreeWriterData* data){
  }

  void bookMulticlassWTopMVA(TreeWriterData* data){

    // multiclass mva
    data->add<int>("nmvaTopT", 0);
    data->add<int>("nmvaWT", 0);
    data->add<int>("nrestopM_topT_wT", 0);

  }

  void bookSyst(TreeWriterData* data){
    i_systweights       = data->addMulti<float>("","systweights",0);
    i_wpolWeightUp      = data->add<float>("","wpolWeightUp","F",1);
    i_wpolWeightDn      = data->add<float>("","wpolWeightDn","F",1);
    i_costhetastar      = data->add<float>("","costhetastar","F",-1);
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
    i_csvj2eta       = data->add<float>("","csvj2eta","F",0);
    i_csv1           = data->add<float>("","csv1","F",0);
    i_csv2           = data->add<float>("","csv2","F",0);
    i_dphicsv1met    = data->add<float>("","dphicsv1met","F",0);
    i_dphicsv2met    = data->add<float>("","dphicsv2met","F",0);
    i_dphicsv12met   = data->add<float>("","dphicsv12met","F",0);
    i_meff           = data->add<float>("","meff","F",0);
    //i_dphitopmet     = data->add<float>("","dphitopmet","F",0);
    //i_toppt          = data->add<float>("","toppt","F",0);
    //i_httwoleadfatjet = data->add<float>("","httwoleadfatjet","F",0);

    i_j2lpt          = data->add<float>("","j2lpt","F",0);
    i_dphij2lmet     = data->add<float>("","dphij2lmet","F",0);
    i_dphij1lj2l     = data->add<float>("","dphij1lj2l","F",0);
    i_detaj1lj2l     = data->add<float>("","detaj1lj2l","F",0);
    i_drj1lj2l       = data->add<float>("","drj1lj2l","F",0);
    i_dphicsv1csv2   = data->add<float>("","dphicsv1csv2","F",0);
    i_drcsv1csv2     = data->add<float>("","drcsv1csv2","F",0);

    i_j1chhadn2      = data->add<int>("","j1chhadn2","I",0);
    i_j1chhadn4      = data->add<int>("","j1chhadn4","I",0);
    i_j1chhadn6      = data->add<int>("","j1chhadn6","I",0);

  }

  void bookLepton(TreeWriterData* data){
    i_absdphilepmet  = data->add<float>("","absdphilepmet","F",-1);
    i_absdphilepw    = data->add<float>("","absdphilepw","F",0);
    i_htalonglep     = data->add<float>("","htalonglep","F",0);
    i_annulus        = data->add<float>("","annulus","F",0);
    i_nvetoele       = data->add<int>("","nvetoele","I",0);
    i_nvetomu        = data->add<int>("","nvetomu","I",0);
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
    i_genelpt        = data->addMulti<float>("","genelpt",0);
    i_geneleta       = data->addMulti<float>("","geneleta",0);
    i_genmupt        = data->addMulti<float>("","genmupt",0);
    i_genmueta       = data->addMulti<float>("","genmueta",0);
    i_gentaupt       = data->addMulti<float>("","gentaupt",0);
    i_gentaueta      = data->addMulti<float>("","gentaueta",0);
  }

  void bookWTag(TreeWriterData* data) {
    i_sfbclose2lep     = data->add<bool>("","sfbclose2lep","O",0);
    i_ak8candmass      = data->add<float>("","ak8candmass","F",0.);
    i_ak8candmasstau21 = data->add<float>("","ak8candmasstau21","F",0.);
    i_ak8candmasstau32 = data->add<float>("","ak8candmasstau32","F",0.);
    i_ak8candpt        = data->add<float>("","ak8candpt","F",0.);
    i_ak8candpttau21   = data->add<float>("","ak8candpttau21","F",0.);
    i_ak8candpttau32   = data->add<float>("","ak8candpttau32","F",0.);
    i_ak8wpassmass     = data->add<float>("","ak8wpassmass","F",0.);
    i_ak8wpasspt       = data->add<float>("","ak8wpasspt","F",0.);
    i_ak8toppassmass   = data->add<float>("","ak8toppassmass","F",0.);
    i_ak8toppasspt     = data->add<float>("","ak8toppasspt","F",0.);
    i_ak8pt        = data->addMulti<float>("","ak8pt",0);
    i_ak8eta       = data->addMulti<float>("","ak8eta",0);
    i_ak8phi       = data->addMulti<float>("","ak8phi",0);
    i_ak8rawmass   = data->addMulti<float>("","ak8rawmass",0);
    i_ak8prunmass  = data->addMulti<float>("","ak8prunmass",0);
    i_ak8sdmass    = data->addMulti<float>("","ak8sdmass",0);
    i_ak8tau21     = data->addMulti<float>("","ak8tau21",0);
    i_ak8tau31     = data->addMulti<float>("","ak8tau31",0);
    i_ak8tau32     = data->addMulti<float>("","ak8tau32",0);
    i_ak8fromgenhadw = data->add<bool>("","ak8fromgenhadw","O",0);
    i_ak8fromgenhadt = data->add<bool>("","ak8fromgenhadt","O",0);
  }


  void bookTopFrameTagger(TreeWriterData* data) {
    i_fatjetmass            = data->add<float>("","fatjetmass","F",0);
    i_fatjetenergy          = data->add<float>("","fatjetenergy","F",0);
    i_fatjetp               = data->add<float>("","fatjetp","F",0);
    i_fatjetpt              = data->add<float>("","fatjetpt","F",0);
    i_fatjetpl              = data->add<float>("","fatjetpl","F",0);
    i_fatjettau1            = data->add<float>("","fatjettau1","F",0);
    i_fatjettau2            = data->add<float>("","fatjettau2","F",0);
    i_fatjettau3            = data->add<float>("","fatjettau3","F",0);
    i_subjetwmass           = data->add<float>("","subjetwmass","F",0);
    i_subjetwenergy         = data->add<float>("","subjetwenergy","F",0);
    i_subjetwp              = data->add<float>("","subjetwp","F",0);
    i_subjetwpt             = data->add<float>("","subjetwpt","F",0);
    i_subjetwpl             = data->add<float>("","subjetwpl","F",0);
    i_subjetweta            = data->add<float>("","subjetweta","F",0);
    i_subjetwphi            = data->add<float>("","subjetwphi","F",0);
    i_subjettopframewmass   = data->add<float>("","subjettopframewmass","F",0);
    i_subjettopframewenergy = data->add<float>("","subjettopframewenergy","F",0);
    i_subjettopframewp      = data->add<float>("","subjettopframewp","F",0);
    i_subjettopframewpt     = data->add<float>("","subjettopframewpt","F",0);
    i_subjettopframewpl     = data->add<float>("","subjettopframewpl","F",0);
    i_subjettopframeweta    = data->add<float>("","subjettopframeweta","F",0);
    i_subjettopframewphi    = data->add<float>("","subjettopframewphi","F",0);
    i_subjetbmass           = data->add<float>("","subjetbmass","F",0);
    i_subjetbenergy         = data->add<float>("","subjetbenergy","F",0);
    i_subjetbp              = data->add<float>("","subjetbp","F",0);
    i_subjetbpt             = data->add<float>("","subjetbpt","F",0);
    i_subjetbpl             = data->add<float>("","subjetbpl","F",0);
    i_subjetbeta            = data->add<float>("","subjetbeta","F",0);
    i_subjetbphi            = data->add<float>("","subjetbphi","F",0);
    i_subjettopframebmass   = data->add<float>("","subjettopframebmass","F",0);
    i_subjettopframebenergy = data->add<float>("","subjettopframebenergy","F",0);
    i_subjettopframebp      = data->add<float>("","subjettopframebp","F",0);
    i_subjettopframebpt     = data->add<float>("","subjettopframebpt","F",0);
    i_subjettopframebpl     = data->add<float>("","subjettopframebpl","F",0);
    i_subjettopframebeta    = data->add<float>("","subjettopframebeta","F",0);
    i_subjettopframebphi    = data->add<float>("","subjettopframebphi","F",0);
  }


  void fillHistograms(const BaseTreeAnalyzer *ana){
    if (ana->isMC()){
      for(unsigned i=0; i < ana->evtInfoReader.systweights->size(); ++i) {
        double syswgt = ana->evtInfoReader.systweights->at(i)/ana->evtInfoReader.lhecentralweight;
        hsys->Fill(i, syswgt*ana->weight);
      }

      vector<GenParticleF*> partons;
      for (auto *p : ana->genParts){
        if (ParticleInfo::isDocOutgoing(p->status()) && ParticleInfo::isQuarkOrGluon(p->pdgId()))
          partons.push_back(p);
      }
      hpartonht->Fill(JetKinematics::ht(partons), ana->weight);
    }
  }

  void fillTestVars(TreeWriterData* data, const BaseTreeAnalyzer* ana){
  }

  void fillMulticlassWTopMVA(TreeWriterData* data, BaseTreeAnalyzer* ana){

    if (!ana->wtopMVA) ana->wtopMVA = new SoftdropWTopMulticlassMVA(defaults::MVAWEIGHT_SD_W_TOP);

    // multiclass MVAs
    vector<FatJetF*> mvaTopT, mvaWT;
    for (auto *fj : ana->fatJets){
      if (fj->pt()<200) continue;
      auto discs = ana->wtopMVA->getMVAScore(fj);
      auto top_score = discs.at(SoftdropWTopMulticlassMVA::TOP);
      auto w_score = discs.at(SoftdropWTopMulticlassMVA::W);
      if (fj->pt()>400 && top_score > SoftdropWTopMulticlassMVA::WP_TOP_TIGHT) mvaTopT.push_back(fj);
      if (w_score > SoftdropWTopMulticlassMVA::WP_W_TIGHT) mvaWT.push_back(fj);
    }
    data->fill<int>("nmvaTopT", mvaTopT.size());
    data->fill<int>("nmvaWT", mvaWT.size());

    auto getCleanedAK4 = [](const vector<RecoJetF*>& inAK4jets, const vector<FatJetF*> &topJets, const vector<FatJetF*> wJets){
      vector<RecoJetF*> ak4s;
      for (auto *j : inAK4jets){
        bool isOverlap = false;
        for (auto *fj : topJets) {
          if(PhysicsUtilities::deltaR(*j, *fj) < 0.8) {
            isOverlap = true; break;
          }
        }
        if (!isOverlap){
          for (auto *fj : wJets) {
            if(PhysicsUtilities::deltaR(*j, *fj) < 0.8) {
              isOverlap = true; break;
            }
          }
        }
        if(!isOverlap) ak4s.push_back(j);
      }
      return ak4s;
    };


    {// multiClassMVA, topT, wT
      auto ak4jets = getCleanedAK4(ana->jets, mvaTopT, mvaWT);
      auto resTopM = ana->resTopMVA->getTopCandidates(ak4jets, ResolvedTopMVA::WP_MEDIUM);
      data->fill<int>("nrestopM_topT_wT", resTopM.size());
    }// end multiClassMVA, topT, wT

  }

  void fillSystInfo(TreeWriterData* data, const BaseTreeAnalyzer* ana){
    for(auto wgt : *ana->evtInfoReader.systweights) {
      data->fillMulti<float>(i_systweights, wgt/ana->evtInfoReader.lhecentralweight);
    }
    data->fill<float>(i_wpolWeightUp, ana->wpolCorrections.getWpolWeightUp());
    data->fill<float>(i_wpolWeightDn, ana->wpolCorrections.getWpolWeightDn());
    data->fill<float>(i_costhetastar, ana->wpolCorrections.getCosThetaStar());
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
        data->fill<float>(i_csvj2eta, jetsCSVranked[1]->eta());
        data->fill<float>(i_csv2, jetsCSVranked[1]->csv());
        data->fill<float>(i_dphicsv1csv2,fabs(PhysicsUtilities::deltaPhi(*jetsCSVranked[0], *jetsCSVranked[1])));
        data->fill<float>(i_drcsv1csv2,  fabs(PhysicsUtilities::deltaR(*jetsCSVranked[0], *jetsCSVranked[1])));

      }
    }
    data->fill<float>(i_dphicsv12met,dphicsv12met);

    /*
    if (ana->nSelCTTTops > 0) {
      data->fill<float>(i_toppt,ana->selectedCTTTops[0]->pt());
      data->fill<float>(i_dphitopmet,fabs(PhysicsUtilities::deltaPhi(*ana->selectedCTTTops[0], *ana->met)));
    }
    if (ana->cttTops.size() > 1) {
      data->fill<float>(i_httwoleadfatjet,(ana->cttTops[0]->pt()+ana->cttTops[1]->pt()));
    }
    */

    if (ana->isrJets.size() > 1) {
      data->fill<float>(i_j2lpt,ana->isrJets[1]->pt());
      data->fill<float>(i_dphij2lmet,fabs(PhysicsUtilities::deltaPhi(*ana->isrJets[1],*met)));
      data->fill<float>(i_dphij1lj2l,fabs(PhysicsUtilities::deltaPhi(*ana->isrJets[0],*ana->isrJets[1])));
      data->fill<float>(i_detaj1lj2l,fabs(PhysicsUtilities::deltaEta(*ana->isrJets[0],*ana->isrJets[1])));
      data->fill<float>(i_drj1lj2l,fabs(PhysicsUtilities::deltaR(*ana->isrJets[0],*ana->isrJets[1])));
    }

    int j1chhadn2_ = -1;
    int j1chhadn4_ = -1;
    int j1chhadn6_ = -1;
    if(jets.size()>0){
      j1chhadn2_ = jets.at(0)->chHadN2();
      j1chhadn4_ = jets.at(0)->chHadN4();
      j1chhadn6_ = jets.at(0)->chHadN6();
    }
    data->fill<int>(i_j1chhadn2, j1chhadn2_);
    data->fill<int>(i_j1chhadn4, j1chhadn4_);
    data->fill<int>(i_j1chhadn6, j1chhadn6_);

  }

  void fillLeptonInfo(TreeWriterData* data, const BaseTreeAnalyzer* ana){

    int nVetoEle = 0; int nVetoMu = 0;
    for(auto i: ana->selectedLeptons){
      if(fabs(i->pdgid()) == 11) nVetoEle++;
      if(fabs(i->pdgid()) == 13) nVetoMu++;
    }
    data->fill<int  >(i_nvetomu, nVetoMu);
    data->fill<int  >(i_nvetoele, nVetoEle);

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
        bool passMu = (to->filterflags() & kSingleIsoMu24) && (to->pathflags() & kHLT_IsoMu24);
        bool passTkMu = (to->filterflags() & kSingleIsoTkMu24) && (to->pathflags() & kHLT_IsoTkMu24);
        if(passMu || passTkMu) {
          if(PhysicsUtilities::deltaR(*lep, *to) < 0.05) {
            matchtrigmu = true;
          }
        }
        if((to->filterflags() & kSingleEle27WPTight) && (to->pathflags() & kHLT_Ele27_WPTight_Gsf)) {
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

  void fillWTagInfo(TreeWriterData* data, const BaseTreeAnalyzer* ana) {

    bool  sfbclose2lep_     = false;
    float ak8candmass_      = 0.;
    float ak8candmasstau21_ = 0.;
    float ak8candmasstau32_ = 0.;
    float ak8candpt_        = 0.;
    float ak8candpttau21_   = 0.;
    float ak8candpttau32_   = 0.;
    float ak8wpassmass_     = 0.;
    float ak8wpasspt_       = 0.;
    float ak8toppassmass_   = 0.;
    float ak8toppasspt_     = 0.;
    bool  ak8fromgenhadw_   = false;
    bool  ak8fromgenhadt_   = false;

    std::vector<MomentumF> csvmjets;
    for(auto* j : ana->jets) {
      if(j->csv() > defaults::CSV_MEDIUM) {
        MomentumF tmpVecCSVMJets; tmpVecCSVMJets = j->p4();
        csvmjets.push_back(tmpVecCSVMJets); }
    }

    // check if there is a lepton
    if (ana->nSelLeptons > 0) {
      MomentumF lepf(ana->selectedLeptons.at(0)->p4());
      MomentumF* lep = &lepf;

      // find a b close to the lepton
      for (unsigned int i0=0; i0<csvmjets.size(); ++i0) {
        float drlepbjet_ = PhysicsUtilities::deltaR(*lep,csvmjets[i0]);
        if (drlepbjet_<(3.14/2.)) { sfbclose2lep_ = true; }
      }

      unsigned int count = 0;
      for(auto* fj : ana->fatJets) {
        if (PhysicsUtilities::deltaR(*lep,*fj)<(3.14/2.)) { continue; }
        if (count >0) { continue; }
        ak8candmass_ = fj->softDropMass();
        ak8candpt_   = fj->pt();
        // apply only the tau21 selection
        if (cfgSet::isSoftDropTagged(fj, 150, 0.,  1e9, 1e9,  0.60)) {
          ak8candmasstau21_ = fj->softDropMass();
          ak8candpttau21_   = fj->pt();
        }
        // apply only the tau32 selection
        if (cfgSet::isSoftDropTagged(fj, 150, 0.,  1e9, 0.69,  1e9)) {
          ak8candmasstau32_ = fj->softDropMass();
          ak8candpttau32_   = fj->pt();
        }
        // apply the w-tag selection
        if (cfgSet::isSoftDropTagged(fj, 150, 60,  110, 1e9,  0.60)) {
          ak8wpassmass_ = fj->softDropMass();
          ak8wpasspt_   = fj->pt();
        }
  // apply the top-tag selection
        if (cfgSet::isSoftDropTagged(fj, 150, 110,  210, 0.69,  1e9)) {
          ak8toppassmass_ = fj->softDropMass();
          ak8toppasspt_   = fj->pt();
        }
        if(doesFatJetMatch(ana, fj, 0.6, ParticleInfo::p_t)) {ak8fromgenhadt_ = true; }
        if(doesFatJetMatch(ana, fj, 0.6, ParticleInfo::p_Wplus)) {ak8fromgenhadw_ = true; }
        ++count;
      }

    } // end of checking a sel lepton

    else {

      unsigned int count = 0;
      for(auto* fj : ana->fatJets) {
        if (count >0) { continue; }
        ak8candmass_ = fj->softDropMass();
        ak8candpt_   = fj->pt();
        // apply only the tau21 selection
        if (cfgSet::isSoftDropTagged(fj, 150, 0.,  1e9, 1e9,  0.60)) {
          ak8candmasstau21_ = fj->softDropMass();
          ak8candpttau21_   = fj->pt();
        }
        // apply only the tau32 selection
        if (cfgSet::isSoftDropTagged(fj, 150, 0.,  1e9, 0.69,  1e9)) {
          ak8candmasstau32_ = fj->softDropMass();
          ak8candpttau32_   = fj->pt();
        }
        // apply the w-tag selection
        if (cfgSet::isSoftDropTagged(fj, 150, 60,  110, 1e9,  0.60)) {
          ak8wpassmass_ = fj->softDropMass();
          ak8wpasspt_   = fj->pt();
        }
        // apply the top-tag selection
        if (cfgSet::isSoftDropTagged(fj, 150, 110,  210, 0.69,  1e9)) {
          ak8toppassmass_ = fj->softDropMass();
          ak8toppasspt_   = fj->pt();
        }
        if(doesFatJetMatch(ana, fj, 0.6, ParticleInfo::p_t))     {ak8fromgenhadt_ = true; }
        if(doesFatJetMatch(ana, fj, 0.6, ParticleInfo::p_Wplus)) {ak8fromgenhadw_ = true; }
        ++count;
      }
    } // end of lep = 0

    if (ana->nSelLeptons==0) { sfbclose2lep_ = true; }
    if (csvmjets.size()==0)  { sfbclose2lep_ = true; }

    data->fill<bool> (i_sfbclose2lep     , sfbclose2lep_    );
    data->fill<float>(i_ak8candmass      , ak8candmass_     );
    data->fill<float>(i_ak8candmasstau21 , ak8candmasstau21_);
    data->fill<float>(i_ak8candmasstau32 , ak8candmasstau32_);
    data->fill<float>(i_ak8candpt        , ak8candpt_       );
    data->fill<float>(i_ak8candpttau21   , ak8candpttau21_  );
    data->fill<float>(i_ak8candpttau32   , ak8candpttau32_  );
    data->fill<float>(i_ak8wpassmass     , ak8wpassmass_    );
    data->fill<float>(i_ak8wpasspt       , ak8wpasspt_      );
    data->fill<float>(i_ak8toppassmass   , ak8toppassmass_  );
    data->fill<float>(i_ak8toppasspt     , ak8toppasspt_    );
    data->fill<bool >(i_ak8fromgenhadw   , ak8fromgenhadw_  );
    data->fill<bool >(i_ak8fromgenhadt   , ak8fromgenhadt_  );

    /*
    for(auto* fj : ana->fatJets) {
      // if (passSoftDropTaggerFJ(fj,110.,210.,0.50))    { ++nsdtopjmewp1tight_; }
      // if (passSoftDropTaggerFJ(fj,110.,210.,0.69))    { ++nsdtopjmewp1loose_; }
      // if (passSoftDropTaggerFJ(fj,60.,110.,10.,0.45)) { ++nsdwjmewp1tight_; }
      // if (passSoftDropTaggerFJ(fj,60.,110.,10.,0.60)) { ++nsdwjmewp1loose_; }

      data->fillMulti<float>(i_ak8pt      , fj->pt());
      data->fillMulti<float>(i_ak8eta     , fj->eta());
      data->fillMulti<float>(i_ak8phi     , fj->phi());
      data->fillMulti<float>(i_ak8rawmass , fj->fjRawMass());
      data->fillMulti<float>(i_ak8prunmass, fj->fjPrunedMass());
      data->fillMulti<float>(i_ak8sdmass  , fj->fjSoftDropMass());
      data->fillMulti<float>(i_ak8tau21   , (fj->fjTau2())/(fj->fjTau1()));
      data->fillMulti<float>(i_ak8tau31   , (fj->fjTau3())/(fj->fjTau1()));
      data->fillMulti<float>(i_ak8tau32   , (fj->fjTau3())/(fj->fjTau2()));
    }
    */
  }



  void fillTopFrameTaggerInfo(TreeWriterData* data, const BaseTreeAnalyzer* ana) {

    for(auto* fj : ana->fatJets) {
      //      TLorentzVector ak8p4_; ak8p4_.SetPtEtaPhiM(fj->pt(),fj->eta(),fj->phi(),fj->fjSoftDropMass());

      bool foundhadtop = false;
      bool ishadronictop_ = false;
      if(doesFatJetMatch(ana, fj, 0.6, ParticleInfo::p_t)) { ishadronictop_ = true; }

      //      if (ishadronictop_ && fj->fjNSDSubjets()>1) {
      if (fj->nSubjets()>1) {

	TLorentzVector genhadwp4_;
	for(auto* p : ana->genParts) {
	  if ( (abs(p->pdgId()) == 24) && (whadronicdecay(p)) ) { genhadwp4_.SetPtEtaPhiM(p->pt(),p->eta(),p->phi(),p->mass()); }
	}

	// get the fatjet and subjet p4
	TLorentzVector sj1p4_; if(fj->nSubjets() > 0 ) sj1p4_.SetPtEtaPhiM(fj->subJet(0).pt(),fj->subJet(0).eta(),fj->subJet(0).phi(),fj->subJet(0).mass());
	TLorentzVector sj2p4_; if(fj->nSubjets() > 1 ) sj2p4_.SetPtEtaPhiM(fj->subJet(1).pt(),fj->subJet(1).eta(),fj->subJet(1).phi(),fj->subJet(1).mass());
	TLorentzVector ak8p4_ = sj1p4_ + sj2p4_;

	// match the subjet to the W boson
	float drsj1genhadw = ROOT::Math::VectorUtil::DeltaR(genhadwp4_,sj1p4_);
	float drsj2genhadw = ROOT::Math::VectorUtil::DeltaR(genhadwp4_,sj2p4_);

	TLorentzVector sjw_;
	TLorentzVector sjb_;
	//if (drsj1genhadw<drsj2genhadw) { sjw_ = sj1p4_; sjb_ = sj2p4_; }
	//else                           { sjw_ = sj2p4_; sjb_ = sj1p4_; }
	if (sj1p4_.M()>sj2p4_.M()) { sjw_ = sj1p4_; sjb_ = sj2p4_; }
	else                       { sjw_ = sj2p4_; sjb_ = sj1p4_; }

	// get the top boost
	TVector3 boosttop; boosttop = ak8p4_.BoostVector();

	// calculate subjet vectors in top rest frame
	TLorentzVector sjtopframew_ = sjw_;
	sjtopframew_.Boost(-boosttop);

	TLorentzVector sjtopframeb_ = sjb_;
	sjtopframeb_.Boost(-boosttop);

	// store variables
	data->fill<float>(i_fatjetmass  , ak8p4_.M());
	data->fill<float>(i_fatjetenergy, ak8p4_.Energy());
	data->fill<float>(i_fatjetp     , ak8p4_.P());
	data->fill<float>(i_fatjetpt    , ak8p4_.Pt());
	data->fill<float>(i_fatjetpl    , ak8p4_.Pz());
	data->fill<float>(i_fatjettau1  , fj->tau1());
	data->fill<float>(i_fatjettau2  , fj->tau2());
	data->fill<float>(i_fatjettau3  , fj->tau3());
	data->fill<float>(i_subjetwmass  , sjw_.M());
	data->fill<float>(i_subjetwenergy, sjw_.Energy());
	data->fill<float>(i_subjetwp     , sjw_.P());
	data->fill<float>(i_subjetwpt    , sjw_.Pt());
	data->fill<float>(i_subjetwpl    , sjw_.Pz());
	data->fill<float>(i_subjetweta   , sjw_.Eta());
	data->fill<float>(i_subjetwphi   , sjw_.Phi());
	data->fill<float>(i_subjettopframewmass  , sjtopframew_.M());
	data->fill<float>(i_subjettopframewenergy, sjtopframew_.Energy());
	data->fill<float>(i_subjettopframewp     , sjtopframew_.P());
	data->fill<float>(i_subjettopframewpt    , sjtopframew_.Pt());
	data->fill<float>(i_subjettopframewpl    , sjtopframew_.Pz());
	data->fill<float>(i_subjettopframeweta   , sjtopframew_.Eta());
	data->fill<float>(i_subjettopframewphi   , sjtopframew_.Phi());
	data->fill<float>(i_subjetbmass  , sjb_.M());
	data->fill<float>(i_subjetbenergy, sjb_.Energy());
	data->fill<float>(i_subjetbp     , sjb_.P());
	data->fill<float>(i_subjetbpt    , sjb_.Pt());
	data->fill<float>(i_subjetbpl    , sjb_.Pz());
	data->fill<float>(i_subjetbeta   , sjb_.Eta());
	data->fill<float>(i_subjetbphi   , sjb_.Phi());
	data->fill<float>(i_subjettopframebmass  , sjtopframeb_.M());
	data->fill<float>(i_subjettopframebenergy, sjtopframeb_.Energy());
	data->fill<float>(i_subjettopframebp     , sjtopframeb_.P());
	data->fill<float>(i_subjettopframebpt    , sjtopframeb_.Pt());
	data->fill<float>(i_subjettopframebpl    , sjtopframeb_.Pz());
	data->fill<float>(i_subjettopframebeta   , sjtopframeb_.Eta());
	data->fill<float>(i_subjettopframebphi   , sjtopframeb_.Phi());

	foundhadtop = true;
      } // end of if hadronically decaying top
      if (foundhadtop) { continue; }

    }
  } // end of topframetagging

  bool whadronicdecay(const GenParticleF* genw) {
    bool whadronicdecay_ = true;
    for (unsigned int i0=0; i0<genw->numberOfDaughters(); ++i0) {
      int wdaupdgid = abs(genw->daughter(i0)->pdgId());
      if ( (wdaupdgid==11) || (wdaupdgid==12) || (wdaupdgid==13) ||
	   (wdaupdgid==14) || (wdaupdgid==15) || (wdaupdgid==16) )  {
	whadronicdecay_ = false; }
    }
      return whadronicdecay_;
  }


  bool doesFatJetMatch(const BaseTreeAnalyzer *ana, FatJetF* fj, float dr, ParticleInfo::ParticleID matchtoid){
    // match fj to gen w or top
    for(auto* p : ana->genParts) {
      if (abs(p->pdgId()) == matchtoid && ParticleInfo::isLastInChain(p)){
        if( (ParticleInfo::isGenTopHadronic(p) && matchtoid==ParticleInfo::p_t) ||
            (ParticleInfo::isGenWHadronic(p)   && matchtoid==ParticleInfo::p_Wplus)){
          std::cout << "found a candidate gen part with " << p->pdgId() << " " << p->p4() << std::endl;
          std::cout << "and dr " << PhysicsUtilities::deltaR(*p, *fj) << std::endl;
          if(PhysicsUtilities::deltaR(*p, *fj) < dr) { return true; }
        }
      }
    }
    return false;
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

      if (p->numberOfMothers()==0) continue;

      const GenParticleF *genPartMom = p->mother(0);
      if ( (abs(p->pdgId())==11) && (abs(genPartMom->pdgId())==24) ) {
        ++ngenel_;  ++ngenlep_; genlepq_=p->pdgId()>0 ? -1 : 1;
        data->fillMulti<float>(i_genelpt, p->pt());
        data->fillMulti<float>(i_geneleta, p->eta());
      }
      if ( (abs(p->pdgId())==13) && (abs(genPartMom->pdgId())==24) ) {
        ++ngenmu_;  ++ngenlep_; genlepq_=p->pdgId()>0 ? -1 : 1;
        data->fillMulti<float>(i_genmupt, p->pt());
        data->fillMulti<float>(i_genmueta, p->eta());
      }
      if ( (abs(p->pdgId())==15) && (abs(genPartMom->pdgId())==24) ) {
        ++ngentau_; ++ngenlep_; genlepq_=p->pdgId()>0 ? -1 : 1;
        data->fillMulti<float>(i_gentaupt, p->pt());
        data->fillMulti<float>(i_gentaueta, p->eta());
      }

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
