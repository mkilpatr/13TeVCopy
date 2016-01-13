#ifndef REGIONINFO_H
#define REGIONINFO_H

struct cutInfo {
  cutInfo(){
    name = "";
    dscR = "";
    dscL = "";
    defn = "";
  }
  cutInfo(TString p_name, TString p_dscR, TString p_dscL, TString p_defn){
    name = p_name;
    dscR = p_dscR;
    dscL = p_dscL;
    defn = p_defn;
  }
  TString name;
  TString dscR;
  TString dscL;
  TString defn;
};

struct subRegionInfo {
  subRegionInfo(){
    common_CI  = cutInfo();
    MET_CI     = cutInfo();
    nJets_CI   = cutInfo();
    MT_CI      = cutInfo();
    hT_CI      = cutInfo();
    dPhi12_CI  = cutInfo();
    dPhi3_CI   = cutInfo();
    nJets75_CI = cutInfo();
    nlBjets_CI = cutInfo();
    nBjets_CI  = cutInfo();
    nTtags_CI  = cutInfo();
  }
  cutInfo common_CI;
  cutInfo MET_CI;
  cutInfo nJets_CI;
  cutInfo MT_CI;
  cutInfo hT_CI;
  cutInfo dPhi12_CI;
  cutInfo dPhi3_CI;
  cutInfo nJets75_CI;
  cutInfo nlBjets_CI;
  cutInfo nBjets_CI;
  cutInfo nTtags_CI;

  TString getName(){ return common_CI.name + MET_CI.name + nJets_CI.name + MT_CI.name + hT_CI.name + dPhi12_CI.name + dPhi3_CI.name + nJets75_CI.name + nlBjets_CI.name + nBjets_CI.name + nTtags_CI.name; }
  TString getDscR(){ return common_CI.dscR + MET_CI.dscR + (nJets_CI.dscR.Length()>0 ? ", ":"") + nJets_CI.dscR + (MT_CI.dscR.Length()>0 ? ", ":"") + MT_CI.dscR + (hT_CI.dscR.Length()>0 ? ", ":"") + hT_CI.dscR + (dPhi12_CI.dscR.Length()>0 ? ", ":"") + dPhi12_CI.dscR +
                     (dPhi3_CI.dscR.Length()>0 ? ", ":"") + dPhi3_CI.dscR + (nJets75_CI.dscR.Length()>0 ? ", ":"") + nJets75_CI.dscR + (nlBjets_CI.dscR.Length()>0 ? ", ":"") + nlBjets_CI.dscR + (nBjets_CI.dscR.Length()>0 ? ", ":"") + nBjets_CI.dscR + (nTtags_CI.dscR.Length()>0 ? ", ":"") + nTtags_CI.dscR; }
  TString getDscL(){ return common_CI.dscL + MET_CI.dscL + (nJets_CI.dscL.Length()>0 ? ", ":"") + nJets_CI.dscL + (MT_CI.dscL.Length()>0 ? ", ":"") + MT_CI.dscL + (hT_CI.dscL.Length()>0 ? ", ":"") + hT_CI.dscL + (dPhi12_CI.dscL.Length()>0 ? ", ":"") + dPhi12_CI.dscL +
                     (dPhi3_CI.dscL.Length()>0 ? ", ":"") + dPhi3_CI.dscL + (nJets75_CI.dscL.Length()>0 ? ", ":"") + nJets75_CI.dscL + (nlBjets_CI.dscL.Length()>0 ? ", ":"") + nlBjets_CI.dscL + (nBjets_CI.dscL.Length()>0 ? ", ":"") + nBjets_CI.dscL + (nTtags_CI.dscL.Length()>0 ? ", ":"") + nTtags_CI.dscL; }
  TString getDefn(){ return common_CI.defn + MET_CI.defn + nJets_CI.defn + MT_CI.defn + hT_CI.defn + dPhi12_CI.defn + dPhi3_CI.defn + nJets75_CI.defn + nlBjets_CI.defn + nBjets_CI.defn + nTtags_CI.defn; }
};

class RegionInfo {
  public:
    RegionInfo(){
      CR = subRegionInfo();
      TR = subRegionInfo();
      SR = subRegionInfo();
    }
    ~RegionInfo(){};
    subRegionInfo CR;
    subRegionInfo TR;
    subRegionInfo SR;
    TString uniqueName;

    TString getName_CR(){ return CR.getName(); }
    TString getName_TR(){ return TR.getName(); }
    TString getName_SR(){ return SR.getName(); }

    TString getDscR_CR(){ return CR.getDscR(); }
    TString getDscR_TR(){ return TR.getDscR(); }
    TString getDscR_SR(){ return SR.getDscR(); }

    TString getDefn_CR(){ return CR.getDefn(); }
    TString getDefn_TR(){ return TR.getDefn(); }
    TString getDefn_SR(){ return SR.getDefn(); }
};

#endif
