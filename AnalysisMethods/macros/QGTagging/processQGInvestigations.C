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

using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  TString                         prefix;

  Analyze(TString fname, string treeName, bool isMCTree) : BaseTreeAnalyzer(fname,treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter)
  {
    ParamatrixMVA* temp;
    TFile*              genFile    = TFile::Open("QGDisc_gen.root", "READ");
    temp = dynamic_cast<ParamatrixMVA*>(genFile->Get("QG_0"));
    genMVA = temp->getOne();
    delete genFile;

    cout << genMVA << endl;

    TFile*              recoFile    = TFile::Open("QGDisc_reco.root", "READ");
    temp = dynamic_cast<ParamatrixMVA*>(recoFile->Get("QG_0"));
    recoMVA = temp->getOne();
    temp = dynamic_cast<ParamatrixMVA*>(recoFile->Get("QG_1"));
    recoMVAWithPU = temp->getOne();
    delete recoFile;

    cout << genMVA <<" "<<recoMVA<<" "<<recoMVAWithPU << endl;

  };


  double getGenMVA(const subJet& jet){
    static const int gen_ak4pfchs_genjet_ptD     = genMVA->findVariable("ak4pfchs_genjet_ptD"    );
    static const int gen_ak4pfchs_genjet_axis1   = genMVA->findVariable("ak4pfchs_genjet_axis1"  );
    static const int gen_ak4pfchs_genjet_axis2   = genMVA->findVariable("ak4pfchs_genjet_axis2"  );
    static const int gen_ak4pfchs_genjet_jetMult = genMVA->findVariable("ak4pfchs_genjet_jetMult");
    static const int gen_ak4pfchs_genjet_pt      = genMVA->findVariable("ak4pfchs_genjet_pt"     );
    static const int gen_ak4pfchs_genjet_eta     = genMVA->findVariable("ak4pfchs_genjet_eta"    );
    genMVA->setVariable(gen_ak4pfchs_genjet_ptD    ,jet.ptD);
    genMVA->setVariable(gen_ak4pfchs_genjet_axis1  ,jet.axis1);
    genMVA->setVariable(gen_ak4pfchs_genjet_axis2  ,jet.axis2);
    genMVA->setVariable(gen_ak4pfchs_genjet_jetMult,jet.mult);
    genMVA->setVariable(gen_ak4pfchs_genjet_pt     ,jet.jet->pt());
    genMVA->setVariable(gen_ak4pfchs_genjet_eta    ,TMath::Abs(jet.jet->eta()));
    return genMVA->evaluateMethod(0);
  }

  double getRecoMVA(const subJet& jet){
    static const int reco_ak4pfchs_jet_ptD     = recoMVA->findVariable("ak4pfchs_jet_ptD"    );
    static const int reco_ak4pfchs_jet_axis1   = recoMVA->findVariable("ak4pfchs_jet_axis1"  );
    static const int reco_ak4pfchs_jet_axis2   = recoMVA->findVariable("ak4pfchs_jet_axis2"  );
    static const int reco_ak4pfchs_jet_jetMult = recoMVA->findVariable("ak4pfchs_jet_jetMult");
    static const int reco_ak4pfchs_jet_pt      = recoMVA->findVariable("ak4pfchs_jet_pt"     );
    static const int reco_ak4pfchs_jet_eta     = recoMVA->findVariable("ak4pfchs_jet_eta"    );
    recoMVA->setVariable(reco_ak4pfchs_jet_ptD    ,jet.ptD);
    recoMVA->setVariable(reco_ak4pfchs_jet_axis1  ,jet.axis1);
    recoMVA->setVariable(reco_ak4pfchs_jet_axis2  ,jet.axis2);
    recoMVA->setVariable(reco_ak4pfchs_jet_jetMult,jet.mult);
    recoMVA->setVariable(reco_ak4pfchs_jet_pt     ,jet.jet->pt());
    recoMVA->setVariable(reco_ak4pfchs_jet_eta    ,TMath::Abs(jet.jet->eta()));
    return recoMVA->evaluateMethod(0);
  }

  double getRecoPUMVA(const subJet& jet){
    static const int recoPU_ak4pfchs_jet_ptD     = recoMVAWithPU->findVariable("ak4pfchs_jet_ptD"    );
    static const int recoPU_ak4pfchs_jet_axis1   = recoMVAWithPU->findVariable("ak4pfchs_jet_axis1"  );
    static const int recoPU_ak4pfchs_jet_axis2   = recoMVAWithPU->findVariable("ak4pfchs_jet_axis2"  );
    static const int recoPU_ak4pfchs_jet_jetMult = recoMVAWithPU->findVariable("ak4pfchs_jet_jetMult");
    static const int recoPU_ak4pfchs_jet_pt      = recoMVAWithPU->findVariable("ak4pfchs_jet_pt"     );
    static const int recoPU_ak4pfchs_jet_eta     = recoMVAWithPU->findVariable("ak4pfchs_jet_eta"    );
    static const int recoPU_rho                  = recoMVAWithPU->findVariable("rho"    );
    static const int recoPU_npv                  = recoMVAWithPU->findVariable("npv"    );
    recoMVAWithPU->setVariable(recoPU_ak4pfchs_jet_ptD    ,jet.ptD);
    recoMVAWithPU->setVariable(recoPU_ak4pfchs_jet_axis1  ,jet.axis1);
    recoMVAWithPU->setVariable(recoPU_ak4pfchs_jet_axis2  ,jet.axis2);
    recoMVAWithPU->setVariable(recoPU_ak4pfchs_jet_jetMult,jet.mult);
    recoMVAWithPU->setVariable(recoPU_ak4pfchs_jet_pt     ,jet.jet->pt());
    recoMVAWithPU->setVariable(recoPU_ak4pfchs_jet_eta    ,TMath::Abs(jet.jet->eta()));
    recoMVAWithPU->setVariable(recoPU_rho                 ,rho);
    recoMVAWithPU->setVariable(recoPU_npv                 ,nPV);
    return recoMVAWithPU->evaluateMethod(0);
  }

  virtual void loadVariables(){
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(GENPARTICLES);
  }

    std::vector<subJet> genJets ;
    std::vector<subJet> recoJets ;

  virtual void processVariables(){
    procesSubjets(recoJets,genJets,this);
  }
  void makePlots(std::vector<subJet>& jets, bool isReco){
    categorize(jets,genParts,isReco);
    eventPlots.rewind();
    eventPlots("",true);

    eventPlots.checkPoint(0);
    if(isReco) eventPlots.fill(nPV    ,1,"nPV"     ,";# of prim. vert."           ,40,-0.5,39.5);
    eventPlots.revert(0);

    ++eventPlots;
    eventPlots("gen__",!isReco);
    eventPlots("reco__",isReco);

    ++eventPlots;
    eventPlots("none__", true);
    eventPlots("npv_nPV_incl__"    , !isReco);
    eventPlots("npv_nPV_lt15__"    , isReco && nPV < 15);
    eventPlots("npv_nPV_eq15to22__", isReco && nPV >= 15 && nPV <22);
    eventPlots("npv_nPV_geq22__"   , isReco && nPV >= 22);

    eventPlots.checkPoint(1);

    for(const auto& j : jets){
      eventPlots.revert(1);
      ++eventPlots;

      eventPlots("nonPU__", j.type != PU);
      eventPlots("def_uds_quark__", j.type == Q);
      eventPlots("def_gluon__", j.type == G);
      eventPlots("def_b_quark__", j.type == B);
      eventPlots("def_pu_jet__", j.type == PU);
      eventPlots("def_undefined__", j.type == U);
      eventPlots("def_c_quark__", j.type == C);

      eventPlots.fill(j.jet->pt()  ,1,"pt"     ,";p_{t}"           ,100,0,1000);
      eventPlots.fill(j.ptD     ,1,"ptD"     ,";p_{t}(D)"           ,50,0,1);
      eventPlots.fill(j.axis1   ,1,"axis1"   ,";axis_{1}"           ,50,0,.4);
      eventPlots.fill(j.axis2   ,1,"axis2"   ,";axis_{2}"           ,50,0,.4);
      eventPlots.fill(j.mult    ,1,"mult"    ,";# of part."         ,100,-0.5,99.5);
      eventPlots.fill(j.nearDR  ,1,"nearDR"  ,";#DeltaR(jet,parton)",50,0,.4);

      if(isReco){
        eventPlots.fill(j.betaStar,1,"betaStar",";betaStar"           ,50, 0,.3);
        eventPlots.fill(j.betaStar / (.2 * TMath::Log(float(nPV) - .67)),1,"betaStar_o_cut",";betaStar/ln(nPV - .67)"           ,50, 0,.3);
        eventPlots.fill(j.qgl     ,1,"qgl"     ,";Q/G likili"         ,50,0,1);
        eventPlots.fill(j.csv     ,1,"csv"     ,";D_{b}(csv)"         ,50,0,1);
        eventPlots.fill(getRecoMVA(j)     ,1,"newQgl"     ,";Q/G likili"         ,50,-1,1);
        eventPlots.fill(getRecoPUMVA(j)     ,1,"newQglwPU"     ,";Q/G likili"         ,50,-1,1);
      }
      else {
        eventPlots.fill(getGenMVA(j)     ,1,"newQgl"     ,";Q/G likili"         ,50,-1,1);
      }

    }


  }

  void runEvent(){
    if(nPV < 1) return;

    makePlots(genJets,false);
    makePlots(recoJets,true);

  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }


  const Panvariate*  genMVA;
  const Panvariate*  recoMVA;
  const Panvariate*  recoMVAWithPU;
};


#endif



void processQGInvestigations(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", bool isMCTree = true) {

  Analyze a(fname, treeName, isMCTree);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze();
  a.out(TString::Format("%s_plots.root",name.Data()));
}
