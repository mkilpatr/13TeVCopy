/* 
 * CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll 
 */

/* 
   B-tagging efficiency corrections:
     data/mc scale factors are in CSVv2.csv, from 
	https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation74X50ns
     they're read with a .csv reader I got from the twiki, modified to return the nearest efficiency
       if the pt is out of range of the official scale factors (eg 20 GeV jet gets the eff of the nearest
       official scale factor at 30 GeV). abs(eta) should never be > 2.4, I didn't change that.

     **** there's a hack in place so light flavors are read using "comb" measurementType, and heavy
     ****   flavors using the user-defined string below, since right now only "comb" has light flavor measurements.
     ****   search for "hack" to remove if scale factors improve.

     mc efficiencies are calculated using the function in EffMapMaker.h
     and are stored in TH2D(pt,eta) (a histo for each process type, working point, and flavor) format in
	jetCorr.root
     systematics are all calculated at once. reweights are returned by
        getBtagCorrection
     and you have the option of which systematics (see below) and whether one or multiple operating points.
     default behavior is central values and multiple (LOOSE, MEDIUM, and TIGHT) operating points.

     adjustable options:
        oneOperatingPoint:    set which b-tagging operating point (OP) to use for the one-OP calculation.
				default MEDIUM
        operatingPointsToUse: set which OPs to use for the multiple-OP calculation.
	                        default LOOSE, MEDIUM, and TIGHT. can do just LOOSE and MEDIUM. 
        measurementType:      the data/mc scale factors have this option. for now we only have "mujets" and "comb".
        mcFlavStrings:        you can switch between UDS/G flavors being one MC eff measurement or two. always make sure the
				correctly named MC eff histogram is available. split is recommended by twiki, since pileup
				is often tagged as low-pt gluon jets   
*/

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_JETCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_JETCORRECTIONSET_H_
#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BTagCalibrationStandalone.h" // necessary since forward declaration doesn't let me make an array of objects :(
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "TFile.h"

class TH2D;

namespace ucsbsusy {

class BTAGCorr : public Correction {};

class JetCorrectionSet : public CorrectionSet {
public:
  enum CorrectionOptions {
                            NULLOPT         = 0         // no jet corrections
                          , BTAGWEIGHT      = (1 << 0)  // b-tag effs event reweighting
                          , BTAGOBJECTS     = (1 << 1)  // OR b-tag effs bjet shuffling (no reweighting)
  };

  // b-tag effs correction types
  //   light is udsg (for the moment; it's recommended to split uds/g as low-pT g effs are biased by PU), heavy is bc.
  //   up means positive systematics, eg for 
  //     the scale factor 1.01 + 0.05 - 0.03, up is 1.06 and down is 0.98.
  //   NOMINAL   - apply central systematics (cc) to all
  //   UPLIGHT   - cc to heavy, up to light
  //   DOWNLIGHT - cc to heavy, down to light
  //   UPHEAVY   - cc to light, up to heavy
  //   DOWNHEAVY - cc to light, down to heavy
  // jet flavors (official convention)
  //   B 0, C 1, UDSG 2
  // operating points (official convention)
  //   loose 0, medium 1, tight 2, csv reshaping (not used here) 3
  // systematics types 
  //   central 0, down 1, up 2

  enum BtagCorrectionType { NOMINAL, UPLIGHT, DOWNLIGHT, UPHEAVY, DOWNHEAVY, NUMCORRECTIONTYPES };
  enum MCJetFlavor        { JF_MC_B, JF_MC_C, JF_MC_UDS, JF_MC_G, NUMMCJETFLAVORS};
  enum jetFlavor          { JF_B, JF_C, JF_UDSG, NUMJETFLAVORS };
  enum operatingPoint     { OP_LOOSE, OP_MEDIUM, OP_TIGHT, NUMOPERATINGPOINTS };
  enum sysType            { CENTRAL, DOWN, UP, NUMSYSTYPES };

  // define the single and multiple operating points to use. both are always calculated.
  // adjust here the single OP by which we reweight
  unsigned int oneOperatingPoint = OP_MEDIUM;    
  // adjust here the multiple operating points by which we reweight. loose to tight please.
  std::vector<unsigned int> operatingPointsToUse = {
						    static_cast<unsigned int>(operatingPoint::OP_LOOSE)
						   ,static_cast<unsigned int>(operatingPoint::OP_MEDIUM)
//						   ,static_cast<unsigned int>(operatingPoint::OP_TIGHT) 
					           };
    
