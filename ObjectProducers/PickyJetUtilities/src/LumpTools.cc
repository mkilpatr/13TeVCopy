/**
  @file         LumpTools.cc
  @author       Sue Ann Koay (sakoay@cern.ch)
*/


#include "../interface/LumpTools.h"

#include <gsl/gsl_errno.h>
#include <gsl/gsl_roots.h>

#include <TVector2.h>
#include <TMath.h>
#include <Math/Functor.h>
#include <Math/GSLMinimizer.h>
#include <Math/BrentMinimizer1D.h>
#include <Math/BrentRootFinder.h>

#include "ObjectProducers/PickyJetUtilities/interface/FastFunctions.h"
#include "ObjectProducers/PickyJetUtilities/interface/StreamSamplerESWithJumps.h"

//#define DEBUG_DUMP


//=============================================================================
//    KernelDensityEstimate
//=============================================================================


const double KernelDensityEstimate::DIMENSIONAL_CONSTANT  = TMath::Power( 2 * TMath::Pi(), -1./2 );

//_____________________________________________________________________________
void KernelDensityEstimate::addData(double x, double weight)
{
  points .push_back( x      );
  weights.push_back( weight );

  if (x < xMin)     xMin  = x;
  if (x > xMax)     xMax  = x;
  sumWeights       += weight;

  orderedWeights.push(weight);
}

//_____________________________________________________________________________
double KernelDensityEstimate::value( double x, int firstPoint, int lastPoint )  const
{
  if (points.empty()) return 0;
  firstPoint        = Procedures::normalIndex(firstPoint, points.size());
  lastPoint         = Procedures::normalIndex(lastPoint , points.size());

  double            value       = 0;
  for (int iPoint = firstPoint; iPoint <= lastPoint; ++iPoint) {
    if (weights[iPoint] < minPointWeight)     continue;

    const double    kernelSize  = ( uniformKernel > 0 ? uniformKernel : kernel[iPoint] );
    const double    deviation   = TMath::Abs(x - points[iPoint]) / kernelSize;
    if (deviation > maxNSigmas) continue;
    value          += weights[iPoint] * fastexp( - deviation * deviation / 2 ) / kernelSize;
  } // end loop over points

  return DIMENSIONAL_CONSTANT * value;
}

//_____________________________________________________________________________
double KernelDensityEstimate::gradient( double x ) const
{
  double            value       = 0;
  for (size iPoint = 0; iPoint < points.size(); ++iPoint) {
    if (weights[iPoint] < minPointWeight)     continue;

    const double    kernelSize  = ( uniformKernel > 0 ? uniformKernel : kernel[iPoint] );
    const double    deviation   = ( x - points[iPoint] ) / kernelSize;
    if (TMath::Abs(deviation) > maxNSigmas)   continue;

    const double    strength    = weights[iPoint] * fastexp( - deviation * deviation / 2 ) / kernelSize;
    value          -= strength * deviation / kernelSize;
  } // end loop over points

  return DIMENSIONAL_CONSTANT * value;
}

//_____________________________________________________________________________
double KernelDensityEstimate::curvature( double x ) const
{
  double            value       = 0;
  for (size iPoint = 0; iPoint < points.size(); ++iPoint) {
    if (weights[iPoint] < minPointWeight)     continue;

    const double    kernelSize  = ( uniformKernel > 0 ? uniformKernel : kernel[iPoint] );
    const double    deviation   = ( x - points[iPoint] ) / kernelSize;
    if (TMath::Abs(deviation) > maxNSigmas)   continue;

    const double    strength    = weights[iPoint] * fastexp( - deviation * deviation / 2 ) / kernelSize;
    value          += strength * ( deviation*deviation - 1 ) / kernelSize;
  } // end loop over points

  return DIMENSIONAL_CONSTANT * value;
}

//_____________________________________________________________________________
double KernelDensityEstimate::gradient( double x, void* params )
{
  return ((const KernelDensityEstimate*) params)->gradient(x);
}

//_____________________________________________________________________________
int KernelDensityEstimate::numberOfNodes( double kernelSize ) const 
{
  const double          prevKernel      = uniformKernel;
  uniformKernel         = kernelSize;
  std::vector<double>   maxima          = findMaxima();
  uniformKernel         = prevKernel;

  return maxima.size();
}

