#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/TnPUtils/interface/TagProbeFitter.h"
#endif

using namespace std;

// map of PDF labels and vector holding the corresponding PDF commands
map<string, vector<string>> fitPDFs;

// input variables
struct TnPVariable {

  TnPVariable(string vname, string vtitle, double vmin, double vmax, string vunits) : name(vname), title(vtitle), min(vmin), max(vmax), units(vunits) {}

  string name;
  string title;
  double min;
  double max;
  string units;

};

// information about efficiency parametrization
struct EfficiencySet {

  EfficiencySet() {}

  EfficiencySet(string esname, map<string, vector<double>> esbinning = map<string, vector<double>>(), map<string, vector<string>> escategories = map<string, vector<string>>()) : name(esname), binning(esbinning), categories(escategories) {}

  ~EfficiencySet() {}

  string name;
  map<string, vector<double>> binning;
  map<string, vector<string>> categories;

};

// helper class to interface with TagProbeFitter (from official TagAndProbe package) and to set up the fits
class TnPFitter {
  public:
    TnPFitter(vector<string> fileNames, string dirName, string treeName, string outfileName, vector<TnPVariable> tnpVars, vector<EfficiencySet> effSets, vector<string> binToPdfMap, unsigned int numCPU=1, bool saveWS=false);
    virtual ~TnPFitter(){};

    bool verbose = false;
    vector<string> unbinnedVariables = {"mass","weight"};

    void calculateEfficiencies();

  private:
    vector<EfficiencySet> effCalcs;
    vector<string>        pdfMapInput;
    TagProbeFitter        fitter;
};

// constructor and setup for TagProbeFitter
TnPFitter::TnPFitter(vector<string> fileNames, string dirName, string treeName, string outfileName, vector<TnPVariable> tnpVars, vector<EfficiencySet> effSets, vector<string> binToPdfMap, unsigned int numCPU, bool saveWS) :
  effCalcs(effSets),
  pdfMapInput(binToPdfMap),
  fitter(fileNames, dirName, treeName, outfileName, numCPU, saveWS)
{
  // do setup
  fitter.setQuiet(verbose);
  fitter.setBinnedFit(true, 60);
  fitter.setSaveDistributionsPlot(true);
  fitter.setWeightVar("weight");

  // add variables of interest
  for(auto var : tnpVars) {
    fitter.addVariable(var.name, var.title, var.min, var.max, var.units);
  }

  // define flags
  fitter.addCategory("pass","selected","dummy[true=1,false=0]");

  // add PDFs to be used for fitting
  assert(pdfMapInput.size()>=1);
  fitter.addPdf(pdfMapInput[0],fitPDFs[pdfMapInput[0]]);
  if(pdfMapInput.size()>1) {
    assert(pdfMapInput.size()%2 != 0); // after the first entry, the inputs need to be in bin label - pdf label pairs
    for(unsigned int index = 1; index < pdfMapInput.size(); index+=2) {
      fitter.addPdf(pdfMapInput[index+1], fitPDFs[pdfMapInput[index+1]]);
    }
  }
}

// carry out the fits/efficiency calculations
void TnPFitter::calculateEfficiencies()
{
  for (auto effset : effCalcs) {
    fitter.calculateEfficiency(effset.name, "pass", "true", unbinnedVariables, effset.binning, effset.categories, pdfMapInput);
  }
}


