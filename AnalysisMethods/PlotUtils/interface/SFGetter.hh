#ifndef SFGETTER_HH
#define SFGETTER_HH

#include <vector>
#include <TString.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompLU.h>
#include <TRandom3.h>
#include <TMath.h>
#include <TTree.h>
#include <TTreeFormula.h>

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
  PseudoVectorGausGenerator(TTree * dataTree, TTree * bkgTree, const std::vector<double>* bkgSF, const std::vector<double>* bkgSystUnc, const TString selection, const TString dataWeight, TString bkgWeight, const std::vector<TString>& recoCategories,  TRandom3* randGen);
  ~PseudoVectorGausGenerator();
  virtual const TVectorD* getOriginal() const {return result;}
  virtual const TVectorD* getError() const {return error;}
  virtual const TVectorD* generatePseudo() const;
  virtual void reset() {}
private:
  void fillOneSet(TTree *tree, const TString selection,const TString weight, const std::vector<TString>& recoCategories, TVectorD& counts, TVectorD& sumW2s );
  void fillData(TTree * dataTree, TTree * bkgTree, const std::vector<double>* bkgSF, const std::vector<double>* bkgSystUnc,const TString selection, const TString dataWeight, TString bkgWeight, const std::vector<TString>& recoCategories);
  TRandom3*  rand;
  TVectorD * result;
  mutable TVectorD * smearResult;
  TVectorD * sumW2;
  TVectorD * error;
};

class PseudoMatrixGausGenerator : public PseudoGenerator<TMatrixD>{
public:
  PseudoMatrixGausGenerator(TTree * tree, const TString selection, const TString weight, const std::vector<TString>& recoCategories, const std::vector<TString>& genCategories, TRandom3* randGen);
  ~PseudoMatrixGausGenerator();
  virtual const TMatrixD* getOriginal() const {return result;}
  virtual const TMatrixD* getError() const {return error;}
  virtual const TMatrixD* generatePseudo() const;
  virtual void reset() {}
private:
  void fillData(TTree * tree, const TString selection, const TString weight,  const std::vector<TString>& recoCategories, const std::vector<TString>& genCategories);
  TRandom3* rand;
  TMatrixD * result;
  mutable TMatrixD * smearResult;
  TMatrixD * sumW2;
  TMatrixD * error;
};

class PseudoMatrixBootstrapGenerator : public PseudoGenerator<TMatrixD>{
public:
  PseudoMatrixBootstrapGenerator(TTree * tree, const TString selection, const TString weight, const std::vector<TString>& recoCategories, const std::vector<TString>& genCategories, const int nBootstraps, const TString bootstrapWeight);
  ~PseudoMatrixBootstrapGenerator();
  virtual const TMatrixD* getOriginal() const {return originalResult;}
  virtual const TMatrixD* generatePseudo() const;
  virtual void reset() {currBS = 0;}
  virtual const TMatrixD* getError() const {return 0;}
  virtual int getMaxIterations() const {return nBS;}

private:
  void fillData(TTree * tree, const TString selection, const TString weight,  const std::vector<TString>& recoCategories, const std::vector<TString>& genCategories, const TString bootstrapWeight);

  unsigned int nBS;
  mutable unsigned int currBS;
  TMatrixD * originalResult;
  std::vector<TMatrixD>* bootstrapResults;
};

class MatrixSolver{
public:
  MatrixSolver(const PseudoGenerator<TVectorD>* dataGenerator, const PseudoGenerator<TMatrixD>* mcGenerator,int nIterations);

  ~MatrixSolver();
  std::vector<double> getSF() {std::vector<double> out; for(unsigned int i = 0; i < nDim; ++i)out.push_back((*sf)(i)); return out; };
  std::vector<double> getSFError() {std::vector<double> out; for(unsigned int i = 0; i < nDim; ++i)out.push_back((*sfError)(i)); return out; };
  std::vector<double> getSFDataError() {std::vector<double> out; for(unsigned int i = 0; i < nDim; ++i)out.push_back((*sfDataError)(i)); return out; };
  std::vector<double> getSFMCError() {std::vector<double> out; for(unsigned int i = 0; i < nDim; ++i)out.push_back((*sfMCError)(i)); return out; };

private:
  bool solve();

  void getDataError(TDecompBase* solver,TMatrixD& errorCov);
  void getMCError(TMatrixD& errorCov);

  unsigned int nIt;
  const PseudoGenerator<TVectorD>* dataGenerator;
  const PseudoGenerator<TMatrixD>* mcGenerator;
  unsigned int nDim;
  TVectorD* sf;
  TVectorD* sfError;
  TVectorD* sfDataError;
  TVectorD* sfMCError;
  TMatrixD* covMatrix;

};

}

#endif