//_____________________________________________________________________________
double KernelDensityEstimate::modeSignificance( double& uncertainty, double kernelSize, int numNodes, TRandom* random, double maxUncertainty2, int minExperiments, int maxExperiments ) const
{
  //-- Clear previous information ---------------------------------------------
  KernelDensityEstimate         bootstrap(weights.size(), maxNSigmas);
  bootstrap.xMin                = xMin;
  bootstrap.xMax                = xMax;

  //-- Setup bootstrap sampler ------------------------------------------------
  PStatistics                   pointStats;
  int                           numPoints   = 0;
  for (size iPoint = 0; iPoint < weights.size(); ++iPoint)
    if (weights[iPoint] >= minPointWeight) {
      pointStats.add(points[iPoint], weights[iPoint]);
      ++numPoints;
    }
  const double                  scale       = fastpow ( 1 + sqr(kernelSize/pointStats.getRMS())
                                                      , -0.5
                                                      );

  StreamSamplerESWithJumps      sampler(numPoints, random);

  ////std::cout << TString::Format(" ??????   %7.3g = %d ? ", kernelSize, numNodes);
  double                        probability = 0;
  int                           numAbove    = 0;
  int                           numSamples  = 0;
  uncertainty                   = 1;
  while (++numSamples <= maxExperiments) {
    //-- Number of nodes per pseudoexperiment ---------------------------------
    sampler.initiate();
    for (size iPoint = 0; iPoint < weights.size(); ++iPoint)
      if (weights[iPoint] >= minPointWeight)
        sampler.feedItem(WeightedItem(iPoint, weights[iPoint]));

    const SampledSet            sample      = sampler.getSample();
    bootstrap.points .clear();
    bootstrap.weights.clear();

    for (size iPoint = 0; iPoint < sample.size(); ++iPoint) {
      const int                 index       = sample[iPoint].wItem.getID();
      bootstrap.points .push_back ( scale * ( points[index] + kernelSize*gRandom->Gaus() ) );
      bootstrap.weights.push_back ( 1 /*sample[iPoint].wItem.getWeight()*/ );
    } // end loop over points
  
    const int                   expNodes    = bootstrap.numberOfNodes(kernelSize);
    ////std::cout << " " << expNodes;
    if (expNodes > numNodes)    ++numAbove;

    //-- Probability and uncertainty ------------------------------------------
    if (numSamples < minExperiments)        continue;

    probability                 = 1. * numAbove / numSamples;
    if (numAbove == 0)          continue;
    if (numAbove == numSamples) continue;

    uncertainty                 = (1 - probability) / probability / numSamples;
    if (uncertainty < maxUncertainty2)      break;
  } // end loop over pseudoexperiments
  ////std::cout << std::endl;


  uncertainty                   = fastpow(uncertainty, 0.5);
  return probability;
}

//_____________________________________________________________________________
std::vector<double> KernelDensityEstimate::findMaxima(double xStep, int maxIterations, double absoluteTolerance, double relativeTolerance) const
{
  gsl_root_fsolver*           rootFinder  = gsl_root_fsolver_alloc(gsl_root_fsolver_brent);
  gsl_function                function    = { &KernelDensityEstimate::gradient, (void*) this };

  const double                xRange      = ( xMax - xMin - xStep );
  const int                   numSteps    = TMath::Max( 1, TMath::FloorNint( xRange / xStep ) );
  double                      xLeft       = xMin + (xMax - xMin - numSteps*xStep) / 2;

  std::vector<double>         maxima;
  double                      leftValue   = gradient(xLeft);
  for (int iStep = 0; iStep < numSteps; ++iStep) {
    const double              xRight      = xLeft + xStep;
    const double              rightValue  = gradient(xRight);
    
    if ( (leftValue > 0) && (rightValue < 0) ) {
      gsl_root_fsolver_set(rootFinder, &function, xLeft, xRight);

      for (int iteration = 0; iteration < maxIterations; ++iteration) {
        gsl_root_fsolver_iterate(rootFinder);

                                            gsl_root_fsolver_root   (rootFinder);
        const double          xLower      = gsl_root_fsolver_x_lower(rootFinder);
        const double          xUpper      = gsl_root_fsolver_x_upper(rootFinder);
        const int             status      = gsl_root_test_interval  (xLower, xUpper, absoluteTolerance, relativeTolerance);

        if (status == GSL_SUCCESS) {
          maxima.push_back( (xLower + xUpper) / 2 );
          break;
        }
        assert(status == GSL_CONTINUE);
      } // end loop over iterations
    }

    xLeft                     = xRight;
    leftValue                 = rightValue;
  } // end loop over intervals

  gsl_root_fsolver_free(rootFinder);
  return maxima;
}


//=============================================================================
//    BimodalShape1D
//=============================================================================


