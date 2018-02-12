#ifndef LLPREDICTIONHELPER_H
#define LLPREDICTIONHELPER_H
#include "TTree.h"
#include <iostream>
#include <regex>
#include <assert.h>
#include "AnalysisMethods/EstTools/utils/HistGetter.hh"
#include "AnalysisMethods/EstTools/utils/Estimator.hh"
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/EstTools/Moriond2017/SRParameters.hh"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
//#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"

namespace LLSupport {

TTree * getTree(TString filename){
  TFile * sf = new TFile(filename,"read");
  TTree * st =0;
  sf->GetObject("Events",st);
  return st;
}

void setTitleOffset(TCanvas *c, double xOff = .950, double yOff = 1.400){
  TList * list = c->GetListOfPrimitives();
  for(unsigned int iP = 0; iP < list->GetSize(); ++iP){
    TH1 * h = dynamic_cast<TH1*>(list->At(iP));
    if(h == 0) continue;
    h->GetXaxis()->SetTitleOffset(xOff);
    h->GetYaxis()->SetTitleOffset(yOff);
  }
}

  bool integrating_over_met = true;
  double integrating_met_systematic = 0;
  const TString inputDir = "../../run/plots_18_02_05_prodIso";
  const TString inputDir_old = "../../run/plots_18_01_25_prodIso";
  const TString format = "pdf";
  const TString lumi = "35.6";
  const TString header = "#sqrt{s} = 13 TeV, L = 35.6 fb^{-1}";
  const TString eventsToSkip = "!(run == 1 && lumi == 91626 && event == 208129617)";
  const TString METPresel = "passjson && passmetmht && (passmetfilters || process==10) && (met >= 250) && (j1chEnFrac > 0.1) && (j1chEnFrac < 0.99)";
  const TString METPresel_forRespTailSF = "passjson && passmetmht100 && passmetfilters && (met >= 250) && (j1chEnFrac > 0.1) && (j1chEnFrac < 0.99)";
  const TString ResTailExtraCuts = "(((dphij1met < .1) || (dphij2met < .1)) && (nvetolep == 0) && (pseudoRespPassFilter == 1))";
  const TString BaselineExtraCuts = "((njets >= 5) && (nlbjets>= 2) && (nbjets>=1))";
  const TString METBaseline = "passjson && passmetmht && (passmetfilters || process==10) && (met >= 250) && (j1chEnFrac > 0.1) && (j1chEnFrac < 0.99) && (njets >= 5) && (nlbjets>= 2) && (nbjets>=1)";
  const TString CR_def = "((dphij1met < .1) || (dphij2met < .1) || (dphij3met < .1))";
  const TString SR_def = "((dphij1met >= 0.5) && (dphij2met>=0.5) && (dphij3met>=0.5) && (dphij4met>=0.5))";
  const TString stdWeight = "1.0";
  const TString stdMCWeight  = lumi + "*weight*truePUWeight*btagWeight";
  const TString stdLLWeight = lumi + "*weight*truePUWeight*btagWeight*qcdRespTailWeight";
  const TString stdMCWeightNonLL = stdMCWeight + "* 0.5205";
  const TString topMCWeight = "cttWeight"; //FIXME
  const TString dphi1 = "min(dphij1met, min(dphij2met, dphij3met)) == dphij1met";
  const TString dphi2 = "min(dphij1met, min(dphij2met, dphij3met)) == dphij2met";
  const TString dphi3 = "min(dphij1met, min(dphij2met, dphij3met)) == dphij3met";
  const TString elecSel = "(ngoodgenele == 1 && leptonpdgid == 11 && nvetotau == 0)";
  const TString muSel   = "(ngoodgenmu  == 1 && leptonpdgid == 13 && nvetotau == 0)";
  const TString tauElecSel = "(ngoodgenele == 1)";
  const TString tauMuSel   = "(ngoodgenmu  == 1)";
  const TString negelecSel = "(ngoodgenele == 0)";
  const TString negmuSel   = "(ngoodgenmu  == 0)";
  const TString tauSel  = "(ismc && npromptgentau == 1)";
  const TString zeroLep = "(nvetolep == 0 && nvetotau == 0)";
  const TString oneLep  = "(nvetolep == 1 && mtlepmet < 100)";
  const TString threeLep  = "(nvetolep == 3 && mtlepmet < 100)";


