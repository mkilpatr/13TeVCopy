/**
  @file         ShapeTools.cc
  @author       Sue Ann Koay (sakoay@cern.ch)
*/


#include "../interface/MetricSpaceSample.h"

#include<assert.h>

using namespace std;
//=============================================================================
//    MetricSpaceSample
//=============================================================================

//_____________________________________________________________________________
MetricSpaceSample::MetricSpaceSample(ParamatrixStats::Iterator iStatistics, int indexQuantity, double (*potentialFunction )(double), double (*potentialGradient)(double)) 
  : dimensionality    (iStatistics.getAxis(indexQuantity)->getNumBins())
  , sumWeights        (0                )
  , selfEnergy        (0                )
  , coordinateScale   (dimensionality   )
  , potentialFunction (potentialFunction)
  , potentialGradient (potentialGradient)
{ 
  assert(potentialFunction);

  for (unsigned iQuantity = 0; iQuantity < dimensionality; ++iQuantity) {
    iStatistics.setBin(indexQuantity, iQuantity);
    PStatistics*    distanceStats   = iStatistics.get();
    assert(distanceStats);

    coordinateScale[iQuantity]      = distanceStats->getRMS();
    assert(coordinateScale[iQuantity]);
    assert(coordinateScale[iQuantity] == coordinateScale[iQuantity]);
  } // end loop over coordinates
}

//___________________________________________________________________________
MetricSpaceSample::MetricSpaceSample(unsigned dimensionality, double (*potentialFunction)(double), double (*potentialGradient)(double), std::vector<WeightedDatum>::size_type reserveSize)
  : dimensionality    (dimensionality   )
  , sumWeights        (0                )
  , selfEnergy        (0                )
  , coordinateScale   (dimensionality, 1)
  , potentialFunction (potentialFunction)
  , potentialGradient (potentialGradient)
{
  reserve(reserveSize);
}

//___________________________________________________________________________
MetricSpaceSample::MetricSpaceSample(const MetricSpaceSample& other)
  : dimensionality    (other.dimensionality   )
  , sumWeights        (0                      )
  , selfEnergy        (0                      )
  , coordinateScale   (other.coordinateScale  )
  , potentialFunction (other.potentialFunction)
  , potentialGradient (other.potentialGradient)
{ }


//___________________________________________________________________________
void MetricSpaceSample::normalize()
{
  for (unsigned iData = 0; iData < data.size(); ++iData)
    data[iData].weight /= sumWeights;
  sumWeights            = 1;
}

//___________________________________________________________________________
void MetricSpaceSample::addData(double weight, ...) 
{ 
  data.push_back(weight);

  sumWeights         += weight;
  WeightedDatum&      datum     = data.back();
  datum.location.resize(dimensionality);

  va_list             arguments;
  va_start(arguments, weight);
  for (unsigned iParam = 0; iParam < dimensionality; ++iParam)
    datum.location[iParam]      = va_arg(arguments, double)
                                / coordinateScale[iParam]
                                ;
  va_end(arguments);
}

//___________________________________________________________________________
void MetricSpaceSample::computeSelfInteraction()
{
  selfEnergy        = interactionEnergy ( *this
                                        , potentialGradient
                                        ? &selfField
                                        : 0
                                        );
}

//___________________________________________________________________________
double MetricSpaceSample::interactionEnergy(const MetricSpaceSample& sample, MetricVector* field) const 
{
  if (field) {
    field->assign(dimensionality, 0);
    assert(potentialGradient);
  }

  double                      energy    = 0;
  for (unsigned iData = 0; iData < data.size(); ++iData) {
    const WeightedDatum&      iDatum    = data[iData];
    for (unsigned jData = (&sample == this ? iData+1 : 0); jData < sample.data.size(); ++jData) {
      const WeightedDatum&    jDatum    = sample.data[jData];
      const double            distance2 = iDatum.location.distance2(jDatum.location);

      energy                 += iDatum.weight
                              * jDatum.weight
                              * (*potentialFunction)(distance2)
                              ;
      
      if (field) {
        const double          strength  = iDatum.weight
                                        * jDatum.weight
                                        * (*potentialGradient)(distance2)
                                        ;
        for (unsigned index = 0; index < dimensionality; ++index)
          (*field)[index]    += strength 
                              * ( jDatum.location[index]
                                - iDatum.location[index]
                                );
      }
    } // end loop over other data
  } // end loop over data

  assert(sumWeights);
  assert(sample.sumWeights);
  energy       /= sumWeights;
  energy       /= sample.sumWeights;
  return energy;
}



//=============================================================================
//    Paramatrix
//=============================================================================

template<>  TH1*    ParamatrixSample::histogram(const MetricSpaceSample*, const Int_t, const Bool_t) const    { return 0; }
template<>  void    ParamatrixSample::addTo    (MetricSpaceSample*, const MetricSpaceSample*, const Double_t) { }
template<>  Bool_t  ParamatrixSample::printDebug = kFALSE;

