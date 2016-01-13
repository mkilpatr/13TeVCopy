/*
 * CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_JETANDMETCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_JETANDMETCORRECTIONSET_H_


#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"

class TRandom3;
class TFormula;

namespace QuickRefold {
class TF1Container;
}
namespace ROOT{
namespace Math{
template <class ScalarType>
class PtEtaPhiM4D;
template< class CoordSystem >
class LorentzVector;
}
}

namespace ucsbsusy {
typedef ROOT::Math::PtEtaPhiM4D<float> CylLorentzCoordF;
typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >  CylLorentzVectorF;
template <class InputCoordSystem>
class Momentum;
typedef Momentum<CylLorentzCoordF> MomentumF;
template <class CoordSystem>
class RecoJet;
typedef RecoJet<CylLorentzCoordF> RecoJetF;
typedef std::vector<RecoJetF> RecoJetFCollection;
template <class CoordSystem>
class GenJet;
typedef GenJet<CylLorentzCoordF> GenJetF;
class JetReader;


class JetResolutionCorr : public Correction {
public:
  JetResolutionCorr(TRandom3 * inRandGen, TString jetResFileName, TString jetResName = "AK4ResTrend" );
  ~JetResolutionCorr();

  //return false if below threshold
  //Note, this is the width of the gaussian = sqrt(c^2 -1)
  bool getCorrectedPT(const double gausWidth, const RecoJetF& jet, double& newPT) const;

  //resSF is the ratio of resolutions, aka c
  void correctJetsAndMET(const double resSF, std::vector<RecoJetF>& jets, MomentumF& met) const;

private:
  TRandom3 * randGen;
  TFile * jetResolutionFile;
  const QuickRefold::TF1Container * jetResolution;
};

class JetScaleCorr : public Correction {
public:
  JetScaleCorr() : Correction("JetScale") {}
  ~JetScaleCorr() {}

  void correctJetsAndMET(CORRTYPE corrType, std::vector<RecoJetF>& jets, MomentumF& met) const;
};

class RespTailCorr : public HistogramCorrection {
public:
  RespTailCorr(TString inputFile) : HistogramCorrection("RespTailCorr",inputFile), mmResp(-1),mmInd(-1)  {}
  ~RespTailCorr() {}

  void process(const std::vector<RecoJetF>& jets, const std::vector<GenJetF>& genJets, const MomentumF& met);
  float getWeight(CORRTYPE corrType) const;
  float mmResp; // reco/gen pt of most mis-measured jet
  int mmInd; //index (as returned from .index() ) of that jet

};

class METScaleCorr : public Correction {
public:
  METScaleCorr() : Correction("METScale") {}
  float get() const {return 1.0;}
  float scaleCorr(float trueMETPT) const;
  CylLorentzVectorF getCorrectedMET(const CylLorentzVectorF& trueBosons, const CylLorentzVectorF& met) const;
};

class METResCorr : public Correction {
public:
  METResCorr();
  float get() const {return 1.0;}
  CylLorentzVectorF getCorrectedMET(const CylLorentzVectorF& trueBosons,const CylLorentzVectorF& trueMET, CylLorentzVectorF met) const;
};

class METNoHFScaleCorr : public Correction {
public:
  METNoHFScaleCorr() : Correction("METNoHFScale") {}
  float get() const {return 1.0;}
  float scaleCorr(float trueMETPT) const;
  CylLorentzVectorF getCorrectedMET(const CylLorentzVectorF& trueBosons, const CylLorentzVectorF& met) const;
};

class METNoHFResCorr : public Correction {
public:
  METNoHFResCorr();
  float get() const {return 1.0;}
  CylLorentzVectorF getCorrectedMET(const CylLorentzVectorF& trueBosons,const CylLorentzVectorF& trueMET, CylLorentzVectorF met) const;
};

class JetAndMETCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT               = 0
                          , METSCALE              = (1 <<  0)   ///< Correct MET Scale
                          , METRESOLUTION         = (1 <<  1)   ///< Correct MET Resolution
                          , JETRESOLUTION         = (1 <<  2)   ///< Correct jet Resolution
                          , JETSCALE              = (1 <<  3)   ///< Correct jet Scale
                          , QCDRESPTAIL           = (1 <<  4)   ///< Correct response tail in QCD
  };
  JetAndMETCorrectionSet();
  virtual ~JetAndMETCorrectionSet();
  virtual void load(int correctionOptions = NULLOPT, TString jetResolutionFile = "",TString jetResponseTailFile ="", TRandom3 * randomGenerator = 0);
  virtual void processMET(const BaseTreeAnalyzer * ana);
  virtual void correctJetResolution(const BaseTreeAnalyzer * ana, RecoJetFCollection& jets, MomentumF& met);
  virtual void correctJetScale(const BaseTreeAnalyzer * ana, RecoJetFCollection& jets, MomentumF& met);
  void processRespTail(const BaseTreeAnalyzer * ana, const JetReader& jetReader, const MomentumF& met);
  virtual void processCorrection(const BaseTreeAnalyzer * ana) {}; //does not apply to this guy, must process corrections when he wants


  //individual accessors
  MomentumF getCorrectedMET() const;
  MomentumF getOriginalMET() const;

  MomentumF getCorrectedMETNoHF() const;
  MomentumF getOriginalMETNoHF() const;

  float getQCDRespTailWeight() const {return respTailWeight;}
  const RespTailCorr * getQCDRespTailCorrector() const {return  respTail;}

private:
  //Correction list
  METScaleCorr * metScale;
  METResCorr * metResolution;
  JetResolutionCorr * jetResolution;
  JetScaleCorr * jetScale;
  RespTailCorr * respTail;

  METNoHFScaleCorr * metNoHFScale;
  METNoHFResCorr * metNoHFResolution;

  MomentumF * originalMET;
  MomentumF * correctedMET;

  MomentumF * originalMETNoHF;
  MomentumF * correctedMETNoHF;

  CylLorentzVectorF * trueBosons; //direction of the correction
  CylLorentzVectorF * trueMET; //neutrinos from EWK bosons or LSPs

  float respTailWeight;
};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_JETANDMETCORRECTIONSET_H_ */
