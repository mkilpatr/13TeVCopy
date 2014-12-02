/*
 * JetFlavorMatching.cc
 *
 *  Created on: Aug 27, 2014
 *      Author: nmccoll
 */

#include "AnalysisTools/Utilities/interface/JetFlavorMatching.h"
#include "AnalysisTools/Utilities/interface/ParticleUtilities.h"

using namespace std;
using namespace JetFlavorMatching;


vector<HadronDecay> JetFlavorMatching::getBHadronDecays(const edm::Handle<vector<reco::GenParticle> >& particles)
{
  vector<HadronDecay>                     bDecays;
  vector<reco::GenParticleRef>            bQuarks;
  vector<int>                             bHadronNumBs;

  //-- Get last b quarks and first B hadrons ----------------------------------
  for (size iPtcl = 0; iPtcl < particles->size(); ++iPtcl) {
    const reco::GenParticle&              particle      = (*particles)[iPtcl];
    const int                             pdgId         = TMath::Abs(particle.pdgId());

    //if they are b-quarks...lets get the last one
    if (pdgId == ParticleInfo::p_b &&  ParticleUtilities::isLastInChain(&particle)) {
      bQuarks.push_back(reco::GenParticleRef(particles, iPtcl));
      continue;
    }

    //if it is a b-hadron we want the first guy (so we can get everything that radiates off it it)
    int                                   numBLines     = 0;
    if (ParticleInfo::isBHadron(ParticleInfo::typeOfHadron(pdgId, &numBLines)) &&  ParticleUtilities::isFirstInChain(&particle)){
      //if his mother is another bhadron...continue...we want the first bhadron
      int numBHadMoms = 0;
      for(unsigned int iMom = 0; iMom < particle.numberOfMothers(); ++iMom){
        if (ParticleInfo::isBHadron(ParticleInfo::typeOfHadron(particle.mother(iMom)->pdgId()))) numBHadMoms++;
      }
      if(numBHadMoms) continue;
      bDecays     .push_back(HadronDecay(particles, iPtcl, numBLines));
      bHadronNumBs.push_back(numBLines);
    }
  }

  //Now make sure that every quark has is associated to a b-hadron
  for (size iPtcl = 0; iPtcl < bQuarks.size(); ++iPtcl) {
    int numBHad = 0;
    for(unsigned int iD = 0; iD < bQuarks[iPtcl]->numberOfDaughters(); ++iD){
     if(ParticleInfo::isBHadron(ParticleInfo::typeOfHadron(bQuarks[iPtcl]->daughter(iD)->pdgId()))) numBHad++;
    }
    if(numBHad != 1){
      ParticleUtilities::printGenInfo((*particles),-1);
      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Failed to obtain proper number of bHadrons (%d) for a b-quark (index %d).",numBHad, bQuarks[iPtcl].key() ).Data());
    }
  }

  //Now make sure that every b-hadron is matched to a b-quark...plus do association
  for (size iHad = 0; iHad < bDecays.size(); ++iHad) {
    const reco::GenParticle & had = *bDecays[iHad].hadron;
    for(unsigned int iMom = 0; iMom < had.numberOfMothers(); ++iMom){
      const reco::GenParticleRef&  mom = had.motherRef(iMom);
      if(TMath::Abs(mom->pdgId()) != ParticleInfo::p_b  ) continue;

      //If the b-quark has another b-quark as a daughter...this means that the linking is messed up from the pruned gen particle
      //pruner...let's skip it and use the final version
      if(!ParticleUtilities::isLastInChain(&(*mom))) continue;

      bDecays[iHad].quark.emplace_back(particles,mom.key());
    }
    if(bDecays[iHad].quark.size() != bDecays[iHad].numQuarksInHad){
      ParticleUtilities::printGenInfo((*particles),-1);
      cout << "JetFlavorMatching.getBHadronDecays() :: Associated bQuarks ->  ";
      for(unsigned int iQ = 0; iQ < bDecays[iHad].quark.size(); ++iQ) cout << bDecays[iHad].quark[iQ].key() <<" ";
      cout << endl;
      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Failed to obtain proper number of bQuarks (%d) for a b-hadron (index %d).",int(bDecays[iHad].quark.size()) , bDecays[iHad].hadron.key() ).Data());
    }
  }

  return bDecays;
}


//  for (size iDau = 0; iDau < particle.numberOfDaughters(); ++iDau) {
//    ParticleInfo::HadronType hadType = ParticleInfo::typeOfHadron(particle.daughter(iDau)->pdgId());
//    if(!ParticleInfo::isBHadron(hadType)) continue;
//  }


