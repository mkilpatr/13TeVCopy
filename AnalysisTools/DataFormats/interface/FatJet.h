//--------------------------------------------------------------------------------------------------
//
// Jet
//
// Class to hold basic jet information. To be enhanced as needed.
//
// Jet.h created on Tue Aug 19 16:26:39 CEST 2014
//
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_FATJET_H
#define ANALYSISTOOLS_DATAFORMATS_FATJET_H

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Jet.h"

namespace ucsbsusy {

template <class CoordSystem>
class SubJet : public Jet<CoordSystem>
  {
  public:
  SubJet(){}

  template <class InputCoordSystem>
  SubJet(const ROOT::Math::LorentzVector<InputCoordSystem> &inMomentum,
          const int inIndex = -1, const float inCSV = -10, float inCMVA = -10, float inCvsL = -10, float inCvsB = -10)
      : Jet<CoordSystem>(inMomentum, inIndex), csv_(inCSV), cmva_(inCMVA), cvsl_(inCvsL), cvsb_(inCvsB) {}
  ~SubJet() {}

  float csv()  const { return csv_;  }
  void  setCsv(float inCsv) { csv_  = inCsv; }

  float cmva() const { return cmva_; }
  void  setCmva(float cmva) { cmva_ = cmva; }

  float cvsb() const { return cvsb_; }
  void  setCvsb(float cvsb) { cvsb_ = cvsb; }

  float cvsl() const { return cvsl_; }
  void  setCvsl(float cvsl) { cvsl_ = cvsl; }

  float axis1() const {
    return axis1_;
  }
  void setAxis1(float axis1) {
    axis1_ = axis1;
  }

  float axis2() const {
    return axis2_;
  }
  void setAxis2(float axis2) {
    axis2_ = axis2;
  }

  float betaStar() const {
    return betaStar_;
  }
  void setBetaStar(float betaStar) {
    betaStar_ = betaStar;
  }

  int multiplicity() const {
    return multiplicity_;
  }
  void setMultiplicity(int multiplicity) {
    multiplicity_ = multiplicity;
  }

  float ptD() const {
    return ptD_;
  }
  void setPtD(float ptD) {
    ptD_ = ptD;
  }

  protected:
  float csv_  = -10;
  float cmva_ = -10;
  float cvsl_ = -10;
  float cvsb_ = -10;
  float betaStar_ = -10;
  float ptD_ = -10;
  float axis1_ = -10;
  float axis2_ = -10;
  int multiplicity_ = -1;
  };


template <class CoordSystem>
class FatJet : public Jet<CoordSystem>
  {
  public :
  FatJet() :
    csv_         (-9.),
    cvsl_        (-9.),
    cvsb_        (-9.),
    prunedMass_  (-9.),
    softDropMass_(-9.),
    tau1_        (-9.),
    tau2_        (-9.),
    tau3_        (-9.),
    puppi_softDropMass_(-9.),
    puppi_tau1_        (-9.),
    puppi_tau2_        (-9.),
    puppi_tau3_        (-9.)
  {}

  template <class InputCoordSystem>
    FatJet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, const int inIndex = -1,
        const float inCSV = -9, float inCvsL = -9, float inCvsB = -9,
        const float inPrunedMass = -9, const float inSoftDropMass = -9, const float inTau1 = -9,
        const float inTau2= -9, const float inTau3= -9 ) : Jet<CoordSystem>(inMomentum, inIndex),
        csv_         (inCSV),
        cvsl_        (inCvsL),
        cvsb_        (inCvsB),
        prunedMass_  (inPrunedMass),
        softDropMass_(inSoftDropMass),
        tau1_        (inTau1),
        tau2_        (inTau2),
        tau3_        (inTau3),
        puppi_softDropMass_(-9.),
        puppi_tau1_        (-9.),
        puppi_tau2_        (-9.),
        puppi_tau3_        (-9.)
        {}


