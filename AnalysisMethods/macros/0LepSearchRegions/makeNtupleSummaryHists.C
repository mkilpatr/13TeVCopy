#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class InclusiveAnalyzer : public TreeCopierManualBranches {

  public :

    InclusiveAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      TreeCopierManualBranches(fileName, treeName, outfileName, randSeed, isMCTree, pars) {}

    // booking
    TH1D* hsumweights = nullptr;
    TH1D* hpartonht = nullptr;
    TH1D* hgenht = nullptr;
    TH1D* hrecoht = nullptr;
    TH1D* hgenmet = nullptr;
    TH1D* hrecomet = nullptr;

    void book() {
      outFile_->cd();
      hsumweights = new TH1D("hsumweights", ";;Events", 1, 0, 10);
      hpartonht = new TH1D("hpartonht", ";parton HT;Events", 300, 0, 3000);
      hgenht = new TH1D("hgenht", ";gen HT;Events", 300, 0, 3000);
      hrecoht = new TH1D("hrecoht", ";reco HT;Events", 300, 0, 3000);
      hgenmet = new TH1D("hgenmet", ";gen MET;Events", 200, 0, 2000);
      hrecomet = new TH1D("hrecomet", ";reco MET;Events", 200, 0, 2000);
    }

    void loadVariables()
    {
      load(cfgSet::EVTINFO);
      load(cfgSet::AK4JETS, JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN : JetReader::NULLOPT) | JetReader::LOADJETEXTRA);
      if(isMC()) load(cfgSet::GENPARTICLES);
    }


    bool fillEvent() {
      hsumweights->Fill(1, weight);

      vector<GenParticleF*> partons;
      for (auto *p : genParts){
        if (ParticleInfo::isDocOutgoing(p->status()) && ParticleInfo::isQuarkOrGluon(p->pdgId()))
          partons.push_back(p);
      }
      hpartonht->Fill(JetKinematics::ht(partons), weight);

      vector<GenJetF*> genJets;
      for (auto &j : defaultJets->genJets) {
        if(cfgSet::isSelGenJet(j,configSet.jets))
          genJets.push_back(&j);
      }
      hgenht->Fill(JetKinematics::ht(genJets), weight);

      hrecoht->Fill(JetKinematics::ht(jets), weight);
      hgenmet->Fill(genmet->pt(), weight);
      hrecomet->Fill(met->pt(), weight);

      return true;
    }


};

void makeNtupleSummaryHists(TString sname = "dyjetstoll_cr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/vdutta/13TeV/150715/74X/merged/dyjetstoll_1_ntuple_wgtxsec.root",
                                 const TString outputdir = "trees",
                                 const TString fileprefix = "root://eoscms//eos/cms",
                                 const TString json="")
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;
  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  cfgSet::ConfigSet pars = pars0lep(json);

  InclusiveAnalyzer a(fullname, "Events", outfilename, fileindex+2, isMC, &pars);
  a.analyze(100000);
//  a.analyze(100000, 10000);


}
