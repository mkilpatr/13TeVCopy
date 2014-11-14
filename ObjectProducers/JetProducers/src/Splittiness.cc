/**
  @file         Splittiness.cc
  @author       Sue Ann Koay (sakoay@cern.ch)
*/

#include <utility>

#include "ObjectProducers/JetProducers/interface/Splittiness.h"
#include "ObjectProducers/JetProducers/interface/FastJetClusterer.h"
#include "AnalysisBase/Analyzer/interface/FileUtilities.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
//#include "SUSYReferenceAnalysis2/BackgroundEstimation/interface/ShapeTools.h"


using namespace std;
using namespace ucsbsusy;

//=============================================================================
//    Construction and properties
//=============================================================================

const TString Splittiness::NSUBJETTINESS_AXES[] = { "kt"
                                                  , "ca"
                                                  , "ak2"
                                                  , "min"
                                                  , "manual"
                                                  , "1passKT"
                                                  , "1passCA"
                                                  , "1passAK2"
                                                  , "1passManual"
                                                  , ""
                                                  };
const TString Splittiness::STANDARD_JET_ALGOS[] = { "kt"
                                                  , "ca"
                                                  , "ak"
                                                  , ""
                                                  };


Splittiness::Splittiness(const edm::ParameterSet& iConfig, bool loadPartonSpread, double nSubBeta, double nSubR0, double nSubRcutoff)
  : currentRhoPT    (-9)
  , currentRhoMass  (-9)
  , currentJet      (0)
  , jetStuff        (0)

  , declusterVsDirty( FileUtilities::loadObject<ParamatrixMVA>(iConfig.getParameter<string>("mvaVsDirtyDecluster").data(), "mvaVsDirtyDecluster") )
  , declusterVsMixed( FileUtilities::loadObject<ParamatrixMVA>(iConfig.getParameter<string>("mvaVsMixedDecluster").data(), "mvaVsMixedDecluster") )
  , nsubjettiVsDirty( FileUtilities::loadObject<ParamatrixMVA>(iConfig.getParameter<string>("mvaVsDirtyNsubjetti").data(), "mvaVsDirtyNsubjetti") )
  , nsubjettiVsMixed( FileUtilities::loadObject<ParamatrixMVA>(iConfig.getParameter<string>("mvaVsMixedNsubjetti").data(), "mvaVsMixedNsubjetti") )
  , goodVsJunkSubjet( FileUtilities::loadObject<ParamatrixMVA>(iConfig.getParameter<string>("mvaVsJunkSubjets"   ).data(), "mvaVsJunkSubjets"   ) )

//  , partonSpread    ( 0 )
  , meanRhoVsEta    ( 0 )

#if 0
  , l1FastPFchs     (EventAnalyzer::getJECParameters( "AK5PFchs", iConfig.getParameter<string>("globalTag").data() ))
  , l1FastPFall     (EventAnalyzer::getJECParameters( "AK5PF"   , iConfig.getParameter<string>("globalTag").data() ))
