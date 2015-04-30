
#include "AnalysisTools/TreeReader/interface/CORRALReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "ObjectProducers/TopTagging/interface/CORRAL.h"

using namespace std;
using namespace ucsbsusy;

const int CORRALReader::defaultOptions = CORRALReader::NULLOPT;

//--------------------------------------------------------------------------------------------------
CORRALReader::CORRALReader() : BaseReader(){
  bjetind  = new vector<int16>;
  wjet1ind = new vector<int16>;
  wjet2ind = new vector<int16>;
  wmva     = new vector<float>;
  tmva     = new vector<float>;
  corral     = new CORRAL::CORRALData();
}

CORRALReader::~CORRALReader() {delete corral;}

//--------------------------------------------------------------------------------------------------
void CORRALReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") jets with: ";

    treeReader->setBranchAddress(branchName_, "bjetind"       , &bjetind     ,true);
    treeReader->setBranchAddress(branchName_, "wjet1ind"      , &wjet1ind    ,true);
    treeReader->setBranchAddress(branchName_, "wjet2ind"      , &wjet2ind    ,true);
    treeReader->setBranchAddress(branchName_, "wmva"          , &wmva        ,true);
    treeReader->setBranchAddress(branchName_, "tmva"          , &tmva        ,true);

  clog << endl;
}

//--------------------------------------------------------------------------------------------------
void CORRALReader::refresh(){
}

//--------------------------------------------------------------------------------------------------
int CORRALReader::transformIndex(int inIndex, const vector<RecoJetF*>& recoJets, bool forceMatch) const{
  for(unsigned int iJ = 0; iJ <  recoJets.size(); ++iJ)
    if(inIndex == recoJets[iJ]->index())
      return iJ;

  if(forceMatch){
    throw std::range_error(( TString("CORRALReader::transformIndex you gave an invalid index!" )).Data() );
  }

  return -1;
}

//--------------------------------------------------------------------------------------------------
void CORRALReader::getCORRALData(const GenParticleReader * genParticleReader, JetReader * jetReader, const int nPV) const{
  corral->reset();

  const unsigned int nTops = bjetind->size();

  //filter jets
  CORRAL::setup(genParticleReader,jetReader, corral->recoJets,corral->decays);


  //we now need to associate our reco jets with the jet ind
  vector<int> trnBjetind (nTops);
  vector<int> trnWjet1ind(nTops);
  vector<int> trnWjet2ind(nTops);

  for(unsigned int iT = 0; iT < nTops; ++iT){
    trnBjetind[iT]  = transformIndex(bjetind->at(iT) , corral->recoJets, true);
    trnWjet1ind[iT] = transformIndex(wjet1ind->at(iT), corral->recoJets, true);
    trnWjet2ind[iT] = transformIndex(wjet2ind->at(iT), corral->recoJets, true);
  }

  //now fill W cands
  corral->wCands.reserve(nTops);
  for(unsigned int iT = 0; iT < nTops; ++iT){
    CORRAL::addWCandidate(trnWjet1ind[iT],trnWjet2ind[iT],corral->recoJets,corral->decays,corral->wCands);
  }

  //now fill T cands
  corral->tCands.reserve(nTops );
  corral->tCandVars.reserve(nTops);
  for(unsigned int iT = 0; iT < nTops; ++iT){
    CORRAL::addTCandidate(iT,corral->wCands[iT], trnBjetind[iT],corral->recoJets,corral->decays,corral->tCands);
    CORRAL::TCandVars vars;
    vars.wDisc = wmva->at(iT);
    vars.mva = tmva->at(iT);
    corral->tCandVars.push_back(vars);
  }
  vector<RankedIndex> prunedTops;
  for(unsigned int iT = 0; iT < nTops; ++iT){
    prunedTops.emplace_back(tmva->at(iT),iT);
  }

  //now fill pairs
  corral->rankedTPairs = CORRAL::getRankedTopPairs(corral->tCands,corral->tCandVars,prunedTops);

  if(corral->rankedTPairs.size()){
    corral->reconstructedTop = true;
    corral->top1 = &corral->tCands[corral->rankedTPairs[0].first];
    corral->top2 = &corral->tCands[corral->rankedTPairs[0].second];
    corral->top1_disc = corral->tCandVars[corral->rankedTPairs[0].first].mva;
    corral->top2_disc = corral->tCandVars[corral->rankedTPairs[0].second].mva;
  }


}
