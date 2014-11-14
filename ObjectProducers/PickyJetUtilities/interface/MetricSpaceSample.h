/**
  @file         MetricSpaceSample.h
  @author       Sue Ann Koay (sakoay@cern.ch)
*/

#ifndef __SAMPLEDISTANCE_H__
#define __SAMPLEDISTANCE_H__


#include <vector>
#include <stdarg.h>
#include <TObject.h>

#include "../interface/MetricCoordinate.h"
#include "AnalysisTools/Parang/interface/Persistables.h"
#include "AnalysisTools/Parang/interface/Prescriptions.h"
#include "AnalysisTools/Parang/interface/Paramatrix.h"



//=============================================================================
//    MetricSpaceSample
//=============================================================================

class MetricSpaceSample : public TObject, public Piece {
protected:
  std::vector<WeightedDatum>  data;
  unsigned                    dimensionality    ;
  double                      sumWeights        ;
  double                      selfEnergy        ;
  MetricVector                selfField         ;
  std::vector<double>         coordinateScale   ;
  double                      (*potentialFunction)(double);  ///< Function V = V(r^2) specifying the potential between two points separated by r
  double                      (*potentialGradient)(double);  ///< Should be G = G(r^2) = (1/r) (dV/dr) where V = V(r^2) is the potential function

public:
  MetricSpaceSample(ParamatrixStats::Iterator iStatistics, int indexQuantity, double (*potentialFunction)(double), double (*potentialGradient)(double) = 0);
  MetricSpaceSample(unsigned dimensionality, double (*potentialFunction)(double), double (*potentialGradient)(double) = 0, std::vector<WeightedDatum>::size_type reserveSize = 0);
  MetricSpaceSample(const MetricSpaceSample& other);

  //---------------------------------------------------------------------------
  /// @name   Interface
  //---------------------------------------------------------------------------
  ///@{
public:
  void          reserve               (std::vector<WeightedDatum>::size_type n) { data.reserve(n); }
  void          normalize             ();
  void          addData               (double weight, ...);
  void          computeSelfInteraction();
  double        interactionEnergy     (const MetricSpaceSample& sample, MetricVector* field = 0) const ;

  unsigned            getDimensionality ()  const { return dimensionality;  }
  unsigned            getNumData        ()  const { return data.size();     }
  double              getSelfEnergy     ()  const { return selfEnergy ;     }
  const MetricVector& getSelfField      ()  const { return selfField  ;     }

  double              getCoordinateScale(int iCoord)  const         { return coordinateScale[iCoord];   }
  void                setCoordinateScale(int iCoord, double value)  { coordinateScale[iCoord] = value;  }
  ///@}


  //---------------------------------------------------------------------------
  /// @name   Paramatrix requirements
  //---------------------------------------------------------------------------
  ///@{
public:
  void          associate(const std::vector<TObject*>&) { }
  Int_t         getNumFilledBins(Int_t = -1)  const     { return  getNumData(); }
  virtual Int_t getSumw2N() const                       { return  0; }
  Double_t      getRMS () const                         { return -9; }
  Double_t      getMean() const                         { return -9; }
  Double_t      getMean(Int_t, Int_t, Int_t) const      { return -9; }
  const Space*  getAxis() const                         { return  0; }
  Space*        getAxis()                               { return  0; }
  Double_t      getBinContent(Int_t, Int_t = 0) const   { return  0; }
  Double_t      getBinError2 (Int_t, Int_t = 0) const   { return  0; }
  Double_t      getEntries() const                      { return  sumWeights; }
  virtual void  sumw2()                                 { }
  Int_t         getNumBins() const                      { return  1; }
  ///@}


  //---------------------------------------------------------------------------
  /// @name   Not supported
  //---------------------------------------------------------------------------
  ///@{
public:
  virtual void  add   (const Piece* addend, const Double_t weight = 1)  { }
  void          copy  (const PStatistics* other)                        { }
  virtual void  scale (const Double_t scale)                            { }
  MetricSpaceSample*  clone() const                                     { return  0; }
  ///@}

  ClassDef(MetricSpaceSample, 1);
};


typedef   Paramatrix<MetricSpaceSample>   ParamatrixSample;


#endif //__SAMPLEDISTANCE_H__
