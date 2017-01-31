//
// strategy as of jan 30 2017
// by event reweighting
// per https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
//
//   eff(j,WP) refers to MC efficiency of the jet j to pass working point WP, 
//   sf() refers to data/MC SF of that eff, sfeff refers to product.
//
//   evt weight = P(DATA)/P(MC) ratio of 'probabilities of that jet configuration'
//
//   P(MC) = product over every tight-tagged jet j [ eff(j,tight) ]
//          *product over every medium-but-not-tight-tagged jet j [ eff(j,medium) - eff(j,tight) ]
//          *product over every loose-but-not-medium-tagged jet j [ eff(j,loose) - eff(j,medium) ]
//          *product over every not-loose-tagged jet j [1 - eff(j,loose)]
//
//   P(DATA) = same with eff replaced by sfeff
//
//   implementation looks like: for each jet, a loop over the working points forming the above product.
//     then a product over all the jets.
//

#include "AnalysisBase/TreeAnalyzer/interface/BTagCorrectionSet.h"
#include "AnalysisTools/QuickRefold/interface/TObjectContainer.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/BTagCalibrationStandalone.h"

#include "TFile.h"


namespace ucsbsusy {


BTagByEvtWeightCorr::BTagByEvtWeightCorr(TString effInput, TString sfInput, bool isFastSim ) : Correction("BTagByEvtWeight"),
    calib(0),
    effGetter (isFastSim ? &BTagByEvtWeightCorr::getJetFastSimEff : &BTagByEvtWeightCorr::getJetEff),
    sfGetter  (isFastSim ? &BTagByEvtWeightCorr::getJetFastSimEffSF : &BTagByEvtWeightCorr::getJetEffSF),
    corrReadersByOp(BTagEntry::OP_TIGHT+1), // 1 per working point
    effFile(0),
    eff(0)
{
  // setup calibration
  //std::clog << "[BTag Corrections] Doing fastsim weights for this file? "<< isFastSim << std::endl;;
  std::clog << "[BTag Corrections] Loading SF File: "<< sfInput<<std::endl;;
  calib = new BTagCalibration("csv",sfInput.Data());

  // setup readers, one per operating point
  for(unsigned iOP = BTagEntry::OP_LOOSE ; iOP <= BTagEntry::OP_TIGHT ; ++iOP){
    corrReadersByOp[iOP] = new BTagCalibrationReader(BTagEntry::OperatingPoint(iOP), "central", {"up","down"});
  }

  // load readers with all the systematics types / measurement types / flavors / operating points
  // "incl" for udsg, "comb" for bc, for CSVv2, per https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco#AK4_jets
  //   and "fastsim" for fastsim
  for(auto reader : corrReadersByOp){                          // (systematics types / operating point)
    if(!isFastSim){
      reader->load(*calib, BTagEntry::FLAV_B,    "comb");       // (measurement type / flavor)
      reader->load(*calib, BTagEntry::FLAV_C,    "comb");
      reader->load(*calib, BTagEntry::FLAV_UDSG, "incl");
    }else{
      reader->load(*calib, BTagEntry::FLAV_B,    "fastsim");
      reader->load(*calib, BTagEntry::FLAV_C,    "fastsim");
      reader->load(*calib, BTagEntry::FLAV_UDSG, "fastsim");
    }
  }

  // load efficiencies
  std::clog << "[BTag Corrections] Loading Eff File: "<< effInput << std::endl;

  effFile = TFile::Open(effInput,"read");
  if(!effFile) throw std::invalid_argument("BTagByEvtWeightCorr::BTagByEvtWeightCorr: Eff file could not be found!");

  eff = (const QuickRefold::TH1FContainer*)(effFile->Get("CSVEff") );
  if(!eff)     throw std::invalid_argument("BTagByEvtWeightCorr::BTagByEvtWeightCorr: Eff histo could not be found!");
}

BTagByEvtWeightCorr::~BTagByEvtWeightCorr(){
  delete calib;
  for(auto reader : corrReadersByOp){ delete reader; }
  if(effFile) { effFile->Close(); delete effFile; }
}

int BTagByEvtWeightCorr::flavorToOurHistBin(JetFlavorInfo::JetFlavor flavor) const {
  // these are the histogram bins we use for each of our flavors...
  int effFlavor = 0;
  switch (flavor){
  case JetFlavorInfo::g_jet:
  case JetFlavorInfo::ps_g_jet:
    effFlavor = 3;
    break;
  case JetFlavorInfo::uds_jet:
  case JetFlavorInfo::ps_uds_jet:
    effFlavor = 2;
    break;
  case JetFlavorInfo::c_jet:
    effFlavor = 1;
    break;
  case JetFlavorInfo::b_jet:
    effFlavor = 0;
    break;
  default:
    effFlavor = 4;
  }
  return effFlavor;
}

double BTagByEvtWeightCorr::getJetEff(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor, defaults::CSVWPs wp, bool isTtbarLike) const {
  eff->setBin(TYPE,isTtbarLike);
  int effFlavor = flavorToOurHistBin(flavor);
  eff->setBin(FLAVOR,effFlavor);
  eff->setBin(ETA,TMath::Abs(jetETA));
  eff->setBin(WP,wp);

  const TH1F& effHisto = eff->getValue();
  return effHisto.GetBinContent( std::max(std::min(effHisto.FindFixBin(jetPT),effHisto.GetNbinsX()), 1) );
}

double BTagByEvtWeightCorr::getJetFastSimEff(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor, defaults::CSVWPs wp, bool isTtbarLike) const {
  // discrepancy: below, if isFastSim, isTtbarLike is set to true. But for the eff getters, it's assumed false (0).
  return getJetEff(jetPT, jetETA, flavor, wp, false);
}

double BTagByEvtWeightCorr::getJetEffSF(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor,defaults::CSVWPs wp, CORRTYPE systType) const {
  if(systType == NONE) return 1;
  if(TMath::Abs(jetETA) > 2.4) return 0; // reader.eval / eval_auto_bounds should do this anyway.

  // map our enum JetFlavorInfo::JetFlavor to their BTagEntry::FLAV_X flavors...
  BTagEntry::JetFlavor sfFlavor = BTagEntry::FLAV_UDSG; // covers several cases: JetFlavorInfo::{g,ps_g,uds,ps_uds}
  if(     flavor == JetFlavorInfo::c_jet){ sfFlavor = BTagEntry::FLAV_C; }
  else if(flavor == JetFlavorInfo::b_jet){ sfFlavor = BTagEntry::FLAV_B; }

  // map our enum defaults::CSVWPs to their BTagEntry::OP_X operating points (currently they match...)
  BTagEntry::OperatingPoint sfOp = BTagEntry::OP_MEDIUM;
  if(     wp == defaults::CSVL){ sfOp = BTagEntry::OP_LOOSE;  }
  else if(wp == defaults::CSVM){ sfOp = BTagEntry::OP_MEDIUM; }
  else if(wp == defaults::CSVT){ sfOp = BTagEntry::OP_TIGHT;  }
  else{ throw std::range_error("BTagByEvtWeightCorr::getJetEffSF: wp from defaults:: does not match any operating point from csv reader!"); }

  const BTagCalibrationReader * reader = corrReadersByOp[sfOp];
  if(systType == DOWN || systType == UP){
    return reader->eval_auto_bounds("up", sfFlavor, jetETA, jetPT); // eval_auto_bounds automatically doubles syst error if pT is out-of-bounds
  }
  return reader->eval_auto_bounds("central", sfFlavor, jetETA, jetPT);
}

double BTagByEvtWeightCorr::getJetFastSimEffSF(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor,defaults::CSVWPs wp, CORRTYPE systType) const {
  // no longer special cases to read fastsim file. just "fastsim" measurement type.
  return getJetEffSF(jetPT, jetETA, flavor, wp, systType);
}

double BTagByEvtWeightCorr::getJetWeight(const BaseTreeAnalyzer * ana, const RecoJetF* j, CORRTYPE lightCorrType, CORRTYPE heavyCorrType, bool isTtbarLike) const {
  // formula written in header
  bool dbg = false;

  // get correction type from jet's matched gen flavor
  JetFlavorInfo::JetFlavor flavor = j->genJet() ? JetFlavorInfo::JetFlavor(j->genJet()->flavor()) : JetFlavorInfo::unmatched_jet;
  CORRTYPE corrType = (flavor == JetFlavorInfo::c_jet || flavor == JetFlavorInfo::b_jet ) ?  heavyCorrType : lightCorrType;

  // iterate through our WPs from -1 (everything passes) to TIGHT
  for(int iWP = -1; iWP <= defaults::CSVT; iWP++ ){
    if(dbg) std::cout << "[BTagByEvtWeightCorr::getJetWeight] current WP loop, searching for threshold: " << iWP << std::endl;

    // find the 'threshold of working points' for this jet: 
    //   loosest WP such that jet passes it but not the next WP, and call these WPs "L" and "H"
    if(iWP != defaults::CSVT && j->csv() >= defaults::CSVValues[iWP+1]) continue;
    double effL, effH, sfL, sfH;

    if(dbg) std::cout << "[BTagByEvtWeightCorr::getJetWeight] Threshold for this jet " << 
      TString::Format("of pt %.3f, eta %.3f, csv %.3f is passing (L) %d but not (H) %d", j->pt(), j->absEta(), j->csv(), iWP, iWP+1) << std::endl;

    // fetch eff and sf for L and H WPs
    if(iWP != -1){
      effL = (this->*effGetter)(j->pt(),j->eta(),flavor, defaults::CSVWPs(iWP), isTtbarLike);
      sfL  = (this->*sfGetter )(j->pt(),j->eta(),flavor, defaults::CSVWPs(iWP), corrType);
    }else{
      // special L term for WP -1 is of the form [1-Eff], at tail of formula. so just say effL*sfL = 1
      effL = 1; sfL = 1;
    }

    if(iWP != defaults::CSVT){
      effH = (this->*effGetter)(j->pt(),j->eta(),flavor, defaults::CSVWPs(iWP+1), isTtbarLike);
      sfH  = (this->*sfGetter )(j->pt(),j->eta(),flavor, defaults::CSVWPs(iWP+1), corrType);
    }else{
      // special H term for WP TIGHT is of the form [Eff], at front of formula. so just say effH*sfH = 0
      effH = 0; sfH = 0;
    }
    if(dbg) std::cout << "[BTagByEvtWeightCorr::getJetWeight] fetched effs, sfs: " << TString::Format("pt %.3f eta %.3f flavor %d effL %.3f effH %.3f sfL %.3f sfH %.3f",j->pt(),j->eta(),flavor,effL,effH,sfL,sfH) << std::endl;

    // make the terms for this jet in P(MC) and P(DATA)
    double mcEff   = effL - effH;
    double dataEff = effL*sfL - effH*sfH;
    if(dbg) std::cout << "[BTagByEvtWeightCorr::getJetWeight] " << TString::Format("mcEff %.3f dataEff %.3f", mcEff, dataEff) << std::endl;

    // sanity check
    if( (mcEff <= 0) || (mcEff > 1) || (dataEff <= 0) || (dataEff > 1) ){
      std::clog << std::endl << "[BTagByEvtWeightCorr::getJetWeight] Sth wrong with this term. Eff or SF wonky, must throw exception. Check these: " << std::endl << 
        TString::Format("run %d / lumi %d / event %d / j pT %.3f / WP %d / effL %.3f / effH %.3f / sfL %.3f / sfH %.3f", ana->run, ana->lumi, ana->event, j->pt(), iWP, effL, effH, sfL, sfH) << std::endl;
      //throw std::range_error("");
    }

    return dataEff/mcEff;
  }// WP loop

  assert(!"possible in BTagByEvtWeightCorr::getJetWeight");
  return -1;
}


double BTagByEvtWeightCorr::getEvtWeight(const BaseTreeAnalyzer * ana, const std::vector<RecoJetF*>& jets, CORRTYPE lightCorrType, CORRTYPE heavyCorrType, bool isTtbarLike, double maxETA, double minPT) const {
  bool dbg = false;
  if(dbg) std::cout << "[BTagByEvtWeightCorr::getEvtWeight] starting calculation with " << jets.size() << " jets" << std::endl;

  double btagWeight = 1;
  for(const auto * j : jets){
    if(dbg) std::cout << "[BTagByEvtWeightCorr::getEvtWeight] " << TString::Format("jet pt %.3f, eta %.3f, current evt weight %.3f",j->pt(),j->absEta(),btagWeight) << std::endl;
    if(j->absEta() > maxETA) continue;
    if(j->pt() < minPT) continue;
    btagWeight *= getJetWeight(ana, j,lightCorrType,heavyCorrType,isTtbarLike);
  }
  if(dbg) std::cout << "[BTagByEvtWeightCorr::getEvtWeight] final evt weight " << btagWeight << std::endl;
  if(dbg) std::cout << "[BTagByEvtWeightCorr::getEvtWeight] ******** END OF EVENT ********** " << std::endl << std::endl;
  return btagWeight;
}

void BTagCorrectionSet::load(TString effFileName,TString sfFileName,TString fastSimEffFileName,TString fastSimSfFileName, int correctionOptions){
  loadSimple("BTagCorrection",correctionOptions);

  if(options_ & BYEVTWEIGHT){
    bTagByEvtWeightCorr = new BTagByEvtWeightCorr(effFileName,sfFileName,false);
    corrections.push_back(bTagByEvtWeightCorr);
  }

  if(options_ & FASTSIMBYEVTWEIGHT){
    bTagFastSimByEvtWeightCorr = new BTagByEvtWeightCorr(fastSimEffFileName,fastSimSfFileName,true);
    corrections.push_back(bTagFastSimByEvtWeightCorr);
  }
}

void BTagCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {
  bool dbg = false;

  bTagByEvtWeight = 1;
  bTagFastSimByEvtWeight = 1;
  if(!ana->isMC()) return;

  bool isFastSim = ana->evtInfoReader.isfastsim;
  bool isTtbarLike = ana->process == defaults::TTBAR || 
                     ana->process == defaults::SINGLE_T || 
                     ana->process == defaults::TTZ || 
                     ana->process == defaults::TTW;
  isTtbarLike |= (ana->evtInfoReader.signalType == defaults::T2tt || 
                  ana->evtInfoReader.signalType == defaults::T2bW ||
                  ana->evtInfoReader.signalType == defaults::T2fb ||
                  ana->evtInfoReader.signalType == defaults::T2tb); // not T2cc

  if(options_ & BYEVTWEIGHT){
    if(dbg) std::cout << "[BTagByEvtWeightCorr::processCorrection] Starting non-fastsim event weight" << std::endl;
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    bTagByEvtWeight = bTagByEvtWeightCorr->getEvtWeight(ana, ana->jets,cfg.corrections.lightBTagCorrType,cfg.corrections.heavyBTagCorrType,isTtbarLike,cfg.jets.maxBJetEta,cfg.jets.minBJetPt);
  }
  if((options_ & FASTSIMBYEVTWEIGHT) && isFastSim){
    if(dbg) std::cout << "[BTagByEvtWeightCorr::processCorrection] Starting fastsim event weight" << std::endl;
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    bTagFastSimByEvtWeight = bTagFastSimByEvtWeightCorr->getEvtWeight(ana, ana->jets,cfg.corrections.lightFastSimBTagCorrType,cfg.corrections.heavyFastSimBTagCorrType,isTtbarLike,cfg.jets.maxBJetEta,cfg.jets.minBJetPt);
  }
}

} /* namespace ucsbsusy */
