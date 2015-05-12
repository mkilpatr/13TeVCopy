#include "ObjectProducers/TopTagging/interface/CORRAL.h"

#include "AnalysisTools/TreeReader/interface/GenParticleReader.h"
#include "AnalysisTools/TreeReader/interface/JetReader.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

using namespace std;
using namespace ucsbsusy;

// ---------------------------------------------------------------------
//
//     GENERAL
//
// ---------------------------------------------------------------------

TopJetMatching::TopDecayEvent* CORRAL::associateDecays(const ucsbsusy::GenParticleReader* genParticleReader, ucsbsusy::JetReader * jetReader,
    std::vector<ucsbsusy::RecoJetF*>& recoJets,  std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays){

  vector<GenJetF*> genJets;
  recoJets.clear();
  decays.clear();
  for(auto& j : jetReader->recoJets){
    if(!isCORRALJet(j.pt(),j.eta())) continue;
    recoJets.push_back(&j);
    if(j.genJet() == 0)  continue;
    genJets.push_back(j.genJet());
  }

  if(genParticleReader){
    TopJetMatching::TopDecayEvent * topDecayEvent = new TopJetMatching::TopDecayEvent(*genParticleReader,*jetReader,genJets);
    topDecayEvent->getDecayMatches(recoJets,decays);
    return topDecayEvent;
  } else {
    return 0;
  }


}

bool CORRAL::setup(const ucsbsusy::GenParticleReader* genParticleReader, ucsbsusy::JetReader * jetReader,
    std::vector<ucsbsusy::RecoJetF*>& recoJets,  std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays){

  TopJetMatching::TopDecayEvent* topDecayEvent = associateDecays(genParticleReader,jetReader,recoJets,decays);
  bool decision = true;

  //filter
  for(const auto& t : topDecayEvent->topDecays){
    if(t.isLeptonic) decision =  false;
    if(t.diag != TopJetMatching::RESOLVED_TOP) decision = false;
  }

  delete topDecayEvent;
  return decision;
}


// ---------------------------------------------------------------------
//
//     W Jet Likilihood
//
// ---------------------------------------------------------------------
void CORRAL::getJetCandidates(const std::vector<ucsbsusy::RecoJetF*>& recoJets, const std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays,
    std::vector<int>& types ){
  types.clear();
  types.resize(recoJets.size());
  for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
    if(decays[iJ].type == TopJetMatching::TopDecayEvent::DecayID::TOP_W)
      types[iJ] = 1;
    else if(decays[iJ].type == TopJetMatching::TopDecayEvent::DecayID::TOP_B)
      types[iJ] = 0;
    else
      types[iJ] = 2;
  }
}

CORRAL::jetCandVars CORRAL::calculateJetCandVars(const int nPV, const ucsbsusy::JetReader * jetReader, const ucsbsusy::RecoJetF * jet){
  jetCandVars vars;
  vars.jetPT = jet->pt();
  vars.axis1 = jetReader->jetaxis1_->at(jet->index());
  vars.axis2 = jetReader->jetaxis2_->at(jet->index());
  vars.ptD = jetReader->jetptD_->at(jet->index());
  vars.jetMult = jetReader->jetMult_->at(jet->index());
  vars.betaStar = nPV == 0? 10 : jetReader->jetbetaStar_->at(jet->index())/TMath::Log(float(nPV) -.67);
  vars.area = jetReader->jetarea_->at(jet->index());
  vars.mass  = jet->mass();
  return vars;
}

CORRAL::WJetLikliMVA::WJetLikliMVA(TString filename,TString bdtName ){
  TFile* file = TFile::Open(filename);
  param = dynamic_cast<ParamatrixMVA*>(file->Get(bdtName));
  delete file;

  const auto * mva = param->getOne();
  i_jetPT     = mva->findVariable("jetPT"   );
  i_axis1     = mva->findVariable("axis1"   );
  i_axis2     = mva->findVariable("axis2"   );
  i_ptD       = mva->findVariable("ptD"     );
  i_jetMult   = mva->findVariable("jetMult" );
  i_betaStar  = mva->findVariable("betaStar");
  i_area      = mva->findVariable("area"    );
  i_mass      = mva->findVariable("mass"    );
}

