
#ifndef __QUARKGLUONTAGINTERFACE_H__
#define __QUARKGLUONTAGINTERFACE_H__


#include <vector>
#include <utility>

#include "AnalysisTools/JetShapeVariables/interface/QGLikelihoodCalculator.h"


//_____________________________________________________________________________
class QuarkGluonTagInterface {
protected:
  QGLikelihoodCalculator*   qglikeli_;

public:
  ~QuarkGluonTagInterface() {  delete qglikeli_;}
  QuarkGluonTagInterface() : qglikeli_  (new QGLikelihoodCalculator()) {}

  template <class Jet>
  double getDiscriminator(const Jet& jet, double rho)
  {
    double    corPt           = jet.pt();
    double    ptD             = jet.constituentPtDistribution();
    int       nCharged        = jet.chargedHadronMultiplicity();
    int       nNeutral        = jet.neutralHadronMultiplicity()+jet.photonMultiplicity();

    if(TMath::Abs(jet.eta()) >= 2.4 || jet.pt() < 10 || nCharged + nNeutral == 0  ) return -1;

    return qglikeli_->computeQGLikelihood(corPt,rho,nCharged,nNeutral,ptD);
  }
};

#endif //__QUARKGLUONTAGINTERFACE_H__

