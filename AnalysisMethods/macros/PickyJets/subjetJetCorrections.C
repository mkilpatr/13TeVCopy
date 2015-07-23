#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TSystem.h"
#include "TMath.h"
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#endif

using namespace ucsbsusy;

class Analyzer : public TreeCopierManualBranches {

  public :

    Analyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, cfgSet::ConfigSet * pars) :
      TreeCopierManualBranches(fileName, treeName, outfileName, isMCTree, pars)
    {
      corrections = {0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45};
      ptbins = {20.0, 30.0, 40.0, 50.0, 60.0, 80.0, 100.0, 150.0, 200.0, 300.0, 500.0, 1000.0};
      etabins = {0.0, 1.5, 2.4};
    }

    virtual ~Analyzer() {}

    vector<float> corrections;
    vector<float> ptbins;
    vector<float> etabins;

    void book();
    bool fillEvent();

    size i_npv;
    size i_rho;
    size i_nj20_gen;
    size i_nj30_gen;
    size i_nj40_gen;
    size i_nj50_gen;
    size i_nj60_gen;
    vector<size> i_njptetabins_gen;
    size i_nb20_gen;
    size i_nb30_gen;
    size i_nb40_gen;
    size i_nb50_gen;
    size i_nb60_gen;
    map<TString,size> i_nj20_reco;
    map<TString,size> i_nj30_reco;
    map<TString,size> i_nj40_reco;
    map<TString,size> i_nj50_reco;
    map<TString,size> i_nj60_reco;
    map<TString,vector<size> > i_njptetabins_reco;
    map<TString,size> i_jptcorr_reco;
    size i_jpt_reco;
    size i_jeta_reco;
    size i_jphi_reco;
    size i_jmass_reco;
    size i_jarea_reco;
    size i_jpt_gen;
    size i_jeta_gen;
    size i_jphi_gen;
    size i_jmass_gen;

};

void Analyzer::book()
{

  i_npv       = data.add<int>("","npv","I",0);
  i_rho       = data.add<float>("","rho","F",0);
  i_nj20_gen  = data.add<int>("gen","nj20","I",0);
  i_nj30_gen  = data.add<int>("gen","nj30","I",0);
  i_nj40_gen  = data.add<int>("gen","nj40","I",0);
  i_nj50_gen  = data.add<int>("gen","nj50","I",0);
  i_nj60_gen  = data.add<int>("gen","nj60","I",0);
  i_nb20_gen  = data.add<int>("gen","nb20","I",0);
  i_nb30_gen  = data.add<int>("gen","nb30","I",0);
  i_nb40_gen  = data.add<int>("gen","nb40","I",0);
  i_nb50_gen  = data.add<int>("gen","nb50","I",0);
  i_nb60_gen  = data.add<int>("gen","nb60","I",0);

  for(unsigned int iptbin = 0; iptbin < ptbins.size() - 1; iptbin++) {
    for(unsigned int ietabin = 0; ietabin < etabins.size() - 1; ietabin++) {
      TString ptetalabel = TString::Format("nj_pt%4.2fto%4.2f_eta%4.2fto%4.2f", ptbins[iptbin], ptbins[iptbin+1], etabins[ietabin], etabins[ietabin+1]);
      i_njptetabins_gen.push_back(data.add<int>("gen",string(ptetalabel.Data()),"I",0));
    }
  }

  for(auto corr : corrections) {
    TString label = TString::Format("reco_corr%4.2f",corr);
    string labelstr = label.Data();
    i_nj20_reco[label] = data.add<int>(labelstr,"nj20","I",0);
    i_nj30_reco[label] = data.add<int>(labelstr,"nj30","I",0);
    i_nj40_reco[label] = data.add<int>(labelstr,"nj40","I",0);
    i_nj50_reco[label] = data.add<int>(labelstr,"nj50","I",0);
    i_nj60_reco[label] = data.add<int>(labelstr,"nj60","I",0);
    i_jptcorr_reco[label] = data.addMulti<float>(labelstr,"jptcorr",0);
    for(unsigned int iptbin = 0; iptbin < ptbins.size() - 1; iptbin++) {
      for(unsigned int ietabin = 0; ietabin < etabins.size() - 1; ietabin++) {
        TString ptetalabel = TString::Format("nj_pt%4.2fto%4.2f_eta%4.2fto%4.2f", ptbins[iptbin], ptbins[iptbin+1], etabins[ietabin], etabins[ietabin+1]);
        i_njptetabins_reco[label].push_back(data.add<int>(labelstr,string(ptetalabel.Data()),"I",0));
      }
    }
  }
  i_jpt_reco   = data.addMulti<float>("reco","jpt",0);
  i_jeta_reco  = data.addMulti<float>("reco","jeta",0);
  i_jphi_reco  = data.addMulti<float>("reco","jphi",0);
  i_jmass_reco = data.addMulti<float>("reco","jmass",0);
  i_jarea_reco = data.addMulti<float>("reco","jarea",0);
  i_jpt_gen    = data.addMulti<float>("gen","jpt",0);
  i_jeta_gen   = data.addMulti<float>("gen","jeta",0);
  i_jphi_gen   = data.addMulti<float>("gen","jphi",0);
  i_jmass_gen  = data.addMulti<float>("gen","jmass",0);

}

