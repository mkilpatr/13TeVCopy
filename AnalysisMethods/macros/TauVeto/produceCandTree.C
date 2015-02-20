#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/DataFormats/interface/PFCandidate.h"
#endif

using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :

  Analyzer(TString fileName, TString treeName, bool isMCTree, TString outPath, TString outName) : BaseTreeAnalyzer(fileName, treeName, isMCTree), outputPath_(outPath), outputName_(outName) {
    setDefaultJets(AK4JETS);
    loadPlots();
  }


  TString outputPath_;
  TString outputName_;

  TFile *outfile;
  TTree *candtree;


  int nGenTaus;
  int nGenHadTaus;
  int nGenLeptons;
  int   njets;
  float pt;
  float mt;
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

  void loadVariables();

  void processVariables();

  void runEvent();
  
  void loadPlots();
  
  void out(TString outputName, TString outputPath);

  const GenParticleF* findMother(const GenParticleF* p, int pid) const;

  vector<ExtendedPFCandidate*> extpfcands;

};

const GenParticleF* Analyzer::findMother(const GenParticleF* p, int pid) const
{

  const GenParticleF* mother=0;

  for(unsigned int im = 0; im < p->numberOfMothers(); im++) {
    if(ParticleInfo::isA(pid, p->mother(im))) {
      mother = p->mother(im);
      break;
    }
  }

  return mother;

}

void Analyzer::loadVariables()
{

  load(EVTINFO);
  load(AK4JETS);
  load(PFCANDS,PFCandidateReader::LOADRECO | PFCandidateReader::LOADEXTRECO | PFCandidateReader::FILLOBJ);
  if(isMC()) load(GENPARTICLES);

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
    for(auto& pfc : pfcandReader.extpfcands)
      extpfcands.push_back(&pfc);


  if(genParticleReader.isLoaded()){
    genParts.clear();
    genParts.reserve(genParticleReader.genParticles.size());
    for(auto& p : genParticleReader.genParticles) genParts.push_back(&p);
  }

}

void Analyzer::loadPlots()
{

  gSystem->mkdir(outputPath_,true);
  TString filename = outputPath_ + "/" + outputName_ + "_plots.root";
  outfile = new TFile(filename, "RECREATE");
  outfile->cd();

  candtree = new TTree("Candidates","Candidates");
  candtree->Branch("nGenHadTaus",&nGenHadTaus,"nGenHadTaus/I");
  candtree->Branch("nGenTaus",&nGenTaus,"nGenTaus/I");
  candtree->Branch("nGenLeptons",&nGenLeptons,"nGenLeptons/I");
  candtree->Branch("pt",&pt,"pt/F");
  candtree->Branch("njets",&njets,"njets/I");
  candtree->Branch("mt",&mt,"mt/F");
  candtree->Branch("misset",&misset,"misset/F");
  candtree->Branch("abseta",&abseta,"abseta/F");
  candtree->Branch("absdz",&absdz,"absdz/F");
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
  candtree->Branch("gentaumatch",&gentaumatch,"gentaumatch/O");

}

