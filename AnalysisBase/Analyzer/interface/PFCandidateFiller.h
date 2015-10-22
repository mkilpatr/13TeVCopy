//--------------------------------------------------------------------------------------------------
// 
// PFCandidateFiller
// 
// Class to fill PF candidate information.
// 
// PFCandidateFiller.h created on Fri Jan 9 13:43:01 CET 2015 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_PFCANDIDATEFILLER_H
#define ANALYSISBASE_ANALYZER_PFCANDIDATEFILLER_H

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "AnalysisTools/Utilities/interface/TreeWriterData.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/DataFormats/interface/Momentum.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisTools/ObjectSelection/interface/TauMVA.h"

namespace ucsbsusy {

  class PFCandidateFiller : public BaseFiller {

    public :
      PFCandidateFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller);
      ~PFCandidateFiller() {}

      enum Options {
                    NULLOPT      = 0,
                    SAVEALLCANDS = (1 << 0)
                   };
      static const int defaultOptions = NULLOPT;

      void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
      void fill();

      int   getContainingJetIndex(const pat::PackedCandidate* pfc);
      int   getHPSTauIndex(const unsigned int pfInd);
      int   getNearPhotonIndex(const pat::PackedCandidate* pfc);
      float getDRNearestTrack(const pat::PackedCandidate* particle, const float minTrackPt=1.0);
      float computeMT(const pat::PackedCandidate* pfc, const pat::MET* met);
      float computePFIsolation(const pat::PackedCandidate* particle, const float minDR, const float maxDR, const unsigned int isotype=0, const float minNeutralPt=0.5, const float minPUPt=0.5, const float dBeta=0.5);
      float TrackIso(const pat::PackedCandidate* particle, const float maxDR=0.3, const float deltaZCut=0.1);

    private :
      const EventInfoFiller * evtInfoFiller_;
      edm::EDGetTokenT<pat::PackedCandidateCollection> pfCandToken_;
      edm::EDGetTokenT<pat::JetCollection>             jetToken_;
      edm::EDGetTokenT<pat::TauCollection>             tauToken_;
      const double        candptMin_;
      const double        candetaMax_;
      const double        taudiscMin_;
      TauMVA*             tauMVA_MtPresel_;
      TauMVA*             tauMVA_DphiPresel_;

      size ipt_;
      size ieta_;
      size iphi_;
      size imass_;
      size iq_;
      size ipdgid_;
      size id0_;
      size idz_;
      size ifromPV_;
      size ichiso0p1_;
      size ichiso0p2_;
      size ichiso0p3_;
      size ichiso0p4_;
      size itotiso0p1_;
      size itotiso0p2_;
      size itotiso0p3_;
      size itotiso0p4_;
      size inearphopt_;
      size inearphoeta_;
      size inearphophi_;
      size inearestTrkDR_;
      size icontJetIndex_;
      size icontJetDR_;
      size icontJetCSV_;
      size itaudisc_mtpresel_;
      size itaudisc_dphipresel_;
      size itrackiso_;
    public :
      edm::Handle<pat::PackedCandidateCollection> pfcands_;
      edm::Handle<pat::JetCollection> jets_;
      edm::Handle<pat::TauCollection> taus_;

  };

}

#endif
