#ifndef AnalysisTools_JetShapeVariables_QuarkGluonTaggingVariables_h
#define AnalysisTools_JetShapeVariables_QuarkGluonTaggingVariables_h

#include <string>
#include <assert.h>
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"


namespace ucsbsusy{

class QuarkGluonTaggingVariables {

 public:

  QuarkGluonTaggingVariables():
    axis1_    (0),
    axis2_    (0),
    ptD_      (0),
    totalMult_(0)
  {};
   ~QuarkGluonTaggingVariables();


   void compute(const reco::Jet * jet){

     totalMult_ = jet->numberOfDaughters();
     ptD_ = 0;
     axis1_ = 0;
     axis2_ = 0;
     if(totalMult_ == 0) return;

     double sumPT2 = 0;
     double sumPT  = 0;

     double m11 = 0;
     double m22 = 0;
     double m12 = 0;
     for(int iD = 0; iD < totalMult_; ++iD){
       const reco::Candidate * daughter = jet->daughter(iD);
       double pT = daughter->pt();
       sumPT += pT;
       sumPT2 += pT*pT;
       double dEta = PhysicsUtilities::deltaEta(*daughter,*jet);
       double dPhi = PhysicsUtilities::deltaPhi(*daughter,*jet);

       m11 += pT*pT*dEta*dEta;
       m22 += pT*pT*dPhi*dPhi;
       m12 += pT*pT*dPhi*dEta;
     }

     const double lp = TMath::Sqrt(4*m12*m12 + (m11 - m22)*(m11 - m22) );
     const double l1 = ((m11 + m22) + lp)/2;
     const double l2 = ((m11 + m22) - lp)/2;




     axis1_ = TMath::Sqrt((l1 < 0 ? 0 : l1) / sumPT2);
     axis2_ = TMath::Sqrt((l2 < 0 ? 0 : l2) / sumPT2);
     ptD_ = TMath::Sqrt(sumPT2)/sumPT;
   }

   double getAxis1() const {return axis1_;}
   double getAxis2() const {return axis2_;}
   double getPtD() const {return ptD_;}
   int    getTotalMult() const {return totalMult_;}

   template<typename Jet>
   double getBetaStar(const Jet * jet, const reco::VertexCollection & verticies, const int primVertIndex) const {
     if(verticies.size() == 0) return 10;
     int nConstituents = jet->numberOfDaughters();
     double betaStar = 0;
     double totalPT = 0;

     for(int iD = 0; iD < nConstituents; ++iD){
       const pat::PackedCandidate* part = (pat::PackedCandidate*)jet->daughter(iD);
//       if(part->pt() < 1) continue;
       if(part->charge() == 0) continue;
       double pt = part->pt();


       totalPT += pt;
       reco::PFCandidate d;
       double dZ0 = fabs(part->dz(verticies[primVertIndex].position()));
       double dZ  = dZ0;

       for(unsigned int iV = 1; iV < verticies.size(); ++iV){
         const auto& iv = verticies[iV];
         if( iv.isFake() || iv.ndof() < 4 ) { continue; }
         dZ = std::min(dZ,double(fabs(part->dz(iv.position()))));
       }

       if(dZ0 >= .2 && dZ < .2){
         betaStar += pt;
       }
     }
     if(totalPT == 0) return 10;
     return betaStar / totalPT;
   }

 private:
  double axis1_    ;
  double axis2_    ;
  double ptD_      ;
  int    totalMult_;


};

}


#endif
