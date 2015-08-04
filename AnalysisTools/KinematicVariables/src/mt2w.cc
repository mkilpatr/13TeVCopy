#include "../interface/mt2w.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h" // CSV_MEDIUM

using namespace std;


double round(double num, int x){
return ceil( ( num * pow( 10,x ) ) - 0.5 ) / pow( 10,x );
}

// 
//--------------------------------------------------------------------
double calculateMT2w(vector<LorentzVector>& jets, vector<float>& bvalue, LorentzVector& lep, float met, float metphi){

    // I am asumming that jets is sorted by Pt
    assert ( jets.size() == bvalue.size() );

    // don't call without at least two jets!
    if(jets.size()<2) return -9.;
  
    vector<pair<double,int> > rankedJets(jets.size());
    for(unsigned int iJ =0; iJ < jets.size(); ++iJ){
      rankedJets[iJ].first = bvalue[iJ];
      rankedJets[iJ].second = iJ;
    }
    std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());
    vector<int> bjets;
    vector<int> addjets;

    for(unsigned int iJ =0; iJ < rankedJets.size(); ++iJ){
      if(rankedJets[iJ].first>defaults::CSV_MEDIUM) bjets.push_back(rankedJets[iJ].second);
      else {
	if (bjets.size()<2 && bjets.size()+addjets.size()<3) addjets.push_back(rankedJets[iJ].second);
      }
    }
    float min_mt2w = 9999;

    if(bjets.size()>0){
      for(unsigned int n = 0; n<bjets.size(); ++n){
	for(unsigned int m = n+1; m<bjets.size(); ++m){
	  float c_mt2w = mt2wWrapper(lep, jets[bjets[n]], jets[bjets[m]], met, metphi);
	  if (c_mt2w < min_mt2w) min_mt2w = c_mt2w;
	  c_mt2w = mt2wWrapper(lep, jets[bjets[m]], jets[bjets[n]], met, metphi);
          if (c_mt2w < min_mt2w) min_mt2w = c_mt2w;
	}
	for(unsigned int m = 0; m<addjets.size(); ++m){
          float c_mt2w = mt2wWrapper(lep, jets[bjets[n]], jets[addjets[m]], met, metphi);
          if (c_mt2w < min_mt2w) min_mt2w = c_mt2w;
          c_mt2w = mt2wWrapper(lep, jets[addjets[m]], jets[bjets[n]], met, metphi);
          if (c_mt2w < min_mt2w) min_mt2w = c_mt2w;
	}
      }
    }
    else{
      for(unsigned int m = 1; m<addjets.size(); ++m){
	float c_mt2w = mt2wWrapper(lep, jets[addjets[0]], jets[addjets[m]], met, metphi);
	if (c_mt2w < min_mt2w) min_mt2w = c_mt2w;
	c_mt2w = mt2wWrapper(lep, jets[addjets[m]], jets[addjets[0]], met, metphi);
	if (c_mt2w < min_mt2w) min_mt2w = c_mt2w;
      }
    }

    return min_mt2w;
}

// This funcion is a wrapper for mt2w_bisect etc that takes LorentzVectors instead of doubles
double mt2wWrapper(LorentzVector& lep, LorentzVector& jet_o, LorentzVector& jet_b, float met, float metphi){

    // same for all MT2x variables
    float metx = met * cos( metphi );
    float mety = met * sin( metphi );

    double pl[4];     // Visible lepton
    double pb1[4];    // bottom on the same side as the visible lepton
    double pb2[4];    // other bottom, paired with the invisible W
    double pmiss[3];  // <unused>, pmx, pmy   missing pT

    pl[0]= round(lep.E(),3); pl[1]= round(lep.Px(),3); pl[2]= round(lep.Py(),3); pl[3]= round(lep.Pz(),3);
    pb1[1] = round(jet_o.Px(),3);  pb1[2] = round(jet_o.Py(),3);   pb1[3] = round(jet_o.Pz(),3);
    pb2[1] = round(jet_b.Px(),3);  pb2[2] = round(jet_b.Py(),3);   pb2[3] = round(jet_b.Pz(),3);
    pmiss[0] = round(0.,3); pmiss[1] = round(metx,3) ; pmiss[2] = round(mety,3);

    pb1[0] = round(jet_o.E(),3);
    pb2[0] = round(jet_b.E(),3);

    mt2w_bisect::mt2w mt2w_event;
    mt2w_event.set_momenta(pl, pb1, pb2, pmiss);

    return mt2w_event.get_mt2w();
}

