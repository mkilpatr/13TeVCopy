#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/QuickRefold/interface/TObjectContainer.h"

#include "TreeReadingHelper.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include<iostream>

#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompSVD.h>
#include <TDecompLU.h>
#include <TRandom3.h>
#include <TMath.h>


//SF getting classes/functions
namespace SFGetter {

template <typename container>
class PseudoGenerator {
public:
  PseudoGenerator() : nDim(0) {};
  virtual ~PseudoGenerator() {};
  virtual const container* getOriginal() const = 0;
  virtual const container* getError() const = 0;
  virtual const container* generatePseudo() const = 0;
  virtual unsigned int getNDim() const {return nDim;}
  virtual int getMaxIterations() const {return -1;}
  virtual void reset() = 0;
protected:
  unsigned int nDim;

};

class PseudoVectorGausGenerator : public PseudoGenerator<TVectorD>{
public:
  PseudoVectorGausGenerator(TTree * dataTree, TTree * bkgTree, const std::vector<double>* bkgSF, const std::vector<double>* bkgSystUnc, const TString selection, const TString dataWeight, TString bkgWeight, const vector<TString>& recoCategories,  TRandom3* randGen)
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
  ~PseudoVectorGausGenerator() {
    delete result;
    delete smearResult;
    delete sumW2;
    delete error;
  }
  virtual const TVectorD* getOriginal() const {return result;}
  virtual const TVectorD* getError() const {return error;}
  virtual const TVectorD* generatePseudo() const {
    for(unsigned int iR = 0; iR < nDim; ++iR){
        (*smearResult)(iR) = rand->Gaus((*result)(iR),(*error)(iR));
        if((*smearResult)(iR) < 0) (*smearResult)(iR) = -1 * (*smearResult)(iR);
    }
    return smearResult;
  }
  virtual void reset() {}


private:

