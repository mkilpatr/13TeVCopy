#include "../interface/Mt2Helper.h"
#include "../interface/Davismt2.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;



Mt2Helper::Mt2Helper(){};
Mt2Helper::~Mt2Helper(){};

double Mt2Helper::CalcMT2( const MomentumF *visibleA, const MomentumF *visibleB, const MomentumF *ptmiss, double mEachInvisible){
  double pa[3];
  double pb[3];
  double pmiss[3];

  pmiss[0] = 0;
  pmiss[1] = (ptmiss->p4().px());
  pmiss[2] = static_cast<double> (ptmiss->p4().py());

  pa[0] = static_cast<double> (mEachInvisible);
  pa[1] = static_cast<double> (visibleA->p4().px());
  pa[2] = static_cast<double> (visibleA->p4().py());

  pb[0] = static_cast<double> (mEachInvisible);
  pb[1] = static_cast<double> (visibleB->p4().px());
  pb[2] = static_cast<double> (visibleB->p4().py());

  Davismt2 *mt2 = new Davismt2();
  mt2->set_momenta(pa, pb, pmiss);
  mt2->set_mn(0);
  float MT2=mt2->get_mt2();

  return MT2;
}
