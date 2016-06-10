#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/DataFormats/interface/PFCandidate.h"
#endif

using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :

  Analyzer(TString fileName, TString treeName, bool isMCTree, TString outPath, TString outName, cfgSet::ConfigSet& cfg) : BaseTreeAnalyzer(fileName, treeName, isMCTree,12345,&cfg), outputPath_(outPath), outputName_(outName) {
    loadTree();
  }

  TString outputPath_;
  TString outputName_;

  TFile *outfile;
  TTree *candtree;

  int   nGenTaus;
  int   nGenHadTaus;
  int   nGenLeptons;
  int   nGenChHads;
  int   nGenChHadsAcc;
  int   njets;
  float pt;
  float mt;
  float mtnophoton;
  float dphimet;
  float dphiw;
  float misset;
  float abseta;
  float absdz;
  float taumva;
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
  float nearphodr;
  float nearphopt;
  bool  gentaumatch;
  float ptmatch;
  float etamatch;

  void  loadVariables();

  void  processVariables();

  void  runEvent();
  
  void  loadTree();
  
  void  out(TString outputName, TString outputPath);

  vector<ExtendedPFCandidate*> extpfcands;

};

void Analyzer::loadVariables()
{

  load(cfgSet::EVTINFO);
  load(cfgSet::AK4JETS);
  load(cfgSet::PFCANDS,PFCandidateReader::LOADRECO | PFCandidateReader::LOADEXTRECO | PFCandidateReader::FILLOBJ | PFCandidateReader::LOADTAUVETOMT);
  if(isMC()) load(cfgSet::GENPARTICLES);

}

void Analyzer::processVariables()
{

  nJets = 0;
  jets.clear();
  jets.reserve(ak4Reader.recoJets.size());
  extpfcands.clear();
  extpfcands.reserve(pfcandReader.extpfcands.size());

  if(evtInfoReader.isLoaded()) {
    run   = evtInfoReader.run;
    lumi  = evtInfoReader.lumi;
    event = evtInfoReader.event;
    nPV   = evtInfoReader.nPV;
    rho   = evtInfoReader.rho;
    met   = &evtInfoReader.met;
  }

  if(ak4Reader.isLoaded())
    for(auto& jet : ak4Reader.recoJets) {
      jets.push_back(&jet);
      if(jet.pt() > 30.0 && fabs(jet.eta()) < 2.4) nJets++;
    }

  if(pfcandReader.isLoaded())
    for(auto& pfc : pfcandReader.extpfcands){
      MomentumF* cand = new MomentumF(pfc.p4());
      if(pfc.nearPhoton().pt() > -1.) cand->setP4(pfc.p4() + pfc.nearPhoton().p4());
      pfc.setMt(JetKinematics::transverseMass(*cand,*met));
      extpfcands.push_back(&pfc);
    }


  if(genParticleReader.isLoaded()){
    genParts.clear();
    genParts.reserve(genParticleReader.genParticles.size());
    for(auto& p : genParticleReader.genParticles) genParts.push_back(&p);
  }

}

void Analyzer::loadTree()
{

  gSystem->mkdir(outputPath_,true);
  TString filename = outputPath_ + "/" + outputName_ + ".root";
  outfile = new TFile(filename, "RECREATE");
  outfile->cd();

  candtree = new TTree("Candidates","Candidates");
  candtree->Branch("nGenHadTaus",&nGenHadTaus,"nGenHadTaus/I");
  candtree->Branch("nGenTaus",&nGenTaus,"nGenTaus/I");
  candtree->Branch("nGenLeptons",&nGenLeptons,"nGenLeptons/I");
  candtree->Branch("nGenChHads",&nGenChHads,"nGenChHads/I");
  candtree->Branch("nGenChHadsAcc",&nGenChHadsAcc,"nGenChHadsAcc/I");
  candtree->Branch("pt",&pt,"pt/F");
  candtree->Branch("njets",&njets,"njets/I");
  candtree->Branch("mt",&mt,"mt/F");
  candtree->Branch("mtnophoton",&mtnophoton,"mtnophoton/F");
  candtree->Branch("dphimet",&dphimet,"dphimet/F");
  candtree->Branch("dphiw",&dphiw,"dphiw/F");
  candtree->Branch("misset",&misset,"misset/F");
  candtree->Branch("abseta",&abseta,"abseta/F");
  candtree->Branch("absdz",&absdz,"absdz/F");
  candtree->Branch("taumva",&taumva,"taumva/F");
  candtree->Branch("chiso0p1",&chiso0p1,"chiso0p1/F");
  candtree->Branch("chiso0p2",&chiso0p2,"chiso0p2/F");
  candtree->Branch("chiso0p3",&chiso0p3,"chiso0p3/F");
  candtree->Branch("chiso0p4",&chiso0p4,"chiso0p4/F");
  candtree->Branch("totiso0p1",&totiso0p1,"totiso0p1/F");
  candtree->Branch("totiso0p2",&totiso0p2,"totiso0p2/F");
  candtree->Branch("totiso0p3",&totiso0p3,"totiso0p3/F");
  candtree->Branch("totiso0p4",&totiso0p4,"totiso0p4/F");
  candtree->Branch("chreliso0p1",&chreliso0p1,"chreliso0p1/F");
  candtree->Branch("chreliso0p2",&chreliso0p2,"chreliso0p2/F");
  candtree->Branch("chreliso0p3",&chreliso0p3,"chreliso0p3/F");
  candtree->Branch("chreliso0p4",&chreliso0p4,"chreliso0p4/F");
  candtree->Branch("totreliso0p1",&totreliso0p1,"totreliso0p1/F");
  candtree->Branch("totreliso0p2",&totreliso0p2,"totreliso0p2/F");
  candtree->Branch("totreliso0p3",&totreliso0p3,"totreliso0p3/F");
  candtree->Branch("totreliso0p4",&totreliso0p4,"totreliso0p4/F");
  candtree->Branch("neartrkdr",&neartrkdr,"neartrkdr/F");
  candtree->Branch("contjetdr",&contjetdr,"contjetdr/F");
  candtree->Branch("contjetcsv",&contjetcsv,"contjetcsv/F");
  candtree->Branch("nearphodr",&nearphodr,"nearphodr/F");
  candtree->Branch("nearphopt",&nearphopt,"nearphopt/F");
  candtree->Branch("gentaumatch",&gentaumatch,"gentaumatch/O");
  candtree->Branch("ptmatch",&ptmatch,"ptmatch/F");
  candtree->Branch("etamatch",&etamatch,"etamatch/F");

}

