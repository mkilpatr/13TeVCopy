#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "PickyHelper.h"
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
  };

  virtual void loadVariables(){
    load(EVTINFO);
    load(GENPARTICLES, GenParticleReader::FILLOBJ | GenParticleReader::LOADPARTONDECAY);
    load(AK4JETS,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC );

  }
  virtual void processVariables(){
//    goodParts.clear();
//    vetoParts.clear();
//    lepParts.clear();
//
//    SplitVars::getParts(genParticleReader.genParticles.particles,goodParts,vetoParts,lepParts);
//
//    if(evtInfoReader.isLoaded()) {
//      run   = evtInfoReader.run;
//      lumi  = evtInfoReader.lumi;
//      event = evtInfoReader.event;
//      nPV   = evtInfoReader.nPV;
//      rho   = evtInfoReader.rho;
//      met   = &evtInfoReader.met;
//    }
  }




  void runEvent(){
    cout <<"------------"<<endl;
    for(unsigned int iP = 0; iP < genParticleReader.genParticles.size(); ++iP){
      const GenParticleF& p = genParticleReader.genParticles[iP];
      cout <<iP<<" " << p.status() <<" "<< p.pdgId()<<" "<< p.pt()<<" "<<p.energy()<< " "<<  genParticleReader.hade_->at(iP)<<endl;
    }

    for(unsigned int iJ = 0; iJ < ak4Reader.genJets.size(); ++iJ){
      const GenJetF& j = ak4Reader.genJets[iJ];
      cout <<iJ <<" " <<j.pt()<<" "<<j.energy() <<" "<<j.flavor()<<endl;
    }

    for(unsigned int iP = 0; iP < ak4Reader.genAssocPrtIndex_->size(); ++iP){
      cout << (int)ak4Reader.genAssocPrtIndex_->at(iP)<<endl;
    }

    for(unsigned int iP = 0; iP < ak4Reader.genAssocCont_->size(); ++iP){
      cout << (int)ak4Reader.genAssocCont_->at(iP) <<" "<<(int)ak4Reader.genAssocJetIndex_->at(iP)<<endl;
    }




  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }

//  vector<GenParticleF*> goodParts;
//  vector<GenParticleF*> vetoParts;
//  vector<GenParticleF*> lepParts;

};


#endif



void testTopAssoc(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", bool isMCTree = true) {

  Analyze a(fname, treeName, isMCTree);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze(1000);
  a.out(TString::Format("%s_plots.root",name.Data()));
}
