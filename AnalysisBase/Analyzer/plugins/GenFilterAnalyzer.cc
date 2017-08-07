#include <memory>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/GeneratorProducts/interface/GenLumiInfoHeader.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"

#include "TTree.h"
#include "TString.h"


class GenFilterAnalyzer : public edm::EDAnalyzer {

public:
  GenFilterAnalyzer(const edm::ParameterSet&);
  virtual ~GenFilterAnalyzer();

private:
     virtual void beginJob() override {}
     virtual void analyze(const edm::Event&, const edm::EventSetup&) override {}
     virtual void endJob() override {}

  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  bool printGenLumiInfo = false;

  edm::EDGetTokenT<GenLumiInfoHeader> genLumiHeaderToken_;
  edm::EDGetTokenT<GenFilterInfo>     genFilterInfoToken_;

  TTree *tree_;

  std::vector<int> massparams_;
  double sumPassWeights_ = 0;
  double sumWeights_ = 0;
};

//---------------------------------------------------------------------
GenFilterAnalyzer::GenFilterAnalyzer(const edm::ParameterSet& iConfig) :
        printGenLumiInfo    (iConfig.getUntrackedParameter<bool>("printGenLumiInfo",false)),
        genLumiHeaderToken_ (consumes<GenLumiInfoHeader,edm::InLumi>(iConfig.getParameter<edm::InputTag>("genLumiInfo"))),
        genFilterInfoToken_ (consumes<GenFilterInfo,edm::InLumi>(iConfig.getParameter<edm::InputTag>("genFilterInfo")))
{
  // Create and register TTree
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("GenFilter", "");
  tree_->Branch("massparams", &massparams_);

  tree_->Branch("sumPassWeights", &sumPassWeights_, "sumPassWeights/D");
  tree_->Branch("sumWeights",     &sumWeights_,     "sumWeights/D");
}

//---------------------------------------------------------------------
GenFilterAnalyzer::~GenFilterAnalyzer() {
}

//---------------------------------------------------------------------
void GenFilterAnalyzer::endLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& es)
{

  massparams_.clear();
  sumPassWeights_ = 0;
  sumWeights_ = 0;

  edm::Handle<GenLumiInfoHeader> gen_header;
  iLumi.getByToken(genLumiHeaderToken_, gen_header);
  auto model = gen_header->configDescription();

  TString massstring(model);
  // model string examples:
  // T2tt_dM-10to80_genHT-160_genMET-80_400_350
  // T2tt_525_325
  while(massstring.Index("_") != TString::kNPOS) {
    massstring.Remove(0,massstring.Index("_")+1);
    TString mstr;
    if(massstring.Index("_") != TString::kNPOS)
      mstr = TString(massstring(0,massstring.First('_')));
    else
      mstr = TString(massstring(0,massstring.Length()));
    if(!mstr.IsDigit()) continue;
    massparams_.push_back(mstr.Atoi());
  }

  edm::Handle<GenFilterInfo> genFilter;
  iLumi.getByToken(genFilterInfoToken_,genFilter);
  sumPassWeights_ = genFilter->sumPassWeights();
  sumWeights_     = genFilter->sumWeights();

  tree_->Fill();

  if(!printGenLumiInfo) return;
  std::cout << "Printing GenFilterInfo!" << std::endl;
  std::cout << model << std::endl;
  std::cout << "Total = " << genFilter->sumWeights() << " Passed = " << genFilter->sumPassWeights() << std::endl;
  std::cout << " ============ " << std::endl;

}

DEFINE_FWK_MODULE(GenFilterAnalyzer);
