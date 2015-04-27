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
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

#include "ObjectProducers/TopTagging/interface/CORRAL.h"

using namespace std;
using namespace ucsbsusy;
static TString MVAprefix;

class Copier : public TreeCopierAllBranches {
public:

  CORRAL::CORRALReconstructor  corral;

  Copier(string fileName, string treeName, string outFileName, bool isMCTree, ConfigPars * pars) : TreeCopierAllBranches(fileName,treeName,outFileName,isMCTree,pars)
  ,corral(MVAprefix){};
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(EVTINFO);
    load(GENPARTICLES, GenParticleReader::FILLOBJ | GenParticleReader::LOADPARTONDECAY);
    load(PICKYJETS,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC | JetReader::LOADJETSHAPE );
  }

  virtual bool fillEvent() {
    vector<RankedIndex> prunedTops;
    corral.getTopPairs(&genParticleReader, &pickyJetReader,nPV,&prunedTops);
    CORRAL::CORRALData& corralData = corral.data;

    for(const auto& pT : prunedTops){
      const auto& cand = corralData.tCands[pT.second];
      const auto& vars = corralData.tCandVars[pT.second];

      data.fillMulti<int16>(ibjetind     ,convertTo<int16>(cand.bJet->index(),"CORRALWriter::ibjetind"));
      data.fillMulti<int16>(iwjet1ind    ,convertTo<int16>(cand.wCand->jet1->index(),"CORRALWriter::iwjet1ind"));
      data.fillMulti<int16>(iwjet2ind    ,convertTo<int16>(cand.wCand->jet2->index(),"CORRALWriter::iwjet2ind"));

      data.fillMulti<float>(iwmva    ,vars.wDisc);
      data.fillMulti<float>(itmva    ,pT.first);
    }



    return true;
  }

  void book() {
    ibjetind           = data.addMulti<int16  >(defaults::BRANCH_CORRAL,"bjetind" ,-1);
    iwjet1ind          = data.addMulti<int16  >(defaults::BRANCH_CORRAL,"wjet1ind",-1);
    iwjet2ind          = data.addMulti<int16  >(defaults::BRANCH_CORRAL,"wjet2ind",-1);
    iwmva              = data.addMulti<float>  (defaults::BRANCH_CORRAL,"wmva"    ,-1);
    itmva              = data.addMulti<float>  (defaults::BRANCH_CORRAL,"tmva"    ,-1);
  }
  size ibjetind ;
  size iwjet1ind;
  size iwjet2ind;
  size iwmva    ;
  size itmva    ;

};


#endif

void CORRALWriter(string fileName, string treeName = "TestAnalyzer/Events", string outPostfix ="corral", TString MVAprefix_ = "$CMSSW_BASE/src/data/CORRAL/") {
  BaseTreeAnalyzer::ConfigPars pars;

  MVAprefix = MVAprefix_;

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());


  Copier a(fileName,treeName,outName.Data(),true, 0);


  clog << "Adding CORRAL to "<< a.getEntries() <<" events into file "<< outName << endl;
  a.analyze();
}