#endif
  , nSubParams      ( iConfig.exists("beta"      ) ? iConfig.getParameter<double>("beta"      ) : nSubBeta
                    , iConfig.exists("rParameter") ? iConfig.getParameter<double>("rParameter") : nSubR0
                    , iConfig.exists("rCutoff"   ) ? iConfig.getParameter<double>("rCutoff"   ) : nSubRcutoff
                    )
  , nSubjettiness   ( iConfig.exists("nSubAxes")
                    ? getMode(iConfig.getParameter<std::string>("nSubAxes"))
                    : Njettiness::onepass_kt_axes
                    , nSubParams
                    )
{
  clog << " ++  (N-sub) beta        = " << nSubParams.beta   () << endl;
  clog << " ++  (N-sub) rParameter  = " << nSubParams.R0     () << endl;
  clog << " ++  (N-sub) rCutoff     = " << nSubParams.Rcutoff() << endl;
//clog << " ++  (N-sub) nSubAxes    = " << nSubjettiness        << endl;

  //-- Parton spread ----------------------------------------------------------
  const std::string   spreadPath    = iConfig.getParameter<string>("partonSpreadFile");
  clog << " ++  partonSpreadFile    = ";

  if (loadPartonSpread) {
//    TFile*            spreadFile    = FileUtilities::open(spreadPath.data(), "READ");
//    clog << spreadFile->GetName() << endl;
//    clog << " ++                      \\-->  -sqrt(r) :";
//    partonSpread        = new SampleEnergyTest( FileUtilities::getObject<ParamatrixF    >(spreadFile, "partonSpread"    )
//                                              , FileUtilities::getObject<ParamatrixStats>(spreadFile, "spreadStatistics")
//                                              , FileUtilities::getObject<Space          >(spreadFile, "energyFraction"  )
//                                              , &sqrtPotential
//                                              );
//    clog << "  ...  " << (partonSpread->normalizedDistance ? "[M]" : "[dR]") << endl;
//    spreadFile->Close();
  }
  else  clog << "(off)" << endl;

  
  //-- rho detector dependence ------------------------------------------------
  ParamatrixStats*    rhoDependence = FileUtilities::loadObject<ParamatrixStats>(iConfig.getParameter<string>("puRhoDependence").data(), "puRhoDependence");
  const int           indexEta      = rhoDependence->findAxis("eta");       assert(indexEta      >= 0);
  const int           indexQuantity = rhoDependence->findAxis("quantity");  assert(indexQuantity >= 0);
  const Space*        axisEta       = rhoDependence->getAxis(indexEta);
  meanRhoVsEta        = new PopulationD( new GridSpace(axisEta->GetName(), axisEta->GetTitle(), axisEta->getNumBins()-2, axisEta->getBinLowEdge(2), axisEta->getMaximum()), numPileUpRhoTypes );

  ParamatrixStats::Iterator         rhoIter = rhoDependence->iterator();
  rhoIter.fix( rhoDependence->findAxis("rho"), 0 );
  while (rhoIter.next()) {
    if (rhoIter.getBin(indexEta) < 2)       continue;

    ParamatrixStats::Iterator       refIter = rhoIter;
    refIter.setBin( indexEta, 0 );
    meanRhoVsEta->setBinContent( rhoIter.getBin(indexEta)-2, rhoIter.get()->getMean()/refIter.get()->getMean(), rhoIter.getBin(indexQuantity) );
  } // end loop over bins
  
  ////meanRhoVsEta->sitrep();


  clog << "............................................................................... " << iConfig.getParameter<std::string>("@module_type") << " @ Splittiness" << endl;
}

Splittiness::~Splittiness() 
{
  if (jetStuff)   delete jetStuff;
}

Njettiness::AxesMode  Splittiness::getMode(std::string setting){
  for(unsigned int iS = 0; NSUBJETTINESS_AXES[iS][0]; ++iS){
    if(setting == NSUBJETTINESS_AXES[iS]) return Njettiness::AxesMode(iS);
  }
  throw cms::Exception("Splittiness.getMode()", "Mode is not valid!");
}

void Splittiness::setProductIDs(const edm::ProductID& recoParticlesID, const edm::ProductID& genParticlesID)
{
  recoID  = recoParticlesID;
  genID   = genParticlesID ;
}


//=============================================================================
//    Computations
//=============================================================================

