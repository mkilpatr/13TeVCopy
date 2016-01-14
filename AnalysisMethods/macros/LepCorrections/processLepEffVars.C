
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/PlotUtils/interface/EffPlotTools.hh"

using namespace ucsbsusy;
class Analyzer : public BaseTreeAnalyzer {

  public :
  Analyzer(TString fileName, TString treeName, bool isMCTree, cfgSet::ConfigSet *pars, double xSec, TString sname, TString outputdir, double randSeed) :
    BaseTreeAnalyzer(fileName, treeName, isMCTree, pars)
  {
    //loadPlots(); // initialize plots
	sampleName = sname;
	gSystem->mkdir(outputdir,true);
	fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
	fout->cd();
    outtree = new TTree("events","analysis tree");
    outtree->Branch( "passDiJetmet"		 	, &passDiJetmet		    ,			  "passDiJetmet/I");
    outtree->Branch( "npv"        		 	, &npv        			,         	  "npv/I" );
    outtree->Branch( "nbjets"        		, &nbjets        		,         	  "nbjets/I" );
    outtree->Branch( "njets60"           	, &nJets60    			,             "njets60/I" );
    outtree->Branch( "nvetotau"          	, &nVetoTau      		,             "nvetotau/I" );
    outtree->Branch( "nvetolep "         	, &nVetoLep         	,             "nVetoLep/F" );
    outtree->Branch( "nSelectedMuons"    	, &nSelectedMuons       ,             "nSelectedMuons/I" );
    outtree->Branch( "nSelectedElectrons"	, &nSelectedElectrons	,             "nSelectedElectrons/I" );
    outtree->Branch( "ncttstd"           	, &NCTTstd          	,             "ncttstd/I" );
    outtree->Branch( "njets	"           	, &njets	          	,             "njets/I" );
    outtree->Branch( "met"               	, &ptMet            	,             "met/F" );
    outtree->Branch( "mtcsv12met"        	, &mtcsv12MET         	,             "mtcsv12MET/F" );
    outtree->Branch( "leadingCsv"        	, &leadingCsv         	,             "leadingCsv/F" );
    outtree->Branch( "dphij12met"        	, &dPhiMETJ12         	,             "dphij12met/F" );
    outtree->Branch( "dphij3met"         	, &dPhiMETJ3         	,             "dphij3met/F" );
    outtree->Branch( "nGoodEle"          	, &nGoodEle      		,             "nGoodEle/I" );
    outtree->Branch( "nGoodEleAcc"       	, &nGoodEleAcc      	,             "nGoodEleAcc/I" );
    outtree->Branch( "nPromptEle"        	, &nPromptEle      	    ,             "nPromptEle/I" );
    outtree->Branch( "nPromptEleAcc"     	, &nPromptEleAcc      	,             "nPromptEleAcc/I" );
    outtree->Branch( "nGoodMu"           	, &nGoodMu      		,             "nGoodMu/I" );
    outtree->Branch( "nGoodMuAcc"        	, &nGoodMuAcc      	    ,             "nGoodMuAcc/I" );
    outtree->Branch( "nPromptMu"         	, &nPromptMu      		,             "nPromptMu/I" );
    outtree->Branch( "nPromptMuAcc"      	, &nPromptMuAcc      	,             "nPromptMuAcc/I" );
    outtree->Branch( "nPromptTau"        	, &nPromptTau      	    ,             "nPromptTau/I" );
    outtree->Branch( "nPromptTauAcc"     	, &nPromptTauAcc      	,             "nPromptTauAcc/I" );
    outtree->Branch( "nGoodLep"          	, &nGoodLep      		,             "nGoodLep/I" );
    outtree->Branch( "nGoodLepAcc"       	, &nGoodLepAcc      	,             "nGoodLepAcc/I" );
    outtree->Branch( "nPromptTau1Prong"  	, &nPromptTau1Prong     ,             "nPromptTau1Prong/I" );
    outtree->Branch( "nPromptTauAcc1Prong"	, &nPromptTauAcc1Prong  ,             "nPromptTauAcc1Prong/I" );
    outtree->Branch( "nPromptTau3Prong"     , &nPromptTau3Prong     ,             "nPromptTau3Prong/I" );
    outtree->Branch( "nPromptTauAcc3Prong"  , &nPromptTauAcc3Prong  ,             "nPromptTauAcc3Prong/I" );
    outtree->Branch( "nPromptLep"           , &nPromptLep      	    ,             "nPromptLep/I" );
    outtree->Branch( "nPromptLepAcc"        , &nPromptLepAcc      	,             "nPromptLepAcc/I" );
    outtree->Branch( "htmlAlongHardestEle"	, &htmlAlongHardestEle	, 			  "htmlAlongHardestEle/F" );
    outtree->Branch( "htmlAlongHardestMu"	, &htmlAlongHardestMu	, 			  "htmlAlongHardestMu/F" );
    outtree->Branch( "htmlAlongHardestTauD"	, &htmlAlongHardestTauD	, 			  "htmlAlongHardestTauD/F" );
    outtree->Branch( "hardestGenElePt"			, &hardestGenElePt		, 			  "hardestGenElePt/F" );
    outtree->Branch( "hardestGenMuPt"	, &hardestGenMuPt		, 			  "hardestGenMuPt/F" );
    outtree->Branch( "hardestGenTauDPt"	, &hardestGenTauDPt		, 			  "hardestGenTauDPt/F" );
    outtree->Branch( "hardestGenEleEta"	, &hardestGenEleEta		, 			  "hardestGenEleEta/F" );
    outtree->Branch( "hardestGenMuEta"	, &hardestGenMuEta		, 			  "hardestGenMuEta/F" );
    outtree->Branch( "hardestGenTauEtaD"	, &hardestGenTauEtaD	, 			  "hardestGenTauEtaD/F" );
    outtree->Branch( "weight"			    , &scaleFactor			, 			  "weight/F" );
    outtree->Branch( "mtCandVals"			, &mtCandVals		    );
    outtree->Branch( "ptCandVals"			, &ptCandVals		    );
    outtree->Branch( "etaCandVals"			, &etaCandVals		    );
    outtree->Branch( "discCandVals"			, &discCandVals		    );
  } // Analyzer()

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  } // ~Analyzer()

  TFile *fout    ;
  TTree *outtree ;
  TString sampleName;
  const double  metcut_    = 200.0 ;
  const int     minNJets_  =   3   ;
  const int     minNBjets_ =   1   ;
  int		    passDiJetmet       ;
  int	 		nJets60			   ;
  int 			npv				   ;
  int			nbjets			   ;
  int 			nVetoTau		   ;
  int 			nVetoLep		   ;
  int  			nSelectedMuons     ;
  int   		nSelectedElectrons ;
  int		    NCTTstd		   	   ;
  int			nGoodEle 		   ;
  int			nGoodEleAcc		   ;
  int 			nPromptEle 		   ;
  int 			nPromptEleAcc 	   ;
  int	 		nGoodMu 		   ;
  int 			nGoodMuAcc  	   ;
  int 			nPromptMu  		   ;
  int 			nPromptMuAcc  	   ;
  int 			nPromptTau 		   ;
  int 			nPromptTauAcc      ;
  int 			nGoodLep 		   ;
  int			nGoodLepAcc 	   ;
  int 			nPromptLep 		   ;
  int 			nPromptLepAcc 	   ;
  int	 	 	nPromptTau1Prong   ;
  int			nPromptTauAcc1Prong;
  int	 	 	nPromptTau3Prong   ;
  int			nPromptTauAcc3Prong;
  float 		leadingCsv		   ;
  float 		hardestGenElePt	   ;
  float 		hardestGenMuPt	   ;
  float 		hardestGenTauDPt   ;
  float 		hardestGenEleEta   ;
  float 		hardestGenMuEta	   ;
  float 		hardestGenTauEtaD  ;
  float 		htmlAlongHardestEle;
  float 		htmlAlongHardestMu ;
  float 		htmlAlongHardestTauD;
  float 		scaleFactor		   ;
  float			ptMet 			   ;
  float			dPhiMETJ12		   ;
  float			dPhiMETJ3		   ;
  float  		mtcsv12MET		   ;
  int	 		njets			   ;
  std::vector<GenParticleF*> genElectrons;	  std::vector<GenParticleF*> genMuons;	   std::vector<GenParticleF*> genTaus;
  std::vector<LeptonF*> selectedElectrons;	  std::vector<LeptonF*> selectedMuons;
  std::vector<const  GenParticleF*> taudecayprods;
  std::vector<float>				mtCandVals;  std::vector<float>				ptCandVals;
  std::vector<float>				etaCandVals;  std::vector<float>			discCandVals;

  void loadVariables();
  void runEvent();
  void rankedByCSV(vector<RecoJetF*> inJets,vector<RecoJetF*>& outJets);
  bool ApplyCTTSelection(CMSTopF* fj);
}; // Analyzer : BaseTreeAnalyzer

