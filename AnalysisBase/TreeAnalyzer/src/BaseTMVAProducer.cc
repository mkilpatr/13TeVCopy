#include "AnalysisBase/TreeAnalyzer/interface/BaseTMVAProducer.h"

using namespace std;
using namespace ucsbsusy;


const TString TMVAFactoryConfiguration::defaultOptions = "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedGrad:GradBaggingFraction=0.5:nCuts=200:NNodesMax=5";
const TString TMVAConfiguration::defaultFactoryOptions = "!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification";
const TString TMVAConfiguration::defaultSplittingOptions = "SplitMode=Random:NormMode=EqualNumEvents:V";

BaseTMVAProducer::BaseTMVAProducer(TString outputFileName){
  outFile = new TFile(outputFileName,"RECREATE");
  outputFileName.Prepend("tempOut_");
  tempFile = new TFile(outputFileName,"RECREATE");
}

BaseTMVAProducer::~BaseTMVAProducer(){
  //-- Output -----------------------------------------------------------------
  tempFile ->Close();
  outFile->Close();
  cout << "\n\n  \033[1;37;45m  ======================================= DONE! =======================================  \033[0m\n" << endl;
}

Panvariate::InputTree BaseTMVAProducer::makeInput(TTree* inputTree, TFile* inputFile, const Char_t * name, TString selection)
{
  inputFile ->cd          ();
  TTree*    subTree       = inputTree->CopyTree(selection);
  subTree   ->SetName     (name);
  subTree   ->SetDirectory(inputFile);
  subTree   ->Write       (name, TObject::kWriteDelete);

  return Panvariate::InputTree::make(subTree, name);
}

void BaseTMVAProducer::addVariable(const Char_t*  name, const Char_t* branchName, char type){
  allVariables.push_back(LabeledVariable( name, Panvariate::InputVariable::make(branchName, type ) ));
}

void BaseTMVAProducer::addVariableSet( const char* varLabel, ...){
  vector<const LabeledVariable*>    variables;
  va_list                           arguments;
  va_start(arguments, varLabel);

  const char*   label   = varLabel;
  while (label && label[0]) {
    bool        found   = false;
    for (unsigned iVar = 0; iVar < allVariables.size(); ++iVar){
      if (strcmp(allVariables[iVar].first, label) == 0) {
        found   = true;
        variables.push_back(&allVariables[iVar]);
        break;
      }
    }
    if (!found) cerr << "ERROR: Unknown variable '" << label << "'." << endl;
    assert(found);

    label       = va_arg(arguments, const char*);
  } // end loop over labels
  va_end(arguments);
  variableSets.push_back(variables);
}

void BaseTMVAProducer::addInputTree(TTree * inputTree, const Char_t* name, TString preselection){
  inputTrees.push_back(makeInput(inputTree,tempFile,name,preselection));
}

void BaseTMVAProducer::addFactory(const TMVA::Types::EMVA type, const TString title, const TString options){
  TMVAFactoryConfiguration temp(type,title,options);
  config.factories.push_back(temp);
}
void BaseTMVAProducer::addAxis(Space * space){
  config.axes.push_back(space);
}

void BaseTMVAProducer::setConfiguration(const TString name, const TString title, const TString xmlDir, const TString factoryOptions, const TString splittingOptions){
  config.namePrefix = name;
  config.title = title;
  config.xmlDir = xmlDir;
  config.factoryOptions = factoryOptions;
  config.splittingOptions = splittingOptions;
}

void BaseTMVAProducer::runVarSet(size iSet){
  cout << endl << "\033[1;37;45m  ==========[[ " << iSet << "/" << variableSets.size() << " ]]-----  ";
  const vector<const LabeledVariable*>&     variableSet = variableSets[iSet];
  Panvariate::InputVariables    inputVariables;
  TString                       outputPrefix;
  for (unsigned index = 0; index < variableSet.size(); ++index) {
    const LabeledVariable&      labeledVar  = *variableSet[index];
    inputVariables.push_back(labeledVar.second);

    if (index)  outputPrefix   += "_";
    outputPrefix               += labeledVar.first;
    cout << (index ? " + " : "") << labeledVar.first;
  } // end loop over variables
  cout << "\033[0m\n" << endl;
  if (variableSet.size() == allVariables.size())
    outputPrefix                = "all";

  TString name = TString::Format("%s_%u",config.namePrefix.Data(),iSet);

  //-- Setup factories ------------------------------------------------------
  ParamatrixMVA*  classifiedMVA = new ParamatrixMVA( name, config.title );
  for(unsigned int iP = 0; iP < config.axes.size(); ++iP) classifiedMVA->addParameter ( config.axes[iP]);
  classifiedMVA->bake         ();

  name.Prepend(TString::Format("%s/",config.xmlDir.Data()));
  Panvariate::createFactories ( inputTrees
      , inputVariables
      , classifiedMVA
      , name
      , config.factoryOptions
      , true
  );


  //-- Train methods --------------------------------------------------------
  for (ParamatrixMVA::Iterator iterator = classifiedMVA->iterator(); iterator.next();) {
    Panvariate*     mva         = iterator.get();
    TMVA::Factory*  factory     = mva->getFactory();
    cout << "_____________________________________________________________________________________________________\n" << endl;
    cout << "  " << iterator.getBinTitle()                                                                            << endl;
    cout << "_____________________________________________________________________________________________________\n" << endl;

    factory->PrepareTrainingAndTestTree("", config.splittingOptions);
    for(unsigned int iF = 0; iF < config.factories.size(); ++iF)
      mva    ->bookMethod         ( config.factories[iF].type, config.factories[iF].title, config.factories[iF].options  );
    factory->TrainAllMethods    ();

  }

  outFile->cd();
  classifiedMVA->Write(0, TObject::kWriteDelete);
  }

void BaseTMVAProducer::process(){
  loadVars();
  if(variableSets.size() == 0) throw std::invalid_argument("BaseTMVAProducer::process() :You did not define a variable set!" );
  configure();
  if(inputTrees.size() < 2) throw std::invalid_argument("BaseTMVAProducer::process() : You need at least two input trees!" );
  if(config.factories.size() == 0) throw std::invalid_argument("BaseTMVAProducer::process() : You loaded no factories!" );
  if(config.axes.size() == 0) throw std::invalid_argument("BaseTMVAProducer::process() : You need at least one axis!" );

  for(unsigned int iV = 0; iV < variableSets.size(); ++iV)
    runVarSet(iV);
}
