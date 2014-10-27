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
  Jet(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum, int inIndex = -1) : Momentum<CoordSystem>(inMomentum), index_(inIndex) {}

  ~Jet(){}

  void setIndex(const int& newIndex) {index_ = newIndex;};
  int index() const {return index_;};

  //----Convenience function for throwing an exception when a member does not exist
  static void checkStorage (void * ptr, std::string message){
    //if(ptr == 0) throw cms::Exception(message, "The object was never loaded!");
    if(ptr == 0) throw (message+string("The object was never loaded!"));
  }

protected :
  int      index_;  //Index in Jet vector
};

template <class CoordSystem>
class GenJet : public Jet<CoordSystem>
{

  typedef TaggableType Flavor;

public :
  GenJet() :flavor_(0) {}

  template <class InputCoordSystem>
  GenJet(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum, int inIndex = -1, Flavor * inFlavor = 0) : Jet<CoordSystem>(inMomentum, inIndex), flavor_(inFlavor) {};
  ~GenJet(){}

  void setPtr(Flavor * inFlavor) { flavor_ = inFlavor;}

  void   setFlavor(const Flavor& inFlavor) { this->checkStorage(flavor_,"GenJet.setflavor()"); (*flavor_) = inFlavor; }
  Flavor flavor()    const { this->checkStorage(flavor_,"GenJet.flavor()"); return *flavor_;       }

protected :
  Flavor * flavor_;
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

  void setPtr(float* inCSV = 0, GenJet<CoordSystem>* inGenJet = 0) { csv_ = inCSV; genJet_ = inGenJet;}

  const GenJet<CoordSystem>& genJet() const { this->checkStorage(genJet_,"RecoJet.genJet()"); return *genJet_; }
  GenJet<CoordSystem>& genJet() { return const_cast<GenJet<CoordSystem>&>(static_cast<const RecoJet<CoordSystem>*>(this)->genJet()); }

  void   setCsv(const float& inCsv) { this->checkStorage(csv_,"RecoJet.setCsv()"); (*csv_) = inCsv; }
  float  csv()    const { this->checkStorage(csv_,"RecoJet.csv()"); return *csv_;       }


protected :
  float*       csv_;    //pointer to csv information
  GenJet<CoordSystem>*      genJet_;  //Matched genJet
};

typedef RecoJet<CylLorentzCoordF> RecoJetF;
typedef std::vector<RecoJetF > RecoJetFCollection;
}

#endif