Splittiness::JetDeposition::JetDeposition( const std::vector<fastjet::PseudoJet*>& subJets, const edm::ProductID& recoID, const edm::ProductID& genID )
  : lobes         ( 2
                  , 200
                  , 10
                  , subJets.size() < 2 ? 0 : PhysicsUtilities::deltaEta( subJets[1]->eta(), subJets[0]->eta() )
                  , subJets.size() < 2 ? 0 : PhysicsUtilities::deltaPhi( subJets[1]->phi(), subJets[0]->phi() )
                  )
  , pileUpEnergy  ( 0 )
  , subJetPUEnergy( subJets.size(), 0 )
{
  assert(subJets.size() >= 2);
  const fastjet::PseudoJet&               refJet        = *subJets[0];

  for (size iSub = 0; iSub < subJets.size(); ++iSub) {
    std::vector<fastjet::PseudoJet>       constituents  = subJets[iSub]->constituents();

    for (size iCon = 0; iCon < constituents.size(); ++iCon) {
      const fastjet::PseudoJet&           constituent   = constituents[iCon];
      if (!constituent.has_user_info())   continue;

      const FastJetClusterer::UserInfo&   info          = constituent.user_info<FastJetClusterer::UserInfo>();
      if (info.type == FastJetClusterer::RECO)
        lobes.addData ( constituent.E()
                      , PhysicsUtilities::deltaEta( constituent.eta(), refJet.eta() )
                      , PhysicsUtilities::deltaPhi( constituent.phi(), refJet.phi() )
                      );
      else if (info.type != FastJetClusterer::GEN) {
        subJetPUEnergy[iSub]             += info->energy();
        pileUpEnergy                     += subJetPUEnergy[iSub];
      }
    } // end loop over constituents
    lobes.newClass();
  } // end loop over subjets

  lobes.findTwoPeaks(0, lobes.getAxisLength());


  assert(lobes.numClasses() >= 2);
  centerLocation                          = ( lobes.getStatistics(0).getMaximum() 
                                            + lobes.getStatistics(1).getMinimum() 
                                            )
                                            / 2
                                            ;
  centerValue                             = lobes.getDensity()(centerLocation);
}

//_____________________________________________________________________________
void Splittiness::cacheJetInfo(const fastjet::PseudoJet& superJet, const std::vector<fastjet::PseudoJet*>* subJets, bool forceReset) const
{
  if (!recoID.isValid())
    throw cms::Exception("Splittiness.cacheJetInfo()", "recoID is not valid -- are you sure that you have called setProductIDs() beforehand?");

  ////std::cout << "========  CACHE  ========  " << currentJet;
  if (forceReset || currentJet != &superJet) {
    currentJet            = &superJet;
    ////std::cout << " -> " << currentJet;
    currentConstituents   = superJet.constituents();
    ////std::cout << " (" << currentConstituents.size() << ")";

    if (jetStuff) {
      ////std::cout << " ... XXXX ";
      delete jetStuff;
      jetStuff            = 0;
    }
  }

  if (subJets && !jetStuff) {
    ////std::cout << " ... !!!! ";
    jetStuff              = new JetDeposition( *subJets, recoID, genID );
  }
  ////std::cout << std::endl;
}


//=============================================================================
//    Services
//=============================================================================


#if 0
//_____________________________________________________________________________
CartLorentzVector Splittiness::computePileUp(const fastjet::PseudoJet& location, double rho) const
{
  std::vector<fastjet::PseudoJet>   ghosts(1, location);
  return computePileUp(ghosts, rho);
}
#endif

