/*
 * BaseUtilities.h
 *
 *  Created on: Aug 20, 2014
 *      Author: nmccoll
 */

#ifndef BASEUTILITIES_H_
#define BASEUTILITIES_H_

#include <TString.h>

#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/LorentzVector.h"

namespace ucsbsusy {

typedef   unsigned int                  size;
typedef   unsigned char                 multiplicity;

class BaseUtilities {
public:
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

  //_____________________________________________________________________________
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

  //_____________________________________________________________________________
  template<typename Target, typename Source>
  Target convertTo(Source source, const char name[], bool lenient = false, bool* good = 0)
  {
    Target            converted = static_cast<Target>(source);

    if (static_cast<Source>(converted) != source) {
      const Target    lowest    = !std::numeric_limits<Target>::is_signed
                                ? 0
                                : std::numeric_limits<Target>::has_infinity
                                ? -std::numeric_limits<Target>::infinity()
                                :  std::numeric_limits<Target>::min()
                                ;
      TString         problem;
      problem.Form( "Source value %.10g outside of target range [%.10g,%.10g] for '%s'."
                  , (double)  source
                  , (double)  lowest
                  , (double)  std::numeric_limits<Target>::max()
                  , name
                  );
      if (good)      *good      = false;
      if (lenient) {
        std::cerr << "WARNING: " << problem << std::endl;
        return  ( source > static_cast<Source>(std::numeric_limits<Target>::max())
                ? std::numeric_limits<Target>::max()
                : lowest
                );
      }
      throw cms::Exception("convertTo()", problem.Data());
    }

    return converted;
  }

  //_____________________________________________________________________________
  template<typename Object>
  static void trash(std::vector<Object*>& objects)
  {
    const size    numObjects  = objects.size();
    for (size iObj = 0; iObj < numObjects; ++iObj)
      if (objects[iObj])      delete objects[iObj];
    objects.clear();
  }

  //_____________________________________________________________________________
  template<typename Key, typename Object>
  static void trash(std::map<Key,Object*>& objects)
  {
    const typename std::map<Key,Object*>::const_iterator endObjects  = objects.end();
    for (typename std::map<Key,Object*>::iterator iObj = objects.begin(); iObj != endObjects; ++iObj)
      if (iObj->second)       delete iObj->second;
    objects.clear();
  }
};

}


#endif /* BASEUTILITIES_H_ */
