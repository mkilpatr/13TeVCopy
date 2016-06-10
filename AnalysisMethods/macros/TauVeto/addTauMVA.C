#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/MVAWrapper.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree) : TreeCopierAllBranches(fileName,treeName,outFileName,12345,isMCTree,0), mvaWrap("tauDisc_new.root","mva_0")
  {};
  virtual ~Copier() {};

  virtual void setupTree() {
    reader.getTree()->SetBranchStatus("*",1);

    outFile_ = new TFile(outFileName_,"RECREATE");
    outFile_->cd();
    treeWriter_ = new TreeWriter(reader.getTree()->CloneTree(0), reader.getTree()->GetName());
  }

  void loadVariables(){
    reader.setBranchAddress("","pt",&pt);
    reader.setBranchAddress("","mt",&mt);
    reader.setBranchAddress("","dphimet",&dphimet);
    reader.setBranchAddress("","misset",&misset);
    reader.setBranchAddress("","njets",&njets);
    reader.setBranchAddress("","abseta",&abseta);
    reader.setBranchAddress("","absdz",&absdz);
    reader.setBranchAddress("","chiso0p1",&chiso0p1);
    reader.setBranchAddress("","chiso0p2",&chiso0p2);
    reader.setBranchAddress("","chiso0p3",&chiso0p3);
    reader.setBranchAddress("","chiso0p4",&chiso0p4);
    reader.setBranchAddress("","totiso0p1",&totiso0p1);
    reader.setBranchAddress("","totiso0p2",&totiso0p2);
    reader.setBranchAddress("","totiso0p3",&totiso0p3);
    reader.setBranchAddress("","totiso0p4",&totiso0p4);
    reader.setBranchAddress("","chreliso0p1",&chreliso0p1);
    reader.setBranchAddress("","chreliso0p2",&chreliso0p2);
    reader.setBranchAddress("","chreliso0p3",&chreliso0p3);
    reader.setBranchAddress("","chreliso0p4",&chreliso0p4);
    reader.setBranchAddress("","totreliso0p1",&totreliso0p1);
    reader.setBranchAddress("","totreliso0p2",&totreliso0p2);
    reader.setBranchAddress("","totreliso0p3",&totreliso0p3);
    reader.setBranchAddress("","totreliso0p4",&totreliso0p4);
    reader.setBranchAddress("","neartrkdr",&neartrkdr);
    reader.setBranchAddress("","contjetdr",&contjetdr);
    reader.setBranchAddress("","contjetcsv",&contjetcsv);
    reader.setBranchAddress("","gentaumatch",&gentaumatch);
  }

  virtual void processVariables(){}

  double evaluateMVA(){
    float filtered_pt = pt;
    if(pt > 1000.0) filtered_pt = 999.9;

    static const int parIndex_pt = mvaWrap.findAxis("pt");
    static vector<double> mva_parameters(1,0);
    mva_parameters[parIndex_pt] = filtered_pt;

    mvaWrap.setParameters(mva_parameters);

    static const int index_pt         =mvaWrap.findVariable("pt"        );
    static const int index_abseta     =mvaWrap.findVariable("abseta"    );
    static const int index_chiso0p1   =mvaWrap.findVariable("chiso0p1"  );
    static const int index_chiso0p2   =mvaWrap.findVariable("chiso0p2"  );
    static const int index_chiso0p3   =mvaWrap.findVariable("chiso0p3"  );
    static const int index_chiso0p4   =mvaWrap.findVariable("chiso0p4"  );
    static const int index_totiso0p1  =mvaWrap.findVariable("totiso0p1" );
    static const int index_totiso0p2  =mvaWrap.findVariable("totiso0p2" );
    static const int index_totiso0p3  =mvaWrap.findVariable("totiso0p3" );
    static const int index_totiso0p4  =mvaWrap.findVariable("totiso0p4" );
    static const int index_neartrkdr  =mvaWrap.findVariable("neartrkdr" );
    static const int index_contjetdr  =mvaWrap.findVariable("contjetdr" );
    static const int index_contjetcsv =mvaWrap.findVariable("contjetcsv");

    mvaWrap.setVariable(index_pt, pt);
    mvaWrap.setVariable(index_abseta, abseta);
    mvaWrap.setVariable(index_chiso0p1, chiso0p1);
    mvaWrap.setVariable(index_chiso0p2, chiso0p2);
    mvaWrap.setVariable(index_chiso0p3, chiso0p3);
    mvaWrap.setVariable(index_chiso0p4, chiso0p4);
    mvaWrap.setVariable(index_totiso0p1, totiso0p1);
    mvaWrap.setVariable(index_totiso0p2, totiso0p2);
    mvaWrap.setVariable(index_totiso0p3, totiso0p3);
    mvaWrap.setVariable(index_totiso0p4, totiso0p4);
    mvaWrap.setVariable(index_neartrkdr, neartrkdr);
    mvaWrap.setVariable(index_contjetdr, contjetdr);
    mvaWrap.setVariable(index_contjetcsv, contjetcsv);

    return mvaWrap.evaluate();
  }

  bool fillEvent() {
    data.fill<float>(taumva_new,   evaluateMVA());
    return true;
  }

  void book() {
    taumva_new      = data.add<float>("","taumva_new"      ,"F",0);
  }

  MVAWrapper mvaWrap;

  size taumva_new;

  int   njets;
  float pt;
  float mt;
  float dphimet;
  float misset;
  float abseta;
  float absdz;
  float chiso0p1;
  float chiso0p2;
  float chiso0p3;
  float chiso0p4;
  float totiso0p1;
  float totiso0p2;
  float totiso0p3;
  float totiso0p4;
  float chreliso0p1;
  float chreliso0p2;
  float chreliso0p3;
  float chreliso0p4;
  float totreliso0p1;
  float totreliso0p2;
  float totreliso0p3;
  float totreliso0p4;
  float neartrkdr;
  float contjetdr;
  float contjetcsv;
  bool  gentaumatch;

};

#endif

void addTauMVA(string fileName = "tauMVATrees/realtaus_tt1tau.root", string treeName = "Candidates", string outFileName ="tauMVATrees/realtaus_tt1tau_addtaumva.root", bool isMCTree = true) {
  Copier a(fileName,treeName,outFileName,isMCTree);
  a.analyze(100000);
}