//_____________________________________________________________________________
CartLorentzVector Splittiness::computePUSubtractedP4(const fastjet::PseudoJet& jet, double rhoPT, double rhoMass, double nominalArea, const PopulationD* etaScaling) const
{
  const vector<fastjet::PseudoJet>      constituents  = jet.constituents();
  double                                puSubPx       = jet.px();
  double                                puSubPy       = jet.py();
  double                                puSubPz       = jet.pz();
  double                                puSubE        = jet.E ();
  int                                   numGhosts     = 0;
  for (size iCon = 0; iCon < constituents.size(); ++iCon) {
    const fastjet::PseudoJet&           ghost         = constituents[iCon];
    if (ghost.has_user_info())          continue;
    assert(ghost.pt() < 1e-80);
    ++numGhosts;

    const double                        ptEtaScale    = etaScaling
                                                      ? etaScaling->linearInterpolate(ghost.eta(), Splittiness::RHO_PT)
                                                      : 1
                                                      ;
    const double                        massEtaScale  = etaScaling
                                                      ? etaScaling->linearInterpolate(ghost.eta(), Splittiness::RHO_MASS)
                                                      : 1
                                                      ;
    const double                        massPTratio   = sqr( (rhoMass * massEtaScale)/(rhoPT * ptEtaScale) + 1 );
    const double                        rhoScale      = ghost.area() * rhoPT * ptEtaScale;


    const double                        pzOverPT      = TMath::SinH( ghost.eta() );
    puSubPx                            -= rhoScale * TMath::Cos( ghost.phi() );
    puSubPy                            -= rhoScale * TMath::Sin( ghost.phi() );
    puSubPz                            -= rhoScale * pzOverPT;
    puSubE                             -= rhoScale * sqrt( pzOverPT*pzOverPT + massPTratio );
  } // end loop over constituents

  if (numGhosts == 0) {
    std::cerr << TString::Format( "WARNING:  No ghosts found for %d constituent jet with pT = %.4g, eta = %.4g, mass = %.4g, area = %.4g ..."
                                , int(constituents.size())
                                , jet.pt()
                                , jet.eta()
                                , jet.m()
                                , jet.area()
                                ) << std::endl;

    for (size iCon = 0; iCon < constituents.size(); ++iCon) {
      const fastjet::PseudoJet&         constituent   = constituents[iCon];
      const FastJetClusterer::UserInfo& info          = constituent.user_info<FastJetClusterer::UserInfo>();
      std::cerr << TString::Format( " [%4s  %-5s]  pT = %10.4g (%8.4g), eta = %8.3g, phi = %8.3g, mass = %8.4g"
                                  , info.type == FastJetClusterer::RECO
                                  ? "RECO" 
                                  : info.type == FastJetClusterer::GEN
                                  ? "GEN"
                                  : "JUNK"
                                  , info.type == FastJetClusterer::RECO
                                  ? ParticleInfo::pfTypeNames()[dynamic_cast<const reco::PFCandidate*>( info.get() )->particleId()].Data()
                                  : ParticleInfo::nameFor(info->pdgId()).Data()
                                  , constituent.pt()
                                  , info->pt()
                                  , constituent.eta()
                                  , constituent.phi()
                                  , constituent.m()
                                  ) << std::endl;

      const double                      ptEtaScale    = etaScaling
                                                      ? etaScaling->linearInterpolate(constituent.eta(), Splittiness::RHO_PT)
                                                      : 1
                                                      ;
      const double                      massEtaScale  = etaScaling
                                                      ? etaScaling->linearInterpolate(constituent.eta(), Splittiness::RHO_MASS)
                                                      : 1
                                                      ;
      const double                      massPTratio   = sqr( (rhoMass * massEtaScale)/(rhoPT * ptEtaScale) + 1 );
      const double                      rhoScale      = nominalArea * rhoPT * ptEtaScale;


      const double                      pzOverPT      = TMath::SinH( constituent.eta() );
      puSubPx                          -= rhoScale * TMath::Cos( constituent.phi() );
      puSubPy                          -= rhoScale * TMath::Sin( constituent.phi() );
      puSubPz                          -= rhoScale * pzOverPT;
      puSubE                           -= rhoScale * sqrt( pzOverPT*pzOverPT + massPTratio );
    } // end loop over constituents

    if (nominalArea < 0)
      throw cms::Exception("Splittiness.computePUSubtractedP4()", "Encountered jet with no ghosts when nominalArea is negative.");
    std::cerr << TString::Format( "           ... Pile-up subtraction computed using constituent positions each with nominal area = %.4g.", nominalArea) << std::endl;
  }

  return CartLorentzVector( puSubPx, puSubPy, puSubPz, puSubE );
}


//=============================================================================
//    Declustering
//=============================================================================


