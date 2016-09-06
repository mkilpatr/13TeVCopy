

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
    corrReaders(isFastSim ? 1 : INCL +1,
        std::vector<std::vector<BTagCalibrationReader*> > ( defaults::CSVT+1,
            std::vector<BTagCalibrationReader*>(DOWN+1,0)
        )
    ),
    effFile(0),
    eff(0)
{
  //Fill SF reading
  std::clog << "Loading SF File: "<< sfInput<<std::endl;;
  calib = new BTagCalibration("csv",sfInput.Data());
  std::clog << "Loaded: "<< sfInput <<std::endl;


  std::vector<std::string> measTypes;
  if(isFastSim) measTypes.push_back("fastsim");
  else{
    measTypes.push_back("comb");
    measTypes.push_back("incl");
  }

//  for(unsigned int iM = MUJET; iM <= COMB; ++iM ){
  for(unsigned int iM = 0; iM < measTypes.size(); ++iM ){
    for(unsigned int iWP = defaults::CSVL; iWP <= defaults::CSVT; ++iWP ){
      for(unsigned int iCT = NOMINAL; iCT <= DOWN; ++iCT ){
        std::string systType;
        switch (iCT) {
        case NOMINAL:
          systType = "central";
          break;
        case UP:
          systType = "up";
          break;
        case DOWN:
          systType = "down";
          break;
        default:
          throw std::range_error("BTagByEvtWeightCorr::BTagByEvtWeightCorr: Incorrect syst type!");
        }
        corrReaders[iM][iWP][iCT] = new BTagCalibrationReader(calib,BTagEntry::OperatingPoint(iWP),measTypes[iM],systType);
      }
    }
  }
  //END SF READING

  //Load efficiencies
  std::clog << "     Loading Efficiencies File: "<< effInput << std::endl;
  effFile = TFile::Open(effInput,"read");
  if(!effFile) throw std::invalid_argument("BTagByEvtWeightCorr::BTagByEvtWeightCorr: Eff file not found!");
  eff = (const QuickRefold::TH1FContainer*)(effFile->Get("CSVEff") );
  if(!eff) throw std::invalid_argument("BTagByEvtWeightCorr::BTagByEvtWeightCorr: Efficeincy file could not be found!");
}

BTagByEvtWeightCorr::~BTagByEvtWeightCorr(){
  for(auto lay1 : corrReaders)
    for(auto lay2 : lay1)
      for(auto lay3 : lay2)
        delete lay3;
  delete calib;

  if(effFile)
    effFile->Close();
  delete effFile;
}

double BTagByEvtWeightCorr::getJetEff(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor, defaults::CSVWPs wp, bool isTTBARLike) const {
  eff->setBin(TYPE,isTTBARLike);

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

  eff->setBin(FLAVOR,effFlavor);
  eff->setBin(ETA,TMath::Abs(jetETA));
  eff->setBin(WP,wp);

  const TH1F& effHisto = eff->getValue();
  return effHisto.GetBinContent( std::max(std::min(effHisto.FindFixBin(jetPT),effHisto.GetNbinsX()), 1)   );
}

double BTagByEvtWeightCorr::getJetFastSimEff(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor, defaults::CSVWPs wp, bool isTTBARLike) const {
  eff->setBin(TYPE,0  );

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

  eff->setBin(FLAVOR,effFlavor);
  eff->setBin(ETA,TMath::Abs(jetETA));
  eff->setBin(WP,wp);

  const TH1F& effHisto = eff->getValue();
  return effHisto.GetBinContent( std::max(std::min(effHisto.FindFixBin(jetPT),effHisto.GetNbinsX()), 1)   );
}

