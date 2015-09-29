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
#include "AnalysisTools/Utilities/interface/PartonMatching.h"
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
//    load(PUPPIJETS,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC );
//    load(&trimmedJetReader,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC ,"trimmed");

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

  void plotJets(JetReader * jetReader, bool useGenJets, bool useRawPT, TString prefix){
    const double minPT = 20;
    const double maxETA = 2.4;

    vector<GenJetF*> genJets;
    if(useGenJets){
      for(auto& j : jetReader->genJets){
        if(TMath::Abs(j.eta()) >= maxETA) continue;
        if(j.pt() < minPT) continue;
        genJets.push_back(&j);
      }
    } else {
      for(auto& j : jetReader->recoJets){
        if(j.genJet() == 0)  continue;
        if(TMath::Abs(j.eta()) >= maxETA) continue;
        if( (useRawPT ? jetReader->jetptraw_->at(j.index()) : j.pt()) < minPT) continue;
        genJets.push_back(j.genJet());
      }
    }

	    //Get pointers to necessary variables
	    const vector<size16 >* genAssocPrtIndex = jetReader->genAssocPrtIndex_;
	    const vector<size16 >* genAssocJetIndex = jetReader->genAssocJetIndex_;
	    const vector<int8>* genAssocCon      = jetReader->genAssocCont_;

	    const vector<GenParticleF>* genParticles = &genParticleReader.genParticles;
	    const vector<float   >* hadronE      = genParticleReader.hade_;

	    PartonMatching::PartonEvent topDecayEvent(genAssocPrtIndex,genAssocJetIndex,genAssocCon,genParticles,hadronE,genJets);

	    for(const auto& t : topDecayEvent.topDecays){
	      eventPlots.rewind();
	      if(t.isLeptonic) continue;
	      double eta = TMath::Abs(t.top->eta());

	      eventPlots("eta_lt2p4__", eta < 2.4);
	      eventPlots("eta_eq2p4to4p6__",eta >= 2.4 && eta < 4.6  );

	      ++eventPlots;
	      eventPlots("all__",true);
	      eventPlots("good_partons__",t.diag  > PartonMatching::BAD_PARTON);
	      eventPlots("assoc_jets__"  ,t.diag  > PartonMatching::LOST_JET);
	      eventPlots("no_splits__"   ,t.diag  > PartonMatching::SPLIT_PARTONS);
	      eventPlots("good_top__"    ,t.diag  > PartonMatching::CONTAMINATION);


	      eventPlots.fill(t.top->pt(),1,"top_pt",";top p_{T}",50,0,1000,prefix);
	    }

	    //Reasons to fail
      for(const auto& t : topDecayEvent.topDecays){
        eventPlots.rewind();
        if(t.isLeptonic) continue;
        if(t.diag  <= PartonMatching::BAD_PARTON) continue;

        double eta = TMath::Abs(t.top->eta());

        eventPlots("eta_lt2p4__", eta < 2.4);
        eventPlots("eta_eq2p4to4p6__",false  );

        ++eventPlots;
        eventPlots("all__"         ,true);
        eventPlots("good_partons__",false);
        eventPlots("assoc_jets__"  ,false);
        eventPlots("no_splits__"   ,false);
        eventPlots("good_top__"    ,false);

        ++eventPlots;
        eventPlots("all__"              ,true);
        eventPlots("top_pt_lt250__"     ,t.top->pt() < 250);
        eventPlots("top_pt_eq250to550__",t.top->pt() >= 250 && t.top->pt() < 550);
        eventPlots("top_pt_geq550__"    ,t.top->pt() >= 550);

        eventPlots.checkPoint();
        for(const auto* p : t.hadronicPartons ){
          eventPlots.revert();
          ++eventPlots;
          eventPlots("noJet__"          ,p->diag == PartonMatching::NO_JET);
          eventPlots("splitJet__"       ,p->diag == PartonMatching::SPLIT_JETS);
          eventPlots("dirtyJet__"       ,p->diag == PartonMatching::DIRTY_JET);
          eventPlots("resolvedParton__" ,p->diag == PartonMatching::RESOLVED_PARTON);

          double pt = p->hadE;

          if(p->diag == PartonMatching::DIRTY_JET || p->diag == PartonMatching::RESOLVED_PARTON)
            pt = p->matchedJet->pt();

          eventPlots.fill(pt,1,"parton_pt",";parton p_{T}",100,0,1000,prefix);
        }
      }


	    int nB = 0;
	    int nL = 0;
	    int nS = 0;
	    int nC = 0;
	    vector<MomentumF> tops;
	    for(const auto& t : topDecayEvent.topDecays){
	    	if(t.isLeptonic) continue;
	    	if(TMath::Abs(t.top->eta()) >= 2.4 ) continue;
	    	if(t.diag  == PartonMatching::BAD_PARTON)nB++;
	    	if(t.diag  == PartonMatching::LOST_JET) nL++;
	    	if(t.diag  == PartonMatching::SPLIT_PARTONS) nS++;
	    	if(t.diag  == PartonMatching::CONTAMINATION) nC++;
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



	    //Plot # of good partons
	    int nGoodPartons      = 0;
	    int nNoJetPartons     = 0;
	    int nSplitPartons     = 0;
	    int nDirtyPartons     = 0;
	    int nResolovedPartons = 0;

	    for(const auto& t : topDecayEvent.topDecays){
	      for(const auto* p : t.hadronicPartons ){
	        if(p->diag > PartonMatching::DISPERSED_PARTON) nGoodPartons++;
	        if(p->diag == PartonMatching::NO_JET) nNoJetPartons++;
          if(p->diag == PartonMatching::SPLIT_JETS)nSplitPartons++;
          if(p->diag == PartonMatching::DIRTY_JET)nDirtyPartons++;
          if(p->diag == PartonMatching::RESOLVED_PARTON)nResolovedPartons++;
	      }
	    }
	    eventPlots.rewind();
      eventPlots("eta_lt2p4__", true);
      eventPlots("eta_eq2p4to4p6__",false  );
      eventPlots.fill(nGoodPartons     ,1,"nGoodPartons",";# of partons",16,-.5,15.5,prefix);
      eventPlots.fill(nNoJetPartons    ,1,"nNoJetPartons",";# of partons",16,-.5,15.5,prefix);
      eventPlots.fill(nSplitPartons    ,1,"nSplitPartons",";# of partons",16,-.5,15.5,prefix);
      eventPlots.fill(nDirtyPartons    ,1,"nDirtyPartons",";# of partons",16,-.5,15.5,prefix);
      eventPlots.fill(nResolovedPartons,1,"nResolovedPartons",";# of partons",16,-.5,15.5,prefix);

  }

  //kinematics dependence on efficieciency
  void plotKinematicsDependence(JetReader * jetReader, bool useGenJets, bool useCorrectedPT, TString prefix){

    const double minPT = 20;
    const double maxETA = 2.4;

    if(useCorrectedPT)
      for(auto& j : jetReader->recoJets){
      j.setP4(CylLorentzVectorF(correctedPickyPT(j.pt(),j.eta(),jetReader->jetarea_->at(j.index()),evtInfoReader.rho),
          j.eta(),j.phi(),j.mass()) );
      }

    vector<GenJetF*> genJets;
    vector<RecoJetF*> recoJets;
    if(useGenJets){
      for(auto& j : jetReader->genJets){
        if(TMath::Abs(j.eta()) >= maxETA) continue;
        if(j.pt() < minPT) continue;
        genJets.push_back(&j);
      }
    } else {
      for(auto& j : jetReader->recoJets){
        if(TMath::Abs(j.eta()) >= maxETA) continue;
        if(j.pt() < minPT) continue;
        recoJets.push_back(&j);
        if(j.genJet() == 0)  continue;
        genJets.push_back(j.genJet());
      }
    }

      //Get pointers to necessary variables
      const vector<size16 >* genAssocPrtIndex = jetReader->genAssocPrtIndex_;
      const vector<size16 >* genAssocJetIndex = jetReader->genAssocJetIndex_;
      const vector<int8>* genAssocCon      = jetReader->genAssocCont_;

      const vector<GenParticleF>* genParticles = &genParticleReader.genParticles;
      const vector<float   >* hadronE      = genParticleReader.hade_;

      PartonMatching::PartonEvent topDecayEvent(genAssocPrtIndex,genAssocJetIndex,genAssocCon,genParticles,hadronE,genJets);


    int nB = 0;
    int nL = 0;
    int nS = 0;
    int nC = 0;
    vector<MomentumF> tops;
    for(const auto& t : topDecayEvent.topDecays){
      if(t.isLeptonic) continue;
      if(TMath::Abs(t.top->eta()) >= 2.4 ) continue;
      if(t.diag  == PartonMatching::BAD_PARTON)nB++;
      if(t.diag  == PartonMatching::LOST_JET) nL++;
      if(t.diag  == PartonMatching::SPLIT_PARTONS) nS++;
      if(t.diag  == PartonMatching::CONTAMINATION || t.diag  == PartonMatching::MERGED_PARTONS) nC++;
      tops.push_back(t.top->p4());
    }

    if(tops.size() != 2) return;

    eventPlots.rewind();

    const int nj30 =  PhysicsUtilities::countObjects(ak4Reader.recoJets,30,2.4);
    const int pickynj30 =  PhysicsUtilities::countObjects(jetReader->recoJets,30,2.4);
    const int pickynj20 =  PhysicsUtilities::countObjects(jetReader->recoJets,20,2.4);

    eventPlots("inclusive__" ,true);
    eventPlots("ak5_nJ30_geq5__", nj30 >= 5);
    eventPlots("ak5_nJ30_geq6__"   ,nj30 >= 6 );
    eventPlots("picky_nJ30_geq5__", pickynj30 >= 5);
    eventPlots("picky_nJ30_geq6__" ,pickynj30 >= 6 );
    eventPlots("picky_nJ30_geq5__", pickynj20 >= 5);
    eventPlots("picky_nJ20_geq6__" ,pickynj20 >= 6 );

    eventPlots.checkPoint(0);
    if(useGenJets)
      for(auto* j: genJets){
        eventPlots.revert(0);
        ++eventPlots;
        eventPlots("",false);
        eventPlots("pt_eq20to30__",j->pt() >= 20 && j->pt() < 30);
        eventPlots("pt_eq30to50__",j->pt() >= 30 && j->pt() < 50);
        eventPlots("pt_eq50to100__",j->pt() >= 50 && j->pt() < 100);
        eventPlots("pt_geq100__"  ,j->pt() >= 100);
        eventPlots.fill(j->eta(),"jet_eta",";jet #eta",50,-2.5,2.5,prefix);
      }
    else
      for(auto* j: recoJets){
        eventPlots.revert(0);
        ++eventPlots;
        eventPlots("",false);
        eventPlots("pt_eq20to30__",j->pt() >= 20 && j->pt() < 30);
        eventPlots("pt_eq30to50__",j->pt() >= 30 && j->pt() < 50);
        eventPlots("pt_eq50to100__",j->pt() >= 50 && j->pt() < 100);
        eventPlots("pt_geq100__"  ,j->pt() >= 100);
        eventPlots.fill(j->eta(),"jet_eta",";jet #eta",50,-2.5,2.5,prefix);

        if(j->genJet() != 0 && j->genJet()->pt() > 0){
          eventPlots.revert(0);
          ++eventPlots;
          eventPlots("",false);
          eventPlots("pt_eq20to30__",j->genJet()->pt() >= 20 && j->genJet()->pt() < 30);
          eventPlots("pt_eq30to50__",j->genJet()->pt() >= 30 && j->genJet()->pt() < 50);
          eventPlots("pt_eq50to100__",j->genJet()->pt() >= 50 && j->genJet()->pt() < 100);
          eventPlots("pt_geq100__"  ,j->genJet()->pt() >= 100);
          eventPlots.fill(j->pt() / j->genJet()->pt(),"jet_res",";reco jet p_{T} / gen jet p_{T}",100,0,3,prefix);

        }
      }

    eventPlots.revert(0);
    ++eventPlots;
    eventPlots("",true);
    eventPlots("pt_eq20to30__" ,false);
    eventPlots("pt_eq30to50__" ,false);
    eventPlots("pt_eq50to100__",false);
    eventPlots("pt_geq100__"   ,false);

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

//  void testAssoc(){
////    if(!genParticleReader.hasOption(GenParticleReader::LOADPARTONDECAY) || !genParticleReader.hasOption(GenParticleReader::FILLOBJ) )
////      throw std::invalid_argument("TopDecayEvent::TopDecayEvent():  both objects and parton decay must be loaded in GenParticleReader!");
////
////    if(!ak4Reader.hasOption(JetReader::LOADTOPASSOC) || !ak4Reader.hasOption(JetReader::FILLOBJ) )
////      throw std::invalid_argument("TopDecayEvent::TopDecayEvent():  both objects and top assoc. must be loaded in JetReader!");
////
////    if(!ak4Reader.isLoaded() || !genParticleReader.isLoaded())
////      throw std::invalid_argument("TopDecayEvent::TopDecayEvent():  both JetReader and GenParticleReader must be loaded before doing this!");
//
//	  vector<GenJetF*> genJets;
//	    for(auto& j : ak4Reader.recoJets){
//	      if(j.genJet() == 0)  continue;
//	      if(TMath::Abs(j.eta()) < 2.4) continue;
//	      if(j.pt() < 20) continue;
//	      genJets.push_back(j.genJet());
//	    }
//
//
////    Get pointers to necessary variables
//    const vector<size16 >* genAssocPrtIndex = ak4Reader.genAssocPrtIndex_;
//    const vector<size16 >* genAssocJetIndex = ak4Reader.genAssocJetIndex_;
//    const vector<int8>* genAssocCon      = ak4Reader.genAssocCont_;
//
//    const vector<GenParticleF>* genParticles = &genParticleReader.genParticles;
//    const vector<float   >* hadronE      = genParticleReader.hade_;
//
////    ParticleInfo::printGenInfo(*genParticles,-1);
//
//    PartonMatching::TopDecayEvent topDecayEvent(genAssocPrtIndex,genAssocJetIndex,genAssocCon,genParticles,hadronE,genJets);
//
////    for(unsigned int iP = 0; iP < topDecayEvent.partons.size(); ++iP){
////      const PartonMatching::Parton& p = topDecayEvent.partons[iP];
////      ParticleInfo::printGenParticleInfo(p.parton,p.genIdx);
////      cout <<p.parton->energy() <<" "<< p.hadE << " :: ";
////      for(unsigned int iJ = 0; iJ < p.containment.size(); ++iJ)
////        cout <<"("<< p.containment[iJ].first <<","<< p.containment[iJ].second <<") ";
////      cout << endl;
////    }
//
////    for(const auto& t : topDecayEvent.topDecays){
////      cout << "<>"<<endl;
////      cout << *t.top;
////      cout << " "<< t.isLeptonic <<" --> " << t.diag << endl;
////
////      cout << t.b->genIdx <<" "<< *t.b->parton <<" --> "<< t.b->diag <<endl;
////      cout << *t.W<<endl;
////      cout << t.W_dau1->genIdx <<" "<< *t.W_dau1->parton <<" --> "<< t.W_dau1->diag <<endl;
////      cout << t.W_dau2->genIdx <<" "<< *t.W_dau2->parton <<" --> "<< t.W_dau2->diag <<endl;
////    }
////
//
//    for(const auto& t : topDecayEvent.topDecays){
//      eventPlots.rewind();
//      if(t.isLeptonic) continue;
//      double eta = TMath::Abs(t.top->eta());
//
//      eventPlots("eta_lt2p4__", eta < 2.4);
//      eventPlots("eta_eq2p4to4p6__",eta >= 2.4 && eta < 4.6  );
//      eventPlots("",false  );
//
//      ++eventPlots;
//      eventPlots("all__",true);
//      eventPlots("good_partons__",t.diag  > PartonMatching::BAD_PARTON);
//      eventPlots("assoc_jets__"  ,t.diag  > PartonMatching::LOST_JET);
//      eventPlots("no_splits__"   ,t.diag  > PartonMatching::SPLIT_PARTONS);
//      eventPlots("good_top__"    ,t.diag  > PartonMatching::CONTAMINATION);
//
//
//      eventPlots.fill(t.top->pt(),1,"top_pt","top p_{T}",50,0,800);
//    }
//  }




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
//	  plotJets(&ak4Reader,false,false,"ak4_");
//	  plotJets(&ak4Reader,true,false,"ak4Gen_");
//	  plotJets(&pickyJetReader,false,false,"picky_");
//	  plotJets(&pickyJetReader,true,false,"pickyGen_");
//	  plotJets(&trimmedJetReader,false,false,"trimmed_");
//	  plotJets(&trimmedJetReader,true,false,"trimmedGen_");

	  plotKinematicsDependence(&pickyJetReader,true,false,"gen_");
	  plotKinematicsDependence(&pickyJetReader,false,false,"reco__");
	  plotKinematicsDependence(&pickyJetReader,false,true,"puSub__");



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



void testTopAssoc(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", string outname = "testPlots.root", bool isMCTree = true) {

  Analyze a(fname, treeName, isMCTree);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze(100000,500000);
//  a.out(TString::Format("%s_plots.root",name.Data()));
  a.out(outname);
}