//_____________________________________________________________________________
double Splittiness::getDeclusterSubjets(const fastjet::PseudoJet& superJet, std::vector<fastjet::PseudoJet*>& subJets) const
{
  subJets.push_back(new fastjet::PseudoJet);
  subJets.push_back(new fastjet::PseudoJet);
  if (!superJet.has_parents(*subJets[0], *subJets[1]))
    PhysicsUtilities::trash(subJets);

  return subJets.size();
}



//=============================================================================
//    N-subjettiness
//=============================================================================


//_____________________________________________________________________________
double Splittiness::getNSubjettinessSubjets(const fastjet::PseudoJet& superJet, std::vector<fastjet::PseudoJet*>& subJets) const
{
  cacheJetInfo(superJet, 0, true);
  if (currentConstituents.size() < 3)   return false;

  const double    tau2  = nSubjettiness.getTau(2, currentConstituents);
  nSubjettiness.getJets(currentConstituents, subJets);

  return tau2;
}

//_____________________________________________________________________________
bool Splittiness::shouldCleanNSubjettiness(const fastjet::PseudoJet& superJet, std::vector<fastjet::PseudoJet*>& subJets, double tau2, double * discResult) const
{
  assert(subJets.size() == 2);
  cacheJetInfo(superJet, &subJets);

  static const int            indexJetPT    = nsubjettiVsDirty->findAxis("jet_pt"  );
  static const Space*         axisJetPT     = nsubjettiVsDirty->getAxis (indexJetPT);
  std::vector<double>         parameters    ( nsubjettiVsDirty->getNumParams() );
  parameters[indexJetPT]      = superJet.pt();
  if (parameters[indexJetPT] < axisJetPT->getMinimum())
    return false;

  const Panvariate*           mvaReader     = nsubjettiVsDirty->get(parameters);
  if (!mvaReader)             throw cms::Exception("Splittiness.shouldCleanNSubjettiness()", TString::Format("No discriminator available for jet with pT = %.4g", parameters[indexJetPT]).Data());

  static const int            indexSub2PU   = mvaReader->findVariable("sub2_pu_over_main_e");   assert(indexSub2PU    >= 0);
  static const int            indexSub2PT   = mvaReader->findVariable("sub2_pt_ratio"      );   assert(indexSub2PT    >= 0);
  static const int            indexHPeakVal = mvaReader->findVariable("highest_peak"       );   assert(indexHPeakVal  >= 0);
  static const int            indexLPeakVal = mvaReader->findVariable("lowest_peak"        );   assert(indexLPeakVal  >= 0);
  static const int            indexCenterVal= mvaReader->findVariable("center_value"       );   assert(indexCenterVal >= 0);
  static const int            indexSub12DR  = mvaReader->findVariable("sub12_dr"           );   assert(indexSub12DR   >= 0);

  const fastjet::PseudoJet&   subJet1       = *subJets[0];
  const fastjet::PseudoJet&   subJet2       = *subJets[1];
  
  mvaReader->setVariable( indexSub2PU   , jetStuff->subJetPUEnergy[1]/subJet2.E()                               );
  mvaReader->setVariable( indexSub2PT   , subJet2.pt() / parameters[indexJetPT]                                 );
  mvaReader->setVariable( indexHPeakVal , jetStuff->lobes.getHighestPeak()                                      );
  mvaReader->setVariable( indexLPeakVal , jetStuff->lobes.isUnimodal() ? -0.1 : jetStuff->lobes.getLowestPeak() );
  mvaReader->setVariable( indexCenterVal, jetStuff->centerValue                                                 );
  mvaReader->setVariable( indexSub12DR  , subJet1.delta_R(subJet2)                                              );
  
  static const PopulationD    discriCuts(const_cast<Space*>(axisJetPT), "0.822799,0.876514,0.875456,0.88095,0.879904,0.862254,0.857595,0.814076,0.769608,0.846078,0.70197,0.865599");  // 38% S, 1% B
//static const PopulationD    discriCuts(const_cast<Space*>(axisJetPT), "0.777031,0.825374,0.814386,0.796273,0.773939,0.751922,0.740191,0.679816,0.654231,0.717148,0.625557,0.81015" );  // 48% S, 2% B

  double result = mvaReader->evaluateMethod(0);
  if(discResult) (*discResult) = result;

  if ( result > discriCuts.at(parameters[indexJetPT]) ) {
    delete subJets[1];
    subJets[1]                = 0;
    return true;
  }
  return false;
}

