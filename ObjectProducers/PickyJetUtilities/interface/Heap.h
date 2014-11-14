/**
  @file         Heap.h
  @author       Sue Ann Koay (sakoay@cern.ch)

  Adapted from:
    http://utopia.duth.gr/~pefraimi/projects/WRS/
*/
#ifndef HEAP
#define HEAP
#include <vector>
#include <algorithm>
#include <assert.h>

#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "ObjectProducers/PickyJetUtilities/interface/StreamSampler.h"

template<typename E>
class Heap 
{
protected:
	std::vector<E*> vHeap;  // Contains the items of the heap
	int             last;   // The last valid entry of the heap
	int             size;

public:
	Heap(int parSize) 
    : vHeap (parSize)
    , last  (-1     )
    , size  (parSize)
  { }

  ~Heap() {
    PhysicsUtilities::trash(vHeap);
  }

	const E* rootItem() {
		if (last >= 0) {
			return vHeap.front();
		} else {
			return 0;
		}
	}

	int getLength() {
		return last + 1;
	}

	void clear() {
	  PhysicsUtilities::trash(vHeap);
		last = -1;
	}

	void clearAndInit() {
	  PhysicsUtilities::trash(vHeap);
    vHeap.resize(size, 0);
		last = -1;
	}
	
	E* addItem(E* item) {
		++last;
    assert(last >= 0);
    assert(last < int(vHeap.size()));
    assert(vHeap[last] == 0);
		vHeap[last]   = item;

		int           current = last;
		SampledItem*  cItem   = 0; // current
		SampledItem*  pItem   = 0; // parent
		bool          bStop   = false;
		do {
  		int         parent  = (current - 1) / 2;
			if (parent >= current || parent < 0) {
				bStop     = true;
				break;
			} else {
				cItem     = vHeap[current];
				pItem     = vHeap[parent ];
				if (cItem->key < pItem->key) {
					swapItems(current, parent);
					current = parent;
				} else {
					bStop   = true;
					break;
				}
			}
		} while (!bStop);

		return item;
	}

	void swapItems(int item1, int item2) {
    std::swap(vHeap[item1], vHeap[item2]);
	}

	E* replaceHead(E* item) {
		if (last < 0) {
			// Heap Empty
			// throw exception
      assert(false);
		} else {
      delete vHeap[0];
			vHeap[0]  = item;
			percolateDown();
		}

		return item;
	}

	E* peek(int item) const {
		return vHeap[item];
	}

protected:
	void percolateDown() {
		int           current = 0;
		SampledItem*  cItem   = 0;      // current
		SampledItem*  lItem   = 0;      // left
		SampledItem*  rItem   = 0;      // right
		SampledItem*  mItem   = 0;      // min
		bool          bStop   = false;
		do {
			if (current <= last) {
				cItem     = vHeap[current];
			} else {
				// current (index) is out of the heap
				return;
			}
			int         left    = 2 * current + 1;
			int         right   = 2 * current + 2;
			if (left <= last) {
				lItem     = vHeap[left];

        int       min;
				if (right > last) {
					min     = left;
					mItem   = lItem;
				} else {
					rItem   = vHeap[right];
					if (lItem->key < rItem->key) {
						min   = left;
						mItem = lItem;
					} else {
						min   = right;
						mItem = rItem;
					}
				}
				// Compare current with min
				if (cItem->key > mItem->key) {
					swapItems(current, min);
					current = min;
				} else {
					bStop   = true;
					break;
				}
			} else {
				bStop     = true;
				break;
			}
		} while (!bStop);
	}
};
#endif
