#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  TString                         prefix;

  const ParamatrixMVA*  genMVA;
  const ParamatrixMVA*  recoMVA;

  TRandom3 * rand;

  //tree inputs
  size8 nVert                ;
  size8 splitResult          ;
  bool  isGen                ;
  size8 numSplits            ;
  float superJet_pt          ;
  float superJet_eta         ;
  float superJet_phi         ;
  float superJet_mass        ;
  float tau1                 ;
  float tau2                 ;
  float subJet_1_pt          ;
  float subJet_1_eta         ;
  float subJet_1_phi         ;
  float subJet_1_mass        ;
  float subJet_2_pt          ;
  float subJet_2_eta         ;
  float subJet_2_phi         ;
  float subJet_2_mass        ;
  float highest_peak         ;
  float lowest_peak          ;
  float minimum_value        ;
  float lowest_peak_location ;
  float highest_peak_location;
  float minimum_location     ;
  float oldDisc              ;
  bool  oldDisc_shouldSplit  ;

  float subjet_dr;


  Analyze(TString fname, string treeName, bool isMCTree) : BaseTreeAnalyzer(fname,treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter), rand(new TRandom3(1234))
  {
    TFile*              genFile    = TFile::Open("gen/pickymva_puppi_gen.root", "READ");
    genMVA = dynamic_cast<ParamatrixMVA*>(genFile->Get("picky_puppi_gen_0"));
    delete genFile;

    TFile*              recoFile    = TFile::Open("reco/pickymva_puppi_reco.root", "READ");
    recoMVA = dynamic_cast<ParamatrixMVA*>(recoFile->Get("picky_puppi_reco_0"));
    delete recoFile;

    assert(genMVA);
    assert(recoMVA);

//    TFile*              genFile    = TFile::Open("gen/pickymva_nopuppi_gen.root", "READ");
//    genMVA = dynamic_cast<ParamatrixMVA*>(genFile->Get("picky_nopuppi_gen_0"));
//    delete genFile;
//
//    TFile*              recoFile    = TFile::Open("reco/pickymva_nopuppi_reco.root", "READ");
//    recoMVA = dynamic_cast<ParamatrixMVA*>(recoFile->Get("picky_nopuppi_reco_0"));
//    delete recoFile;

    assert(genMVA);
    assert(recoMVA);


  };

  void loadVariables() override{
    reader.setBranchAddress("","nVert"                ,&nVert                );
    reader.setBranchAddress("","splitResult"          ,&splitResult          );
    reader.setBranchAddress("","isGen"                ,&isGen                );
    reader.setBranchAddress("","numSplits"            ,&numSplits            );
    reader.setBranchAddress("","superJet_pt"          ,&superJet_pt          );
    reader.setBranchAddress("","superJet_eta"         ,&superJet_eta         );
    reader.setBranchAddress("","superJet_phi"         ,&superJet_phi         );
    reader.setBranchAddress("","superJet_mass"        ,&superJet_mass        );
    reader.setBranchAddress("","tau1"                 ,&tau1                 );
    reader.setBranchAddress("","tau2"                 ,&tau2                 );
    reader.setBranchAddress("","subJet_1_pt"          ,&subJet_1_pt          );
    reader.setBranchAddress("","subJet_1_eta"         ,&subJet_1_eta         );
    reader.setBranchAddress("","subJet_1_phi"         ,&subJet_1_phi         );
    reader.setBranchAddress("","subJet_1_mass"        ,&subJet_1_mass        );
    reader.setBranchAddress("","subJet_2_pt"          ,&subJet_2_pt          );
    reader.setBranchAddress("","subJet_2_eta"         ,&subJet_2_eta         );
    reader.setBranchAddress("","subJet_2_phi"         ,&subJet_2_phi         );
    reader.setBranchAddress("","subJet_2_mass"        ,&subJet_2_mass        );
    reader.setBranchAddress("","highest_peak"         ,&highest_peak         );
    reader.setBranchAddress("","lowest_peak"          ,&lowest_peak          );
    reader.setBranchAddress("","minimum_value"        ,&minimum_value        );
    reader.setBranchAddress("","lowest_peak_location" ,&lowest_peak_location );
    reader.setBranchAddress("","highest_peak_location",&highest_peak_location);
    reader.setBranchAddress("","minimum_location"     ,&minimum_location     );
    reader.setBranchAddress("","oldDisc"              ,&oldDisc              );
    reader.setBranchAddress("","oldDisc_shouldSplit"  ,&oldDisc_shouldSplit  );
  }
  void processVariables() override{  subjet_dr = PhysicsUtilities::deltaR(subJet_1_eta,subJet_1_phi,subJet_2_eta,subJet_2_phi);}

  double evaluateMVA(const ParamatrixMVA* mvas){
	float filtered_pt = superJet_pt;
    float filtered_eta = superJet_eta;
    if(superJet_pt >= 300 && TMath::Abs(superJet_eta) >=2 && TMath::Abs(superJet_eta) < 3 ){
      filtered_pt = 305;
    } else if (superJet_pt >= 100 && TMath::Abs(superJet_eta) >= 3){
    	filtered_pt = 105;
    }
    filtered_eta = TMath::Abs(filtered_eta);

    static const int parIndex_superJet_pt = mvas->findAxis("superJet_pt");
    static const int parIndex_superJet_superJet_eta = mvas->findAxis("superJet_eta");
    static vector<double> mva_parameters(2,0);
    mva_parameters[parIndex_superJet_pt] = filtered_pt;
    mva_parameters[parIndex_superJet_superJet_eta] = filtered_eta;

    const Panvariate * mva = mvas->get(mva_parameters);
    if(mva == 0){
    	cout << mvas->getAxis("superJet_pt")->findBin(filtered_pt)<<" "<<mvas->getAxis("superJet_eta")->findBin(filtered_eta)<<endl;
    	cout << superJet_pt <<" "<<superJet_eta <<endl;
    	cout << filtered_pt <<" "<<filtered_eta <<endl;
    	assert(mva);
    }


    static const int index_superJet_mass         = mva->findVariable("superJet_mass"        );
    static const int index_tau1                  = mva->findVariable("tau1"                 );
    static const int index_tau2                  = mva->findVariable("tau2"                 );
    static const int index_highest_peak          = mva->findVariable("highest_peak"         );
    static const int index_lowest_peak           = mva->findVariable("lowest_peak"          );
    static const int index_minimum_value         = mva->findVariable("minimum_value"        );
    static const int index_lowest_peak_location  = mva->findVariable("lowest_peak_location" );
    static const int index_highest_peak_location = mva->findVariable("highest_peak_location");
    static const int index_minimum_location      = mva->findVariable("minimum_location"     );
    static const int index_subjet_dr             = mva->findVariable("subjet_dr"            );

    mva->setVariable(index_superJet_mass        ,superJet_mass        );
    mva->setVariable(index_tau1                 ,tau1                 );
    mva->setVariable(index_tau2                 ,tau2                 );
    mva->setVariable(index_highest_peak         ,highest_peak         );
    mva->setVariable(index_lowest_peak          ,lowest_peak          );
    mva->setVariable(index_minimum_value        ,minimum_value        );
    mva->setVariable(index_lowest_peak_location ,lowest_peak_location );
    mva->setVariable(index_highest_peak_location,highest_peak_location);
    mva->setVariable(index_minimum_location     ,minimum_location     );
    mva->setVariable(index_subjet_dr            ,subjet_dr            );

    return mva->evaluateMethod(0);
  }

  void makePlots(){
    double super_jet_abs_eta = TMath::Abs(superJet_eta);
    //filtering
	  if(superJet_pt < 60 && splitResult == 1  && TMath::Abs(superJet_eta) < 1.9 && rand->Uniform() < .9){
		  return;
	  }

	  if(superJet_pt >= 60 && superJet_pt < 100 && splitResult == 1  && TMath::Abs(superJet_eta) < 2 && rand->Uniform() < .8){
		  return ;
	  }
	  if(superJet_pt >= 100 && superJet_pt < 160 && splitResult == 1  && TMath::Abs(superJet_eta) < 2 && rand->Uniform() < .5){
		  return ;
	  }
	  if(superJet_pt < 60 && splitResult == 1  && TMath::Abs(superJet_eta) >= 2 && TMath::Abs(superJet_eta) < 3 && rand->Uniform() < .5){
		  return ;
	  }

    double newMVA = evaluateMVA(isGen ? genMVA : recoMVA);
    eventPlots.rewind();
    eventPlots("gen__",isGen > 0);
    eventPlots("reco__",isGen == 0);
    enum SplitResult  {PURE_TO_CLEAN, PURE_TO_SPLIT, MIXED_TO_BETTER, MIXED_TO_MOSTLYBETTER, MIXED_TO_CLEANER, MIXED_TO_MOSTLYCLEANER, MIXED_TO_WORSE, EMPTY };

    ++eventPlots;
    eventPlots("goodSplit__", splitResult == MIXED_TO_BETTER || splitResult == MIXED_TO_MOSTLYBETTER);
    eventPlots("badSplit__",  splitResult == PURE_TO_SPLIT );
    eventPlots("PURE_TO_CLEAN__", splitResult == PURE_TO_CLEAN);
    eventPlots("PURE_TO_SPLIT__", splitResult == PURE_TO_SPLIT);
    eventPlots("MIXED_TO_BETTER__", splitResult == MIXED_TO_BETTER);
    eventPlots("MIXED_TO_MOSTLYBETTER__", splitResult == MIXED_TO_MOSTLYBETTER);
    eventPlots("MIXED_TO_CLEANER__", splitResult == MIXED_TO_CLEANER);
    eventPlots("MIXED_TO_MOSTLYCLEANER__", splitResult == MIXED_TO_MOSTLYCLEANER);
    eventPlots("MIXED_TO_WORSE__", splitResult == MIXED_TO_WORSE);
    eventPlots.checkPoint();

    ++eventPlots;
    eventPlots("inclusive__",true );
    eventPlots("ee_eta_lt1p9__", super_jet_abs_eta < 1.9);
    eventPlots("ee_eta_eq1p9to2p9__", super_jet_abs_eta >= 1.9 && super_jet_abs_eta < 2.9);
    eventPlots("ee_eta_geq2p9__", super_jet_abs_eta >= 2.9 );


    ++eventPlots;
    eventPlots("inclusive__",true);
    eventPlots("ep_pt_eq20to50__"  ,superJet_pt >= 20 && superJet_pt < 50);
    eventPlots("ep_pt_eq50to100__" ,superJet_pt >= 50 && superJet_pt < 100);
    eventPlots("ep_pt_eq100to200__",superJet_pt >= 100 && superJet_pt < 200);
    eventPlots("ep_pt_eq200to400__",superJet_pt >= 200 && superJet_pt < 400);
    eventPlots("ep_pt_geq400__"    ,superJet_pt >= 400);

    eventPlots("emp_20_40__"   ,false);
    eventPlots("emp_40_60__"   ,false);
    eventPlots("emp_60_80__"   ,false);
    eventPlots("emp_80_100__"  ,false);
    eventPlots("emp_100_120__" ,false);
    eventPlots("emp_120_160__" ,false);
    eventPlots("emp_160_200__" ,false);
    eventPlots("emp_200_250__" ,false);
    eventPlots("emp_250_300__" ,false);
    eventPlots("emp_300_400__" ,false);
    eventPlots("emp_400_550__" ,false);
    eventPlots("emp_550_1000__",false);

    eventPlots.fill(superJet_mass        ,1, "jet_mass"             , ";M(Super jet)}"                  , 30, 0, 200 );
    eventPlots.fill(tau1                 ,1, "1_subjettiness"       , ";#tau_{1}"                       , 30, 0, 1   );
    eventPlots.fill(tau2                 ,1, "2_subjettiness"       , ";#tau_{2}"                       , 30, 0, 1   );
    eventPlots.fill(highest_peak         ,1, "highest_peak"         , ";Highest peak"                   , 60, 0, 2000);
    eventPlots.fill(lowest_peak          ,1, "lowest_peak"          , ";Lowest peak"                    , 30, 0, 500 );
    eventPlots.fill(minimum_value        ,1, "minimum_value"        , ";Minimum value"                  , 30, 0, 200 );
    eventPlots.fill(lowest_peak_location ,1, "lowest_peak_location" , ";Lowest peak location"           , 30, -1, 1  );
    eventPlots.fill(highest_peak_location,1, "highest_peak_location", ";Highest peak location"          , 30, -1, 1  );
    eventPlots.fill(minimum_location     ,1, "minimum_location"     , ";Minimum location"               , 30, -1, 1  );
    eventPlots.fill(subjet_dr            ,1, "sub12_dr"             , ";DR(subjet 1, subjet 2)"         , 30, 0, 1.2 );

    eventPlots.fill(oldDisc              ,1, "oldDisc"      , ";D_{splitting}"              , 200, -1, 1  );
    eventPlots.fill(newMVA               ,1, "newDisc"      , ";D_{splitting}"              , 200, -1, 1  );

    eventPlots.revert();

    float pt = superJet_pt;
    float eta = superJet_eta;
    if(pt >= 300 && TMath::Abs(superJet_eta) >= 1.9 ){
      pt = 305;
      eta = 2.0;
    } else if (superJet_pt >= 100 && TMath::Abs(superJet_eta) >= 2.9 ){
      eta = 2.0;
    }
    eta = TMath::Abs(eta);

    ++eventPlots;
    eventPlots("inclusive__",true );
    eventPlots("ee_eta_lt1p9__", eta < 1.9);
    eventPlots("ee_eta_eq1p9to2p9__", eta >= 1.9 && eta < 2.9);
    eventPlots("ee_eta_geq2p9__", eta >= 2.9 );

    ++eventPlots;
    eventPlots("inclusive__"       ,false);
    eventPlots("ep_pt_eq20to50__"  ,false);
    eventPlots("ep_pt_eq50to100__" ,false);
    eventPlots("ep_pt_eq100to200__",false);
    eventPlots("ep_pt_eq200to400__",false);
    eventPlots("ep_pt_geq400__"    ,false);

    eventPlots("emp_20_40__"   ,pt >= 20 && pt < 40);
    eventPlots("emp_40_60__"   ,pt >= 40 && pt < 60);
    eventPlots("emp_60_80__"   ,pt >= 60 && pt < 80);
    eventPlots("emp_80_100__"  ,pt >= 80 && pt < 100);
    eventPlots("emp_100_120__" ,pt >= 100 && pt < 120);
    eventPlots("emp_120_160__" ,pt >= 120 && pt < 160);
    eventPlots("emp_160_200__" ,pt >= 160 && pt < 200);
    eventPlots("emp_200_250__" ,pt >= 200 && pt < 250);
    eventPlots("emp_250_300__" ,pt >= 250 && pt < 300);
    eventPlots("emp_300_400__" ,pt >= 300 && pt < 400);
    eventPlots("emp_400_550__" ,pt >= 400 && pt < 550);
    eventPlots("emp_550_1000__",pt >= 550 && pt);

    eventPlots.fill(oldDisc              ,1, "oldDisc"      , ";D_{splitting}"              , 2000, -1, 1  );
    eventPlots.fill(newMVA               ,1, "newDisc"      , ";D_{splitting}"              , 2000, -1, 1  );

  }

  void runEvent(){
    if(nVert < 1) return;
    makePlots();

  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }



};


#endif



void testPickyMVA(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", bool isMCTree = true) {

  Analyze a(fname, treeName, isMCTree);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze(100000,-1);
  a.out(TString::Format("%s_plots.root",name.Data()));
}