float CORRAL::WJetLikliMVA::mvaVal(jetCandVars& v) const {
  const auto * mva = param->get(min(max(v.jetPT, float(20.)), float(1000.)));
  mva->setVariable(i_jetPT   ,float(v.jetPT));
  mva->setVariable(i_axis1   ,float(v.axis1));
  mva->setVariable(i_axis2   ,float(v.axis2));
  mva->setVariable(i_ptD     ,float(v.ptD));
  mva->setVariable(i_jetMult ,int(v.jetMult));
  mva->setVariable(i_betaStar,float(v.betaStar));
  mva->setVariable(i_area    ,float(v.area));
  mva->setVariable(i_mass    ,float(v.mass));
  v.mva = mva->evaluateMethod(0);
  return v.mva;
}

// ---------------------------------------------------------------------
//
//     W Candidates
//
// ---------------------------------------------------------------------
CORRAL::WCand::WCand(const ucsbsusy::RecoJetF * jet1_,const ucsbsusy::RecoJetF * jet2_, int ind1_, int ind2_, bool isW_, int topIndex_, int fakeCategory_) :
        jet1(jet1_),
        jet2(jet2_),
        ind1(ind1_),
        ind2(ind2_),
        mom(jet1->p4() + jet2->p4()),
        isW(isW_),
        topIndex(topIndex_),
        fakeCategory(fakeCategory_)
      {
      }
CORRAL::WCand::WCand(const WCand& other) :
  jet1(other.jet1),
  jet2(other.jet2),
  ind1(other.ind1),
  ind2(other.ind2),
  mom(other.mom),
  isW(other.isW),
  topIndex(other.topIndex),
  fakeCategory(other.fakeCategory)
{}

void CORRAL::addWCandidate(const unsigned int iJ, const unsigned int iJ2,
    const std::vector<ucsbsusy::RecoJetF*>& recoJets, const std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays,
    std::vector<WCand>& wCands){
  bool isW = false;
  int topIn = -1;
  TopJetMatching::TopDecayEvent::DecayID::Type id1 = decays[iJ].type;
  TopJetMatching::TopDecayEvent::DecayID::Type id2 = decays[iJ2].type;
  if(id1 ==  TopJetMatching::TopDecayEvent::DecayID::TOP_W &&  id2 == TopJetMatching::TopDecayEvent::DecayID::TOP_W){
    if(decays[iJ].topInd == decays[iJ2].topInd){
      if(decays[iJ].mainParton() != decays[iJ2].mainParton())
        isW = true;
        topIn = decays[iJ].topInd;
    }
  }
  int fakeCategory = -10;
  if(!isW){
    int nWs = 0;
    int nBs = 0;
    if(id1 ==  TopJetMatching::TopDecayEvent::DecayID::TOP_B )nBs++;
    if(id2 ==  TopJetMatching::TopDecayEvent::DecayID::TOP_B )nBs++;
    if(id1 ==  TopJetMatching::TopDecayEvent::DecayID::TOP_W )nWs++;
    if(id2 ==  TopJetMatching::TopDecayEvent::DecayID::TOP_W )nWs++;

    if(nWs == 2) fakeCategory = 0;
    else if(nWs == 1) {
      if(nBs == 1) fakeCategory = 1;
      else fakeCategory = 2;
    } else if (nBs == 2) fakeCategory  = 3;
    else if (nBs == 1) fakeCategory = 4;
    else fakeCategory = 5;
  }
  wCands.emplace_back(recoJets[iJ],recoJets[iJ2],iJ,iJ2,isW, topIn, fakeCategory);
}

void CORRAL::getWCandidates(const std::vector<ucsbsusy::RecoJetF*>& recoJets, const std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays,
    std::vector<WCand>& wCands  ){
  wCands.clear();
  if(recoJets.size() < 2) return;
  wCands.reserve(recoJets.size() * (recoJets.size() - 1)/2);

  for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
    for(unsigned int iJ2 = iJ + 1; iJ2 < recoJets.size(); ++iJ2){
      addWCandidate(iJ,iJ2,recoJets,decays,wCands);
    }
  }
}

