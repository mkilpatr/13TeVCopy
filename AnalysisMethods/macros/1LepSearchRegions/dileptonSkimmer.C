// derived from AnalysisMethods/macros/0LepSearchRegions/ZeroPlusOneLeptonSkimmer.C
// skims for one or two leptons, with MET cut
/* example submitall.sh script
#!/bin/bash
runmacro=DileptonSkimmer.C
prefix=root://eoscms//eos/cms/
root -l -q -b ../rootlogon.C ../$runmacro+\(\"/store/user/vdutta/13TeV/290615/merged/ttbar_1_ntuple_wgtxsec.root\",\"skimmed\",\"$prefix\"\)
*/

/*
 * Uses the tree copier to copy over a tree!
 *
 * We use the loaded branches option so that we make the tree smaller
 *
 * Use this guy as a base class or an example!
 *
 */
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"

#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/DataFormats/interface/GenParticle.h"


using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(TString fileName, TString treeName, TString outFileName, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierAllBranches(fileName,treeName,outFileName,isMCTree,pars) {
  };
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::PFCANDS);
    load(cfgSet::GENPARTICLES);
  }

  virtual bool fillEvent() {

	int genlepsfromtop=0;
	if(met->pt() < 150) return false;
	
	for (UInt_t i=0; i<genParts.size(); ++i) {
		const GenParticleF * genPartMom = 0;
		if (genParts.at(i)->numberOfMothers()>0) 
			{ genPartMom = genParts.at(i)->mother(0); }
		// leave taus in, and make sure mum is a W
		if (( ((abs(genParts.at(i)->pdgId()) == 11) || (abs(genParts.at(i)->pdgId()) == 13) || (abs(genParts.at(i)->pdgId()) == 15)) && (abs(genPartMom->pdgId())==24))) genlepsfromtop++;
	}
	if (genlepsfromtop != 2) return false; // dilepton

// leave jets alone
//    if(nJets < 4) return false;
//    if(nBJets < 1) return false;
    return true;
  }

  void book() {
  }


};


#endif

/*
 * process -> string from defaults::PROCESS_NAMES
 * crossSection -> processCrossSection
 * lumi -> luminosity that you wish to scale for
 * nEvents -> number of events in sample (if < 0 take it from the tree)
 *
 */

void lepSkimmer(string fileName, string outPostfix ="skimmed",const TString fileprefix = "file:$CMSSW_BASE/src/AnalysisBase/Analyzer/test/") {

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;

  TString treeName="Events";
// don't veto any leptons
//  cfg.vetoedLeptons.selectedMuon = (&MuonF::ismultiisovetomuonl);
//  cfg.vetoedLeptons.selectedElectron = (&ElectronF::ismultiisovetoelectronl);

  TString fullName = fileprefix+fileName;

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  Copier a(fullName,treeName,outName,true, &cfg);

  a.analyze();
}
