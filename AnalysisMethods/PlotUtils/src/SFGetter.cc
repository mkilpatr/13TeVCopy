#include "AnalysisMethods/PlotUtils/interface/SFGetter.hh"
#include <assert.h>
#include <iostream>


namespace SFGetter {

PseudoVectorGausGenerator::PseudoVectorGausGenerator(TTree * dataTree, TTree * bkgTree, const std::vector<double>* bkgSF, const std::vector<double>* bkgSystUnc, const TString selection, const TString dataWeight, TString bkgWeight, const std::vector<TString>& recoCategories,  TRandom3* randGen)
: PseudoGenerator<TVectorD>()
{
  if(recoCategories.size() < 2) throw std::invalid_argument("PseudoVectorGausGenerator::PseudoVectorGausGenerator: Must have at least two categories!");
  if(bkgTree && bkgSF     && bkgSF->size() != recoCategories.size()) throw std::invalid_argument("PseudoVectorGausGenerator::PseudoVectorGausGenerator: BKG SF must have the same dimension as categories!");
  if(bkgTree && bkgSystUnc && bkgSystUnc->size() != recoCategories.size()) throw std::invalid_argument("PseudoVectorGausGenerator::PseudoVectorGausGenerator: BKG syst unc must have the same dimension as categories!");
  rand = randGen;
  nDim = recoCategories.size();
  result   = new TVectorD(nDim);
  smearResult   = new TVectorD(nDim);
  sumW2   = new TVectorD(nDim);
  error = new TVectorD(nDim);
  fillData(dataTree,bkgTree,bkgSF,bkgSystUnc,selection,dataWeight,bkgWeight,recoCategories);

}
PseudoVectorGausGenerator::~PseudoVectorGausGenerator() {
    delete result;
    delete smearResult;
    delete sumW2;
    delete error;
  }
const TVectorD* PseudoVectorGausGenerator::generatePseudo() const {
  for(unsigned int iR = 0; iR < nDim; ++iR){
      (*smearResult)(iR) = rand->Gaus((*result)(iR),(*error)(iR));
      if((*smearResult)(iR) < 0) (*smearResult)(iR) = -1 * (*smearResult)(iR);
  }
  return smearResult;
}

void PseudoVectorGausGenerator::fillOneSet(TTree *tree, const TString selection,const TString weight, const std::vector<TString>& recoCategories, TVectorD& counts, TVectorD& sumW2s ){
  //prepare TTree formulas
  TTreeFormula* selectionFormula = new TTreeFormula("selectionFormula",TString::Format("%s*(%s)",weight.Data(),selection.Data()),tree);
  std::vector<TTreeFormula*> recoFormulas; recoFormulas.reserve(nDim);
  for(unsigned int iF = 0; iF<nDim; ++iF){
    recoFormulas.push_back(new TTreeFormula(TString::Format("recoFormula_%u",iF),recoCategories[iF],tree));
    counts[iF] = 0;
    sumW2s[iF] = 0;
  }

  int nEntries = tree->GetEntries();
  for (int i= 0;i<nEntries;i++) {
   tree->GetEntry(i);
   double weightV   = selectionFormula->EvalInstance();
   if(weightV == 0) continue;
   int recoBin = -1;
   for(unsigned int iS = 0; iS < nDim; ++iS){
     if(recoFormulas[iS]->EvalInstance() > 0 ){
       if(recoBin != -1) throw std::invalid_argument("PseudoVectorGausGenerator::fillOneSet: Event passed more than one reco category!");
       recoBin = iS;
     }
   }
   if(recoBin < 0) continue;
   (counts)(recoBin) += weightV;
   (sumW2s)(recoBin) += weightV*weightV;
  }
  delete selectionFormula;
  for(auto* i : recoFormulas) delete i;
}

void PseudoVectorGausGenerator::fillData(TTree * dataTree, TTree * bkgTree, const std::vector<double>* bkgSF, const std::vector<double>* bkgSystUnc,const TString selection, const TString dataWeight, TString bkgWeight, const std::vector<TString>& recoCategories){

  fillOneSet(dataTree,selection,dataWeight,recoCategories,*result,*sumW2);

  if(bkgTree){
    TVectorD bkgCounts(nDim);
    TVectorD bkgSumW2s(nDim);
    fillOneSet(bkgTree,selection,bkgWeight,recoCategories,bkgCounts,bkgSumW2s);
    for(unsigned int i = 0; i < nDim; ++i){
      double SF  = bkgSF ? bkgSF->at(i) : 1;
      double SFUnc  = bkgSystUnc ? bkgSystUnc->at(i) : 0;
//        cout << (*sumW2)[i] <<" "<< bkgSumW2s[i] <<" "<< bkgSumW2s[i] * SF*SF <<" "<< bkgCounts[i]*bkgCounts[i]*SFUnc*SFUnc <<endl;
      (*sumW2)[i] += bkgSumW2s[i] * SF*SF + bkgCounts[i]*bkgCounts[i]*SFUnc*SFUnc;
      (*result)[i] -= bkgCounts[i]*SF;
      if((*result)[i] < 0) throw std::invalid_argument("PseudoVectorGausGenerator::fillData: BKG subtraction leads to negative data counts!!!!");
    }
  }

  for(unsigned int iR = 0; iR < nDim; ++iR) (*error)(iR) = TMath::Sqrt((*sumW2)(iR));
}

PseudoMatrixGausGenerator::PseudoMatrixGausGenerator(TTree * tree, const TString selection, const TString weight, const std::vector<TString>& recoCategories, const std::vector<TString>& genCategories, TRandom3* randGen)
  : PseudoGenerator<TMatrixD>()
  {
    if(recoCategories.size() != genCategories.size()) throw std::invalid_argument("PseudoMatrixGausGenerator::PseudoMatrixGausGenerator: # of gen categories must equal reco categories!");
    if(recoCategories.size() < 2) throw std::invalid_argument("PseudoMatrixGausGenerator::PseudoMatrixGausGenerator: Must have at least two categories!");
    rand = randGen;
    nDim = recoCategories.size();
    result   = new TMatrixD(nDim,nDim);
    smearResult   = new TMatrixD(nDim,nDim);
    sumW2   = new TMatrixD(nDim,nDim);
    error = new TMatrixD(nDim,nDim);
    fillData(tree,selection,weight,recoCategories,genCategories);

  }
PseudoMatrixGausGenerator::~PseudoMatrixGausGenerator() {
  delete result;
  delete smearResult;
  delete sumW2;
  delete error;
}
const TMatrixD* PseudoMatrixGausGenerator::generatePseudo() const {
  for(unsigned int iR = 0; iR < nDim; ++iR)
    for(unsigned int iG = 0; iG < nDim; ++iG){
      (*smearResult)(iR,iG) = rand->Gaus((*result)(iR,iG),(*error)(iR,iG));
      if((*smearResult)(iR,iG) < 0) (*smearResult)(iR,iG) = -1 * (*smearResult)(iR,iG);
  }
  return smearResult;
}
void PseudoMatrixGausGenerator::fillData(TTree * tree, const TString selection, const TString weight,  const std::vector<TString>& recoCategories, const std::vector<TString>& genCategories){
  //prepare TTree formulas
  TTreeFormula* selectionFormula = new TTreeFormula("selectionFormula",TString::Format("%s*(%s)",weight.Data(),selection.Data()),tree);
  std::vector<TTreeFormula*> recoFormulas; recoFormulas.reserve(nDim);
  std::vector<TTreeFormula*> genFormulas; genFormulas.reserve(nDim);
  for(unsigned int iF = 0; iF<nDim; ++iF){
    recoFormulas.push_back(new TTreeFormula(TString::Format("recoFormula_%u",iF),recoCategories[iF],tree));
    genFormulas.push_back(new TTreeFormula(TString::Format("genFormula_%u",iF),genCategories[iF],tree));
  }
  int nEntries = tree->GetEntries();
  for (int i= 0;i<nEntries;i++) {
   tree->GetEntry(i);
   double weightV   = selectionFormula->EvalInstance();
   if(weightV == 0) continue;
   int recoBin = -1;
   int genBin = -1;
   for(unsigned int iS = 0; iS < nDim; ++iS){
     if(recoFormulas[iS]->EvalInstance() > 0 ){
       if(recoBin != -1) throw std::invalid_argument("PseudoMatrixGausGenerator::fillData: Event passed more than one reco category!");
       recoBin = iS;
     }
     if(genFormulas[iS]->EvalInstance() > 0 ){
       if(genBin != -1) throw std::invalid_argument("PseudoMatrixGausGenerator::fillData: Event passed more than one gen category!");
       genBin = iS;
     }
   }
   if(recoBin < 0) continue;
   if(genBin < 0) throw std::invalid_argument("PseudoMatrixGausGenerator::fillData: Event passes only a reco bin but no gen bin!");

   (*result)(recoBin,genBin) += weightV;
   (*sumW2)(recoBin,genBin) += weightV*weightV;
  }

  for(unsigned int iR = 0; iR < nDim; ++iR)
    for(unsigned int iG = 0; iG < nDim; ++iG){
      (*error)(iR,iG) = TMath::Sqrt((*sumW2)(iR,iG));
  }

  //cleanup;
  delete selectionFormula;
  for(auto* i : recoFormulas) delete i;
  for(auto* i : genFormulas) delete i;
}

PseudoMatrixBootstrapGenerator::PseudoMatrixBootstrapGenerator(TTree * tree, const TString selection, const TString weight, const std::vector<TString>& recoCategories, const std::vector<TString>& genCategories, const int nBootstraps, const TString bootstrapWeight)
  : PseudoGenerator<TMatrixD>()
  {
    if(recoCategories.size() != genCategories.size()) throw std::invalid_argument("PseudoMatrixBootstrapGenerator::PseudoMatrixBootstrapGenerator: # of gen categories must equal reco categories!");
    if(recoCategories.size() < 2) throw std::invalid_argument("PseudoMatrixBootstrapGenerator::PseudoMatrixBootstrapGenerator: Must have at least two categories!");
    nDim = recoCategories.size();
    nBS = nBootstraps;
    currBS = 0;
    originalResult   = new TMatrixD(nDim,nDim);
    bootstrapResults = new std::vector<TMatrixD>(nBS,TMatrixD(nDim,nDim));
    fillData(tree,selection,weight,recoCategories,genCategories,bootstrapWeight);

  }
PseudoMatrixBootstrapGenerator::~PseudoMatrixBootstrapGenerator() {
  delete originalResult;
  delete bootstrapResults;
}
const TMatrixD* PseudoMatrixBootstrapGenerator::generatePseudo() const {
  ++currBS;
  if(currBS > nBS) throw std::invalid_argument("PseudoMatrixBootstrapGenerator::generatePseudo: Asking for more bootstraps than I have!");
  return &bootstrapResults->at(currBS-1);
}
void PseudoMatrixBootstrapGenerator::fillData(TTree * tree, const TString selection, const TString weight,  const std::vector<TString>& recoCategories, const std::vector<TString>& genCategories, const TString bootstrapWeight){
  //prepare TTree formulas
  TTreeFormula* selectionFormula = new TTreeFormula("selectionFormula",TString::Format("%s*(%s)",weight.Data(),selection.Data()),tree);
  std::vector<TTreeFormula*> recoFormulas; recoFormulas.reserve(nDim);
  std::vector<TTreeFormula*> genFormulas; genFormulas.reserve(nDim);
  for(unsigned int iF = 0; iF<nDim; ++iF){
    recoFormulas.push_back(new TTreeFormula(TString::Format("recoFormula_%u",iF),recoCategories[iF],tree));
    genFormulas.push_back(new TTreeFormula(TString::Format("genFormula_%u",iF),genCategories[iF],tree));
  }
  std::vector<TTreeFormula*> bsFomulas; bsFomulas.reserve(nBS);
  for(unsigned int iF = 0; iF<nBS; ++iF){
    bsFomulas.push_back(new TTreeFormula(TString::Format("bsFormula_%u",iF),TString::Format("%s[%u]",bootstrapWeight.Data(),iF),tree));
  }
  int nEntries = tree->GetEntries();
  for (int i= 0;i<nEntries;i++) {
   tree->GetEntry(i);
   double weightV   = selectionFormula->EvalInstance();
   if(weightV == 0) continue;
   int recoBin = -1;
   int genBin = -1;
   for(unsigned int iS = 0; iS < nDim; ++iS){
     if(recoFormulas[iS]->EvalInstance() > 0 ){
       if(recoBin != -1) throw std::invalid_argument("PseudoMatrixBootstrapGenerator::fillData: Event passed more than one reco category!");
       recoBin = iS;
     }
     if(genFormulas[iS]->EvalInstance() > 0 ){
       if(genBin != -1) throw std::invalid_argument("PseudoMatrixBootstrapGenerator::fillData: Event passed more than one gen category!");
       genBin = iS;
     }
   }
   if(recoBin < 0) continue;
   if(genBin < 0) throw std::invalid_argument("PseudoMatrixBootstrapGenerator::fillData: Event passes only a reco bin but no gen bin!");

   (*originalResult)(recoBin,genBin) += weightV;

   for(unsigned int iB = 0; iB < nBS; ++iB){
     bsFomulas[iB]->GetNdata();
     double bsWeight = bsFomulas[iB]->EvalInstance();
     if(bsWeight > 0) (*bootstrapResults)[iB](recoBin,genBin) += weightV*bsWeight;
   }
  }
  //cleanup;
  delete selectionFormula;
  for(auto* i : recoFormulas) delete i;
  for(auto* i : genFormulas) delete i;
  for(auto* i : bsFomulas) delete i;
}

MatrixSolver::MatrixSolver(const PseudoGenerator<TVectorD>* dataGenerator, const PseudoGenerator<TMatrixD>* mcGenerator,int nIterations) :
  nIt(nIterations),
  dataGenerator(dataGenerator),
  mcGenerator(mcGenerator),
  nDim(dataGenerator->getNDim()),
  sf(new TVectorD(nDim) ),
  sfError(new TVectorD(nDim) ),
  sfDataError(new TVectorD(nDim) ),
  sfMCError(new TVectorD(nDim) ),
  covMatrix(new TMatrixD(nDim,nDim) )
  {
  if(nDim != mcGenerator->getNDim()) throw std::invalid_argument("MatrixSolver::MatrixSolver: # of MC and data categories must agree");
  std::cout << "The attempting to solve: "<< std::endl;
  std::cout << "Data: "<< std::endl;
  dataGenerator->getOriginal()->Print();
  std::cout << "MC: "<< std::endl;
  mcGenerator->getOriginal()->Print();
  solve();
  }
MatrixSolver::~MatrixSolver(){
  delete sf;
  delete sfError;
  delete sfDataError;
  delete sfMCError;
  delete covMatrix;
}
bool MatrixSolver::solve(){
  TDecompLU solver(*mcGenerator->getOriginal());
  (*sf) = (*dataGenerator->getOriginal());
  if(!solver.Solve(*sf)){
    std::cout << "The original matrix can not be solved! "<< std::endl;
    std::cout << "Exiting! "<< std::endl;
    return false;
  }
  sf->Print();

  TMatrixD dataCovMatrix(nDim,nDim);
  getDataError(&solver,dataCovMatrix);
  (*covMatrix) += dataCovMatrix;

  TMatrixD mcCovMatrix(nDim,nDim);
  getMCError(mcCovMatrix);
  (*covMatrix) += mcCovMatrix;

  std::cout << "Data Cov matrix: "<< std::endl;
  dataCovMatrix.Print();
  std::cout << "MC Cov matrix: "<< std::endl;
  mcCovMatrix.Print();

  for(unsigned int iR = 0; iR < nDim; ++iR){ (*sfError)(iR)  = TMath::Sqrt((*covMatrix)(iR,iR));}
  for(unsigned int iR = 0; iR < nDim; ++iR){ (*sfDataError)(iR)  = TMath::Sqrt((dataCovMatrix)(iR,iR));}
  for(unsigned int iR = 0; iR < nDim; ++iR){ (*sfMCError)(iR)  = TMath::Sqrt((mcCovMatrix)(iR,iR));}

  std::cout << "Total errors: "<< std::endl;
  sfError->Print();

  return true;
}
void MatrixSolver::getDataError(TDecompBase* solver,TMatrixD& errorCov){
  TVectorD meanSF(nDim);
  TMatrixD meanSqSF(nDim,nDim);
  TVectorD newSF(nDim);

  unsigned int maxIt = dataGenerator->getMaxIterations() < 0 ? nIt : std::min(int(nIt),dataGenerator->getMaxIterations());
  int nComplete = 0;
  for(unsigned int iI = 0; iI < maxIt; ++iI){
    newSF = *dataGenerator->generatePseudo();
    bool pass = solver->Solve(newSF);
    assert(pass);
    nComplete++;

    for(unsigned int iR = 0; iR < nDim; ++iR){
      meanSF(iR) += newSF(iR);
      for(unsigned int iC = 0; iC < nDim; ++iC){
        meanSqSF(iR,iC) += newSF(iR)*newSF(iC);
      }
    }
  }
  meanSF *= 1/double(nComplete);
  meanSqSF *= 1/double(nComplete);
  for(unsigned int iR = 0; iR < nDim; ++iR){
    for(unsigned int iC = 0; iC < nDim; ++iC){
      errorCov(iR,iC) = meanSqSF(iR,iC) - meanSF(iR)*meanSF(iC);
    }
  }
}
void MatrixSolver::getMCError(TMatrixD& errorCov){
  TVectorD meanSF(nDim);
  TMatrixD meanSqSF(nDim,nDim);
  TVectorD newSF(nDim);
  TMatrixD newMC = *mcGenerator->getOriginal();
  unsigned int maxIt = mcGenerator->getMaxIterations() < 0 ? nIt : std::min(int(nIt),mcGenerator->getMaxIterations());
  int nComplete = 0;
  for(unsigned int iI = 0; iI < maxIt; ++iI){
    newSF = *dataGenerator->getOriginal();
    TDecompLU solver(*mcGenerator->generatePseudo());
    if(!solver.Solve(newSF)) continue;
    nComplete++;
    for(unsigned int iR = 0; iR < nDim; ++iR){
      meanSF(iR) += newSF(iR);
      for(unsigned int iC = 0; iC < nDim; ++iC){
        meanSqSF(iR,iC) += newSF(iR)*newSF(iC);
      }
    }
  }
  if(nComplete < .8*maxIt){
    std::cout << "Less than 80% of iterations could be solved!"<<std::endl;
    std::cout << "I would not trust these errors!"<<std::endl;
  }
  if(!nComplete) return;

  meanSF *= 1/double(nComplete);
  meanSqSF *= 1/double(nComplete);
  for(unsigned int iR = 0; iR < nDim; ++iR){
    for(unsigned int iC = 0; iC < nDim; ++iC){
      errorCov(iR,iC) = meanSqSF(iR,iC) - meanSF(iR)*meanSF(iC);
    }
  }
}

}