bool Analyzer::fillEvent()
{

  data.fill<int>(i_npv, nPV);
  data.fill<float>(i_rho, rho);

  vector<int> nj20(corrections.size(), 0);
  vector<int> nj30(corrections.size(), 0);
  vector<int> nj40(corrections.size(), 0);
  vector<int> nj50(corrections.size(), 0);
  vector<int> nj60(corrections.size(), 0);
  int nptetabins = int(ptbins.size() - 1) * int(etabins.size() - 1) * int(corrections.size());
  vector<int> njptetabins(nptetabins, 0);
  int iptetabin = 0;

  for(unsigned int ij = 0; ij < caSubJetReader.recoJets.size(); ++ij) {
    const auto& j = caSubJetReader.recoJets.at(ij);
    if(fabs(j.eta()) > 2.4) continue;

    data.fillMulti<float>(i_jpt_reco, j.pt());
    data.fillMulti<float>(i_jeta_reco, j.eta());
    data.fillMulti<float>(i_jphi_reco, j.phi());
    data.fillMulti<float>(i_jmass_reco, j.mass());
    data.fillMulti<float>(i_jarea_reco, caSubJetReader.jetarea_->at(ij));

    iptetabin = 0;
    for(unsigned int icorr = 0; icorr < corrections.size(); ++icorr) {
      TString label = TString::Format("reco_corr%4.2f",corrections[icorr]);
      float correction = corrections[icorr];
      float corrjetpt = TMath::Max((j.pt() - rho*correction*caSubJetReader.jetarea_->at(ij)), 0.00);
      data.fillMulti<float>(i_jptcorr_reco[label], corrjetpt);

      if(corrjetpt > 20.0) {
        nj20[icorr]++;
      }
      if(corrjetpt > 30.0) {
        nj30[icorr]++;
      }
      if(corrjetpt > 40.0) {
        nj40[icorr]++;
      }
      if(corrjetpt > 50.0) {
        nj50[icorr]++;
      }
      if(corrjetpt > 60.0) {
        nj60[icorr]++;
      }
      for(unsigned int iptbin = 0; iptbin < ptbins.size() - 1; iptbin++) {
        for(unsigned int ietabin = 0; ietabin < etabins.size() - 1; ietabin++) {
          if(corrjetpt > ptbins[iptbin] && corrjetpt <= ptbins[iptbin+1] && fabs(j.eta()) > etabins[ietabin] && fabs(j.eta()) <= etabins[ietabin+1]) njptetabins[iptetabin]++;
          iptetabin++;
        }
      }
    }
  }

  iptetabin = 0;
  for(unsigned int icorr = 0; icorr < corrections.size(); ++icorr) {
    TString label = TString::Format("reco_corr%4.2f",corrections[icorr]);
    data.fill<int>(i_nj20_reco[label], nj20[icorr]);
    data.fill<int>(i_nj30_reco[label], nj30[icorr]);
    data.fill<int>(i_nj40_reco[label], nj40[icorr]);
    data.fill<int>(i_nj50_reco[label], nj50[icorr]);
    data.fill<int>(i_nj60_reco[label], nj60[icorr]);
    int ilocalbin = 0;
    for(unsigned int iptbin = 0; iptbin < ptbins.size() - 1; iptbin++) {
      for(unsigned int ietabin = 0; ietabin < etabins.size() - 1; ietabin++) {
        data.fill<int>(i_njptetabins_reco[label][ilocalbin], njptetabins[iptetabin]);
        ilocalbin++;
        iptetabin++;
      }
    }
  }

  int ngj20 = 0, ngj30 = 0, ngj40 = 0, ngj50 = 0, ngj60 = 0;
  int ngb20 = 0, ngb30 = 0, ngb40 = 0, ngb50 = 0, ngb60 = 0;
  nptetabins = int(ptbins.size() - 1) * int(etabins.size() - 1);
  njptetabins.resize(nptetabins, 0);

  for(unsigned int ij = 0; ij < caSubJetReader.genJets.size(); ++ij) {
    const auto& j = caSubJetReader.genJets.at(ij);
    if(fabs(j.eta()) > 2.4) continue;

    data.fillMulti<float>(i_jpt_gen, j.pt());
    data.fillMulti<float>(i_jeta_gen, j.eta());
    data.fillMulti<float>(i_jphi_gen, j.phi());
    data.fillMulti<float>(i_jmass_gen, j.mass());

    if(j.pt() > 20.0) {
      ngj20++;
      if(j.flavor() == JetFlavorInfo::b_jet)
        ngb20++;
    }
    if(j.pt() > 30.0) {
      ngj30++;
      if(j.flavor() == JetFlavorInfo::b_jet)
        ngb30++;
    }
    if(j.pt() > 40.0) {
      ngj40++;
      if(j.flavor() == JetFlavorInfo::b_jet)
        ngb40++;
    }
    if(j.pt() > 50.0) {
      ngj50++;
      if(j.flavor() == JetFlavorInfo::b_jet)
        ngb50++;
    }
    if(j.pt() > 60.0) {
      ngj60++;
      if(j.flavor() == JetFlavorInfo::b_jet)
        ngb60++;
    }
    iptetabin = 0;
    for(unsigned int iptbin = 0; iptbin < ptbins.size() - 1; iptbin++) {
      for(unsigned int ietabin = 0; ietabin < etabins.size() - 1; ietabin++) {
        if(j.pt() > ptbins[iptbin] && j.pt() <= ptbins[iptbin+1] && fabs(j.eta()) > etabins[ietabin] && fabs(j.eta()) <= etabins[ietabin+1]) njptetabins[iptetabin]++;
        iptetabin++;
      }
    }
  }

  iptetabin = 0;
  data.fill<int>(i_nj20_gen, ngj20);
  data.fill<int>(i_nj30_gen, ngj30);
  data.fill<int>(i_nj40_gen, ngj40);
  data.fill<int>(i_nj50_gen, ngj50);
  data.fill<int>(i_nj60_gen, ngj60);
  data.fill<int>(i_nb20_gen, ngb20);
  data.fill<int>(i_nb30_gen, ngb30);
  data.fill<int>(i_nb40_gen, ngb40);
  data.fill<int>(i_nb50_gen, ngb50);
  data.fill<int>(i_nb60_gen, ngb60);
  for(unsigned int iptbin = 0; iptbin < ptbins.size() - 1; iptbin++) {
    for(unsigned int ietabin = 0; ietabin < etabins.size() - 1; ietabin++) {
      data.fill<int>(i_njptetabins_gen[iptetabin], njptetabins[iptetabin]);
      iptetabin++;
    }
  }

  return true;

}

void subjetJetCorrections(TString sname            = "T2tt_850_100",
                          const int fileindex      = -1,
                          const bool isMC          = true,
                          const TString fname      = "/store/user/vdutta/13TeV/290615/merged/T2tt_850_100_ntuple_wgtxsec.root",
                          const double xsec        = 1.0,
                          const TString outputdir  = "trees",
                          const TString fileprefix = "root://eoscms//eos/cms")
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;

  Analyzer a(fullname, "Events", outfilename, isMC, &cfg);

  a.analyze(100000);

}
