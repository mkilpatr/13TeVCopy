/**
  @file         LumpTools.h
  @author       Sue Ann Koay (sakoay@cern.ch)
*/


#ifndef __LUMP_TOOLS__
#define __LUMP_TOOLS__


#include <vector>
#include <iostream>
#include <stdarg.h>
#include <assert.h>

#include <TRandom.h>
#include <gsl/gsl_multimin.h>

//#include "SUSYReferenceAnalysis2/DataFormats/interface/StandardTypes.h"
//#include "SUSYReferenceAnalysis2/BackgroundEstimation/interface/FiniteQueue.h"
#include "ObjectProducers/PickyJetUtilities/interface/MetricSpaceSample.h"
#include "AnalysisTools/Parang/interface/Persistables.h"
#include "AnalysisTools/Parang/interface/PartialQueue.h"
//#include "Utilities/Parang/interface/Persistables.h"
//#include "Utilities/ALGLIB/src/linalg.h"

//#define DEBUG_DUMP

  typedef   unsigned int                  size;

//_____________________________________________________________________________
class KernelDensityEstimate
{
public:
  static const double     DIMENSIONAL_CONSTANT;
  double                  maxNSigmas;
  std::vector<double>     points  ;
  std::vector<double>     weights ;
  std::vector<double>     kernel  ;
  mutable double          minPointWeight;
  mutable double          uniformKernel ;

  double                  xMin      ;
  double                  xMax      ;
  double                  sumWeights;
  AscendingQueue          orderedWeights;

  KernelDensityEstimate(size reserve = 0, double maxNSigmas = 10, size numTopWeights = 10)
    : maxNSigmas    (maxNSigmas)
    , minPointWeight(0     )
    , uniformKernel (-999  )
    , xMin          ( 1e308)
    , xMax          (-1e308)
    , sumWeights    (0     )
    , orderedWeights(numTopWeights)
  {
    points  .reserve(reserve);
    weights .reserve(reserve);
    kernel  .reserve(reserve);
  }

  void    addData(double x, double weight);

  double  value         ( double x, int firstPoint = 0, int lastPoint = -1 )  const;
  double  operator()    ( double x )  const { return   value(x); }
  double  positive      ( double x )  const { return + value(x); }
  double  negative      ( double x )  const { return - value(x); }
  double  gradient      ( double x )  const;
  double  curvature     ( double x )  const;

  int     numberOfNodes ( double kernelSize ) const;
  std::vector<double>   findMaxima(double xStep = 0.05, int maxIterations = 100, double absoluteTolerance = 1e-4, double relativeTolerance = 1e-2) const;

  double  modeSignificance( double& uncertainty, double kernelSize, int numNodes, TRandom* random, double maxUncertainty2 = 0.1*0.1, int minExperiments = 50, int maxExperiments = 10000 ) const;
  
  static double gradient( double x, void* params );
};


//_____________________________________________________________________________
class BimodalShape1D
{
protected:
  MetricVector                      axis          ;
  double                            axisLength    ;
  KernelDensityEstimate             kernelDensity ;
  std::vector<PStatistics>          locationStats ;
  std::vector<size>                 classBoundary ;

  double                            minimumLoc     ;
  double                            minimumValue   ;
  double                            leftPeakLoc    ;
  double                            leftPeakValue  ;
  double                            rightPeakLoc   ;
  double                            rightPeakValue ;
  bool                              unimodal       ;
  double                            peakDistance   ;
  bool                              leftIsHigher   ;
  double                            lowestPeak     ;
  double                            highestPeak    ;
  double                            lowestLoc      ;
  double                            highestLoc     ;

  void  finalize();
  void  finalize(double regularization, double kernelScale);

public:
  BimodalShape1D(size dimension, size expectedNumData, double maxKernelNSigmas, ...);
  ~BimodalShape1D();

  const MetricVector&               getAxis                 () const { return axis;             }
  const double                      getAxisLength           () const { return axisLength;       }
  KernelDensityEstimate&            getDensity              ()       { return kernelDensity;    }
  const KernelDensityEstimate&      getDensity              () const { return kernelDensity;    }

  size                              numClasses              ()            const { return classBoundary.size();  }
  size                              numDataInClass          (size iClass) const { return classBoundary[iClass] - (iClass ? classBoundary[iClass-1] : 0); }
  const PStatistics&                getStatistics           (size iClass) const { return locationStats[iClass]; }

  double                            getMinimumLocation      () const { return minimumLoc     ;  }
  double                            getMinimumValue         () const { return minimumValue   ;  }
  double                            getLeftPeakLocation     () const { return leftPeakLoc    ;  }
  double                            getLeftPeakValue        () const { return leftPeakValue  ;  }
  double                            getRightPeakLocation    () const { return rightPeakLoc   ;  }
  double                            getRightPeakValue       () const { return rightPeakValue ;  }
  bool                              isUnimodal              () const { return unimodal       ;  }
  double                            getDistanceBetweenPeaks () const { return peakDistance   ;  }
  bool                              isLeftPeakHigher        () const { return leftIsHigher   ;  }
  double                            getLowestPeak           () const { return lowestPeak     ;  }
  double                            getHighestPeak          () const { return highestPeak    ;  }
  double                            getLowestLocation       () const { return lowestLoc      ;  }
  double                            getHighestLocation      () const { return highestLoc     ;  }