//Load event variables:
void Analyzer::loadVariables(){
  load(cfgSet::EVTINFO);
  if(isMC())
  load(cfgSet::AK4JETS, JetReader::FILLOBJ | JetReader::LOADGEN |JetReader::LOADRECO | JetReader::LOADJETSHAPE  | JetReader::LOADTOPASSOC);
  else
  load(cfgSet::AK4JETS, JetReader::FILLOBJ |JetReader::LOADRECO | JetReader::LOADJETSHAPE);
  load(cfgSet::ELECTRONS);
  load(cfgSet::MUONS);
  load(cfgSet::CMSTOPS);
  load(cfgSet::PFCANDS);
  if(isMC())load(cfgSet::GENPARTICLES, GenParticleReader::FILLOBJ | GenParticleReader::LOADPARTONDECAY);
}

// Analyze event and fill plots
void Analyzer::runEvent() {
////ClEAR AHEAD OF TIME
	  mtCandVals.clear();
	  ptCandVals.clear();
	  etaCandVals.clear();
	  discCandVals.clear();

////PRESELECTIONS
	  if(nJets < minNJets_) return;
	  //if(nBJets < minNBjets_) return;
	  ptMet = met->pt();
	  if(ptMet < metcut_) return;
	  if(!goodvertex) return;
	  if(!isMC() && hasJSONFile() && !passesLumiMask())  return;
	  njets = nJets;
////SETUP
 	  vector<RecoJetF*> jetsCSV;
 	  rankedByCSV(jets,jetsCSV);

 	  std::cout << "This is a test, we find that the veto lepton correction is " << eventCorrections.getVetoLepWeight() << std::endl;
 	  std::cout << "This is a test, we find that the selected lepton correction is " << eventCorrections.getSelLepWeight() << std::endl;

////RESET VARS
 	 nJets60 = 0;
 	 NCTTstd = 0;
 	 nSelectedMuons = 0;		nSelectedElectrons = 0;
 	 nGoodEle 		= 0;		nGoodEleAcc 	   = 0;		nPromptEle 		 = 0;		nPromptEleAcc 		= 0;
 	 nGoodMu 		= 0;		nGoodMuAcc  	   = 0;		nPromptMu  		 = 0;		nPromptMuAcc  		= 0;
 	 	 	 	 	 	 	 	 	 	 	 	 	 		nPromptTau		 = 0;		nPromptTauAcc	    = 0;
 	 	 	 	 	 	 	 	 	 	 	 	 	 		nPromptTau1Prong = 0;		nPromptTau3Prong 	= 0;
 	 	 	 	 	 	 	 	 	 	 	 	 	 		nPromptTauAcc1Prong = 0;	nPromptTauAcc3Prong = 0;
 	 nGoodLep 		= 0;	 	nGoodLepAcc 	   = 0;	  	nPromptLep 		   = 0;		nPromptLepAcc 		= 0;
 	 	 	 	 	 	 	    htmlAlongHardestEle=-10;	htmlAlongHardestMu=-10;    	htmlAlongHardestTauD=-10;
 	 	 	 	 	 	 	    hardestGenElePt	   =-10;	hardestGenMuPt   =-10;		hardestGenTauDPt 	=-10;
 	 	 	 	 	 	 	    hardestGenEleEta   =-10;	hardestGenMuEta  =-10;		hardestGenTauEtaD 	=-10;

////FILL EVENT VARIABLES
 	  passDiJetmet = (isMC() ? true : (triggerflag & kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned));
	  scaleFactor = weight;
	  npv         = nPV;
	  nVetoTau    = nVetoedTracks;
	  nVetoLep    = nSecondaryLeptons;
	  nbjets	  = nBJets;
	  for (UInt_t i=0; i<cttTops.size(); ++i)
		  if( ApplyCTTSelection(cttTops.at(i)))
			  ++NCTTstd;

 	  for(auto j: jets)if(j->pt() >= 60)nJets60++;

 	  dPhiMETJ12 = JetKinematics::absDPhiMETJ12(*met,jets);
 	  dPhiMETJ3  = JetKinematics::absDPhiMETJ3 (*met,jets);
 	  float mtcsv1MET = JetKinematics::transverseMass(jetsCSV.at(0)->p4(),*met);
 	  float mtcsv2MET = JetKinematics::transverseMass(jetsCSV.at(1)->p4(),*met);
 	  leadingCsv = jetsCSV.at(0)->csv();
 	  mtcsv12MET = (mtcsv1MET<mtcsv2MET) ? mtcsv1MET : mtcsv2MET;

      //FILL EVT INFO FOR RECO ELECTRONS AND MUONS
	  for(auto* i : selectedLeptons)
	  {
		  if(fabs(i->pdgid()) == 11) nSelectedElectrons++;
		  if(fabs(i->pdgid()) == 13) nSelectedMuons++;
	  }
 	  //FILL EVT INFO FOR GEN ELECTRONS, MUONS AND TAUS


	  for (UInt_t i=0; i< genParts.size(); ++i)
	  {
		const GenParticleF * genParton = genParts.at(i);
		const GenParticleF * genPartMom = 0;
		if (genParton->numberOfMothers()>0) { genPartMom = genParton->mother(0); }
		else{continue;}

		//(ELECTRON) make sure mom is a W or a Z or a Tau
		if ((abs(genParton->pdgId()) == 11) && (abs(genPartMom->pdgId()) == 23 or abs(genPartMom->pdgId()) == 24 or abs(genPartMom->pdgId()) == 15) ) {
		    //COUNT "GOOD" ELE's
				nGoodEle++;
		    	double htAlongEle = 0;
		    	bool cleanHt = false;
				//CHECK FOR HARDEST ELE. AND CALCULATE HT-ALONG
		    	if(genParton->pt() > hardestGenElePt)
		    	{
		    		hardestGenElePt = genParton->pt();
		    		hardestGenEleEta = genParton->eta();
		    		for(const auto* jet : jets ){
		    			double            dPhi            = PhysicsUtilities::absDeltaPhi(*jet,*genParton);
		    		    double            dR              = PhysicsUtilities::deltaR(*jet,*genParton);
				        if (dPhi < TMath::PiOver2()) {
				        	htAlongEle              += jet->pt();
		    				if(dR < .4) cleanHt = true;
				     }
				 }
		    		 if(cleanHt)
		    			 htmlAlongHardestEle	=	htAlongEle - 	genParton->pt()   ;
		    		 else
		    			 htmlAlongHardestEle	=	htAlongEle;
		    	}
		    if(genParton->pt() >5 and fabs(genParton->eta()) < 2.4)			nGoodEleAcc++;
		    //COUNT "PROMPT" ELE's (W or Z decay only)
			if(abs(genPartMom->pdgId()) == 23 or abs(genPartMom->pdgId()) == 24){
				nPromptEle++;
				//CHECK FOR HARDEST ELE IN ACC AND CALCULATE HT-ALONG
			    if(genParton->pt() >5 and fabs(genParton->eta()) < 2.4)	{
			    	nPromptEleAcc++;
			    }
			}
		}
		//(MUON) make sure mom is a W or a Z or a Tau
		if ((abs(genParton->pdgId()) == 13) && (abs(genPartMom->pdgId()) == 23 or abs(genPartMom->pdgId()) == 24 or abs(genPartMom->pdgId()) == 15)) {
			nGoodMu++;
	    	double htAlongMu = 0;
	    	bool cleanHt = false;
			//CHECK FOR HARDEST MU AND CALCULATE HT-ALONG
	    	if(genParton->pt() > hardestGenMuPt)
	    	{
	    		hardestGenMuPt = genParton->pt();
	    		hardestGenMuEta = genParton->eta();
	    		for(const auto* jet : jets ){
	    			double            dPhi            = PhysicsUtilities::absDeltaPhi(*jet,*genParton);
	    		    double            dR              = PhysicsUtilities::deltaR(*jet,*genParton);
			        if (dPhi < TMath::PiOver2()) {
			        	htAlongMu              += jet->pt();
	    				if(dR < .4) cleanHt = true;
			     }
			 }
	    		 if(cleanHt)
	    			 htmlAlongHardestMu	=	htAlongMu - 	genParton->pt()   ;
	    		 else
	    			 htmlAlongHardestMu	=	htAlongMu;
	    	}

		    if(genParton->pt() >5 and fabs(genParton->eta()) < 2.4)			nGoodMuAcc++;
		    //COUNT "PROMPT" MU's (W or Z decay only)
			if(abs(genPartMom->pdgId()) == 23 or abs(genPartMom->pdgId()) == 24){
				nPromptMu++;
			    if(genParton->pt() >5 and fabs(genParton->eta()) < 2.4)		{
			    	nPromptMuAcc++;

			    }
			}
		}
		//(TAU) make sure mom is a W or a Z
		if ((abs(genParton->pdgId()) == 15) && (abs(genPartMom->pdgId()) == 23 or abs(genPartMom->pdgId()) == 24)){

			bool inAcc = false;
			bool lepDecay = false;
			int  prongCounter = 0;

			//COUNT TAU IN ACC (CHARGED DAUGHTER WITH PT > 10)
			for(unsigned int itd = 0; itd < genParton->numberOfDaughters(); itd++) {
				const GenParticleF* dau = genParton->daughter(itd);
		    	double htAlongTau = 0;
		    	bool cleanHt = false;
				if(ParticleInfo::isA(12, dau) || ParticleInfo::isA(14, dau) || ParticleInfo::isA(16, dau)) continue;
				//CHECK FOR HARDEST TAU DAU. AND CALCULATE HT-ALONG
		    	if(dau->pt() > hardestGenTauDPt)
		    	{
		    		hardestGenTauDPt  = dau->pt();
		    		hardestGenTauEtaD = dau->eta();
		    		for(const auto* jet : jets ){
		    			double            dPhi            = PhysicsUtilities::absDeltaPhi(*jet,*genParton);
		    		    double            dR              = PhysicsUtilities::deltaR(*jet,*genParton);
				        if (dPhi < TMath::PiOver2()) {
				        	htAlongTau              += jet->pt();
		    				if(dR < .4) cleanHt = true;
				     }
				 }
		    		 if(cleanHt)
		    			 htmlAlongHardestTauD	=	htAlongTau - 	hardestGenTauDPt   ;
		    		 else
		    			 htmlAlongHardestTauD	=	htAlongTau;
		    	}
				if(ParticleInfo::isA(ParticleInfo::p_eminus, dau) || ParticleInfo::isA(ParticleInfo::p_muminus, dau)) lepDecay = true;
				if(!(fabs(dau->pdgId())==211) and !(fabs(dau->pdgId())==321)) continue;
				if(dau->pt() > 10 and fabs(dau->eta()) < 2.4) {
					inAcc = true;
				}
				prongCounter++;
				taudecayprods.push_back(dau);
			}
			if(!lepDecay)
			nPromptTau++;
			if(inAcc)
				nPromptTauAcc++;

			if(prongCounter < 3){
				if(inAcc) 	nPromptTauAcc1Prong++;
				nPromptTau1Prong++;
			}
			else{
				if(inAcc) 	nPromptTauAcc3Prong++;
				nPromptTau3Prong++;
			}
		}
	  }
	  for(uint iT =0; iT < pfcandReader.pfcands.size(); ++iT){
		PFCandidateF c = pfcandReader.pfcands.at(iT);
		if(!(c.ischargedhadron())) continue;
		float maxdR = 0.05;
		float kpt = 0.01;
		float candPt = c.pt();
		float candEta = c.eta();
			for(const auto* genchhad : taudecayprods) {
				float dpt = 0.0;
		    	if(genchhad->pt()>0.5) dpt = fabs(1.0 - c.pt()/genchhad->pt());
		    	if(sampleName=="t2tt"){
		    		if(candPt > 10 && candEta < 2.4){
		    			mtCandVals.push_back(c.mt());
		    			discCandVals.push_back(c.taudisc());
		    			ptCandVals.push_back(c.pt());
		    			etaCandVals.push_back(c.eta());

		    		}
		    	}
		    	else{
		    		if((PhysicsUtilities::deltaR(c, *genchhad) +  kpt*dpt) < maxdR && dpt < 0.4 && candPt > 10 && candEta < 2.4) {
		    			mtCandVals.push_back(c.mt());
		    			discCandVals.push_back(c.taudisc());
		    			ptCandVals.push_back(c.pt());
		    			etaCandVals.push_back(c.eta());

		    		}
		    	}
			}
	  }
	  nGoodLep 		= nGoodMu + nGoodEle;
	  nGoodLepAcc   = nGoodMuAcc + nGoodEleAcc;
	  nPromptLep 	= nPromptMu + nPromptEle + nPromptTau;
	  nPromptLep 	= nPromptMuAcc + nPromptEleAcc + nPromptTauAcc;
	  outtree->Fill();
	  //Clear Vectors
	  taudecayprods.clear();
	  return;

} // Analyzer::runEvent()