//_____________________________________________________________________________
BimodalShape1D::BimodalShape1D(size dimension, size expectedNumData, double maxKernelNSigmas, ...)
  : axis          (dimension)
  , axisLength    (1)
  , kernelDensity (expectedNumData, maxKernelNSigmas)
  , minimumLoc    (-9999) 
  , minimumValue  (-9999) 
  , leftPeakLoc   (-9999) 
  , leftPeakValue (-9999) 
  , rightPeakLoc  (-9999) 
  , rightPeakValue(-9999) 
  , unimodal      (-9999) 
  , peakDistance  (-9999) 
  , leftIsHigher  (-9999) 
  , lowestPeak    (-9999) 
  , highestPeak   (-9999) 
  , lowestLoc     (-9999) 
  , highestLoc    (-9999) 
{
  classBoundary.reserve(2);

  va_list           arguments;
  va_start(arguments, maxKernelNSigmas);
  for (size iCoord = 0; iCoord < dimension; ++iCoord)
    axis[iCoord]    = va_arg(arguments, double);
  va_end(arguments);

  axisLength        = axis.length();
  if (axisLength)   axis.scale(1. / axisLength);
}

//_____________________________________________________________________________
BimodalShape1D::~BimodalShape1D()
{
}

//_____________________________________________________________________________
void BimodalShape1D::newClass()
{
  classBoundary.push_back(kernelDensity.points.size());
}

//_____________________________________________________________________________
void BimodalShape1D::addData( double weight, ... )
{
  double            location    = 0;
  va_list           arguments;
  va_start(arguments, weight);
  for (size iCoord = 0; iCoord < axis.size(); ++iCoord)
    location       += axis[iCoord] * va_arg(arguments, double);
  va_end(arguments);

  kernelDensity .addData( location, weight );
}

//_____________________________________________________________________________
void BimodalShape1D::finalize()
{
  if (locationStats.size()) {
    assert( kernelDensity.kernel.size() == kernelDensity.points.size() );
    return;
  }

  if (classBoundary.empty() || classBoundary.back() < kernelDensity.points.size())
    newClass();
  locationStats.resize(classBoundary.size());
}

//_____________________________________________________________________________
void BimodalShape1D::finalize(double regularization, double kernelScale)
{
  finalize();

  for (size iClass = 0, firstPoint = 0; iClass < classBoundary.size(); ++iClass) {

    const size            lastPoint   = classBoundary[iClass];
    PStatistics&          statistics  = locationStats[iClass];
    double                kernelSize;

    if (kernelScale < 0) {
      for (size iCon = firstPoint; iCon < lastPoint; ++iCon)
        statistics.add( kernelDensity.points [iCon]
                      , kernelDensity.weights[iCon]
                      );
      kernelSize          = TMath::Max( regularization
                                      , - kernelScale
                                      * statistics.getRMS()
                                      * TMath::Power( lastPoint-firstPoint, -0.2 )
                                      );
    }
    else {
      for (size iCon = firstPoint; iCon < lastPoint; ++iCon)
        for (size jCon = iCon + 1; jCon < lastPoint; ++jCon)
          statistics.add( TMath::Abs(kernelDensity.points [iCon] - kernelDensity.points [jCon])
                        ,            kernelDensity.weights[iCon] + kernelDensity.weights[jCon]
                        );
      kernelSize          = TMath::Max( regularization, kernelScale * statistics.getMean() );
    }

    kernelDensity.kernel.resize( kernelDensity.kernel.size() + lastPoint - firstPoint, kernelSize );
    firstPoint            = lastPoint;
  } // end loop over classes of data

  assert( kernelDensity.kernel.size() == kernelDensity.points.size() );
}

