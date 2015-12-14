#include "AnalysisBase/TreeAnalyzer/interface/QCDRespSmearingAnalyzer.h"
#include "AnalysisTools/QuickRefold/interface/TObjectContainer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"




namespace JetRespSmear {

//----------------------------------------------------------------------////
SmearOptions::SmearOptions() : minWindow(0),maxWindow(2.0), nSmears(0), nSmearJets(0), nBootstraps(0),winType(FLAT),doFlatSampling(false) {}
std::ostream& operator<<(std::ostream& os, const SmearOptions& a){
  os << "Seting up jet response smearing! Good luck." <<std::endl;
  os << "Window options: ";
  switch(a.winType){
  case JetRespSmear::FLAT:
    os <<"Flat"; break;
  case JetRespSmear::LINEAR_GRANULATED:
    os <<"Scaled"; break;
  case JetRespSmear::SECORDER_GRANULATED:
    os <<"Scaled w/ a second order param"; break;
  }
  os << " ("<<a.minWindow<<","<<a.maxWindow<<") "<<std::endl;
  os << a.nSmears <<" smears, with "<< (a.doFlatSampling ? "flat sampling" : "pdf sampling" ) <<" and "<< a.nBootstraps <<" bootstraps!"<< std::endl;
  os<< "Response functions are taken from: "<< a.respFileName <<" and object name: "<< a.respInputName << std::endl;
  return os;
}


SmearInfo::SmearInfo() : gJ(0),rJI(-1), cdf(0),minProb(0),maxProb(0),minRes(0),maxRes(0) {}
SmearInfo::SmearInfo(const ucsbsusy::GenJetF* gJ, int rJI, const TH1 * cdf, double minProb, double maxProb, double minRes, double maxRes)
  : gJ(gJ),rJI(rJI), cdf(cdf),minProb(minProb),maxProb(maxProb),minRes(minRes),maxRes(maxRes) {}
//----------------------------------------------------------------------////

double interpolateResToProb(const TH1 * cdf, const double res) {
  int bin = cdf->FindFixBin(res);
  if(bin <= 0) return 0;
  double binWidth = cdf->GetBinWidth(bin);
  double b = ((cdf->GetBinWidth(bin) + cdf->GetBinLowEdge(bin))*cdf->GetBinContent(bin -1) - (cdf->GetBinLowEdge(bin))*cdf->GetBinContent(bin)) /binWidth;
  double m = (cdf->GetBinContent(bin) - cdf->GetBinContent(bin-1)) /binWidth;
  return res*m + b;
}

double interpolateProbToRes(const TH1 * cdf, const double prob) {
  int binAbove = cdf->FindFirstBinAbove(prob);
  if(binAbove <= 1) return 0;
  double deltaProb = cdf->GetBinContent(binAbove) - cdf->GetBinContent(binAbove-1);
  double newResValue = cdf->GetBinCenter(binAbove);


  if(deltaProb > 0){
          double b = (cdf->GetBinContent(binAbove) * cdf->GetBinLowEdge(binAbove) - cdf->GetBinContent(binAbove-1) * (cdf->GetBinWidth(binAbove) + cdf->GetBinLowEdge(binAbove)) ) /
              deltaProb;
          double m = cdf->GetBinWidth(binAbove) /deltaProb;

          newResValue = m*prob + b;
  }
  return newResValue;
}

//----------------------------------------------------------------------////

double getScaledWindow(const double res,const double minW,const double maxW){
  if(res < 1)
    return (minW - maxW)*res + maxW;
  return -1*(minW - maxW)*res + 2*minW - maxW;
}

double getUpIntegratedScaledWindow(const double res,const double minW,const double maxW){
  if(res < 1 - getScaledWindow(1.,minW,maxW) ){
    return (res + maxW)/(1 - (minW - maxW));
  } else{
    return (res + 2*minW - maxW)/(1 + (minW - maxW));
  }
}

double getLowIntegratedScaledWindow(const double res,const double minW,const double maxW){
  if(res > 1 + getScaledWindow(1.,minW,maxW) ){
    return (res - (2*minW - maxW))/(1 - (minW - maxW));
  } else{
    return (res - maxW)/(1 + (minW - maxW));
  }
}

//----------------------------------------------------------------------////

double getScaledWindowPol2(const double res,const double minW,const double maxW){
  double A = maxW;
  double B = -.95;
  double C = minW - A - B;
  double x = res < 1 ? res : 2 - res;
return A +B*x + C*x*x;
}

double getUpIntegratedScaledWindowPol2(const double res,const double minW,const double maxW){

  double A = maxW;
  double B = -.95;
  double C = minW - A - B;
  double x = res;

  if(res < 1 - getScaledWindow(1.,minW,maxW)){
    return  ( 1 - B - sqrt(1 -2*B +B*B -4*C*A -4*C*x))/(2*C);
  } else {
    if(.00001 > 1 +2*B +B*B -4*C*A -4*C*x + 8*C ) return 2;
    return ( 1 + B + 4*C - sqrt(1 +2*B +B*B -4*C*A -4*C*x + 8*C))/(2*C);
  }

}
double getLowIntegratedScaledWindowPol2(const double res,const double minW,const double maxW){

  double A = maxW;
  double B = -.95;
  double C = minW - A - B;
  double  x = res;

  if(res < 1 + getScaledWindow(1.,minW,maxW)){
    if(1 + 2*B +B*B -4*C*A +4*C*x <= (-1 - B)*(-1 - B) +.000001 ) return 0;
    return ( -1 - B + sqrt(double(1 + 2*B +B*B -4*C*A +4*C*x)))/(2*C);
  } else {
    return ( -1 + B + 4*C + sqrt(1 - 2*B +B*B -4*C*A +4*C*x - 8*C))/(2*C);

  }


}

//----------------------------------------------------------------------////
void getWindowProb(const TH1 * cdf, double& minProb, double& maxProb, double& minRes,double& maxRes){
minRes = std::max(0.0001, minRes);
maxRes = std::min(1.9999, maxRes);

if(minRes >= maxRes) {
  minProb = 0;
  maxProb = 0;
  return;
}

minProb = interpolateResToProb(cdf,minRes);
maxProb = interpolateResToProb(cdf,maxRes);
}
void getConstWindowAndProb(const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb, double& minRes, double& maxRes){
minRes = res - maxWindow;
maxRes = res + maxWindow;
getWindowProb(cdf,minProb,maxProb,minRes,maxRes);
}
void getScaledWindowAndProb(const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb, double& minRes, double& maxRes){
double window = getScaledWindow(res,minWindow,maxWindow);
minRes = res - window;
maxRes = res + window;
getWindowProb(cdf,minProb,maxProb,minRes,maxRes);
}
void getScaledWindowAndProbPol2(const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb, double& minRes, double& maxRes){
double window = getScaledWindowPol2(res,minWindow,maxWindow);
minRes = res - window;
maxRes = res + window;
getWindowProb(cdf,minProb,maxProb,minRes,maxRes);
}
void getContributionScaledWindowAndProb(const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb,double& minRes, double& maxRes){
minRes = getLowIntegratedScaledWindow(res,minWindow,maxWindow);
maxRes = getUpIntegratedScaledWindow(res,minWindow,maxWindow);
getWindowProb(cdf,minProb,maxProb,minRes,maxRes);
}
void getContributionScaledWindowAndProbPol2(const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb,double& minRes, double& maxRes){
minRes = getLowIntegratedScaledWindowPol2(res,minWindow,maxWindow);
maxRes = getUpIntegratedScaledWindowPol2(res,minWindow,maxWindow);
getWindowProb(cdf,minProb,maxProb,minRes,maxRes);
}
//----------------------------------------------------------------------////
}


