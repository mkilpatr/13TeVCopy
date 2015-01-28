//--------------------------------------------------------------------------------------------------
// 
// TauMVA
// 
// Class to compute discriminator value for hadronic tau candidates.
// 
// TauMVA.h created on Wed Jan 28 03:33:04 CET 2015 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_OBJECTSELECTION_TAUMVA_H
#define ANALYSISTOOLS_OBJECTSELECTION_TAUMVA_H

#include "AnalysisTools/Parang/interface/Panvariate.h"

namespace ucsbsusy {

  class TauMVA {

    public :
      TauMVA(TString mvafileName, TString mvaName);
      ~TauMVA();

      double evaluateMVA(float pt, float eta, float dz, float chiso0p1, float chiso0p2, float chiso0p3, float chiso0p4, float totiso0p1, float totiso0p2, float totiso0p3, float totiso0p4, float nearesttrkdr, float contjetdr, float contjetcsv);

    private :
      const ParamatrixMVA*  mvaPar;

      const int numParameters;
      const int parIndex_pt;
      const int index_pt;
      const int index_abseta;
      const int index_absdz;
      const int index_chreliso0p1;
      const int index_chreliso0p2;
      const int index_chreliso0p3;
      const int index_chreliso0p4;
      const int index_totreliso0p1;
      const int index_totreliso0p2;
      const int index_totreliso0p3;
      const int index_totreliso0p4;
      const int index_nearesttrkdr;
      const int index_contjetdr;
      const int index_contjetcsv;

  };

}

#endif
