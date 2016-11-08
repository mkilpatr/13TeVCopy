/*
 * ResolvedTopMVA.h
 *
 *  Created on: Oct 6, 2016
 *      Author: hqu
 */

#ifndef ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_RESOLVEDTOPMVA_H_
#define ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_RESOLVEDTOPMVA_H_

#include "AnalysisTools/DataFormats/interface/Jet.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisBase/TreeAnalyzer/interface/TMVAReader.h"


namespace ucsbsusy {

class TopCand{
public:
  TopCand() {}
  TopCand(const RecoJetF *inB, const RecoJetF *inJ2, const RecoJetF *inJ3) : b(inB) {
    if (inJ2->pt()>inJ3->pt()){
      j2 = inJ2; j3 = inJ3;
    }else {
      j2 = inJ3; j3 = inJ2;
    }
    wcand = j2->p4() + j3->p4();
    topcand = b->p4() + wcand.p4();
  }

  bool passMassW(double range=40)   const { return std::abs(wcand.mass()-80)    <= range; }
  bool passMassTop(double range=80) const { return std::abs(topcand.mass()-175) <= range; }

  friend std::ostream &operator<<(std::ostream &os, const TopCand &c){
    os << "top (disc=" << c.disc << "): " << c.topcand << std::endl
        << " -- b : " << c.b->p4() << "csv = " << c.b->csv() << std::endl
        << " -- j2: " << c.j2->p4() << std::endl
        << " -- j3: " << c.j3->p4() << std::endl;
    return os;
  }

  bool sameAs(const TopCand &c) const {
    return b==c.b && j2==c.j2 && j3==c.j3;
  }

  bool overlaps(const TopCand &c) const {
    return b ==c.b || b ==c.j2 || b ==c.j3
        || j2==c.b || j2==c.j2 || j2==c.j3
        || j3==c.b || j3==c.j2 || j3==c.j3;
  }

public:
  const RecoJetF *b  = nullptr;
  const RecoJetF *j2 = nullptr;
  const RecoJetF *j3 = nullptr;
  MomentumF wcand, topcand;

  double disc = -9;

};

class ResolvedTopMVA {
public:
  ResolvedTopMVA(TString weightfile, TString mvaname);
  virtual ~ResolvedTopMVA();

  std::vector<TopCand> getTopCandidates(const std::vector<RecoJetF*> &jets, double WP=WP_TIGHT);

private:
  void initTopMVA();
  std::map<TString, float> calcTopCandVars(const TopCand *topcand);
  std::vector<TopCand> removeOverlap(std::vector<TopCand> &cands, double threshold);

public:
  static constexpr double WP_LOOSE  = 0.83;
  static constexpr double WP_MEDIUM = 0.98;
  static constexpr double WP_TIGHT  = 0.99;

private:
  TMVAReader mvaReader;
  std::vector<TString> varsF;
  std::vector<TString> varsI;

};

} /* namespace ucsbsusy */

#endif /* ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_RESOLVEDTOPMVA_H_ */
