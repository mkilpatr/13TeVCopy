//--------------------------------------------------------------------------------------------------
// 
// PhysicsAnalyzer
// 
// Implementation of BaseAnalyzer with a few additions:
// It provides an interface to event objects, providing functions to load the event content
// It stores the event content classes for easy processing
// It includes a few more advanced header files that are useful in analyzers
// It stores what kind of event it is in addition to the event weight
// 
//--------------------------------------------------------------------------------------------------

#ifndef PHYSICSANALYZER_H
#define PHYSICSANALYZER_H

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "AnalysisBase/Analyzer/interface/BaseAnalyzer.h"
#include "AnalysisBase/Analyzer/interface/PhysicsUtilities.h"

namespace ucsbsusy {
  class PhysicsAnalyzer : public BaseAnalyzer, public PhysicsUtilities {
    public:
      PhysicsAnalyzer(const edm::ParameterSet& iConfig);
      virtual ~PhysicsAnalyzer() {};

      virtual bool filter(edm::Event& iEvent, const edm::EventSetup& iSetup) override;

      //--------------------------------------------------------------------------------------------------
      // Functions to manipulate and get stored data members
      //--------------------------------------------------------------------------------------------------
      bool   isData() const;
      bool   isMC  () const;
      double eventWeight   () const        { return eventWeight_;    }
      void   setEventWeight(double weight) { eventWeight_  = weight; }

      //--------------------------------------------------------------------------------------------------
      // Event info data members
      //--------------------------------------------------------------------------------------------------
    private:
      const edm::InputTag         eventInfoSource;      ///< To fill the generator info
      double                      eventWeight_;         ///< From generator info
    public:
      const int                   isRealData;           ///< Whether or not processing real data; deduced from input file name and verified once first event is loaded
      const TString               globalTag;            ///< Global tag name
      const TString               process;              ///< The input process (e.g. ttbar, QCD, etc.)
      const TString               dataset;              ///< The input dataset name (process + generator + binning info)
      double                      crossSection;         ///< MC cross section (if relevant) -- updated every run from input file information if negative
      const int                   totalEvents;          ///< Total number of events in sample as stored in database
      const double                crossSectionScaling;  ///< If nonnegative (and for MC only), multiplies the event weight by this factor, which is the cross-section scaling for the given luminosity (in fb^-1)

      //--------------------------------------------------------------------------------------------------
      // Event data members
      //--------------------------------------------------------------------------------------------------
      edm::Handle<GenEventInfoProduct>               eventInfo;          ///< Standard RECO collection



  };
}

#endif
