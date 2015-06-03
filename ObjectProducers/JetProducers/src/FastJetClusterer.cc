#include "ObjectProducers/JetProducers/interface/FastJetClusterer.h"
#include "RecoJets/JetProducers/interface/JetSpecific.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include <fastjet/tools/Filter.hh>

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
  compoundJets        = other.compoundJets        ;
  superJets           = other.superJets           ;

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
  jets            = fastjet::sorted_by_pt(clusterSequence->inclusive_jets(std::max(minJetPT,1e-5)));
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
void    FastJetClusterer::trimJets         (const double rFilter, double trimPtFracMin, bool useTrimmedSubjets){
  fastjet::Filter trimmer( fastjet::JetDefinition(fastjet::kt_algorithm, rFilter), fastjet::SelectorPtFractionMin(trimPtFracMin));

  std::vector<fastjet::PseudoJet>                           trimmedJets;
  for ( std::vector<fastjet::PseudoJet>::const_iterator ijet = jets.begin(),
      ijetEnd = jets.end(); ijet != ijetEnd; ++ijet ) {
    fastjet::PseudoJet trimmedJet = trimmer(*ijet);
    if(!trimmedJet.has_constituents()) continue;
    if(useTrimmedSubjets){
    	std::vector<fastjet::PseudoJet>  lSubJets = trimmedJet.pieces();
    	trimmedJets.insert(trimmedJets.end(), lSubJets.begin(), lSubJets.end());
    } else trimmedJets.push_back(trimmedJet);
  }
  jets = trimmedJets;
}
// implementation from https://github.com/cms-externals/fastjet-contrib/blob/master/SubjetCounting/SubjetCounting.cc
//_____________________________________________________________________________
void FastJetClusterer::applySubjetCountingCA(const double mass_cut_off, const double ycut, const double R_min, const double pt_cut)
{

  fastjet::JetDefinition jetDef(fastjet::cambridge_algorithm, fastjet::JetDefinition::max_allowable_R);

  std::vector<CompoundPseudoJet> outputSubJets;
  nSubJetsCA.clear();

  nSubJetsCA.reserve(jets.size());

  std::vector<fastjet::PseudoJet>::const_iterator ijetBegin = jets.begin(), ijet = ijetBegin, ijetEnd = jets.end();
  int njet = -1;
  for(; ijet != ijetEnd; ++ijet) {
    njet++;
    nSubJetsCA.push_back(0);
    if(*ijet == 0) continue;
    if(!ijet->has_constituents()) continue;
    fastjet::ClusterSequence clust_seq(ijet->constituents(), jetDef);
    std::vector<fastjet::PseudoJet> ca_jets = sorted_by_pt(clust_seq.inclusive_jets());
    std::vector<fastjet::PseudoJet> output_subjets;

    findHardSubst(ca_jets[0], output_subjets, mass_cut_off, R_min, ycut);

    for(unsigned int isub = 0; isub < output_subjets.size(); ++isub) {
      if(!(output_subjets[isub].perp() > pt_cut)) continue;
      outputSubJets.emplace_back(output_subjets[isub], output_subjets[isub].constituents(), njet);
      nSubJetsCA[njet]++;
    }
  }

  compoundJets = outputSubJets;

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

//_____________________________________________________________________________
void FastJetClusterer::findHardSubst(const fastjet::PseudoJet & this_jet,
                                     std::vector<fastjet::PseudoJet> & t_parts, const double mCutoff, const double rMin, const double yCut) const
 {
  fastjet::PseudoJet parent1(0.0,0.0,0.0,0.0), parent2(0.0,0.0,0.0,0.0);
  bool had_parents=this_jet.validated_cs()->has_parents(this_jet,parent1,parent2);

  if ( (this_jet.m() < mCutoff) || (!had_parents) )
  {
  t_parts.push_back(this_jet);  return;
  } // stop recursion on this branch

  if (had_parents && parent1.plain_distance(parent2) < (rMin*rMin) )
  {
  t_parts.push_back(this_jet);  return;
  } // stop recursion on this branch

  if (parent1.perp() < parent2.perp()) std::swap(parent1,parent2);

  double pt1=parent1.perp();
  double pt2=parent2.perp();
  double totalpt=pt1+pt2;

  if (pt2>yCut*totalpt)
  {
    findHardSubst(parent1, t_parts, mCutoff, rMin, yCut);
    findHardSubst(parent2, t_parts, mCutoff, rMin, yCut);
  } // continue recursion on both branches

  else
    findHardSubst(parent1, t_parts, mCutoff, rMin, yCut);
    // continue recursion on harder branch, discarding softer branch
  return;
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