  JetCorrectionSet(): btagCorr(0), b_correctionOptions(NULLOPT), f_corr(0), histosLoaded(false) {}
  virtual ~JetCorrectionSet() { if(f_corr) f_corr->Close(); }; // gave a seg fault bug if file was never loaded (NULLOPT)

  virtual void load(TString fileName, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  // get b-tag eff event reweighting correction
  float getBtagCorrection(BtagCorrectionType CT, bool multipleOperatingPoints = true) const {
    return multipleOperatingPoints ? bTagWeightsOneOP[CT] : bTagWeightsMultipleOP[CT];
  }

private:
  //Correction list
  BTAGCorr * btagCorr;
    
  // btagging correction variables
  std::string heavyMeasurementType = "mujets";        // how scale factors were measured, see .csv scale factors file
  std::string lightMeasurementType = "comb";         // temporary hack? light flavor jets only measured in comb
  int b_correctionOptions;                       // preserved correctionOptions for processCorrection
  TFile * f_corr;                                // MC effs file
  bool histosLoaded;                             // load histos only once
  TH2D * h2_eff[NUMOPERATINGPOINTS][NUMMCJETFLAVORS]; // MC effs histos
  float bTagWeightsOneOP     [NUMCORRECTIONTYPES];    // weight using just one operating point
  float bTagWeightsMultipleOP[NUMCORRECTIONTYPES];    // weight using multiple operating points

  std::map<unsigned int, std::string>  opStrings        = { {OP_LOOSE, "LOOSE"}, {OP_MEDIUM, "MEDIUM"}, {OP_TIGHT, "TIGHT"}};
  std::map<unsigned int, std::string>  corrTypeToString = { {CENTRAL,"central"}, {DOWN,"down"}, {UP,"up"} };

  // change mcFlavStrings to adjust MC flavors (eg compress uds/g into udsg. make sure histogram is available.)
  std::map<unsigned int, std::string>  mcFlavStrings    = { {JF_MC_B,"b"}, {JF_MC_C,"c"}, {JF_MC_UDS,"uds"}, {JF_MC_G,"g"} };

  // map JetFlavorInfo numbers to those used by official scale factors
  std::map<unsigned int, unsigned int> flavMapToSF ={{JetFlavorInfo::b_jet,   BTagEntry::FLAV_B   },   {JetFlavorInfo::ps_b_jet,   BTagEntry::FLAV_B},
                                                     {JetFlavorInfo::c_jet,   BTagEntry::FLAV_C   },   {JetFlavorInfo::ps_c_jet,   BTagEntry::FLAV_C},
                                                     {JetFlavorInfo::uds_jet, BTagEntry::FLAV_UDSG},   {JetFlavorInfo::ps_uds_jet, BTagEntry::FLAV_UDSG},
                                                     {JetFlavorInfo::g_jet,   BTagEntry::FLAV_UDSG},   {JetFlavorInfo::ps_g_jet,   BTagEntry::FLAV_UDSG} };
  // map JetFlavorInfo numbers to those used by monte carlo
  std::map<unsigned int, unsigned int> flavMapToMC ={{JetFlavorInfo::b_jet,   MCJetFlavor::JF_MC_B   },   {JetFlavorInfo::ps_b_jet,   MCJetFlavor::JF_MC_B  },
                                                     {JetFlavorInfo::c_jet,   MCJetFlavor::JF_MC_C   },   {JetFlavorInfo::ps_c_jet,   MCJetFlavor::JF_MC_C  },
                                                     {JetFlavorInfo::uds_jet, MCJetFlavor::JF_MC_UDS },   {JetFlavorInfo::ps_uds_jet, MCJetFlavor::JF_MC_UDS},
                                                     {JetFlavorInfo::g_jet,   MCJetFlavor::JF_MC_G   },   {JetFlavorInfo::ps_g_jet,   MCJetFlavor::JF_MC_G  }};

  BTagCalibration * calib;
//  BTagCalibrationReader reader[NUMOPERATINGPOINTS][NUMSYSTYPES]; // before hack (one measurementType)
  BTagCalibrationReader heavyFlavorReader[NUMOPERATINGPOINTS][NUMSYSTYPES]; // hack
  BTagCalibrationReader lightFlavorReader[NUMOPERATINGPOINTS][NUMSYSTYPES]; // hack for light flavors only in "comb"
};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_JETCORRECTIONSET_H_ */
