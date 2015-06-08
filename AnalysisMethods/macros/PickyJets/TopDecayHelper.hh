#ifndef TOPDECAYHELPER_HH
#define TOPDECAYHELPER_HH

#include "TString.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/DataFormats/interface/Momentum.h"
#include "AnalysisTools/Utilities/interface/Types.h"

using namespace ucsbsusy;

namespace topdecays {

  enum SplitDecision {NOSPLIT_NO_PARENTS, NOSPLIT_BELOW_MCUT, NOSPLIT_FAIL_PTCUT, NOSPLIT_BELOW_DRMIN, SPLIT_BOTH_SUBJETS, SPLIT_LEADING_SUBJET, EMPTY};

  static double minpt_           = 20.0;
  static double maxeta_          = 2.4;
  static double ptcut            = 20.0;
  static double minpartonrele    = 0.15;
  static double minsigpartonrele = 0.5;
  static double minsigjetrele    = 0.5;
  static double minjetretainment = 0.9;

  const char* printSplitDecision(SplitDecision decision) {
    switch(decision) {
      case NOSPLIT_NO_PARENTS   : { return "no subjets, nothing to split"; break; }
      case NOSPLIT_BELOW_MCUT   : { return "fails mcut, won't split"; break; }
      case NOSPLIT_FAIL_PTCUT   : { return "subjets fail ptcut"; break; }
      case NOSPLIT_BELOW_DRMIN  : { return "subjets fail drcut, won't split"; break; }
      case SPLIT_BOTH_SUBJETS   : { return "both subjets will continue"; break; }
      case SPLIT_LEADING_SUBJET : { return "leading subjet will continue"; break; }
      default                   : { return "empty"; break; }
    }
  }

  struct AssocParton {
    public :
      enum Diagnosis {OUT_OF_RANGE, NO_JET, SPLIT, MERGED, CONTAMINATED, RESOLVED, EMPTY};

      AssocParton() : pt(0), eta(0), phi(0), energy(0), topindex(-1), diag(EMPTY) {}
      AssocParton(float inpt_, float ineta_, float inphi_, float inenergy_, int inindex_) :
        pt(inpt_), eta(ineta_), phi(inphi_), energy(inenergy_), topindex(inindex_), diag(EMPTY) {}
      AssocParton(const AssocParton& other)
      {
        pt          = other.pt;
        eta         = other.eta;
        phi         = other.phi;
        energy      = other.energy;
        topindex    = other.topindex;
        containment = other.containment;
        diag        = other.diag;
      }

      ~AssocParton() {}

      void setContainment(int jetindex, float jetcon) { containment.emplace_back(jetcon, jetindex); sort(containment.begin(), containment.end(), PhysicsUtilities::greaterFirst<float,int>()); }
      void setDiagnosis(Diagnosis indiag) { diag = indiag; }

      const char* printDiagnosis() const {
        switch(diag) {
          case OUT_OF_RANGE : { return "out of range"; break; }
          case NO_JET       : { return "no jet"; break;       }
          case SPLIT        : { return "split"; break;        }
          case MERGED       : { return "merged"; break;       }
          case CONTAMINATED : { return "contaminated"; break; }
          case RESOLVED     : { return "resolved"; break;     }
          default           : { return "empty";               }
        }
      }

      float pt;
      float eta;
      float phi;
      float energy;
      int   topindex;
      vector<pair<float,int>> containment;
      Diagnosis diag;

  };

  struct TopPartonAssoc {
    public :
      enum TopDiagnosis {BAD_PARTON, MISSING_JET, SPLIT_PARTON, MERGED_PARTONS, CONTAMINATED, RESOLVED, EMPTY};
      TopPartonAssoc(float inpt_, float ineta_, float inphi_, float inmass_) :
        pt(inpt_),
        eta(ineta_),
        phi(inphi_),
        mass(inmass_),
        diag(EMPTY)
      {
        partons.reserve(3);
      }

      ~TopPartonAssoc() {}

      TopPartonAssoc(const TopPartonAssoc& other) {
        pt      = other.pt;
        eta     = other.eta;
        phi     = other.phi;
        mass    = other.mass;
        partons = other.partons;
        diag    = other.diag;
      }