//_____________________________________________________________________________
bool Splittiness::shouldSplitNSubjettiness(const fastjet::PseudoJet& superJet, const std::vector<fastjet::PseudoJet*>& subJets, double nSubjettinessTau2, double * discResult) const
{
  assert(subJets.size() == 2);
  cacheJetInfo(superJet, &subJets);

  static const int            indexJetPT    = nsubjettiVsMixed->findAxis("jet_pt"  );
  static const Space*         axisJetPT     = nsubjettiVsMixed->getAxis (indexJetPT);
  std::vector<double>         parameters    ( nsubjettiVsMixed->getNumParams() );
  parameters[indexJetPT]      = superJet.pt();
  if (parameters[indexJetPT] < axisJetPT->getMinimum())
    return false;

  const Panvariate*           mvaReader     = nsubjettiVsMixed->get(parameters);
  if (!mvaReader)             throw cms::Exception("Splittiness.shouldSplitNSubjettiness()", TString::Format("No discriminator available for jet with pT = %.4g", parameters[indexJetPT]).Data());

  static const int            indexJetMass  = mvaReader->findVariable("jet_mass"             );   assert(indexJetMass   >= 0);
  static const int            indexHPeakVal = mvaReader->findVariable("highest_peak"         );   assert(indexHPeakVal  >= 0);
  static const int            indexLPeakVal = mvaReader->findVariable("lowest_peak"          );   assert(indexLPeakVal  >= 0);
  static const int            indexMinVal   = mvaReader->findVariable("minimum_value"        );   assert(indexMinVal    >= 0);
  static const int            indexLPeakLoc = mvaReader->findVariable("lowest_peak_location" );   assert(indexLPeakLoc  >= 0);
  static const int            indexHPeakLoc = mvaReader->findVariable("highest_peak_location");   assert(indexHPeakLoc  >= 0);
  static const int            indexMinLoc   = mvaReader->findVariable("minimum_location"     );   assert(indexMinLoc    >= 0);
  static const int            indexNSubTau1 = mvaReader->findVariable("1_subjettiness"       );   assert(indexNSubTau1  >= 0);
  static const int            indexNSubTau2 = mvaReader->findVariable("2_subjettiness"       );   assert(indexNSubTau2  >= 0);
  static const int            indexSub12DR  = mvaReader->findVariable("sub12_dr"             );   assert(indexSub12DR   >= 0);

  const fastjet::PseudoJet&   subJet1       = *subJets[0];
  const fastjet::PseudoJet&   subJet2       = *subJets[1];
  
  mvaReader->setVariable( indexJetMass  , superJet.m()                                                            );
  mvaReader->setVariable( indexHPeakVal , jetStuff->lobes.getHighestPeak()                                        );
  mvaReader->setVariable( indexLPeakVal , jetStuff->lobes.isUnimodal() ? -0.1 : jetStuff->lobes.getLowestPeak  () );
  mvaReader->setVariable( indexMinVal   , jetStuff->lobes.isUnimodal() ? -0.1 : jetStuff->lobes.getMinimumValue() );
  mvaReader->setVariable( indexLPeakLoc , jetStuff->lobes.getLowestLocation () - jetStuff->centerLocation         );
  mvaReader->setVariable( indexHPeakLoc , jetStuff->lobes.getHighestLocation() - jetStuff->centerLocation         );
  mvaReader->setVariable( indexMinLoc   , jetStuff->lobes.getMinimumLocation() - jetStuff->centerLocation         );
  mvaReader->setVariable( indexNSubTau1 , nSubjettiness.getTau(1, currentConstituents)                            );
  mvaReader->setVariable( indexNSubTau2 , nSubjettinessTau2                                                       );
  mvaReader->setVariable( indexSub12DR  , subJet1.delta_R(subJet2)                                                );

//static const PopulationD    discriCuts(const_cast<Space*>(axisJetPT), "0.999225,0.996505,0.794059,0.698006,0.629486,0.382803,-0.0369979,-0.454534,-0.698265,-0.853396,-0.919803,-0.837683");   // 95% S, 5% B
  static const PopulationD    discriCuts(const_cast<Space*>(axisJetPT), "0.998606,0.838187,0.682839,0.432167,0.24949,-0.211865,-0.647571,-0.795921,-0.905484,-0.956958,-0.963985,-0.840577" );   // 97% S, 9% B

  double result = mvaReader->evaluateMethod(0);
  if(discResult) (*discResult) = result;

  return ( result > discriCuts.at(parameters[indexJetPT]) );
}


