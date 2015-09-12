#include "AnalysisBase/TreeAnalyzer/interface/BTagCalibrationStandalone.h"
#include "AnalysisBase/TreeAnalyzer/interface/JetCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "TH2D.h"
#include "TFile.h"

namespace ucsbsusy {


void JetCorrectionSet::load(TString fileName, int correctionOptions)
{
  // preserve options (since processCorrection is invoked without it)  
  b_correctionOptions = correctionOptions;

  // contradiction checks in options
  if((correctionOptions & BTAGOBJECTS) && (correctionOptions & BTAGWEIGHT)) {
    throw "error in b-tagging corrections: Can't declare both options BTAGOBJECTS and BTAGWEIGHT";
  }

  // BTAG correction file loading
  if(correctionOptions != NULLOPT) {
    corrections.push_back(btagCorr);

    // load MC efficiency histogram file
    TString s_corr_path = TString::Format("%s/src/data/corrections/",cfgSet::CMSSW_BASE); 
    TString s_beff_path = s_corr_path+"jetCorr.root";
    std::string s_csv_path = s_corr_path.Data();
    std::string s_csv_name = "CSVv2.csv"; // .c_str()?
    s_csv_path += s_csv_name;

    std::cout << "Loading files: " << std::endl;
    std::cout << "  " << s_beff_path << std::endl;
    std::cout << "  " << s_csv_path << std::endl;
    std::cout << "Loading correction: ";
    if(correctionOptions & BTAGWEIGHT) std::cout << "BTAGWEIGHT";
    else std::cout << "BTAGOBJECTS";
    std::cout << std::endl;

    f_corr = new TFile(s_beff_path, "READ");
    if(f_corr->IsZombie()) throw "error in b-tagging corrections: root file is a zoombie. A zoombie, Carl!";
    
    // load .csv offical scale factor file and readers (operating point, systematics type)
    calib = new BTagCalibration("csvv2", s_csv_path);
    for(unsigned int iOP = 0; iOP < NUMOPERATINGPOINTS; iOP++){
      // before hack, three lines: reader[][CENTRAL/DOWN/UP] = ...
      // hack for light flavors being only in "comb":
      heavyFlavorReader[iOP][CENTRAL] = BTagCalibrationReader(&(*calib),(BTagEntry::OperatingPoint)iOP, heavyMeasurementType,"central");
      heavyFlavorReader[iOP][DOWN   ] = BTagCalibrationReader(&(*calib),(BTagEntry::OperatingPoint)iOP, heavyMeasurementType,"down");
      heavyFlavorReader[iOP][UP     ] = BTagCalibrationReader(&(*calib),(BTagEntry::OperatingPoint)iOP, heavyMeasurementType,"up");
      lightFlavorReader[iOP][CENTRAL] = BTagCalibrationReader(&(*calib),(BTagEntry::OperatingPoint)iOP, lightMeasurementType,"central"); 
      lightFlavorReader[iOP][DOWN   ] = BTagCalibrationReader(&(*calib),(BTagEntry::OperatingPoint)iOP, lightMeasurementType,"down");
      lightFlavorReader[iOP][UP     ] = BTagCalibrationReader(&(*calib),(BTagEntry::OperatingPoint)iOP, lightMeasurementType,"up");
    }
  }//BTAG file loading

}//load

void JetCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  // reweight only MC
  if(!ana->isMC()){
    return;
  }

  // load this specific process' MC efficiency histograms
  // note: need to do just once, but have to do it _after_ the event info 
  //   (process name in particular) is filled in the Analyzer, so it can't go in load().
  TString s_proc = defaults::PROCESS_NAMES[ana->process];
  if(!histosLoaded){
//    std::cout << "**** histos available: " << std::endl;
//    f_corr->ls();
    for(unsigned int iOP = 0; iOP < NUMOPERATINGPOINTS; iOP++)
      for(unsigned int iFl = 0; iFl < NUMMCJETFLAVORS; iFl++){
        TString debugs = "h2_BTaggingEfficiency_" + s_proc + "_" + opStrings[iOP] + "_"+mcFlavStrings[iFl];
//        std::cout << "**** loading histo: " << debugs << std::endl;
        if (!(f_corr->GetListOfKeys()->Contains("h2_BTaggingEfficiency_" + s_proc + "_" + opStrings[iOP] + "_"+mcFlavStrings[iFl])))
          throw "error in b-tagging corrections: root file doesn't contain all needed histograms";
        h2_eff[iOP][iFl] = (TH2D*)f_corr->Get("h2_BTaggingEfficiency_" + s_proc + "_" + opStrings[iOP] + "_"+mcFlavStrings[iFl]);
      }
    histosLoaded = true;
  }

//  std::cout << "**** event start" << std::endl;

