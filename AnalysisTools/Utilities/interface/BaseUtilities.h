/*
 * BaseUtilities.h
 *
 *  Created on: Aug 20, 2014
 *      Author: nmccoll
 */

#ifndef BASEUTILITIES_H_
#define BASEUTILITIES_H_

#include <map>
#include <TString.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "FWCore/Utilities/interface/Exception.h"

namespace ucsbsusy {

typedef   unsigned int                  size;
typedef   unsigned char                 multiplicity;

class BaseUtilities {
public:
  //_____________________________________________________________________________
  // Basic sorting
  //_____________________________________________________________________________
  template<typename Field, typename Object>
  struct lesserAbsFirst : public std::binary_function<const std::pair<Field,Object>&, const std::pair<Field,Object>&, Bool_t> {
    Bool_t operator()(const std::pair<Field,Object>& x, const std::pair<Field,Object>& y) const {
      return std::abs(x.first) < std::abs(y.first);
    }
  };
  template<typename Field, typename Object>
  struct greaterAbsFirst : public std::binary_function<const std::pair<Field,Object>&, const std::pair<Field,Object>&, Bool_t> {
    Bool_t operator()(const std::pair<Field,Object>& x, const std::pair<Field,Object>& y) const {
      return std::abs(x.first) > std::abs(y.first);
    }
  };

  template<typename Field1, typename Field2, typename Pair = std::pair<Field1,Field2> >
  struct greaterFirstOrSecond : public std::binary_function<Pair, Pair, Bool_t> {
    Bool_t operator()(const Pair& t1, const Pair& t2) const
    {
      if (t1.first  > t2.first )        return kTRUE  ;
      if (t1.first  < t2.first )        return kFALSE ;
      if (t1.second > t2.second)        return kTRUE  ;
      if (t1.second < t2.second)        return kFALSE ;
      return kFALSE;
    }
  };

  template<typename T1, typename T2 = T1>
  struct AbsDeltaPhi {
    double operator()(const T1 & t1, const T2 & t2) const {
      return std::abs(reco::deltaPhi(t1.phi(), t2.phi()));
    }
  };

  template<typename Object>
  struct greaterPTDeref : public std::binary_function<const Object*, const Object*, Bool_t> {
    Bool_t operator()(const Object* x, const Object* y) const { return x->pt() > y->pt(); }
  };
  template<typename Object>
  struct greaterEDeref : public std::binary_function<const Object*, const Object*, Bool_t> {
    Bool_t operator()(const Object* x, const Object* y) const { return x->energy() > y->energy(); }
  };

  template<typename ObjectRef>
  struct greaterRefPT : public std::binary_function<const ObjectRef&, const ObjectRef&, Bool_t> {
    Bool_t operator()(const ObjectRef& x, const ObjectRef& y) const { return x->pt() > y->pt(); }
  };

  template<typename Object>
  struct greaterPT : public std::binary_function<const Object&, const Object&, Bool_t> {
    Bool_t operator()(const Object& x, const Object& y) const { return x.pt() > y.pt(); }
  };

  //_____________________________________________________________________________
  // Number conversion
  //_____________________________________________________________________________
  template<typename Target, typename Source>
  Target convertTo(Source source, const char name[], bool lenient = false, bool* good = 0);

  //_____________________________________________________________________________
  // vector destruction
  //_____________________________________________________________________________
  template<typename Object>
  static void trash(std::vector<Object*>& objects);

  template<typename Key, typename Object>
  static void trash(std::map<Key,Object*>& objects);

  //_____________________________________________________________________________
  // InputTag name output
  //_____________________________________________________________________________
  static TString str(const edm::InputTag& tag)  { return tag.encode(); }
  static TString str(const std::string&   tag)  { return tag.data();   }
  static TString str(const char*          tag)  { return tag;          }
  static TString str(const TString&       tag)  { return tag;          }

  //_____________________________________________________________________________
  // EDM object getting
  //_____________________________________________________________________________
  /**
    Helper function to load a product only if it is part of event content. This
    decision is stored in the provided isAvailable variable, which MUST be
    initialized to true before the first event if you want it to check at all.
  */
  template<typename Source, typename Tag, typename Product>
  static bool tryToGet(const Source& source, const Tag& tag, edm::Handle<Product>& product, int numAnalyzed, bool& isAvailable);

  template<typename Source, typename Tag, typename Product>
  static bool tryToGet(const Source& source, const Tag& tag, const Product*& product, int numAnalyzed, bool& isAvailable);

  //_____________________________________________________________________________
  /// Helper function to ensure loading of a product, throwing an exception if this fails.
  template<typename Source, typename Tag, typename Product>
  static bool enforceGet(const Source& source, const Tag& tag, edm::Handle<Product>& product, bool enforcePresence = true);

  template<typename Source, typename Tag, typename Product>
  static bool enforceGet(const Source& source, const Tag& tag, const Product*& product, bool enforcePresence = true);

  //_______________________________________________________________________
  // File opening and object getting
  //_____________________________________________________________________________
  static TFile* open(const char* path, const char option[], bool stopIfMissing = true){
    if (path == 0 || path[0] == 0)  return 0;

    if (!stopIfMissing) {
      Long_t    id, flags, modtime;
      Long64_t  fileSize;
      if (gSystem->GetPathInfo(path, &id, &fileSize, &flags, &modtime) || flags)
        return 0;
    }

    TFile*    file  = TFile::Open(path, option);
    if (stopIfMissing && file->IsZombie())
      throw cms::Exception("BaseUtilities.open()", TString::Format("Failed to open '%s' for %s.", path, (file->IsWritable() ? "output" : "input")).Data());
    return file;
  }

  template<typename Object>
  static Object* getObject(TFile* file, const char* name, const char* alternative = 0, bool stopIfMissing = true);

  template<typename Object>
  static Object* loadObject(TString objectPath, const char* configName = 0, const char* alternative = 0, bool stopIfMissing = true);
};

}

#include "AnalysisTools/Utilities/src/BaseUtilities.icc"

#endif /* BASEUTILITIES_H_ */