double BTagByEvtWeightCorr::getJetEffSF(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor,defaults::CSVWPs wp, CORRTYPE sytType) const {

  if(sytType == NONE) return 1;

  BTagEntry::JetFlavor sfFlavor = BTagEntry::FLAV_UDSG;
  if(flavor == JetFlavorInfo::c_jet){
    sfFlavor = BTagEntry::FLAV_C;
  } else if(flavor == JetFlavorInfo::b_jet){
    sfFlavor = BTagEntry::FLAV_B;
  }

  //light jets use incl...b/c use comb
  const BTagCalibrationReader * reader = corrReaders[sfFlavor == BTagEntry::FLAV_UDSG ?  INCL : COMB ] [wp] [sytType];

  //Current SFs are only valid between 30-670 GeV
  double newPT = -1;
  if(sfFlavor == BTagEntry::FLAV_UDSG){
    if(jetPT < 20) newPT = 20;
    else if(jetPT > 1000) newPT = 999;
  } else {
    if(jetPT < 30) newPT = 30;
    else if(jetPT > 670) newPT = 669;
  }
  if(newPT > 0){
    if(sytType == DOWN || sytType == UP){
      const BTagCalibrationReader * readerNominal = corrReaders[sfFlavor == BTagEntry::FLAV_UDSG ?  INCL : COMB ] [wp] [NOMINAL];
      return 2*reader->eval(sfFlavor,jetETA,newPT)- readerNominal->eval(sfFlavor,jetETA,newPT);
    }
    return reader->eval(sfFlavor,jetETA,newPT);
  }
  return reader->eval(sfFlavor,jetETA,jetPT);
}

double BTagByEvtWeightCorr::getJetFastSimEffSF(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor,defaults::CSVWPs wp, CORRTYPE sytType) const {

  if(sytType == NONE) return 1;

  BTagEntry::JetFlavor sfFlavor = BTagEntry::FLAV_UDSG;
  if(flavor == JetFlavorInfo::c_jet){
    sfFlavor = BTagEntry::FLAV_C;
  } else if(flavor == JetFlavorInfo::b_jet){
    sfFlavor = BTagEntry::FLAV_B;
  }

  //light jets use combo...b/c use mujets
  const BTagCalibrationReader * reader = corrReaders[0] [wp] [sytType];

  //If CSV == tight do nothing for light jets
  if(wp == defaults::CSVT && sfFlavor == BTagEntry::FLAV_UDSG) return 1;
  //IF CSV == tight give b-jet correction for c-jets
  if(wp == defaults::CSVT && sfFlavor == BTagEntry::FLAV_C) sfFlavor = BTagEntry::FLAV_B;

  //Current SFs are only valid between 20-800 GeV
  double newPT = -1;
  if(jetPT <= 20) newPT = 20;
  else if(jetPT >= 800) newPT = 799;
  if(newPT > 0){
    if(sytType == DOWN || sytType == UP){
      const BTagCalibrationReader * readerNominal = corrReaders[0] [wp] [NOMINAL];
      return 2*reader->eval(sfFlavor,jetETA,newPT)- readerNominal->eval(sfFlavor,jetETA,newPT);
    }
    return reader->eval(sfFlavor,jetETA,newPT);
  }
  return reader->eval(sfFlavor,jetETA,jetPT);
}