  const TString mtb_lt_175 = "(mtcsv12met<175)";               const TString mtb_lt_175_R = "M_{T}(b_{1,2},#slash{E}_{T}) < 175";        const TString mtb_lt_175_L = "\\mtb < 175";    
  const TString mtb_ge_175 = "(mtcsv12met>=175)";              const TString mtb_ge_175_R = "M_{T}(b_{1,2},#slash{E}_{T}) #geq 175";     const TString mtb_ge_175_L = "\\mtb \\geq 175";
  const TString nbInc      = "(nbjets>=1)";                    const TString nbInc_R      = "N_{b} #geq 1";                              const TString nbInc_L      = "\\nb \\geq 1";
  const TString nb1        = "(nbjets==1)";                    const TString nb1_R        = "N_{b} = 1";                                 const TString nb1_L        = "\\nb = 1";
  const TString nb2        = "(nbjets>=2)";                    const TString nb2_R        = "N_{b} #geq 2";                              const TString nb2_L        = "\\nb \\geq 2";
  const TString int_nj     = "((njets>=5) && (njets<=6))";     const TString int_nj_R     = "5 #leq N_{j} #leq 6";                       const TString int_nj_L     = "5 \\leq \\njet \\leq 6";
  const TString med_nj     = "(njets>=5)";                     const TString med_nj_R     = "N_{j} #geq 5";                              const TString med_nj_L     = "\\njet \\geq 5";
  const TString hgh_nj     = "(njets>=7)";                     const TString hgh_nj_R     = "N_{j} #geq 7";                              const TString hgh_nj_L     = "\\njet \\geq 7";
  const TString nt0        = "(nsdtoploose==0)";               const TString nt0_R        = "N_{t} = 0";                                 const TString nt0_L        = "\\nt = 0";
  const TString nt1        = "(nsdtoploose>=1)";               const TString nt1_R        = "N_{t} #geq 1";                              const TString nt1_L        = "\\nt \\geq 1";
  const TString nw0        = "(nsdwloose==0)";                 const TString nw0_R        = "N_{W} = 0";                                 const TString nw0_L        = "\\nW = 0";
  const TString nw1        = "(nsdwloose>=1)";                 const TString nw1_R        = "N_{W} #geq 1";                              const TString nw1_L        = "\\nW \\geq 1";
  const TString mj0_1  = "(metj1 < 300)";                      const TString mj0_R = "(#slash{E}_{T}) + jet) < 300";	                 const TString mj0_L  = "\\Mt + \\jet < 300";
  const TString mj300_1  = "((metj1 >= 300) && (metj1 < 500))";const TString mj300_R = "300 <= (#slash{E}_{T}) + jet) < 500";	         const TString mj300_L  = "300 <= \\Mt + \\jet < 500";
  const TString mj500_1  = "((metj1 >= 500) && (metj1 < 700))";const TString mj500_R = "500 <= (#slash{E}_{T}) + jet) < 700";	         const TString mj500_L  = "500 <= \\Mt + \\jet < 700";
  const TString mj700_1  = "(metj1 >= 700)";                     const TString mj700_R = "(#slash{E}_{T}) + jet) >= 700";	                 const TString mj700_L = "\\Mt + \\jet >= 700"; 
  const TString mj0_2  = "(metj2 < 300)"; 
  const TString mj300_2  = "((metj2 >= 300) && (metj2 < 500))"; 
  const TString mj500_2  = "((metj2 >= 500) && (metj2 < 700))"; 
  const TString mj700_2  = "(metj2 >= 700)";  
  const TString mj0_3  = "(metj3 < 300)";   
  const TString mj300_3  = "((metj3 >= 300) && (metj3 < 500))";   
  const TString mj500_3  = "((metj3 >= 500) && (metj3 < 700))";   
  const TString mj700_3  = "(metj3 >= 700)";
  const TString mj0 = "(( " + dphi1 + " && " + mj0_1 + ") || (" + dphi2 + " && " + mj0_2 + ") || (" + dphi3 + " && " + mj0_3 + "))";
  const TString mj300 = "(( " + dphi1 + " && " + mj300_1 + ") || (" + dphi2 + " && " + mj300_2 + ") || (" + dphi3 + " && " + mj300_3 + "))";
  const TString mj500 = "(( " + dphi1 + " && " + mj500_1 + ") || (" + dphi2 + " && " + mj500_2 + ") || (" + dphi3 + " && " + mj500_3 + "))";
  const TString mj700 = "(( " + dphi1 + " && " + mj700_1 + ") || (" + dphi2 + " && " + mj700_2 + ") || (" + dphi3 + " && " + mj700_3 + "))";  

};

#endif
