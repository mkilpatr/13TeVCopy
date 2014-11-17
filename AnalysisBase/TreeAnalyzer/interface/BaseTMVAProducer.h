//--------------------------------------------------------------------------------------------------
// 
// BaseTreeAnalyzer
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_TREEANALYZER_BASETMVAProducer_H
#define ANALYSISBASE_TREEANALYZER_BASETMVAProducer_H

#include <string>
#include <vector>
#include <assert.h>
#include <TString.h>
#include <TTree.h>

#include "AnalysisTools/Parang/interface/Panvariate.h"
#include "AnalysisTools/Utilities/interface/Types.h"

namespace ucsbsusy {

//_____________________________________________________________________________
// Holds basic information on how to setup a factory
//_____________________________________________________________________________
  struct TMVAFactoryConfiguration{
    TMVAFactoryConfiguration(TMVA::Types::EMVA type_, TString title_, TString options_) :
      type(type_),
      title(title_),
      options(options_) {}
    TMVA::Types::EMVA type;
    TString title;
    TString options;

    static const TString defaultOptions;
  };

  //_____________________________________________________________________________
  // Holds complete configuration for training
  //_____________________________________________________________________________
    struct TMVAConfiguration{
    TString namePrefix;
    TString title;
    TString xmlDir;
    TString factoryOptions;
    TString splittingOptions;
    std::vector<Space*>  axes;
    std::vector<TMVAFactoryConfiguration> factories;

    static const TString defaultFactoryOptions;
    static const TString defaultSplittingOptions;
    };


    //_____________________________________________________________________________
    // Actually producer
    //_____________________________________________________________________________
  class BaseTMVAProducer {
  public:
    typedef   std::pair<const Char_t*, Panvariate::InputVariable>  LabeledVariable;
  public:
    BaseTMVAProducer(TString outputFileName);
    virtual ~BaseTMVAProducer();

    //_____________________________________________________________________________
    // Basic utilities
    //_____________________________________________________________________________
  public:
    Panvariate::InputTree makeInput(TTree* inputTree, TFile* inputFile,  const Char_t *  name, TString selection);
    //_____________________________________________________________________________
    // Establish input variables
    //_____________________________________________________________________________
  public:
    virtual void loadVars() = 0; //Add all possible variables here ( can choose a subset later)
    void addVariable(const Char_t*  name,const Char_t* branchName, char type); // add a variable to varInputs
    void addVariableSet( const char* varLabel, ...); //Setup a variable subset

    //_____________________________________________________________________________
    // Configuration
    //_____________________________________________________________________________
  public:
    virtual void configure() = 0; //Apply all configurations
    void addInputTree(TTree * inputTree, const Char_t* name, TString preselection); //Add an input tree to TMVA
    void addFactory(const TMVA::Types::EMVA type, const TString title, const TString options = TMVAFactoryConfiguration::defaultOptions );
    void addAxis(Space * space);
    void setConfiguration(const TString name, const TString title, const TString xmlDir,
        const TString factoryOptions = TMVAConfiguration::defaultFactoryOptions, const TString splittingOptions = TMVAConfiguration::defaultSplittingOptions);

  protected:
    //_____________________________________________________________________________
    // Running
    //_____________________________________________________________________________
    virtual void process();
    void runVarSet(size iSet);



  public:
  protected:
    TFile * outFile; //Holds the TMVA output
    TFile * tempFile; //Holds the temporary TMVA trees
    TMVAConfiguration config;
    Panvariate::InputTrees         inputTrees;                      //Subsets of trees to run signal or background
    std::vector<LabeledVariable> allVariables;                      //TMVA input variables
    std::vector<std::vector<const LabeledVariable*> > variableSets; //The actual sets to make MVAs out of
  public:

  };




}

#endif
