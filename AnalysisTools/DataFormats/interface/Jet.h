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


#define FLVRECOASSOC

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
    if(ptr == 0) throw (message+std::string("The object was never loaded!"));
  }

protected :
  int	index_;  //Index in Jet vector

};
#ifdef FLVRECOASSOC
template <class CoordSystem>
class GenJet : public Jet<CoordSystem>
{
public :
  GenJet() {}

  template <class InputCoordSystem>
  GenJet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, const int inIndex = -1) : Jet<CoordSystem>(inMomentum, inIndex){};
  ~GenJet(){}
};

typedef GenJet<CylLorentzCoordF> GenJetF;
typedef std::vector<GenJetF> GenJetFCollection;

template <class CoordSystem>
class RecoJet : public Jet<CoordSystem>
{
public :

  RecoJet() : flavor_(-1), csv_(-10), genJet_(0) {}

  template <class InputCoordSystem>
  RecoJet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, const int inIndex = -1,
      const float inCSV = -10,const int inFlavor = -1, GenJet<CoordSystem>* inGenJet = 0)
      :Jet<CoordSystem>(inMomentum, inIndex), flavor_(inFlavor), csv_(inCSV), genJet_(inGenJet) {}
  ~RecoJet(){}

  const int& flavor()        const { return flavor_;}
  const float& csv()         const { return csv_;   }
  const GenJet<CoordSystem>&  genJet()        const { this->checkStorage(genJet_,"RecoJet.genJet()"); return *genJet_;  }
  GenJet<CoordSystem>&        genJet()        { return const_cast<GenJet<CoordSystem>&>(static_cast<const RecoJet<CoordSystem>*>(this)->genJet());  }

  void  setPtr(GenJet<CoordSystem>* inGenJet = 0) { genJet_ = inGenJet;       }
  void  setCsv(const float inCsv)               {csv_ = inCsv; }
  void  setFlavor(const int inFlavor)           { flavor_ = inFlavor; }

protected :
  int   flavor_;
  float csv_;     //pointer to csv information
  GenJet<CoordSystem>*  genJet_;  //Matched genJet

};

typedef RecoJet<CylLorentzCoordF> RecoJetF;
typedef std::vector<RecoJetF>   RecoJetFCollection;
}
#else
template <class CoordSystem>
class GenJet : public Jet<CoordSystem>
{
public :
  GenJet() : flavor_(0) {}

  template <class InputCoordSystem>
  GenJet(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum, int inIndex = -1, int * inFlavor = 0) : Jet<CoordSystem>(inMomentum, inIndex), flavor_(inFlavor) {};
  ~GenJet(){}

  int flavor()      const { this->checkStorage(flavor_,"GenJet.flavor()"); return *flavor_;   }

  void  setPtr(int * inFlavor)    { flavor_ = inFlavor; }
  void  setFlavor(const int& inFlavor)  { this->checkStorage(flavor_,"GenJet.setflavor()"); (*flavor_) = inFlavor;  }

protected :
  int * flavor_;
};

typedef GenJet<CylLorentzCoordF> GenJetF;
typedef std::vector<GenJetF> GenJetFCollection;

template <class CoordSystem>
class RecoJet : public Jet<CoordSystem>
{
public :

  RecoJet() : csv_(0), genJet_(0) {}

  template <class InputCoordSystem>
  RecoJet(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum, int inIndex = -1,float* inCSV = 0, GenJet<CoordSystem>* inGenJet = 0) :Jet<CoordSystem>(inMomentum, inIndex), csv_(inCSV), genJet_(inGenJet) {}
  ~RecoJet(){}

  float csv()               const { this->checkStorage(csv_,"RecoJet.csv()"); return *csv_;   }
  const GenJet<CoordSystem>&  genJet()        const { this->checkStorage(genJet_,"RecoJet.genJet()"); return *genJet_;  }
  GenJet<CoordSystem>&    genJet()        { return const_cast<GenJet<CoordSystem>&>(static_cast<const RecoJet<CoordSystem>*>(this)->genJet());  }

  void  setPtr(float* inCSV = 0, GenJet<CoordSystem>* inGenJet = 0) { csv_ = inCSV; genJet_ = inGenJet;       }
  void  setCsv(const float& inCsv)          { this->checkStorage(csv_,"RecoJet.setCsv()"); (*csv_) = inCsv; }

protected :
  float*    csv_;     //pointer to csv information
  GenJet<CoordSystem>*  genJet_;  //Matched genJet

};

typedef RecoJet<CylLorentzCoordF> RecoJetF;
typedef std::vector<RecoJetF>   RecoJetFCollection;
}
#endif

#endif
