#ifndef AnalysisTools_JetShapeVariables_QuarkGluonTaggingVariables_h
#define AnalysisTools_JetShapeVariables_QuarkGluonTaggingVariables_h

#include <string>
#include <assert.h>
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "TF1.h"
#include "TGraph.h"

#include "DataFormats/JetReco/interface/Jet.h"

namespace ucsbsusy{

class QuarkGluonTaggingVariables {

 public:

  QuarkGluonTaggingVariables():
    axis1_    (0),
    axis2_    (0),
    ptD_      (0),
    totalMult_(0),
    blf0_     (0),
    blf1_     (0),
    blf2_     (0)
  {};
   ~QuarkGluonTaggingVariables();


   void compute(const reco::Jet * jet){
	 std::cout << "computing qgl vars" << std::endl;

	 float sum_weight    = 0.0;
	 float sum_dEta      = 0.0;
	 float sum_dPhi      = 0.0;
	 float sum_dEta2     = 0.0;
	 float sum_dPhi2     = 0.0;
	 float sum_dEta_dPhi = 0.0;
	 float sum_pt        = 0.0;
	 bool useQC          = false; // works for now, still need to look into...
	 totalMult_          = 0;

	 // loop over the jet constituents
	 // (packed candidate situation)
	 std::cout << "...begin part loop" << std::endl;
	 int temp = 0;
	 for(auto part : jet->getJetConstituentsQuick()) {
	   std::cout << "...... " << temp << std::endl; ++temp;
       auto p = static_cast<const pat::PackedCandidate*>(part);
       if(p->charge()){ // charged particles
       	 if(!( p->fromPV() > 1 && p->trackHighPurity() )) continue;
       	 if(useQC) {
           if( (p->dz()*p->dz() ) / (p->dzError()*p->dzError() ) > 25. ) continue;
           if( (p->dxy()*p->dxy()) / (p->dxyError()*p->dxyError()) < 25. ) ++totalMult_; // this cut only applies to multiplicity
       	 } else ++totalMult_;
       } else { // neutral particles
       	 if(p->pt() < 1.0) continue;
       	 ++totalMult_;
       } // charged, neutral particles

       float dEta   = part->eta() - jet->eta();
       float dPhi   = reco::deltaPhi(part->phi(), jet->phi());
       float partPt = part->pt();
       float weight = partPt*partPt;

       sum_weight    += weight;
       sum_pt        += partPt;
       sum_dEta      += dEta      * weight;
       sum_dPhi      += dPhi      * weight;
       sum_dEta2     += dEta*dEta * weight;
       sum_dEta_dPhi += dEta*dPhi * weight;
       sum_dPhi2     += dPhi*dPhi * weight;
	 } // jet->getJetConstituentsQuick()
	 std::cout << "...done with loop" << std::endl;

	 // calculate axis2 and ptD
	 float a = 0.0;
     float b = 0.0;
     float c = 0.0;
	 float ave_dEta  = 0.0;
	 float ave_dPhi  = 0.0;
	 float ave_dEta2 = 0.0;
	 float ave_dPhi2 = 0.0;

	 if(sum_weight > 0){
	   ptD_ = sqrt(sum_weight)/sum_pt;
	   ave_dEta  = sum_dEta  / sum_weight;
	   ave_dPhi  = sum_dPhi  / sum_weight;
	   ave_dEta2 = sum_dEta2 / sum_weight;
	   ave_dPhi2 = sum_dPhi2 / sum_weight;
	   a = ave_dEta2 - ave_dEta*ave_dEta;
	   b = ave_dPhi2 - ave_dPhi*ave_dPhi;
	   c = -(sum_dEta_dPhi/sum_weight - ave_dEta*ave_dPhi);
	 } else ptD_ = 0;

	 float delta = sqrt(fabs( (a-b)*(a-b) + 4*c*c ));
	 if(a+b-delta > 0) axis2_ = sqrt(0.5*(a+b-delta));
	 else              axis2_ = 0.0;
	 std::cout << "done computing qgl vars" << std::endl;
   }


