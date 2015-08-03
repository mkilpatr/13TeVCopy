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
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "AnalysisTools/Utilities/interface/Types.h"
#include "AnalysisTools/Utilities/interface/TreeWriter.h"
#include "AnalysisTools/Utilities/interface/TreeWriterData.h"
#include "AnalysisBase/Analyzer/interface/FileUtilities.h"

using namespace std;

namespace ucsbsusy {

  class BaseFiller {

  public:
    BaseFiller() : options_(0), branchName_(""), isLoaded_(false), isFilled_(false) {};
    BaseFiller(const int options, const std::string branchName) : options_(options), branchName_(branchName), isLoaded_(false), isFilled_(false) {};
    virtual ~BaseFiller() {};

    // Create branches needed in the tree
    virtual void book(TreeWriter& tW) {data.book(&tW);}

    //virtual void beginJob(edm::Run const&, edm::EventSetup const&)=0;

    //virtual void beginRun(edm::Run const &run, edm::EventSetup const &iSetup)=0;

    // Reset objects
    virtual void reset() {isLoaded_ = false; isFilled_ = false; data.reset();}

    // Default loading function
    // This guy needs to be defined for each implementation and is run once per event
    // If storeOnlyPtr is true object creation is cancelled and only the pointer is loaded
    virtual void load(const edm::Event& iEvent, const edm::EventSetup &iSetup = _dummySetup) = 0;

    // Tree filling function
    virtual void fill() = 0;

    bool isLoaded() const {return isLoaded_;}
    bool isFilled() const {return isFilled_;}

  protected:
    const int options_; //filling options
    const std::string branchName_;  //branch prefix
    TreeWriterData data;
    bool isLoaded_;
    bool isFilled_;
    static edm::EventSetup _dummySetup;

  };

}

#endif /* BASEFILLER_H_ */