      TopPartonAssoc& operator=(const TopPartonAssoc& other) {
        pt      = other.pt;
        eta     = other.eta;
        phi     = other.phi;
        mass    = other.mass;
        partons = other.partons;
        diag    = other.diag;
        return *this;
      }

      void swap(TopPartonAssoc& other) {
        TopPartonAssoc tmp(*this);
        *this = other;
        other = tmp;
      }

      void addParton(const AssocParton* parton) { partons.push_back(parton); }
      void diagnose() {
        int nbadpartons = 0, nmissjets = 0, nsplits = 0, nmerged = 0, ncontaminated = 0;
        for(const auto* p: partons) {
          if(p->diag == AssocParton::OUT_OF_RANGE || p->diag == AssocParton::EMPTY) nbadpartons++;
          else if(p->diag == AssocParton::NO_JET) nmissjets++;
          else if(p->diag == AssocParton::SPLIT) nsplits++;
          else if(p->diag == AssocParton::MERGED) nmerged++;
          else if(p->diag == AssocParton::CONTAMINATED) ncontaminated++;
        }
        if(nbadpartons)   { diag = BAD_PARTON; return;     }
        if(nmissjets)     { diag = MISSING_JET; return;    }
        if(nsplits)       { diag = SPLIT_PARTON; return;   }
        if(nmerged)       { diag = MERGED_PARTONS; return; }
        if(ncontaminated) { diag = CONTAMINATED; return;   }
        diag = RESOLVED;
        return;
      }
      const char* printDiagnosis() {
        switch(diag) {
          case BAD_PARTON     : { return "bad parton(s)"; break;       }
          case MISSING_JET    : { return "missing jet(s)"; break;      }
          case SPLIT_PARTON   : { return "split parton(s)"; break;     }
          case MERGED_PARTONS : { return "merged partons"; break;      }
          case CONTAMINATED   : { return "contaminated jet(s)"; break; }
          case RESOLVED       : { return "resolved!"; break;           }
          default             : { return "empty";                      }
        }
      }

      float pt;
      float eta;
      float phi;
      float mass;
      vector<const AssocParton*> partons;
      TopDiagnosis diag;

  };

  struct AssocJet {
    public :
      AssocJet(float inpt_, float ineta_, float inphi_, float inmass_, float ingene_, int innsplits_=0) : 
        mom(CylLorentzVectorF(inpt_, ineta_, inphi_, inmass_)), fatJetMom(CylLorentzVectorF(0,0,0,0)), genE(ingene_), fatJetGenE(0), nsplits(innsplits_)
       {
         purity.resize(0);
         fatJetPurity.resize(0);
         partons.resize(0);
       }

      AssocJet(const AssocJet& otherjet)
      {
        mom          = otherjet.mom;
        fatJetMom    = otherjet.fatJetMom;
        genE         = otherjet.genE;
        fatJetGenE   = otherjet.fatJetGenE;
        nsplits      = otherjet.nsplits;
        purity       = otherjet.purity;
        fatJetPurity = otherjet.fatJetPurity;
        partons      = otherjet.partons;
      }

      ~AssocJet() {}

      float pt()   const { return mom.pt();   }
      float eta()  const { return mom.eta();  }
      float phi()  const { return mom.phi();  }
      float mass() const { return mom.mass(); }
      float fatjetpt()   const { return fatJetMom.pt();   }
      float fatjeteta()  const { return fatJetMom.eta();  }
      float fatjetphi()  const { return fatJetMom.phi();  }
      float fatjetmass() const { return fatJetMom.mass(); }

      bool  isEmpty()  const { return purity.size() == 0; }
      bool  isSingle() const { return purity.size() == 1; }
      bool  isMulti()  const { return purity.size() > 1;  }

