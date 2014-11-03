//--------------------------------------------------------------------------------------------------
// 
// BaseTreeAnalyzer
// 
//
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
BaseTreeAnalyzer::BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree, TString readOption) : reader(fileName,treeName,readOption),  isMC(isMCTree)
    , eventInfo(0)
    , ak4Jets (0)
    , ak4GenJets(0)
    , electrons(0)
    , muons(0)
    , taus(0)
{
  cout << "Running over: " << (isMC ? "MC" : "data") <<endl;
}

//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::load(VarType type, int options, string branchName)
{
  switch (type) {
    case EVTINFO : {
      reader.load(&evtInfoReader, 0, "");
      eventInfo = &evtInfoReader.evtInfo;
      break;
    }
    case AK4JETS : {
      int defaultOptions = JetReader::defaultOptions | (isMC ? JetReader::LOADGEN : JetReader::NULLOPT);
      reader.load(&ak4Reader, options < 0 ? defaultOptions : options, branchName == "" ? "ak4pfchs" : branchName);
      ak4Jets = &ak4Reader.recoJets;
      ak4GenJets = &ak4Reader.genJets;
      break;
    }
    case ELECTRONS : {
      int defaultOptions = ElectronReader::defaultOptions;
      reader.load(&electronReader, options < 0 ? defaultOptions : options, branchName == "" ? "ele" : branchName );
      electrons = &electronReader.electrons;
      break;
    }
    case MUONS : {
      int defaultOptions = MuonReader::defaultOptions;
      reader.load(&muonReader, options < 0 ? defaultOptions : options, branchName == "" ? "mu" : branchName );
      muons = &muonReader.muons;
      break;
    }
    case TAUS : {
      int defaultOptions = TauReader::defaultOptions;
      reader.load(&tauReader, options < 0 ? defaultOptions : options, branchName == "" ? "tau" : branchName );
      taus = &tauReader.taus;
      break;
    }
    default : {
      cout << endl << "No settings for type: " << type << " found!"<<endl;
      break;
    }
  }
}

//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::nextEvent(int reportFrequency)
{
  return reader.nextEvent(reportFrequency);
}