//_____________________________________________________________________________
void BimodalShape1D::findTwoPeaks(double leftOfMinimum, double rightOfMinimum, double regularization, double kernelScale, int maxIterations, double absoluteTolerance, double relativeTolerance)
{
  finalize(regularization, kernelScale);
  assert  (kernelDensity.xMax >= kernelDensity.xMin);

  ROOT::Math::Functor1D             kernelMaximum(&kernelDensity, &KernelDensityEstimate::negative);
  ROOT::Math::Functor1D             kernelMinimum(&kernelDensity, &KernelDensityEstimate::positive);
  ROOT::Math::BrentMinimizer1D      minimizer;
    
  const double                      epsilon         = ( kernelDensity.xMax - kernelDensity.xMin ) * relativeTolerance;

  minimizer.SetFunction   (kernelMinimum, leftOfMinimum, rightOfMinimum);
  minimizer.Minimize      (maxIterations, absoluteTolerance, relativeTolerance);
  minimumLoc                        =   minimizer.XMinimum   ();
  minimumValue                      =   minimizer.FValMinimum();

  minimizer.SetFunction   (kernelMaximum, kernelDensity.xMin, minimumLoc);
  minimizer.Minimize      (maxIterations, absoluteTolerance, relativeTolerance);
  if (minimizer.XMinimum() > minimumLoc - epsilon) {
    minimizer.SetFunction (kernelMaximum, kernelDensity.xMin, kernelDensity.xMax);
    minimizer.Minimize    (maxIterations, absoluteTolerance, relativeTolerance);
  }
  leftPeakLoc                       =   minimizer.XMinimum   ();
  leftPeakValue                     = - minimizer.FValMinimum();

  minimizer.SetFunction   (kernelMaximum, minimumLoc, kernelDensity.xMax);
  minimizer.Minimize      (maxIterations, absoluteTolerance, relativeTolerance);
  if (minimizer.XMinimum() < minimumLoc + epsilon) {
    minimizer.SetFunction (kernelMaximum, kernelDensity.xMin, kernelDensity.xMax);
    minimizer.Minimize    (maxIterations, absoluteTolerance, relativeTolerance);
  }
  rightPeakLoc                      =   minimizer.XMinimum   ();
  rightPeakValue                    = - minimizer.FValMinimum();


  unimodal                          = ( minimumLoc < leftPeakLoc || minimumLoc > rightPeakLoc );
  peakDistance                      =   rightPeakLoc  - leftPeakLoc;
  leftIsHigher                      = ( leftPeakValue > rightPeakValue );
  lowestPeak                        = ( leftIsHigher ? rightPeakValue : leftPeakValue  );
  highestPeak                       = ( leftIsHigher ? leftPeakValue  : rightPeakValue );
  lowestLoc                         = ( leftIsHigher ? rightPeakLoc   : leftPeakLoc    );
  highestLoc                        = ( leftIsHigher ? leftPeakLoc    : rightPeakLoc   );
}

//_____________________________________________________________________________
int BimodalShape1D::bestNumPeaks(double& optimalKernel, int minNumPeaks, int maxNumPeaks, double minKernel, double maxKernel, double kernelTolerance, TRandom* random) const
{
  //-- Only consider particles above threshold --------------------------------
  kernelDensity.minPointWeight      = TMath::Min( kernelDensity.orderedWeights.top()
                                      , 0.123104 - 2.12048 * fastpow(kernelDensity.sumWeights, 0.524974) + 2.47928 * fastpow(kernelDensity.sumWeights, 0.50706)
                                      );


  //-- Select smallest number of peaks with enough probability ----------------
#ifdef DEBUG_DUMP
  std::vector<double>               kernels;
#endif //DEBUG_DUMP
  double                            prevKernel    = maxKernel;
  for (int numPeaks = minNumPeaks; numPeaks <= maxNumPeaks; ++numPeaks) {
    int                             bestNumNodes;
    const double                    bestKernel    = findByBisection(&kernelDensity, &KernelDensityEstimate::numberOfNodes, minKernel, prevKernel, numPeaks, bestNumNodes, kernelTolerance);
    prevKernel                      = bestKernel;

#ifdef DEBUG_DUMP
    std::cout << TString::Format(" %d peak ? kernel = %7.3g -> %d peaks : "
                                , numPeaks
                                , bestKernel
                                , bestNumNodes
                                );
    kernels.push_back(bestKernel);
#endif //DEBUG_DUMP
  
    kernelDensity.uniformKernel     = bestKernel;
    std::vector<double>             maxima        = kernelDensity.findMaxima();

    TString                         report;
    for (unsigned iMax = 0; iMax < maxima.size(); ++iMax)
      report                       += TString::Format("%s%7.3g", iMax ? ", " : "", maxima[iMax]);
              
#ifdef DEBUG_DUMP
    double                          uncertainty;
    const double                    probability   = kernelDensity.modeSignificance( uncertainty, bestKernel, bestNumNodes, random );
    std::cout << std::left << std::setw(50) << report << "   ===>>>   " << probability << " +/- " << uncertainty << std::endl;
#endif //DEBUG_DUMP
  } // end loop over number-of-peak hypotheses

#ifdef DEBUG_DUMP
  std::cout << std::endl;
  kernelDensity.minPointWeight      = 0;
  for (size i = 0; i < kernels.size(); ++i) {
    kernelDensity.uniformKernel = kernels[i];
    printMathematica();
  }
#endif //DEBUG_DUMP

  kernelDensity.minPointWeight      = 0;
  return -1;
}


