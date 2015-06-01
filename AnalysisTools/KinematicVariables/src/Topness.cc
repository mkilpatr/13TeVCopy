#include "../interface/Topness.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;


const double Topness::mW  = 81.;
const double Topness::mT  = 172.;
const double Topness::aW  = 5.;
const double Topness::aT  = 15.;
const double Topness::aCM = 1000.;


Topness::Topness() : minimizer(new TFitter(4))
  {
  // less print-outs
    double p1 = -1;
    minimizer->ExecuteCommand("SET PRINTOUT",&p1,1);

    // tell minimizer about the function to be minimized
    minimizer->SetFCN(minuitFunctionWrapper);
  }

Topness::~Topness(){
  delete minimizer;
}

double Topness::topnessFunction(double pwx_, double pwy_, double pwz_, double pnz_,
			       double plx_, double ply_, double plz_, double ple_,
			       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
			       double pb2x_, double pb2y_, double pb2z_, double pb2e_,
			       double pmx_, double pmy_, double pmz_, double pme_, TopnessInformation * info
			       ) {


  // construct the lorentz vectors
  MomentumD vW;  vW.p4().SetPxPyPzE(pwx_,pwy_,pwz_,(sqrt((mW*mW)+(pwx_*pwx_)+(pwy_*pwy_)+(pwz_*pwz_)))) ;

  MomentumD vL;   vL.p4().SetPxPyPzE(plx_,ply_,plz_,ple_);

  MomentumD vB1;  vB1.p4().SetPxPyPzE(pb1x_,pb1y_,pb1z_,pb1e_);

  MomentumD vB2;  vB2.p4().SetPxPyPzE(pb2x_,pb2y_,pb2z_,pb2e_);

  MomentumD vMET; vMET.p4().SetPxPyPzE(pmx_,pmy_,pmz_,pme_);

  MomentumD vN;   vN.p4().SetPxPyPzE((pmx_-pwx_),(pmy_-pwy_),pnz_,(sqrt(pow((pmx_-pwx_),2)+pow((pmy_-pwy_),2)+pow(pnz_,2))));

  MomentumD vW1;  vW1.p4() = vL.p4() + vN.p4();

  // construct the w-term
  double tW = ( pow( ((mW*mW) - (vW1.p4().M2())),2) ) / (pow(aW,4));

  // construct the tL-term [seen lepton]
  double tTL = ( pow( ((mT*mT) - ((vL.p4()+vB1.p4()+vN.p4()).M2())),2) ) / (pow(aT,4));

  // construct the tM-term [miss lepton]
  double tTM = ( pow( ((mT*mT) - ((vB2.p4()+vW.p4()).M2())),2) ) / (pow(aT,4));

  // construct the CM-term
  double tCM = ( pow( ((4*(mT*mT)) - ((vL.p4()+vN.p4()+vW.p4()+vB1.p4()+vB2.p4()).M2())),2) ) / (pow(aCM,4));


  // calculate Topness
  double Topness_ = tW + tTL + tTM + tCM;

  if(info){
    info->topness = log(Topness_);
    info->top1_l= vL;
    info->top1_n= vN;
    info->top1_b= vB1;
    info->top1_w= vW1;
    info->top2_w= vW;
    info->top2_b= vB2;
    info->tW  = tW ;
    info->tTL = tTL;
    info->tTM = tTM;
    info->tCM = tCM;

  }


  return Topness_;
} // ~ end of Topness function



void Topness::minuitFunctionWrapper(int& nDim, double* gout, double& result, double* par, int flg) {


  result = topnessFunction(par[0],par[1],par[2],par[3],
			   par[4],par[5],par[6],par[7],
			   par[8],par[9],par[10],par[11],
			   par[12],par[13],par[14],par[15],
			   par[16],par[17],par[18],par[19]
			   );

} // ~end of minuit function



