//--------------------------------------------------------------------------------------------------
// 
// Jet
// 
// Class to hold basic jet information. To be enhanced as needed.
// 
// Jet.h created on Tue Aug 19 16:26:39 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_HTTFATJET_H
#define ANALYSISTOOLS_DATAFORMATS_HTTFATJET_H

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/FatJet.h"

namespace ucsbsusy {



template <class CoordSystem>
class HTTFatJet : public FatJet<CoordSystem>
  {
  public :
  HTTFatJet() : FatJet<CoordSystem>(),
  ropt_              (-9.),
  frec_              (-9.),
  roptcalc_          (-9.),
  ptforopt_          (-9.),
  ropt_tau1_         (-9.),
  ropt_tau2_         (-9.),
  ropt_tau3_         (-9.)
{}

  template <class InputCoordSystem>
  HTTFatJet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, const int inIndex = -1,
        const float inCSV = -9,
        const float inPrunedMass = -9, const float inSoftDropMass = -9, const float inTau1 = -9,
        const float inTau2= -9, const float inTau3= -9 ) :
        FatJet<CoordSystem>(inMomentum,inIndex, inCSV,inPrunedMass,inSoftDropMass,inTau1,inTau2,inTau3),
        ropt_              (-9.),
        frec_              (-9.),
        roptcalc_          (-9.),
        ptforopt_          (-9.),
        ropt_tau1_         (-9.),
        ropt_tau2_         (-9.),
        ropt_tau3_         (-9.)
        {}

  const SubJet<CoordSystem>& w1Subjet() const { return FatJet<CoordSystem>::subJet(0);}
  SubJet<CoordSystem>&       w1Subjet()       { return FatJet<CoordSystem>::subJet(0);}
  const SubJet<CoordSystem>& w2Subjet() const { return FatJet<CoordSystem>::subJet(1);}
  SubJet<CoordSystem>&       w2Subjet()       { return FatJet<CoordSystem>::subJet(1);}
  const SubJet<CoordSystem>&  bSubjet() const { return FatJet<CoordSystem>::subJet(2);}
  SubJet<CoordSystem>&        bSubjet()       { return FatJet<CoordSystem>::subJet(2);}

  //ROPt accessors
  const Momentum<CoordSystem>&                  ropt_mom() const {return roptMomentum;}
  Momentum<CoordSystem>&                        ropt_mom()       {return roptMomentum;}
  const ROOT::Math::LorentzVector<CoordSystem>& ropt_p4()  const {return roptMomentum.p4();}
  ROOT::Math::LorentzVector<CoordSystem>&       ropt_p4()        {return roptMomentum.p4();}

  float ropt          ()         const { return ropt_     ;    }
  float frec          ()         const { return frec_     ;    }
  float roptcalc      ()         const { return roptcalc_ ;    }
  float ptforopt      ()         const { return ptforopt_ ;    }
  float ropt_tau1     ()         const { return ropt_tau1_;    }
  float ropt_tau2     ()         const { return ropt_tau2_;    }
  float ropt_tau3     ()         const { return ropt_tau3_;    }


  template <class InputCoordSystem>
    void addROptInfo(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum,
        const float inRopt,const float inFrec,const float inRoptcalc,const float inPtforopt,
        const float inRopt_tau1,const float inRopt_tau2,const float inRopt_tau3) {
    roptMomentum = inMomentum;
    ropt_        =inRopt;
    frec_        =inFrec;
    roptcalc_    =inRoptcalc;
    ptforopt_    =inPtforopt;
    ropt_tau1_   =inRopt_tau1;
    ropt_tau2_   =inRopt_tau2;
    ropt_tau3_   =inRopt_tau3;


    }

    ~HTTFatJet(){}
    
  protected :

    float ropt_             ;
    float frec_             ;
    float roptcalc_         ;
    float ptforopt_         ;
    float ropt_tau1_        ;
    float ropt_tau2_        ;
    float ropt_tau3_        ;

    Momentum<CoordSystem> roptMomentum;
    
  };
  
  typedef HTTFatJet<CylLorentzCoordF> HTTFatJetF;
  typedef std::vector<HTTFatJetF>     HTTFatJetFCollection;
}
#endif
