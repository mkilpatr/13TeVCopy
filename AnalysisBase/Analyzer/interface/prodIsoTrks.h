#include <memory>
#include <algorithm>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/GenMET.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "TLorentzVector.h"

// additional headers
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "TString.h"
#include "TTree.h"
#include <TFile.h>
#include <vector>
#include "Math/GenVector/LorentzVector.h"
#include <DataFormats/Math/interface/deltaR.h>
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"
//#include "AnalysisBase/Analyzer/interface/prodIsoTrksTest.h"

using namespace ucsbsusy;
using namespace std;

class prodIsoTrks : public edm::EDFilter {
//class prodIsoTrks : public PhysicsAnalyzer {

  public:
    FILTER_MODE;
    explicit prodIsoTrks(const edm::ParameterSet & iConfig);
    ~prodIsoTrks();
  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag vtxSrc_, metSrc_;

    edm::InputTag forVetoIsoTrkSrc_;
    edm::Handle<pat::PackedCandidateCollection> forVetoIsoTrks_;

    double isotrk_dR_, isotrk_dz_;

    edm::InputTag pfCandSrc_, loose_isoTrkSrc_, loose_isotrk_isoVecSrc_, loose_isotrk_dzpvVecSrc_;
    edm::Handle<pat::PackedCandidateCollection> pfCandHandle_, loose_isoTrksHandle_;

    //edm::InputTag ref_all_isoTrkSrc_, ref_all_isoTrk_isoVecSrc_;
    //edm::Handle<pat::PackedCandidateCollection> ref_all_isoTrksHandle_;
    //edm::Handle<std::vector<double> > ref_all_isoTrks_isoVecHandle_;

    //edm::InputTag W_emuVec_Src_, W_tau_emuVec_Src_, W_tau_prongsVec_Src_;
    //edm::Handle<std::vector<int> > W_emuVec_, W_tau_emuVec_, W_tau_prongsVec_;

    //edm::InputTag genDecayLVec_Src_;
    //edm::Handle<std::vector<TLorentzVector> > genDecayLVec_;

    //edm::EDGetTokenT<std::vector<int> >W_EmVec_Tok_;
    //edm::EDGetTokenT<std::vector<int> >W_Tau_EmuVec_Tok_;
    //edm::EDGetTokenT<std::vector<int> >W_Tau_ProngsVec_Tok_;
    //edm::EDGetTokenT<std::vector<TLorentzVector> >GenDecayLVec_Tok_;
    edm::EDGetTokenT<pat::PackedCandidateCollection>PfCandTok_;
    edm::EDGetTokenT<pat::PackedCandidateCollection> Loose_IsoTrksHandle_Tok_;
    //edm::EDGetTokenT<pat::PackedCandidateCollection> Ref_All_IsoTrksHandle_Tok_;
    //edm::EDGetTokenT<std::vector<double> > Ref_All_IsoTrks_IsoVecHandle_Tok_;
    edm::EDGetTokenT< std::vector<reco::Vertex> >VtxTok_;
    edm::EDGetTokenT<edm::View<reco::MET> >MetTok_;
    edm::EDGetTokenT<std::vector<double> >  Loose_Isotrk_IsoVecHandle_Tok_;
    edm::EDGetTokenT<std::vector<double> >Loose_Isotrk_DzpvVecHandle_Tok_;
    edm::EDGetTokenT<pat::PackedCandidateCollection> ForVetoIsoTrks_Tok_;

    unsigned int loose_nIsoTrks, nIsoTrksForVeto;

    bool debug_;

    int find_idx(const reco::Candidate & target);
    int find_idx(int genIdx, const std::vector<int> &genDecayIdxVec);

    bool isData_;

    double GetTrackActivity(const edm::Handle<pat::PackedCandidateCollection> & other_pfcands, const pat::PackedCandidate* track);


};