//  for (size iPtcl = 6; iPtcl < particles->size(); ++iPtcl) {
//
//    if (particle.status() == ParticleInfo::status_doc)  continue;
//
//    //.. Require b quarks with no b daughters .................................
//    if (particle.status() == ParticleInfo::status_decayed && pdgId == ParticleInfo::p_b) {
//      if (ParticleUtilities::isLastInChain(&particle)) {
//        bQuarks.push_back(reco::GenParticleRef(particles, iPtcl));
////        size                              maxIndex      = iPtcl;
//        for (size iDau = 0; iDau < particle.numberOfDaughters(); ++iDau) {
//          const reco::Candidate*          daughter      = particle.daughter(iDau);
////          maxIndex                        = TMath::Max(maxIndex, particle.daughterRef(iDau).key());
//          if (!ParticleInfo::isHadronizationModel(daughter->pdgId())) continue;
//          if (bHadronizations.size() >= bQuarks.size())
//            throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "More than one link encountered in hadronization of a b-quark.");
//          bHadronizations.push_back(reco::GenParticleRef(particles, particle.daughterRef(iDau).key()));
//        } // end loop over daughters
//        if (bHadronizations.size() != bQuarks.size()) {
//          ParticleUtilities::printGenInfo(*particles, -1);
//          throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Failed to obtain hadronization link for a b-quark (index %d).", iPtcl).Data());
//        }
//      }
//      continue;
//    }
//
//    //.. Require B hadrons with string mother .................................
//    int                                   numBLines     = 0;
//    if (ParticleInfo::isBHadron(ParticleInfo::typeOfHadron(pdgId, &numBLines))) {
//      if (particle.status() != ParticleInfo::status_decayed)
//        throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "Encountered an undecayed B hadron.");
//      if (particle.numberOfMothers() != 1)
//        throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Encountered a B hadron with %d mothers.",int(particle.numberOfMothers())).Data());
//
//      const int                           motherID      = TMath::Abs(particle.mother()->pdgId());
//      if (ParticleInfo::isHadronizationModel(motherID)) {
//        bDecays     .push_back(HadronDecay(particles, iPtcl));
//        bHadronNumBs.push_back(numBLines);
//      }
//      else if (!ParticleInfo::isBHadron(ParticleInfo::typeOfHadron(motherID)))
//        throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Encountered a B hadron with a %s as mother.",ParticleInfo::nameFor(motherID).Data()).Data());
//    }
//  } // end loop over particles
//
//
//  //-- Link quarks to hadrons -------------------------------------------------
//  // First get all the quarks and hadrons that end up/originate from the same string
//  static const vector<size>               NIL;
//  vector<const reco::Candidate*>          strings;
//  vector<vector<size> >                   stringQuarks;
//  vector<vector<size> >                   stringHadrons;
//  vector<int>                             stringBCount;
//
//  for (size iHadron = 0; iHadron < bDecays.size(); ++iHadron) {
//    const reco::Candidate*                reference     = bDecays[iHadron].hadron->mother();
//    size                                  iString       = 0;
//    while (iString < strings.size() && strings[iString] != reference)
//      ++iString;
//    if (iString >= strings.size())      { strings.push_back(reference);   stringQuarks.push_back(NIL);  stringHadrons.push_back(NIL);   stringBCount.push_back(0);  }
//    stringHadrons[iString].push_back(iHadron);
//    stringBCount [iString]               += bHadronNumBs[iHadron];
//  } // end loop over hadrons
//
//  for (size iQuark = 0; iQuark < bQuarks.size(); ++iQuark) {
//    const reco::Candidate*                reference     = bHadronizations[iQuark].get();
//    size                                  iString       = 0;
//    while (iString < strings.size() && strings[iString] != reference)
//      ++iString;
//    if (iString >= strings.size())      { strings.push_back(reference);   stringQuarks.push_back(NIL);  stringHadrons.push_back(NIL);   stringBCount.push_back(0);  }
//    stringQuarks[iString].push_back(iQuark);
//  } // end loop over quarks
//
//  // Now disambiguate by assigning the closest pairs first
//  ////cout << "_______________________________________________________________________________" << endl;
//  for (size iString = 0; iString < strings.size(); ++iString) {
//    const vector<size>&                   assocQuarks   = stringQuarks [iString];
//    const vector<size>&                   assocHadrons  = stringHadrons[iString];
//    if (assocQuarks .empty())             throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "Encountered a string with no associated b quarks.");
//    if (assocHadrons.empty())             throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "Encountered a string with no associated B hadrons.");
//    if (stringBCount[iString] != int(assocQuarks.size()))
//      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Mismatch in number of b quarks (%d) and expected b content in hadrons (%d).", int(assocQuarks.size()), stringBCount[iString]).Data());
//
//    ////cout << "b:";
//    ////for (size iQ = 0; iQ < assocQuarks.size(); ++iQ)
//    ////  cout << TString::Format(" (pT=%8.4g, id=%5d)", bQuarks[assocQuarks[iQ]]->pt(), bQuarks[assocQuarks[iQ]]->pdgId());
//    ////cout << endl;
//    ////cout << "B:";
//    ////for (size iH = 0; iH < assocHadrons.size(); ++iH)
//    ////  cout << TString::Format(" (pT=%8.4g, id=%5d)", bDecays[assocHadrons[iH]].hadron->pt(), bDecays[assocHadrons[iH]].hadron->pdgId());
//    ////cout << endl;
//    ////cout << " -> ";
//
//
//    // Compute distances between all pairs of quarks and hadrons
//    vector<vector<double> >               dRQuarkHadron (assocQuarks.size());
//    for (size iQ = 0; iQ < assocQuarks.size(); ++iQ) {
//      vector<double>&                     dRHadron      = dRQuarkHadron[iQ];
//      dRHadron.resize(assocHadrons.size());
//      for (size iH = 0; iH < assocHadrons.size(); ++iH)
//        dRHadron[iH]                      = PhysicsUtilities::deltaR(*bQuarks[assocQuarks[iQ]], *bDecays[assocHadrons[iH]].hadron);
//    } // end loop over quarks
//
//    // Assign closest first, allowing as many assignment to hadrons as the b content according to the PDG ID
//    vector<bool>                          usedQuark     (assocQuarks .size(), false);
//    vector<bool>                          usedHadron    (assocHadrons.size(), false);
//    for (size numAssigned = 0; numAssigned < assocQuarks.size(); ++numAssigned) {
//      int                                 nearestQuark  = -1;
//      int                                 nearestHadron = -1;
//      for (size iQ = 0; iQ < assocQuarks.size(); ++iQ) {
//        if (usedQuark[iQ])                continue;
//        for (size iH = 0; iH < assocHadrons.size(); ++iH) {
//          if (usedHadron[iH])             continue;
//          if (nearestQuark < 0 || dRQuarkHadron[iQ][iH] < dRQuarkHadron[nearestQuark][nearestHadron]) {
//            nearestQuark                  = iQ;
//            nearestHadron                 = iH;
//          }
//        } // end loop over hadrons
//      } // end loop over quarks
//      if (nearestQuark < 0 || dRQuarkHadron[nearestQuark][nearestHadron] > 1e307)
//        throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "Failed to find nearest quark-hadron pair.");
//
//      ////cout << "(" << nearestQuark << "," << nearestHadron << ")";
//      const size                          iHadron       = assocHadrons[nearestHadron];
//      HadronDecay&                        decay         = bDecays[iHadron];
//      decay.quark.push_back(bQuarks[assocQuarks[nearestQuark]]);
//      if (int(decay.quark.size()) >= bHadronNumBs[iHadron])
//        usedHadron[nearestHadron]         = true;
//      usedQuark[nearestQuark]             = true;
//    } // end loop over assignments
//    ////cout << endl;
//  } // end loop over strings
//
//
//  //-- Final round to deduce hadron decay products ----------------------------
//  for (size iHadron = 0; iHadron < bDecays.size(); ++iHadron) {
//    HadronDecay&                          decay         = bDecays[iHadron];
//    if (int(decay.quark.size()) != bHadronNumBs[iHadron])
//      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Mismatch in number of matched b quarks (%d) and expected b content (%d) for hadron (pdgId = %d).", int(decay.quark.size()), bHadronNumBs[iHadron], decay.hadron->pdgId()).Data());
//
//    getDecayProducts(decay.hadron.key(), particles, decay.decay, decay.sumVisible, decay.sumCharged, decay.numVisible, decay.numCharged);
//    if (decay.decay.empty())
//      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Encountered an undecayed hadron (pdgId = %d, pT = %.4g, eta = %.4g).", decay.hadron->pdgId(), decay.hadron->pt(), decay.hadron->eta()).Data());
//
//    if (decay.sumVisible.energy() > 0.01 + decay.hadron->energy()) {
//      cerr << endl;
//      cerr << "!!!!!!  " << TString::Format("pt = %8.4g, eta = %8.3g, phi = %8.3g, mass = %8.4g, E = %8.4g", decay.hadron->pt(), decay.hadron->eta(), decay.hadron->phi(), decay.hadron->mass(), decay.hadron->energy()) << endl;
//      cerr << " -vs-   " << TString::Format("pt = %8.4g, eta = %8.3g, phi = %8.3g, mass = %8.4g, E = %8.4g", decay.sumVisible.pt(), decay.sumVisible.eta(), decay.sumVisible.phi(), decay.sumVisible.mass(), decay.sumVisible.energy()) << endl;
//      cerr << endl;
//      throw cms::Exception("EventAnalyzer.getBHadronDecays()", TString::Format("Visible energy (%.8g) exceeds total energy (%.8g) for hadron (pdgId = %d).", decay.sumVisible.energy(), decay.hadron->energy(), decay.hadron->pdgId()).Data());
//    }
//  } // end loop over hadrons
//
//
//#define SANITY_CHECK
//#ifdef SANITY_CHECK
//  for (size iQuark = 0; iQuark < bQuarks.size(); ++iQuark) {
//    int                                   numAssigns    = 0;
//    for (size iHadron = 0; iHadron < bDecays.size(); ++iHadron)
//      for (size iQ = 0; iQ < bDecays[iHadron].quark.size(); ++iQ)
//        if (bQuarks[iQuark] == bDecays[iHadron].quark[iQ])
//          ++numAssigns;
//    if (numAssigns < 1)
//      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "Failed to assign a b quark to any B hadron.");
//    if (numAssigns > 1)
//      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Assigned a b quark to %d B hadrons.", numAssigns).Data());
//  }
//#endif //SANITY_CHECK
//
//  std::sort(bDecays.begin(), bDecays.end(), GreaterHadronPT<HadronDecay>());
////std::sort(bDecays.begin(), bDecays.end(), GreaterHadronE<HadronDecay>());
//  return bDecays;
//}


