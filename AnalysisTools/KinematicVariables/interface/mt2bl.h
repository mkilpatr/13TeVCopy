/*
  MT2bl
  mt2bl.cc: Helper fxns mt2blIndirect, mt2blDirect, and mt2blOrder; original authors' code
  mt2bl.h:  Prototypes of helper fxns; original authors' code

  Usage with two arbitrary jets:
    // make an instance of mt2bl, it's in the namespace mt2bl_bisect
    mt2bl_bisect::mt2bl mt2bl_test;
    // turn your arbitrary jets into MomentumF's
    MomentumF* b1 = new MomentumF(jets.at(0)->p4());
    MomentumF* b2 = new MomentumF(jets.at(1)->p4());
    // call mt2blDirect with jets, the visible lepton, and met
    mt2bl = mt2bl_test.mt2blDirect(b1, b2, lep, met);

  Usage with user-defined btagging
    // make an instance of mt2bl, it's in the namespace mt2bl_bisect
    mt2bl_bisect::mt2bl mt2bl_test2;
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
    // call mt2blIndirect with converted jets, btags, visible lepton, and met
    mt2bl = mt2bl_test2.mt2blIndirect(fjets,btags,lep,met);
*/

// original authors' textblock follows:

/***********************************************************************/
/*                                                                     */
/*              Finding MT2bl                                           */
/*              Reference:  arXiv:1203.4813 [hep-ph]                   */
/*              Authors: Yang Bai, Hsin-Chia Cheng,                    */
/*                       Jason Gallicchio, Jiayin Gu                   */
/*              Based on MT2 by: Hsin-Chia Cheng, Zhenyu Han           */
/*              Nov 9, 2012, v1.00a                                    */
/*                                                                     */
/***********************************************************************/
/*******************************************************************************
 Usage:

 1. Define an object of type "mt2bl":

 mt2blw_bisect::mt2bl mt2bl_event;

 2. Set momenta:

 mt2bl_event.set_momenta(pl,pb1,pb2,pmiss);

 where array pl[0..3], pb1[0..3], pb2[0..3] contains (E,px,py,pz), pmiss[0..2] contains (0,px,py)
 for the visible particles and the missing momentum. pmiss[0] is not used.
 All quantities are given in double.

 (Or use non-pointer method to do the same.)

 3. Use mt2bl::get_mt2bl() to obtain the value of mt2bl:

 double mt2bl_value = mt2bl_event.get_mt2bl();

 *******************************************************************************/

//note:  For a given trial mother mass my (for top in this case), Deltasq is defined as my^2-mn1^2,
//       where mn1 is the mass of one of the daughter particle (the neutrino on the visible lepton
//       chain or the invisible W-boson, see the code for details.)  We try to find the minimum
//       compatible Deltasq and transfer it to minimum my.
//
//       There is also variables "delta" and "delta2" which are NOT the sqrt of Delasq.
//
//another note:  This code can be easily modified to calculate mT2 with different daughter masses
//               in general.

#ifndef mt2bl_BISECT_H
#define mt2bl_BISECT_H

#include <iostream>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <utility>
#include "TLorentzVector.h"
#include "Math/VectorUtil.h"
#include "AnalysisTools/DataFormats/interface/Momentum.h"
#include "AnalysisTools/DataFormats/interface/Lepton.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
//#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h" // need for deltaR

namespace mt2bl_bisect
{
  /*The user can change the desired precision below, the larger one of the following two definitions is used. Relative precision less than 0.00001 is not guaranteed to be achievable--use with caution*/

  #define RELATIVE_PRECISION 0.00001 //defined as precision = RELATIVE_PRECISION * scale, where scale = max{Ea, Eb}
  #define ABSOLUTE_PRECISION 0.0     //absolute precision for mt2bl, unused by default

class mt2bl
{
  public:
      mt2bl();

      // new helper functions
      double mt2blIndirect(const vector<MomentumF*>& fjets, const vector<bool>& btags, const LeptonF* lep, const MomentumF* met);
      double mt2blOrder( const MomentumF* b1, const MomentumF* b2, const LeptonF* lep, const MomentumF* met);
      double mt2blDirect(const MomentumF* b1, const MomentumF* b2, const LeptonF* lep, const MomentumF* met);

// original authors' class
      void   mt2bl_bisect();
      void   mt2bl_massless();
	    void   set_momenta(double *pl0, double *pb10, double *pb20, double* pmiss0);  //b1 pairs with l
	    void   set_momenta(double El,  double plx,  double ply,  double plz,
					   double Eb1, double pb1x, double pb1y, double pb1z,
					   double Eb2, double pb2x, double pb2y, double pb2z,
					   double pmissx, double pmissy);  // Same as above without pointers/arrays
//	void   set_mn(double mn);
      double get_mt2bl();
//      void   print();
//      int    nevt;
   private:

      bool   solved;
      bool   momenta_set;
	  double scan_step;
    double error_value = 999.; // alex added
      double mt2bl_b;

      int    nsols(double Dsq);
      int    nsols_massless(double Dsq);
      inline int    signchange_n( long double t1, long double t2, long double t3, long double t4, long double t5);
      inline int    signchange_p( long double t1, long double t2, long double t3, long double t4, long double t5);
      int scan_high(double &Deltasq_high);
      int find_high(double &Deltasq_high);
      //data members
      double pax, pay, ma, Ea;
      double pmissx, pmissy;
      double pbx, pby, mb, Eb;
      double mn1, mn2;


      //auxiliary definitions
      double masq, Easq;
      double mbsq, Ebsq;
      double pmissxsq, pmissysq;
      double mn1sq, mn2sq;

      //auxiliary coefficients
      double a1, b1, c1, a2, b2, c2, d1, e1, f1, d2, e2, f2;
      double d11, e11, f12, f10, d21, d20, e21, e20, f22, f21, f20;

//      double scale;
      double precision;
};

}//end namespace mt2bl_bisect

#endif
