//--------------------------------------------------------------------------------------------------
// 
// BaseAnalyzer
// 
// Base class to define interface for analyzer classes. Implementation of a CMS EDFilter..
// 
// BaseAnalyzer.cc created on Fri Aug 8 15:50:43 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include <iostream>

#include "AnalysisBase/Analyzer/interface/BaseAnalyzer.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
BaseAnalyzer::BaseAnalyzer(const edm::ParameterSet& iConfig)
  : outputPath          (iConfig.getUntrackedParameter<std::string>("outputPath", "").data())
  , outputInterval      (iConfig.getUntrackedParameter<int             >("outputInterval",-9   ))
  , randomGenerator     (gRandom = new TRandom3(iConfig.getParameter<unsigned int>("randomSeed")))
  , numProcessed_       (0)
  , numAnalyzed_        (0)
  , runNumber_          (-1)
  , lumiBlock_          (-1)
  , eventNumber_        (-1)

{
  if(outputPath.Length()) {
    std::clog << " ++  outputPath          = " << (outputPath.Length() ? outputPath.Data() : "(n/a)") << std::endl;
    std::clog << " ++  outputInterval      = " << outputInterval                                      << std::endl;
  }
  std::clog << " ++  randomSeed          = " << iConfig.getParameter<unsigned int>("randomSeed")      << std::endl;

  // Create and register TTree
  edm::Service<TFileService> fs;
  eventTree_ = fs->make<TTree>("Events", "");
  treeWriter_ = new TreeWriter(eventTree_);

}

//--------------------------------------------------------------------------------------------------
BaseAnalyzer::~BaseAnalyzer()
{
  std::clog << std::endl;
  std::clog << " \033[1;34m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\033[0m"  << std::endl;
  std::clog << " \033[1;34m~~\033[0m  Done! There were:" << std::endl;
  std::clog << " \033[1;34m~~\033[0m   " << TString::Format("%6d", numProcessed()) << " events processed in this job."                   << std::endl;
  std::clog << " \033[1;34m~~\033[0m   " << TString::Format("%6d", numAnalyzed ()) << " events successfully analyzed."                   << std::endl;
  if (numProcessed() == 0)
    std::clog << " \033[31m~~  WARNING : Either the input file(s) are empty, or some error occurred before processing! \033[0m"          << std::endl;
  std::clog << " \033[1;34m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\033[0m"  << std::endl;

  std::clog << std::endl;
}

//--------------------------------------------------------------------------------------------------
void BaseAnalyzer::beginJob()
{
  // do nothing here
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void BaseAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//--------------------------------------------------------------------------------------------------
bool BaseAnalyzer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  runNumber_                = iEvent.run();
  lumiBlock_                = iEvent.luminosityBlock();
  eventNumber_              = iEvent.id().event();


  bool          success     = load    (iEvent, iSetup);
  if (success)  success    &= filter  ();
  if (success)                analyze ();
  if (success)  success    &= produce (iEvent, iSetup);

  ++numProcessed_;
  if (success)  ++numAnalyzed_;

  return success;
}

//--------------------------------------------------------------------------------------------------
void BaseAnalyzer::printEventCoordinates(std::ostream& out) const
{
  out << TString::Format( "*****  %d:%d:%llu = Run %7d, luminosity block %4d, event %10llu"
                        , runNumber(), lumiBlock(), eventNumber()
                        , runNumber(), lumiBlock(), eventNumber()
                        )
      << std::endl;
}
