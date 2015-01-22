#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "QGHelper.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

#include "AnalysisTools/TreeReader/interface/Defaults.h"

using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*      plotter;
  Polybook       eventPlots;
  TString        prefix;
  bool           usePuppi;
  ParamatrixMVA* paramGen;
  ParamatrixMVA* paramRec;

  Analyze(TString fname, string treeName, bool isMCTree, bool usePuppi) : BaseTreeAnalyzer(fname, treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter), usePuppi(usePuppi)
  {
	if (usePuppi) cout <<     "using PUPPI" << endl;
	else          cout << "NOT using PUPPI" << endl;

    TFile* genFile = TFile::Open("QGDisc_gen_ak4.root",   "READ");
    paramGen = dynamic_cast<ParamatrixMVA*>(genFile->Get("QG_0"));
    delete genFile;

	string recoFileName = "";
	if (usePuppi) recoFileName = "QGDisc_reco_puppi.root";
	else          recoFileName = "QGDisc_reco_ak4.root";
    TFile* recoFile = TFile::Open(recoFileName.c_str(), "READ");
    paramRec = dynamic_cast<ParamatrixMVA*>(recoFile->Get("QG_0"));
    delete recoFile;

  }; // Analyze()


  double getGenMVA(const subJet& jet){
	genMVA = paramGen->get(jet.jet->pt(),TMath::Abs(jet.jet->eta()));
    static const int gen_ak4pfchs_genjet_ptD     = genMVA->findVariable("ak4pfchs_genjet_ptD"    );
    static const int gen_ak4pfchs_genjet_axis1   = genMVA->findVariable("ak4pfchs_genjet_axis1"  );
    static const int gen_ak4pfchs_genjet_axis2   = genMVA->findVariable("ak4pfchs_genjet_axis2"  );
    static const int gen_ak4pfchs_genjet_jetMult = genMVA->findVariable("ak4pfchs_genjet_jetMult");
    genMVA->setVariable(gen_ak4pfchs_genjet_ptD    ,jet.ptD);
    genMVA->setVariable(gen_ak4pfchs_genjet_axis1  ,jet.axis1);
    genMVA->setVariable(gen_ak4pfchs_genjet_axis2  ,jet.axis2);
    genMVA->setVariable(gen_ak4pfchs_genjet_jetMult,jet.mult);
    return genMVA->evaluateMethod(0);
  } //getGenMVA()

  double getRecoMVA(const subJet& jet){
	recoMVA = paramRec->get(jet.jet->pt(),TMath::Abs(jet.jet->eta()));
	if (usePuppi) {
		static const int reco_ak4pfchs_jet_ptD     = recoMVA->findVariable("ak4pfpuppi_jet_ptD"    );
		static const int reco_ak4pfchs_jet_axis1   = recoMVA->findVariable("ak4pfpuppi_jet_axis1"  );
		static const int reco_ak4pfchs_jet_axis2   = recoMVA->findVariable("ak4pfpuppi_jet_axis2"  );
		static const int reco_ak4pfchs_jet_jetMult = recoMVA->findVariable("ak4pfpuppi_jet_jetMult");
		recoMVA->setVariable(reco_ak4pfchs_jet_ptD    ,jet.ptD  );
		recoMVA->setVariable(reco_ak4pfchs_jet_axis1  ,jet.axis1);
		recoMVA->setVariable(reco_ak4pfchs_jet_axis2  ,jet.axis2);
		recoMVA->setVariable(reco_ak4pfchs_jet_jetMult,jet.mult );
	} // usePuppi
	else {
		static const int reco_ak4pfchs_jet_ptD     = recoMVA->findVariable("ak4pfchs_jet_ptD"    );
		static const int reco_ak4pfchs_jet_axis1   = recoMVA->findVariable("ak4pfchs_jet_axis1"  );
		static const int reco_ak4pfchs_jet_axis2   = recoMVA->findVariable("ak4pfchs_jet_axis2"  );
		static const int reco_ak4pfchs_jet_jetMult = recoMVA->findVariable("ak4pfchs_jet_jetMult");
		recoMVA->setVariable(reco_ak4pfchs_jet_ptD    ,jet.ptD  );
		recoMVA->setVariable(reco_ak4pfchs_jet_axis1  ,jet.axis1);
		recoMVA->setVariable(reco_ak4pfchs_jet_axis2  ,jet.axis2);
		recoMVA->setVariable(reco_ak4pfchs_jet_jetMult,jet.mult );
	} // !usePuppi
    return recoMVA->evaluateMethod(0);
  } // getRecoMVA()

  virtual void loadVariables(){
    load(EVTINFO);
    if (usePuppi) load(AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ, defaults::BRANCH_PUPPIJETS );
    else          load(AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ, defaults::BRANCH_AK4JETS   );
    load(GENPARTICLES);
  } // loadVariables()

  std::vector<subJet> genJets ;
  std::vector<subJet> recoJets ;

  virtual void processVariables(){
    procesSubjets(recoJets,genJets,this);
  } // processVariables()

  void makePlots(std::vector<subJet>& jets, bool isReco){
    categorize(jets,genParts,isReco);
    eventPlots.rewind();
    eventPlots("",true);

    ++eventPlots;
    eventPlots( "gen___", !isReco );
    eventPlots( "reco__",  isReco );

    ++eventPlots;
    eventPlots( "npv_all__",  true                );
    eventPlots( "npv_low___", isReco && nPV <= 15 );
    eventPlots( "npv_hgh___", isReco && nPV >= 22 );

    eventPlots.checkPoint(1);

    for(const auto& j : jets){
      eventPlots.revert(1);

      ++eventPlots;
      eventPlots( "pt_all__", true );
      eventPlots( "pt_low__", j.jet->pt() >  30 and j.jet->pt() <  50 );
      eventPlots( "pt_mid__", j.jet->pt() >  50 and j.jet->pt() < 100 );
      eventPlots( "pt_hgh__", j.jet->pt() > 100                       );

      ++eventPlots;
      eventPlots( "eta_all__", true );
      eventPlots( "eta_trk__",                        j.jet->eta() <  2.4 );
      eventPlots( "eta_trn__", j.jet->eta() > 2.4 and j.jet->eta() <  3.0 );
      eventPlots( "eta_out__", j.jet->eta() > 3.0 and j.jet->eta() <  4.7 );

      ++eventPlots;
      //eventPlots( "nonPU__",         j.type != PU );
      eventPlots( "q__", j.type == Q  );
      eventPlots( "g__", j.type == G  );
      eventPlots( "b__", j.type == B  );
      eventPlots( "p__", j.type == PU );
      eventPlots( "U__", j.type == U  );
      //eventPlots( "c_k__", j.type == C  );

      eventPlots.fill( j.jet->pt() , 1, "_pt"   , ";p_{t}"              , 100,   0,  1000   );
      eventPlots.fill( j.jet->eta(), 1, "_eta"  , ";eta"                , 200,  -5,     5   );
      eventPlots.fill( j.ptD       , 1, "ptD"   , ";p_{t}(D)"           ,  50,   0,     1   );
      eventPlots.fill( j.axis1     , 1, "axis1" , ";axis_{1}"           ,  50,   0,     0.4 );
      eventPlots.fill( j.axis2     , 1, "axis2" , ";axis_{2}"           ,  50,   0,     0.4 );
      eventPlots.fill( j.mult      , 1, "mult"  , ";# of part."         , 100, -0.5,   99.5 );

      if(isReco) eventPlots.fill( getRecoMVA(j), 1, "_disc", ";Q/G disc", 50, -1, 1  );
      else       eventPlots.fill( getGenMVA(j),  1, "_disc", ";Q/G disc", 50, -1, 1  );

    } // jets

  } // makePlots()


  void runEvent(){
    if(nPV < 1) return;
    makePlots(genJets,false);
    makePlots(recoJets,true);
  } // runEvent()

  void out(TString outputPath){
    plotter->write(outputPath);
  } // out()


  const Panvariate* genMVA;
  const Panvariate* recoMVA;

}; // BaseTreeAnalyzer


#endif



void processQGInvestigations(string fname = "evttree_lowStat_QCD_test.root", string fnameOut = "out", string treeName = "TestAnalyzer/Events", bool isMCTree = true, bool usePuppi = true) {
  Analyze a(fname, treeName, isMCTree, usePuppi);
  a.prefix  = fnameOut;
  a.prefix += "_";
  a.analyze();
  if (usePuppi) fnameOut += "_puppi.root";
  else          fnameOut += "_ak4.root";
  a.out(fnameOut);
} // processQGInvestigations()
