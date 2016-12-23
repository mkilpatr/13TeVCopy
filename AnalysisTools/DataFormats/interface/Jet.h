//--------------------------------------------------------------------------------------------------
//
// Jet
//
// Class to hold basic jet information. To be enhanced as needed.
//
// Jet.h created on Tue Aug 19 16:26:39 CEST 2014
//
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_JET_H
#define ANALYSISTOOLS_DATAFORMATS_JET_H

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

//_____________________________________________________________________________
// Basic jet type....used for both reco and gen jets
//_____________________________________________________________________________

template <class CoordSystem>
class Jet : public Momentum<CoordSystem>
{
public :
  Jet() : index_(-1) {}

  template <class InputCoordSystem>
  Jet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, const int inIndex = -1) : Momentum<CoordSystem>(inMomentum), index_(inIndex) {}

  ~Jet(){}

  int	index()				const { return index_;	}

  void	setIndex(const int& newIndex)	{ index_ = newIndex;	}

  //----Convenience function for throwing an exception when a member does not exist
  void checkStorage (const void * ptr, std::string message) const {
    if(ptr == 0) throw std::invalid_argument(message+std::string("The object was never loaded!"));
  }

protected :
  int	index_;  //Index in Jet vector

};

typedef Jet<CylLorentzCoordF> JetF;

template <class CoordSystem>
class GenJet : public Jet<CoordSystem>
{
public :
  GenJet() : flavor_(-1) {}

  template <class InputCoordSystem>
  GenJet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, const int inIndex = -1,const int inFlavor = -1)
  : Jet<CoordSystem>(inMomentum, inIndex),  flavor_(inFlavor){};
  ~GenJet(){}

  int flavor()        const { return flavor_;}
  void  setFlavor(const int inFlavor)           { flavor_ = convertTo<size8>(inFlavor,"GenJet::setFlavor"); }


private:
  size8   flavor_;
};

typedef GenJet<CylLorentzCoordF> GenJetF;
typedef std::vector<GenJetF> GenJetFCollection;

template <class CoordSystem> class RecoJet : public Jet<CoordSystem>
{
public :
    RecoJet() :  csv_(-10), genJet_(0) {}

    template <class InputCoordSystem>
    RecoJet(const ROOT::Math::LorentzVector<InputCoordSystem> &inMomentum,
            const int inIndex = -1,
            const float inCSV = -10, const float inPtRaw = 0, const float inUncert = 0,
            const bool inLooseId = false, GenJet<CoordSystem> *inGenJet = 0)
    // const float inUncert = 0)
        : Jet<CoordSystem>(inMomentum, inIndex), csv_(inCSV), pt_raw_(inPtRaw),
          uncertainty_(inUncert), looseid_(inLooseId), genJet_(inGenJet) {}
    ~RecoJet() {}


    float csv()         const { return csv_;    }
    bool  looseid()     const { return looseid_;}
    bool  tightid()     const { return tightid_;}
    float pt_raw()      const { return pt_raw_; }
    float uncertainty() const { return uncertainty_;}
    float chHadFrac()   const { return chHadFrac_;}
    int   chHadN2()      const { return chHadN2_;}
    int   chHadN4()      const { return chHadN4_;}
    int   chHadN6()      const { return chHadN6_;}
    const GenJet<CoordSystem>  *genJet()        const { return genJet_;  }
    GenJet<CoordSystem>        *genJet()        { return genJet_;  }

    void  setPtr(GenJet<CoordSystem> *inGenJet = 0) { genJet_   = inGenJet; }
    void  setCsv(const float inCsv)               {csv_         = inCsv;    }
    void  setLooseId(bool inLooseId)              {looseid_     = inLooseId;}
    void  setTightId(bool inTightId)              {tightid_     = inTightId;}
    void  setPtRaw(const float inPtRaw )          {pt_raw_      = inPtRaw;  }
    void  setUncertainty(const float inUncert)    {uncertainty_ = inUncert; }
    void  setChHadFrac(const float inChHadFrac)   {chHadFrac_   = inChHadFrac;}
    void  setChHadN2(const int inNChHad)           {chHadN2_      = inNChHad; }
    void  setChHadN4(const int inNChHad)           {chHadN4_      = inNChHad; }
    void  setChHadN6(const int inNChHad)           {chHadN6_      = inNChHad; }

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

  float cvsb() const {
    return cvsb_;
  }

  void setCvsb(float cvsb) {
    cvsb_ = cvsb;
  }

  float cvsl() const {
    return cvsl_;
  }

  void setCvsl(float cvsl) {
    cvsl_ = cvsl;
  }

  int jetMult() const {
    return jetMult_;
  }

  void setJetMult(int jetMult) {
    jetMult_ = jetMult;
  }

  float ptD() const {
    return ptD_;
  }

  void setPtD(float ptD) {
    ptD_ = ptD;
  }

  float jetcharge() const {
    return jetcharge_;
  }

  void setJetcharge(float jetcharge) {
    jetcharge_ = jetcharge;
  }

  float qgl() const {
    return qgl_;
  }

  void setQgl(float qgl) {
    qgl_ = qgl;
  }

  float cmva() const {
    return cmva_;
  }

  void setCmva(float cmva = -10) {
    cmva_ = cmva;
  }

protected :
    float csv_ = -10;                     //pointer to csv information
    float cmva_ = -10;                    //cmva
    float cvsl_ = -10;                    //charm vs light discriminator
    float cvsb_ = -10;                    //charm vs b discriminator
    float pt_raw_ = -10;                  //magnitude of uncorrected transverse momentum
    float uncertainty_ = -10;             //JEC error magnitude
    bool  looseid_ = false;                 //passes loose jet id or not
    bool  tightid_ = false;                 //passes loose jet id or not
    float chHadFrac_ = -10;               //charged hadron fraction
    int   chHadN2_ = -10;                 //charged hadron multiplicity pt 2+
    int   chHadN4_ = -10;                 //charged hadron multiplicity pt 4+
    int   chHadN6_ = -10;                 //charged hadron multiplicity pt 6+
    float betaStar_ = -10;
    float qgl_ = -10;
    float ptD_ = -10;
    float axis1_ = -10;
    float axis2_ = -10;
    int   jetMult_ = -10;
    float jetcharge_ = -10;
    GenJet<CoordSystem>  *genJet_;  //Matched genJet

};
typedef RecoJet<CylLorentzCoordF> RecoJetF;
typedef std::vector<RecoJetF>   RecoJetFCollection;
}
#endif
