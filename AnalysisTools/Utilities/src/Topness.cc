#include "../interface/Topness.h"

using namespace ucsbsusy;


double Topness::mW  = 81.;
double Topness::mT  = 172.;
double Topness::aW  = 5.;
double Topness::aT  = 15.;
double Topness::aCM = 1000.;


Topness::Topness() {}


double Topness::topnessFunction(double pwx_, double pwy_, double pwz_, double pnz_,
				double plx_, double ply_, double plz_, double ple_,
				double pb1x_, double pb1y_, double pb1z_, double pb1e_,
				double pb2x_, double pb2y_, double pb2z_, double pb2e_,
				double pmx_, double pmy_, double pmz_, double pme_
				) {


  // construct the lorentz vectors
  MomentumF vW;
  vW.p4().SetPxPyPzE(pwx_,pwy_,pwz_,(sqrt((mW*mW)+(pwx_*pwx_)+(pwy_*pwy_)+(pwz_*pwz_)))) ;

  MomentumF vL;
  vL.p4().SetPxPyPzE(plx_,ply_,plz_,ple_);

  MomentumF vB1;
  vB1.p4().SetPxPyPzE(pb1x_,pb1y_,pb1z_,pb1e_);

  MomentumF vB2;
  vB2.p4().SetPxPyPzE(pb2x_,pb2y_,pb2z_,pb2e_);

  MomentumF vMET;
  vMET.p4().SetPxPyPzE(pmx_,pmy_,pmz_,pme_);

  MomentumF vN;
  vN.p4().SetPxPyPzE((pmx_-pwx_),(pmy_-pwy_),pnz_,(sqrt(pow((pmx_-pwx_),2)+pow((pmy_-pwy_),2)+pow(pnz_,2))));


  // construct the w-term
  double tW = ( pow( ((mW*mW) - (vW.p4().M2())),2) ) / (pow(aW,4));

  // construct the tL-term [seen lepton]
  double tTL = ( pow( ((mT*mT) - ((vL.p4()+vB1.p4()+vN.p4()).M2())),2) ) / (pow(aT,4));

  // construct the tM-term [miss lepton]
  double tTM = ( pow( ((mT*mT) - ((vB2.p4()+vW.p4()).M2())),2) ) / (pow(aT,4));

  // construct the CM-term
  double tCM = ( pow( ((4*(mT*mT)) - ((vL.p4()+vN.p4()+vW.p4()+vB1.p4()+vB2.p4()).M2())),2) ) / (pow(aCM,4));


  // calculate Topness
  double Topness_ = tW + tTL + tTM + tCM;

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



double Topness::topnessMinimization(MomentumF lep_, MomentumF bjet1_, MomentumF bjet2_, MomentumF met_) {


  // get variables for Topness
  double iLpx = lep_.px();
  double iLpy = lep_.py();
  double iLpz = lep_.pz();
  double iLpe = lep_.E();
  double iB1px = bjet1_.px();
  double iB1py = bjet1_.py();
  double iB1pz = bjet1_.pz();
  double iB1pe = bjet1_.E();
  double iB2px = bjet2_.px();
  double iB2py = bjet2_.py();
  double iB2pz = bjet2_.pz();
  double iB2pe = bjet2_.E();
  double iMpx = met_.px();
  double iMpy = met_.py();
  double iMpz = met_.pz();
  double iMpe = met_.E();

  TFitter *minimizer = new TFitter(4); // need to check if this values has to go to 20. Is there an effect?


  // less print-outs
  {
    double p1 = -1;
    minimizer->ExecuteCommand("SET PRINTOUT",&p1,1);
  }


  // tell minimizer about the function to be minimized
  minimizer->SetFCN(minuitFunctionWrapper);


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
  double minTopnessFunction = topnessFunction(pwx_fit,pwy_fit,pwz_fit,pnz_fit,
					      iLpx,iLpy,iLpz,iLpe,
					      iB1px,iB1py,iB1pz,iB1pe,
					      iB2px,iB2py,iB2pz,iB2pe,
					      iMpx,iMpy,iMpz,iMpe
					      );
  double minTopness = log(minTopnessFunction);


  // reconstruct the two tops
  MomentumF vW;
  vW.p4().SetPxPyPzE(pwx_fit,pwy_fit,pwz_fit,(sqrt((mW*mW)+(pwx_fit*pwx_fit)+(pwy_fit*pwy_fit)+(pwz_fit*pwz_fit)))) ;

  MomentumF vL;
  vL.p4().SetPxPyPzE(iLpx,iLpy,iLpz,iLpe);
  
  MomentumF vB1;
  vB1.p4().SetPxPyPzE(iB1px,iB1py,iB1pz,iB1pe);
  
  MomentumF vB2;
  vB2.p4().SetPxPyPzE(iB2px,iB2py,iB2pz,iB2pe);
  
  MomentumF vMET;
  vMET.p4().SetPxPyPzE(iMpx,iMpy,iMpz,iMpe);
  
  MomentumF vN;
  vN.p4().SetPxPyPzE((iMpx-pwx_fit),(iMpy-pwy_fit),pnz_fit,(sqrt(pow((iMpx-pwx_fit),2)+pow((iMpy-pwy_fit),2)+pow(pnz_fit,2))));

  topSeen1_.p4().SetPxPyPzE(0.001,0.001,0.001,0.001);
  topMiss1_.p4().SetPxPyPzE(0.001,0.001,0.001,0.001);
  topSeen2_.p4().SetPxPyPzE(0.001,0.001,0.001,0.001);
  topMiss2_.p4().SetPxPyPzE(0.001,0.001,0.001,0.001);
  
  topSeen1_.p4() = vL.p4()+vB1.p4()+vN.p4();
  topMiss1_.p4() = vB2.p4()+vW.p4();
  
  topSeen2_.p4() = vL.p4()+vB2.p4()+vN.p4();
  topMiss2_.p4() = vB1.p4()+vW.p4();
  

  delete minimizer;

  return minTopness;
} // ~ end of Topness Minimization()

double Topness::getMinTopness(MomentumF lep_, MomentumF bjet1_, MomentumF bjet2_, MomentumF met_) {
  return std::min( topnessMinimization(lep_,bjet1_,bjet2_,met_),topnessMinimization(lep_,bjet2_,bjet1_,met_) );
}

double Topness::getMaxTopness(MomentumF lep_, MomentumF bjet1_, MomentumF bjet2_, MomentumF met_) {
  return std::max( topnessMinimization(lep_,bjet1_,bjet2_,met_),topnessMinimization(lep_,bjet2_,bjet1_,met_) );
}

std::vector<double> Topness::topnessArray(MomentumF lep_, MomentumF bjet1_, MomentumF bjet2_, MomentumF met_) {


	  // get variables for topness
	  double iLpx = lep_.px();
	  double iLpy = lep_.py();
	  double iLpz = lep_.pz();
	  double iLpe = lep_.E();
	  double iB1px = bjet1_.px();
	  double iB1py = bjet1_.py();
	  double iB1pz = bjet1_.pz();
	  double iB1pe = bjet1_.E();
	  double iB2px = bjet2_.px();
	  double iB2py = bjet2_.py();
	  double iB2pz = bjet2_.pz();
	  double iB2pe = bjet2_.E();
	  double iMpx = met_.px();
	  double iMpy = met_.py();
	  double iMpz = met_.pz();
	  double iMpe = met_.E();

	  TFitter *minimizer = new TFitter(4); // need to check if this values has to go to 20. Is there an effect?


	  // less print-outs
	  {
	    double p1 = -1;
	    minimizer->ExecuteCommand("SET PRINTOUT",&p1,1);
	  }


	  // tell minimizer about the function to be minimized
	  minimizer->SetFCN(minuitFunctionWrapper);


	  // Define parameters [param number, param name, init val, estimated distance to min, bla, bla]
	  minimizer->SetParameter(0,"pwx",300,3000,-3000,3000);
	  minimizer->SetParameter(1,"pwy",300,3000,-3000,3000);
	  minimizer->SetParameter(2,"pwz",300,3000,-3000,3000);
	  minimizer->SetParameter(3,"pnz",300,3000,-3000,3000);

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


  // construct the lorentz vectors

  MomentumF vW;
  vW.p4().SetPxPyPzE(pwx_fit,pwy_fit,pwz_fit,(sqrt((mW*mW)+(pwx_fit*pwx_fit)+(pwy_fit*pwy_fit)+(pwz_fit*pwz_fit)))) ;

  MomentumF vL;
  vL.p4().SetPxPyPzE(iLpx,iLpy,iLpz,iLpe);

  MomentumF vB1;
  vB1.p4().SetPxPyPzE(iB1px,iB1py,iB1pz,iB1pe);

  MomentumF vB2;
  vB2.p4().SetPxPyPzE(iB2px,iB2py,iB2pz,iB2pe);

  MomentumF vMET;
  vMET.p4().SetPxPyPzE(iMpx,iMpy,iMpz,iMpe);

  MomentumF vN;
  vN.p4().SetPxPyPzE((iMpx-pwx_fit),(iMpy-pwy_fit),pnz_fit,(sqrt(pow((iMpx-pwx_fit),2)+pow((iMpy-pwy_fit),2)+pow(pnz_fit,2))));


  // construct the w-term
  double tW = ( pow( ((mW*mW) - (vW.p4().M2())),2) ) / (pow(aW,4));

  // construct the tL-term [seen lepton]
  double tTL = ( pow( ((mT*mT) - ((vL.p4()+vB1.p4()+vN.p4()).M2())),2) ) / (pow(aT,4));

  // construct the tM-term [miss lepton]
  double tTM = ( pow( ((mT*mT) - ((vB2.p4()+vW.p4()).M2())),2) ) / (pow(aT,4));

  // construct the CM-term
  double tCM = ( pow( ((4*(mT*mT)) - ((vL.p4()+vN.p4()+vW.p4()+vB1.p4()+vB2.p4()).M2())),2) ) / (pow(aCM,4));


  // calculate topness
  std::vector<double>  topness;
  topness.push_back(tW);  topness.push_back(tTL);   topness.push_back(tTM);   topness.push_back(tCM);
  /*topness[1]= tTL;
  topness[2] = tTM;
  topness[3] = tCM;*/

  return topness;
  delete minimizer;

} // ~ end of topness function


MomentumF Topness::getRecoTopSeen1() { return topSeen1_; }
MomentumF Topness::getRecoTopMiss1() { return topMiss1_; }
MomentumF Topness::getRecoTopSeen2() { return topSeen2_; }
MomentumF Topness::getRecoTopMiss2() { return topMiss2_; }