//=============================================================================
//    Jet Trimming
//=============================================================================

//_____________________________________________________________________________
void Splittiness::eraseTrailingJunkSubjets(const fastjet::PseudoJet& superJet, size superIndex, std::vector<fastjet::PseudoJet>& subJets) const
{
  if (subJets.empty())            return;

  static const int                indexJetPT      = goodVsJunkSubjet->findAxis("jet_pt"  );
  std::vector<double>             parameters      ( goodVsJunkSubjet->getNumParams() );
  parameters[indexJetPT]          = superJet.pt();

  const Panvariate*               mvaReader       = goodVsJunkSubjet->get(parameters);
  if (!mvaReader)                 throw cms::Exception("Splittiness.shouldClean()", TString::Format("No discriminator available for jet with pT = %.4g", parameters[indexJetPT]).Data());

  static const int                indexNSubjets   = mvaReader->findVariable("num_subjets"       );   assert(indexNSubjets  >= 0);
  static const int                indexMeanDR     = mvaReader->findVariable("mean_sub_dr_so_far");   assert(indexMeanDR    >= 0);
  static const int                indexMassSoFar  = mvaReader->findVariable("mass_so_far"       );   assert(indexMassSoFar >= 0);
  static const int                indexRemainPT   = mvaReader->findVariable("pt_remaining"      );   assert(indexRemainPT  >= 0);
  static const int                indexSubPT      = mvaReader->findVariable("sub_pt"            );   assert(indexSubPT     >= 0);
  mvaReader->setVariable( indexNSubjets, subJets.size() );


  //-- Precomputed quantities -------------------------------------------------
  std::vector<RankedIndex>        ptOrdered(subJets.size());
  for (size iSub = 0; iSub < subJets.size(); ++iSub) {
    ptOrdered[iSub].first         = subJets[iSub].pt2();
    ptOrdered[iSub].second        = iSub;
  } // end loop over subJets
  std::sort(ptOrdered.begin(), ptOrdered.end(), greaterFirst<double,int>());

  std::vector<CartLorentzVector>  remainingP4(ptOrdered.size()+1);
  for (int iRank = int(ptOrdered.size()) - 1; --iRank > 0;) {
    remainingP4[iRank]            = remainingP4[iRank+1];
    PhysicsUtilities::addTo( remainingP4[iRank], subJets[ptOrdered[iRank+1].second] );
  } // end loop over subjets


  //-- Find best subset of leading subjets to keep ----------------------------
  CartLorentzVector               sumP4soFar;
  PStatistics                     dRfromJetStats;
  double                          bestDiscri      = -1e308;
  size                            bestRankToKeep  = 0;
  std::vector<double>             discriminators  (ptOrdered.size());
  for (size iRank = 0; iRank < ptOrdered.size(); ++iRank) {
    const fastjet::PseudoJet&     subJet          = subJets[ptOrdered[iRank].second];
    PhysicsUtilities::addTo             ( sumP4soFar                     , subJet      );
    dRfromJetStats.add( PhysicsUtilities::deltaR(subJet, superJet) , subJet.pt() );

    mvaReader->setVariable( indexMeanDR    , dRfromJetStats.getMean() );
    mvaReader->setVariable( indexMassSoFar , sumP4soFar    .mass   () );
    mvaReader->setVariable( indexRemainPT  , remainingP4[iRank].pt()  );
    mvaReader->setVariable( indexSubPT     , subJet        .pt     () );

    discriminators[iRank]         = mvaReader->evaluateMethod(0);
    if (discriminators[iRank] > bestDiscri) {
      bestDiscri                  = discriminators[iRank];
      bestRankToKeep              = iRank;
    }
  } // end loop over subJets

  //-- Copy only kept subjets -------------------------------------------------
  std::vector<fastjet::PseudoJet> keptSubjets;
  keptSubjets.reserve(bestRankToKeep + 1);
  for (size iRank = 0; iRank <= bestRankToKeep; ++iRank)
    keptSubjets.push_back( subJets[ptOrdered[iRank].second] );
  subJets.swap(keptSubjets);
}

