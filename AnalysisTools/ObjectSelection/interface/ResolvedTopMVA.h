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
#include "AnalysisTools/Utilities/interface/PartonMatching.h"

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

  // max number of subjets of one hadronic gen top which match j1,j2,j3
  static int nMatchedSubjets(const std::vector<PartonMatching::TopDecay*> &tops, const RecoJetF* j1, const RecoJetF* j2, const RecoJetF* j3, float matchdr = 0.4, float matchpt = 1e6) {
    //if( j1 == 0 || j2 == 0 || j3 == 0) return -1;
    if(matchpt < 0 || matchdr < 0) return -1;

    //check if a single jet can be matched to a single parton
    auto jetMatchesParton = [&](const GenParticleF* part, const RecoJetF* jet)->bool {
      if(!part || !jet) return false;
      return ( (PhysicsUtilities::deltaR(*part,*jet) < matchdr) && (abs(jet->pt() - part->pt())/part->pt() < matchpt) );
    };

    //check if a single jet can be matched to a top's three partons
    auto jetMatchesTop = [&](const PartonMatching::TopDecay* top, const RecoJetF* jet)->int {
      if(!jet || !top) return -1; // no match
      if(jetMatchesParton(top->b->parton,      jet)) return 0; // return unique ID for the parton to which it matches
      if(jetMatchesParton(top->W_dau1->parton, jet)) return 1;
      if(jetMatchesParton(top->W_dau2->parton, jet)) return 2;
      return -1; // no match
    };

    int maxmatchedsubjets = 0;
    for(const auto &top : tops){
      int matchedsubjets = 0;
      std::vector<int> matchcodes;
      int matchj1 = jetMatchesTop(top,j1);
      int matchj2 = jetMatchesTop(top,j2);
      int matchj3 = jetMatchesTop(top,j3);
      if(matchj1 > -1) matchcodes.push_back(matchj1);
      if(matchj2 > -1) matchcodes.push_back(matchj2);
      if(matchj3 > -1) matchcodes.push_back(matchj3);
      std::set<int> s( matchcodes.begin(), matchcodes.end() ); // trick to remove duplicates (two reco jets matching same parton)
      matchcodes.assign( s.begin(), s.end() );
      matchedsubjets = matchcodes.size();
      maxmatchedsubjets = std::max(maxmatchedsubjets, matchedsubjets);
    }
    return maxmatchedsubjets;
  }


  int nMatchedSubjets(const std::vector<PartonMatching::TopDecay*> &tops) const {
    return nMatchedSubjets(tops, b, j2, j3);
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
  static constexpr double WP_ALL    = -1.0; // used for candidate studies
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