  // option to produce reweight for each event, leaving bjet objects untouched.
  // we calculate all reweights (varying systematics) so user has to run just once.
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods
  if(b_correctionOptions & BTAGWEIGHT) {

//    std::cout << "**** BTAGWEIGHT " << std::endl;

    // start (multiplicative) event probabilities at one
    float pmc = 1;                   // event's probability (given btagging) in MC
    float pdata[NUMCORRECTIONTYPES]; // same but for data, so systematics
    float pmcOneOP = 1;
    float pdataOneOP[NUMCORRECTIONTYPES];

    for(unsigned int iCT = 0; iCT < NUMCORRECTIONTYPES; iCT++) 
      {pdata[iCT] = 1; pdataOneOP[iCT]=1;}

    // loop over all matched jets, MUST use same code as when creating MC eff histos
    for(auto * jet : ana->jets) {
      if(!(jet->genJet())) continue;
      int flavor = jet->genJet()->flavor();
      if(flavor == JetFlavorInfo::unmatched_jet) continue;

//      std::cout << "****  jet chosen JetFlavorInfo, JetFlavor, pt, eta: " << flavor << " " << flavMapToSF[flavor] << " " << jet->pt() << " " << jet->eta() << std::endl;

      float csv = jet->csv(); // default (error) from csv algo is -10, so assume untagged

      // assign btag to jet for every operating point
      bool tagged[NUMOPERATINGPOINTS];
      tagged[OP_LOOSE]  = csv > defaults::CSV_LOOSE;
      tagged[OP_MEDIUM] = csv > defaults::CSV_MEDIUM;
      tagged[OP_TIGHT]  = csv > defaults::CSV_TIGHT;

//      std::cout << "****  tagging (LMT): " << tagged[0] << tagged[1] << tagged[2] << std::endl;

      // get MC effs for the jet and every operating point in use
      Int_t binPt;
      Int_t binEta;
      float eff[NUMOPERATINGPOINTS];
      for(unsigned int iOP = 0; iOP < NUMOPERATINGPOINTS; iOP++){
        binPt    = h2_eff[iOP][flavMapToMC[flavor]]->GetXaxis()->FindBin(jet->pt());
        binEta   = h2_eff[iOP][flavMapToMC[flavor]]->GetYaxis()->FindBin(jet->eta());          
        eff[iOP] = h2_eff[iOP][flavMapToMC[flavor]]->GetBinContent(binPt,binEta);
//       std::cout << "**** MC effs for OP: " << iOP <<std::endl; 
//       std::cout << "****    binPt, binEta, eff:  " << binPt << " " << binEta <<  " " << eff[iOP] << std::endl;
      }

      // get official scale factors for the jet, every operating point, and every systematics type
      float sf[NUMOPERATINGPOINTS][NUMCORRECTIONTYPES];
      unsigned int sysTypeLight, sysTypeHeavy;
      for(unsigned int iOP = 0; iOP < NUMOPERATINGPOINTS; iOP++){
        for(unsigned int iCT = 0; iCT < NUMCORRECTIONTYPES; iCT++){
          switch ((BtagCorrectionType)iCT){
            case NOMINAL :
              sysTypeLight = CENTRAL; sysTypeHeavy = CENTRAL; break;
            case UPLIGHT :
              sysTypeLight = UP     ; sysTypeHeavy = CENTRAL; break;
            case DOWNLIGHT :
              sysTypeLight = DOWN   ; sysTypeHeavy = CENTRAL; break;
            case UPHEAVY :
              sysTypeLight = CENTRAL; sysTypeHeavy = UP     ; break;
            case DOWNHEAVY :
              sysTypeLight = CENTRAL; sysTypeHeavy = DOWN   ; break;
            default : throw "unknown b-tag effs correction systematic type";
          }
	  // before hack: same reader for light and heavy
          sf[iOP][iCT] = ((BTagEntry::JetFlavor)flavMapToSF[flavor] < BTagEntry::FLAV_UDSG)
            ? heavyFlavorReader[iOP][sysTypeHeavy].eval((BTagEntry::JetFlavor)flavMapToSF[flavor],jet->eta(),jet->pt())
            : lightFlavorReader[iOP][sysTypeLight].eval((BTagEntry::JetFlavor)flavMapToSF[flavor],jet->eta(),jet->pt());
//          std::cout << "****  scale factor: iOP, iCT, SF: " << iOP << " " << iCT << " " << sf[iOP][iCT] << std::endl;
//          std::cout << "****    BTagEntry jet flavor: " << flavMapToSF[flavor] << std::endl;
//          std::cout << "****    sysTypeHeavy and sysTypeLight: " << sysTypeHeavy << sysTypeLight << std::endl;
//          std::cout << "****    jet pt and eta: " << jet->pt() << " " << jet->eta() << std::endl;
        }
      }//operatingPoints

      // update mc and data probabilities for multiple operating points.
      // method:
      //   loop through the multiple operating points to use
      //   and translate from them to the whole enum of points (operatingPoint)
      //   using the user-adjustable list operatingPointsToUse.      

//      std::cout << "**** using OPs: ";
//      for(unsigned int J = 0; J <  operatingPointsToUse.size(); J++) 
//        std::cout << "  " << operatingPointsToUse[J];
//      std::cout << std::endl;
 
      for(unsigned int J = 0; J <  operatingPointsToUse.size(); J++){
//        std::cout << "**** for operating point: " << J << std::endl;

        // only update this part of (mc and data) product if jet is tagged J but not J+1, eg loose but not medium
        // and no jet is tagged as beyond-tightest operating point
        bool isTaggedJ   = tagged[operatingPointsToUse[J]];
        bool isTaggedJp1 = (J != operatingPointsToUse.size()-1) ? tagged[operatingPointsToUse[J+1]] : 0;
//        std::cout << "**** isTagged, isTaggedJp1: " << isTaggedJ << " " << isTaggedJp1 << std::endl;
        if (!(isTaggedJ && !isTaggedJp1)) continue;

        // eff for Jth tag for the jet's properties. beyond-loosest OP (J=0) always given eff = 1.
        // eff for J+1st tag. beyond-tightest OP (J=mOP) always given eff = 0.
        float effJ   = (J != 0) ? eff[operatingPointsToUse[J]] : 1;
        float effJp1 = (J != operatingPointsToUse.size()-1) ? eff[operatingPointsToUse[J+1]] : 0;
  
//        std::cout << "**** effJ, effJp1: " << effJ << " " << effJp1 << std::endl;
//        std::cout << "**** old pmc:  " << pmc << std::endl;
  
        // update MC probability (no systematics)
        pmc *= effJ - effJp1;
  
//        std::cout << "**** new pmc: " << pmc << std::endl;

        // update data probability (for each set of systematics) in same way
        for(unsigned int iCT = 0; iCT < NUMCORRECTIONTYPES; iCT++){
          float sfJ    = (J != 0) ? sf [operatingPointsToUse[J]][iCT] : 1;
          float sfJp1  = (J != operatingPointsToUse.size()-1) ? sf [operatingPointsToUse[J+1]][iCT] : 0;
//          std::cout << "**** old pdata, iCT: " << pdata[iCT] << " " << iCT << std::endl;
          pdata[iCT] *= sfJ*effJ - sfJp1*effJp1;       
//          std::cout << "**** new pdata, iCT: " << pdata[iCT] << " " << iCT << std::endl;
        }
      }//operatingPointsToUse
      
      // also update mc and data probabilities for one operating point
      bool isTaggedOneOP = tagged[oneOperatingPoint];
//      std::cout << "**** istaggedoneop (MED?):  " << isTaggedOneOP << std::endl;      
//        std::cout << "**** old pmcOneOP:  " << pmcOneOP << std::endl;
//        std::cout << "**** oneop eff:  " << eff[oneOperatingPoint] << std::endl;

      pmcOneOP *= isTaggedOneOP ? eff[oneOperatingPoint] : 1-eff[oneOperatingPoint];
//        std::cout << "**** new pmcOneOP:  " << pmcOneOP << std::endl;

      for(unsigned int iCT = 0; iCT < NUMCORRECTIONTYPES; iCT++){
//        std::cout << "**** old pdataOneOP, iCT:  " << pdataOneOP[iCT] << " " << iCT << std::endl;
        float sfTimesEff = sf[oneOperatingPoint][iCT]*eff[oneOperatingPoint];
//        std::cout << "**** sf, eff, sfTimesEFf for one OP: " << sf[oneOperatingPoint][iCT] << " " << eff[oneOperatingPoint] << " " << sfTimesEff << std::endl;
        pdataOneOP[iCT] *= isTaggedOneOP ? sfTimesEff : 1 - sfTimesEff;
//        std::cout << "**** new pdataOneOP, iCT:  " << pdataOneOP[iCT] << " " << iCT << std::endl;
      }

    }//jets

    // record the reweight of the event
    for(unsigned int iCT = 0; iCT < NUMCORRECTIONTYPES; iCT++){
      bTagWeightsMultipleOP[iCT] = pdata[iCT]/pmc;
      bTagWeightsOneOP[iCT]      = pdataOneOP[iCT]/pmcOneOP;
//        std::cout << "**** multiple weights, iCT:  " << bTagWeightsMultipleOP[iCT]  << std::endl;
//        std::cout << "**** one weights, iCT:  " << bTagWeightsOneOP[iCT]  << std::endl;

    } 

  }//BTAGWEIGHT

  // OR option to shuffle bjets, without event reweighting
  else if (b_correctionOptions & BTAGOBJECTS) {

  }//BTAGOBJECTS

}//processCorrection

}// namespace ucsbsusy