// fitModel: try MCConvGaussPlusExp for muons, MCConvGaussPlusErfExp for electrons
// will need MC templates for any "MCConv" PDF
// basic idea: we do a simultaneous fit of the the "passing" and "failing" samples to the assigned signal+background PDF
// the choices of fit model, binning are sources of systematic uncertainties, which can be estimated by varying those choices
void doTnPFits(const string mcFile       = "trees/dyjets_mu_tree.root",             // MC file ... leave blank if don't want to do MC effs
               const string dataFile     = "trees/singlemu-50ns_tree.root",         // data file ... leave blank if don't want to do data effs
               const string dirName      = "fits",                                  // directory name in output file in which fits will be saved
               const string outFileMC    = "fitResults_mu_mc.root",                 // MC output file name
               const string outFileData  = "fitResults_mu_data.root",               // data output file name
               const string fitModel     = "MCConvGaussPlusExp",                    // options for fit model
               const string templateFile = "AnalysisMethods/macros/TnP/MCTemplates/zmm_templates.root", // file with MC templates, relevant for PDFs using MC-based Z-shape
               const bool   doPtEtaEff   = true,          // 2D pt-eta binning of efficiencies
               const bool   doPtEff      = true,          // pt-binned efficiencies
               const bool   doEtaEff     = true,          // eta-binned efficiencies
               const bool   doHtEff      = false,          // htalong-binned eff
               const bool   doPtHtEff    = false,          // 2D pt-htalong binning
               const bool   doAnEff      = false,          // annulus-binned eff
               const bool   doPtAnEff    = false)          // 2D pt-annulus binning
{

  // 2D pt/eta binning for effs
  EfficiencySet pt_eta_effset("pt_eta_eff");
  pt_eta_effset.binning["pt"] = {10.0, 15.0, 20.0, 30.0, 40.0, 60.0, 100.0};
  //pt_eta_effset.binning["abseta"] = {0.0, 0.8, 1.6, 2.4}; // muons
  pt_eta_effset.binning["abseta"] = {0.0, 0.8, 1.5, 2.4}; // electrons

  // 2D pt/htalong binning for effs
  EfficiencySet pt_ht_effset("pt_eta_eff");
  pt_ht_effset.binning["pt"] = {10.0, 15.0, 20.0, 30.0, 40.0, 60.0, 100.0};
  pt_ht_effset.binning["htalong"] = {0.0, 50.0, 100.0, 300.0};   // htalong

  // 2D pt/annulus binning for effs
  EfficiencySet pt_an_effset("pt_eta_eff");
  pt_an_effset.binning["pt"] = {10.0, 15.0, 20.0, 30.0, 40.0, 60.0, 100.0};
  pt_an_effset.binning["annulus"] = {0.0, 1.0, 2.0, 3.0, 10.0};   // annulus

  // effs vs pt
  EfficiencySet pt_effset("pt_eff");
  pt_effset.binning["pt"] = {10.0, 15.0, 20.0, 30.0, 40.0, 60.0, 100.0};

  // effs vs eta
  EfficiencySet eta_effset("eta_eff");
  //eta_effset.binning["eta"] = {-2.4, -1.6, -0.8, 0.0, 0.8, 1.6, 2.4}; // muons
  eta_effset.binning["eta"] = {-2.4, -1.5, -0.8, 0.0, 0.8, 1.5, 2.4}; // electrons

  // effs vs htalong
  EfficiencySet ht_effset("eta_eff");
  ht_effset.binning["htalong"] = {0.0, 50.0, 100.0, 300.0};   // htalong

  // effs vs annulus
  EfficiencySet an_effset("eta_eff");
  an_effset.binning["annulus"] = {0.0, 1.0, 2.0, 3.0, 10.0};   // annulus

  // which efficiency parametrizations to evaluate
  vector<EfficiencySet> effsets;
  if(doPtEtaEff) effsets.push_back(pt_eta_effset);
  if(doPtEff)    effsets.push_back(pt_effset);
  if(doEtaEff)   effsets.push_back(eta_effset);
  if(doHtEff)    effsets.push_back(ht_effset);
  if(doPtHtEff)  effsets.push_back(pt_ht_effset);
  if(doAnEff)    effsets.push_back(an_effset);
  if(doPtAnEff)  effsets.push_back(pt_an_effset);

  // add relevant variables from tree: parameters are name, title, min, max, units
  vector<TnPVariable> vars;
  vars.push_back(TnPVariable("mass","m_{ll}",70,110,"GeV")); // was 60,120, try with 75,105
  vars.push_back(TnPVariable("weight","weight",0,1e+08,""));
  vars.push_back(TnPVariable("pt","Probe p_{T}",0,1000,"GeV"));
  vars.push_back(TnPVariable("eta","Probe #eta",-2.4,2.4,""));
  vars.push_back(TnPVariable("abseta","Probe |#eta|",0,2.4,""));
  vars.push_back(TnPVariable("htalong","Probe ht_{along}",0,1000,"GeV"));
  vars.push_back(TnPVariable("annulus","Probe annulus",0,1,""));

  // hold default PDF name (first entry), and, if there are different PDFs used for different bins/categories, hold pairs of bin name - PDF name
  vector<string> binToPDFmap;

  // definition of various signal + background PDF options. Needs a bit of trial-and-error to see what works best
  if(fitModel=="VPVPlusExpo") {              // signal: sum of 2 Voigtians, background: exponential
    fitPDFs["VPVPlusExpo"] = {
            "Voigtian::signal1(mass, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(mass, mean2[90,80,100], width,        sigma2[4,2,10])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"};
    binToPDFmap.push_back("VPVPlusExpo");
  }

  if(fitModel=="ZGenLinePlusErfExp") {       // signal: Z NLO line shape convolved with modified Crystal Ball, background: erf + exponential
    fitPDFs["ZGenLinePlusErfExp"] = {
            "RooCBExGaussShape::signalResPass(mass,meanP[-0.0,-5.000,5.000],sigmaP[0.956,0.00,5.000],alphaP[0.999, 0.0,50.0],nP[1.405,0.000,50.000],sigmaP_2[1.000,0.500,15.00])",
            "RooCBExGaussShape::signalResFail(mass,meanF[-0.0,-5.000,5.000],sigmaF[3.331,0.00,5.000],alphaF[1.586, 0.0,50.0],nF[0.464,0.000,20.00],sigmaF_2[1.675,0.500,2.000])",                                   
            "ZGeneratorLineShape::signalPhy(mass)", // NLO line shape
            "RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0])",             
            "RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0])",
            "FCONV::signalPass(mass, signalPhy, signalResPass)",
            "FCONV::signalFail(mass, signalPhy, signalResFail)",                                          
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[1.0]"};
    binToPDFmap.push_back("ZGenLinePlusErfExp");
  }

  if(fitModel=="ZGenLinePlusExp") {          // signal: Z NLO line shape convolved with modified Crystal Ball, background: exponential
    fitPDFs["ZGenLinePlusExp"] = {
            "RooCBExGaussShape::signalResPass(mass,meanP[0,-5.000,5.000],sigmaP[1.419,1.00,15.000],alphaP[0.130],nP[50.08],sigmaP_2[1, 1, 15.000])",
            "RooCBExGaussShape::signalResFail(mass,meanF[1.1,-5.000,5.000],sigmaF[1.331,0.100,15.000],alphaF[0.284], nF[3.350],sigmaF_2[1, 1, 15.000])",
            "ZGeneratorLineShape::signalPhy(mass)", // NLO line shape
            "RooExponential::backgroundPass(mass, aPass[-0.1, -1., 0])",
            "RooExponential::backgroundFail(mass, aFail[-0.1, -1., 0])",
            "FCONV::signalPass(mass, signalPhy, signalResPass)",
            "FCONV::signalFail(mass, signalPhy, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[0.9]"};
    binToPDFmap.push_back("ZGenLinePlusExp");
  }

  if(fitModel == "MCConvGaussPlusErfExp") {  // signal: MC template convolved with Gaussian, background: erf + exponential
    fitPDFs["MCConvGaussPlusErfExp"] = {
            "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
            "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
            "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_All_Pass\")",
            "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_All_Fail\")",
            "RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0])",
            "RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0])",
            "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
            "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[1.0]"};
    binToPDFmap.push_back("MCConvGaussPlusErfExp");
  }

  if(fitModel == "MCConvGaussPlusExp") {     // signal: MC template convolved with Gaussian, background: exponential
    fitPDFs["MCConvGaussPlusExp"] = {
            "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
            "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
            "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_All_Pass\")",
            "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_All_Fail\")",
            "RooExponential::backgroundPass(mass, aPass[-0.1, -1., 0])",
            "RooExponential::backgroundFail(mass, aFail[-0.1, -1., 0])",
            "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
            "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[0.9]"};
    binToPDFmap.push_back("MCConvGaussPlusExp");
  }

  // for MC templates, need to assign correct template/PDF for each pt/eta bin
  if(doPtEtaEff && (fitModel=="MCConvGaussPlusErfExp" || fitModel=="MCConvGaussPlusExp")) {
    for(int ipt = 0; ipt < pt_eta_effset.binning["pt"].size()-1; ++ipt) {
      for(int ieta = 0; ieta < pt_eta_effset.binning["abseta"].size()-1; ++ieta) {
        TString ptetabinstr = TString::Format("%2.1fTo%2.1f_%2.1fTo%2.1f", pt_eta_effset.binning["pt"][ipt], pt_eta_effset.binning["pt"][ipt+1], pt_eta_effset.binning["abseta"][ieta], pt_eta_effset.binning["abseta"][ieta+1]);
        TString ptetabinlabel = TString::Format("abseta_bin%d;pt_bin%d",ieta,ipt);

        string catlabel = "{" + string(ptetabinlabel.Data()) + "}";

        ptetabinlabel.ReplaceAll(";","__");

        string pdflabel = "MCConvGaussPlusErfExp_"+string(ptetabinlabel.Data());
        fitPDFs[pdflabel] = {
              "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
              "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
              "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+ptetabinstr.Data()+"_Pass\")",
              "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+ptetabinstr.Data()+"_Fail\")",
              "RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0])",
              "RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0])",
              "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
              "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
              "efficiency[0.5,0,1]",
              "signalFractionInPassing[1.0]"};

        if(fitModel == "MCConvGaussPlusErfExp") {
          binToPDFmap.push_back(catlabel);
          binToPDFmap.push_back(pdflabel);
        }
  
        pdflabel = "MCConvGaussPlusExp_"+string(ptetabinlabel.Data());
        fitPDFs[pdflabel] = {
              "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
              "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
              "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+ptetabinstr.Data()+"_Pass\")",
              "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+ptetabinstr.Data()+"_Fail\")",
              "RooExponential::backgroundPass(mass, aPass[-0.1, -1., 0])",
              "RooExponential::backgroundFail(mass, aFail[-0.1, -1., 0])",
              "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
              "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
              "efficiency[0.5,0,1]",
              "signalFractionInPassing[0.9]"};
  
        if(fitModel == "MCConvGaussPlusExp") {
          binToPDFmap.push_back(catlabel);
          binToPDFmap.push_back(pdflabel);
        }
      }
    }
  }

  if(doPtHtEff && (fitModel=="MCConvGaussPlusErfExp" || fitModel=="MCConvGaussPlusExp")) {
    for(int ipt = 0; ipt < pt_ht_effset.binning["pt"].size()-1; ++ipt) {
      for(int iht = 0; iht < pt_ht_effset.binning["htalong"].size()-1; ++iht) {
        TString pthtbinstr = TString::Format("%2.1fTo%2.1f_%2.1fTo%2.1f", pt_ht_effset.binning["pt"][ipt], pt_ht_effset.binning["pt"][ipt+1], pt_ht_effset.binning["htalong"][iht], pt_ht_effset.binning["htalong"][iht+1]);
        TString pthtbinlabel = TString::Format("htalong_bin%d;pt_bin%d",iht,ipt);

        string catlabel = "{" + string(pthtbinlabel.Data()) + "}";

        pthtbinlabel.ReplaceAll(";","__");

        string pdflabel = "MCConvGaussPlusErfExp_"+string(pthtbinlabel.Data());
        fitPDFs[pdflabel] = {
              "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
              "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
              "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+pthtbinstr.Data()+"_Pass\")",
              "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+pthtbinstr.Data()+"_Fail\")",
              "RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0])",
              "RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0])",
              "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
              "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
              "efficiency[0.5,0,1]",
              "signalFractionInPassing[1.0]"};

        if(fitModel == "MCConvGaussPlusErfExp") {
          binToPDFmap.push_back(catlabel);
          binToPDFmap.push_back(pdflabel);
        }

        pdflabel = "MCConvGaussPlusExp_"+string(pthtbinlabel.Data());
        fitPDFs[pdflabel] = {
              "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
              "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
              "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+pthtbinstr.Data()+"_Pass\")",
              "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+pthtbinstr.Data()+"_Fail\")",
              "RooExponential::backgroundPass(mass, aPass[-0.1, -1., 0])",
              "RooExponential::backgroundFail(mass, aFail[-0.1, -1., 0])",
              "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
              "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
              "efficiency[0.5,0,1]",
              "signalFractionInPassing[0.9]"};

        if(fitModel == "MCConvGaussPlusExp") {
          binToPDFmap.push_back(catlabel);
          binToPDFmap.push_back(pdflabel);
        }
      }
    }
  }

  if(doPtAnEff && (fitModel=="MCConvGaussPlusErfExp" || fitModel=="MCConvGaussPlusExp")) {
    for(int ipt = 0; ipt < pt_an_effset.binning["pt"].size()-1; ++ipt) {
      for(int iht = 0; iht < pt_an_effset.binning["annulus"].size()-1; ++iht) {
        TString ptanbinstr = TString::Format("%2.1fTo%2.1f_%2.1fTo%2.1f", pt_an_effset.binning["pt"][ipt], pt_an_effset.binning["pt"][ipt+1], pt_an_effset.binning["annulus"][iht], pt_an_effset.binning["annulus"][iht+1]);
        TString ptanbinlabel = TString::Format("annulus_bin%d;pt_bin%d",iht,ipt);

        string catlabel = "{" + string(ptanbinlabel.Data()) + "}";

        ptanbinlabel.ReplaceAll(";","__");

        string pdflabel = "MCConvGaussPlusErfExp_"+string(ptanbinlabel.Data());
        fitPDFs[pdflabel] = {
              "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
              "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
              "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+ptanbinstr.Data()+"_Pass\")",
              "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+ptanbinstr.Data()+"_Fail\")",
              "RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0])",
              "RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0])",
              "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
              "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
              "efficiency[0.5,0,1]",
              "signalFractionInPassing[1.0]"};

        if(fitModel == "MCConvGaussPlusErfExp") {
          binToPDFmap.push_back(catlabel);
          binToPDFmap.push_back(pdflabel);
        }

        pdflabel = "MCConvGaussPlusExp_"+string(ptanbinlabel.Data());
        fitPDFs[pdflabel] = {
              "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
              "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
              "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+ptanbinstr.Data()+"_Pass\")",
              "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+ptanbinstr.Data()+"_Fail\")",
              "RooExponential::backgroundPass(mass, aPass[-0.1, -1., 0])",
              "RooExponential::backgroundFail(mass, aFail[-0.1, -1., 0])",
              "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
              "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
              "efficiency[0.5,0,1]",
              "signalFractionInPassing[0.9]"};

        if(fitModel == "MCConvGaussPlusExp") {
          binToPDFmap.push_back(catlabel);
          binToPDFmap.push_back(pdflabel);
        }
      }
    }
  }

  if(doPtEff && (fitModel=="MCConvGaussPlusErfExp" || fitModel=="MCConvGaussPlusExp")) {
    for(int ipt = 0; ipt < pt_effset.binning["pt"].size()-1; ++ipt) {
      TString ptbinstr = TString::Format("%2.1fTo%2.1f", pt_effset.binning["pt"][ipt], pt_effset.binning["pt"][ipt+1]);
      TString ptbinlabel = TString::Format("pt_bin%d",ipt);

      string catlabel = "{" + string(ptbinlabel.Data()) + "}";

      string pdflabel = "MCConvGaussPlusErfExp_"+string(ptbinlabel.Data());
      fitPDFs[pdflabel] = {
            "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
            "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
            "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+ptbinstr.Data()+"_Pass\")",
            "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+ptbinstr.Data()+"_Fail\")",
            "RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0])",
            "RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0])",
            "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
            "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[1.0]"};
  
      if(fitModel == "MCConvGaussPlusErfExp") {
        binToPDFmap.push_back(catlabel);
        binToPDFmap.push_back(pdflabel);
      }

      pdflabel = "MCConvGaussPlusExp_"+string(ptbinlabel.Data());
      fitPDFs[pdflabel] = {
            "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
            "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
            "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+ptbinstr.Data()+"_Pass\")",
            "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+ptbinstr.Data()+"_Fail\")",
            "RooExponential::backgroundPass(mass, aPass[-0.1, -1., 0])",
            "RooExponential::backgroundFail(mass, aFail[-0.1, -1., 0])",
            "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
            "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[0.9]"};
  
      if(fitModel == "MCConvGaussPlusExp") {
        binToPDFmap.push_back(catlabel);
        binToPDFmap.push_back(pdflabel);
      }
    }
  }

  if(doEtaEff && (fitModel=="MCConvGaussPlusErfExp" || fitModel=="MCConvGaussPlusExp")) {
    for(int ieta = 0; ieta < eta_effset.binning["eta"].size()-1; ++ieta) {
      double mineta = min(fabs(eta_effset.binning["eta"][ieta]), fabs(eta_effset.binning["eta"][ieta+1]));
      double maxeta = max(fabs(eta_effset.binning["eta"][ieta]), fabs(eta_effset.binning["eta"][ieta+1]));
      TString etabinstr = TString::Format("%2.1fTo%2.1f", mineta, maxeta);
      TString etabinlabel = TString::Format("eta_bin%d",ieta);

      string catlabel = "{" + string(etabinlabel.Data()) + "}";

      string pdflabel = "MCConvGaussPlusErfExp_"+string(etabinlabel.Data());
      fitPDFs[pdflabel] = {
            "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
            "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
            "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+etabinstr.Data()+"_Pass\")",
            "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+etabinstr.Data()+"_Fail\")",
            "RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0])",
            "RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0])",
            "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
            "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[1.0]"};

      if(fitModel == "MCConvGaussPlusErfExp") {
        binToPDFmap.push_back(catlabel);
        binToPDFmap.push_back(pdflabel);
      }

      pdflabel = "MCConvGaussPlusExp_"+string(etabinlabel.Data());  
      fitPDFs[pdflabel] = {
            "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
            "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
            "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+etabinstr.Data()+"_Pass\")",
            "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+etabinstr.Data()+"_Fail\")",
            "RooExponential::backgroundPass(mass, aPass[-0.1, -1., 0])",
            "RooExponential::backgroundFail(mass, aFail[-0.1, -1., 0])",
            "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
            "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[0.9]"};
  
      if(fitModel == "MCConvGaussPlusExp") {
        binToPDFmap.push_back(catlabel);
        binToPDFmap.push_back(pdflabel);
      }
    }
  }

  if(doHtEff && (fitModel=="MCConvGaussPlusErfExp" || fitModel=="MCConvGaussPlusExp")) {
    for(int iht = 0; iht < ht_effset.binning["htalong"].size()-1; ++iht) {
      double minhtalong = min(fabs(ht_effset.binning["htalong"][iht]), fabs(ht_effset.binning["htalong"][iht+1]));
      double maxhtalong = max(fabs(ht_effset.binning["htalong"][iht]), fabs(ht_effset.binning["htalong"][iht+1]));
      TString htalongbinstr = TString::Format("%2.1fTo%2.1f", minhtalong, maxhtalong);
      TString htalongbinlabel = TString::Format("htalong_bin%d",iht);

      string catlabel = "{" + string(htalongbinlabel.Data()) + "}";

      string pdflabel = "MCConvGaussPlusErfExp_"+string(htalongbinlabel.Data());
      fitPDFs[pdflabel] = {
            "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
            "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
            "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+htalongbinstr.Data()+"_Pass\")",
            "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+htalongbinstr.Data()+"_Fail\")",
            "RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0])",
            "RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0])",
            "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
            "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[1.0]"};

      if(fitModel == "MCConvGaussPlusErfExp") {
        binToPDFmap.push_back(catlabel);
        binToPDFmap.push_back(pdflabel);
      }

      pdflabel = "MCConvGaussPlusExp_"+string(htalongbinlabel.Data());
      fitPDFs[pdflabel] = {
            "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
            "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
            "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+htalongbinstr.Data()+"_Pass\")",
            "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+htalongbinstr.Data()+"_Fail\")",
            "RooExponential::backgroundPass(mass, aPass[-0.1, -1., 0])",
            "RooExponential::backgroundFail(mass, aFail[-0.1, -1., 0])",
            "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
            "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[0.9]"};

      if(fitModel == "MCConvGaussPlusExp") {
        binToPDFmap.push_back(catlabel);
        binToPDFmap.push_back(pdflabel);
      }
    }
  }

  if(doAnEff && (fitModel=="MCConvGaussPlusErfExp" || fitModel=="MCConvGaussPlusExp")) {
    for(int iht = 0; iht < an_effset.binning["annulus"].size()-1; ++iht) {
      double minannulus = min(fabs(an_effset.binning["annulus"][iht]), fabs(an_effset.binning["annulus"][iht+1]));
      double maxannulus = max(fabs(an_effset.binning["annulus"][iht]), fabs(an_effset.binning["annulus"][iht+1]));
      TString anbinstr = TString::Format("%2.1fTo%2.1f", minannulus, maxannulus);
      TString anbinlabel = TString::Format("annulus%d",iht);

      string catlabel = "{" + string(anbinlabel.Data()) + "}";

      string pdflabel = "MCConvGaussPlusErfExp_"+string(anbinlabel.Data());
      fitPDFs[pdflabel] = {
            "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
            "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
            "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+anbinstr.Data()+"_Pass\")",
            "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+anbinstr.Data()+"_Fail\")",
            "RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0])",
            "RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0])",
            "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
            "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[1.0]"};

      if(fitModel == "MCConvGaussPlusErfExp") {
        binToPDFmap.push_back(catlabel);
        binToPDFmap.push_back(pdflabel);
      }

      pdflabel = "MCConvGaussPlusExp_"+string(anbinlabel.Data());
      fitPDFs[pdflabel] = {
            "RooGaussian::signalResPass(mass, meanP[.0,-5.000,5.000],sigmaP[0.956,0.00,5.000])",
            "RooGaussian::signalResFail(mass, meanF[.0,-5.000,5.000],sigmaF[0.956,0.00,5.000])",
            "ZGeneratorLineShape::signalPhyPass(mass,\""+templateFile+"\", \"hMass_"+anbinstr.Data()+"_Pass\")",
            "ZGeneratorLineShape::signalPhyFail(mass,\""+templateFile+"\", \"hMass_"+anbinstr.Data()+"_Fail\")",
            "RooExponential::backgroundPass(mass, aPass[-0.1, -1., 0])",
            "RooExponential::backgroundFail(mass, aFail[-0.1, -1., 0])",
            "FCONV::signalPass(mass, signalPhyPass, signalResPass)",
            "FCONV::signalFail(mass, signalPhyFail, signalResFail)",
            "efficiency[0.5,0,1]",
            "signalFractionInPassing[0.9]"};

      if(fitModel == "MCConvGaussPlusExp") {
        binToPDFmap.push_back(catlabel);
        binToPDFmap.push_back(pdflabel);
      }
    }
  }

  vector<string> fileNames;

  // do MC efficiencies
  if(mcFile != "") {
    fileNames = {mcFile};
    TnPFitter mcTnPFitter(fileNames, dirName, "Probes", outFileMC, vars, effsets, binToPDFmap);
    mcTnPFitter.calculateEfficiencies();
  }

  // do data efficiencies
  if(dataFile != "") {
    fileNames = {dataFile};
    TnPFitter dataTnPFitter(fileNames, dirName, "Probes", outFileData, vars, effsets, binToPDFmap);
    dataTnPFitter.calculateEfficiencies();
  }

}