void Analyzer::rankedByCSV(vector<RecoJetF*> inJets,vector<RecoJetF*>& outJets) {
  outJets.clear();
  outJets.resize(inJets.size());
  vector<pair<double,int> > rankedJets(inJets.size());

  for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){
    rankedJets[iJ].first = inJets[iJ]->csv();
    rankedJets[iJ].second = iJ;
  } // inJets.size()

  std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());
  for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){ outJets[iJ] = inJets[rankedJets[iJ].second]; }
} // rankedByCSV()

bool Analyzer::ApplyCTTSelection(CMSTopF* fj) {

      bool tmpVal = false;

      float topRawMass   = fj->topRawMass();
      float inTopMinMass  = fj->topMinMass();
      int   topNsubJets = fj->topNsubJets();

      if ( ((topRawMass)>140.) && ((topRawMass)<250.) && ((inTopMinMass)>50.) && ((topNsubJets)>=3) ) { tmpVal = true; }
      return tmpVal;
}	//ApplyCTT

// get the x-section for the given sample
double getXsec(string sample) {
  if (sample=="ttbar") return 831.76   ;
  if (sample=="ttZ")   return    .7598 ;
  if (sample=="znunu_ht100to200") return 372.6   ;
  if (sample=="znunu_ht200to400") return 100.8   ;
  if (sample=="znunu_ht400to600") return  11.99  ;
  if (sample=="znunu_ht600toinf") return   4.113 ;
  if (sample=="T2tt_425_325") return 1.31169   ;
  if (sample=="T2tt_500_325") return 0.51848   ;
  if (sample=="T2tt_650_325") return 0.107045  ;
  if (sample=="T2tt_850_100") return 0.0189612 ;
  cout << "WARNING: no cross section available for " << sample << "!" << endl
	     << "         setting xsec = 1" << endl;
  return 1;
}
//
#endif

