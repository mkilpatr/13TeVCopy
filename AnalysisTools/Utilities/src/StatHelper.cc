#include "AnalysisTools/Utilities/interface/StatHelper.h"

//___________________________________________________________________________
void StatHelper::add(double x, double weight)
{
  if (!weight)      return;

  sumWeights2      += weight * weight;

  const double    temp    = weight + sumWeights;
  const double    delta   = x - mean;
  const double    R       = delta * weight / temp;
  mean             += R;
  if (sumWeights > 0)
    M2             += sumWeights * delta * R;
  sumWeights        = temp;
  ++numEntries;

  if (x < minimum)  minimum = x;
  if (x > maximum)  maximum = x;
}

//___________________________________________________________________________
void StatHelper::add(const StatHelper& other, const double scale)
{
  numEntries       += other.numEntries;
  const Double_t    otherW  = other.sumWeights * scale;
  const Double_t    total   = sumWeights + otherW;
  if (total <= 0)   return;

  const Double_t    delta   = other.mean - mean;
  mean             += delta * otherW / total;
  M2               += other.M2 + delta*delta * sumWeights * otherW / total;
  sumWeights       += otherW;

  if (other.minimum < minimum)
    minimum         = other.minimum;
  if (other.maximum > maximum)
    maximum         = other.maximum;
}