CORRAL::WCandVars CORRAL::calculateWCandVars(const ucsbsusy::JetReader * jetReader, const std::vector<ucsbsusy::RecoJetF*>& recoJets,
    const std::vector<jetCandVars>& jetCandVars, const WCand& cand){
  WCandVars vars;
  const auto  * j1 = cand.jet1;
  const auto  * j2 = cand.jet2;
  const auto  * W = &cand.mom;
  const double wJetLikli_j1 = jetCandVars[cand.ind1].mva;
  const double wJetLikli_j2 = jetCandVars[cand.ind2].mva;

  vars.wPT         = W->pt();
  vars.wMass       = W->mass();
  vars.pt2opt1     = j1->pt() == 0 ? 10 : j2->pt()/j1->pt();
  vars.wJetLikli1  = wJetLikli_j1 > wJetLikli_j2 ? wJetLikli_j1 : wJetLikli_j2;
  vars.wJetLikli2  = wJetLikli_j1 > wJetLikli_j2 ? wJetLikli_j2 : wJetLikli_j1;
  vars.maxCSV      = max(cand.jet1->csv(),j1->csv());
  vars.dr          = PhysicsUtilities::deltaR(*j1,*j2);
  vars.deta        = TMath::Abs(PhysicsUtilities::deltaEta(*j1,*j2));
  vars.dphi        = TMath::Abs(PhysicsUtilities::deltaPhi(*j1,*j2));
  vars.charge      = 0;//vars.wPT == 0? 10 :  (jetReader->jetcharge_->at(j1->index())*j1->pt() + jetReader->jetcharge_->at(j2->index())*j2->pt())/(vars.wPT);

//  const double dRapJ2J1 = j2->p4().Rapidity() - j1->p4().Rapidity();
//  const double dPhiJ2J1 = PhysicsUtilities::deltaPhi(*j2,*j1);
//  const double dJ2J1Norm =   TMath::Sqrt(dRapJ2J1*dRapJ2J1 + dPhiJ2J1*dPhiJ2J1);
//  const double dJ2J1RapN = dRapJ2J1/dJ2J1Norm;
//  const double dJ2J1PhiN = dPhiJ2J1/dJ2J1Norm;


//  const double pull1N =  TMath::Sqrt(jetReader->jetpulleta_->at(j1->index())*jetReader->jetpulleta_->at(j1->index()) +jetReader->jetpullphi_->at(j1->index())*jetReader->jetpullphi_->at(j1->index()));
//  const double pull1EtaN = jetReader->jetpulleta_->at(j1->index()) / pull1N;
//  const double pull1PhiN = jetReader->jetpullphi_->at(j1->index()) / pull1N;
//  const double pull2N =  TMath::Sqrt(jetReader->jetpulleta_->at(j2->index())*jetReader->jetpulleta_->at(j2->index()) +jetReader->jetpullphi_->at(j2->index())*jetReader->jetpullphi_->at(j2->index()));
//  const double pull2EtaN = jetReader->jetpulleta_->at(j2->index()) / pull2N;
//  const double pull2PhiN = jetReader->jetpullphi_->at(j2->index()) / pull2N;
//
//  const double pull1_dot_dJ2J1 =  pull1EtaN*dJ2J1RapN + pull1PhiN*dJ2J1PhiN;
//  const double pull1_cross_dJ2J1 = dJ2J1RapN*pull1PhiN-dJ2J1PhiN*pull1EtaN;
//
//  const double pull2_dot_dJ2J1   =  pull2EtaN*dJ2J1RapN + pull2PhiN*dJ2J1PhiN;
//  const double pull2_cross_dJ2J1 = dJ2J1RapN*pull2PhiN-dJ2J1PhiN*pull2EtaN;

  vars.pullAng1    = 0;//TMath::ATan2( pull1_cross_dJ2J1,pull1_dot_dJ2J1 );
  vars.pullAng2    = 0;//TMath::ATan2( pull2_cross_dJ2J1,pull2_dot_dJ2J1 );

  const double maxWRadius = max(PhysicsUtilities::deltaR(*j1,*W),PhysicsUtilities::deltaR(*j2,*W));
  int nWConJets = 0;
  for(unsigned int iJ = 0;iJ < recoJets.size(); ++iJ){
    if(int(iJ) == cand.ind1 || int(iJ) == cand.ind2) continue;
    if(PhysicsUtilities::deltaR(*recoJets[iJ],*W) < maxWRadius) ++nWConJets;
  }
  vars.nWCon       = nWConJets;
  return vars;
}