double Topness::topnessMinimization(const MomentumF *lep_, const MomentumF *bjet1_, const MomentumF *bjet2_, const MomentumF *met_, TopnessInformation * info) {


  // get variables for Topness
  double iLpx = lep_->p4().px();
  double iLpy = lep_->p4().py();
  double iLpz = lep_->p4().pz();
  double iLpe = lep_->p4().E();
  double iB1px = bjet1_->p4().px();
  double iB1py = bjet1_->p4().py();
  double iB1pz = bjet1_->p4().pz();
  double iB1pe = bjet1_->p4().E();
  double iB2px = bjet2_->p4().px();
  double iB2py = bjet2_->p4().py();
  double iB2pz = bjet2_->p4().pz();
  double iB2pe = bjet2_->p4().E();
  double iMpx = met_->p4().px();
  double iMpy = met_->p4().py();
  double iMpz = met_->p4().pz();
  double iMpe = met_->p4().E();

  // Define parameters [param number, param name, init val, estimated distance to min, bla, bla] // 300,3000,-3000,3000
  minimizer->SetParameter(0,"pwx",0,500,-3000,3000);
  minimizer->SetParameter(1,"pwy",0,500,-3000,3000);
  minimizer->SetParameter(2,"pwz",0,500,-3000,3000);
  minimizer->SetParameter(3,"pnz",0,500,-3000,3000);

  // fixed parameters
  minimizer->SetParameter(4,"plx",iLpx,0,iLpx-0.001,iLpx+0.001);
  minimizer->SetParameter(5,"ply",iLpy,0,iLpy-0.001,iLpy+0.001);
  minimizer->SetParameter(6,"plz",iLpz,0,iLpz-0.001,iLpz+0.001);
  minimizer->SetParameter(7,"ple",iLpe,0,iLpe-0.001,iLpe+0.001);
  minimizer->SetParameter(8,"pb1x",iB1px,0,iB1px-0.001,iB1px+0.001);
  minimizer->SetParameter(9,"pb1y",iB1py,0,iB1py-0.001,iB1py+0.001);
  minimizer->SetParameter(10,"pb1z",iB1pz,0,iB1pz-0.001,iB1pz+0.001);
  minimizer->SetParameter(11,"pb1e",iB1pe,0,iB1pe-0.001,iB1pe+0.001);
  minimizer->SetParameter(12,"pb2x",iB2px,0,iB2px-0.001,iB2px+0.001);
  minimizer->SetParameter(13,"pb2y",iB2py,0,iB2py-0.001,iB2py+0.001);
  minimizer->SetParameter(14,"pb2z",iB2pz,0,iB2pz-0.001,iB2pz+0.001);
  minimizer->SetParameter(15,"pb2e",iB2pe,0,iB2pe-0.001,iB2pe+0.001);
  minimizer->SetParameter(16,"pmx",iMpx,0,iMpx-0.001,iMpx+0.001);
  minimizer->SetParameter(17,"pmy",iMpy,0,iMpy-0.001,iMpy+0.001);
  minimizer->SetParameter(18,"pmz",iMpz,0,iMpz-0.001,iMpz+0.001);
  minimizer->SetParameter(19,"pme",iMpe,0,iMpe-0.001,iMpe+0.001);

  minimizer->FixParameter(4);
  minimizer->FixParameter(5);
  minimizer->FixParameter(6);
  minimizer->FixParameter(7);
  minimizer->FixParameter(8);
  minimizer->FixParameter(9);
  minimizer->FixParameter(10);
  minimizer->FixParameter(11);
  minimizer->FixParameter(12);
  minimizer->FixParameter(13);
  minimizer->FixParameter(14);
  minimizer->FixParameter(15);
  minimizer->FixParameter(16);
  minimizer->FixParameter(17);
  minimizer->FixParameter(18);
  minimizer->FixParameter(19);


  // Run the simplex minimizer to get close to the minimum [no good precision]
  minimizer->ExecuteCommand("SIMPLEX",0,0);

  // Run the migrad minimizer to precisely estimate the minimum
  //    minimizer->ExecuteCommand("MIGRAD",0,0);


  //Get the best fit values
  double pwx_fit = minimizer->GetParameter(0);
  double pwy_fit = minimizer->GetParameter(1);
  double pwz_fit = minimizer->GetParameter(2);
  double pnz_fit = minimizer->GetParameter(3);


  // get the function value at best fit
  return  log(topnessFunction(pwx_fit,pwy_fit,pwz_fit,pnz_fit,
      iLpx,iLpy,iLpz,iLpe,
      iB1px,iB1py,iB1pz,iB1pe,
      iB2px,iB2py,iB2pz,iB2pe,
      iMpx,iMpy,iMpz,iMpe,info
      ));
} // ~ end of Topness Minimization()