      void  attachPartons   (const vector<AssocParton*>& inpartons) { partons = inpartons; }
      void  setMom          (float pt, float eta, float phi, float mass)      { mom.setP4(CylLorentzVectorF(pt, eta, phi, mass)); }
      void  setGenE         (float ingene)                                    { genE = ingene; }
      void  setNSplits      (int innsplits)                                   { nsplits = innsplits; }
      void  setPurity       (int index, float inpurity)                       { purity.emplace_back(inpurity, index); sort(purity.begin(), purity.end(), PhysicsUtilities::greaterFirst<float,int>()); }
      void  setPurity       (const vector<pair<float,int>>& inpurity)         { purity = inpurity; }
      void  setFatJetMom    (float pt, float eta, float phi, float mass)      { fatJetMom.setP4(CylLorentzVectorF(pt, eta, phi, mass)); }
      void  setFatJetGenE   (float ingene)                                    { fatJetGenE = ingene; }
      void  setFatJetPurity (const vector<pair<float,int>>& inpurity)         { fatJetPurity = inpurity; }
      void  setFatJet       (const AssocJet* infatjet)                        { fatJetMom = infatjet->mom; fatJetGenE = infatjet->genE, fatJetPurity = infatjet->purity; }
      float getPurity       (int index)                                       { for(auto p : purity) { if(p.second == index) return p.first; } return 0; }

      string print() {
        string result;
        char text[50];
        sprintf(text, "%4.2f, %4.2f, %4.2f, %4.2f ", pt(), eta(), phi(), mass());
        result.append(text);
        string puritystr = printPurity();
        result.append(puritystr);
        return result;
      }

      string printPurity() {
        string result("parton contents: ");
        char tmpstr[20];
        for(auto p : purity) {
          sprintf(tmpstr, "%d, %4.2f (%4.2f of %4.2f)  ", p.second, p.first, p.first*genE, partons[p.second]->energy);
          result.append(tmpstr);
        }
        return result;
      }

      MomentumF mom;
      MomentumF fatJetMom;
      float     genE;
      float     fatJetGenE;
      int       nsplits;
      vector<pair<float,int>> purity;
      vector<pair<float,int>> fatJetPurity;
      vector<AssocParton*>    partons;

  };

  struct CachedEventInfo {
    public : 
      CachedEventInfo() : run(0), lumi(0), event(0), mcut(0), rmin(0), ycut(0), savedNSplits(0), savedSplitDecision(EMPTY), isfilled(false) {}
      CachedEventInfo(int inrun, int inlumi, int inevent) : run(inrun), lumi(inlumi), event(inevent), mcut(0), rmin(0), ycut(0), savedNSplits(0), savedSplitDecision(EMPTY), isfilled(false) {}
      CachedEventInfo(int inrun, int inlumi, int inevent, double inmcut, double inrmin, double inycut) : run(inrun), lumi(inlumi), event(inevent), mcut(inmcut), rmin(inrmin), ycut(inycut), savedNSplits(0), savedSplitDecision(EMPTY), isfilled(false) {}

      ~CachedEventInfo() {}

      bool sameEvent(int newrun, int newlumi, int newevent) { return (run == newrun && lumi == newlumi && event == newevent); }
      void setEvent (int newrun, int newlumi, int newevent) { run = newrun; lumi = newlumi; event = newevent; }
      void setCuts  (double newmcut, double newrmin, double newycut) { mcut = newmcut; rmin = newrmin; ycut = newycut; }
      void  addPartons(const vector<AssocParton*>& inpartons) { partons.clear(); for(auto* p : inpartons) partons.push_back(new AssocParton(*p)); }
      void  addTops   (const vector<TopPartonAssoc*>& intops) { tops.clear(); for(auto* t : intops) tops.push_back(new TopPartonAssoc(*t)); }

      std::string getLabel() {
        char label[50];

        if(mcut < 10.0)
          sprintf(label, "cuts_mcut%1.0f_rmin%4.2f_ycut%4.2f",mcut,rmin,ycut);
        else 
          sprintf(label, "cuts_mcut%2.0f_rmin%4.2f_ycut%4.2f",mcut,rmin,ycut);

        return string(label);
      }

      void reset() {
        isfilled = false;
        run = 0; lumi = 0; event = 0;
        savedNSplits = 0; savedSplitDecision = EMPTY;
        savedReentryPoints.clear();
        partons.clear(); tops.clear(); fatjets.clear(); finaljets.clear();
      }

      string printReentries() {
        string result;
        char tmpstr[2];
        for(auto i : savedReentryPoints) {
          sprintf(tmpstr, "%d ",i);
          result.append(tmpstr);
        }
        return result;
      }

