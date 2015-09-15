#ifndef BTAGGINGEFFMAPMAKER_HH
#define BTAGGINGEFFMAPMAKER_HH
 
//  written September 2015 by Alex Patterson
//
// part of b-tagging efficiency calculations:
//   BTaggingEffMapMaker.hh ---------> mergeEffMaps.py -----> jetCorrectionSet
//   numerator/denominator histos      merged eff histos      application of eff histos
//
//  creates TH2Ds with numerator and denominator of b-tagging efficiencies in X_effmap.root
//  links to the usual makeZeroLeptonSRTrees.C and ZeroLeptonTreeHelper.hh.
//
//  instructions to link this code and generate b-tagging numerator/denominator histos:
//    in makeZeroLeptonSRTrees.C (or OneLep...)
//      before calling analyze() put the two lines:
//        TString effmapname = outputdir+"/"+sname+"_effmap.root";
//        EffMapMaker * effmapmaker = new EffMapMaker(effmapname);
//      after calling analyze() you MUST put the line (it calls the destructor):
//        delete effmapmaker;
//      and add this new argument to the end of the analyzer's argument list:
//        effmapmaker
//    in ZeroLeptonTreeHelper.hh (or OneLep...)
//      add this argument to the analyzer:
//        EffMapMaker * effmapmaker 
//      this to its constructor's initializer list:
//        effmapmaker_(effmapmaker)
//      (so it looks like ", pars), effmapmaker_(effmapmaker) {" )
//      this to its data section (next to "TreeFiller filler" perhaps):
//        EffMapMaker * effmapmaker_;
//      and this to fillEvent(), before any cuts are made:
//        effmapmaker_->fillEffMaps(jets,process);
//      and this header file (make sure it's in current dir)
//        #include "BTaggingEffMapMaker.hh"
//      to speed things up, consider then commenting out 
//      the rest of fillEvent, as nothing else is needed)
//
//  after generating the X_effmap.root files you MUST run the the merger,
//    mergeEffMaps.py,
//  which combines the efficiency maps of sample-parts in a sample (eg ttbar_*_effmap.root),
//  and creates a rebinned efficiency = numerator/denominator histo ready for jetCorrectionSet.
//

#include "TH2D.h"
#include "TFile.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h" // CSV_MEDIUM

struct EffMapMaker {

// initialize effmap file (can't do histos until event info is loaded, need process name)
EffMapMaker(TString effmapname, TString sname) : effmapname_(effmapname), sname_(sname), isLoaded_(false) {
  // make effmap file
  f_EffMap = new TFile(effmapname,"RECREATE");
}

void createEffMaps(defaults::Process process) {

  process_ = process;
  if (process_ > defaults::Process::SIGNAL) return; // don't bother with effmaps for data

  std::string s_proc;
  // for signal MC samples, 'signal' is insufficient. this code grabs eg 'T2tt' or 'T1tttt' from the file name.
//  if (process_ != defaults::SIGNAL)
    s_proc = defaults::PROCESS_NAMES[process_];
//  else {
//    TString s2(sname_(0,sname_.First('_')));
//    s_proc = s2.Data(); 
//  }
  if (process_ > defaults::Process::SIGNAL) return; // don't bother with effmaps for data

  // declare effmap histos
  f_EffMap->cd();
  std::cout << "Filling these b-tagging efficiency maps (TH2D) in the file " << std::endl << effmapname_ << std::endl;
  for(unsigned int iOP = 0; iOP < NUMOPERATINGPOINTS; iOP++){ // operating points
    for(unsigned int iFl = 0; iFl < NUMMCJETFLAVORS; iFl++){ // MC jet flavors
      std::string sNum   = "h2_BTaggingEfficiency_Num_"   + s_proc + "_" + opStrings[iOP] + "_"+mcFlavStrings[iFl];
      std::string sDenom = "h2_BTaggingEfficiency_Denom_" + s_proc + "_" + opStrings[iOP] + "_"+mcFlavStrings[iFl];
      std::cout << sNum.c_str() << std::endl;
      std::cout << sDenom.c_str() << std::endl;
      h2_num  [iOP][iFl] = new TH2D(sNum.c_str(), ";p_{T} [GeV];#eta",ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax);
      h2_denom[iOP][iFl] = new TH2D(sDenom.c_str(), ";p_{T} [GeV];#eta",ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax);
    }
  }

  isLoaded_ = true;
}

// write and close effmap histos and file
~EffMapMaker() {
  std::cout << "**** destructor called, writing histos to file" << std::endl;
  f_EffMap->cd();
  f_EffMap->Write();
  f_EffMap->Close();
}//destruct

// loop over event's jets making effmaps
void fillEffMaps(vector<RecoJetF*> jets, defaults::Process process) {
  if(!isLoaded_) {createEffMaps(process);};
  if (process_ > defaults::Process::SIGNAL) return; // don't bother with effmaps for data

  for(auto* jet : jets){
    if(!(jet->genJet())) continue; // avoid segfault in flavor check
    int flavor = jet->genJet()->flavor();
    if(flavor == JetFlavorInfo::unmatched_jet) continue; // only matched jets

    float csv = jet->csv(); // default error from csv algo is -10, which should be (and is) considered untagged

    bool tagged[NUMOPERATINGPOINTS];
    tagged[0] = csv > defaults::CSV_LOOSE;
    tagged[1] = csv > defaults::CSV_MEDIUM;
    tagged[2] = csv > defaults::CSV_TIGHT;
    // room for expansion?

    // fill effmap histos
    for(unsigned int iOP = 0; iOP < NUMOPERATINGPOINTS; iOP++){
      // fill this flavor's denominators always
      h2_denom[iOP][flavMapToMC[flavor]]->Fill(jet->pt(),jet->eta());
      // if jet passes operating point, fill this flavor's numerator
      if(tagged[iOP])
        h2_num[iOP][flavMapToMC[flavor]]->Fill(jet->pt(),jet->eta());
    }
  }//jets
}//fillEffMaps

