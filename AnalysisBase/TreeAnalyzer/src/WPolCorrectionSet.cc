#include "AnalysisBase/TreeAnalyzer/interface/WPolCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"


namespace ucsbsusy {

void WPolCorrectionSet::load(int correctionOptions) {
  if (correctionOptions & WPOLWGT)    { wpolon = true; }
  else                                { wpolon = false; }
}


void WPolCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  float wpolunc = 1.;
  //  if ( (ana->process == defaults::TTBAR) || (ana->process == defaults::SINGLE_T)) { wpolunc = 0.05; }
  if ( ana->process == defaults::TTBAR )    { wpolunc = 0.05; }
  if ( ana->process == defaults::SINGLE_W ) { wpolunc = 0.10; }

  std::vector<TLorentzVector> genlepp4;             genlepp4.clear();
  std::vector<TLorentzVector> genwp4;               genwp4.clear();
  std::vector<TLorentzVector> genlepinwrestframep4; genlepinwrestframep4.clear();

  std::vector<TLorentzVector> gentopp4;             gentopp4.clear();
  std::vector<TLorentzVector> genwintoprestframep4; genwintoprestframep4.clear();


  for (const auto * p : ana->genParts) {

    const GenParticleF * genPartMom = 0;
    if (p->numberOfMothers()>0) { genPartMom = p->mother(0); }
    else                        { continue; }

    const GenParticleF * genPartMomOfMom = 0;
    if (genPartMom->numberOfMothers()>0) { genPartMomOfMom = genPartMom->mother(0); }

    if (abs(genPartMom->pdgId())==24) {
      if ( (abs(p->pdgId())==11) || (abs(p->pdgId())==13) || (abs(p->pdgId())==15) ) {
        TLorentzVector tmplep; tmplep.SetPxPyPzE(p->px(),p->py(),p->pz(),p->E());
        genlepp4.push_back(tmplep);
        TLorentzVector tmpw; tmpw.SetPxPyPzE(genPartMom->px(),genPartMom->py(),genPartMom->pz(),genPartMom->E());
        genwp4.push_back(tmpw);

        // only in ttbar events
        if ( ( (ana->process == defaults::TTBAR) || (ana->process == defaults::SINGLE_T) )
            && (genPartMomOfMom) && (abs(genPartMomOfMom->pdgId())==6) ) {
          TLorentzVector tmptop; tmptop.SetPxPyPzE(genPartMomOfMom->px(),genPartMomOfMom->py(),genPartMomOfMom->pz(),genPartMomOfMom->E());
          gentopp4.push_back(tmptop);
        }
      }
    }
  } // end of looping over the gen particles

  std::vector<float> costhetastar_; costhetastar_.clear();
  std::vector<float> wpolweightup_; wpolweightup_.clear();
  std::vector<float> wpolweightdn_; wpolweightdn_.clear();

  for (unsigned int i0=0; i0<genlepp4.size(); ++i0) {

    // w+jets events
    if ( (ana->process == defaults::SINGLE_W) && (genwp4.size()>0) && (genlepp4.size()>0) ) {
      TVector3 boostw; boostw = genwp4[i0].BoostVector();
      TLorentzVector genlepinwrestframep4_ = genlepp4[i0];
      genlepinwrestframep4_.Boost(-boostw);
      genlepinwrestframep4.push_back(genlepinwrestframep4_);
      float tmpcosthetastar_ = cos(ROOT::Math::VectorUtil::Angle(genlepinwrestframep4[i0],genwp4[i0]));
      float tmpwpolweightup_ = 1+wpolunc*(1-tmpcosthetastar_)*(1-tmpcosthetastar_);
      float tmpwpolweightdn_ = 1-wpolunc*(1-tmpcosthetastar_)*(1-tmpcosthetastar_);
      costhetastar_.push_back(tmpcosthetastar_);
      wpolweightup_.push_back(tmpwpolweightup_);
      wpolweightdn_.push_back(tmpwpolweightdn_);
    }

    // ttbar events
    if ( (ana->process == defaults::TTBAR || (ana->process == defaults::SINGLE_T) )  &&
        (gentopp4.size()>0) && (genwp4.size()>0) && (genlepp4.size()>0) ) {
      TVector3 boostw; boostw = genwp4[i0].BoostVector();
      TLorentzVector genlepinwrestframep4_ = genlepp4[i0];
      genlepinwrestframep4_.Boost(-boostw);
      genlepinwrestframep4.push_back(genlepinwrestframep4_);

      TVector3 boosttop; boosttop = gentopp4[i0].BoostVector();
      TLorentzVector genwintoprestframep4_ = genwp4[i0];
      genwintoprestframep4_.Boost(-boosttop);
      genwintoprestframep4.push_back(genwintoprestframep4_);

      float tmpcosthetastar_ = cos(ROOT::Math::VectorUtil::Angle(genlepinwrestframep4[i0],genwintoprestframep4[i0]));
      float tmpwpolweightup_ = 1+wpolunc*(1-tmpcosthetastar_)*(1-tmpcosthetastar_);
      float tmpwpolweightdn_ = 1-wpolunc*(1-tmpcosthetastar_)*(1-tmpcosthetastar_);
      costhetastar_.push_back(tmpcosthetastar_);
      wpolweightup_.push_back(tmpwpolweightup_);
      wpolweightdn_.push_back(tmpwpolweightdn_);
    }

    break;
  }

  float wgtnormup = 1.; 
  float wgtnormdn = 1.;

  // 74X
  // ttbar   : sumwgt sumwgtpolup sumwgtpoldn = 452650. 487884. 417416.
  // w+jets  : sumwgt sumwgtpolup sumwgtpoldn = 2.14475e+06 2.45384e+06 1.83567e+06
  // single-t: sumwgt sumwgtpolup sumwgtpoldn = 71200 72815.9 69584.1 --- negative weights

  // 80X
  // ttbar : sumwgt,sumwgtpolup,sumwgtpoldn final = 451670 486828 416512
  // wjets :sumwgt,sumwgtpolup,sumwgtpoldn final = 2.14345e+06 2.45234e+06 1.83455e+06

  if ((ana->process == defaults::TTBAR)    && (wpolweightup_.size()>0)) { wgtnormup = 451670./486828.;         wgtnormdn = 451670./416512.;         }
  if ((ana->process == defaults::SINGLE_W) && (wpolweightup_.size()>0)) { wgtnormup = 2.14345e+06/2.45234e+06; wgtnormdn = 2.14345e+06/1.83455e+06; }
  //  if ((ana->process == defaults::SINGLE_T) && (wpolweightup_.size()>0)) { wgtnormup = 71200./72815.9;          wgtnormdn = 71200./69584.1; }

  if (wpolweightup_.size()>0) { wpolweightup = wpolweightup_[0]*wgtnormup; } else { wpolweightup = 1.;  }
  if (wpolweightdn_.size()>0) { wpolweightdn = wpolweightdn_[0]*wgtnormdn; } else { wpolweightdn = 1.;  }
  //if (wpolweightup_.size()>0) { wpolweightup = wpolweightup_[0]; } else { wpolweightup = 1.;  }
  //if (wpolweightdn_.size()>0) { wpolweightdn = wpolweightdn_[0]; } else { wpolweightdn = 1.;  }
  if (costhetastar_.size()>0) { costhetastar = costhetastar_[0];                 } else { costhetastar = -2.; }
}

}