CORRAL::WMVA::WMVA(TString filename,TString bdtName ){
  TFile* file = TFile::Open(filename);
  param = dynamic_cast<ParamatrixMVA*>(file->Get(bdtName));
  delete file;

  const auto * mva = param->getOne();
  i_wPT        = mva->findVariable("wPT"        );
  i_wMass      = mva->findVariable("mass"       );
  i_pt2opt1    = mva->findVariable("pt2opt1"    );
  i_wJetLikli1 = mva->findVariable("wJetLikli1" );
  i_wJetLikli2 = mva->findVariable("wJetLikli2" );
  i_maxCSV     = mva->findVariable("maxCSV"     );
  i_dr         = mva->findVariable("dr"         );
  i_deta       = mva->findVariable("deta"       );
  i_dphi       = mva->findVariable("dphi"       );
  i_nWCon      = mva->findVariable("nWCon"      );
}

bool CORRAL::WMVA::passPresel(WCandVars& vars) const {
//  if(vars.wMass > 200)      return false;
//  if(vars.maxCSV > .941)     return false;
//  if(vars.wJetLikli2 < -.8) return false;
//  if(vars.deta > 2.5)        return false;
//  if(vars.dr > 3.5)         return false;
  return true;
}

float CORRAL::WMVA::mvaVal(WCandVars& vars) const {
  const auto * mva = param->get(min(vars.wPT,float(950.)));

  if(!passPresel(vars)){
    vars.mva = -1;
    return -1;
  }

  mva->setVariable(i_wPT        ,vars.wPT               );
  mva->setVariable(i_wMass      ,min(vars.wMass,float(500)));
  mva->setVariable(i_pt2opt1    ,vars.pt2opt1           );
  mva->setVariable(i_wJetLikli1 ,vars.wJetLikli1        );
  mva->setVariable(i_wJetLikli2 ,vars.wJetLikli2        );
  mva->setVariable(i_maxCSV     ,max(vars.maxCSV,float(0)));
  mva->setVariable(i_dr         ,vars.dr                );
  mva->setVariable(i_deta       ,vars.deta              );
  mva->setVariable(i_dphi       ,vars.dphi              );
  mva->setVariable(i_nWCon      ,vars.nWCon             );

  vars.mva = mva->evaluateMethod(0);
  return vars.mva;

}

bool CORRAL::WMVA::passMVA(const double pt, const double mvaV) const {
  if(pt < 100){
    return mvaV > -.6;
  }
  if(pt  < 300){
    return mvaV > -.7;
  }
  return mvaV > -.9;
}

// ---------------------------------------------------------------------
//
//     T Candidates
//
// ---------------------------------------------------------------------

CORRAL::TCand::TCand(const WCand * wCand_,const ucsbsusy::RecoJetF * bJet_, int wInd_, int bInd_, int type_, int topIndex_, int fakeCategory_) :
        wCand(wCand_),
        bJet(bJet_),
        wInd(wInd_),
        bInd(bInd_),
        mom(wCand->mom.p4() + bJet->p4()),
        type(type_),
        topIndex(topIndex_),
        fakeCategory(fakeCategory_)
      {
      }

void CORRAL::addTCandidate(const unsigned int iW, const WCand& wCand, const unsigned int iJ,
    const std::vector<ucsbsusy::RecoJetF*>& recoJets, const std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays,
    std::vector<TCand>& tCands){
  int type = 2;
  int topIndex = -1;
  if(decays[iJ].topInd >= 0 ){
    if(decays[iJ].topInd  ==  decays[wCand.ind1].topInd && decays[iJ].topInd  ==  decays[wCand.ind2].topInd ){
      topIndex = decays[iJ].topInd;
      if(decays[iJ].type == TopJetMatching::TopDecayEvent::DecayID::TOP_B){
        type = 1;
      } else {
        type = 0;
      }
    }
  }
  int fakeCategory = -1;
  if(type == 2){
    if(wCand.isW){
      if(decays[iJ].type == TopJetMatching::TopDecayEvent::DecayID::TOP_B) fakeCategory = 0; //w1b2;
      else fakeCategory = 1; //wF;
    } else if(decays[wCand.ind1].topInd == decays[wCand.ind2].topInd){
      fakeCategory = 2; //w(bw)F
    } else if(decays[iJ].type == TopJetMatching::TopDecayEvent::DecayID::TOP_B){
      if( decays[wCand.ind1].topInd ==  decays[iJ].topInd ) fakeCategory = 3; //w(wf)b
      else if( decays[wCand.ind2].topInd ==  decays[iJ].topInd) fakeCategory = 3; //w(wf)b
      else fakeCategory = 4; //w(ff)b;
    }
    else fakeCategory = 5; //w(ff)F
  }
  tCands.emplace_back(&wCand,recoJets[iJ],iW,iJ,type, topIndex, fakeCategory);
}

