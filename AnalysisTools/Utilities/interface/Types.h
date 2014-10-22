//--------------------------------------------------------------------------------------------------
// 
// Types
// 
// Class with definitions of various types.
// 
// Types.h created on Tue Oct 21 21:34:00 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_UTILITIES_TYPES_H
#define ANALYSISTOOLS_UTILITIES_TYPES_H

#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"


namespace ucsbsusy {

  typedef ROOT::Math::PtEtaPhiM4D<double>				CylLorentzCoord;
  typedef ROOT::Math::PtEtaPhiM4D<float> 				CylLorentzCoordF;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >	XYZTLorentzVectorF;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> >	PtEtaPhiMLorentzVectorD;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >	PtEtaPhiMLorentzVectorF;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >	CartLorentzVector;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> >	CylLorentzVector;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >	CylLorentzVectorF;

  enum TaggableType	{ TAGGABLE_MONO_B, TAGGABLE_MULTI_B, TAGGABLE_CHARM, TAGGABLE_LIGHT, TAGGABLE_GLUON, TAGGABLE_NONB, numTaggableTypes };

}

#endif
