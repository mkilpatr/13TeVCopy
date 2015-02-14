#if !defined(__CINT__) || defined(__MAKECINT__)
#include "selectionVariables.h"
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierManualBranches {
public:
  Copier(string fileName, string treeName, string outFileName, ConfigPars * pars) : TreeCopierManualBranches(fileName,treeName,outFileName,true,pars){};
  virtual ~Copier() {};

  VariableCalculator              vars;

  void loadVariables(){
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(PICKYJETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(ELECTRONS);
    load(MUONS);
    load(PFCANDS);
  }

  bool fillEvent() {
    vector<RecoJetF*> ak4Jets;
    cleanJets(&ak4Reader,ak4Jets,0,0);
    vars.processVariables(this,&ak4Reader,ak4Jets,jets,met);
    if(!vars.passPreselction) return false;
    if(nVetoedLeptons + nVetoedTaus > 0) return false;

    data.fill<float>(met_pt          ,float(vars.met_pt         ));
    data.fill<int  >(nJ20            ,int  (vars.nJ20           ));
    data.fill<float>(dPhiMET3        ,float(vars.dPhiMET3       ));
    data.fill<int  >(nMedBTags       ,int  (vars.nMedBTags      ));
    data.fill<float>(secLeadQL       ,float(vars.secLeadQL      ));
    data.fill<float>(leadLeadQL      ,float(vars.leadLeadQL     ));
    data.fill<float>(prodQL          ,float(vars.prodQL         ));
    data.fill<float>(htAlongAway     ,float(vars.htAlongAway    ));
    data.fill<float>(rmsJetPT        ,float(vars.rmsJetPT       ));
    data.fill<float>(rmsJetDphiMET   ,float(vars.rmsJetDphiMET  ));
    data.fill<float>(bInvMass        ,float(vars.bInvMass       ));
    data.fill<float>(bTransverseMass ,float(vars.bTransverseMass));
    data.fill<float>(rmsBEta         ,float(vars.rmsBEta        ));
    data.fill<float>(wInvMass        ,float(vars.wInvMass       ));
    data.fill<float>(ht         ,float(vars.ht       ));


    data.fill<float>(i_weight        ,float(weight       ));
    data.fill<int  >(i_process       ,int(process      ));




    return true;
  }

  void book() {
    met_pt           = data.add<float>("","met"               ,"F",0);
    nJ20             = data.add<int  >("","num_j20"           ,"I",0);
    dPhiMET3         = data.add<float>("","dphi_j3_met"       ,"F",0);
    nMedBTags        = data.add<int  >("","num_medium_btags"  ,"I",0);
    secLeadQL        = data.add<float>("","q2_likeli"         ,"F",0);
    leadLeadQL       = data.add<float>("","q1_likeli"         ,"F",0);
    prodQL           = data.add<float>("","quark_likeli"      ,"F",0);
    htAlongAway      = data.add<float>("","ht_along_over_away","F",0);
    rmsJetPT         = data.add<float>("","rms_pt"            ,"F",0);
    rmsJetDphiMET    = data.add<float>("","rms_dphi"          ,"F",0);
    bInvMass         = data.add<float>("","bb_mass"           ,"F",0);
    bTransverseMass  = data.add<float>("","mTb"               ,"F",0);
    rmsBEta          = data.add<float>("","deta_b_rms"        ,"F",0);
    wInvMass         = data.add<float>("","leading_jj_mass"   ,"F",0);
    ht               = data.add<float>("","ht"   ,"F",0);

    i_weight         = data.add<float>("","weight"   ,"F",0);
    i_process         = data.add<int >("","process"   ,"I",0);

  }


  size met_pt         ;
  size nJ20           ;
  size dPhiMET3       ;
  size nMedBTags      ;
  size secLeadQL      ;
  size leadLeadQL     ;
  size prodQL         ;
  size htAlongAway    ;
  size rmsJetPT       ;
  size rmsJetDphiMET  ;
  size bInvMass       ;
  size bTransverseMass;
  size rmsBEta        ;
  size wInvMass       ;
//  size leadBPT;
//  size secLeadBPT;
  size ht;


  size i_weight       ;
  size i_process      ;

};


#endif



void flattenSRTree(string fileName = "evttree.root", string treeName = "TestAnalyzer/Events", string outFileName ="newTree.root") {

  BaseTreeAnalyzer::ConfigPars pars;
  pars.cleanJetsvSelectedLeptons_ = false;
  pars.leptonSelection = BaseTreeAnalyzer::SEL_0_LEP;
  pars.defaultJetCollection = BaseTreeAnalyzer::PICKYJETS;
  Copier a(fileName,treeName,outFileName,&pars);
  a.analyze();
}
