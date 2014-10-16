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
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

// Data products
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "AnalysisBase/Analyzer/interface/JetFiller.h"
#include "AnalysisBase/Analyzer/interface/ElectronFiller.h"

namespace ucsbsusy {

  class PhysicsAnalyzer : public BaseAnalyzer, public PhysicsUtilities {

    public :
      PhysicsAnalyzer(const edm::ParameterSet& iConfig);
      virtual ~PhysicsAnalyzer();

      virtual void beginJob() override;
      virtual bool filter(edm::Event& iEvent, const edm::EventSetup& iSetup) override;

      // Functions to manipulate and get stored data members
      bool	isData() const;
      bool	isMC  () const;
      double	eventWeight   () 	  const	{ return eventWeight_;		}
      void	setEventWeight(double weight)	{ eventWeight_  = weight;	}

      // Function that calls the default booking function for each filler
      void book(BaseFiller* filler);

      // Function that calls the default reset function for each filler
      void reset(BaseFiller* filler);

      // Function that calls the default load for each filler
      void loadObj(BaseFiller* filler, bool storeOnlyPtr = false);

      // Function that calls the default fill tree function for each filler
      void fillObj(BaseFiller* filler);

    private :
      // Event info data members
      edm::Event*		event_;
      const edm::InputTag	genEventInfoSource_;   // To fill the generator info
      double			eventWeight_;          // From generator info

    public :
      // are all of these needed here?
      const int			isRealData;            // Whether or not processing real data; deduced from input file name and verified once first event is loaded
      const TString		globalTag;             // Global tag name
      const TString		process;               // The input process (e.g. ttbar, QCD, etc.)
      const TString		dataset;               // The input dataset name (process + generator + binning info)
      double			crossSection;          // MC cross section (if relevant) -- updated every run from input file information if negative
      const int			totalEvents;           // Total number of events in sample as stored in database
      const double		crossSectionScaling;   // If nonnegative (and for MC only), multiplies the event weight by this factor, which is the cross-section scaling for the given luminosity (in fb^-1)

      // Event data members
      edm::Handle<GenEventInfoProduct>	genEventInfo;  // Standard RECO collection

      // "Filler" classes to store event information
      EventInfoFiller		eventInfo;
      JetFiller			jets;
      ElectronFiller		electrons;

  };

}

#endif