void Analyzer::runEvent()
{

  nGenTaus = 0;
  nGenHadTaus = 0;
  nGenLeptons = 0;
  nGenChHads = 0;
  nGenChHadsAcc = 0;
  pt = 0.0;
  mt = 0.0;
  mtnophoton = 0.0;
  dphimet = 0.0;
  dphiw = 0.0;
  misset = 0.0;
  abseta = 0.0;
  absdz = 0.0;
  taumva = 0.0;
  chiso0p1 = 0.0;
  chiso0p2 = 0.0;
  chiso0p3 = 0.0;
  chiso0p4 = 0.0;
  totiso0p1 = 0.0;
  totiso0p2 = 0.0;
  totiso0p3 = 0.0;
  totiso0p4 = 0.0;
  chreliso0p1 = 0.0;
  chreliso0p2 = 0.0;
  chreliso0p3 = 0.0;
  chreliso0p4 = 0.0;
  totreliso0p1 = 0.0;
  totreliso0p2 = 0.0;
  totreliso0p3 = 0.0;
  totreliso0p4 = 0.0;
  neartrkdr = 0.0;
  contjetdr = 0.0;
  contjetcsv = 0.0;
  nearphodr = 0.0;
  nearphopt = 0.0;
  gentaumatch = false;
  ptmatch = 0.0;
  etamatch = 0.0;

  vector<const ExtendedPFCandidate*> pfchargedhads;
  vector<const ExtendedPFCandidate*> pfphotons;

  for(const auto* c : extpfcands) {
    if(c->ischargedhadron()) pfchargedhads.push_back(c);
    if(c->isphoton())        pfphotons.push_back(c);
  }

  vector<const GenParticleF*> taudecayprods;
  for(const auto* p : genParts) {
    if(ParticleInfo::isA(ParticleInfo::p_Z0, p) || ParticleInfo::isA(ParticleInfo::p_Wplus, p)) {
      for(unsigned int id = 0; id < p->numberOfDaughters(); id++) {
        if(ParticleInfo::isA(ParticleInfo::p_tauminus, p->daughter(id))) {
          const GenParticleF* tau = p->daughter(id);
          while(ParticleInfo::hasDaughter(tau, tau->pdgId(), true))
            tau = ParticleInfo::findDaughter(tau, tau->pdgId(), true);
          nGenTaus++;
          bool lepdecay = false;
          for(unsigned int itd = 0; itd < tau->numberOfDaughters(); itd++) {
            const GenParticleF* dau = tau->daughter(itd);
            if(ParticleInfo::isA(ParticleInfo::p_eminus, dau) || ParticleInfo::isA(ParticleInfo::p_muminus, dau)) {
              lepdecay = true;
              break;
            }
            if(!ParticleInfo::isANeutrino(dau->pdgId())) {
              if(fabs(dau->pdgId())==211 || fabs(dau->pdgId())==321) {
                while(ParticleInfo::hasDaughter(dau, dau->pdgId(), true))
                  dau = ParticleInfo::findDaughter(dau, dau->pdgId(), true);
                taudecayprods.push_back(dau);
                if(dau->pt() > 10.0 && fabs(dau->eta()) < 2.4) nGenChHadsAcc++;
              }
              else {
                for(unsigned int itgd = 0; itgd < dau->numberOfDaughters(); itgd++) {
                  if(fabs(dau->daughter(itgd)->pdgId())==211 || fabs(dau->daughter(itgd)->pdgId())==321) { 
                    const GenParticleF* gdau = dau->daughter(itgd);
                    while(ParticleInfo::hasDaughter(gdau, gdau->pdgId(), true))
                      gdau = ParticleInfo::findDaughter(gdau, gdau->pdgId(), true);
                    taudecayprods.push_back(gdau);
                    if(gdau->pt() > 10.0 && fabs(gdau->eta()) < 2.4) nGenChHadsAcc++;
                  }
                }
              }
            }
          }
          if(!lepdecay) {
            nGenHadTaus++;
          }
        }
        if(ParticleInfo::isA(ParticleInfo::p_eminus, p->daughter(id)) || ParticleInfo::isA(ParticleInfo::p_muminus, p->daughter(id))) {
          nGenLeptons++;
        }
      }
    }
  }

  nGenChHads = taudecayprods.size();

  misset = met->pt();

  for(const auto* c : pfchargedhads) {

    bool match = false;
    float tmpDr = 0.05;
    float kpt = 0.01;

    for(const auto* genchhad : taudecayprods) {
      float dpt = 0.0;
      if(genchhad->pt()>0.5) dpt = fabs(1.0 - c->pt()/genchhad->pt());
      if((PhysicsUtilities::deltaR(*c, *genchhad) +  kpt*dpt) < tmpDr && dpt < 0.4) {
        tmpDr = PhysicsUtilities::deltaR(*c, *genchhad) + kpt*dpt;
        match = true;
        ptmatch = genchhad->pt();
        etamatch = genchhad->eta();
      }
    }

    if(c->pt() > 10.0 && fabs(c->eta()) < 2.4) {

      pt = std::min(c->pt(),float(300.0));
      njets = nJets;
      mt = c->mt();
//      mtnophoton = JetKinematics::transverseMass(*c, *met);
//      dphimet = fabs(PhysicsUtilities::deltaPhi(*c, *met));

//      MomentumF* W = new MomentumF(c->p4() + met->p4());
//      dphiw = fabs(PhysicsUtilities::deltaPhi(*c, *W));

      abseta       = fabs(c->eta());
      absdz        = fabs(c->dz());
      taumva       = c->taudisc();
      chiso0p1     = std::min(c->chiso0p1(),float(700.0));
      chiso0p2     = std::min(c->chiso0p2(),float(700.0));
      chiso0p3     = std::min(c->chiso0p3(),float(700.0));
      chiso0p4     = std::min(c->chiso0p4(),float(700.0));
//      chreliso0p1  = chiso0p1/pt;
//      chreliso0p2  = chiso0p2/pt;
//      chreliso0p3  = chiso0p3/pt;
//      chreliso0p4  = chiso0p4/pt;
      totiso0p1    = std::min(c->totiso0p1(),float(700.0));
      totiso0p2    = std::min(c->totiso0p2(),float(700.0));
      totiso0p3    = std::min(c->totiso0p3(),float(700.0));
      totiso0p4    = std::min(c->totiso0p4(),float(700.0));
//      totreliso0p1 = totiso0p1/pt;
//      totreliso0p2 = totiso0p2/pt;
//      totreliso0p3 = totiso0p3/pt;
//      totreliso0p4 = totiso0p4/pt;
      neartrkdr    = c->neartrkdr();

      contjetdr  = std::min(float(0.4), c->contjetdr() );
      if(contjetdr < 0.0) contjetdr = 0.0;
      contjetcsv =  c->contjetcsv();
      if(contjetcsv < 0.0) contjetcsv = 0.0;

//      double nearestDR = 10.7;
//      int nearpho_pt1_ind = PhysicsUtilities::findNearestDRDeref(*c, pfphotons, nearestDR, 1e308, 1.0);
//      nearphodr = nearpho_pt1_ind > -1 ? PhysicsUtilities::deltaR(*c, *pfphotons.at(nearpho_pt1_ind)) : -1.0;
//      nearphopt = nearpho_pt1_ind > -1 && nearestDR < 0.05 ? pfphotons.at(nearpho_pt1_ind)->pt() : 0.0;

      if(match && nGenHadTaus > 0) gentaumatch = true;
      else                         gentaumatch = false;

      candtree->Fill();

    }

  }

  pfchargedhads.clear();
  pfphotons.clear();
  taudecayprods.clear();

}

void Analyzer::out(TString outputName, TString outputPath)
{

  outfile->cd();

  candtree->Write();

  outfile->Close();

  delete outfile;

}

void produceCandTree(TString sname = "T2tt_500_325",
                     const int fileindex = -1,
                     const bool isMC = true,
                     const TString fname = "T2tt_500_325_ntuple.root",
                     const TString outputdir = "trees",
                     const TString fileprefix = "root://cmseos:1094/",
                     const TString json=TString::Format("%s/src/data/JSON/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt",getenv("CMSSW_BASE")))
{
  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);



  TString fullname = fileprefix+fname;

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
  // disable JetID for signal samples
  if (sname.Contains("T2tt") || sname.Contains("T2tb") || sname.Contains("T2bW")) cfg.jets.applyJetID = false;

  Analyzer a(fullname, "Events", isMC, outputdir, sname, cfg);

  a.analyze(10000);

  a.out(sname, outputdir);

}
