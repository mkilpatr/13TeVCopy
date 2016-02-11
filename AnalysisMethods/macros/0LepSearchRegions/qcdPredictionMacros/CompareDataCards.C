#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

struct ourPred{
  double data ;
  double TFTC ;
  double extU ;
  double respU;
  double subU ;
  double statU;

  void fill(QCDSupport::SRegInfo& srinfo, QCDSupport::CRegInfo& crinfo, QCDSupport::SRegInfo::SRReg reg, int mB){
    data  = std::max(float(1.0),crinfo.getDataYield(srinfo.crRegs[reg],mB));
    TFTC  = srinfo.getTF(reg,mB) * crinfo.getDataYieldCorr(srinfo.crRegs[reg],mB);
    extU  = srinfo.getExtUncs(reg,mB);
    respU = srinfo.getRespUnc(reg,mB);
    subU  = crinfo.getDataYieldCorrUnc(srinfo.crRegs[reg],mB);
    statU = srinfo.getTFStatUnc(reg,mB);
  }
  TString print(){return TString::Format("D: %.0f TF: %.5f P: %.3f  +/- %.3f +/- %.3f +/- %.3f +/- %.3f",data,TFTC,data*TFTC,statU,subU,respU,extU);}

};

struct theirPred{
  double data ;
  double TFTC ;
  double extU ;
  double respU;
  double subU ;
  double statU;

  void fill(QCDSupport::SRegInfo& srinfo, std::string dirName, QCDSupport::SRegInfo::SRReg reg, int mB){
    std::string fName = dirName + srinfo.convertToTemplateName(srinfo.srRegBinNames[reg].Data(),QCDSupport::metBins[mB]);

    std::string line;
    std::ifstream inFile (fName);
    if(!inFile.is_open())std::cout <<"Could not find: " << fName << std::endl;
    assert(inFile.is_open());


    std::regex lnUnc ("^(\\S+)\\s+lnN\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+(\\S+)\\s*$");
    std::regex gmUnc ("^(\\S+)\\s+gmN\\s+(\\S+)\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+(\\S+)\\s*$");
    std::smatch sm;

    std::vector<double> extUnc;

    auto convFloat = [](float& in, std::string str) -> bool {try {in = stof(str);}catch (const std::invalid_argument& ia) {return false;} return true;};

    while(getline(inFile,line)){
      if (std::regex_match (line,sm,gmUnc)){
        assert(sm.size() == 3);
        bool isGood = true;
        float d = -1;
        float tf = -1;

        if(!TString(sm[1]).BeginsWith("qcd_")) isGood = false;
        if(!convFloat(d,sm[2])) isGood = false;
        if(!convFloat(tf,sm[3])) isGood = false;
        if(isGood){
          data = d;
          TFTC = tf;
        }
      }
      if (std::regex_match (line,sm,lnUnc)){
        assert(sm.size() == 3);
        bool isGood = true;
        float unc = -1;
        try {unc = stof(sm[2]);}catch (const std::invalid_argument& ia) {isGood = false;}
        if(!isGood) continue;
        if(TString(sm[1]).BeginsWith("qcd_bkgsubunc")){
          float v = 0; if(convFloat(v,sm[2])) subU = v;
        }
        else if(TString(sm[1]).BeginsWith("qcd_jetresptailunc")){
          float v = 0; if(convFloat(v,sm[2])) respU = v;
        }
        else if(TString(sm[1]).BeginsWith("qcd_tfstatunc")){
          float v = 0; if(convFloat(v,sm[2])) statU = v;
        }
        else {
          float v = 0; if(convFloat(v,sm[2])) extUnc.push_back(v);
        }
      }
    }
    extU = 0;
    for(unsigned int iU = 0; iU < extUnc.size(); ++iU){
      extU += fabs(extUnc[iU]-1)*fabs(extUnc[iU]-1);
    }
    extU = sqrt(extU) + 1;
  }
  TString print(){return TString::Format("D: %.0f TF: %.5f P: %.3f  +/- %.3f +/- %.3f +/- %.3f +/- %.3f",data,TFTC,data*TFTC,statU,subU,respU,extU);}

};


void comp(std::string  dataCardDir){
  QCDSupport::CRegInfo crinfo;
  crinfo.fillOtherBkgSFs("pieces/htmht_tree_skimmed_baseline.root","pieces/nonQCD_tree_skimmed_baseline.root","pieces/qcd_tree_skimmed_baseline.root");
  crinfo.fillCorrectedOtherBkgYields();
  crinfo.fillDataCorr();

  QCDSupport::SRegInfo srinfo;
  srinfo.fillTFVariations("pieces/qcd_tree_skimmed_baseline.root","pieces/qcd_origtree_skimmed_baseline.root");
  srinfo.loadExternUnc(dataCardDir);

  auto floatComp = [] (float a, float b) -> bool {return fabs(a - b)/((a+b)/2) < 0.01 && a*b > 0;};

  for(unsigned int iS = 0; iS < srinfo.nSR; ++iS){
    cout << srinfo.srRegBinNames[iS] << endl;
    for(unsigned int iM = 0; iM < QCDSupport::nMETBins; ++iM){
      cout << QCDSupport::metBins[iM]<<" ";
      theirPred b; b.fill(srinfo,dataCardDir,QCDSupport::SRegInfo::SRReg(iS),iM);
      ourPred a; a.fill(srinfo,crinfo,QCDSupport::SRegInfo::SRReg(iS),iM);
      bool pass = true;
      if(a.data != b.data){ cout << "(Data) "; pass = false;}
      if(!floatComp(a.TFTC,b.TFTC)){ cout << "(TF) "; pass = false;}
      if(!floatComp(a.extU,b.extU)){ cout << "(extU) "; pass = false;}
      if(!floatComp(a.respU,b.respU)){ cout << "(respU) "; pass = false;}
      if(!floatComp(a.subU,b.subU)){ cout << "(subU) "; pass = false;}
      if(!floatComp(a.statU,b.statU)){ cout << "(statU) "; pass = false;}
      cout << a.print() << endl;
      if(!pass) cout << b.print() << endl;
    }

  }
}



#endif

void CompareDataCards(std::string dataCardDir = "extUncs/"){
  comp(dataCardDir);
}