void Analyzer::runEvent()
{

  nGenTaus = 0;
  nGenHadTaus = 0;
  nGenLeptons = 0;
  pt = 0.0;
  mt = 0.0;
  misset = 0.0;
  abseta = 0.0;
  absdz = 0.0;
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
  gentaumatch = false;

  vector<const ExtendedPFCandidate*> pfchargedhads;
  for(const auto* c : extpfcands) {
    if(c->ischargedhadron()) {
      pfchargedhads.push_back(c);
    }
  }

  vector<const GenParticleF*> pions;
  for(const auto* p : genParts) {
    if(ParticleInfo::isA(ParticleInfo::p_Z0, p) || ParticleInfo::isA(ParticleInfo::p_Wplus, p)) {
      for(unsigned int id = 0; id < p->numberOfDaughters(); id++) {
        if(ParticleInfo::isA(ParticleInfo::p_tauminus, p->daughter(id))) {
          nGenTaus++;
          const GenParticleF* tau = p->daughter(id);
          bool lepdecay = false;
          for(unsigned int itd = 0; itd < tau->numberOfDaughters(); itd++) {
            const GenParticleF* dau = tau->daughter(itd);
            if(ParticleInfo::isA(ParticleInfo::p_eminus, dau) || ParticleInfo::isA(ParticleInfo::p_muminus, dau)) {
              lepdecay = true;
              break;
            }
            if(!ParticleInfo::isANeutrino(dau->pdgId())) {
              if(fabs(dau->pdgId())==211) pions.push_back(dau);
              else {
                for(unsigned int itgd = 0; itgd < dau->numberOfDaughters(); itgd++) {
                  if(fabs(dau->daughter(itgd)->pdgId())==211) pions.push_back(dau->daughter(itgd));
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

  misset = met->pt();

  for(unsigned int ic = 0; ic < pfchargedhads.size(); ic++) {
    bool match = false;
    float tmpDr = 0.4;
    for(unsigned int it = 0; it < pions.size(); it++) {
      if(PhysicsUtilities::deltaR(*pfchargedhads.at(ic), *pions.at(it)) < tmpDr) {
        tmpDr = PhysicsUtilities::deltaR(*pfchargedhads.at(ic), *pions.at(it));
        match = true;
      }
    }

    const ExtendedPFCandidate* c = pfchargedhads.at(ic);

    if(c->pt() > 5.0 && fabs(c->eta()) < 2.4) {

      pt = c->pt();
      njets = nJets;
      mt = c->mt();
      abseta = fabs(c->eta());
      absdz = fabs(c->dz());
      chiso0p1 = c->chiso0p1();
      chiso0p2 = c->chiso0p2();
      chiso0p3 = c->chiso0p3();
      chiso0p4 = c->chiso0p4();
      chreliso0p1 = chiso0p1/pt;
      chreliso0p2 = chiso0p2/pt;
      chreliso0p3 = chiso0p3/pt;
      chreliso0p4 = chiso0p4/pt;
      totiso0p1 = c->totiso0p1();
      totiso0p2 = c->totiso0p2();
      totiso0p3 = c->totiso0p3();
      totiso0p4 = c->totiso0p4();
      totreliso0p1 = totiso0p1/pt;
      totreliso0p2 = totiso0p2/pt;
      totreliso0p3 = totiso0p3/pt;
      totreliso0p4 = totiso0p4/pt;
      neartrkdr = c->neartrkdr();
      const RecoJetF* jet = c->jetIndex() > -1 && c->jetIndex() < int(jets.size()) ? jets.at(c->jetIndex()) : 0;
      contjetdr = (c->jetIndex() > -1 && c->jetIndex() < int(jets.size()) &&  jet->pt() > 30.0 && fabs(jet->eta()) < 2.4) ? PhysicsUtilities::deltaR(*c, *jet) : -1.0;
      contjetcsv = (c->jetIndex() > -1 && c->jetIndex() < int(jets.size()) &&  jet->pt() > 30.0 && fabs(jet->eta()) < 2.4) ? jet->csv() : -1.0;
      if(match && nGenHadTaus > 0) gentaumatch = true;
      else gentaumatch = false;

      candtree->Fill();

    }

  }

  pfchargedhads.clear();
  pions.clear();

}

void Analyzer::out(TString outputName, TString outputPath)
{

  outfile->cd();

  candtree->Write();

  outfile->Close();

  delete outfile;

}

void produceCandTree(TString sname = "test",
                     const int fileindex = -1,
                     const bool isMC = true,
                     const TString fname = "output_3_T2tt_850_100_numEvent20000.root",
                     const double xsec = 1.0,
                     const TString outputdir = "../run/plots",
                     const TString fileprefix = "root://eoscms//eos/cms/store/user/vdutta/13TeV/040215/")
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

  Analyzer a(fullname, "TestAnalyzer/Events", isMC, outputdir, sname);

  a.analyze(1000);

  a.out(sname, outputdir);

}
