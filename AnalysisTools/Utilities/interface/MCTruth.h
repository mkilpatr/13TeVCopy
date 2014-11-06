#ifndef ANALYSISTOOLS_UTILITIES_PARTICLEINFO_H
#define ANALYSISTOOLS_UTILITIES_PARTICLEINFO_H

#include "DataFormats/Candidate/interface/Candidate.h"

namespace ucsbsusy {

  class MCTruth
  {

    public :
      MCTruth() {}

      static bool isAncestor(const reco::Candidate* ancestor, const reco::Candidate* particle)
      {
	//particle is already the ancestor
	if(ancestor == particle) return true;
	//otherwise loop on mothers, if any and return true if the ancestor is found
	for(size_t i=0; i<particle->numberOfMothers(); i++) {
	  if(isAncestor(ancestor,particle->mother(i))) return true;
	}
	//if we did not return yet, then particle and ancestor are not relatives
	return false;
      }

  };

}

#endif
