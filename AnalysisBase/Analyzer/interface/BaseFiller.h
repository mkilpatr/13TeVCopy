/*
 * BaseFiller.h
 *
 *  Base class for object getting, storing, and filling
 *
 *  There are three responsibilities for this class:
 *  1) Get configuration parameters
 *  2) Load the edm object into a pointer owned by this class and (optionally) fill a custom object with the info
 *  3) Provide a ttree filling interface
 *
 *  Created on: Aug 22, 2014
 *      Author: nmccoll
 */

#ifndef BASEFILLER_H_
#define BASEFILLER_H_

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "AnalysisTools/Parang/interface/Planter.h"
#include "AnalysisBase/Analyzer/interface/PhysicsUtilities.h"
#include "AnalysisBase/Analyzer/interface/BaseUtilities.h"

namespace ucsbsusy {
class BaseFiller : public PhysicsUtilities, public BaseUtilities {
public:
  BaseFiller() {};
  virtual ~BaseFiller() {};

  //_____________________________________________________________________________
  // Default loading function
  // This guy needs to be defined for each implementation and is run once per event
  // If storeOnlyPtr is true object creation is cancelled and only the pointer is loaded
  //_____________________________________________________________________________
  virtual void load(edm::Event& iEvent, bool storeOnlyPtr = false) = 0;

  //_____________________________________________________________________________
  // Tree filling function
  // This guy places a bookmark in the planter at the value presented and increments it accordingly
  //_____________________________________________________________________________
  virtual void fill(Planter& plant, int& bookMark) = 0;
};

}

#endif /* BASEFILLER_H_ */