//using namespace ucsbsusy;
//using namespace std;
//
//const TString       JetFlavorMatching::TAGGABLE_NAME    []  = {"monoB", "multiB", "charm", "light", "gluon", "nonB", ""};
//
//const std::string   JetFlavorMatching::STORED_BHAD_INFO   = "storedBHadInfo";
//const std::string   JetFlavorMatching::NUM_FROM_ME        = "numME";
//const std::string   JetFlavorMatching::MAIN_B_P4          = "mainB";
//const std::string   JetFlavorMatching::SATELLITE_B_P4     = "satelliteB";
//const std::string   JetFlavorMatching::MAIN_B_INDEX       = "iMainB";
//const std::string   JetFlavorMatching::SATELLITE_B_INDEX  = "iSatelliteB";
//
//vector<HadronDecay> JetFlavorMatching::getBHadronDecays(const edm::Handle<vector<reco::GenParticle> >& particles)
//{
//  //-- Get last b quarks and first B hadrons ----------------------------------
//  vector<HadronDecay>                     bDecays;
//  vector<reco::GenParticleRef>            bQuarks;
//  vector<reco::GenParticleRef>            bHadronizations;
//  vector<int>                             bHadronNumBs;
//
//  for (size iPtcl = 6; iPtcl < particles->size(); ++iPtcl) {
//    const reco::GenParticle&              particle      = (*particles)[iPtcl];
//    const int                             pdgId         = TMath::Abs(particle.pdgId());
//    if (particle.status() == ParticleInfo::status_doc)  continue;
//
//    //.. Require b quarks with no b daughters .................................
//    if (particle.status() == ParticleInfo::status_decayed && pdgId == ParticleInfo::p_b) {
//      if (ParticleUtilities::isLastInChain(&particle)) {
//        bQuarks.push_back(reco::GenParticleRef(particles, iPtcl));
////        size                              maxIndex      = iPtcl;
//        for (size iDau = 0; iDau < particle.numberOfDaughters(); ++iDau) {
//          const reco::Candidate*          daughter      = particle.daughter(iDau);
////          maxIndex                        = TMath::Max(maxIndex, particle.daughterRef(iDau).key());
//          if (!ParticleInfo::isHadronizationModel(daughter->pdgId())) continue;
//          if (bHadronizations.size() >= bQuarks.size())
//            throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "More than one link encountered in hadronization of a b-quark.");
//          bHadronizations.push_back(reco::GenParticleRef(particles, particle.daughterRef(iDau).key()));
//        } // end loop over daughters
//        if (bHadronizations.size() != bQuarks.size()) {
//          ParticleUtilities::printGenInfo(*particles, -1);
//          throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Failed to obtain hadronization link for a b-quark (index %d).", iPtcl).Data());
//        }
//      }
//      continue;
//    }
//
//    //.. Require B hadrons with string mother .................................
//    int                                   numBLines     = 0;
//    if (ParticleInfo::isBHadron(ParticleInfo::typeOfHadron(pdgId, &numBLines))) {
//      if (particle.status() != ParticleInfo::status_decayed)
//        throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "Encountered an undecayed B hadron.");
//      if (particle.numberOfMothers() != 1)
//        throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Encountered a B hadron with %d mothers.",int(particle.numberOfMothers())).Data());
//
//      const int                           motherID      = TMath::Abs(particle.mother()->pdgId());
//      if (ParticleInfo::isHadronizationModel(motherID)) {
//        bDecays     .push_back(HadronDecay(particles, iPtcl));
//        bHadronNumBs.push_back(numBLines);
//      }
//      else if (!ParticleInfo::isBHadron(ParticleInfo::typeOfHadron(motherID)))
//        throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Encountered a B hadron with a %s as mother.",ParticleInfo::nameFor(motherID).Data()).Data());
//    }
//  } // end loop over particles
//
//
//  //-- Link quarks to hadrons -------------------------------------------------
//  // First get all the quarks and hadrons that end up/originate from the same string
//  static const vector<size>               NIL;
//  vector<const reco::Candidate*>          strings;
//  vector<vector<size> >                   stringQuarks;
//  vector<vector<size> >                   stringHadrons;
//  vector<int>                             stringBCount;
//
//  for (size iHadron = 0; iHadron < bDecays.size(); ++iHadron) {
//    const reco::Candidate*                reference     = bDecays[iHadron].hadron->mother();
//    size                                  iString       = 0;
//    while (iString < strings.size() && strings[iString] != reference)
//      ++iString;
//    if (iString >= strings.size())      { strings.push_back(reference);   stringQuarks.push_back(NIL);  stringHadrons.push_back(NIL);   stringBCount.push_back(0);  }
//    stringHadrons[iString].push_back(iHadron);
//    stringBCount [iString]               += bHadronNumBs[iHadron];
//  } // end loop over hadrons
//
//  for (size iQuark = 0; iQuark < bQuarks.size(); ++iQuark) {
//    const reco::Candidate*                reference     = bHadronizations[iQuark].get();
//    size                                  iString       = 0;
//    while (iString < strings.size() && strings[iString] != reference)
//      ++iString;
//    if (iString >= strings.size())      { strings.push_back(reference);   stringQuarks.push_back(NIL);  stringHadrons.push_back(NIL);   stringBCount.push_back(0);  }
//    stringQuarks[iString].push_back(iQuark);
//  } // end loop over quarks
//
//  // Now disambiguate by assigning the closest pairs first
//  ////cout << "_______________________________________________________________________________" << endl;
//  for (size iString = 0; iString < strings.size(); ++iString) {
//    const vector<size>&                   assocQuarks   = stringQuarks [iString];
//    const vector<size>&                   assocHadrons  = stringHadrons[iString];
//    if (assocQuarks .empty())             throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "Encountered a string with no associated b quarks.");
//    if (assocHadrons.empty())             throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "Encountered a string with no associated B hadrons.");
//    if (stringBCount[iString] != int(assocQuarks.size()))
//      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Mismatch in number of b quarks (%d) and expected b content in hadrons (%d).", int(assocQuarks.size()), stringBCount[iString]).Data());
//
//    ////cout << "b:";
//    ////for (size iQ = 0; iQ < assocQuarks.size(); ++iQ)
//    ////  cout << TString::Format(" (pT=%8.4g, id=%5d)", bQuarks[assocQuarks[iQ]]->pt(), bQuarks[assocQuarks[iQ]]->pdgId());
//    ////cout << endl;
//    ////cout << "B:";
//    ////for (size iH = 0; iH < assocHadrons.size(); ++iH)
//    ////  cout << TString::Format(" (pT=%8.4g, id=%5d)", bDecays[assocHadrons[iH]].hadron->pt(), bDecays[assocHadrons[iH]].hadron->pdgId());
//    ////cout << endl;
//    ////cout << " -> ";
//
//
//    // Compute distances between all pairs of quarks and hadrons
//    vector<vector<double> >               dRQuarkHadron (assocQuarks.size());
//    for (size iQ = 0; iQ < assocQuarks.size(); ++iQ) {
//      vector<double>&                     dRHadron      = dRQuarkHadron[iQ];
//      dRHadron.resize(assocHadrons.size());
//      for (size iH = 0; iH < assocHadrons.size(); ++iH)
//        dRHadron[iH]                      = PhysicsUtilities::deltaR(*bQuarks[assocQuarks[iQ]], *bDecays[assocHadrons[iH]].hadron);
//    } // end loop over quarks
//
//    // Assign closest first, allowing as many assignment to hadrons as the b content according to the PDG ID
//    vector<bool>                          usedQuark     (assocQuarks .size(), false);
//    vector<bool>                          usedHadron    (assocHadrons.size(), false);
//    for (size numAssigned = 0; numAssigned < assocQuarks.size(); ++numAssigned) {
//      int                                 nearestQuark  = -1;
//      int                                 nearestHadron = -1;
//      for (size iQ = 0; iQ < assocQuarks.size(); ++iQ) {
//        if (usedQuark[iQ])                continue;
//        for (size iH = 0; iH < assocHadrons.size(); ++iH) {
//          if (usedHadron[iH])             continue;
//          if (nearestQuark < 0 || dRQuarkHadron[iQ][iH] < dRQuarkHadron[nearestQuark][nearestHadron]) {
//            nearestQuark                  = iQ;
//            nearestHadron                 = iH;
//          }
//        } // end loop over hadrons
//      } // end loop over quarks
//      if (nearestQuark < 0 || dRQuarkHadron[nearestQuark][nearestHadron] > 1e307)
//        throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "Failed to find nearest quark-hadron pair.");
//
//      ////cout << "(" << nearestQuark << "," << nearestHadron << ")";
//      const size                          iHadron       = assocHadrons[nearestHadron];
//      HadronDecay&                        decay         = bDecays[iHadron];
//      decay.quark.push_back(bQuarks[assocQuarks[nearestQuark]]);
//      if (int(decay.quark.size()) >= bHadronNumBs[iHadron])
//        usedHadron[nearestHadron]         = true;
//      usedQuark[nearestQuark]             = true;
//    } // end loop over assignments
//    ////cout << endl;
//  } // end loop over strings
//
//
//  //-- Final round to deduce hadron decay products ----------------------------
//  for (size iHadron = 0; iHadron < bDecays.size(); ++iHadron) {
//    HadronDecay&                          decay         = bDecays[iHadron];
//    if (int(decay.quark.size()) != bHadronNumBs[iHadron])
//      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Mismatch in number of matched b quarks (%d) and expected b content (%d) for hadron (pdgId = %d).", int(decay.quark.size()), bHadronNumBs[iHadron], decay.hadron->pdgId()).Data());
//
//    getDecayProducts(decay.hadron.key(), particles, decay.decay, decay.sumVisible, decay.sumCharged, decay.numVisible, decay.numCharged);
//    if (decay.decay.empty())
//      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Encountered an undecayed hadron (pdgId = %d, pT = %.4g, eta = %.4g).", decay.hadron->pdgId(), decay.hadron->pt(), decay.hadron->eta()).Data());
//
//    if (decay.sumVisible.energy() > 0.01 + decay.hadron->energy()) {
//      cerr << endl;
//      cerr << "!!!!!!  " << TString::Format("pt = %8.4g, eta = %8.3g, phi = %8.3g, mass = %8.4g, E = %8.4g", decay.hadron->pt(), decay.hadron->eta(), decay.hadron->phi(), decay.hadron->mass(), decay.hadron->energy()) << endl;
//      cerr << " -vs-   " << TString::Format("pt = %8.4g, eta = %8.3g, phi = %8.3g, mass = %8.4g, E = %8.4g", decay.sumVisible.pt(), decay.sumVisible.eta(), decay.sumVisible.phi(), decay.sumVisible.mass(), decay.sumVisible.energy()) << endl;
//      cerr << endl;
//      throw cms::Exception("EventAnalyzer.getBHadronDecays()", TString::Format("Visible energy (%.8g) exceeds total energy (%.8g) for hadron (pdgId = %d).", decay.sumVisible.energy(), decay.hadron->energy(), decay.hadron->pdgId()).Data());
//    }
//  } // end loop over hadrons
//
//
//#define SANITY_CHECK
//#ifdef SANITY_CHECK
//  for (size iQuark = 0; iQuark < bQuarks.size(); ++iQuark) {
//    int                                   numAssigns    = 0;
//    for (size iHadron = 0; iHadron < bDecays.size(); ++iHadron)
//      for (size iQ = 0; iQ < bDecays[iHadron].quark.size(); ++iQ)
//        if (bQuarks[iQuark] == bDecays[iHadron].quark[iQ])
//          ++numAssigns;
//    if (numAssigns < 1)
//      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", "Failed to assign a b quark to any B hadron.");
//    if (numAssigns > 1)
//      throw cms::Exception("JetFlavorMatching.getBHadronDecays()", TString::Format("Assigned a b quark to %d B hadrons.", numAssigns).Data());
//  }
//#endif //SANITY_CHECK
//
//  std::sort(bDecays.begin(), bDecays.end(), GreaterHadronPT<HadronDecay>());
////std::sort(bDecays.begin(), bDecays.end(), GreaterHadronE<HadronDecay>());
//  return bDecays;
//}
////_____________________________________________________________________________
//void JetFlavorMatching::associateBHadronsToJets ( const vector<reco::GenJet>& jets, const vector<HadronDecay>& bHadronDecays
//                                            , vector<HadronContainments>& mainBHadrons, vector<HadronContainments>& satelliteBHadrons
//                                            )
//{
//  mainBHadrons      .clear ();
//  satelliteBHadrons .clear ();
//  mainBHadrons      .resize(jets.size());
//  satelliteBHadrons .resize(jets.size());
//
//
//  for (size iHadron = 0; iHadron < bHadronDecays.size(); ++iHadron) {
//    //-- Record B hadrons by matching genParticles ----------------------------
//    vector<pair<size,CartLorentzVector> >         containingJets;
//    int                                           bestIndex     = -1;
//    for (size iJet = 0; iJet < jets.size(); ++iJet) {
//      const reco::GenJet&                         genJet        = jets[iJet];
//      CartLorentzVector                           sumVisible, sumCharged;
//      int                                         numVisible    = 0;
//      int                                         numCharged    = 0;
//
//      containingJets.resize(containingJets.size() + 1);
//      pair<size,CartLorentzVector>&               containment   = containingJets.back();
//      if (matchConstituents(genJet, bHadronDecays[iHadron].decay, containment.second, sumVisible, sumCharged, numVisible, numCharged)) {
//        containment.first                         = iJet;
//        if (bestIndex < 0 || containingJets[bestIndex].second.energy() < containment.second.energy())
//          bestIndex                               = containingJets.size() - 1u;
//      }
//      else  containingJets.pop_back();
//    } // end loop over jets
//
//
//    //-- Preferential match to main jets --------------------------------------
//    for (size iCon = 0; iCon < containingJets.size(); ++iCon) {
//      HadronContainments&                         containment   = int(iCon) == bestIndex
//                                                                ? mainBHadrons      [ containingJets[iCon].first ]
//                                                                : satelliteBHadrons [ containingJets[iCon].first ]
//                                                                ;
//      containment.resize(containment.size() + 1);
//      containment.back().first                    = HadronDecayRef(&bHadronDecays, iHadron);
//      containment.back().second                   = containingJets[iCon].second;
//    } // end loop over containing jets
//  } // end loop over B hadrons
//}
////_____________________________________________________________________________
//void JetFlavorMatching::storeBHadronInfo( const vector<pat::Jet>& jets, const vector<HadronContainments>& mainBHadrons
//                                    , const vector<HadronContainments>& satelliteBHadrons, const std::string& genJetLabel
//                                    )
//{
//  assert(mainBHadrons.size() == satelliteBHadrons.size());
//
//
//  for (size iJet = 0; iJet < jets.size(); ++iJet) {
//    pat::Jet&                                     jet           = const_cast<pat::Jet&>( jets[iJet] );
//    if(jet.hasUserInt(STORED_BHAD_INFO)) return;
//    jet.addUserInt  (STORED_BHAD_INFO , 1);
//
//    const reco::CandidatePtr                      genJet        = jet.userCand(genJetLabel);
//    assert(genJet.isNonnull());
//    assert(genJet.key() < mainBHadrons.size());
//
//
//    //-- Main B hadrons revise jet flavor assignment --------------------------
//    const HadronContainments&                     mainBs        = mainBHadrons[genJet.key()];
//    if (mainBs.size()) {
//      vector<CartLorentzVector>                   bHadronP4s;   bHadronP4s.reserve(mainBs.size());
//      vector<int>                                 bHadronIdx;   bHadronIdx.reserve(mainBs.size());
//      for (size iHadron = 0; iHadron < mainBs.size(); ++iHadron) {
//        const HadronDecay&                        hadron        = *mainBs[iHadron].first;
//        assert(hadron.quark.size());
//
//        if (iHadron == 0 && TMath::Abs(jet.partonFlavour()) != ParticleInfo::p_b)
//          jet.setPartonFlavour(hadron.quark[0]->pdgId());
//
//        if (iHadron == 0)   jet.setGenParticleRef(hadron.hadron);
//        else                jet.addGenParticleRef(hadron.hadron);
//
//        bHadronP4s.push_back(mainBs[iHadron].second);
//        bHadronIdx.push_back(mainBs[iHadron].first.key());
//      } // end loop over contributing B hadrons
//
//
//      int                                         numMEPartons  = 0;
//      for (size iHadron = 0; iHadron < mainBs.size(); ++iHadron) {
//        const HadronDecay&                        hadron        = *mainBs[iHadron].first;
//        for (size iQuark = 0; iQuark < hadron.quark.size(); ++iQuark) {
//          jet.addGenParticleRef(hadron.quark[iQuark]);
//          const reco::GenParticle*                original      = ParticleUtilities::getOriginal( hadron.quark[iQuark].get() );
//          if (original->status() == ParticleInfo::status_doc)   ++numMEPartons;
//        } // end loop over quarks
//      } // end loop over contributing B hadrons
//
//      jet.addUserInt  (NUM_FROM_ME , numMEPartons);
//      jet.addUserData (MAIN_B_P4   , bHadronP4s  );
//      jet.addUserData (MAIN_B_INDEX, bHadronIdx, true);
//    }
//    else if (TMath::Abs(jet.partonFlavour()) == ParticleInfo::p_b)
//      jet.setPartonFlavour(ParticleInfo::p_g);
//
//
//    //-- All other B hadrons are just associated ------------------------------
//    const HadronContainments&                     satelliteBs   = satelliteBHadrons[genJet.key()];
//    if (satelliteBs.size()) {
//      vector<CartLorentzVector>                   bHadronP4s;   bHadronP4s.reserve(satelliteBs.size());
//      vector<int>                                 bHadronIdx;   bHadronIdx.reserve(satelliteBs.size());
//      for (size iHadron = 0; iHadron < satelliteBs.size(); ++iHadron) {
//        bHadronP4s.push_back(satelliteBs[iHadron].second);
//        bHadronIdx.push_back(satelliteBs[iHadron].first.key());
//      } // end loop over hadrons
//
//      jet.addUserData(SATELLITE_B_P4   , bHadronP4s);
//      jet.addUserData(SATELLITE_B_INDEX, bHadronIdx, true);
//    }
//  } // end loop over jets
//}
////_____________________________________________________________________________
//void JetFlavorMatching::storeBHadronInfo( const vector<pat::Jet>& recoJets, const vector<reco::GenJet>& genJets, const vector<HadronDecay>& bHadronDecays )
//{
//  vector<HadronContainments>            mainBHadrons     ;
//  vector<HadronContainments>            satelliteBHadrons;
//  associateBHadronsToJets(genJets , bHadronDecays, mainBHadrons, satelliteBHadrons);
//  storeBHadronInfo       (recoJets,                mainBHadrons, satelliteBHadrons);
//}
////_____________________________________________________________________________
//TaggableType JetFlavorMatching::getTaggableType(double jetEta, int numMainBHadrons, int partonFlavor, double etaAcceptance)
//{
//  if (TMath::Abs(jetEta) > etaAcceptance)                   return numTaggableTypes;
//  if (numMainBHadrons == 1)                                 return TAGGABLE_MONO_B;
//  if (numMainBHadrons >= 2)                                 return TAGGABLE_MULTI_B;
//  if (TMath::Abs(partonFlavor) == ParticleInfo::p_c)        return TAGGABLE_CHARM;
//  if (TMath::Abs(partonFlavor) != ParticleInfo::p_g)        return TAGGABLE_LIGHT;
//  return TAGGABLE_GLUON;                      // Default since gluons are more numerous
//}
////_____________________________________________________________________________
//TaggableType JetFlavorMatching::getTaggableType(const pat::Jet& jet, int* numMEpartons, double etaAcceptance)
//{
//  const vector<CartLorentzVector>*    mainBs  = jet.userData<vector<CartLorentzVector> >(MAIN_B_P4);
//  assert( (mainBs != 0) == (TMath::Abs(jet.partonFlavour()) == ParticleInfo::p_b) );
//  if (mainBs)   assert(mainBs->size());
//
//  const TaggableType                  tagType = JetFlavorMatching::getTaggableType(jet.eta(), mainBs ? mainBs->size() : 0, jet.partonFlavour(), etaAcceptance);
//
//  if (numMEpartons) {
//    *numMEpartons                     = 0;
//
//    if (tagType == TAGGABLE_MONO_B || tagType == TAGGABLE_MULTI_B) {
//      *numMEpartons                   = jet.userInt(NUM_FROM_ME);
//    }
//    else if (const reco::GenParticle* genParton = jet.genParticle()) {
//      const int                       pdgId   = TMath::Abs(genParton->pdgId());
//      if (pdgId == ParticleInfo::p_c) {
//        if (tagType==TAGGABLE_CHARM)  ++(*numMEpartons);
//      }
//      else if (pdgId != ParticleInfo::p_g) {
//        if (tagType==TAGGABLE_GLUON)  ++(*numMEpartons);
//      }
//      else
//        if (tagType==TAGGABLE_LIGHT)  ++(*numMEpartons);
//    }
//  }
//
//  return tagType;
//}
////_____________________________________________________________________________
//TaggableType JetFlavorMatching::getPATTaggableType(const pat::Jet& jet, int* numMEpartons, double etaAcceptance)
//{
//
//  if (TMath::Abs(jet.eta()) > etaAcceptance)                   return numTaggableTypes;
//  switch(TMath::Abs(jet.partonFlavour())) {
//  case  ParticleInfo::p_b:
//    return TAGGABLE_MONO_B;
//  case  ParticleInfo::p_c:
//    return TAGGABLE_CHARM;
//  case  ParticleInfo::p_g:
//    return TAGGABLE_GLUON;
//  default:
//    return numTaggableTypes;
//  }
//}
////_____________________________________________________________________________
//reco::GenParticleRef JetFlavorMatching::getMainBQuark(const pat::Jet& jet, const edm::Handle<reco::GenParticleCollection>& genParticles, size which)
//{
//  for (size iGen = 0, count = 0; iGen < jet.genParticlesSize(); ++iGen) {
//    reco::GenParticleRef      genParticle     = jet.genParticleRef(iGen);
//    assert(genParticle.isNonnull());
//    if (TMath::Abs(genParticle->pdgId()) == ParticleInfo::p_b && ++count > which)
//      return ParticleUtilities::getOriginal(genParticle, genParticles);
//  } // end loop over genParticles
//
//  static const reco::GenParticleRef   NADA;
//  return NADA;
//}
