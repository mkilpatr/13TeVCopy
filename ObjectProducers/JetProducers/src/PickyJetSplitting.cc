/*
 * PickyJetSplitting.cc
 *
 *  Created on: Jan 16, 2015
 *      Author: nmccoll
 */

#include <assert.h>
#include <fastjet/PseudoJet.hh>
#include "ObjectProducers/JetProducers/interface/PickyJetSplitting.h"



using namespace std;
using namespace ucsbsusy;
PickyJetSplitting::PickyJetSplitting(TString mvaFileName, TString mvaName,PickyJetCuts cuts, const edm::ParameterSet &cfg) : splittiness (cfg,false)
    ,numParameters               (0)
    ,parIndex_superJet_pt        (-1)
    ,parIndex_superJet_eta       (-1)
    ,index_superJet_mass         (-1)
    ,index_tau1                  (-1)
    ,index_tau2                  (-1)
    ,index_highest_peak          (-1)
    ,index_lowest_peak           (-1)
    ,index_minimum_value         (-1)
    ,index_lowest_peak_location  (-1)
    ,index_highest_peak_location (-1)
    ,index_minimum_location      (-1)
    ,index_subjet_dr             (-1)
{
  TFile*              inFile    = TFile::Open(mvaFileName, "READ");
  if(!inFile) throw cms::Exception("PickyJetSplitting::PickyJetSplitting()", TString::Format("could not load file: %s",mvaFileName.Data()));
  mvaPar = dynamic_cast<ParamatrixMVA*>(inFile->Get(mvaName));
  if(!mvaPar	) throw cms::Exception("PickyJetSplitting::PickyJetSplitting()", TString::Format("could not load MVA: %s",mvaName.Data()));
  delete inFile;

  std::clog << "Loading PickyJet MVA: "<< mvaFileName <<" ("<<mvaName<<") cut enum: "<< cuts <<std::endl;
  mvaPar->sitrep();

  const_cast<int&>(numParameters) = mvaPar->getNumParams();

  const_cast<int&>(parIndex_superJet_pt ) = mvaPar->findAxis("superJet_pt");  assert(parIndex_superJet_pt  >= 0);
  const_cast<int&>(parIndex_superJet_eta) = mvaPar->findAxis("superJet_eta"); assert(parIndex_superJet_eta >= 0);

  const auto * mva = mvaPar->getOne();
  assert(mva);

  const_cast<int&>(index_superJet_mass        ) = mva->findVariable("superJet_mass"        ); assert(index_superJet_mass        >=0);
  const_cast<int&>(index_tau1                 ) = mva->findVariable("tau1"                 ); assert(index_tau1                 >=0);
  const_cast<int&>(index_tau2                 ) = mva->findVariable("tau2"                 ); assert(index_tau2                 >=0);
  const_cast<int&>(index_highest_peak         ) = mva->findVariable("highest_peak"         ); assert(index_highest_peak         >=0);
  const_cast<int&>(index_lowest_peak          ) = mva->findVariable("lowest_peak"          ); assert(index_lowest_peak          >=0);
  const_cast<int&>(index_minimum_value        ) = mva->findVariable("minimum_value"        ); assert(index_minimum_value        >=0);
  const_cast<int&>(index_lowest_peak_location ) = mva->findVariable("lowest_peak_location" ); assert(index_lowest_peak_location >=0);
  const_cast<int&>(index_highest_peak_location) = mva->findVariable("highest_peak_location"); assert(index_highest_peak_location>=0);
  const_cast<int&>(index_minimum_location     ) = mva->findVariable("minimum_location"     ); assert(index_minimum_location     >=0);
  const_cast<int&>(index_subjet_dr            ) = mva->findVariable("subjet_dr"            ); assert(index_subjet_dr            >=0);


  const Space*         axisJetPT     = mvaPar->getAxis (parIndex_superJet_pt);
  axisETA       = mvaPar->getAxis (parIndex_superJet_eta);

  switch(cuts){
  case PUPPI_RECO:
    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.34,0.25,0.14,0.05,-0.06,-0.33,-0.59,-0.68,-0.67,-0.65,-0.40,0.01"));
    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.43,0.28,0.23,0.13,0.02,-0.17,-0.28,-0.20,0.06,0.26,-1.00,-1.00"));
    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.25,0.31,0.43,0.41,-1.00,-1.00,-1.00,-1.00,-1.00,-1.00,-1.00,-1.00"));
    break;
  case PUPPI_GEN:
	discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.38,0.26,0.15,0.10,0.05,-0.13,-0.49,-0.64,-0.70,-0.71,-0.60,-0.29"));
	discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.46,0.38,0.34,0.24,0.15,0.01,-0.23,-0.28,-0.18,0.20,-1.00,-1.00"));
	discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.43,0.44,0.36,0.27,-1.00,-1.00,-1.00,-1.00,-1.00,-1.00,-1.00,-1.00"));
	break;

  case NOPUPPI_RECO:
  case   NOPUPPI_GEN:
    //Increase tail FR + decrease low pt FR
    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.55,0.52,0.50,0.48,0.44,0.06,-0.30,-0.49,-0.52,-0.46,-0.26,-.68,"));
    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.47,0.56,0.58,0.57,0.55,0.32,0.10,0.00,0.06,0.30,99,99"));
    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.40,0.54,0.53,0.53,0.43,99,99,99,99,99,99,99"));

    //Increase tail FR
//    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.46,0.36,0.32,0.29,0.24,0.06,-0.30,-0.49,-0.52,-0.46,-0.26,-.68,"));
//    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.43,0.49,0.49,0.46,0.42,0.32,0.10,0.00,0.06,0.30,-1.00,-1.00"));
//    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.35,0.46,0.45,0.44,0.28,-1.00,-1.00,-1.00,-1.00,-1.00,-1.00,-1.00"));
    //Nominal
//    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.46,0.36,0.32,0.29,0.24,0.06,-0.30,-0.49,-0.52,-0.46,-0.26,0.09,"));
//    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.43,0.49,0.49,0.46,0.42,0.32,0.10,0.00,0.06,0.30,-1.00,-1.00"));
//    discriCuts.push_back(new PopulationD(const_cast<Space*>(axisJetPT),"0.35,0.46,0.45,0.44,0.28,-1.00,-1.00,-1.00,-1.00,-1.00,-1.00,-1.00"));
    break;
  default:
    throw cms::Exception("PickyJetSplitting::PickyJetSplitting()", TString::Format("Not a valid cut: %i",cuts));
  }


}

