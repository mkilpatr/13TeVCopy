//--------------------------------------------------------------------------------------------------
// 
// BaseAnalyzer
// 
// Base class to define interface for analyzer classes. Implementation of a CMS EDAnalyzer.
// 
// BaseAnalyzer.cc created on Fri Aug 8 15:50:43 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/BaseAnalyzer.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
BaseAnalyzer::BaseAnalyzer(const edm::ParameterSet& iConfig)
{
   // now do what ever initialization is needed

}

//--------------------------------------------------------------------------------------------------
void
BaseAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // do nothing

}

//--------------------------------------------------------------------------------------------------
void
BaseAnalyzer::beginJob()
{
  // do nothing

}

//--------------------------------------------------------------------------------------------------
void
BaseAnalyzer::endJob()
{
  // do nothing

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
BaseAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