      void print() {
        printf("Run: %d, Lumi: %d, Event: %d\n",run,lumi,event);
        printf("Cuts: mcut=%4.0f, rmin=%4.2f, ycut=%4.2f\n",mcut,rmin,ycut);
        printf("Tops:\n");
        for(auto* t : tops) {
          printf("\ttop pt, eta, phi, mass: %4.2f, %4.2f, %4.2f, %4.2f: \n", t->pt, t->eta, t->phi, t->mass);
          for(auto* p : t->partons)
            printf("\t\tparton pt, eta, phi, energy: %4.2f, %4.2f, %4.2f, %4.2f, diagnosis: %s\n", p->pt, p->eta, p->phi, p->energy, p->printDiagnosis());
          printf("\tdiagnosis: %s\n", t->printDiagnosis());
        }
        printf("Jets:\n");
        for(auto* jet : finaljets) {
          printf("\tfat jet 4vec: %4.2f, %4.2f, %4.2f, %4.2f\n", jet->fatjetpt(), jet->fatjeteta(), jet->fatjetphi(), jet->fatjetmass());
          for(auto p : jet->fatJetPurity)
            printf("\t\tparton %d (top %d): purity %4.2f\n",p.second,partons[p.second]->topindex+1,p.first);
          printf("\tjet 4vec: %4.2f, %4.2f, %4.2f, %4.2f, genE: %4.2f, nsplits: %d\n", jet->pt(), jet->eta(), jet->phi(), jet->mass(), jet->genE, jet->nsplits);
          for(auto p : jet->purity)
            printf("\t\tparton %d : purity %4.2f\n",p.second,p.first);
        }
      }

      void finalizeDiagnosis() {
        for(unsigned int ip = 0; ip < partons.size(); ++ip) {
          auto* p = partons.at(ip);
          if(p->pt < minpt_ || fabs(p->eta) > maxeta_)
            p->setDiagnosis(AssocParton::OUT_OF_RANGE);
          else {
            int nJetsWithSigContribution = 0;
            AssocJet* mainJet = 0;
            for(unsigned int ij = 0; ij < finaljets.size(); ++ij) {
              auto* jet = finaljets.at(ij);
              for(auto jp : jet->purity) {
                if(jp.second == int(ip) && jet->pt() > minpt_ && fabs(jet->eta()) < maxeta_) {
                  p->setContainment(ij, jp.first);
                  if(jp.first*jet->genE > minsigpartonrele*p->energy) {
                    if(!mainJet || jp.first > mainJet->getPurity(ip))
                      mainJet = jet;
                  }
                  if(jp.first*jet->genE > minpartonrele*p->energy)
                    nJetsWithSigContribution++;
                }
              }
            }
            if(!mainJet) // no jet with at least 50% of the parton energy
              p->setDiagnosis(AssocParton::NO_JET);
            else if(nJetsWithSigContribution>1) // additional jets with at least 15% of the parton energy
              p->setDiagnosis(AssocParton::SPLIT);
            else if(p->containment.size() && p->containment[0].first < minsigjetrele) // energy from parton is less than 50% of jet energy
              p->setDiagnosis(AssocParton::CONTAMINATED);
            else // it's resolved
              p->setDiagnosis(AssocParton::RESOLVED);
          }
        }
        for(unsigned int ipa = 0; ipa < partons.size(); ++ipa) {
          auto* pa = partons.at(ipa);
          if(pa->containment.size()) {
            for(unsigned int ipb = 0; ipb < partons.size(); ++ipb) {
              if(ipa == ipb) continue;
              auto* pb = partons.at(ipb);
              if(!pb->containment.size()) continue;
              if(pa->containment[0].second == pb->containment[0].second && pa->diag > AssocParton::SPLIT) // multiple partons in jet
                pa->setDiagnosis(AssocParton::MERGED);
            }
          }
          if(pa->topindex == 0) tops.at(0)->addParton(pa);
          else if(pa->topindex == 1) tops.at(1)->addParton(pa);
        }
        if(tops.at(0)->pt < tops.at(1)->pt)
          std::swap(tops.at(0), tops.at(1));
        for(auto* top : tops)
          top->diagnose();
      }

      int    run;
      int    lumi;
      int    event;
      double mcut;
      double rmin;
      double ycut;
      vector<AssocParton*>    partons;
      vector<TopPartonAssoc*> tops;
      vector<AssocJet*>       fatjets;
      vector<AssocJet*>       finaljets;
      size8       savedNSplits;
      size8       savedSplitDecision;
      vector<int> savedReentryPoints;
      bool        isfilled;

  };

}

#endif