    defaults::Process process_;
    TString effmapname_;
    TString sname_;
    bool isLoaded_ = false;

    // start new effmap file and histos
    // it doesn't matter how finely we bin here, since python rebinner takes care of it.
    // the reference analysis code uses pt 0..1000 in 100 bins, eta -3..3 in 60 bins
    int ptNBins = 100;
    int etaNBins = 60; // (etaMax-etaMin)/etaNBins should divide 0.6 to put eta=2.4 on a bin edge
    double ptMin = 0.;
    double ptMax = 1000.;
    double etaMin = -3.;
    double etaMax =  3.;

    enum operatingPoint     { OP_LOOSE, OP_MEDIUM, OP_TIGHT, NUMOPERATINGPOINTS };
    enum MCJetFlavor        { JF_MC_B, JF_MC_C, JF_MC_UDS, JF_MC_G, NUMMCJETFLAVORS};
    map<unsigned int, std::string> opStrings = { {OP_LOOSE, "LOOSE"}, {OP_MEDIUM, "MEDIUM"}, {OP_TIGHT, "TIGHT"}};
    map<unsigned int, std::string> mcFlavStrings = { {JF_MC_B,"b"}, {JF_MC_C,"c"}, {JF_MC_UDS,"uds"}, {JF_MC_G,"g"} };
    // map JetFlavorInfo numbers to those used by monte carlo
    std::map<unsigned int, unsigned int> flavMapToMC ={{JetFlavorInfo::b_jet,   MCJetFlavor::JF_MC_B   },   {JetFlavorInfo::ps_b_jet,   MCJetFlavor::JF_MC_B  },
                                                       {JetFlavorInfo::c_jet,   MCJetFlavor::JF_MC_C   },   {JetFlavorInfo::ps_c_jet,   MCJetFlavor::JF_MC_C  },
                                                       {JetFlavorInfo::uds_jet, MCJetFlavor::JF_MC_UDS },   {JetFlavorInfo::ps_uds_jet, MCJetFlavor::JF_MC_UDS},
                                                       {JetFlavorInfo::g_jet,   MCJetFlavor::JF_MC_G   },   {JetFlavorInfo::ps_g_jet,   MCJetFlavor::JF_MC_G  }};
 
    TFile *f_EffMap;
    TH2D * h2_num[NUMOPERATINGPOINTS][NUMMCJETFLAVORS]; 
    TH2D * h2_denom[NUMOPERATINGPOINTS][NUMMCJETFLAVORS];

}; //EffMapMaker


#endif
