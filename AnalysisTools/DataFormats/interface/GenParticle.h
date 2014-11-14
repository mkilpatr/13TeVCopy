#ifndef ANALYSISTOOLS_DATAFORMATS_GENPARTICLE_H
#define ANALYSISTOOLS_DATAFORMATS_GENPARTICLE_H


#define FLVRECOASSOC

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {
template <class CoordSystem>
class GenParticle : public Momentum<CoordSystem>
{
public :
  GenParticle() : index_(-1), status_(-1), pdgId_(-1) {}

  template <class InputCoordSystem>
  GenParticle(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum, int inIndex = -1, int inStatus = -1, int inPdgId = -9999) : Momentum<CoordSystem>(inMomentum),
  index_(inIndex),status_(inStatus),pdgId_(inPdgId) {}

  ~GenParticle(){}

  int	index()				const { return index;	}
  void	setIndex(const int newIndex)	{ index_ = newIndex;	}

  int status()       const { return status_; }
  void  setStatus(const int newStatus)  { status_ = newStatus;  }

  int pdgId()       const { return pdgId_; }
  void  setPdgId(const int newPdgId)  { pdgId_ = newPdgId;  }

protected :
  int	index_;  //Index in genparticle vector
  int status_;
  int pdgId_;


};
typedef GenParticle<CylLorentzCoordF> GenParticleF;
typedef std::vector<GenParticleF>     GenParticleFCollection;
}

#endif