//_____________________________________________________________________________
void BimodalShape1D::recordPeaks( const std::vector<double>& peakLocations, int maxIterations, double absoluteTolerance, double relativeTolerance )
{
  assert(peakLocations.size());
  finalize();

  std::vector<std::pair<double,double> >    peaks(peakLocations.size());
  for (size iLoc = 0; iLoc < peaks.size(); ++iLoc) {
    peaks[iLoc].first                       = kernelDensity.value(peakLocations[iLoc]);
    peaks[iLoc].second                      = peakLocations[iLoc];
  } // end loop over peaks

  std::sort(peaks.begin(), peaks.end(), greaterFirst<double,double>());

  highestPeak                               = peaks[0].first  ;
  highestLoc                                = peaks[0].second ;
  lowestPeak                                = peaks.size() > 1 ? peaks[1].first  : highestPeak;
  lowestLoc                                 = peaks.size() > 1 ? peaks[1].second : highestLoc ;

  if (highestLoc < lowestLoc) {
    leftPeakValue                           = highestPeak;
    leftPeakLoc                             = highestLoc ;
    rightPeakValue                          = lowestPeak;
    rightPeakLoc                            = lowestLoc ;
  }
  else {
    leftPeakValue                           = lowestPeak;
    leftPeakLoc                             = lowestLoc ;
    rightPeakValue                          = highestPeak;
    rightPeakLoc                            = highestLoc ;
  }


  ROOT::Math::Functor1D                     kernelMaximum(&kernelDensity, &KernelDensityEstimate::negative);
  ROOT::Math::Functor1D                     kernelMinimum(&kernelDensity, &KernelDensityEstimate::positive);
  ROOT::Math::BrentMinimizer1D              minimizer;
    
  minimizer.SetFunction (kernelMinimum, leftPeakLoc, rightPeakLoc);
  minimizer.Minimize    (maxIterations, absoluteTolerance, relativeTolerance);
  minimumLoc                                = minimizer.XMinimum   ();
  minimumValue                              = minimizer.FValMinimum();

  unimodal                                  = peaks.size() < 2;
  peakDistance                              = rightPeakLoc - leftPeakLoc;
  leftIsHigher                              = leftPeakValue > rightPeakValue;
}


//_____________________________________________________________________________
void BimodalShape1D::printMathematica(double refLocation, const char* title, bool printGradient, bool printOverall, std::ostream& out) const
{
  static const TString    COLORS      = "Red,Blue,Darker[Green],Orange,Purple";


  if (title && title[0])
    out << "\n(*****  " << title << "  *****)" << std::endl;

  out << "   GraphicsRow[{ ListPlot[{";

  for (size iClass = 0, firstPoint = 0; iClass < classBoundary.size(); ++iClass) {
    if (iClass)           out << ",\n              ";
    out << "{";

    const size            lastPoint   = classBoundary[iClass];
    for (size index = firstPoint; index < lastPoint; ++index) {
      out << TString::Format ( "%s{%.3f,%.3f}"
                              , index > firstPoint ? "," : ""
                              , kernelDensity.points [index] - refLocation
                              , kernelDensity.weights[index]
                              );
    } // end loop over data in class

    firstPoint            = lastPoint;
    out << "}";
  } // end loop over classes of data
  out << "\n    }, Filling->Axis, PlotRange->Full, PlotStyle->{" << COLORS << "}, PlotLabel->\"" << (title ? title : "") << "\"]," << std::endl;


  const double            step        = TMath::Max( 0.01, ( kernelDensity.xMax - kernelDensity.xMin ) / 200 );
  TString                 colors, markers;
  out << "                 ListPlot[{";

  if (printOverall) {
    out << "{";
    for (double x = kernelDensity.xMin; x <= kernelDensity.xMax; x += step)
      out << TString::Format("%s{%.3f,%.3f}", x > kernelDensity.xMin ? ", " : "", x - refLocation, kernelDensity(x));
    out << "},";
    colors               += "Black,";
    markers              += "\"\",";
    if (locationStats.size()) {
      out << TString::Format( "\n                           {{%.3f,%.3f}},{{%.3f,%.3f}},{{%.3f,%.3f}},"
                            , leftPeakLoc  - refLocation, leftPeakValue
                            , minimumLoc   - refLocation, minimumValue
                            , rightPeakLoc - refLocation, rightPeakValue
                            );
      colors             += "Black,Black,Black,";
      markers            += "\"L\",\"C\",\"R\",";
    }
  }

  for (size iClass = 0, firstPoint = 0; iClass < classBoundary.size(); ++iClass) {
    if (iClass)           out << ",\n                           ";
    out << "{";
    for (double x = kernelDensity.xMin; x <= kernelDensity.xMax; x += step)
      out << TString::Format("%s{%.3f,%.3f}", x > kernelDensity.xMin ? ", " : "", x - refLocation, kernelDensity.value(x,firstPoint,classBoundary[iClass]-1));
    firstPoint            = classBoundary[iClass];
    out << "}";
      markers            += "\"\",";
  } // end loop over classes of data

  out << "\n    }, Joined->True, PlotMarkers->{" << markers << "\"\"}, PlotStyle->{" << colors << COLORS << "}, PlotRange->{0,Full}, PlotLabel->\"" << (title ? title : "") << "\"]";


  if (printGradient) {
    out << ",\n                 ListPlot[{{";
    for (double x = kernelDensity.xMin; x <= kernelDensity.xMax; x += step)
      out << TString::Format("%s{%.3f,%.3f}", x > kernelDensity.xMin ? ", " : "", x - refLocation, kernelDensity.gradient(x));
    out << "},\n                           ";
    out << TString::Format( "{{%.3f,%.3f}},{{%.3f,%.3f}},{{%.3f,%.3f}}"
                          , leftPeakLoc  - refLocation, leftPeakValue
                          , minimumLoc   - refLocation, minimumValue
                          , rightPeakLoc - refLocation, rightPeakValue
                          );
    out << "\n    }, Joined->True, PlotRange->Full, PlotLabel->\"" << (title ? title : "") << "\"]" << std::endl;
  }

  out << "\n   },ImageSize->600]" << std::endl;
}

