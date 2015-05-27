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
      PFCandidateFiller(const int options,
                        const string branchName,
                        const EventInfoFiller * evtInfoFiller,
                        const edm::InputTag pfCandTag,
                        const edm::InputTag jetTag,
                        const edm::InputTag tauTag,
                        const double candptMin,
                        const double candetaMax,
                        const double taudiscMin,
                        const string tauMVAFileName_MtPresel,
                        const string tauMVAFileName_DphiPresel,
                        const string tauMVAName);
      ~PFCandidateFiller() {}

      enum Options {
                    NULLOPT      = 0,
                    SAVEALLCANDS = (1 << 0)
                   };
      static const int defaultOptions = NULLOPT;

      void load(const edm::Event& iEvent);
      void fill();

      int getContainingJetIndex(const pat::PackedCandidate* pfc);
      int getHPSTauIndex(const unsigned int pfInd);
      float getDRNearestTrack(const pat::PackedCandidate* particle, const float minTrackPt=1.0);
      float computeMT(const pat::PackedCandidate* pfc);
      float computePFIsolation(const pat::PackedCandidate* particle, const float minDR, const float maxDR, const unsigned int isotype=0, const float minNeutralPt=0.5, const float minPUPt=0.5, const float dBeta=0.5);

    private :
      const EventInfoFiller * evtInfoFiller_;
      const edm::InputTag pfCandTag_;
      const edm::InputTag jetTag_;
      const edm::InputTag tauTag_;
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
      size imt_;
      size idphimet_;
      size ichiso0p1_;
      size ichiso0p2_;
      size ichiso0p3_;
      size ichiso0p4_;
      size itotiso0p1_;
      size itotiso0p2_;
      size itotiso0p3_;
      size itotiso0p4_;
      size inearestTrkDR_;
      size icontJetIndex_;
      size icontJetDR_;
      size icontJetCSV_;
      size icontTauIndex_;
      size itaudisc_mtpresel_;
      size itaudisc_dphipresel_;

    public :
      edm::Handle<pat::PackedCandidateCollection> pfcands_;
      edm::Handle<pat::JetCollection> jets_;
      edm::Handle<pat::TauCollection> taus_;

  };

}

#endif