void CORRAL::getTCandidates(const WMVA& wMVA, const std::vector<ucsbsusy::RecoJetF*>& recoJets, const std::vector<TopJetMatching::TopDecayEvent::DecayID>& decays,
    const std::vector<WCand>& wCands,const std::vector<WCandVars>& wCandVars, std::vector<TCand>& tCands   ){
  tCands.clear();
  if(recoJets.size() < 3) return;
  tCands.reserve(wCands.size() *(recoJets.size() - 2) );

  for(unsigned int iW = 0; iW < wCands.size(); ++iW){
    const auto& wCand = wCands[iW];
    if(!wMVA.passMVA(wCandVars[iW].wPT,wCandVars[iW].mva)) continue;
    for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
      if(wCand.ind1 == int(iJ) || wCand.ind2 == int(iJ)  ) continue;
      addTCandidate(iW,wCand,iJ,recoJets,decays,tCands);
    }
  }
}

CORRAL::TCandVars CORRAL::calculateTCandVars(const ucsbsusy::JetReader * jetReader, const std::vector<ucsbsusy::RecoJetF*>& recoJets,
    const std::vector<WCand>& wCands, const std::vector<jetCandVars>& jetCandVars, const std::vector<WCandVars>& wCandVars,
    const TCand& cand){
  TCandVars vars;
  const auto& wCand = *cand.wCand;
  const auto&  T = cand.mom;
  const auto&  W = wCand.mom;
  const auto&  B = *cand.bJet;

  const int bInd = cand.bInd;
  const int wInd = cand.wInd;

  vars.tPT           = T.pt()                                  ;
  vars.wPT           = W.pt()                                  ;
  vars.tMass         = T.mass()                                ;
  vars.wMass         = W.mass()                                ;
  vars.bPTotPT       = vars.tPT  > 0 ? B.pt() / vars.tPT : 10  ;
  vars.bCSV          = B.csv()                                 ;
  vars.maxWCSV       = wCandVars[wInd].maxCSV                  ;
  vars.bWLikli       = jetCandVars[bInd].mva                   ;
  vars.wDisc         = wCandVars[wInd].mva                     ;

  vars.maxOWDisc     = -1;
  for(int iW = 0; iW < int(wCands.size()); ++iW){
    if(iW == wInd) continue;
    if(wCands[iW].ind1 != bInd && wCands[iW].ind2 != bInd) continue;
    if(wCand.ind1 == wCands[iW].ind1 || wCand.ind1 == wCands[iW].ind2 ){
      vars.maxOWDisc = max(vars.maxOWDisc,wCandVars[iW].mva);
    } else if(wCand.ind2 == wCands[iW].ind1 || wCand.ind2 == wCands[iW].ind2 ){
      vars.maxOWDisc = max(vars.maxOWDisc,wCandVars[iW].mva);
    }
  }



  vector<CylLorentzVectorF> momO(3);
  momO[0] = B.p4();momO[1] = wCand.jet1->p4();momO[2] = wCand.jet2->p4();
  sort(momO.begin(),momO.end(),PhysicsUtilities::greaterPT<CylLorentzVectorF>());
  auto m12 = (momO[0] + momO[1]).mass();
  auto m13 = (momO[0] + momO[2]).mass();
  auto m23 = (momO[1] + momO[2]).mass();

  vars.m23om123      = vars.tMass  == 0 ? 10 : m23/vars.tMass;
  vars.m13om12       = m12         == 0 ? 10 : m13/m12       ;
  vars.atan_m13om12  = TMath::ATan( vars.m13om12 )           ;


  vars.maxjjdr       = max(  PhysicsUtilities::deltaR(B,*wCand.jet1),max(PhysicsUtilities::deltaR(B,*wCand.jet2),PhysicsUtilities::deltaR(*wCand.jet1,*wCand.jet2)));
  vars.wbDR          = PhysicsUtilities::deltaR(B,W);
  vars.wbDEta        = TMath::Abs(PhysicsUtilities::deltaEta(B,W));;
  vars.wbDPhi        = TMath::Abs(PhysicsUtilities::deltaPhi(B,W));

  vars.nTCon         = 0;

  double maxTDR2 = max(  PhysicsUtilities::deltaR2(B,T),max(PhysicsUtilities::deltaR2(*wCand.jet2,T),PhysicsUtilities::deltaR2(*wCand.jet1,T)));
  for(int tJ = 0; tJ < int(recoJets.size()); ++tJ){
    if(tJ == bInd || tJ == wCand.ind1 || tJ == wCand.ind2)continue;
    if(PhysicsUtilities::deltaR2(T,*recoJets[tJ]) <  maxTDR2) vars.nTCon++;
  }
  return vars;
}