//
////=============================================================================
////    EtaPhiKernelDensityEstimate
////=============================================================================
//
//const double EtaPhiKernelDensityEstimate::DIMENSIONAL_CONSTANT  = TMath::Power( 2 * TMath::Pi(), -2./2 );
//
////_____________________________________________________________________________
//EtaPhiKernelDensityEstimate::EtaPhiKernelDensityEstimate(double significantPT, size reserve, double scale, double maxNumSigmas)
//  : maxDeltaEta   (-1e308         )
//  , maxDeltaPhi   (-1e308         )
//  , minDeltaEta   ( 1e308         )
//  , minDeltaPhi   ( 1e308         )
//  , significantPT ( significantPT )
//  , kernelSize    (-9             )
//  , maxNumSigmas  ( maxNumSigmas  )
//  , scale         ( scale         )
//{
//  etas   .reserve(reserve);
//  phis   .reserve(reserve);
//  weights.reserve(reserve);
//}
//
////_____________________________________________________________________________
//void EtaPhiKernelDensityEstimate::finalize(double bandwidth, double regularization)
//{
//  assert(weights.size());
//  etaPhiShape.finalize();
//
//  maxDeltaEta         = -1e308;
//  maxDeltaPhi         = -1e308;
//  minDeltaEta         =  1e308;
//  minDeltaPhi         =  1e308;
//
//  for (unsigned int iPtcl = 0; iPtcl < weights.size(); ++iPtcl) {
//    const double      dEta    = etaPhiShape.etaDistance(etas[iPtcl]);
//    const double      dPhi    = etaPhiShape.phiDistance(phis[iPtcl]);
//
//    if (dEta > maxDeltaEta)   maxDeltaEta = dEta;
//    if (dEta < minDeltaEta)   minDeltaEta = dEta;
//    if (dPhi > maxDeltaPhi)   maxDeltaPhi = dPhi;
//    if (dPhi < minDeltaPhi)   minDeltaPhi = dPhi;
//  } // end loop over particles
//
//  setKernelSize(bandwidth, regularization);
//}
//
////_____________________________________________________________________________
//bool EtaPhiKernelDensityEstimate::isOutside(double eta, double phi, double tolerance)
//{
//  const double        etaTol  = tolerance * (maxDeltaEta - minDeltaEta);
//  eta                 = etaPhiShape.etaDistance(eta);
//  if (eta > maxDeltaEta - etaTol)     return true;
//  if (eta < minDeltaEta + etaTol)     return true;
//
//  const double        phiTol  = tolerance * (maxDeltaPhi - minDeltaPhi);
//  phi                 = etaPhiShape.phiDistance(phi);
//  if (phi > maxDeltaPhi - phiTol)     return true;
//  if (phi < minDeltaPhi + phiTol)     return true;
//
//  return false;
//}
//
////_____________________________________________________________________________
//void EtaPhiKernelDensityEstimate::setKernelSize(double bandwidth, double regularization)
//{
//  if (bandwidth > 0)
//    const_cast<double&>( kernelSize ) = bandwidth;
//
//  else {
//    assert( etaPhiShape.getStatistics().getNumData() );
//    const_cast<double&>( kernelSize ) = TMath::Max( regularization
//                                                  , -bandwidth
//                                                  * fastpow( sqr(etaPhiShape.getMajorWidth()) + sqr(etaPhiShape.getMinorWidth()), 0.5 )
//                                                  * fastpow( 4. / 4 / etaPhiShape.getStatistics().getNumData()
//                                                           , 1./6
//                                                           )
//                                                  );
//  }
//}
//
////_____________________________________________________________________________
//double EtaPhiKernelDensityEstimate::evaluate(const double* x, double* gradient, alglib::real_2d_array* curvature) const
//{
//  if (gradient) {
//    gradient[0]           = 0;
//    gradient[1]           = 0;
//  }
//  if (curvature) {
//    (*curvature)(0,0)     = 0;
//    (*curvature)(0,1)     = 0;
//    (*curvature)(1,1)     = 0;
//  }
//
//
//  double                  potential   = 0;
//  for (unsigned int iPtcl = 0; iPtcl < weights.size(); ++iPtcl) {
//    const double          dEta        =                     ( x[0] - etas[iPtcl] ) / kernelSize;
//    const double          dPhi        = TVector2::Phi_mpi_pi( x[1] - phis[iPtcl] ) / kernelSize;
//    const double          deviation2  = ( dEta*dEta + dPhi*dPhi );
//    if (deviation2 > sqr(maxNumSigmas))   continue;
//
//    const double          value       = scale * weights[iPtcl] * fastexp(float( - deviation2 / 2 )) / kernelSize;
//    potential            += value;
//    if (gradient) {
//      gradient[0]        -= value * dEta / kernelSize;
//      gradient[1]        -= value * dPhi / kernelSize;
//    }
//    if (curvature) {
//      (*curvature)(0,0)  += value * (dEta*dEta - 1) / kernelSize;
//      (*curvature)(0,1)  += value * dEta * dPhi     / kernelSize;
//      (*curvature)(1,1)  += value * (dPhi*dPhi - 1) / kernelSize;
//    }
//  } // end loop over particles
//
//
//  if (gradient) {
//    gradient[0]          *= DIMENSIONAL_CONSTANT;
//    gradient[1]          *= DIMENSIONAL_CONSTANT;
//  }
//  if (curvature) {
//    (*curvature)(0,0)    *= DIMENSIONAL_CONSTANT;
//    (*curvature)(0,1)    *= DIMENSIONAL_CONSTANT;
//    (*curvature)(1,0)     = (*curvature)(0,1);
//    (*curvature)(1,1)    *= DIMENSIONAL_CONSTANT;
//  }
//  return DIMENSIONAL_CONSTANT * potential;
//}
//
////_____________________________________________________________________________
//void EtaPhiKernelDensityEstimate::compute(double eta, double phi, double& value, double& gradient, double& gausCurvature, double& meanCurvature) const
//{
//  double                  x   []  = {eta, phi};
//  double                  grad[2]   ;
//  alglib::real_2d_array   curv;
//  curv.setlength(2,2);
//
//  const double            grad2   = sqr(grad[0]) + sqr(grad[1]);
//  value                   = evaluate(x, grad, &curv);
//  gradient                = fastpow( grad2 , 0.5 );
//  gausCurvature           = ( curv(0,0) * curv(1,1) - sqr(curv(0,1)) )
//                            ///// sqr( 1 + grad2 )
//                            ;
//  gausCurvature           = ( gausCurvature < 0 ? -1 : 1 )
//                          * fastpow(TMath::Abs( gausCurvature ), 0.5)
//                          ;
//
//  ////meanCurvature           = ( (1 + sqr(grad[1]))*curv(0,0)
//  ////                          + (1 + sqr(grad[0]))*curv(1,1)
//  ////                          - 2 * grad[0] * grad[1] * curv(0,1)
//  ////                          )
//  ////                          / 2
//  ////                          / fastpow( 1 + grad2, 1.5 )
//  ////                          ;
//  ////meanCurvature           = ( curv(0,0) + curv(1,1) ) / 2;
//
//  const double            det     = fastpow( 4*sqr(curv(0,1)) + sqr(curv(0,0) - curv(1,1)) , 0.5 );
//  const double            eValue1 = curv(0,0) + curv(1,1) - det;
//  const double            eValue2 = curv(0,0) + curv(1,1) + det;
//  meanCurvature           = TMath::Abs(eValue1) > TMath::Abs(eValue2)
//                          ? eValue1
//                          : eValue2
//                          ;
//}
//
////_____________________________________________________________________________
//EtaPhiKernelDensityEstimate::Clumps EtaPhiKernelDensityEstimate::findClumps(double initStep, double dRtolerance, double minClumpPT) const
//{
//  const double                            prevScale = scale;
//  const_cast<double&>( scale )            = -1;
//
//  //-- Evolve each particle towards minimum ---------------------------------
//  gsl_multimin_fdfminimizer*              minimizer = gsl_multimin_fdfminimizer_alloc(gsl_multimin_fdfminimizer_conjugate_fr, 2);
//  gsl_vector*                             params    = gsl_vector_alloc(numDirections);
//
//  std::vector<std::pair<double,double> >  minima;
//  std::vector<std::pair<double,int   > >  rankedPtcls;
//  for (unsigned int iPtcl = 0; iPtcl < weights.size(); ++iPtcl) {
//    gsl_multimin_function_fdf             function  = { &EtaPhiKernelDensityEstimate::potential
//                                                      , &EtaPhiKernelDensityEstimate::gradient
//                                                      , &EtaPhiKernelDensityEstimate::compute
//                                                      , 2
//                                                      , (void*) this
//                                                      };
//
//    gsl_vector_set( params, ETA, etas[iPtcl] );
//    gsl_vector_set( params, PHI, phis[iPtcl] );
//    gsl_multimin_fdfminimizer_set( minimizer, &function, params, initStep, 0.1 );
//
//    for (int iteration = 0; iteration < 1000; ++iteration) {
//      if (gsl_multimin_fdfminimizer_iterate(minimizer))   break;
//
//      const int                           status    = gsl_multimin_test_gradient(minimizer->gradient, 1e-6);
//      if (status != GSL_CONTINUE)         break;
//    } // end loop over minimization steps
//
//    const gsl_vector*                     minimum   = gsl_multimin_fdfminimizer_x(minimizer);
//    minima     .push_back(std::make_pair( gsl_vector_get(minimum, 0), gsl_vector_get(minimum, 1) ));
//    rankedPtcls.push_back(std::make_pair( weights[iPtcl], iPtcl ));
//  } // end loop over particles
//
//  gsl_multimin_fdfminimizer_free(minimizer);
//  gsl_vector_free               (params   );
//
//
//  //-- Collect distinguishable minima ---------------------------------------
//  std::sort(rankedPtcls.begin(), rankedPtcls.end(), greaterFirst<double,int>());
//
//  std::vector<Clump*>                     clumps;
//  std::vector<RankedIndex>                orderedClumps;
//  for (unsigned iRank = 0; iRank < rankedPtcls.size(); ++iRank) {
//    if (rankedPtcls[iRank].second < 0)    continue;
//    const std::pair<double, double>&      iMinimum    = minima[rankedPtcls[iRank].second];
//
//    Clump*                                clump       = new Clump;
//    PStatistics                           etaStats;
//    PStatistics                           phiStats;
//    clump->points.push_back( rankedPtcls[iRank].second );
//    etaStats.add( iMinimum.first  , weights[rankedPtcls[iRank].second] );
//    phiStats.add( iMinimum.second , weights[rankedPtcls[iRank].second] );
//    rankedPtcls[iRank].second             = -1;
//
//    for (unsigned jRank = 0; jRank < rankedPtcls.size(); ++jRank) {
//      if (rankedPtcls[jRank].second < 0)  continue;
//
//      const std::pair<double, double>&    jMinimum    = minima[rankedPtcls[jRank].second];
//      if ( sqr(                      jMinimum.first  - etaStats.getMean()  )
//         + sqr( TVector2::Phi_mpi_pi(jMinimum.second - phiStats.getMean()) )
//         > dRtolerance * dRtolerance
//         )                               continue;
//
//      clump->points.push_back( rankedPtcls[jRank].second );
//      etaStats.add( jMinimum.first , weights[rankedPtcls[jRank].second] );
//      phiStats.add( phiStats.getMean() + TVector2::Phi_mpi_pi(jMinimum.second - phiStats.getMean())
//                  , weights[rankedPtcls[jRank].second]
//                  );
//      rankedPtcls[jRank].second           = -1;
//    } // end loop over particles
//
//    if (etaStats.getSumWeights() < minClumpPT)
//      delete clump;
//    else {
//      clump->sumWeights                   = etaStats.getSumWeights();
//      clump->eta                          = etaStats.getMean();
//      clump->phi                          = phiStats.getMean();
//      double                              x[]         = {clump->eta, clump->phi};
//      clump->height                       = -evaluate(x);
//
//      orderedClumps.push_back(RankedIndex(clump->height, clumps.size()));
//      clumps.push_back(clump);
//    }
//  } // end loop over particles
//
//
//  //-- Sorted clumps ----------------------------------------------------------
//  std::sort(orderedClumps.begin(), orderedClumps.end(), greaterFirst<double,int>());
//  Clumps                                  selectedClumps(clumps.size());
//  for (size iRank = 0; iRank < clumps.size(); ++iRank)
//    selectedClumps[iRank]                 = *clumps[orderedClumps[iRank].second];
//  trash(clumps);
//
//  const_cast<double&>( scale )            = prevScale;
//  return selectedClumps;
//}
