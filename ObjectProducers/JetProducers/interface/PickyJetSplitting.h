/*
 * PickyJetSplitting.h
 *
 *  Created on: Jan 16, 2015
 *      Author: nmccoll
 */

#ifndef OBJECTPRODUCERS_JETPRODUCERS_INTERFACE_PICKYJETSPLITTING_H_
#define OBJECTPRODUCERS_JETPRODUCERS_INTERFACE_PICKYJETSPLITTING_H_

#include "ObjectProducers/JetProducers/interface/Splittiness.h"

namespace ucsbsusy {

class PickyJetSplitting {
public:

  enum PickyJetCuts {PUPPI_RECO, PUPPI_GEN, PUPPI_RECO_HIGHEFF, PUPPI_GEN_HIGHEFF, NOPUPPI_RECO, NOPUPPI_GEN};

  PickyJetSplitting(TString mvaFileName, TString mvaName,PickyJetCuts cuts, const edm::ParameterSet &cfg);
  virtual ~PickyJetSplitting();

  double getSubjets(const fastjet::PseudoJet& superJet,       std::vector<fastjet::PseudoJet*>& subJets        ) const;
  double getDisc(const fastjet::PseudoJet& superJet, const std::vector<fastjet::PseudoJet*>& subJets,const double * tau2 = 0, double*sjPT = 0,double* sjAbsETA = 0) const;
  bool   shouldSplit(const fastjet::PseudoJet& superJet, const std::vector<fastjet::PseudoJet*>& subJets,const double * tau2, double * discResult = 0) const;


//different parameter cuts
public:


private:
  const ParamatrixMVA*  mvaPar;

  Splittiness splittiness;
  const Space*         axisETA;
  std::vector<const PopulationD*> discriCuts;

  const int numParameters              ;
  const int parIndex_superJet_pt       ;
  const int parIndex_superJet_eta      ;

  const int index_superJet_mass        ;
  const int index_tau1                 ;
  const int index_tau2                 ;
  const int index_highest_peak         ;
  const int index_lowest_peak          ;
  const int index_minimum_value        ;
  const int index_lowest_peak_location ;
  const int index_highest_peak_location;
  const int index_minimum_location     ;
  const int index_subjet_dr            ;


};

} /* namespace ucsbsusy */

#endif /* OBJECTPRODUCERS_JETPRODUCERS_INTERFACE_PICKYJETSPLITTING_H_ */
