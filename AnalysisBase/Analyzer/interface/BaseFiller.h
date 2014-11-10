/*
 * BaseFiller.h
 *
 *  Base class for object getting, storing, and filling
 *
 *  There are three responsibilities for this class:
 *  1) Get configuration parameters
 *  2) Load the edm object into a pointer owned by this class and (optionally) fill a custom object with the info
 *  3) Provide a ttree filling interface
 *
 *  Created on: Aug 22, 2014
 *      Author: nmccoll
 */

#ifndef BASEFILLER_H_
#define BASEFILLER_H_

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "AnalysisTools/Utilities/interface/Types.h"
#include "AnalysisTools/Utilities/interface/TreeWriter.h"
#include "AnalysisTools/Utilities/interface/TreeWriterData.h"
#include "AnalysisBase/Analyzer/interface/FileUtilities.h"

using namespace std;

namespace ucsbsusy {

  class BaseFiller {

  public:
    BaseFiller() : isLoaded_(false), isFilled_(false) {};
    virtual ~BaseFiller() {};

    // Create branches needed in the tree
    virtual void	book(TreeWriter& tW) {data.book(&tW);}

    // Reset objects
    virtual void	reset() {isLoaded_ = false; isFilled_ = false; data.reset();}

    // Default loading function
    // This guy needs to be defined for each implementation and is run once per event
    // If storeOnlyPtr is true object creation is cancelled and only the pointer is loaded
    virtual void	load(edm::Event& iEvent) = 0;

    // Tree filling function
    // numAnalyzed is to be used to keep track of number of analyzed events
    virtual void	fill(TreeWriter& tW, const int& numAnalyzed) = 0;

    bool isLoaded() const {return isLoaded_;}
    bool isFilled() const {return isFilled_;}

  protected:
    TreeWriterData data;
    bool isLoaded_;
    bool isFilled_;

  };

}

#endif /* BASEFILLER_H_ */