// Process file belonging to specified sample with a given cross section
//root -b -q "../CMSSW_7_3_1/src/AnalysisMethods/macros/0LepSearchRegions/processZeroLepton.C+()"
void processLepEffVars(TString sname      = "ttbar" // sample name
                     , const int     fileindex  = 1       // index of file (-1 means there is only 1 file for this sample)
                     , const bool    isMC       = true    // data or MC
                     , const TString fname      = "ttbar_1_ntuple_wgtxsec.root" // path of file to be processed
                     , const double  xsec       = 831.76    // cross section to be used with this file
                     , const string  outputdir  = "plots/testing/"  // directory to which files with histograms will be written
                     , const TString fileprefix = "/eos/uscms/store/user/vdutta/13TeV/080615/merged/"
                     )
{
  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  // Make sure the output has a unique name in case there are multiple files to process
  if(fileindex > -1) sname += TString::Format("_%d",fileindex);

  if(isMC) printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

  //Load up search configuration
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt");
  cfgSet::ConfigSet cfg = cfgSet::zl_lepton_set;
  //cfg.jets.cleanJetsvSelectedLeptons = true;
  //Create analyzer
  double randomSeed = fileindex + 2;
  //TString treename = isMC ? "Events" : "TestAnalyzer/Events";
  TString treename = "Events";
  Analyzer a(fullname, treename, isMC, &cfg, xsec, sname, outputdir, randomSeed);//declare analyzer
  a.analyze(10000); // run: Argument is frequency of printout
}