   void computeOld(const reco::Jet * jet, edm::Handle<reco::VertexCollection> vtxs){

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

   void computeBLF(const reco::Jet * jet) {

	 if (totalMult_ == 0) totalMult_ = jet->numberOfDaughters();
     blf0_  = -99;
     blf1_  = -99;
     blf2_  = -99;
     if(totalMult_ == 0) return;

	 // get BLF grid counts
	 TH1I *Nhist = new TH1I("Nhist", "Nhist", 15, 2, 17);
	 for (int d=2; d<=16; ++d) {
		 // create the grid
		 vector<vector<bool>> grid( d, vector<bool>(d,false) );
		 double di = 0.8/(1.0*d); // grid spacing = 2*jetRadius/gridDimens

		 // loop over each particle in the jet
		 for(int iD = 0; iD < totalMult_; ++iD){
		   const reco::Candidate * daughter = jet->daughter(iD);
		   if (daughter->pt() < 0.1) continue;
		   // get eta,phi for particle make sure they're within the jet radius
		   double Eta = PhysicsUtilities::deltaEta(*daughter,*jet);
		   double Phi = PhysicsUtilities::deltaPhi(*daughter,*jet);
		   if ( Eta*Eta +Phi*Phi >= 0.16 ) continue;
		   // set coord system to a corner of the "jet square"
		   Eta += 0.4;
		   Phi += 0.4;
		   // find et,phi coords on grid
		   int nEta = Eta/di;
		   int nPhi = Phi/di;
		   grid[nEta][nPhi] = true;
		 } // iD
		 // count how many cells have hits
		 for(int i=0; i<d; ++i) for(int j=0; j<d; ++j) if(grid[i][j]) Nhist->Fill(d);

	 } // d

	 Nhist->Sumw2();
	 TGraph *logGraph = new TGraph(Nhist->GetNbinsX());

	 bool printBins = false;
	 if (printBins) std::cout << " mult=" << totalMult_ << " ";
	 for (int i=0; i<Nhist->GetNbinsX(); ++i) {
		 logGraph->SetPoint( i, log10((1.0*(i+2))/0.8), log10(1.0*Nhist->GetBinContent(i+1)) );
		 if (printBins) std::cout << Nhist->GetBinContent(i+1) << " ";
	 } // NBinsX
	 if (printBins) std::cout << std::endl;

	 //std::cout << "\n\n=== fitting logGraph ===" << std::endl;
	 TF1 *logFit = new TF1( "logFit", "[0] + [1]*x + [2]*log10(x)", 0.1, 1.5 );
	 logFit->SetParameters(0.8, 0.1, 0.2);
	 logFit->SetParName(0,"p0");
	 logFit->SetParName(1,"p1");
	 logFit->SetParName(2,"p2");
	 logGraph->Fit(logFit,"Q"); // Q: quiet fit, V: verbose

	 blf0_ = logFit->GetParameter(0);
	 blf1_ = logFit->GetParameter(1);
	 blf2_ = logFit->GetParameter(2);

	 delete Nhist;
	 delete logGraph;
	 delete logFit;

   } // computeBLF()

   double getAxis1() const {return axis1_;}
   double getAxis2() const {return axis2_;}
   double getPtD() const {return ptD_;}
   int    getTotalMult() const {return totalMult_;}
   double getBlf0()  const {return blf0_;}
   double getBlf1()  const {return blf1_;}
   double getBlf2()  const {return blf2_;}

   template<typename Jet>
   double getBetaStar(const Jet * jet, const reco::VertexCollection & verticies, const int primVertIndex) const {
     if(verticies.size() == 0) return -10;
     int nConstituents = jet->numberOfDaughters();
     double betaStar = 0;
     double totalPT = 0;

     for(int iD = 0; iD < nConstituents; ++iD){
         const pat::PackedCandidate* part = dynamic_cast<const pat::PackedCandidate*>(jet->daughter(iD));
         if(part == 0) return -10;
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
    
    template<typename Jet>
    double getJetCharge(const Jet * jet){
        int nConstituents = jet->numberOfDaughters();
        if(jet->pt() == 0) return -10;
        double charge = 0;
        for(int iD = 0; iD < nConstituents; ++iD){
            const auto * con =  jet->daughter(iD);
            if(con->charge() == 0) continue;
            charge += float(con->charge())*con->pt();
        }
        return charge/jet->pt();
    }
    
    template<typename Jet>
    void getPull(const Jet * jet, double& pullRap, double& pullPhi){
        pullRap = 0;
        pullPhi = 0;
        double jetRap = jet->rapidity();
        double jetPhi = jet->phi();
        
        if(jet->pt() == 0) return;
        
        int nConstituents = jet->numberOfDaughters();
        for(int iD = 0; iD < nConstituents; ++iD){
            const auto * con =  jet->daughter(iD);
            const double dRap = con->rapidity() - jetRap ;
            const double dPhi = PhysicsUtilities::deltaPhi(con->phi(), jetPhi);
            const double dR = TMath::Sqrt(dRap*dRap + dPhi*dPhi);
            pullRap += dRap * con->pt() * dR;
            pullPhi += dPhi * con->pt() * dR;
        }
        pullRap /= jet->pt();
        pullPhi /= jet->pt();
        
    }
    


 private:
  double axis1_    ;
  double axis2_    ;
  double ptD_      ;
  int    totalMult_;
  double blf0_     ;
  double blf1_     ;
  double blf2_     ;


};

}


#endif
