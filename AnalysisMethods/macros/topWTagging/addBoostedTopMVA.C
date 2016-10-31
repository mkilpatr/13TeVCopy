#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/MVAWrapper.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree) : TreeCopierAllBranches(fileName,treeName,outFileName,12345,isMCTree,0), mvaWrap("topDisc_new.root","mva_0")
  {};
  virtual ~Copier() {};

  virtual void setupTree() {
    reader.getTree()->SetBranchStatus("*",1);

    outFile_ = new TFile(outFileName_,"RECREATE");
    outFile_->cd();
    treeWriter_ = new TreeWriter(reader.getTree()->CloneTree(0), reader.getTree()->GetName());
  }

  void loadVariables(){
    reader.setBranchAddress("","fj_subjet_pt"          ,&fj_subjet_pt          );
    reader.setBranchAddress("","fj_tau1"               ,&fj_tau1               );
    reader.setBranchAddress("","fj_tau2"               ,&fj_tau2               );
    reader.setBranchAddress("","fj_tau3"               ,&fj_tau3               );
    reader.setBranchAddress("","fj_ropt"               ,&fj_ropt               );
    reader.setBranchAddress("","fj_subjet_mass"        ,&fj_subjet_mass        );
    reader.setBranchAddress("","fj_subjet_w_mass"      ,&fj_subjet_w_mass      );
    reader.setBranchAddress("","fj_subjet_w_pt"        ,&fj_subjet_w_pt        );
    reader.setBranchAddress("","fj_b_pt"               ,&fj_b_pt               );
    reader.setBranchAddress("","fj_b_csv"              ,&fj_b_csv              );
    reader.setBranchAddress("","fj_b_mass"             ,&fj_b_mass             );
    reader.setBranchAddress("","fj_subjet_w_bycsv_mass",&fj_subjet_w_bycsv_mass);
    reader.setBranchAddress("","fj_subjet_w_bycsv_pt"  ,&fj_subjet_w_bycsv_pt  );
    reader.setBranchAddress("","fj_subjet_b_bycsv_pt"  ,&fj_subjet_b_bycsv_pt  );
    reader.setBranchAddress("","fj_subjet_b_bycsv_csv" ,&fj_subjet_b_bycsv_csv );
    reader.setBranchAddress("","fj_subjet_b_bycsv_mass",&fj_subjet_b_bycsv_mass);
    reader.setBranchAddress("","fj_pt"                 ,&fj_pt                 );
    reader.setBranchAddress("","fj_eta"                ,&fj_eta                );
    reader.setBranchAddress("","fj_subjet_min_pt"      ,&fj_subjet_min_pt      );


  }

  virtual void processVariables(){}

  double evaluateMVA(){

    static const int parIndex_pt = mvaWrap.findAxis("pt");
    static vector<double> mva_parameters(1,0);
    mva_parameters[parIndex_pt] = 10;

    mvaWrap.setParameters(mva_parameters);

    static const int index_fj_subjet_pt           =mvaWrap.findVariable("fj_subjet_pt"          );
    static const int index_fj_tau1                =mvaWrap.findVariable("fj_tau1"               );
    static const int index_fj_tau2                =mvaWrap.findVariable("fj_tau2"               );
    static const int index_fj_tau3                =mvaWrap.findVariable("fj_tau3"               );
    static const int index_fj_ropt                =mvaWrap.findVariable("fj_ropt"               );
    static const int index_fj_subjet_mass         =mvaWrap.findVariable("fj_subjet_mass"        );
    static const int index_fj_subjet_w_mass       =mvaWrap.findVariable("fj_subjet_w_mass"      );
    static const int index_fj_subjet_w_pt         =mvaWrap.findVariable("fj_subjet_w_pt"        );
    static const int index_fj_b_pt                =mvaWrap.findVariable("fj_b_pt"               );
    static const int index_fj_b_csv               =mvaWrap.findVariable("fj_b_csv"              );
    static const int index_fj_b_mass              =mvaWrap.findVariable("fj_b_mass"             );
    static const int index_fj_subjet_w_bycsv_mass =mvaWrap.findVariable("fj_subjet_w_bycsv_mass");
    static const int index_fj_subjet_w_bycsv_pt   =mvaWrap.findVariable("fj_subjet_w_bycsv_pt"  );
    static const int index_fj_subjet_b_bycsv_pt   =mvaWrap.findVariable("fj_subjet_b_bycsv_pt"  );
    static const int index_fj_subjet_b_bycsv_csv  =mvaWrap.findVariable("fj_subjet_b_bycsv_csv");
    static const int index_fj_subjet_b_bycsv_mass =mvaWrap.findVariable("fj_subjet_b_bycsv_mass");
//  static const int index_fj_pt                  =mvaWrap.findVariable("fj_pt"                 );
//  static const int index_fj_eta                 =mvaWrap.findVariable("fj_eta"                );
//  static const int index_fj_subjet_min_pt       =mvaWrap.findVariable("fj_subjet_min_pt"      );


    mvaWrap.setVariable(index_fj_subjet_pt            ,fj_subjet_pt          );
    mvaWrap.setVariable(index_fj_tau1                 ,fj_tau1               );
    mvaWrap.setVariable(index_fj_tau2                 ,fj_tau2               );
    mvaWrap.setVariable(index_fj_tau3                 ,fj_tau3               );
    mvaWrap.setVariable(index_fj_ropt                 ,fj_ropt               );
    mvaWrap.setVariable(index_fj_subjet_mass          ,fj_subjet_mass        );
    mvaWrap.setVariable(index_fj_subjet_w_mass        ,fj_subjet_w_mass      );
    mvaWrap.setVariable(index_fj_subjet_w_pt          ,fj_subjet_w_pt        );
    mvaWrap.setVariable(index_fj_b_pt                 ,fj_b_pt               );
    mvaWrap.setVariable(index_fj_b_csv                ,fj_b_csv              );
    mvaWrap.setVariable(index_fj_b_mass               ,fj_b_mass             );
    mvaWrap.setVariable(index_fj_subjet_w_bycsv_mass  ,fj_subjet_w_bycsv_mass);
    mvaWrap.setVariable(index_fj_subjet_w_bycsv_pt    ,fj_subjet_w_bycsv_pt  );
    mvaWrap.setVariable(index_fj_subjet_b_bycsv_pt    ,fj_subjet_b_bycsv_pt  );
    mvaWrap.setVariable(index_fj_subjet_b_bycsv_csv  ,fj_subjet_b_bycsv_csv  );
    mvaWrap.setVariable(index_fj_subjet_b_bycsv_mass ,fj_subjet_b_bycsv_mass );
//  mvaWrap.setVariable(index_fj_pt                   ,fj_pt                 );
//  mvaWrap.setVariable(index_fj_eta                  ,fj_eta                );
//  mvaWrap.setVariable(index_fj_subjet_min_pt        ,fj_subjet_min_pt      );

    return mvaWrap.evaluate();
  }

  bool fillEvent() {
    data.fill<float>(topmva_new,   evaluateMVA());
    return true;
  }

  void book() {
    topmva_new      = data.add<float>("","topmva_new"      ,"F",0);
  }

  MVAWrapper mvaWrap;

  size topmva_new;

