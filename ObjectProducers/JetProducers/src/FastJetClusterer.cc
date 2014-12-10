#include "ObjectProducers/JetProducers/interface/FastJetClusterer.h"
#include "RecoJets/JetProducers/interface/JetSpecific.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

/*************************/
/*      Interface        */
/*************************/

using namespace ucsbsusy;
const reco::Jet::Point  FastJetClusterer::DEFAULT_VERTEX;
double                  FastJetClusterer::currentGhostArea  = -9;

//_____________________________________________________________________________
FastJetClusterer::FastJetClusterer(const FastJetClusterer& other)
  : computeArea     (other.computeArea   )
  , explicitGhosts  (other.explicitGhosts)
  , particles       (other.particles)
{
}

//_____________________________________________________________________________
FastJetClusterer& FastJetClusterer::operator=(const FastJetClusterer& other)
{
  computeArea         = other.computeArea         ;
  explicitGhosts      = other.explicitGhosts      ;

  jetDefinition       = other.jetDefinition       ;
  clusterSequence     = other.clusterSequence     ;
  subJetDefinition    = other.subJetDefinition    ;
  subClusterSequences = other.subClusterSequences ;
  subClusterOrigins   = other.subClusterOrigins   ;

  particles           = other.particles           ;
  jets                = other.jets                ;

  return *this;
}

//_____________________________________________________________________________
FastJetClusterer::~FastJetClusterer()
{
}
void FastJetClusterer::setDeterministicSeed(const unsigned int runNumber, const unsigned int eventNumber){
  unsigned int minSeed_ =14327;
  randomSeeds.resize(2);
  randomSeeds[0] = std::max(runNumber,minSeed_ + 3) + 3 * eventNumber;
  randomSeeds[1] = std::max(runNumber,minSeed_ + 5) + 5 * eventNumber;
}
//_____________________________________________________________________________
void FastJetClusterer::clusterJets( const fastjet::JetDefinition& definition, double minJetPT, double maxGhostEta, double ghostArea
                                  , double meanGhostPT, int numAreaRepeats, double ghostGridScatter, double ghostPTScatter
                                  )
{
  if (jetDefinition)
    throw cms::Exception("FastJetClusterer.clusterJets()", "Jets have already been clustered.");

  //reset the random seeds
  if(randomSeeds.size()){
    fastjet::GhostedAreaSpec gas;
    gas.set_random_status(randomSeeds);
  }

  if (subJetDefinition) {
    subJetDefinition    .reset();
    subClusterSequences .clear();
    subClusterOrigins   .clear();
  }

  jetDefinition   .reset( new fastjet::JetDefinition(definition) );
  if (jetDefinition->plugin())
    jetDefinition->delete_plugin_when_unused();

  if(computeArea){
    currentGhostArea  = ghostArea;
    fastjet::AreaDefinition  * areaDef;

    if(ghostArea <= 0)
      areaDef = new fastjet::AreaDefinition ( fastjet::VoronoiAreaSpec(-ghostArea) );
    else{
      fastjet::GhostedAreaSpec spec(maxGhostEta, numAreaRepeats, ghostArea, ghostGridScatter, ghostPTScatter, meanGhostPT);
      spec.set_fj2_placement(true);
      areaDef = new fastjet::AreaDefinition (explicitGhosts ? fastjet::active_area_explicit_ghosts : fastjet::active_area, spec);
    }
    clusterSequence .reset(new fastjet::ClusterSequenceArea( particles, *jetDefinition,*areaDef));
    delete areaDef;
  } else {
    clusterSequence .reset(new fastjet::ClusterSequence    ( particles, *jetDefinition ));
  }
  jets            = fastjet::sorted_by_pt(clusterSequence->inclusive_jets(minJetPT));
}

//_____________________________________________________________________________
void FastJetClusterer::clusterJets( fastjet::JetAlgorithm algorithm, double dCut, double minJetPT, double maxGhostEta, double ghostArea
                                  , double meanGhostPT, int numAreaRepeats, double ghostGridScatter, double ghostPTScatter
                                  )
{
  clusterJets( fastjet::JetDefinition(algorithm, dCut), minJetPT, maxGhostEta, ghostArea, meanGhostPT, numAreaRepeats, ghostGridScatter, ghostPTScatter );
}

//_____________________________________________________________________________
void FastJetClusterer::clusterJets( fastjet::JetDefinition::Plugin* plugin, double minJetPT, double maxGhostEta, double ghostArea
                                  , double meanGhostPT, int numAreaRepeats, double ghostGridScatter, double ghostPTScatter
                                  )
{
  clusterJets( fastjet::JetDefinition(plugin), minJetPT, maxGhostEta, ghostArea, meanGhostPT, numAreaRepeats, ghostGridScatter, ghostPTScatter );
}

//_____________________________________________________________________________
void FastJetClusterer::selectJets(double minJetPT, double maxJetEta)
{
  for (int iJet = jets.size(); --iJet >= 0;) {
    const fastjet::PseudoJet&           jet   = jets[iJet];
    if (jet.pt() < minJetPT || TMath::Abs(jet.eta()) > maxJetEta)
      jets.erase(jets.begin() + iJet);
  } // end loop over jets
}

/*************************/
/*   Helper Functions    */
/*************************/

//_____________________________________________________________________________
const TString& FastJetClusterer::fastJetAlgoName(fastjet::JetAlgorithm algorithm)
{
  static const TString    KT              = "kt"          ;
  static const TString    CA              = "ca"          ;
  static const TString    ANTI_KT         = "ak"          ;
  static const TString    GEN_KT          = "genKT"       ;
  static const TString    CA_PASSIVE      = "caPassive"   ;
  static const TString    GENKT_PASSIVE   = "genKTPassive";
  static const TString    EE_KT           = "eeKT"        ;
  static const TString    EE_GENKT        = "eeGenKT"     ;
  static const TString    PLUGIN          = "plugin"      ;
  static const TString    UNDEFINED       = "x"           ;


  switch (algorithm) {
  case fastjet::kt_algorithm:                     return KT;
  case fastjet::cambridge_algorithm:              return CA;
  case fastjet::antikt_algorithm:                 return ANTI_KT;
  case fastjet::genkt_algorithm:                  return GEN_KT;
  case fastjet::cambridge_for_passive_algorithm:  return CA_PASSIVE;
  case fastjet::genkt_for_passive_algorithm:      return GENKT_PASSIVE;
  case fastjet::ee_kt_algorithm:                  return EE_KT;
  case fastjet::ee_genkt_algorithm:               return EE_GENKT;
  case fastjet::plugin_algorithm:                 return PLUGIN;
  case fastjet::undefined_jet_algorithm:          return UNDEFINED;
  default:
    throw cms::Exception("FastJetClusterer::fastJetAlgoName()", "Unsupported algorithm.");
  }
}

namespace reco
{

void writeSpecific( pat::Jet& jet, reco::Particle::LorentzVector const & p4, reco::Particle::Point const & point
                  , std::vector<reco::CandidatePtr> const & constituents, edm::EventSetup const & c
                  )
{
  reco::PFJet   pfJet;
  reco::writeSpecific(pfJet, p4, point, constituents, c);
  jet           = pat::Jet(pfJet);
}

}
