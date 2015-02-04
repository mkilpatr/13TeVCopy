//--------------------------------------------------------------------------------------------------
// 
// LeptonMVA
// 
// Class to compute discriminator value for hadronic lepton candidates.
// 
// LeptonMVA.h created on Wed Jan 28 03:33:04 CET 2015 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_OBJECTSELECTION_LEPTONMVA_H
#define ANALYSISTOOLS_OBJECTSELECTION_LEPTONMVA_H

#include <vector>
#include <TROOT.h>
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"


namespace ucsbsusy {

  class LeptonMVA {

    public :
      LeptonMVA();
      ~LeptonMVA();

      void initialize(std::string weightsfile );
      double evaluateMVA(float pt, float slf, float sip3d, float rhocorrectediso);

    private :
      TMVA::Reader* reader;
      float fMVAVar_pt;
      float fMVAVar_sef;
      float fMVAVar_sip3d;
      float fMVAVar_rhocorrectediso;

  };

}

#endif