double PickyJetSplitting::getSubjets(const fastjet::PseudoJet& superJet,       std::vector<fastjet::PseudoJet*>& subJets        ) const
{
  return splittiness.getNSubjettinessSubjets(superJet,subJets);
}


double  PickyJetSplitting::getDisc(const fastjet::PseudoJet& superJet, const std::vector<fastjet::PseudoJet*>& subJets, const double * tau2,double*sjPT,double* sjAbsETA) const {
  assert(subJets.size() == 2);
  splittiness.cacheJetInfo(superJet, &subJets);

  std::vector<double>         parameters    ( numParameters );
  parameters[parIndex_superJet_pt]  = sjPT ? *sjPT : superJet.pt();
  parameters[parIndex_superJet_eta] = sjAbsETA ? *sjAbsETA : TMath::Abs(superJet.eta());

  const Panvariate*           mvaReader     = mvaPar->get(parameters);
  if (!mvaReader)             throw cms::Exception("PickyJetSplitting.getDisc()", TString::Format("No discriminator available for jet with pT = %.4g and eta = %.4g"
      , parameters[parIndex_superJet_pt],parameters[parIndex_superJet_eta]).Data());

  const fastjet::PseudoJet&   subJet1       = *subJets[0];
  const fastjet::PseudoJet&   subJet2       = *subJets[1];
  const Splittiness::JetDeposition *       jetStuff      = splittiness.getJetStuff();

 mvaReader->setVariable( index_superJet_mass         , superJet.m()                                                            );
 mvaReader->setVariable( index_highest_peak          , jetStuff->lobes.getHighestPeak()                                        );
 mvaReader->setVariable( index_lowest_peak           , jetStuff->lobes.isUnimodal() ? -0.1 : jetStuff->lobes.getLowestPeak  () );
 mvaReader->setVariable( index_minimum_value         , jetStuff->lobes.isUnimodal() ? -0.1 : jetStuff->lobes.getMinimumValue() );
 mvaReader->setVariable( index_lowest_peak_location  , jetStuff->lobes.getLowestLocation () - jetStuff->centerLocation         );
 mvaReader->setVariable( index_highest_peak_location , jetStuff->lobes.getHighestLocation() - jetStuff->centerLocation         );
 mvaReader->setVariable( index_minimum_location      , jetStuff->lobes.getMinimumLocation() - jetStuff->centerLocation         );
 mvaReader->setVariable( index_tau1                  , splittiness.nSubjettiness.getTau(1, superJet.constituents())                        );
 mvaReader->setVariable( index_tau2                  , tau2 ? *tau2 : splittiness.nSubjettiness.getTau(2, superJet.constituents()));
 mvaReader->setVariable( index_subjet_dr             , subJet1.delta_R(subJet2));

 return mvaReader->evaluateMethod(0);
}

bool  PickyJetSplitting::shouldSplit(const fastjet::PseudoJet& superJet, const std::vector<fastjet::PseudoJet*>& subJets, const double * tau2, double * discResult) const
{
  double sjPT = superJet.pt();
  double sjAbsEta = TMath::Abs(superJet.eta());

  if(sjPT < 20) return false;

  if(sjPT >= 300 && sjAbsEta >= 2 && sjAbsEta < 3 ){
    sjPT = 305;
  } else if (sjPT >= 100 && sjAbsEta >= 3.0 ){
    sjPT = 105;
  }

  const double disc = getDisc(superJet,subJets,tau2,&sjPT,&sjAbsEta);

//  cout << superJet.pt() <<" "<<  TMath::Abs(superJet.eta()) << " "<< sjPT<< " "<< sjAbsEta <<" "<< disc<<" "<<discriCuts[axisETA->findBin(sjAbsEta)]->at(sjPT)<<endl;

  return disc > discriCuts[axisETA->findBin(sjAbsEta)]->at(sjPT);
}


PickyJetSplitting::~PickyJetSplitting() {delete mvaPar; for(auto * c : discriCuts) delete c;}
