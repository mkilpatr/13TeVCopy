//--------------------------------------------------------------------------------------------------
// 
// TauFiller
// 
// Class to fill hadronic tau information in a TTree. NB: MiniAOD stores taus from hpsPFTauProducer with pT > 18 GeV, and passing the basic decayModeFinding id.
// 
// TauFiller.h created on Wed Oct 22 14:12:52 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_TAUFILLER_H
#define ANALYSISBASE_ANALYZER_TAUFILLER_H

#include "DataFormats/PatCandidates/interface/Tau.h"

#include "AnalysisTools/Utilities/interface/Types.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"

namespace ucsbsusy {

  class TauFiller : public BaseFiller {

    public :
      TauFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller);
      ~TauFiller() {}

      enum Options {
                    NULLOPT      = 0,
                    FILLRAWDISCS = (1 << 0),  // whether or not to store the raw HPS discriminator outputs 
                    PRINTIDS     = (1 << 1)   // check which HPS Tau IDs are available
                   };
      static const int defaultOptions = NULLOPT;

      void initTauIdNames();
      //void beginRun(edm::Run const &run, edm::EventSetup const &iSetup);
      void load(const edm::Event& iEvent, const edm::EventSetup& iSetup);
      void fill();

    private :
      const EventInfoFiller * evtInfoFiller_;
      // Input from the config file
      edm::EDGetTokenT<pat::TauCollection> tauToken_;
      double        tauptMin_;
      // Map of HPS discriminator names to flags
      TauIdMap      hpsIds_;

      // Members to hold indices of tree data
      size ipt_;
      size ieta_;
      size iphi_;
      size imass_;
      size iq_;
      size idxy_;
      size idxyerr_;
      size idxysig_;
      size ileadcand_pt_;
      size ileadcand_eta_;
      size ileadcand_phi_;
      size ileadcand_mass_;
      size ileadcand_q_;
      size ileadchargedcand_pt_;
      size ileadchargedcand_eta_;
      size ileadchargedcand_phi_;
      size ileadchargedcand_mass_;
      size ileadchargedcand_q_;
      size ileadchargedcand_d0_;
      size ileadchargedcand_dz_;
      size iidflags_;
      // For raw discriminator values
      size iisodb3hitsraw_;
      size iisomvanoltraw_;
      size iisomvaltraw_;
      size iantielemvaraw_;
      size iantielemvacat_;
      size iantimumvaraw_;
      size iantimumvacat_;

    public :
      // Data members
      edm::Handle<pat::TauCollection> taus_;

  };

}

#endif
