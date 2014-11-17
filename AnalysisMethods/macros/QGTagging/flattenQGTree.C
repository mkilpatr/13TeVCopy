#if !defined(__CINT__) || defined(__MAKECINT__)
#include "QGHelper.h"
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeFlattenCopier {
public:
  Copier(string fileName, string treeName, string outFileName, bool doReco, bool isMCTree) : TreeFlattenCopier(fileName,treeName,outFileName,isMCTree), reco(doReco) {};
  virtual ~Copier() {};

  void loadVariables(){
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(GENPARTICLES);
  }

  virtual void processVariables(){
    procesSubjets(recoJets,genJets,this);
    if(reco){
      categorize(recoJets,genParts,true);
    } else {
      categorize(genJets,genParts,false);
    }
  }

  bool fillEvent() {
    if(reco){
      flv.resize(ak4Reader.jetpt_->size());
      for(unsigned int iF = 0; iF < flv.size(); ++iF )
        flv[iF] = U;
      for(const auto& j : recoJets)
        flv[j.origInd] = j.type;
    } else {
      flv.resize(ak4Reader.genjetpt_->size());
      for(unsigned int iF = 0; iF < flv.size(); ++iF )
        flv[iF] = U;
      for(const auto& j : genJets)
        flv[j.origInd] = j.type;
    }
    return true;
  }

  void book() {
    if(reco){
      addLinkedMulti(ak4Reader.jetbetaStar_,"F");
      addLinkedMulti(ak4Reader.jetqgl_,"F");
      addLinkedMulti(ak4Reader.jetptD_,"F");
      addLinkedMulti(ak4Reader.jetaxis1_,"F");
      addLinkedMulti(ak4Reader.jetaxis2_,"F");
      addLinkedMulti(ak4Reader.jetMult_,"I");
      addLinkedMulti(ak4Reader.jetpt_,"F");
      addLinkedMulti(ak4Reader.jeteta_,"F");
    } else {
      addLinkedMulti(ak4Reader.genjetptD_,"F");
      addLinkedMulti(ak4Reader.genjetaxis1_,"F");
      addLinkedMulti(ak4Reader.genjetaxis2_,"F");
      addLinkedMulti(ak4Reader.genjetMult_,"I");
      addLinkedMulti(ak4Reader.genjetpt_,"F");
      addLinkedMulti(ak4Reader.genjeteta_,"F");
    }
    addLinkedMulti(&flv,"flavor","I");
    addLinked(&evtInfoReader.rho,"F");
    addLinked(&evtInfoReader.nPV,"I");
  }


  bool reco;
  vector<int> flv;
  std::vector<subJet> genJets ;
  std::vector<subJet> recoJets ;

};


#endif



void flattenQGTree(string fileName = "evttree.root", string treeName = "TestAnalyzer/Events", string outFileName ="newTree.root", bool doReco = true, bool isMCTree = true) {
  Copier a(fileName,treeName,outFileName,doReco,isMCTree);
  a.analyze();
}
