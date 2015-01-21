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
#include "AnalysisTools/Utilities/interface/TopJetMatching.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

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
  JetReader                       trimmedJetReader;
  Analyze(TString fname, string treeName, bool isMCTree) : BaseTreeAnalyzer(fname,treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter)
  {
  };

  virtual void loadVariables(){
    load(EVTINFO);
    load(GENPARTICLES, GenParticleReader::FILLOBJ | GenParticleReader::LOADPARTONDECAY);
    load(AK4JETS,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC );
    load(PICKYJETS,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC );
    load(PUPPIJETS,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC );
    load(&trimmedJetReader,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC ,"trimmed");

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

  void plotJets(JetReader * jetReader, TString prefix){
	  vector<GenJetF*> genJets;
	    for(auto& j : jetReader->recoJets){
	      if(j.genJet() == 0)  continue;
	      if(TMath::Abs(j.eta()) >= 2.4) continue;
	      if(j.pt() < 20) continue;
	      genJets.push_back(j.genJet());
	    }

	    //Get pointers to necessary variables
	    const vector<size16 >* genAssocPrtIndex = jetReader->genAssocPrtIndex_;
	    const vector<size16 >* genAssocJetIndex = jetReader->genAssocJetIndex_;
	    const vector<int8>* genAssocCon      = jetReader->genAssocCont_;

	    const vector<GenParticleF>* genParticles = &genParticleReader.genParticles;
	    const vector<float   >* hadronE      = genParticleReader.hade_;

	    TopJetMatching::TopDecayEvent topDecayEvent(genAssocPrtIndex,genAssocJetIndex,genAssocCon,genParticles,hadronE,genJets);

	    for(const auto& t : topDecayEvent.topDecays){
	      eventPlots.rewind();
	      if(t.isLeptonic) continue;
	      double eta = TMath::Abs(t.top->eta());

	      eventPlots("eta_lt2p4__", eta < 2.4);
	      eventPlots("eta_eq2p4to4p6__",eta >= 2.4 && eta < 4.6  );

	      ++eventPlots;
	      eventPlots("all__",true);
	      eventPlots("good_partons__",t.diag  > TopJetMatching::BAD_PARTON);
	      eventPlots("assoc_jets__"  ,t.diag  > TopJetMatching::LOST_JET);
	      eventPlots("no_splits__"   ,t.diag  > TopJetMatching::SPLIT_PARTONS);
	      eventPlots("good_top__"    ,t.diag  > TopJetMatching::CONTAMINATION);


	      eventPlots.fill(t.top->pt(),1,"top_pt",";top p_{T}",50,0,1000,prefix);
	    }

	    int nB = 0;
	    int nL = 0;
	    int nS = 0;
	    int nC = 0;
	    vector<MomentumF> tops;
	    for(const auto& t : topDecayEvent.topDecays){
	    	if(t.isLeptonic) continue;
	    	if(TMath::Abs(t.top->eta()) >= 2.4 ) continue;
	    	if(t.diag  == TopJetMatching::BAD_PARTON)nB++;
	    	if(t.diag  == TopJetMatching::LOST_JET) nL++;
	    	if(t.diag  == TopJetMatching::SPLIT_PARTONS) nS++;
	    	if(t.diag  == TopJetMatching::CONTAMINATION) nC++;
	    	tops.push_back(t.top->p4());
	    }
	    if(tops.size() == 2){
	        eventPlots.rewind();
	        eventPlots("eta_lt2p4__", true);
	        eventPlots("eta_eq2p4to4p6__",false  );
	        ++eventPlots;
	        eventPlots("all__",true);
	        eventPlots("good_partons__",nB == 0);
	        eventPlots("assoc_jets__"  ,nB == 0 && nL == 0);
	        eventPlots("no_splits__"   ,nB == 0 && nL == 0 && nS == 0);
	        eventPlots("good_top__"    ,nB == 0 && nL == 0 &&nS == 0 &&nC == 0);

	        int lT = tops[0].pt() > tops[1].pt() ? 0 : 1;
	        int sT = tops[0].pt() > tops[1].pt() ? 1 : 0;

	        eventPlots.fill(tops[lT].pt() ,1,"lead_top_pt",";lead top 1 p_{T}",50,0,1000,prefix);
	        eventPlots.fill(tops[sT].pt() ,1,"subLead_top_pt",";sublead top p_{T}",50,0,1000,prefix);
	        eventPlots.fill((tops[sT].pt() + tops[lT].pt()) / 2  ,1,"avg_top_pt",";avg top p_{T}",50,0,1000,prefix);
	    }

  }

  void testAssoc(){
//    if(!genParticleReader.hasOption(GenParticleReader::LOADPARTONDECAY) || !genParticleReader.hasOption(GenParticleReader::FILLOBJ) )
//      throw std::invalid_argument("TopDecayEvent::TopDecayEvent():  both objects and parton decay must be loaded in GenParticleReader!");
//
//    if(!ak4Reader.hasOption(JetReader::LOADTOPASSOC) || !ak4Reader.hasOption(JetReader::FILLOBJ) )
//      throw std::invalid_argument("TopDecayEvent::TopDecayEvent():  both objects and top assoc. must be loaded in JetReader!");
//
//    if(!ak4Reader.isLoaded() || !genParticleReader.isLoaded())
//      throw std::invalid_argument("TopDecayEvent::TopDecayEvent():  both JetReader and GenParticleReader must be loaded before doing this!");

	  vector<GenJetF*> genJets;
	    for(auto& j : ak4Reader.recoJets){
	      if(j.genJet() == 0)  continue;
	      if(TMath::Abs(j.eta()) < 2.4) continue;
	      if(j.pt() < 20) continue;
	      genJets.push_back(j.genJet());
	    }


//    Get pointers to necessary variables
    const vector<size16 >* genAssocPrtIndex = ak4Reader.genAssocPrtIndex_;
    const vector<size16 >* genAssocJetIndex = ak4Reader.genAssocJetIndex_;
    const vector<int8>* genAssocCon      = ak4Reader.genAssocCont_;

    const vector<GenParticleF>* genParticles = &genParticleReader.genParticles;
    const vector<float   >* hadronE      = genParticleReader.hade_;

//    ParticleInfo::printGenInfo(*genParticles,-1);

    TopJetMatching::TopDecayEvent topDecayEvent(genAssocPrtIndex,genAssocJetIndex,genAssocCon,genParticles,hadronE,genJets);

//    for(unsigned int iP = 0; iP < topDecayEvent.partons.size(); ++iP){
//      const TopJetMatching::Parton& p = topDecayEvent.partons[iP];
//      ParticleInfo::printGenParticleInfo(p.parton,p.genIdx);
//      cout <<p.parton->energy() <<" "<< p.hadE << " :: ";
//      for(unsigned int iJ = 0; iJ < p.containment.size(); ++iJ)
//        cout <<"("<< p.containment[iJ].first <<","<< p.containment[iJ].second <<") ";
//      cout << endl;
//    }

//    for(const auto& t : topDecayEvent.topDecays){
//      cout << "<>"<<endl;
//      cout << *t.top;
//      cout << " "<< t.isLeptonic <<" --> " << t.diag << endl;
//
//      cout << t.b->genIdx <<" "<< *t.b->parton <<" --> "<< t.b->diag <<endl;
//      cout << *t.W<<endl;
//      cout << t.W_dau1->genIdx <<" "<< *t.W_dau1->parton <<" --> "<< t.W_dau1->diag <<endl;
//      cout << t.W_dau2->genIdx <<" "<< *t.W_dau2->parton <<" --> "<< t.W_dau2->diag <<endl;
//    }
//

    for(const auto& t : topDecayEvent.topDecays){
      eventPlots.rewind();
      if(t.isLeptonic) continue;
      double eta = TMath::Abs(t.top->eta());

      eventPlots("eta_lt2p4__", eta < 2.4);
      eventPlots("eta_eq2p4to4p6__",eta >= 2.4 && eta < 4.6  );
      eventPlots("",false  );

      ++eventPlots;
      eventPlots("all__",true);
      eventPlots("good_partons__",t.diag  > TopJetMatching::BAD_PARTON);
      eventPlots("assoc_jets__"  ,t.diag  > TopJetMatching::LOST_JET);
      eventPlots("no_splits__"   ,t.diag  > TopJetMatching::SPLIT_PARTONS);
      eventPlots("good_top__"    ,t.diag  > TopJetMatching::CONTAMINATION);


      eventPlots.fill(t.top->pt(),1,"top_pt","top p_{T}",50,0,800);
    }
  }




  void runEvent(){
//    cout <<"------------"<<endl;
//    for(unsigned int iP = 0; iP < genParticleReader.genParticles.size(); ++iP){
//      const GenParticleF& p = genParticleReader.genParticles[iP];
//      cout <<iP<<" " << p.status() <<" "<< p.pdgId()<<" "<< p.pt()<<" "<<p.energy()<< " "<<  genParticleReader.hade_->at(iP)<<endl;
//    }
//
//    for(unsigned int iJ = 0; iJ < ak4Reader.genJets.size(); ++iJ){
//      const GenJetF& j = ak4Reader.genJets[iJ];
//      cout <<iJ <<" " <<j.pt()<<" "<<j.energy() <<" "<<j.flavor()<<endl;
//    }
//
//    for(unsigned int iP = 0; iP < ak4Reader.genAssocPrtIndex_->size(); ++iP){
//      cout << (int)ak4Reader.genAssocPrtIndex_->at(iP)<<endl;
//    }
//
//    for(unsigned int iP = 0; iP < ak4Reader.genAssocCont_->size(); ++iP){
//      cout << (int)ak4Reader.genAssocCont_->at(iP) <<" "<<(int)ak4Reader.genAssocJetIndex_->at(iP)<<endl;
//    }


//    testAssoc();
	  plotJets(&ak4Reader,"ak4_");
	  plotJets(&pickyJetReader,"picky_");
	  plotJets(&puppiJetsReader,"puppi_");
	  plotJets(&trimmedJetReader,"trimmed_");


  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }

//  vector<GenParticleF*> goodParts;
//  vector<GenParticleF*> vetoParts;
//  vector<GenParticleF*> lepParts;

//    vector<GenJetF*> genJets;

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