CORRAL::T_MVA::T_MVA(TString filename,TString bdtName ){
  TFile* file = TFile::Open(filename);
  param = dynamic_cast<ParamatrixMVA*>(file->Get(bdtName));
  delete file;

  const auto * mva = param->getOne();
  i_tPT           = mva->findVariable("tPT"         );
  i_wPT           = mva->findVariable("wPT"         );
  i_tMass         = mva->findVariable("tMass"       );
  i_wMass         = mva->findVariable("wMass"       );
  i_bPTotPT       = mva->findVariable("bPTotPT"     );
  i_bCSV          = mva->findVariable("bCSV"        );
  i_maxWCSV       = mva->findVariable("maxWCSV"     );
  i_bWLikli       = mva->findVariable("bWLikli"     );
  i_wDisc         = mva->findVariable("wDisc"       );
  i_maxOWDisc     = mva->findVariable("maxOWDisc"   );
  i_m23om123      = mva->findVariable("m23om123"    );
  i_m13om12       = mva->findVariable("m13om12"     );
  i_atan_m13om12  = mva->findVariable("atan_m13om12");
  i_maxjjdr       = mva->findVariable("maxjjdr"     );
  i_wbDR          = mva->findVariable("wbDR"        );
  i_wbDEta        = mva->findVariable("wbDEta"      );
  i_wbDPhi        = mva->findVariable("wbDPhi"      );
  i_nTCon         = mva->findVariable("nTCon"       );
}

float CORRAL::T_MVA::mvaVal(TCandVars& vars) const {
    const auto * mva = param->get(min(vars.tPT,float(950.)));

    if(!passPresel(vars)){
      vars.mva = -1;
      return -1;
    }

    mva->setVariable(i_tPT           ,float(vars.tPT)           );
    mva->setVariable(i_wPT           ,float(vars.wPT)           );
    mva->setVariable(i_tMass         ,min(float(500),vars.tMass));
    mva->setVariable(i_wMass         ,min(float(500),vars.wMass));
    mva->setVariable(i_bPTotPT       ,min(float(5),vars.bPTotPT));
    mva->setVariable(i_bCSV          ,max(float(0),vars.bCSV)   );
    mva->setVariable(i_maxWCSV       ,max(float(0),vars.maxWCSV));
    mva->setVariable(i_bWLikli       ,float(vars.bWLikli      ) );
    mva->setVariable(i_wDisc         ,float(vars.wDisc        ) );
    mva->setVariable(i_maxOWDisc     ,float(vars.maxOWDisc    ) );
    mva->setVariable(i_m23om123      ,float(vars.m23om123     ) );
    mva->setVariable(i_m13om12       ,float(vars.m13om12      ) );
    mva->setVariable(i_atan_m13om12  ,float(vars.atan_m13om12 ) );
    mva->setVariable(i_maxjjdr       ,float(vars.maxjjdr      ) );
    mva->setVariable(i_wbDR          ,float(vars.wbDR         ) );
    mva->setVariable(i_wbDEta        ,float(vars.wbDEta       ) );
    mva->setVariable(i_wbDPhi        ,float(vars.wbDPhi       ) );
    mva->setVariable(i_nTCon         ,int(vars.nTCon        )   );

    vars.mva = mva->evaluateMethod(0);
    return vars.mva;

  }

