#ifndef BTAGGINGEFFMAPMAKER_HH
#define BTAGGINGEFFMAPMAKER_HH

#include "TH2D.h"
#include "TFile.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h" // CSV_MEDIUM

struct EffMapMaker {

// initialize effmap file (can't do histos until event info is loaded, need process name)
EffMapMaker(TString effmapname) : effmapname_(effmapname), isLoaded_(false) {
  // make effmap file
  f_EffMap = new TFile(effmapname,"RECREATE");
}

void createEffMaps(defaults::Process process) {

  f_EffMap->cd();
  std::string s_proc = defaults::PROCESS_NAMES[process];

  // declare effmap histos
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
    bool isLoaded_ = false;

    // start new effmap file and histos
    // reference uses pt 0..1000 in 100 bins, eta -3..3 in 60 bins
    // we do pt 20+ (rare exceptions) and eta -2.4 ... 2.4 (no exceptions?)
    int ptNBins = 10;
    int etaNBins = 6;
    double ptMin = 0;
    double ptMax = 400;
    double etaMin = -2.4;
    double etaMax =  2.4;

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
