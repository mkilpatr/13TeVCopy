/*
 * PhysicsUtilities.h
 *
 *  Created on: Aug 20, 2014
 *      Author: nmccoll
 */

#ifndef PHYSICSUTILITIES_H_
#define PHYSICSUTILITIES_H_


#include <vector>

#include <TMath.h>

#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "AnalysisBase/Analyzer/interface/BaseUtilities.h"

namespace ucsbsusy {

typedef   math::XYZTLorentzVector       CartLorentzVector;
typedef   math::PtEtaPhiMLorentzVector  CylLorentzVector;

class PhysicsUtilities{
public:
//_____________________________________________________________________________
// Count Objects
//_____________________________________________________________________________
/// Count the number of objects, optionally passing a given test.
template<typename Object, typename ObjectPtr>
static size countObjectsDeref(const std::vector<ObjectPtr>& objects, double minPT, double maxEta, bool (*test)(const Object&));

/// Count the number of objects passing the given momentum thresholds.
template<typename ObjectPtr>
static size countObjectsDeref(const std::vector<ObjectPtr>& objects, double minPT, double maxEta);

/// Count the number of objects, optionally passing a given test.
template<typename Object>
static size countObjects(const std::vector<Object>& objects, double minPT = 0, double maxEta = 9999, bool (*test)(const Object&) = 0);

/// Count the number of objects passing the given test.
template<typename Object, typename Analyzer>
static size countObjects(const std::vector<Object>& objects, const Analyzer* analyzer, bool (Analyzer::*test)(const Object&) const);

/// Count the number of objects passing the given test and momentum thresholds.
template<typename Object, typename Analyzer>
static size countObjects(const std::vector<Object>& objects, const Analyzer* analyzer, bool (Analyzer::*test)(const Object&) const, double minPT, double maxEta);

/// Count the number of objects, optionally passing a given test.
template<typename Property, typename Object>
static size countObjectProperties(const std::vector<Object>& objects, const Property& (Object::*getProperty)() const, double minPT = 0, double maxEta = 9999, bool (*test)(const Property&) = 0);

/// Count the number of objects that are not near a veto set.
template<typename Object, typename Other>
static size countDistinctObjects(const std::vector<Object>& objects, const std::vector<Other>& vetoNearby, double minDRnearby, double minPT = 0, double maxEta = 9999);


//_____________________________________________________________________________
// Momentum manipulations
//_____________________________________________________________________________
//Add a set of coordinates to a CartLorentzVector
template<typename Number>
static CartLorentzVector& addTo(CartLorentzVector& p4, Number px, Number py, Number pz, Number E, Number scale = 1);

template<typename Momentum>
CartLorentzVector& addTo(CartLorentzVector& p4, const Momentum& addend, double scale = 1);

//Get the cartLorentzVector after scaling
template<typename CoordSystem, typename Tag>
CartLorentzVector getP4(const ROOT::Math::DisplacementVector3D<CoordSystem, Tag>& addend, double scale = 1);

/// Sums the 4-momenta of objects
static CartLorentzVector sumMomenta(const std::vector<CartLorentzVector>& p4, double scale = 1);

/// Sums the 4-momenta of objects passing the given test and momentum thresholds.
template<typename Object, typename Analyzer>
static CartLorentzVector sumObjects(const std::vector<Object>& objects, const Analyzer* analyzer, bool (Analyzer::*test)(const Object&) const, double minPT, double maxEta, double scale = 1);


//_____________________________________________________________________________
// Jet calculations
//_____________________________________________________________________________
//Count number of adjacent jets
template<typename Jet>
static double countProximities(const std::vector<Jet>& jets, const size numJets, size index1, size index2, int& numNearer, int* numFurther = 0);

//Count number of adjacent jets
template<typename Jet>
static int countNumNearer(const std::vector<Jet>& jets, const size numJets, size index1, size index2);

//Compute MHT
template<typename Momentum, typename outMomentum>
static outMomentum computeMHT(const std::vector<Momentum>& jets, const double minPT = 20, const double maxEta = 5.2, const std::vector<bool>* selected = 0);

//Compute HT
template<typename Momentum>
static double computeHT(const std::vector<Momentum>& jets, const double minPT = 50, const double maxEta = 2.4, const std::vector<bool>* selected = 0);

};

}

#include "AnalysisBase/Analyzer/src/PhysicsUtilities.icc"

#endif /* PHYSICSUTILITIES_H_ */
