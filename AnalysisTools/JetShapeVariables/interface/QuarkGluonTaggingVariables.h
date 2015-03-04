#ifndef AnalysisTools_JetShapeVariables_QuarkGluonTaggingVariables_h
#define AnalysisTools_JetShapeVariables_QuarkGluonTaggingVariables_h

#include <string>
#include <vector>
#include <assert.h>
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "TF1.h"
#include "TGraph.h"


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

     totalMult_ = jet->numberOfDaughters();
     ptD_   = 0;
     axis1_ = 0;
     axis2_ = 0;
     blf0_  = 0;
     blf1_  = 0;
     blf2_  = 0;
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
     } // iD < totalMult_

     const double lp = TMath::Sqrt(4*m12*m12 + (m11 - m22)*(m11 - m22) );
     const double l1 = ((m11 + m22) + lp)/2;
     const double l2 = ((m11 + m22) - lp)/2;

     axis1_ = TMath::Sqrt((l1 < 0 ? 0 : l1) / sumPT2);
     axis2_ = TMath::Sqrt((l2 < 0 ? 0 : l2) / sumPT2);
     ptD_   = TMath::Sqrt(sumPT2)/sumPT;

     //*
     // get BLF grid counts
     //vector<int> N(15,0); // to store the counts for each grid size 2,3,4,...,16
     TH1I *Nhist = new TH1I("Nhist", "Nhist", 15, 2, 17);
     for (int d=2; d<=16; ++d) {
    	 // create the grid
		 vector<vector<bool>> grid( d, vector<bool>(d,false) );
		 double di = 0.8/(1.0*d); // grid spacing = 2*jetRadius/gridDimens

		 // loop over each particle in the jet
		 double maxDist = 0.0;
		 double deltaR  = 0.0;
	     for(int iD = 0; iD < totalMult_; ++iD){
	       const reco::Candidate * daughter = jet->daughter(iD);
	       if (daughter->pt() < 0.1) continue;
	       // get eta,phi for particle make sure they're within the jet radius
	       double Eta = PhysicsUtilities::deltaEta(*daughter,*jet);
	       double Phi = PhysicsUtilities::deltaPhi(*daughter,*jet);
	       if (            jet->maxDistance() > maxDist ) maxDist = jet->maxDistance();
	       if ( TMath::Sqrt(Eta*Eta +Phi*Phi) > deltaR  ) deltaR  = TMath::Sqrt(Eta*Eta +Phi*Phi);
	       if ( TMath::Sqrt(Eta*Eta +Phi*Phi) >= 0.4 ) continue;
	       //if (TMath::Abs(Eta)>0.4) continue;
	       //if (TMath::Abs(Phi)>0.4) continue;
	       // set coord system to a corner of the "jet square"
	       Eta += 0.4;
	       Phi += 0.4;
	       // find et,phi coords on grid
	       int nEta = Eta/di;
	       int nPhi = Phi/di;
	       grid[nEta][nPhi] = true;
	     } // iD
	     //if (d==2) std::cout << "===== jet->maxDistance() = " << maxDist << "\t" << deltaR << " \t" << maxDist-deltaR << std::endl;
	     // count how many cells have hits
	     for(int i=0; i<d; ++i) for(int j=0; j<d; ++j) if(grid[i][j]) Nhist->Fill(d);

     } // d

     Nhist->Sumw2();
     TGraph *logGraph = new TGraph(Nhist->GetNbinsX());

     bool printBins = false;
     if (printBins) std::cout << "\n\n=======================================================================\n"
    		                  <<     "=======================================================================\n"
    		                  <<     "=======================================================================\n\n\n"
    		                  << " mult=" << totalMult_ << " ";
     for (int i=0; i<Nhist->GetNbinsX(); ++i) {
    	 logGraph->SetPoint( i, log10((1.0*(i+2))/0.8), log10(1.0*Nhist->GetBinContent(i+1)) );
    	 if (printBins) std::cout << Nhist->GetBinContent(i+1) << " ";
     } // NBinsX
     if (printBins) std::cout << std::endl;// */

     /*for (int i=0; i<=14; ++i) {
	 	 double xx=0; double yy=0;
	 	 logGraph->GetPoint(i,xx,yy);
	 	 std::cout << "   " << i+2 << "\t" << xx << " \t" << yy << std::endl;
     }
     std::cout << std::endl; // */

     /*TH1F *testHist = new TH1F("testHist", "testHist", 15, 1.5, 16.5);
     for (int i=1; i<=Nhist->GetNbinsX(); ++i) testHist->SetBinContent( i, log10(Nhist->GetBinContent(i)) );
     TGraph *testGraph = new TGraph(Nhist->GetNbinsX());
     for (int i=0; i<Nhist->GetNbinsX(); ++i) {
    	 testGraph->SetPoint( i, log10((1.0*(i+2))/0.8), testHist->GetBinContent(i+1) );
     } // NBinsX */

     /*std::cout << std::endl << "compare testHist  to  logGraph  to  testGraph" << std::endl;
     for (int j=0; j<=14; ++j) {
		 double xx=0; double yy=0;
		 logGraph->GetPoint(j,xx,yy);
		 double tx=0; double ty=0;
		 logGraph->GetPoint(j,tx,ty);
		 std::cout << "   " << j+2 << " " //<< log10((j+2)/.8) << " \t" << xx
				   << " \t" << testHist->GetBinContent(j+1)
				   << " \t" << yy
				   << " \t" << ty
				   << std::endl;
     } // */

     //std::cout << "\n\n=== fitting testHist ===" << std::endl;
     //TF1 *testFit = new TF1( "testFit", "[0] + [1]*log10((1.0*(x+1))/0.8) + [2]*log10(log10((1.0*(x+1))/0.8))");
     //TF1 *testFit = new TF1( "testFit", "[0] + [1]*log10((1.0*(x))/0.8) + [2]*log10(log10((1.0*(x))/0.8))");
     //testFit->SetParameters(0.8, 0.1, 0.2);
     //testFit->SetParName(0,"t0");
     //testFit->SetParName(1,"t1");
     //testFit->SetParName(2,"t2");
     //testHist->Fit(testFit,"W"); // Q: quiet fit

     //std::cout << "\n\n=== fitting testGraph ===" << std::endl;
     //TF1 *testFit2 = new TF1( "testFit2", "[0] + [1]*x + [2]*log10(x)");
     //testFit2->SetParameters(0.8, 0.1, 0.2);
     //testFit2->SetParName(0,"t0");
     //testFit2->SetParName(1,"t1");
     //testFit2->SetParName(2,"t2");
     //testGraph->Fit(testFit2); //,"V"); // Q: quiet fit

     //std::cout << "\n\n=== fitting logGraph ===" << std::endl;
     TF1 *logFit = new TF1( "logFit", "[0] + [1]*x + [2]*log10(x)", 0.1, 1.5 );
     logFit->SetParameters(0.8, 0.1, 0.2);
     logFit->SetParName(0,"p0");
     logFit->SetParName(1,"p1");
     logFit->SetParName(2,"p2");
     logGraph->Fit(logFit,"Q"); // Q: quiet fit, V: verbose
     // */
     /*std::cout << "\n\n\nfit parameters: testFit  \ttestFit2 \tlogFit" << std::endl
    		   << "               " << testFit->GetParameter(0) << "  \t" << testFit2->GetParameter(0) << "  \t" << logFit->GetParameter(0) << std::endl
    		   << "               " << testFit->GetParameter(1) << "  \t" << testFit2->GetParameter(1) << "  \t" << logFit->GetParameter(1) << std::endl
    		   << "               " << testFit->GetParameter(2) << "  \t" << testFit2->GetParameter(2) << "  \t" << logFit->GetParameter(2) << std::endl
    		   << std::endl; // */

     blf0_ = logFit->GetParameter(0); //testFit->GetParameter(0);
     blf1_ = logFit->GetParameter(1); //testFit->GetParameter(1);
     blf2_ = logFit->GetParameter(2); //testFit->GetParameter(2);

     delete Nhist;
     delete logGraph;
     delete logFit;

     // for testing
     //delete testHist;
     //delete testFit;
     //delete testGraph;
     //delete testFit2;

   } // compute()

   double getAxis1() const {return axis1_;}
   double getAxis2() const {return axis2_;}
   double getPtD()   const {return ptD_;}
   int    getTotalMult() const {return totalMult_;}
   double getBlf0()  const {return blf0_;}
   double getBlf1()  const {return blf1_;}
   double getBlf2()  const {return blf2_;}


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
       } // iV < verticies.size()

       if(dZ0 >= .2 && dZ < .2){
         betaStar += pt;
       } // dZ0>=.2 && dZ<.2
     } // iD < nConstituents

     if(totalPT == 0) return 10;
     return betaStar / totalPT;
   } // getBetaStar()

 private:
  double axis1_    ;
  double axis2_    ;
  double ptD_      ;
  int    totalMult_;
  double blf0_     ;
  double blf1_     ;
  double blf2_     ;


}; // QuarkGluonTaggingVariables

} // ucsbsusy


#endif