namespace ucsbsusy{

//--------------------------------------------------------------------------------------------------
QCDRespSmearingBaseEventAnalyzer::QCDRespSmearingBaseEventAnalyzer() :  respFile(0),respCont(0), smearWeight(1),doAnything_(false) {
  //default options
  smearOptions.minWindow = 0.01;
  smearOptions.maxWindow = 0.5;
  smearOptions.nSmears = 100;
  smearOptions.nSmearJets = 2;
  smearOptions.nBootstraps = 50;
  smearOptions.winType = JetRespSmear::LINEAR_GRANULATED;
  smearOptions.doFlatSampling = true;
  smearOptions.respFileName = TString::Format("%s/src/data/corrections/qcdResponseTails.root",cfgSet::CMSSW_BASE);;
  smearOptions.respInputName = "JetRes";
}

QCDRespSmearingBaseEventAnalyzer::~QCDRespSmearingBaseEventAnalyzer() {
  if(respFile) respFile->Close();
  delete respFile;
}

void QCDRespSmearingBaseEventAnalyzer::setupSmearing(BaseTreeAnalyzer * analyzer) {
    if(smearOptions.maxWindow > 0 && smearOptions.nSmears > 0 && smearOptions.nSmearJets > 0)
      doAnything_ = true;
    else
      doAnything_ = false;

    std::clog <<std::endl<< smearOptions;
    if(!doAnything())
      std::clog<< "But despite all of this the smearer will do NOTHING!"<< std::endl;
    std::clog <<std::endl;

    respFile = TFile::Open(smearOptions.respFileName,"read");
    if(!respFile) throw std::invalid_argument("QCDRespSmearingAnalyzer::setupSmearing: Response file not found!");
    respCont = (const QuickRefold::TH1FContainer*)(respFile->Get("JetRes") );
    if(!respCont) throw std::invalid_argument("QCDRespSmearingAnalyzer::setupSmearing: Response container not found!");
    bootstrapWeights.resize(smearOptions.nBootstraps);
    for(auto& b : bootstrapWeights) b = 1;
  }



void QCDRespSmearingBaseEventAnalyzer::applySmearing(BaseTreeAnalyzer * analyzer){
  //what we use from the analyzer
  TRandom3 * const randGen = analyzer->getRndGen();
  const auto * genJets =  &analyzer->defaultJets->genJets;
  auto * recoJets =  &analyzer->defaultJets->recoJets;
  auto * met =  &analyzer->evtInfoReader.met;
  auto * weight =  &analyzer->evtInfoReader.evtweight;

  //functions to get windows
  void (*getWinAndProb)(const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb, double& minRes, double& maxRes);
  void (*getContribWinAndProb)(const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb, double& minRes, double& maxRes);

  switch(smearOptions.winType){
  case JetRespSmear::FLAT:
    getWinAndProb =  JetRespSmear::getConstWindowAndProb;
    getContribWinAndProb =  JetRespSmear::getConstWindowAndProb;
    break;
  case JetRespSmear::LINEAR_GRANULATED:
    getWinAndProb =  JetRespSmear::getScaledWindowAndProb;
    getContribWinAndProb =  JetRespSmear::getContributionScaledWindowAndProb;
    break;
  case JetRespSmear::SECORDER_GRANULATED:
    getWinAndProb =  JetRespSmear::getScaledWindowAndProbPol2;
    getContribWinAndProb =  JetRespSmear::getContributionScaledWindowAndProbPol2;
    break;
  default:
    throw std::invalid_argument("QCDRespSmearingBaseEventAnalyzer::applySmearing: Incorrect window type!");
  }

  //fill bootstrap weights
  for(auto& b : bootstrapWeights) b = std::min(255,randGen->Poisson(1));
  smearWeight = 1;

  //get the list of jets we can smear and put them in a vector
  std::vector<JetRespSmear::SmearInfo> smearJets;

  for(unsigned int iJ = 0; iJ < smearOptions.nSmearJets && iJ < genJets->size(); ++iJ){
    const auto* gJ = &(*genJets)[iJ];
    int rJI = -1;
    if(gJ->pt() == 0) break;
    for(unsigned int iR = 0; iR < recoJets->size(); ++iR){
      if((*recoJets)[iR].genJet() != gJ) continue;
      rJI = iR;
      break;
    }

    //its a missing jet...or just below 10 GeV
    if(rJI < 0 ){
      rJI = recoJets->size();
      RecoJetF newJet(ucsbsusy::CylLorentzVectorF(9.5, gJ->eta(), gJ->phi(),gJ->mass()), -1,
          0, 9.5, 0,
          1, const_cast<ucsbsusy::GenJetF*>(gJ));

      analyzer->defaultJets->addRecoJet(&newJet);
    }
    RecoJetF * rJ = &(*recoJets)[rJI];


    double origRes = rJ->pt() / gJ->pt();

    //cutoff out of bounds
    if(origRes < 0 || origRes > 2) continue;

    const auto* cdf = &respCont->getValue(gJ->pt(), gJ->flavor() == JetFlavorInfo::b_jet);

    double minProb, maxProb, minRes, maxRes;
    getWinAndProb(cdf,origRes,smearOptions.minWindow,smearOptions.maxWindow,minProb,maxProb,minRes,maxRes);

    //not good for smearing
    if(maxProb - minProb == 0) continue;
    smearJets.emplace_back(gJ,rJI,cdf,minProb,maxProb,minRes,maxRes);
  }

  // In case of no smearable jets
  if(!smearJets.size()){
    runOneInstance(analyzer);
    return;
  }


  //orignal values
  const RecoJetFCollection originalRecoJets = *recoJets;
  const MomentumF originalMET = (*met);
  const float originalWeight = (*weight);

  bool canSmear = false;


  //now for the smearing
  for(unsigned int iS = 0; iS < smearOptions.nSmears; ++iS){

    for(unsigned int iJ = 0; iJ < smearJets.size(); ++iJ ){
      JetRespSmear::SmearInfo * info = &smearJets[iJ];

      double newResValue = 1;
      if(smearOptions.doFlatSampling){
        newResValue = randGen->Uniform( info->minRes, info->maxRes);
      } else {
        double newResProb  = randGen->Uniform( info->minProb, info->maxProb);
        newResValue = JetRespSmear::interpolateProbToRes(info->cdf,newResProb);
      }

      double minProb2, maxProb2;
      double minRes2, maxRes2;
      getContribWinAndProb(info->cdf,newResValue,smearOptions.minWindow,smearOptions.maxWindow,minProb2,maxProb2,minRes2,maxRes2);

      double contribProb = maxProb2 - minProb2;

      if( contribProb == 0) continue;
      canSmear = true;

      double smearingCorr = 1;
      if(smearOptions.doFlatSampling){
        double deltaMinProb, deltaMaxProb;
        double deltaMinRes = newResValue -.001;
        double deltaMaxRes = newResValue +.001;
        JetRespSmear::getWindowProb(info->cdf,deltaMinProb,deltaMaxProb,deltaMinRes,deltaMaxRes);
        double flatProb = (deltaMaxRes -deltaMinRes)/(info->maxRes - info->minRes);
        double trueProb = deltaMaxProb -deltaMinProb;
        smearingCorr = trueProb / flatProb;
      } else {
        smearingCorr = info->maxProb - info->minProb;
      }
      smearWeight *= smearingCorr / contribProb;

      auto& recoJet = (*recoJets)[info->rJI];

      met->p4() += recoJet.p4();
      ucsbsusy::CylLorentzVectorF newP4(newResValue * info->gJ->pt(),recoJet.eta(),recoJet.phi(),recoJet.mass());
      recoJet.setP4(newP4);
      met->p4() -= recoJet.p4();
    }

    if(canSmear){
      std::sort(recoJets->begin(),recoJets->end(),PhysicsUtilities::greaterPT<RecoJetF>());
      smearWeight /= float(smearOptions.nSmears);
      (*weight) *= smearWeight;
      analyzer->defaultJets->pushToTree(); //update vectors
      analyzer->evtInfoReader.pushToTree(); //update vectors
      runOneInstance(analyzer);
    }

    //Reset values..
    smearWeight  = 1.0;
    (*weight) = originalWeight;
    canSmear = false;
    (*met) = originalMET;
    (*recoJets) = originalRecoJets;
  }
}

void QCDRespSmearingBaseEventAnalyzer::runOneInstance(BaseTreeAnalyzer * ana){
  ana->processVariables();
  ana->runEvent();
}


void QCDRespSmearingBaseEventAnalyzer::analyzeEvent(BaseTreeAnalyzer * ana,int reportFrequency = 10000, int numEvents = -1)
{
  std::clog << "Running over " << (numEvents < 0 ? "all" : TString::Format("at most %i",numEvents).Data()) << " events"  <<std::endl;

  setupSmearing(ana);
  ana->loadVariables();
  ana->setLoaded(true);

  while(ana->nextEvent(reportFrequency)){
    ana->setProcessed(false);
    if(numEvents >= 0 && ana->getEventNumber() >= numEvents) return;
    if(!ana->processData()) continue;
    if(canSmearEvent(ana))
      applySmearing(ana);
    else
      runOneInstance(ana);
  }
}



QCDRespSmearingCopierEventAnalyzer::QCDRespSmearingCopierEventAnalyzer() : QCDRespSmearingBaseEventAnalyzer(), i_smearWeight (0),i_bootstrapWeight (0) {}


void QCDRespSmearingCopierEventAnalyzer::bookSmearingWeights (TreeCopier * analyzer) {
    i_smearWeight          = analyzer->fillingData()->add<float>("","smearWeight"                        ,"F",0);
    i_bootstrapWeight      = analyzer->fillingData()->addMulti<ucsbsusy::size8>("","bootstrapWeight"           ,0);
  }

void QCDRespSmearingCopierEventAnalyzer::fillSmearingWeights(TreeCopier * analyzer) {
  analyzer->fillingData()->fill<float>(i_smearWeight  ,smearWeight);
  for(unsigned int iB = 0; iB < bootstrapWeights.size(); ++iB){
    analyzer->fillingData()->fillMulti<ucsbsusy::size8>(i_bootstrapWeight,bootstrapWeights[iB]);
  }
}

void QCDRespSmearingCopierEventAnalyzer::runOneInstance(BaseTreeAnalyzer * analyzer){
  TreeCopier * newAna = dynamic_cast<TreeCopier*>(analyzer);
  if (newAna==0) throw std::invalid_argument("TreeCopierEventAnalyzer::analyzeEvent: Can only be used in an analyzer that inherits from TreeCopier!");

  newAna->processVariables();
  newAna->resetFillingData();
  if(!newAna->fillEvent()) return;
  fillSmearingWeights(newAna);
  newAna->fillFillingTree();
}


void QCDRespSmearingCopierEventAnalyzer::analyzeEvent(BaseTreeAnalyzer * ana, int reportFrequency, int numEvents){
  TreeCopier * newAna = dynamic_cast<TreeCopier*>(ana);
  if (newAna==0) throw std::invalid_argument("TreeCopierEventAnalyzer::analyzeEvent: Can only be used in an analyzer that inherits from TreeCopier!");

  setupSmearing(ana);
  newAna->loadVariables();
  newAna->setLoaded(true);
  newAna->setupTree();
  newAna->book();
  bookSmearingWeights(newAna);
  newAna->bookFillingTree();
  while(newAna->nextEvent(reportFrequency)){
    newAna->setProcessed(false);
    if(numEvents >= 0 && newAna->getEventNumber() >= numEvents) return;
    if(canSmearEvent(ana))
      applySmearing(ana);
    else
      runOneInstance(ana);
  }
}

}


