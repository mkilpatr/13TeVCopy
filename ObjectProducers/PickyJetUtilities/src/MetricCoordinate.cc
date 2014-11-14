/**
  @file         ShapeTools.cc
  @author       Sue Ann Koay (sakoay@cern.ch)
*/


#include "../interface/MetricCoordinate.h"

#include <cassert>


using namespace std;

namespace {

template<typename Number>
static Number sqr(Number x) { return x*x; }

}

//_____________________________________________________________________________
bool MetricCoordinate::equal(double x, double y, double tolerance, double epsilon) const
{
  if ((fabs(x) < epsilon) && (fabs(y) < epsilon))
    return true;
  return fabs(x - y) <= tolerance * fabs(x + y);
}
//=============================================================================
//    MetricCoordinate
//=============================================================================

MetricCoordinate::MetricCoordinate(unsigned dimensionality, ...)
  : std::vector<double>(dimensionality)
{
  if (dimensionality) {
    va_list               arguments;
    va_start(arguments, dimensionality);
    for (unsigned iParam = 0; iParam < dimensionality; ++iParam)
      operator[](iParam)  = va_arg(arguments, double);
    va_end(arguments);
  }
}

MetricCoordinate::MetricCoordinate(const std::vector<double>& coordinates)
  : std::vector<double>(coordinates)
{
}

void MetricCoordinate::scale(double number)
{
  for (unsigned index = 0; index < size(); ++index)
    operator[](index)  *= number;
}

double MetricCoordinate::distance2(const MetricCoordinate& other) const
{
  double  distance  = 0;
  for (unsigned index = 0; index < size(); ++index)
    distance       += sqr(operator[](index) - other[index]);

//assert(distance);
  assert(distance == distance);
  return distance;
}

bool MetricCoordinate::equalTo(const MetricCoordinate& other, double tolerance, double epsilon) const
{
  for (unsigned index = 0; index < size(); ++index)
    if (!equal(operator[](index), other[index], tolerance, epsilon))
      return false;
  return true;
}

bool MetricCoordinate::notEqual(const MetricCoordinate& other, double tolerance, double epsilon) const
{
  for (unsigned index = 0; index < size(); ++index)
    if (!equal(operator[](index), other[index], tolerance, epsilon))
      return true;
  return false;
}

bool MetricCoordinate::operator==(const MetricCoordinate& other) const
{
  for (unsigned index = 0; index < size(); ++index)
    if (operator[](index) != other[index])
      return false;
  return true;
}

bool MetricCoordinate::operator!=(const MetricCoordinate& other) const
{
  for (unsigned index = 0; index < size(); ++index)
    if (operator[](index) != other[index])
      return true;
  return false;
}

MetricCoordinate& MetricCoordinate::add(const MetricCoordinate& other, double scale)
{
  assert(size() == other.size());

  for (unsigned index = 0; index < size(); ++index)
    operator[](index)  += scale * other[index];
 
  return *this;
}

MetricCoordinate& MetricCoordinate::operator=(const MetricCoordinate& other)
{
  if (size() != other.size())
    resize(other.size());

  for (unsigned index = 0; index < size(); ++index)
    operator[](index) = other[index];
 
  return *this;
}


MetricVector MetricCoordinate::to(const MetricCoordinate& other) const
{
  MetricVector      displacement(size());
  for (unsigned index = 0; index < size(); ++index)
    displacement[index] = other[index] - operator[](index);
  return displacement;
}


//=============================================================================
//    MetricVector
//=============================================================================

MetricVector::MetricVector(unsigned dimensionality, ...)
  : MetricCoordinate(dimensionality)
{
  if (dimensionality) {
    va_list               arguments;
    va_start(arguments, dimensionality);
    for (unsigned iParam = 0; iParam < dimensionality; ++iParam)
      operator[](iParam)  = va_arg(arguments, double);
    va_end(arguments);
  }
}

MetricVector::MetricVector(const std::vector<double>& coordinates)
  : MetricCoordinate(coordinates)
{
}

double MetricVector::length2() const
{
  double  distance  = 0;
  for (unsigned index = 0; index < size(); ++index)
    distance       += sqr(operator[](index));
  return distance;
}

void MetricVector::normalize()
{
  const double    norm  = length();
  if (norm)       scale(1. / norm);
}