  float csv()                  const { return csv_         ;    }
  float cvsl()                 const { return cvsl_        ;    }
  float cvsb()                 const { return cvsb_        ;    }
  float prunedMass()           const { return prunedMass_  ;    }
  float softDropMass()         const { return softDropMass_;    }
  float tau1()                 const { return tau1_        ;    }
  float tau2()                 const { return tau2_        ;    }
  float tau3()                 const { return tau3_        ;    }
  size  nSubjets()             const { return subjets.size();   }

  const SubJet<CoordSystem>& subJet(const size idx) const {
    if(idx >= nSubjets() )throw std::invalid_argument("Not a valid subjet index!");
    return subjets[idx];
  }
  SubJet<CoordSystem>&       subJet(const size idx) {
    if(idx >= nSubjets() )throw std::invalid_argument("Not a valid subjet index!");
    return subjets[idx];
  }

  //Puppi accessors
  const Momentum<CoordSystem>&                  puppi_mom() const {return puppiMomentum;}
  Momentum<CoordSystem>&                        puppi_mom()       {return puppiMomentum;}
  const ROOT::Math::LorentzVector<CoordSystem>& puppi_p4()  const {return puppiMomentum.p4();}
  ROOT::Math::LorentzVector<CoordSystem>&       puppi_p4()        {return puppiMomentum.p4();}
  float puppi_softDropMass()         const { return puppi_softDropMass_;    }
  float puppi_tau1()                 const { return puppi_tau1_        ;    }
  float puppi_tau2()                 const { return puppi_tau2_        ;    }
  float puppi_tau3()                 const { return puppi_tau3_        ;    }
  size  puppi_nSubjets()             const { return puppi_subjets.size();   }

  const SubJet<CoordSystem>&   puppi_subJet(const size idx) const {
    if(idx >= puppi_nSubjets() )throw std::invalid_argument("Not a valid puppi subjet index!");
    return puppi_subjets[idx];
  }
  SubJet<CoordSystem>&         puppi_subJet(const size idx) {
    if(idx >= puppi_nSubjets() )throw std::invalid_argument("Not a valid puppi subjet index!");
    return puppi_subjets[idx];
  }


  //setters



  template <class InputCoordSystem>
  void addSubjet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, float inCSV = -9., float inCMVA = -9, float inCvsL = -9, float inCvsB = -9){
    subjets.emplace_back(inMomentum,-1,inCSV,inCMVA,inCvsL,inCvsB);
  }

  template <class InputCoordSystem>
    void addPuppiInfo(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum,
        const float inSoftDropMass = -9, const float inTau1 = -9,
        const float inTau2= -9, const float inTau3= -9 ) {
    puppiMomentum = inMomentum;
    puppi_softDropMass_ = inSoftDropMass;
    puppi_tau1_        =inTau1;
    puppi_tau2_        =inTau2;
    puppi_tau3_        =inTau3;
    }


  template <class InputCoordSystem>
  void addPuppiSubjet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, const float inCSV = -9., float inCMVA = -9, float inCvsL = -9, float inCvsB = -9){
    puppi_subjets.emplace_back(inMomentum,-1,inCSV,inCMVA,inCvsL,inCvsB);
  }



    ~FatJet(){}

  public:
    std::vector<SubJet<CoordSystem> > subjets;
    std::vector<SubJet<CoordSystem> > puppi_subjets;

  protected :
    float csv_         ;
    float cvsl_        ;
    float cvsb_        ;
    float prunedMass_  ;
    float softDropMass_;
    float tau1_        ;
    float tau2_        ;
    float tau3_        ;

    Momentum<CoordSystem> puppiMomentum;
    float puppi_softDropMass_;
    float puppi_tau1_        ;
    float puppi_tau2_        ;
    float puppi_tau3_        ;



  };

  typedef SubJet<CylLorentzCoordF> SubJetF;
  typedef FatJet<CylLorentzCoordF> FatJetF;
  typedef std::vector<FatJetF>     FatJetFCollection;
}
#endif
