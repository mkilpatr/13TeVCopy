#ifndef STATHEPER_H_
#define STATHEPER_H_
#include <TMath.h>

class StatHelper{
protected:
  int         numEntries;
  double      sumWeights;
  double      sumWeights2;
  double      mean;
  double      M2;
  double      minimum;
  double      maximum;

public:
  StatHelper() { clear(); }

  void clear()  { numEntries  = 0;  sumWeights  = 0;  sumWeights2 = 0;  mean  = 0;  M2  = 0;  minimum = 1e308;  maximum = -1e308;  }


  //---------------------------------------------------------------------------
  /// @name   Interface
  //---------------------------------------------------------------------------
  ///@{
public:
  int         getNumEntries         () const  { return numEntries;  }
  double      getSumWeights         () const  { return sumWeights;  }
  double      getSumWeights2        () const  { return sumWeights2; }
  double      getMean               () const  { return mean;        }
  double      getMeanUncertainty    () const  { return getRMS() * sumWeights2 / sumWeights / sumWeights;                       }
  double      getM2                 () const  { return M2;          }
  double      getSampleVariance     () const  { return ( numEntries > 1 ? (M2/sumWeights) * numEntries / (numEntries-1) : 0 ); }
  double      getPopulationVariance () const  { return ( numEntries > 1 ? M2/sumWeights                                 : 0 ); }
  double      getRMS                () const  { return TMath::Sqrt(getSampleVariance()); }
  double      getMinimum            () const  { return minimum;     }
  double      getMaximum            () const  { return maximum;     }
  ///@}



  //---------------------------------------------------------------------------
  /// @name   Combination
  //---------------------------------------------------------------------------
  ///@{
public:
  void  add(double x, double weight = 1);
  void  add(const StatHelper& other, const double weight = 1);
  ///@}
};


#endif

