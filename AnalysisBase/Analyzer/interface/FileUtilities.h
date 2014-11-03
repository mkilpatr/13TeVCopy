/*
 * BaseUtilities.h
 *
 *  Created on: Aug 20, 2014
 *      Author: nmccoll
 */

#ifndef FILEUTILITIES_H_
#define FILEUTILITIES_H_

#include <map>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "AnalysisTools/Utilities/interface/Types.h"

namespace ucsbsusy {

namespace FileUtilities{
//_____________________________________________________________________________
// InputTag name output
//_____________________________________________________________________________
inline TString str(const edm::InputTag& tag)  { return tag.encode(); }
inline TString str(const std::string&   tag)  { return tag.data();   }
inline TString str(const char*          tag)  { return tag;          }
inline TString str(const TString&       tag)  { return tag;          }


//_____________________________________________________________________________
// EDM object getting
//_____________________________________________________________________________
/**
  Helper function to load a product only if it is part of event content. This
  decision is stored in the provided isAvailable variable, which MUST be
  initialized to true before the first event if you want it to check at all.
*/
template<typename Source, typename Tag, typename Product>
bool tryToGet(const Source& source, const Tag& tag, edm::Handle<Product>& product, int numAnalyzed, bool& isAvailable);

template<typename Source, typename Tag, typename Product>
bool tryToGet(const Source& source, const Tag& tag, const Product*& product, int numAnalyzed, bool& isAvailable);

//_____________________________________________________________________________
/// Helper function to ensure loading of a product, throwing an exception if this fails.
template<typename Source, typename Tag, typename Product>
bool enforceGet(const Source& source, const Tag& tag, edm::Handle<Product>& product, bool enforcePresence = true);

template<typename Source, typename Tag, typename Product>
bool enforceGet(const Source& source, const Tag& tag, const Product*& product, bool enforcePresence = true);


//_______________________________________________________________________
// File opening and object getting
//_____________________________________________________________________________
TFile* open(const char* path, const char option[], bool stopIfMissing = true);

template<typename Object>
Object* getObject(TFile* file, const char* name, const char* alternative = 0, bool stopIfMissing = true);

template<typename Object>
Object* loadObject(TString objectPath, const char* configName = 0, const char* alternative = 0, bool stopIfMissing = true);
}
}

#include "AnalysisBase/Analyzer/src/FileUtilities.icc"

#endif /* BASEUTILITIES_H_ */