  void fillOneSet(TTree *tree, const TString selection,const TString weight, const vector<TString>& recoCategories, TVectorD& counts, TVectorD& sumW2s ){
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

  void fillData(TTree * dataTree, TTree * bkgTree, const std::vector<double>* bkgSF, const std::vector<double>* bkgSystUnc,const TString selection, const TString dataWeight, TString bkgWeight, const vector<TString>& recoCategories){

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
  TRandom3*  rand;
  TVectorD * result;
  mutable TVectorD * smearResult;
  TVectorD * sumW2;
  TVectorD * error;
};

class PseudoMatrixGausGenerator : public PseudoGenerator<TMatrixD>{
public:
  PseudoMatrixGausGenerator(TTree * tree, const TString selection, const TString weight, const vector<TString>& recoCategories, const vector<TString>& genCategories, TRandom3* randGen)
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
  ~PseudoMatrixGausGenerator() {
    delete result;
    delete smearResult;
    delete sumW2;
    delete error;
  }
  virtual const TMatrixD* getOriginal() const {return result;}
  virtual const TMatrixD* getError() const {return error;}
  virtual const TMatrixD* generatePseudo() const {
    for(unsigned int iR = 0; iR < nDim; ++iR)
      for(unsigned int iG = 0; iG < nDim; ++iG){
        (*smearResult)(iR,iG) = rand->Gaus((*result)(iR,iG),(*error)(iR,iG));
        if((*smearResult)(iR,iG) < 0) (*smearResult)(iR,iG) = -1 * (*smearResult)(iR,iG);
    }
    return smearResult;
  }
  virtual void reset() {}


private:
  void fillData(TTree * tree, const TString selection, const TString weight,  const vector<TString>& recoCategories, const vector<TString>& genCategories){
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
  TRandom3* rand;
  TMatrixD * result;
  mutable TMatrixD * smearResult;
  TMatrixD * sumW2;
  TMatrixD * error;
};
class PseudoMatrixBootstrapGenerator : public PseudoGenerator<TMatrixD>{
public:
  PseudoMatrixBootstrapGenerator(TTree * tree, const TString selection, const TString weight, const vector<TString>& recoCategories, const vector<TString>& genCategories, const int nBootstraps, const TString bootstrapWeight)
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
  ~PseudoMatrixBootstrapGenerator() {
    delete originalResult;
    delete bootstrapResults;
  }
  virtual const TMatrixD* getOriginal() const {return originalResult;}
  virtual const TMatrixD* generatePseudo() const {
    ++currBS;
    if(currBS > nBS) throw std::invalid_argument("PseudoMatrixBootstrapGenerator::generatePseudo: Asking for more bootstraps than I have!");
    return &bootstrapResults->at(currBS-1);
  }
  virtual void reset() {currBS = 0;}
  virtual const TMatrixD* getError() const {return 0;}
  virtual int getMaxIterations() const {return nBS;}

private:
  void fillData(TTree * tree, const TString selection, const TString weight,  const vector<TString>& recoCategories, const vector<TString>& genCategories, const TString bootstrapWeight){
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

  unsigned int nBS;
  mutable unsigned int currBS;
  TMatrixD * originalResult;
  std::vector<TMatrixD>* bootstrapResults;
};


class MatrixSolver{
public:

  MatrixSolver(const PseudoGenerator<TVectorD>* dataGenerator, const PseudoGenerator<TMatrixD>* mcGenerator,int nIterations) :
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
    std::cout << "The attempting to solve: "<< endl;
    std::cout << "Data: "<< endl;
    dataGenerator->getOriginal()->Print();
    std::cout << "MC: "<< endl;
    mcGenerator->getOriginal()->Print();
    solve();
    }

  ~MatrixSolver(){
    delete sf;
    delete sfError;
    delete sfDataError;
    delete sfMCError;
    delete covMatrix;
  }

  std::vector<double> getSF() {std::vector<double> out; for(unsigned int i = 0; i < nDim; ++i)out.push_back((*sf)(i)); return out; };
  std::vector<double> getSFError() {std::vector<double> out; for(unsigned int i = 0; i < nDim; ++i)out.push_back((*sfError)(i)); return out; };
  std::vector<double> getSFDataError() {std::vector<double> out; for(unsigned int i = 0; i < nDim; ++i)out.push_back((*sfDataError)(i)); return out; };
  std::vector<double> getSFMCError() {std::vector<double> out; for(unsigned int i = 0; i < nDim; ++i)out.push_back((*sfMCError)(i)); return out; };

private:
  bool solve(){
    TDecompLU solver(*mcGenerator->getOriginal());
    (*sf) = (*dataGenerator->getOriginal());
    if(!solver.Solve(*sf)){
      std::cout << "The original matrix can not be solved! "<< endl;
      std::cout << "Exiting! "<< endl;
      return false;
    }
    sf->Print();

    TMatrixD dataCovMatrix(nDim,nDim);
    getDataError(&solver,dataCovMatrix);
    (*covMatrix) += dataCovMatrix;

    TMatrixD mcCovMatrix(nDim,nDim);
    getMCError(mcCovMatrix);
    (*covMatrix) += mcCovMatrix;

    std::cout << "Data Cov matrix: "<< endl;
    dataCovMatrix.Print();
    std::cout << "MC Cov matrix: "<< endl;
    mcCovMatrix.Print();

    for(unsigned int iR = 0; iR < nDim; ++iR){ (*sfError)(iR)  = TMath::Sqrt((*covMatrix)(iR,iR));}
    for(unsigned int iR = 0; iR < nDim; ++iR){ (*sfDataError)(iR)  = TMath::Sqrt((dataCovMatrix)(iR,iR));}
    for(unsigned int iR = 0; iR < nDim; ++iR){ (*sfMCError)(iR)  = TMath::Sqrt((mcCovMatrix)(iR,iR));}

    std::cout << "Total errors: "<< endl;
    sfError->Print();

    return true;
  }

  void getDataError(TDecompBase* solver,TMatrixD& errorCov){
    TVectorD meanSF(nDim);
    TMatrixD meanSqSF(nDim,nDim);
    TVectorD newSF(nDim);

    int maxIt = dataGenerator->getMaxIterations() < 0 ? nIt : std::min(int(nIt),dataGenerator->getMaxIterations());
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
  void getMCError(TMatrixD& errorCov){
    TVectorD meanSF(nDim);
    TMatrixD meanSqSF(nDim,nDim);
    TVectorD newSF(nDim);
    TMatrixD newMC = *mcGenerator->getOriginal();
    int maxIt = mcGenerator->getMaxIterations() < 0 ? nIt : std::min(int(nIt),mcGenerator->getMaxIterations());
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
      cout << "Less than 80% of iterations could be solved!"<<endl;
      cout << "I would not trust these errors!"<<endl;
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
  unsigned int nIt;
  const PseudoGenerator<TVectorD>* dataGenerator;
  const PseudoGenerator<TMatrixD>* mcGenerator;
  int nDim;
  TVectorD* sf;
  TVectorD* sfError;
  TVectorD* sfDataError;
  TVectorD* sfMCError;
  TMatrixD* covMatrix;

};
}




using namespace std;


TTree * getTree(TString filename){
  TFile * sf = new TFile(filename,"read");
  TTree * st =0;
  sf->GetObject("Events",st);
  return st;
}


const TString stdWeight = "1.0";
const TString stdMCWeight = "weight*truePUWeight*btagWeight*2.262";

enum TreeType {Data,QCD, NoSmearQCD, TTbarW, Znuu, ttZ};
const TString TreeNames[] = {"Data", "QCD","NoSmearQCD","t#bar{t}W","Z#rightarrow#nu#nu","t#bar{t}Z"};
const StyleTools::Colors TreeColors[] = {color_data, color_qcd,color_qcd,color_ttbar,color_znunu,color_ttZ};
const TString weights[] = {stdWeight,stdMCWeight,stdMCWeight,stdMCWeight,stdMCWeight,stdMCWeight};

void getSTDTreesAndNames(vector<pair<TTree*,TreeType> >& trees){
  trees.emplace_back(getTree("htmht_tree_skimmed.root"),Data);
  trees.emplace_back(getTree("ttbarplusw_tree_skimmed.root"),TTbarW);
  trees.emplace_back(getTree("znunu_tree_skimmed.root"),Znuu);
  trees.emplace_back(getTree("ttZ_tree_skimmed.root"),ttZ);
  trees.emplace_back(getTree("qcd_smearTree_skimmed.root")     ,QCD);
//  trees.emplace_back(getTree("qcd_origTree.root") ,NoSmearQCD);

}
const TString METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && met >= 250 && j2pt >= 75";
const TString ResTailExtraCuts = "(dphij12met < .1 || dphij3met < .1) && nvetolep == 0 && pseudoRespPassFilter == 1";

void getRegion(){
//  TString presel = TString::Format("%s && nvetolep == 0", METPresel.Data());
  TString presel = TString::Format("%s && (dphij12met < .1 || dphij3met < .1) && nvetolep == 0", METPresel.Data());
//  TString sels[] = {"met >= 200","met >= 250","nbjets >= 1","nbjets >= 1 && nlbjets >= 2","njets >=5 && nbjets >= 1 && nlbjets >= 2",""};
//  TString sels[] = {"met >= 200","njets >=5","nbjets >= 1","njets >=5 && nbjets >= 1",""};
//  TString sels[] = {"dphij12met < .15","dphij12met < .15 && pseudoRespNoFilterRank >= 0 && pseudoRespNoFilterRank < 2","dphij12met < .15 && pseudoRespNoFilterRank >= 0 && pseudoRespNoFilterRank < 2 && pseudoRespNoFilterNextPTRatio < .5",""};
//  TString sels[] = {"njets >=5 && nbjets >= 1 && nlbjets >= 2",""};
//    TString sels[] = {"pseudoResp < .5 && pseudoRespNoFilterRank >= 0","pseudoResp < .3 && pseudoRespNoFilterRank >= 0","pseudoResp < .15 && pseudoRespNoFilterRank >= 0",""};
//    TString sels[] = {"met >= 200","met >= 250",""};
    TString sels[] = {"pseudoRespCSV >= 0.890","pseudoRespCSV >= 0.970","pseudoRespCSV < 0.605",""};


  HistogramGetter histG("pseudoResp","pseudoResp","#it{p}_{T,reco jet} / #it{p}_{T,reco jet} + #slash{#it{E}}_{T}", 40,0,2);
  histG.underOverflow = true;
//  HistogramGetter histG("pseudoRespNoFilterNextPTRatio","pseudoRespNoFilterNextPTRatio","pseudoRespNoFilterNextPTRatio", 20,0,1);
//  HistogramGetter histG("met","met","#it{p}_{T,reco jet} / #it{p}_{T,reco jet} + #slash{#it{E}}_{T}", 20,0,1000);
//      HistogramGetter histG("dphij12met","dphij12met","dphij12met", 12,0,.6);
//      HistogramGetter histG("dphij3met","dphij3met","dphij3met", 12,0,.6);


  vector<pair<TTree*,TreeType> > trees;
  getSTDTreesAndNames(trees);

  for(unsigned int iP = 0; sels[iP][0]; ++iP){
    TString title = TString::Format("%s",sels[iP].Data());
    TString name = TString::Format("getRegion_%u",iP);
    Plot * plot = new Plot(name,title,histG.plotInfo->xTitle,"Events");
    TString sel = TString::Format("%s && %s",presel.Data(),sels[iP].Data());
    for(unsigned int iT = 0; iT < trees.size(); ++iT){

      TH1F * hSN = histG.getHistogram(trees[iT].first,sel,weights[trees[iT].second],TString::Format("tree_%u",iT));

//      if(trees[iT].second == Data)
//        plot->addHist(hSN,TreeNames[trees[iT].second],histG.plotInfo->xTitle,colorGetter(iT));
//      else
//        plot->addToStack(hSN,TreeNames[trees[iT].second],colorGetter(iT));

      if(trees[iT].second == Data)
        plot->addHist(hSN,TreeNames[trees[iT].second],"",TreeColors[trees[iT].second]);
      else
        plot->addToStack(hSN,TreeNames[trees[iT].second],TreeColors[trees[iT].second]);

    }
    TCanvas * c = new TCanvas;
    plot->drawRatioStack(c,false,"png");
//    plot->drawRatios(c,1,true,"png");
    cout << title << endl;
  }
}

void checkContamination(){
//  TString presel = TString::Format("%s && nvetolep == 0", METPresel.Data());
  TString presel = TString::Format("%s && (dphij12met < .1 || dphij3met < .1) && nvetolep == 0 && pseudoRespNoFilterRank >= 0", METPresel.Data());

    TString sels[] = {"nlbjets == 0","pseudoRespCSV < 0.605","pseudoRespCSV >= 0.890","pseudoRespCSV >= 0.970",""};

  vector<HistogramGetter*> histGs;
  double bins[] = {0,.33,.5,.66,1};
  histGs.push_back(new HistogramGetter("pseudoResp","pseudoResp","#it{p}_{T,reco jet} / #it{p}_{T,reco jet} + #slash{#it{E}}_{T}", 4,bins));

  vector<TString> names;
  vector<TTree*> trees;
  trees.emplace_back(getTree("htmht_tree_skimmed.root")); names.push_back("Data");
  trees.emplace_back(getTree("nonQCD_tree_skimmed.root")); names.push_back("BKG");
  trees.emplace_back(getTree("qcd_smearTree_skimmed_split_light_gtp66.root"));  names.push_back("l, >.66");
  trees.emplace_back(getTree("qcd_smearTree_skimmed_split_light_p33top66.root"));  names.push_back("l, .33-.66");
  trees.emplace_back(getTree("qcd_smearTree_skimmed_split_light_ltp33.root"));  names.push_back("l, <.33");
  trees.emplace_back(getTree("qcd_smearTree_skimmed_split_b_gtp50.root"));      names.push_back("b, >.5");
  trees.emplace_back(getTree("qcd_smearTree_skimmed_split_b_ltp50.root"));names.push_back("b, < .5");


  for(unsigned int iH = 0; iH < histGs.size(); ++iH)
  for(unsigned int iP = 0; sels[iP][0]; ++iP){
    TString title = TString::Format("%s",sels[iP].Data());
    TString name = TString::Format("getRegion_%u",iP);
    Plot * plot = new Plot(name,title,histGs[iH]->plotInfo->xTitle,"Events");
    TString sel = TString::Format("%s && %s",presel.Data(),sels[iP].Data());
    for(unsigned int iT = 0; iT < trees.size(); ++iT){

      TH1F * hSN = histGs[iH]->getHistogram(trees[iT],sel,stdMCWeight,TString::Format("tree_%u",iT));
        plot->addHist(hSN,names[iT],"",colorGetter(iT),0,colorGetter(iT));
    }
    TCanvas * c = new TCanvas;
    plot->draw(c,false,"png");
    cout << title << endl;
  }
}

void getSFs(){
  TString presel = TString::Format("%s && (dphij12met < .1 || dphij3met < .1) && nvetolep == 0 && pseudoRespNoFilterRank >= 0", METPresel.Data());
//  TString recoSels[] = {"pseudoResp < . 33","pseudoResp >= .33 && pseudoResp < . 66","pseudoResp >= .66",""};
//  TString genSels[] = {"trueResp < . 33","trueResp >= .33 && trueResp < . 66","trueResp >= .66",""};

  TString recoSels[] = {"pseudoResp <  .33 && pseudoRespCSV < 0.605","pseudoResp >= .33 && pseudoResp < .66 && pseudoRespCSV < 0.605","pseudoResp >= .66 && pseudoRespCSV < 0.605",
      "pseudoResp < . 5 && pseudoRespCSV >= 0.890","pseudoResp >= .5 && pseudoRespCSV >= 0.890"
      ,""};
//  TString recoSels[] = {"pseudoResp <  .33 && nlbjets ==0","pseudoResp >= .33 && pseudoResp < .66 && nlbjets ==0","pseudoResp >= .66 && nlbjets ==0",
//      "pseudoResp < . 5 && pseudoRespCSV >= 0.970","pseudoResp >= .5 && pseudoRespCSV >= 0.970"
//      ,""};
  TString genSels[] = {"trueResp < .33 && trueRespFlv !=4","trueResp >= .33 && trueResp < .66 && trueRespFlv !=4","trueResp >= .66 && trueRespFlv !=4",
                       "trueResp < .5 && trueRespFlv ==4","trueResp >= .5 && trueRespFlv ==4",""};

//  TString recoSels[] = {"pseudoResp <  .33 && pseudoRespCSV < 0.605","pseudoResp >= .33 && pseudoResp < .66 && pseudoRespCSV < 0.605","pseudoResp >= .66 && pseudoRespCSV < 0.605",
//      "pseudoResp < . 33 && pseudoRespCSV >= 0.970","pseudoResp >= .33 && pseudoResp < .66 && pseudoRespCSV >= 0.970","pseudoResp >= .66 && pseudoRespCSV >= 0.970"
//      ,""};
//  TString genSels[] = {"trueResp < .33 && trueRespFlv !=4","trueResp >= .33 && trueResp < .66 && trueRespFlv !=4","trueResp >= .66 && trueRespFlv !=4",
//                       "trueResp < .33 && trueRespFlv ==4","trueResp >= .33 && trueResp < .66  && trueRespFlv ==4","trueResp >= .66 && trueRespFlv ==4",""};

cout << presel <<endl;
vector<double> bkgSFs;
bkgSFs.push_back(.8);
bkgSFs.push_back(.8);
bkgSFs.push_back(.8);
bkgSFs.push_back(.8);
bkgSFs.push_back(.8);
//bkgSFs.push_back(.8);

vector<double> bkgUNCs;
bkgUNCs.push_back(.2);
bkgUNCs.push_back(.2);
bkgUNCs.push_back(.2);
bkgUNCs.push_back(.2);
bkgUNCs.push_back(.2);
//bkgUNCs.push_back(.2);

  vector<TString> recoSelStrings;
  vector<TString> genSelStrings;
  for(unsigned int iD = 0; recoSels[iD][0]; ++iD){
    recoSelStrings.push_back(recoSels[iD]);
    genSelStrings.push_back(genSels[iD]);
  }
  TRandom3 * rand = new TRandom3(1234);
  TTree * dataTree = getTree("htmht_tree_skimmed.root");
  TTree * bkgTree = getTree("nonQCD_tree_skimmed.root");
  TTree * qcdTree = getTree("qcd_smearTree_skimmed_dphipresel.root");

  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVector(dataTree,bkgTree,&bkgSFs,&bkgUNCs,presel,"1.0",stdMCWeight,recoSelStrings,rand);

  cout <<"Loading MC!" <<endl;
  SFGetter::PseudoMatrixBootstrapGenerator mcMatrix(qcdTree,presel,stdMCWeight,recoSelStrings,genSelStrings,50,"bootstrapWeight");
//  TTree * qcdTree = getTree("qcd_origTree.root");
//  cout <<"Loading MC!" <<endl;
//  SFGetter::PseudoMatrixGausGenerator mcMatrix(qcdTree,presel,stdMCWeight,recoSelStrings,genSelStrings,rand);

  SFGetter::MatrixSolver solver(&dataVector,&mcMatrix,5);
}


void doFullExtraction() {

  TRandom3 * rand = new TRandom3(1234);
  //Set binning
  vector<TString> recoSels;
  recoSels.push_back("pseudoResp >= .66 && pseudoRespCSV < 0.605");
  recoSels.push_back("pseudoResp >= .33 && pseudoResp < .66 && pseudoRespCSV < 0.605");
  recoSels.push_back("pseudoResp <  .33 && pseudoRespCSV < 0.605");
  recoSels.push_back("pseudoResp >= .5 && pseudoRespCSV >= 0.890");
  recoSels.push_back("pseudoResp < . 5 && pseudoRespCSV >= 0.890");

  vector<TString> genSels;
  genSels.push_back("trueResp >= .66 && trueRespFlv !=4");
  genSels.push_back("trueResp >= .33 && trueResp < .66 && trueRespFlv !=4");
  genSels.push_back("trueResp < .33 && trueRespFlv !=4");
  genSels.push_back("trueResp >= .5 && trueRespFlv ==4");
  genSels.push_back("trueResp < .5 && trueRespFlv ==4");

  vector<TString> genSelNames;
  genSelNames.push_back("non-b, r(jet) > 0.66");
  genSelNames.push_back("non-b, 0.66 > r(jet) > 0.33");
  genSelNames.push_back("non-b, 0.33 > r(jet)");
  genSelNames.push_back("b, r(jet) > 0.50");
  genSelNames.push_back("b, 0.50 > r(jet)");


  vector<TString> genSelMathNames;
  genSelMathNames.push_back("non-b, $#it{r}_{\\mathrm{jet}} > 0.66$");
  genSelMathNames.push_back("non-b, $0.66 > #it{r}_{\\mathrm{jet}} > 0.33$");
  genSelMathNames.push_back("non-b, $0.33 > #it{r}_{\\mathrm{jet}}$");
  genSelMathNames.push_back("b, $#it{r}_{\\mathrm{jet}} > 0.50$");
  genSelMathNames.push_back("b, $0.50 > #it{r}_{\\mathrm{jet}}$");


  //First get ttbar+W SFs
  vector<double> otherBkgNorms;
  TTree * dataNormTree = getTree("pieces/htmht_tree_skimmed.root");
  TTree * bkgNormTree = getTree("pieces/nonQCD_tree_skimmed.root");
  TTree * qcdNormTree = getTree("pieces/qcd_tree_skimmed.root");
  TString ttbarWNormSels[] = {TString::Format("%s && nvetolep >= 1 && mtlepmet < 100", METPresel.Data()),TString::Format("%s && nvetolep >= 1 && mtlepmet < 100 && nbjets >= 1", METPresel.Data()),""};
  TString ttbarWNormSelNames[] = {"Incl","bTag"};
  HistogramGetter normHisto ("norm","met","norm", 1,150,1000);
  for(unsigned int iS = 0; ttbarWNormSels[iS][0]; ++iS){
    TH1F * dataH  = normHisto.getHistogram(dataNormTree,ttbarWNormSels[iS],"1.0","Data");
    TH1F * otherH = normHisto.getHistogram(bkgNormTree,ttbarWNormSels[iS],stdMCWeight,"Other");
    TH1F * qcdH   = normHisto.getHistogram(qcdNormTree,ttbarWNormSels[iS],stdMCWeight,"MC");

    otherH->Add(qcdH);
    dataH->Divide(otherH);
    cout <<"ttbar+W norm: " << ttbarWNormSelNames[iS] <<" "<< dataH->GetBinContent(1) <<" +/- "<<dataH->GetBinError(1) <<endl;
    otherBkgNorms.push_back(dataH->GetBinContent(1));
  }


  TTree * dataTree = getTree("pieces/htmht_tree_skimmed_dphi.root");
  TTree * bkgTree = getTree("pieces/nonQCD_tree_skimmed_dphi.root");
  TTree * qcdTree = getTree("pieces/qcd_tree_skimmed_dphi.root");
  vector<double> bkgSFs;
  bkgSFs.push_back(otherBkgNorms[0]);
  bkgSFs.push_back(otherBkgNorms[0]);
  bkgSFs.push_back(otherBkgNorms[0]);
  bkgSFs.push_back(otherBkgNorms[1]);
  bkgSFs.push_back(otherBkgNorms[1]);
  vector<double> bkgUNCs;
  bkgUNCs.push_back(.2);
  bkgUNCs.push_back(.2);
  bkgUNCs.push_back(.2);
  bkgUNCs.push_back(.2);
  bkgUNCs.push_back(.2);


  TString sel = TString::Format("%s && %s", METPresel.Data(),ResTailExtraCuts.Data());
  vector<double> SF;
  vector<double> SFTotStatUnc;
  vector<double> SFTotDataStatUnc;
  vector<double> SFNoBKGUncDataStatUnc;
  vector<double> SFMCStatUnc;

  vector<double> SFHeavyUp;
  vector<double> SFLightUp;


  //Now do the central value
  cout <<"Starting first pass: " <<endl;
  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVector(dataTree,bkgTree,&bkgSFs,&bkgUNCs,sel,"1.0",stdMCWeight,recoSels,rand);
  cout <<"Loading MC!" <<endl;
  SFGetter::PseudoMatrixBootstrapGenerator mcMatrix(qcdTree,sel,stdMCWeight,recoSels,genSels,50,"bootstrapWeight");
  SFGetter::MatrixSolver solver(&dataVector,&mcMatrix,10000);
  SF = solver.getSF();
  SFTotStatUnc = solver.getSFError();
  SFTotDataStatUnc = solver.getSFDataError();
  SFMCStatUnc = solver.getSFMCError();

  //Now do it again without an uncertainty on the background subtraction
  cout <<"Starting second pass: " <<endl;
  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVectorNoBKGUnc(dataTree,bkgTree,&bkgSFs,0,sel,"1.0",stdMCWeight,recoSels,rand);
  mcMatrix.reset();
  SFGetter::MatrixSolver solverNoBKGUnc(&dataVectorNoBKGUnc,&mcMatrix,10000);
  SFNoBKGUncDataStatUnc = solverNoBKGUnc.getSFDataError();

  //Get btagup heavy up unc
  const TString upHeavyMCWeight = "weight*truePUWeight*upBTagHeavyWeight*2.262";
  cout <<"Starting third pass: " <<endl;
  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVectorUpHeavy(dataTree,bkgTree,&bkgSFs,&bkgUNCs,sel,"1.0",upHeavyMCWeight,recoSels,rand);
  cout <<"Loading MC!" <<endl;
  SFGetter::PseudoMatrixGausGenerator mcMatrixUpHeavy(qcdTree,sel,upHeavyMCWeight,recoSels,genSels,rand);
  SFGetter::MatrixSolver solverUpHeavy(&dataVectorUpHeavy,&mcMatrixUpHeavy,10000);
  SFHeavyUp = solverUpHeavy.getSF();

  //Get btagup light up unc
  const TString upLightMCWeight = "weight*truePUWeight*upBTagLightWeight*2.262";
  cout <<"Starting fourth pass: " <<endl;
  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVectorUpLight(dataTree,bkgTree,&bkgSFs,&bkgUNCs,sel,"1.0",upLightMCWeight,recoSels,rand);
  cout <<"Loading MC!" <<endl;
  SFGetter::PseudoMatrixGausGenerator mcMatrixUpLight(qcdTree,sel,upLightMCWeight,recoSels,genSels,rand);
  SFGetter::MatrixSolver solverUpLight(&dataVectorUpLight,&mcMatrixUpLight,10000);
  SFLightUp = solverUpLight.getSF();


  //Print results;
  int nDim = SF.size();
  for(unsigned int i = 0; i < nDim; ++i)  cout << genSelNames[i].Data() <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SF[i]                                                                                                    )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFTotStatUnc[i]                                                                                          )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFTotDataStatUnc[i]                                                                                      )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFNoBKGUncDataStatUnc[i]                                                                                 )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFMCStatUnc[i]                                                                                           )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFHeavyUp[i]                                                                                             )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFLightUp[i]                                                                                             )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFHeavyUp[i] - SF[i]                                                                                     )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFLightUp[i] - SF[i]                                                                                     )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",TMath::Sqrt(SFTotDataStatUnc[i]*SFTotDataStatUnc[i] - SFNoBKGUncDataStatUnc[i]*SFNoBKGUncDataStatUnc[i] ))  <<"\t"; cout << endl;

  vector<double> allUnc;
  for(unsigned int i = 0; i < nDim; ++i)  {
    allUnc.push_back(TMath::Sqrt(SFTotStatUnc[i] *SFTotStatUnc[i]  + (SFLightUp[i] - SF[i])*(SFLightUp[i] - SF[i]) + (SFHeavyUp[i] - SF[i])*(SFHeavyUp[i] - SF[i]) ));
  }

  //Print table
  cout << "\\begin{table}[h!]" <<endl;
  cout << "\\caption{\\label{tab:qcd_tailsf}}"<<endl;
  cout << "\\begin{center}";
  cout << "\\begin{tabular}{|c|"; for(unsigned int i = 0; i < nDim; ++i) cout << "c|"; cout << "}"<<endl;
  cout << "\\hline"<<endl;
  cout <<" & "; for(unsigned int i = 0; i < nDim; ++i)  cout << genSelMathNames[i].Data() <<" & "; cout <<"\\hline" << endl;
  cout <<"Correction & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SF[i]) <<" & "; cout <<"\\hline" << endl;
  cout << "\\hline"<<endl;
  cout <<"Data statistics & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFNoBKGUncDataStatUnc[i]) <<" & "; cout <<"\\hline" << endl;
  cout <<"QCD MC statistics & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFMCStatUnc[i]) <<" & "; cout <<"\\hline" << endl;
  cout <<"Non-QCD subtraction & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",TMath::Sqrt(SFTotDataStatUnc[i]*SFTotDataStatUnc[i] - SFNoBKGUncDataStatUnc[i]*SFNoBKGUncDataStatUnc[i] )) <<" & "; cout <<"\\hline" << endl;
  cout <<"Light b-tag SF & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",TMath::Abs(SFLightUp[i] - SF[i]) ) <<" & "; cout <<"\\hline" << endl;
  cout <<"Heavy b-tag SF & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",TMath::Abs(SFHeavyUp[i] - SF[i]) ) <<" & "; cout <<"\\hline" << endl;
  cout << "\\hline" <<endl;
  cout <<"Total unc. & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",allUnc[i] ) <<" & "; cout <<"\\hline" << endl;
  cout <<"\\end{tabular}"<<endl;
  cout <<"\\end{center}"<<endl;
  cout <<"\\end{table}"<<endl;
}