bool CORRAL::T_MVA::passPresel(const TCandVars& vars) const {
//  if(vars.tMass > 300 || vars.tMass < 110 ) return false;
//  if(vars.bWLikli < -.9 ) return false;
//  if(vars.m23om123 < .2) return false;
//  if(vars.m13om12 > 3) return false;
  return true;
}


bool CORRAL::T_MVA::passMVA(const double pt, const double mvaV) const {
  if(pt < 150){
    return mvaV > -.6;
  }
  if(pt  < 300){
    return mvaV > -.6;
  }
  return mvaV > -.8;
}

// ---------------------------------------------------------------------
//
//     T Selection
//
// ---------------------------------------------------------------------

void CORRAL::pruneTopCandidates(const std::vector<TCand>& tCands,const std::vector<TCandVars>& tCandVars,
    std::vector<ucsbsusy::RankedIndex>& prunedTops, T_MVA * tMVA){
  vector<RankedIndex> allTops;
  allTops.reserve(tCands.size());
  for(unsigned int iT = 0; iT < tCands.size(); ++iT){
    if(tMVA && !tMVA->passMVA(tCandVars[iT].tPT,tCandVars[iT].mva)) continue;
    allTops.emplace_back(tCandVars[iT].mva,iT);
  }
  sort(allTops.begin(),allTops.end(),PhysicsUtilities::greaterFirst<double,int>());

  for(unsigned int iT = 0; iT < allTops.size(); ++iT){
    const int topInd =  allTops[iT].second ;

    bool isFirst = true;
    for(unsigned int iT2 = 0; iT2 < iT; ++iT2){
      const int topInd2 =  allTops[iT2].second ;
      if(tCands[topInd].equivJets(tCands[topInd2 ])){
        isFirst = false;
        break;
      }
    }
    if(isFirst)
      prunedTops.push_back(allTops[iT]);
  }
}

std::vector<std::pair<int,int>> CORRAL::getRankedTopPairs(const std::vector<TCand>& tCands, const std::vector<ucsbsusy::RankedIndex>& rankedTops){
  std::vector<std::pair<int,int>> rankedPairs;
  std::vector<ucsbsusy::RankedIndex> multiRanks;

  for(unsigned int iC = 0; iC < rankedTops.size();++iC){
  const auto& cand = tCands[rankedTops[iC].second];
  for(unsigned int iC2 = iC + 1; iC2 < rankedTops.size(); ++iC2){
    const auto& cand2 = tCands[rankedTops[iC2].second];
    if(!cand.exclJets(cand2)) continue;
    multiRanks.emplace_back(
        pairMetric(rankedTops[iC].first,rankedTops[iC2].first)
        ,rankedPairs.size());
    rankedPairs.emplace_back(rankedTops[iC].second,rankedTops[iC2].second);
  }
  }

  sort(multiRanks.begin(),multiRanks.end(),PhysicsUtilities::greaterFirst<double,int>());

  vector<pair<int,int>> rankedByMulti(multiRanks.size());
  for(unsigned int iR = 0; iR < multiRanks.size(); ++iR){
    rankedByMulti[iR].first = rankedPairs[multiRanks[iR].second].first;
    rankedByMulti[iR].second = rankedPairs[multiRanks[iR].second].second;
  }
  return rankedByMulti;
}

void CORRAL::findExclusiveTops(
    const std::vector<TCand>& tCands, const std::vector<ucsbsusy::RankedIndex>& rankedTops,
    const std::pair<double, std::vector<int> >& currentList, const unsigned int startI, const unsigned int endI,
    std::vector< std::pair<double, std::vector<int> > >& bestList)
{
  const unsigned int currentLevel = currentList.second.size();
  assert(bestList.size() >= currentLevel);

    for(unsigned int iJ = startI; iJ < endI; ++iJ){
      bool found = false;
      for(unsigned int iO = 0; iO <currentLevel; ++iO ){
        if(tCands[currentList.second[iO]].exclJets(tCands[rankedTops[iJ].second]) ) continue;
        found = true;
        break;
      }
      if(found) continue;

      std::pair<double, std::vector<int> >* newList = new std::pair<double, std::vector<int> >(currentList);
      newList->first += rankedTops[iJ].first;
      newList->second.push_back(rankedTops[iJ].second);

      if(bestList.size() < currentLevel + 1){
        bestList.push_back(*newList);
      } else if(bestList[currentLevel].first < newList->first ){
        bestList[currentLevel] = *newList;
      }
      if(currentLevel + 1 < 4)
        findExclusiveTops(tCands,rankedTops,*newList,iJ+1,endI, bestList);
      delete newList;
    }
}

