/**
  @file         StreamSamplerESWithJumps.h
  @author       Sue Ann Koay (sakoay@cern.ch)

  Adapted from:
    http://utopia.duth.gr/~pefraimi/projects/WRS/
*/

#ifndef STREAMSAMPLERESWITHJUMPS
#define STREAMSAMPLERESWITHJUMPS

#include <vector>
#include <iostream>
#include <stdarg.h>

#include "../interface/FastFunctions.h"
#include "../interface/Heap.h"
#include "../interface/StreamSampler.h"


class StreamSamplerESWithJumps : public StreamSampler
{
protected:
	Heap<SampledItem>       myHeap;
	int   numOfInsertions;  ///< Contains the number of jumps/reservoir insertions

                          ///< Data of Last Exponential Jump
  bool   inFlight;        ///< The current exponential is not exhausted yet
  double r1;              ///< the random variate used in the current exponential jump
  double expJump;         ///< the current exponential jump

  double currentWeight;
  double nextWeight;
  long   index;           ///< The item
  long   currentIndex;    ///< current index
	
public:
	/**
	 * 
	 * @param parSampleSize
	 * @param parSeed
	 */
	StreamSamplerESWithJumps(int parSampleSize, TRandom* random)
    : StreamSampler (parSampleSize, random)
    , myHeap        (parSampleSize)
  { 
	  initiate();
  }

	void initiate() {
		myHeap.clearAndInit();
		
		itemsProcessed  = 0;
		numOfInsertions = 0;
		inFlight        = false;

    r1              = -999;
    expJump         = -999;
    currentWeight   = -999;
    nextWeight      = -999;
    index           = -1;
    currentIndex    = -1;
  }

	
	void feedItem(const WeightedItem& newItem) {
		double              currentThreshold;
		if (itemsProcessed < sampleSize) {
			// The first m items go directly into the reservoir
			double            key       = newItem.genKey(random);
			myHeap.addItem(new SampledItem(newItem, key));
		} else {
			// current threshold to enter the reservoir
			currentThreshold  = myHeap.rootItem()->key;

			// Exponential Jump
			if (!inFlight) {		
				// Generate exponential jump
				r1              = random->Uniform();
				expJump         = fastlog(r1) / fastlog(currentThreshold);
				
				currentWeight   = 0;
				nextWeight      = 0;
				index           = -1;
				currentIndex    = itemsProcessed;
				
				inFlight        = true;
			}		
			
			// Check if the Exponential Jump lands on the current item
			nextWeight        = currentWeight + newItem.getWeight();
			if (expJump < nextWeight) {
				index           = itemsProcessed;
				double          lowJ      = currentWeight;
				double          highJ     = nextWeight;
				
				// Prepare the new item
				SampledItem*    sItem     = new SampledItem();
				sItem->wItem    = newItem;

				// We have to calculate a key for the new item
				// The ley has to be in the interval (key-of-replaced-item, max-key]
				double          weight    = sItem->wItem.getWeight();
				double          lowR      = fastpow(currentThreshold, weight);

				// We use the random number of the exponential jump 
				// to calculate the random key
				// The random number has to be "normalized" for its new use
				double          lthr      = fastpow(currentThreshold, highJ);
				double          hthr      = fastpow(currentThreshold, lowJ);
				double          r2        = (r1 - lthr) / (hthr - lthr);

				// OK double r3 = lowR + (1-lowR) * myRandom.rand();
				double          r3        = lowR + (1 - lowR) * r2; // myRandom.rand();
				double          key       = fastpow(r3, 1 / weight);

				sItem->key      = key;

				// Insert the Item into the Reservoir
				myHeap.replaceHead(sItem);
				++numOfInsertions;
				
				inFlight        = false;
			} else {
				currentWeight   = nextWeight;
			}	
		}

		++itemsProcessed;
	}

	SampledSet getSample() {
		int             numOfItemsInSample = myHeap.getLength();

		// Prepare the output: SampledSet
		SampledSet      sample;
    sample.reserve(numOfItemsInSample);
		for (int i = 0; i < numOfItemsInSample; ++i)
      sample.push_back(* myHeap.peek(i));
    return sample;
	}

	int getNumOfInsertions() {
		return numOfInsertions;
	}
};

#endif