double Topness::getMinTopness(const MomentumF *lep_, const MomentumF *bjet1_, const MomentumF *bjet2_, const MomentumF *met_,TopnessInformation * info) {
  TopnessInformation info1, info2;
  if (topnessMinimization(lep_,bjet1_,bjet2_,met_,&info1) < topnessMinimization(lep_,bjet2_,bjet1_,met_,&info2)){
    if(info) (*info) = (info1);
    return info1.topness;
  } else{
    if(info) (*info) = (info2);
    return info2.topness;
  }


}

double Topness::getMaxTopness(const MomentumF *lep_, const MomentumF *bjet1_, const MomentumF *bjet2_, const MomentumF *met_,TopnessInformation * info) {
  TopnessInformation info1, info2;
  if (topnessMinimization(lep_,bjet1_,bjet2_,met_,&info1) > topnessMinimization(lep_,bjet2_,bjet1_,met_,&info2)){
    if(info) (*info) = (info1);
    return info1.topness;
  } else{
    if(info) (*info) = (info2);
    return info2.topness;
  }}

bool sortInDecreasingCSV(RecoJetF *jet1, RecoJetF *jet2) { return jet1->csv() > jet2->csv() ; }


double Topness::findMinTopnessConfiguration(const std::vector<LeptonF*>& leptons,const std::vector<RecoJetF*>& jets,const MomentumF *met,TopnessInformation * info) {

  if(leptons.size() == 0 || jets.size() < 2) return -99;
  
  vector<pair<double,int> > rankedJets(jets.size());
  for(unsigned int iJ =0; iJ < jets.size(); ++iJ){
    rankedJets[iJ].first = jets[iJ]->csv();
    rankedJets[iJ].second = iJ;
  }
  std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());

  int csv1 = rankedJets[0].second;
  int csv2 = rankedJets[1].second;

  return getMinTopness(leptons[0],jets[csv1],jets[csv2],met,info);
} // end of Topness::findMinTopnessConfiguration


double Topness::findMinTopnessConfiguration(const LeptonF* lepton,const std::vector<RecoJetF*>& jets,const MomentumF *met,TopnessInformation * info) {

  if(jets.size() < 2) return -99;
  
  vector<pair<double,int> > rankedJets(jets.size());
  for(unsigned int iJ =0; iJ < jets.size(); ++iJ){
    rankedJets[iJ].first = jets[iJ]->csv();
    rankedJets[iJ].second = iJ;
  }
  std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());

  int csv1 = rankedJets[0].second;
  int csv2 = rankedJets[1].second;

  return getMinTopness(lepton,jets[csv1],jets[csv2],met,info);
} // end of Topness::findMinTopnessConfiguration


double Topness::find2BtagWeightedTopness(const std::vector<LeptonF*>& leptons,const std::vector<RecoJetF*>& jets,const MomentumF *met, int option) {

  if(leptons.size() == 0 || jets.size() < 2) return -99;
  
  vector<pair<double,int> > rankedJets(jets.size());
  for(unsigned int iJ =0; iJ < jets.size(); ++iJ){
    rankedJets[iJ].first = jets[iJ]->csv();
    rankedJets[iJ].second = iJ;
  }
  std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());

  int csv1 = rankedJets[0].second;
  int csv2 = rankedJets[1].second;

  double tmpTop = -99;
  if      (option == 1) { tmpTop = topnessMinimization(leptons[0],jets[csv1],jets[csv2],met); }
  else if (option == 2) { tmpTop = topnessMinimization(leptons[0],jets[csv2],jets[csv1],met); }

  return tmpTop;
} // end of Topness::findMinTopnessConfiguration