double BTagByEvtWeightCorr::getJetWeight(const RecoJetF* j, CORRTYPE lightCorrType, CORRTYPE heavyCorrType, bool isTTBARLike  ) const {
  for(int iT = -1; iT <= defaults::CSVT; iT++ ){
    //if next tagger is tagged move on!
    if(iT != defaults::CSVT && j->csv() >= defaults::CSVValues[iT+1]) continue;

    JetFlavorInfo::JetFlavor flavor = j->genJet() ? JetFlavorInfo::JetFlavor(j->genJet()->flavor()) : JetFlavorInfo::unmatched_jet;
    CORRTYPE corrType =  (flavor == JetFlavorInfo::c_jet || flavor == JetFlavorInfo::b_jet ) ?  heavyCorrType : lightCorrType;
    if(corrType == CORRTYPE::NONE) return 1;

    //if iT == -1 low == 100% with no SF;
    double effL = iT == -1 ? 1 :  (this->*effGetter)(j->pt(),j->eta(),flavor, defaults::CSVWPs(iT),isTTBARLike  );
    double sfL =  iT == -1 ? 1 :  (this->*sfGetter)(j->pt(),j->eta(),flavor,defaults::CSVWPs(iT), corrType);

    // if iT == CVT, eff Up = 0, with 0 SF
    double effH = iT == defaults::CSVT  ? 0 : (this->*effGetter)(j->pt(),j->eta(),flavor, defaults::CSVWPs(iT+1),isTTBARLike  );
    double sfH =  iT == defaults::CSVT  ? 0 :  (this->*sfGetter)(j->pt(),j->eta(),flavor,defaults::CSVWPs(iT+1), corrType);

    double mcEff = effL - effH;
    double dataEff = effL*sfL - effH*sfH;

    if(mcEff <= 0){
      std::cout<<std::endl << *j <<" "<< JetFlavorInfo::jetFlavorName(flavor) <<" "<< j->csv()
        << " ("<< effL<<","<< sfL<<","<< effH<<","<< sfH<<")"<< std::endl;
      throw std::range_error("BTagByEvtWeightCorr::getJetWeight: <= 0 MC efficiency!");

    }

    if(mcEff > 1){
        std::cout<<std::endl << *j <<" "<< JetFlavorInfo::jetFlavorName(flavor) <<" "<< j->csv()
          << " ("<< effL<<","<< sfL<<","<< effH<<","<< sfH<<")"<< std::endl;
        throw std::range_error("BTagByEvtWeightCorr::getJetWeight: > 1 MC efficiency!");
    }

    if(dataEff <= 0){
      std::cout<<std::endl << *j <<" "<< JetFlavorInfo::jetFlavorName(flavor) <<" "<< j->csv()
        << " ("<< effL<<","<< sfL<<","<< effH<<","<< sfH<<")"<< std::endl;
      throw std::range_error("BTagByEvtWeightCorr::getJetWeight: <= 0 data efficiency!");
    }

    if(dataEff > 1){
      std::cout<<std::endl << *j <<" "<< JetFlavorInfo::jetFlavorName(flavor) <<" "<< j->csv()
        << " ("<< effL<<","<< sfL<<","<< effH<<","<< sfH<<")"<< std::endl;
      throw std::range_error("BTagByEvtWeightCorr::getJetWeight: > 1 data efficiency!");
    }

//    std::cout << corrType <<" -> "<< j->pt() <<" "<< j->absEta() <<" "<< flavor<<" "<<  j->csv()  <<" ("<< effL <<","<<effH<<") ("<< sfL<<","<<sfH<<") "<<dataEff/mcEff<<std::endl;
    return dataEff/mcEff;
  }

  throw std::range_error("BTagByEvtWeightCorr::getJetWeight: We should have never left this loop!");
  return -1;
}


double BTagByEvtWeightCorr::getEvtWeight(const std::vector<RecoJetF*>& jets, CORRTYPE lightCorrType, CORRTYPE heavyCorrType, bool isTTBARLike, double maxETA, double minPT  ) const {
  double btagWeight = 1;
  for(const auto * j : jets){
    if(j->absEta() > maxETA) continue;
    if(j->pt() < minPT) continue;
    btagWeight *= getJetWeight(j,lightCorrType,heavyCorrType,isTTBARLike);
  }
  return btagWeight;
}


void BTagCorrectionSet::load(TString effFileName,TString sfFileName,TString fastSimEffFileName,TString fastSimSfFileName,   int correctionOptions)
{
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

  bTagByEvtWeight =1;
  bTagFastSimByEvtWeight = 1;
  if(!ana->isMC()) return;

  if(options_ & BYEVTWEIGHT){
    bool isTTBARLike = false;
    if(ana->process == defaults::TTBAR || ana->process == defaults::SINGLE_T || ana->process == defaults::TTZ || ana->process == defaults::TTW || ana->process == defaults::SIGNAL )
      isTTBARLike = true;
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    bTagByEvtWeight = bTagByEvtWeightCorr->getEvtWeight(ana->jets,cfg.corrections.lightBTagCorrType,cfg.corrections.heavyBTagCorrType,isTTBARLike,cfg.jets.maxBJetEta,cfg.jets.minBJetPt);
  }
  if((options_ & FASTSIMBYEVTWEIGHT) && (ana->process == defaults::SIGNAL)  ){
    const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
    bTagFastSimByEvtWeight = bTagFastSimByEvtWeightCorr->getEvtWeight(ana->jets,cfg.corrections.lightFastSimBTagCorrType,cfg.corrections.heavyFastSimBTagCorrType,true,cfg.jets.maxBJetEta,cfg.jets.minBJetPt);
  }

}
} /* namespace ucsbsusy */
