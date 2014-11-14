/**
  @file         MetricCoordinate.h
  @author       Sue Ann Koay (sakoay@cern.ch)
*/

#ifndef __METRICCOORDINATE_H__
#define __METRICCOORDINATE_H__


#include <vector>
#include <stdarg.h>
#include <cmath>


//=============================================================================
//    MetricCoordinate
//=============================================================================

class MetricVector;
class MetricCoordinate : public std::vector<double> {
public:
  MetricCoordinate(unsigned dimensionality = 0, ...);
  MetricCoordinate(const std::vector<double>& coordinates);

  void              scale     (double number);

  double            distance2 (const MetricCoordinate& other) const;
  bool              equalTo   (const MetricCoordinate& other, double tolerance = 1e-6, double epsilon = 2e-7) const;
  bool              notEqual  (const MetricCoordinate& other, double tolerance = 1e-6, double epsilon = 2e-7) const;
  MetricCoordinate& add       (const MetricCoordinate& other, double scale = 1);
  MetricCoordinate& operator= (const MetricCoordinate& other);
  MetricCoordinate& operator+=(const MetricCoordinate& other) { return add(other,  1);  }
  MetricCoordinate& operator-=(const MetricCoordinate& other) { return add(other, -1);  }
  MetricVector      to        (const MetricCoordinate& other) const;
  bool              operator==(const MetricCoordinate& other) const;
  bool              operator!=(const MetricCoordinate& other) const;
  /**
    tolerance = maximum allowable fractional difference
    epsilon   = magnitudes below this are considered equal to 0
  */
  ////bool equal(double x, double y, double tolerance = 1e-10, double epsilon = 1e-14);   // double precision
  bool equal(double x, double y, double tolerance = 1e-6, double epsilon = 2e-7) const;         // float precision
};



//=============================================================================
//    MetricVector
//=============================================================================

class MetricVector : public MetricCoordinate {
public:
  MetricVector(unsigned dimensionality = 0, ...);
  MetricVector(const std::vector<double>& coordinates);

  double        length2   ()  const;
  double        length    ()  const { return sqrt(length2()); }
  void          normalize ();

  template<typename Vector>
  double        dot     (const Vector& other) const
  {
    assert(size() == other.size());

    double      product = 0;
    for (unsigned index = 0; index < size(); ++index)
      product  += operator[](index) * other[index];
    return product;
  }
};



//=============================================================================
//    WeightedDatum
//=============================================================================

class WeightedDatum {
public:
  MetricCoordinate      location;
  double                weight;
  WeightedDatum(double weight = -9) : weight(weight) { }
};



#endif //__METRICCOORDINATE_H__