//_____________________________________________________________________________
void Splittiness::erasePULikeSubjets(const fastjet::PseudoJet& superJet, size superIndex, std::vector<fastjet::PseudoJet>& subJets) const
{
  if (subJets.empty())            return;

  //-- Special treatment for trivial particles --------------------------------
  std::vector<bool>               self(subJets.size(), false);
  for (int iSub = subJets.size(); --iSub >= 0;) {
    const fastjet::PseudoJet&     subJet          = subJets[iSub];
    if (subJet.pt() > 1e-10)      continue;

    self[iSub]                    = true;
    double                        nearestDR;
    const int                     iNearest        = PhysicsUtilities::findNearestDR(subJet, subJets, nearestDR, 1e308, 0, &self);
    self[iSub]                    = false;
    assert(iNearest >= 0);

    subJets[iNearest]             = fastjet::join( subJets[iNearest] , subJet );
    subJets.erase( subJets.begin() + iSub );
  } // end loop over subJets


  //-- Precomputed quantities -------------------------------------------------
  std::vector<RankedIndex>        ordering(subJets.size());
  for (size iSub = 0; iSub < subJets.size(); ++iSub) {
    ordering[iSub].first          = subJets[iSub].pt2();
    ordering[iSub].second         = iSub;
  } // end loop over subJets

  static  greaterFirst<double,int>  COMPARATOR;
  std::sort(ordering.begin(), ordering.end(), COMPARATOR);


  //-- Find best subset of leading subjets to keep ----------------------------
  CartLorentzVector               puSubP4         = computePUSubtractedP4(superJet, currentRhoPT, currentRhoMass, FastJetClusterer::getCurrentGhostArea() /*, meanRhoVsEta*/);
  double                          bestDistance    = 1e308;
  size                            bestRankToKeep  = 0;
  CartLorentzVector               sumP4soFar;
  for (size iRank = 0; iRank < ordering.size(); ++iRank) {
    const fastjet::PseudoJet&     subJet          = subJets[ordering[iRank].second];
    PhysicsUtilities::addTo( sumP4soFar, subJet );

    const double                  distance        = sqr(sumP4soFar.px() - puSubP4.px())
                                                  + sqr(sumP4soFar.py() - puSubP4.py())
                                                  + sqr(sumP4soFar.pz() - puSubP4.pz())
                                                  + sqr(sumP4soFar.M () - puSubP4.M ())
                                                  ;
    if (distance < bestDistance) {
      bestDistance                = distance;
      bestRankToKeep              = iRank;
    }
  } // end loop over subJets

  
  //-- Copy only kept subjets -------------------------------------------------
  std::vector<fastjet::PseudoJet> keptSubjets;
  keptSubjets.reserve(bestRankToKeep + 1);
  for (size iRank = 0; iRank <= bestRankToKeep; ++iRank)
    keptSubjets.push_back( subJets[ordering[iRank].second] );
  subJets.swap(keptSubjets);
}