//  float "fj_subjet_pt"
//  float "fj_tau1"
//  float "fj_tau2"
//  float "fj_tau3"
//  float "fj_ropt"
//  float "fj_subjet_mass"
//  float "fj_subjet_w_mass"
//  float "fj_subjet_w_pt"
//  float "fj_b_pt"
//  float "fj_b_csv"
//  float "fj_subjet_w_bycsv_mass"
//  float "fj_subjet_w_bycsv_pt"
//  float "fj_subjet_b_bycsv_pt"
//  float "fj_subjet_b_bycsv_mass"
//  float "fj_pt"
//  float "fj_eta"
//  float "fj_subjet_min_pt"

    float fj_subjet_pt;
    float fj_tau1;
    float fj_tau2;
    float fj_tau3;
    float fj_ropt;
    float fj_subjet_mass;
    float fj_subjet_w_mass;
    float fj_subjet_w_pt;
    float fj_b_pt;
    float fj_b_csv;
    float fj_b_mass;
    float fj_subjet_w_bycsv_mass;
    float fj_subjet_w_bycsv_pt;
    float fj_subjet_b_bycsv_pt;
    float fj_subjet_b_bycsv_csv;
    float fj_subjet_b_bycsv_mass;
    float fj_pt;
    float fj_eta;
    float fj_subjet_min_pt;






















};

#endif

void addBoostedTopMVA(string fileName = "trees/T2tt_HTT1p5_tree.root", string treeName = "Events", string outFileName ="trees/T2tt_HTT1p5_tree_addmva.root", bool isMCTree = true) {
  Copier a(fileName,treeName,outFileName,isMCTree);
  a.analyze(100000);
}