std::vector<std::vector<int> > CORRAL::countTops(const std::vector<TCand>& tCands,const std::vector<ucsbsusy::RankedIndex>& rankedTops){
  std::vector< std::pair<double, std::vector<int> > >* bestList = new std::vector< std::pair<double, std::vector<int> > >;
  std::pair<double, std::vector<int> >* newList = new  std::pair<double, std::vector<int> >;
  findExclusiveTops(tCands,rankedTops,*newList,0, rankedTops.size(), *bestList);

  std::vector<std::vector<int> > prunedList;
  prunedList.reserve(bestList->size());
  for(const auto& bL : *bestList)
    prunedList.push_back(bL.second);
  delete bestList;
  delete newList;
  return prunedList;
}

// ---------------------------------------------------------------------
//
//     DEFAULT RUNNING -> Produce TOPS and Ws
//
// ---------------------------------------------------------------------

void CORRAL::CORRALData::reset(){
  recoJets.clear();
  decays.clear();
  jetVars.clear();
  wCands   .clear();
  wCandVars.clear();
  tCands   .clear();
  tCandVars.clear();
  rankedTPairs.clear();
  bestTopMatches.clear();
  reconstructedTop = false;
  top1 = 0;
  top2 = 0;
  top1_disc = -1;
  top2_disc = -1;
}

bool CORRAL::CORRALReconstructor::getTopPairs(const ucsbsusy::GenParticleReader * genParticleReader, ucsbsusy::JetReader * jetReader, const int nPV,
    std::vector<ucsbsusy::RankedIndex> * prunedTops) {
  data.reset();
  setup(genParticleReader,jetReader, data.recoJets,data.decays);

  //Jet Variables
  data.jetVars.resize(data.recoJets.size());
  for(unsigned int iJ = 0; iJ < data.recoJets.size(); ++iJ){
    data.jetVars[iJ] = calculateJetCandVars(nPV,jetReader,data.recoJets[iJ]);
    wJetLikliMVA.mvaVal(data.jetVars[iJ]);
  }

  //WCands;
  getWCandidates(data.recoJets,data.decays,data.wCands);
  data.wCandVars.resize(data.wCands.size());
  for(unsigned int iC = 0; iC < data.wCands.size(); ++iC){
    data.wCandVars[iC] = calculateWCandVars(jetReader,data.recoJets,data.jetVars,data.wCands[iC]);
    wMVA.mvaVal(data.wCandVars[iC]);
  }

  //TCands
  getTCandidates(wMVA,data.recoJets,data.decays,data.wCands,data.wCandVars,data.tCands);
  data.tCandVars.resize(data.tCands.size());
  for(unsigned int iC = 0; iC < data.tCands.size(); ++iC){
    data.tCandVars[iC] = calculateTCandVars(jetReader,data.recoJets,data.wCands,data.jetVars,data.wCandVars,data.tCands[iC]);
    tMVA.mvaVal(data.tCandVars[iC]);
  }

  if(prunedTops == 0){
    prunedTops = new vector<RankedIndex>();
    pruneTopCandidates(data.tCands,data.tCandVars,*prunedTops,&tMVA);
    data.rankedTPairs = getRankedTopPairs(data.tCands,*prunedTops);
//    data.bestTopMatches = countTops(data.tCands,*prunedTops);
    delete prunedTops;
  } else{
    prunedTops->clear();
    pruneTopCandidates(data.tCands,data.tCandVars,*prunedTops,&tMVA);
    data.rankedTPairs = getRankedTopPairs(data.tCands,*prunedTops);
//    data.bestTopMatches = countTops(data.tCands,*prunedTops);
  }

  if(data.rankedTPairs.size()){
    data.reconstructedTop = true;
    data.top1 = &data.tCands[data.rankedTPairs[0].first];
    data.top2 = &data.tCands[data.rankedTPairs[0].second];
    data.top1_disc = data.tCandVars[data.rankedTPairs[0].first].mva;
    data.top2_disc = data.tCandVars[data.rankedTPairs[0].second].mva;
  }

  return data.reconstructedTop;
}

