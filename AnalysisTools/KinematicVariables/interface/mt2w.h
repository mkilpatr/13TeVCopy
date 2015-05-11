/*
  MT2W
  mt2w.cc: Helper fxns mt2wIndirect, mt2wDirect, and mt2wOrder; original authors' code
  mt2w.h:  Prototypes of helper fxns; original authors' code

  Usage with two arbitrary jets:
    // make an instance of mt2w, it's in the namespace mt2w_bisect
    mt2w_bisect::mt2w mt2w_test;
    // turn your arbitrary jets into MomentumF's
    MomentumF* b1 = new MomentumF(jets.at(0)->p4());
    MomentumF* b2 = new MomentumF(jets.at(1)->p4());
    // call mt2wDirect with jets, the visible lepton, and met
    mt2w = mt2w_test.mt2wDirect(b1, b2, lep, met);

  Usage with user-defined btagging
    // make an instance of mt2w, it's in the namespace mt2w_bisect
    mt2w_bisect::mt2w mt2w_test2;
    // make a bool vector of btags of the same size as your jets
    // turn your jets into MomentumF's
    vector<bool> btags;
    vector<MomentumF*> fjets;
    MomentumF* tempjet;
    for(auto* jet : jets) {
      btags.push_back(jet->csv() > 0.600); // my bjet selection
      tempjet = new MomentumF(jet->p4()); // ugly, for conversion
      fjets.push_back(tempjet);
    }
    // call mt2wIndirect with converted jets, btags, visible lepton, and met
    mt2w = mt2w_test2.mt2wIndirect(fjets,btags,lep,met);
*/

// original authors' textblock follows:

/***********************************************************************/
/*                                                                     */
/*              Finding MT2W                                           */
/*              Reference:  arXiv:1203.4813 [hep-ph]                   */
/*              Authors: Yang Bai, Hsin-Chia Cheng,                    */
/*                       Jason Gallicchio, Jiayin Gu                   */
/*              Based on MT2 by: Hsin-Chia Cheng, Zhenyu Han           */
/*              May 8, 2012, v1.00a                                    */
/*                                                                     */
/***********************************************************************/
/*******************************************************************************
  Usage:

  1. Define an object of type "mt2w":

     mt2w_bisect::mt2w mt2w_event;

  2. Set momenta:

     mt2w_event.set_momenta(pl,pb1,pb2,pmiss);

     where array pl[0..3], pb1[0..3], pb2[0..3] contains (E,px,py,pz), pmiss[0..2] contains (0,px,py)
     for the visible particles and the missing momentum. pmiss[0] is not used.
     All quantities are given in double.

     (Or use non-pointer method to do the same.)

  3. Use mt2w::get_mt2w() to obtain the value of mt2w:

     double mt2w_value = mt2w_event.get_mt2w();

*******************************************************************************/

#ifndef MT2W_H
#define MT2W_H

#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include "TLorentzVector.h"
#include "TFitter.h"
#include "TString.h"
#include "Math/VectorUtil.h"
#include "AnalysisTools/DataFormats/interface/Momentum.h"
#include "AnalysisTools/DataFormats/interface/Lepton.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"

// not sure on these
#include <vector>
#include <utility>
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
// #include "AnalysisTools/Utilities/interface/ParticleUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/Types.h"

using namespace ucsbsusy;
// using namespace std;

namespace mt2w_bisect
{

// typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

#define RELATIVE_PRECISION 0.00001 //defined as precision = RELATIVE_PRECISION * scale, where scale = max{Ea, Eb}
#define ABSOLUTE_PRECISION 0.0     //absolute precision for mt2w, unused by default


//Reserved for expert
#define MIN_MASS  0.1   //if ma<MINMASS and mb<MINMASS, use massless code
#define ZERO_MASS 0.000 //give massless particles a small mass
#define SCANSTEP 0.1

class mt2w
{
   public:
      // original class code (upper_bound increased from 500.0)
	    mt2w(double upper_bound=1000.0, double error_value=999.0, double scan_step=0.5);
      // new helper functions
      double mt2wIndirect(const vector<MomentumF*>& fjets, const vector<bool>& btags, const LeptonF* lep, const MomentumF* met);
      double mt2wOrder( const MomentumF* b1, const MomentumF* b2, const LeptonF* lep, const MomentumF* met);
      double mt2wDirect(const MomentumF* b1, const MomentumF* b2, const LeptonF* lep, const MomentumF* met);
      // Constructor where:
      //    upper_bound:  the upper bound of search for MT2W, default value is 500 GeV
      //    error_value:  if we couldn't find any compatible region below upper_bound, this value gets returned.
      //                  -1.0 is a reasonable to indicate error, but upper_bound-1.0 allows a simple greater-than cut for signal
      //    scan_step:    if we need to scan to find the compatible region, this is the step of the scan
      void   set_momenta(double *pl0, double *pb10, double *pb20, double* pmiss0);  //b1 pairs with l
      void   set_momenta(double El,  double plx,  double ply,  double plz,
                         double Eb1, double pb1x, double pb1y, double pb1z,
                         double Eb2, double pb2x, double pb2y, double pb2z,
                         double pmissx, double pmissy);  // Same as above without pointers/arrays
      // Where the input 4-vector information represents:
      //    l is the visible lepton
      //    b1 is the bottom on the same side as the visible lepton
      //    b2 is the other bottom (paired with the invisible W)
      //    pmiss is missing momentum with only x and y components.
      double get_mt2w();  // Calculates result, which is cached until set_momenta is called.
      void   print();
      ~mt2w() {};

   protected:
      void   mt2w_bisect();  // The real work is done here.

   private:

      bool   solved;
      bool   momenta_set;
	  double upper_bound;
	  double error_value;
	  double scan_step;
      double mt2w_b;

      int    teco(double mtop);   // test the compatibility of a given trial top mass mtop
      inline int    signchange_n( long double t1, long double t2, long double t3, long double t4, long double t5);
      inline int    signchange_p( long double t1, long double t2, long double t3, long double t4, long double t5);

      //data members
      double plx, ply, plz, ml, El;      // l is the visible lepton
	  double pb1x, pb1y, pb1z, mb1, Eb1;   // b1 is the bottom on the same side as the visible lepton
	  double pb2x, pb2y, pb2z, mb2, Eb2;   // b2 is the other bottom
      double pmissx, pmissy;              // x and y component of missing p_T
	  double mv,mw;           //mass of neutrino and W-boson

      //auxiliary definitions
      double mlsq, Elsq;
      double mb1sq, Eb1sq;
	  double mb2sq, Eb2sq;

      //auxiliary coefficients
      double a1, b1, c1, a2, b2, c2, d1, e1, f1, d2, e2, f2;
	  double d2o, e2o, f2o;

      double precision;
};

} // end namespace mt2w_bisect
#endif