  void  newClass    ();
  void  addData     ( double weight, ... );
  void  findTwoPeaks( double    leftOfMinimum
                    , double    rightOfMinimum
                    , double    regularization      = 0.05
                    , double    kernelScale         = -1.06
                    , int       maxIterations       = 1000
                    , double    absoluteTolerance   = 1e-5
                    , double    relativeTolerance   = 1e-3
                    );
  int   bestNumPeaks( double&   optimalKernel 
                    , int       minNumPeaks         = 1
                    , int       maxNumPeaks         = 3
                    , double    minKernel           = 0.05
                    , double    maxKernel           = 0.5
                    , double    kernelTolerance     = 1e-4
                    , TRandom*  random              = gRandom
                    ) const;
  void  recordPeaks ( const std::vector<double>&    peakLocations 
                    , int       maxIterations       = 1000
                    , double    absoluteTolerance   = 1e-5
                    , double    relativeTolerance   = 1e-3
                    );

  void  printMathematica(double refLocation = 0, const char* title = 0, bool printGradient = true, bool printOverall = true, std::ostream& out = std::cout) const;

  template<typename Object, typename FunctionPtr, typename Number>
  static double findByBisection(const Object* object, FunctionPtr function, double xMin, double xMax, Number targetValue, Number& foundValue, double xTolerance, int maxIterations = 1000)
  {
                                      foundValue  = (object->*function)(xMin);
    Number                            maxValue    = (object->*function)(xMax);
    ////std::cout << TString::Format("~~~~~~~~~~~~~~~~~~~~~~~~~~~::::  %7.3g -> %d  ;  %7.3g -> %d", xMin, foundValue, xMax , maxValue) << std::endl;
    if ((foundValue > targetValue) && (maxValue > targetValue))   return ( foundValue < maxValue ? xMin : (foundValue = maxValue, xMax) );
    if ((foundValue < targetValue) && (maxValue < targetValue))   return ( foundValue > maxValue ? xMin : (foundValue = maxValue, xMax) );

    double                            deltaX;
    double                            xLoc        = ( foundValue < targetValue )
                                                  ? ( deltaX = xMax - xMin, xMin )
                                                  : ( deltaX = xMin - xMax, xMax )
                                                  ;
    for (int iteration = 0; iteration < maxIterations; ++iteration) {
      deltaX                         /= 2;
      const double                    xMid        = xLoc + deltaX;
      const Number                    midValue    = (object->*function)(xMid);
      ////std::cout << TString::Format("...........................      %7.3g -> %d", xMid, midValue) << std::endl;
      if (midValue <= targetValue) {  xLoc        = xMid;
                                      foundValue  = midValue; }

      if (TMath::Abs(deltaX) < xTolerance)        return xLoc;
    } // end loop over allowed iterations

    assert(false);
    return xMax;
  }
}; // end class BimodalShape1D
//
//
////_____________________________________________________________________________
//class EtaPhiKernelDensityEstimate
//{
//public:
//  static const double           DIMENSIONAL_CONSTANT;
//
//  class Clump {
//  public:
//    std::vector<size>           points;
//    double                      sumWeights;
//    double                      eta;
//    double                      phi;
//    double                      height;
//
//    Clump()
//      : sumWeights(-9)
//      , eta       (-9)
//      , phi       (-9)
//      , height    (-9)
//    { }
//  };
//  typedef std::vector<Clump>    Clumps;
//
//public:
//  std::vector<double>           etas;
//  std::vector<double>           phis;
//  std::vector<double>           weights;
//  EtaPhiShape                   etaPhiShape;
//  double                        maxDeltaEta;
//  double                        maxDeltaPhi;
//  double                        minDeltaEta;
//  double                        minDeltaPhi;
//
//  const double                  significantPT ;
//  const double                  kernelSize    ;
//  const double                  maxNumSigmas  ;
//  const double                  scale         ;
//
//  EtaPhiKernelDensityEstimate(double significantPT, size reserve = 0, double scale = 1, double maxNumSigmas = 100);
//
//  template<typename Particle>
//  void  addData       (const Particle& particle, double scale = 1) {
//    if (particle.pt() >= significantPT)
//    etaPhiShape .addData  (particle, scale);
//
//    etas        .push_back(particle.eta());
//    phis        .push_back(particle.phi());
//    weights     .push_back(particle.pt () * scale);
//  }
//  void  finalize      (double bandwidth, double regularization = 1e-3);
//  void  setKernelSize (double bandwidth, double regularization = 1e-3);
//  bool  isOutside     (double eta, double phi, double tolerance = 1e-2);
//
//  double evaluate(const double* x, double* gradient = 0, alglib::real_2d_array* curvature = 0) const;
//
//  static double potential (const gsl_vector * x, void * params) {
//    return ((const EtaPhiKernelDensityEstimate*) params)->evaluate(x->data);
//  }
//  static void   gradient  (const gsl_vector * x, void * params, gsl_vector * g) {
//    ((const EtaPhiKernelDensityEstimate*) params)->evaluate(x->data, g->data);
//  }
//  static void   compute   (const gsl_vector * x, void * params, double * f, gsl_vector * g) {
//    *f  = ((const EtaPhiKernelDensityEstimate*) params)->evaluate(x->data, g->data);
//  }
//
//  void    compute   (double eta, double phi, double& value, double& gradient, double& gausCurvature, double& meanCurvature)  const;
//  Clumps  findClumps(double initStep = 0.01, double dRtolerance = 0.05, double minClumpPT = 0)    const;
//};


#endif //__LUMP_TOOLS__
