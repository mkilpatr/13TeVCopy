/**
  @file         StreamSamplerESWithJumps.h
  @author       Sue Ann Koay (sakoay@cern.ch)

  Adapted from:
    http://utopia.duth.gr/~pefraimi/projects/WRS/
*/

#ifndef STREAMSAMPLERER
#define STREAMSAMPLERER

#include <iostream>
#include <vector>

#include <TMath.h>
#include <TRandom.h>

#include "../interface/MetricSpaceSample.h"


//_____________________________________________________________________________
/**
 * One item of the Population
 *
 */

class WeightedItem {
protected:
  int     m_id;
  double  m_weight;
  double  m_TheoreticalProbability;

public:
  WeightedItem(int id, double weight = 1) 
    : m_id                    (id    )
    , m_weight                (weight)
    , m_TheoreticalProbability(-1    )
  { }

  int getID() const {
    return m_id;
  }

  double getWeight() const {
    return m_weight;
  }

  void setWeight(double weight) {
    m_weight = weight;
  }

  double genKey(TRandom* myRandom) const {
    return TMath::Power(myRandom->Uniform(), 1/m_weight);
  }

  void setTheoreticProb(double prob)
  {
    m_TheoreticalProbability = prob;
  }

  double getTheoreticProb() const
  {
    return m_TheoreticalProbability;
  }
  
  int compare(const WeightedItem& item) const {
	  return WeightedItem::compare(*this, item);
  }
  
  static int compare(const WeightedItem& wi1, const WeightedItem& wi2) {
		double item1Weight = wi1.getWeight();
		double item2Weight = wi2.getWeight();
		if (item1Weight > item2Weight)
			return 1;
		else if (item1Weight < item2Weight)
			return -1;
		else {
			// the weights are equal, use the ID's
			// this is important for data structures like TreeSet's
			// that do not allow duplicate items
			int id1 = wi1.getID();
			int id2 = wi2.getID();
			if (id1 < id2) {
				return 1;
			} else if (id1 > id2) {
				return -1;
			} else {
				return 0;
			}
		}
	}
};


//_____________________________________________________________________________
/**
 * A sampled item (for the random sample)
 */
class SampledItem {
public:
  WeightedItem      wItem;  ///< The corresponding weighted item
  int               order;  ///< The step when the item was selected (valid for Algorithm D)
  double            key ;   ///< The key of the item (valid for algorithm A)

  SampledItem() 
    : wItem (-1)
    , order (-1)
    , key   (-1)
  { }

  SampledItem(const WeightedItem& wItem, double key) 
    : wItem (wItem)
    , order (-1   )
    , key   (key  )
  { }

  SampledItem(const WeightedItem& wItem, int order) 
    : wItem (wItem)
    , order (order)
    , key   (-1   )
  { }
};

typedef std::vector<SampledItem>  SampledSet;


//_____________________________________________________________________________
class StreamSampler 
{
protected:
	TRandom*  random;
	int       sampleSize;     ///< sampleSize, reservoirLength
	long      itemsProcessed; ///< the number of items that have been processed so far


public:
	StreamSampler(int parSampleSize, TRandom* random) 
    : random        (random       )
    , sampleSize    (parSampleSize)
    , itemsProcessed(0            )
  {
	}

	virtual void        initiate  ()                            = 0;
	virtual void        feedItem  (const WeightedItem& newItem) = 0;
	virtual SampledSet  getSample ()                            = 0;
	virtual int         getNumOfInsertions()                    = 0;
};
#endif