void prepareTrees() {

  TString trees[] = {"pieces/htmht_tree",
      "pieces/ttbarplusw_tree",
      "pieces/znunu_tree",
      "pieces/ttZ_tZq",
      "pieces/qcd_tree",
      "pieces/qcd_origtree",
      "pieces/nonQCD_tree",""};



  auto proc = [](TString pre){
    TFile * fI = new TFile(pre +".root","Read");
    TTree *tI =0;
    fI->GetObject("Events",tI);
    tI->SetBranchStatus("*",0);
    tI->SetBranchStatus("bootstrapWeight",1);
    tI->SetBranchStatus("pseudoResp",1);
    tI->SetBranchStatus("pseudoRespCSV",1);
    tI->SetBranchStatus("pseudoRespPassFilter",1);
    tI->SetBranchStatus("trueResp",1);
    tI->SetBranchStatus("trueRespFlv",1);
    tI->SetBranchStatus("weight",1);
    tI->SetBranchStatus("truePUWeight",1);
    tI->SetBranchStatus("btagWeight",1);
    tI->SetBranchStatus("upBTagLightWeight",1);
    tI->SetBranchStatus("upBTagHeavyWeight",1);
    tI->SetBranchStatus("passdijetmet",1);
    tI->SetBranchStatus("passjson",1);
    tI->SetBranchStatus("passeebadscflt",1);
    tI->SetBranchStatus("passhbhefltloose",1);
    tI->SetBranchStatus("passcscbeamhaloflt",1);
    tI->SetBranchStatus("passeebadsc4flt",1);
    tI->SetBranchStatus("passhbheisoflt",1);
    tI->SetBranchStatus("met",1);
    tI->SetBranchStatus("nvetolep",1);
    tI->SetBranchStatus("nvetotau",1);
    tI->SetBranchStatus("ncttstd",1);
    tI->SetBranchStatus("j2pt",1);
    tI->SetBranchStatus("dphij12met",1);
    tI->SetBranchStatus("dphij3met",1);
    tI->SetBranchStatus("dphij4met",1);
    tI->SetBranchStatus("njets",1);
    tI->SetBranchStatus("nbjets",1);
    tI->SetBranchStatus("nlbjets",1);
    tI->SetBranchStatus("dphicsv12met",1);
    tI->SetBranchStatus("mtlepmet",1);
    TFile * fO = new TFile(pre +"_skimmed.root","recreate");
    fO->cd();
    TTree * tO = tI->CopyTree(METPresel);
    tO->Write();
    fO->Close();
  };

//  for(unsigned int iT = 0; trees[iT][0]; ++iT)proc(trees[iT]);

  auto proc2 = [](TString pre){
    TFile * fI = new TFile(pre +"_skimmed.root","Read");
    TTree *tI =0;
    fI->GetObject("Events",tI);
    TFile * fO = new TFile(pre +"_skimmed_dphi.root","recreate");
    fO->cd();
    TTree * tO = tI->CopyTree(ResTailExtraCuts);
    tO->Write();
    fO->Close();
  };

//  for(unsigned int iT = 0; trees[iT][0]; ++iT)proc2(trees[iT]);


  auto proc3 = [](TString pre){
    TFile * fI = new TFile(pre +"_skimmed_dphi.root","Read");
    TTree *tI =0;
    fI->GetObject("Events",tI);
    TFile * fO = new TFile(pre +"_skimmed_dphi_split_light_ltp33.root","recreate");
    fO->cd();
    TTree * tO = tI->CopyTree("trueResp < .33 && trueRespFlv !=4");
    tO->Write();
    fO->Close();
    delete fO;
    fO = new TFile(pre +"_skimmed_dphi_split_light_p33top66.root","recreate");
    fO->cd();
    tO = tI->CopyTree("trueResp >= .33 && trueResp < .66 && trueRespFlv !=4");
    tO->Write();
    fO->Close();
    delete fO;
    fO = new TFile(pre +"_skimmed_dphi_split_light_gtp66.root","recreate");
    fO->cd();
    tO = tI->CopyTree("trueResp >= .66 && trueRespFlv !=4");
    tO->Write();
    fO->Close();
    delete fO;
    fO = new TFile(pre +"_skimmed_dphi_split_b_ltp50.root","recreate");
    fO->cd();
    tO = tI->CopyTree("trueResp < .50 && trueRespFlv ==4");
    tO->Write();
    fO->Close();
    delete fO;
    fO = new TFile(pre +"_skimmed_dphi_split_b_gtp50.root","recreate");
    fO->cd();
    tO = tI->CopyTree("trueResp >= .50 && trueRespFlv ==4");
    tO->Write();
    fO->Close();
    delete fO;
  };
  proc3("pieces/qcd_tree");


}

#endif


void GetTailSF()
{

  StyleTools::SetStyle();
  prepareTrees();
//  doFullExtraction();
//  getRegion();
//  checkContamination();
//  getSFs();
//  test();
}
