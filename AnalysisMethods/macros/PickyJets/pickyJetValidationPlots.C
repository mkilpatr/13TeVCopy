#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

#include "AnalysisTools/KinematicVariables/interface/Topness.h"
using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  JetReader                       trimmedJetReader;



  double getCorrectedPT(double pt, double eta, double rho, double area, bool getX = false){
    double origPT = pt;

    static Space * etaAxis=new BinnedSpace("jet_eta","jet_eta","-5.191,-4.889,-4.716,-4.538,-4.363,-4.191,-4.013,-3.839,-3.664,-3.489,-3.314,-3.139,-2.964,-2.853,-2.65,-2.5,-2.322,-2.172,-2.043,-1.93,-1.83,-1.74,-1.653,-1.566,-1.479,-1.392,-1.305,-1.218,-1.131,-1.044,-0.957,-0.879,-0.783,-0.696,-0.609,-0.522,-0.435,-0.348,-0.261,-0.174,-0.087,0,0.087,0.174,0.261,0.348,0.435,0.522,0.609,0.696,0.783,0.879,0.957,1.044,1.131,1.218,1.305,1.392,1.479,1.566,1.653,1.74,1.83,1.93,2.043,2.172,2.322,2.5,2.65,2.853,2.964,3.139,3.314,3.489,3.664,3.839,4.013,4.191,4.363,4.538,4.716,4.889,5.191");
//    static PopulationD * param0=new PopulationD(const_cast<Space*>(etaAxis),"5.80374,5.79249,5.33837,5.89629,6.4806,6.84991,6.89468,6.75551,6.52702,6.03406,7.50215,10.5427,12.3348,14.8732,14.8737,11.8231,7.11906,5.39837,4.78075,4.81133,5.02165,5.26615,5.6064,5.90175,6.15217,6.31113,6.50699,6.56383,6.56177,6.55793,6.52668,6.5067,6.45947,6.59731,6.57921,6.67241,6.7152,6.71477,6.66187,6.7291,6.74472,6.68365,6.7153,6.70684,6.68933,6.67399,6.61825,6.59131,6.55567,6.50197,6.52004,6.53632,6.56191,6.7241,6.64683,6.54017,6.4353,6.09415,5.95564,5.61219,5.30913,4.98636,4.94411,4.8368,5.81708,6.91859,11.1045,14.948,15.1071,12.4915,10.5759,7.47999,5.96355,6.52512,6.80792,6.93588,6.84042,6.49384,5.99257,5.24862,5.58339,5.8655");
//    static PopulationD * param1=new PopulationD(const_cast<Space*>(etaAxis),"0.0877693,0.0840039,0.0837556,0.439034,0.536651,0.471043,0.583289,0.679734,0.590336,0.761466,1.08624,1.36103,1.70189,2.38381,4.72121,4.10771,2.20969,1.332,1.12879,0.743226,0.735993,0.484375,0.353659,0.59515,0.413376,0.670206,0.410376,0.636037,0.618069,0.604695,0.66732,0.64754,0.665056,0.682621,0.653062,0.681031,0.698904,0.673723,0.734668,0.754793,0.83974,0.655629,0.525876,0.576349,0.709956,0.643882,0.69791,0.63303,0.579431,0.734357,0.692018,0.69076,0.696977,0.913388,0.578298,0.686146,0.564452,0.444948,0.523455,0.416071,0.562469,0.355566,0.549776,0.986211,1.29311,2.0358,3.89191,5.14695,2.40821,2.12674,1.33809,1.05996,0.589024,0.620672,0.5587,0.62418,0.602361,0.39103,0.363522,0.543764,0.691407,0.521286");
//    static PopulationD * param2=new PopulationD(const_cast<Space*>(etaAxis),"5.0,5.0,5.0,0.246506,0.0732016,0.253929,0.0947329,-0.0451513,0.0382665,-0.116839,-0.180008,-0.197401,-0.207488,-0.286412,-0.387388,-0.392088,-0.466234,-0.456536,-0.448648,-0.328321,-0.251234,-0.0721843,0.191019,-0.120102,0.187168,-0.124884,0.26401,-0.056479,-0.0425166,-0.0138093,-0.0592946,-0.0821006,-0.0840624,-0.0828264,-0.0463363,-0.0477217,-0.0697123,-0.0566864,-0.0937715,-0.103413,-0.130816,-0.0271886,0.0937068,0.0661343,-0.0907015,-0.0352237,-0.0947987,-0.00689072,0.0256929,-0.111265,-0.0790057,-0.0964707,-0.0976838,-0.197391,-0.00755874,-0.101485,0.0241364,0.0461376,-0.0306221,0.117529,-0.139029,0.162107,-0.146311,-0.417145,-0.357271,-0.510059,-0.438052,-0.419728,-0.274514,-0.321133,-0.183075,-0.16823,-0.0076001,0.00226411,0.109245,0.0784045,0.0277927,0.426055,0.514765,-0.0582762,-0.197286,0.132692");
//    static PopulationD * param2=new PopulationD(const_cast<Space*>(etaAxis),"5.0,5.0,5.0,0.246506,0.0732016,0.253929,0.0947329, -1");
    static double param0[] = {6.07418,5.15375,5.21246,6.12574,6.67726,7.00043,7.22386,7.06988,7.17471,6.16199,5.9693,11.8487,14.6314,17.6736,19.1965,12.0646,8.16139,6.02472,5.15018,4.61933,4.70834,5.08853,5.572,5.97104,6.35205,6.51533,6.69676,6.61382,6.62988,6.45385,6.5249,6.55317,6.54732,6.74962,6.69582,6.69947,6.709,6.73185,6.81464,6.83419,6.91027,6.87599,6.7878,6.82904,6.71373,6.91727,6.74466,6.66631,6.69642,7.11045,6.5068,6.58787,6.52693,6.6271,6.6723,6.78162,6.58198,6.47172,6.01714,5.48926,5.08035,4.62442,4.60948,5.72999,6.61412,8.57174,12.7483,19.1806,17.9546,14.4508,11.7781,5.68545,6.18164,6.91189,7.11475,7.13811,7.08079,6.7743,6.19774,5.17197,4.92862,5.80904};
    static double param1[] = {0.114417,0.487694,0.585868,0.488438,0.678986,0.75777,0.60215,0.699316,0.192006,0.703243,0.823317,1.98165,3.35002,2.75136,6.71502,3.7598,2.11781,1.86733,1.12858,1.06233,0.766855,0.587702,0.576011,0.661777,0.653329,0.644837,0.705396,0.765774,0.702078,0.695139,0.612245,0.689977,0.677429,0.639489,0.667519,0.653949,0.734078,0.827617,0.810874,0.67904,0.776252,0.709501,0.543145,0.539685,0.676761,0.142253,0.600761,0.703504,0.531818,0.116484,0.737786,0.751709,0.779697,1.10763,0.803209,0.672577,0.656185,0.899339,0.654743,0.621197,0.638796,0.929173,0.822719,1.27118,0.345991,0.752,1.76532,6.48354,3.41832,1.81245,1.98542,0.762833,0.642595,0.733433,0.673233,0.660068,0.792063,0.543513,0.592016,0.554196,0.0881767,0.815195};
    static double param2[] = {5,0.159868,-0.0502729,0.229241,-0.0435546,-0.0801944,0.178451,-0.00418214,2.16851,-0.0739024,-0.152985,-0.279648,-0.438643,-0.336321,-0.403359,-0.457656,-0.462388,-0.524161,-0.363611,-0.424495,-0.30023,-0.148412,-0.130704,-0.168526,-0.0915198,-0.0302764,-0.0695301,-0.144146,-0.0726498,-0.115177,-0.0205677,-0.0884894,-0.0867033,0.00301077,-0.0493622,0.0200383,-0.070214,-0.159415,-0.103223,-0.0518786,-0.0682988,-0.0379111,0.141792,0.138642,-0.0382789,2.51583,0.0433857,-0.0704134,0.121908,5,-0.10904,-0.125988,-0.168878,-0.313798,-0.165923,-0.0663029,-0.0792149,-0.180202,-0.155943,-0.209821,-0.183637,-0.388182,-0.359007,-0.277703,0.765385,-0.181316,-0.160317,-0.409864,-0.405827,-0.247538,-0.27456,-0.187935,-0.0515478,-0.0469459,0.020868,0.0463074,-0.0821376,0.221005,0.0567584,-0.0662956,5,-0.277169};

    if(pt < 1) return pt;
    if(pt > 3500) return pt;

//    pt = max(10.,min(3000.,pt));
    area = min(10.,area);
    rho = min(200.,rho);

    double p0 = param0[etaAxis->findBin(eta)];
    double p1 = param1[etaAxis->findBin(eta)];
    double p2 = param2[etaAxis->findBin(eta)];



//    return origPT * max(0.0001,1.0-area*((p1*rho)*(1+p2*log(pt)))/pt);
    if(getX){
      return area*((p1*rho)*(1+p2*log(pt)));
    } else
    return origPT * max(0.0001,1.0-.35*area*(p0+(p1*rho)*(1+p2*log(pt)))/pt);

//    cout << pt  << " " <<eta<<" "<<area<<" "<<rho <<" "<<p0 <<" "<<p1 <<" "<<p2<<" "<<1.0-area*(p0+(p1*rho)*(1+p2*log(pt)))/pt <<endl;
//    return origPT * max(0.0001,1.0-area*(p0+(p1*rho))/pt);
  }



  Analyze(TString fname, string treeName, bool isMCTree, ConfigPars * pars) : BaseTreeAnalyzer(fname,treeName, isMCTree,pars), plotter (new PlotterD(3)), eventPlots(plotter)
  {
  };

  void loadVariables() override{
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
    load(PICKYJETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
    load(PUPPIJETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
//    load(&trimmedJetReader,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC ,"trimmed");

    load(GENPARTICLES);
  }


  void testRes(const vector<RecoJetF>& jets,TString prefix, vector<float>* ptRaw = 0, vector<float>* jetA = 0){
    int nJC = 0;
    int nJ = 0;
    double ht = 0;
    vector<RankedIndex> rankedJets; rankedJets.reserve(jets.size());
    for(unsigned int iJ = 0; iJ < jets.size(); ++iJ){
      const auto& j = jets[iJ];
      if(j.genJet()==0) continue;
      if(TMath::Abs(j.genJet()->eta()) > 2.4) continue;
      if(j.genJet()->pt()  < 20) continue;
      rankedJets.emplace_back(j.genJet()->pt()  ,iJ);
    }
    sort(rankedJets.begin(), rankedJets.end(), PhysicsUtilities::greaterFirst<double,int>());

    for(unsigned int iJ =0; iJ < rankedJets.size(); ++iJ){
      const auto& j = jets[rankedJets[iJ].second];
      TString name = TString::Format("j%u",iJ+1);
      if(iJ >= 6) name = "jgeq6";

      double standardPT = j.pt();
      double eta = j.eta();
      double genPT = rankedJets[iJ].first;

      double corX = getCorrectedPT(standardPT,eta,rho,jetA->at(j.index()),true);

      double corPT = getCorrectedPT(standardPT,eta,rho,jetA->at(j.index()),false);

      double alpha = (standardPT - genPT)/corX;

      eventPlots.fill(corPT / genPT ,1, name +TString("_ptRes")           , TString(";p_{T} Res ") + name                    , 100,  0, 2, prefix);
      eventPlots.fill(alpha ,1, name +TString("_alpha")           , TString(";alpha ") + name                    , 1000,  0, 1, prefix);


//      eventPlots.fill(rankedJets[iJ].first  , 1, name +TString("_pt")           , TString(";p_{T} ") + name                    , 100,  0, 500, prefix);
//      eventPlots.fill(j.csv()               , 1, name +TString("_csv")         , TString(";csv ") + name                    , 100,  0, 1, prefix);
//      const auto* genJet = j.genJet();
//      if(genJet)eventPlots.fill(genJet->pt() ? rankedJets[iJ].first / genJet->pt() : 999  , 1, name +TString("_ptRes")           , TString(";p_{T} Res ") + name                    , 100,  0, 2, prefix);
    }


  }


  void makePlots2(const vector<RecoJetF>& jets,TString prefix, vector<float>* ptRaw = 0, vector<float>* jetA = 0){
    eventPlots.revert();

    int nJC = 0;
    int nJ = 0;
    double ht = 0;
    vector<RankedIndex> rankedJets; rankedJets.reserve(jets.size());
    for(unsigned int iJ = 0; iJ < jets.size(); ++iJ){
      const auto& j = jets[iJ];

      double pt = j.pt();
      if(ptRaw) pt = ptRaw->at(j.index());
      if(jetA) pt = getCorrectedPT(pt,j.eta(),rho,jetA->at(j.index()));

      if( pt < 20   ) continue;
      nJ++;

      if(TMath::Abs(j.eta())  > 2.4 ) continue;
      nJC++;
      ht+= pt;
      rankedJets.emplace_back(pt  ,iJ);
    }


    eventPlots.fill(nJC               , 1, "num_cen_j20"            , ";N. p_{T} #geq 20 jets (#eta < 2.4)"                    , 11,  1.5, 12.5, prefix);
    eventPlots.fill(nJ              , 1, "num_j20"            , ";N. p_{T} #geq 20 jets"                    , 11,  1.5, 12.5, prefix);



    eventPlots.fill(ht               , 1, "ht"            , ";H_{T}"                    , 100,  0, 1000, prefix);



    for(unsigned int iJ =0; iJ < rankedJets.size(); ++iJ){
      const auto& j = jets[rankedJets[iJ].second];
      TString name = TString::Format("j%u",iJ+1);
      if(iJ >= 6) name = "jgeq6";
      eventPlots.fill(rankedJets[iJ].first  , 1, name +TString("_pt")           , TString(";p_{T} ") + name                    , 100,  0, 500, prefix);
      eventPlots.fill(j.csv()               , 1, name +TString("_csv")         , TString(";csv ") + name                    , 100,  0, 1, prefix);
      const auto* genJet = j.genJet();
      eventPlots.fill(j.eta()  , 1, TString("eta")           , TString(";#eta ") , 104,  -5.2, 5.2, prefix);
      if(genJet)eventPlots.fill(genJet->pt() ? rankedJets[iJ].first / genJet->pt() : 999  , 1, name +TString("_ptRes")           , TString(";p_{T} Res ") + name                    , 100,  0, 2, prefix);
      if(genJet)eventPlots.fill(genJet->pt() ? rankedJets[iJ].first / genJet->pt() : 999  , 1, "ptRes"           , TString(";p_{T} Res ")                     , 80,  0, 2, prefix);
    }


    vector<RankedIndex> csvJets(rankedJets.size());
    for(unsigned int iJ =0; iJ < rankedJets.size(); ++iJ){
      csvJets[iJ].first = jets[rankedJets[iJ].second].csv();
      csvJets[iJ].second = rankedJets[iJ].second;
    }
    sort(csvJets.begin(), csvJets.end(), PhysicsUtilities::greaterFirst<double,int>());

    for(unsigned int iJ =0; iJ < csvJets.size(); ++iJ){
      const auto& j = jets[csvJets[iJ].second];
      TString name = TString::Format("j%u",iJ+1);
      if(iJ >= 4) name = "jgeq6";
      eventPlots.fill(double(ptRaw ? ptRaw->at(j.index()) : j.pt())             , 1, name +TString("_csvRanked_pt")           , TString(";p_{T} ") + name                    , 100,  0, 500, prefix);
      eventPlots.fill(j.csv()               , 1, name +TString("_csvRanked_csv")          ,TString(";csv ") + name                     , 100,  0, 1, prefix);
    }





    for(unsigned int iJ =0; iJ < rankedJets.size(); ++iJ){
      const auto& j = jets[rankedJets[iJ].second];

      eventPlots.revert();
      ++eventPlots;
      eventPlots("pt_eq20to30__", j.pt() < 30);
      eventPlots("pt_eq30to50__", j.pt() >= 30 && j.pt() < 50);
      eventPlots("pt_eq50to100__", j.pt() >= 50 && j.pt() < 100);
      eventPlots("pt_geq100__", j.pt() >= 100);


      const auto* genJet = j.genJet();

      enum JetFlavor      { unmatched_jet, g_jet, uds_jet, c_jet, b_jet, numJetFlavors };
      ++eventPlots;
      eventPlots("incl__", true);
      eventPlots("b__", genJet && genJet->flavor() == JetFlavorInfo::b_jet);
      eventPlots("c__", genJet && genJet->flavor() == JetFlavorInfo::c_jet);
      eventPlots("q__", genJet && genJet->flavor() == JetFlavorInfo::uds_jet);
      eventPlots("g__", genJet && genJet->flavor() == JetFlavorInfo::g_jet);
      eventPlots("u__", (genJet && genJet->flavor() == JetFlavorInfo::unmatched_jet) || !genJet);


      eventPlots.fill(double(ptRaw ? ptRaw->at(j.index()) : j.pt())              , 1, "pt"           , ";p_{T}"                    , 100,  0, 500, prefix);
      eventPlots.fill(j.csv()               , 1, "csv"           , ";csv"                    , 100,  0, 1, prefix);
      if(genJet)eventPlots.fill(genJet->pt() ? rankedJets[iJ].first / genJet->pt() : 999  , 1, "ptRes"           , ";p_{T} Res" , 100,  0, 2, prefix);

    }


  }

  template<typename Jet>
  void makePlots(const vector<Jet>& jets,TString prefix, vector<float>* ptRaw = 0, vector<float>* jetA = 0){
    eventPlots.revert();

    int nJC = 0;
    int nJ = 0;
    double ht = 0;
    vector<RankedIndex> rankedJets; rankedJets.reserve(jets.size());
    for(unsigned int iJ = 0; iJ < jets.size(); ++iJ){
      const auto& j = jets[iJ];

      double pt = j.pt();
      if(ptRaw) pt = ptRaw->at(j.index());
      if(jetA) pt = getCorrectedPT(pt,j.eta(),rho,jetA->at(j.index()));

      if( pt < 20   ) continue;
      nJ++;

      if(TMath::Abs(j.eta())  > 2.4) continue;
      nJC++;
      ht+= pt;
      rankedJets.emplace_back(pt  ,iJ);
    }

    eventPlots.fill(nJC               , 1, "num_cen_j20"            , ";N. p_{T} #geq 20 jets (#eta < 2.4)"                    , 11,  1.5, 12.5, prefix);
    eventPlots.fill(nJ              , 1, "num_j20"            , ";N. p_{T} #geq 20 jets"                    , 11,  1.5, 12.5, prefix);



    eventPlots.fill(ht               , 1, "ht"            , ";H_{T}"                    , 100,  0, 1000, prefix);



    for(unsigned int iJ =0; iJ < rankedJets.size(); ++iJ){
//      const auto& j = jets[rankedJets[iJ].second];
      TString name = TString::Format("j%u",iJ+1);
      if(iJ >= 6) name = "jgeq6";
      eventPlots.fill(rankedJets[iJ].first  , 1, name +TString("_pt")           , TString(";p_{T} ") + name                    , 100,  0, 500, prefix);
//      eventPlots.fill(j.csv()               , 1, name +TString("_csv")         , TString(";csv ") + name                    , 100,  0, 1, prefix);
    }


//    vector<RankedIndex> csvJets(rankedJets.size());
//    for(unsigned int iJ =0; iJ < rankedJets.size(); ++iJ){
//      csvJets[iJ].first = jets[rankedJets[iJ].second].csv();
//      csvJets[iJ].second = rankedJets[iJ].second;
//    }
//    sort(csvJets.begin(), csvJets.end(), PhysicsUtilities::greaterFirst<double,int>());
//
//    for(unsigned int iJ =0; iJ < csvJets.size(); ++iJ){
//      const auto& j = jets[csvJets[iJ].second];
//      TString name = TString::Format("j%u",iJ+1);
//      if(iJ >= 4) name = "jgeq6";
//      eventPlots.fill(double(ptRaw ? ptRaw->at(j.index()) : j.pt())             , 1, name +TString("_csvRanked_pt")           , TString(";p_{T} ") + name                    , 100,  0, 500, prefix);
//      eventPlots.fill(j.csv()               , 1, name +TString("_csvRanked_csv")          ,TString(";csv ") + name                     , 100,  0, 1, prefix);
//    }





//    for(unsigned int iJ =0; iJ < rankedJets.size(); ++iJ){
//      const auto& j = jets[rankedJets[iJ].second];
//
//      eventPlots.revert();
//      ++eventPlots;
//      eventPlots("pt_eq20to30__", j.pt() < 30);
//      eventPlots("pt_eq30to50__", j.pt() >= 30 && j.pt() < 50);
//      eventPlots("pt_eq50to100__", j.pt() >= 50 && j.pt() < 100);
//      eventPlots("pt_geq100__", j.pt() >= 100);
//
//
//      const auto* genJet = j.genJet();
//
//      enum JetFlavor      { unmatched_jet, g_jet, uds_jet, c_jet, b_jet, numJetFlavors };
//      ++eventPlots;
//      eventPlots("incl__", true);
//      eventPlots("b__", genJet && genJet->flavor() == JetFlavorInfo::b_jet);
//      eventPlots("c__", genJet && genJet->flavor() == JetFlavorInfo::c_jet);
//      eventPlots("q__", genJet && genJet->flavor() == JetFlavorInfo::uds_jet);
//      eventPlots("g__", genJet && genJet->flavor() == JetFlavorInfo::g_jet);
//      eventPlots("u__", (genJet && genJet->flavor() == JetFlavorInfo::unmatched_jet) || !genJet);
//
//
//      eventPlots.fill(double(ptRaw ? ptRaw->at(j.index()) : j.pt())              , 1, "pt"           , ";p_{T}"                    , 100,  0, 500, prefix);
//      eventPlots.fill(j.csv()               , 1, "csv"           , ";csv"                    , 100,  0, 1, prefix);
//    }


  }


  template<typename Jet>
    void inclusiveEffects(const vector<Jet>& jets,TString prefix, vector<float>* ptRaw = 0, vector<float>* jetA = 0){
      eventPlots.revert();

      int nJC = 0;
      int nJ = 0;
      double ht = 0;
      vector<RankedIndex> rankedJets; rankedJets.reserve(jets.size());
      for(unsigned int iJ = 0; iJ < jets.size(); ++iJ){
        const auto& j = jets[iJ];

        double pt = j.pt();
        if(ptRaw) pt = ptRaw->at(j.index());
        if(jetA) pt = getCorrectedPT(pt,j.eta(),rho,jetA->at(j.index()));

        if( pt < 20   ) continue;
        nJ++;

        if(TMath::Abs(j.eta())  > 2.4) continue;
        nJC++;
        ht+= pt;
        rankedJets.emplace_back(pt  ,iJ);
      }

      eventPlots.fill(nJC               , 1, "num_cen_j20"            , ";N. p_{T} #geq 20 jets (#eta < 2.4)"                    , 11,  1.5, 12.5, prefix);
      eventPlots.fill(nJ                , 1, "num_j20"            , ";N. p_{T} #geq 20 jets"                    , 11,  1.5, 12.5, prefix);
      eventPlots.fill(ht               , 1, "ht"            , ";H_{T}"                    , 100,  0, 1000, prefix);



      for(unsigned int iJ =0; iJ < rankedJets.size(); ++iJ){
        TString name = TString::Format("j%u",iJ+1);
        if(iJ >= 3) name = "jgeq3";
        eventPlots.fill(rankedJets[iJ].first  , 1, name +TString("_pt")           , TString(";p_{T} ") + name                    , 100,  0, 500, prefix);
        eventPlots.fill(rankedJets[iJ].first / ht  , 1, name +TString("_ptoht")           , TString(";p_{T} ") + name +TString(" / H_{T} "), 100,  0, 1, prefix);
        if(iJ != 0) eventPlots.fill(rankedJets[iJ].first / rankedJets[0].first  , 1, name +TString("_ptopt1")           , TString(";p_{T} ") + name +TString(" / p_{T} j1"), 100,  0, 1, prefix);

      }

    }


  double corrPT(double pt,  double eta, double area, double rho, double correction){
    if(correction == 0) return pt;

    if(pt > 30)
      return max(0.,1.08*(pt - rho*area*correction) );

    if(TMath::Abs(eta) < 1.5)
    return max(0.,1.05*(pt - rho*area*correction*1.43) );


    return max(0.,1.10*(pt - rho*area*correction) );


  }

  void getJetCorrections(JetReader& reader, const double correction, bool fillGen = false){
    TString prefix = TString::Format("corr_%.2f__",correction);
    prefix.ReplaceAll(".","p");

    eventPlots.revert();
    ++eventPlots;
    eventPlots("incl__",true);
    eventPlots("nV_lt15__",nPV < 15);
    eventPlots("nV_eq15to22__",nPV >= 15 && nPV < 22);
    eventPlots("nV_geq22__",nPV >= 22);

    eventPlots.checkPoint(1);

    int nGJ20  = 0;
    int nGJ30  = 0;
    int nGJ50  = 0;
    int nGJ100 = 0;

    for(const auto& j : reader.genJets){
      if(TMath::Abs(j.eta()) > 2.4 ) continue;
      if(j.pt() < 20 ) continue;
      nGJ20++;
      if(j.pt() < 30 ) continue;
      nGJ30++;
      if(j.pt() < 50 ) continue;
      nGJ50++;
      if(j.pt() < 100 ) continue;
      nGJ100++;
    }

    int nRJ20 = 0;
    int nRJ30 = 0;
    int nRJ50 = 0;
    int nRJ100 = 0;

    for(const auto& j : reader.recoJets){
      if(TMath::Abs(j.eta()) > 2.4 ) continue;

      double pt = corrPT(j.pt(),j.eta(), reader.jetarea_->at(j.index()),rho,correction);

      if(pt < 20 ) continue;
      nRJ20++;
      if(pt < 30 ) continue;
      nRJ30++;
      if(pt < 50 ) continue;
      nRJ50++;
      if(pt < 100 ) continue;
      nRJ100++;
    }

    if(fillGen){
      eventPlots.fill(nGJ20 , 1, "gen_num_20"            , ";N. p_{T} #geq 20 jets"                    , 11,  1.5, 12.5, prefix);
      eventPlots.fill(nGJ30 , 1, "gen_num_30"            , ";N. p_{T} #geq 30 jets"                    , 11,  1.5, 12.5, prefix);
      eventPlots.fill(nGJ50 , 1, "gen_num_50"            , ";N. p_{T} #geq 50 jets"                    , 11,  1.5, 12.5, prefix);
      eventPlots.fill(nGJ100, 1, "gen_num_100"           , ";N. p_{T} #geq 100 jets"                    , 11,  1.5, 12.5, prefix);

      for(const auto& j : reader.genJets){
        double pt = j.pt();
        if(TMath::Abs(j.eta()) > 2.4 ) continue;
        eventPlots.revert(1);
        eventPlots.fill(pt  , 1, TString("gen_pt")           , TString(";p_{T} ")                     , 80,  0, 800, prefix);


        ++eventPlots;
        eventPlots("pt_eq20to50__",pt >= 20 && pt < 50);
        eventPlots("pt_eq50to100__",pt >= 50 && pt < 100);
        eventPlots("pt_eq100to300__",pt >= 100 && pt < 300);
        eventPlots("pt_geq300__",pt >= 300 && pt < 300);

        eventPlots.fill(j.eta()  , 1, TString("gen_eta")           , TString(";#eta")                     , 48, -2.4, 2.4, prefix);

      }
    }

    eventPlots.revert(1);
    eventPlots.fill(nRJ20 , 1, "reco_num_20"            , ";N. p_{T} #geq 20 jets"                    , 11,  1.5, 12.5, prefix);
    eventPlots.fill(nRJ30 , 1, "reco_num_30"            , ";N. p_{T} #geq 30 jets"                    , 11,  1.5, 12.5, prefix);
    eventPlots.fill(nRJ50 , 1, "reco_num_50"            , ";N. p_{T} #geq 50 jets"                    , 11,  1.5, 12.5, prefix);
    eventPlots.fill(nRJ100, 1, "reco_num_100"           , ";N. p_{T} #geq 100 jets"                    , 11,  1.5, 12.5, prefix);

    for(const auto& j : reader.recoJets){
      double pt = corrPT(j.pt(),j.eta(), reader.jetarea_->at(j.index()),rho,correction);
      if(TMath::Abs(j.eta()) > 2.4 ) continue;
      eventPlots.revert(1);

      eventPlots.fill(pt  , 1, TString("reco_pt")           , TString(";p_{T} ")                     , 80,  0, 800, prefix);


      ++eventPlots;
      eventPlots("pt_eq20to50__",pt >= 20 && pt < 50);
      eventPlots("pt_eq50to100__",pt >= 50 && pt < 100);
      eventPlots("pt_eq100to300__",pt >= 100 && pt < 300);
      eventPlots("pt_geq300__",pt >= 300);


      eventPlots.fill(j.eta()  , 1, TString("reco_eta")           , TString(";#eta")                     , 48, -2.4, 2.4, prefix);
      const auto* genJet = j.genJet();
      if(genJet == 0 || genJet->pt() == 0) continue;
      eventPlots.revert(1);
      ++eventPlots;
      eventPlots("pt_eq20to50__",genJet->pt() >= 20 && genJet->pt() < 50);
      eventPlots("pt_eq50to100__",genJet->pt() >= 50 && genJet->pt() < 100);
      eventPlots("pt_eq100to300__",genJet->pt() >= 100 && genJet->pt() < 300);
      eventPlots("pt_geq300__",genJet->pt() >= 300);



      eventPlots.fill(pt/ genJet->pt(), 1, "ptRes"           , ";p_{T} Res" , 100,  0, 2, prefix);
    }





  }

  void runEvent(){
    TString prefix ="";
//    bool leptonic = false;
//    float minTopPT = 9999;
//    int nTops = 0;
//    for(const auto * p : genParts){
//      if(ParticleInfo::isLeptonOrNeutrino(p->pdgId())) leptonic = true;
//      if(TMath::Abs(p->pdgId()) == ParticleInfo::p_t){
//        nTops++;
//        minTopPT = std::min(minTopPT,p->pt());
//      }
//    }
//
//    if(leptonic) return;
//    if(nTops != 2) return;





    eventPlots.rewind();
    eventPlots("incl__",true);
//    eventPlots("minTopPT_lt100__",minTopPT < 100);
//    eventPlots("minTopPT_eq100to200__",minTopPT >= 100 && minTopPT < 200);
//    eventPlots("minTopPT_geq200__",minTopPT >= 200);

    eventPlots.checkPoint();
//    makePlots2(ak4Reader.recoJets,"ak4_");
////    makePlots(ak4Reader.recoJets,"ak4Raw_",ak4Reader.jetptraw_);
////    makePlots(ak4Reader.recoJets,"ak4L1_",ak4Reader.jetptraw_,ak4Reader.jetarea_);
//    makePlots2(pickyJetReader.recoJets,"ak4Raw_");
//    makePlots2(puppiJetsReader.recoJets,"puppi_");
//    makePlots2(pickyJetReader.recoJets,"picky_");
//    makePlots(pickyJetReader.genJets,"pickyGen_");
//    makePlots(ak4Reader.recoJets,"ak4_");
//    makePlots(ak4Reader.genJets,"ak4Gen_");
//    makePlots(pickyJetReader.recoJets,"ak4Raw_");
//    makePlots(puppiJetsReader.recoJets,"puppi_");
//    makePlots(puppiJetsReader.genJets,"puppiGen_");


//    inclusiveEffects(ak4Reader.genJets,"ak4_");
//    inclusiveEffects(ak4Reader.genJets,"ak4_");
//    inclusiveEffects(pickyJetReader.genJets,"picky_");
//    inclusiveEffects(trimmedJetReader.genJets,"trimmed_");


//    testRes(pickyJetReader.recoJets,"picky_",0,pickyJetReader.jetarea_);

    getJetCorrections(pickyJetReader, 0, true);
//    getJetCorrections(pickyJetReader, .10);
//    getJetCorrections(pickyJetReader, .15);
//    getJetCorrections(pickyJetReader, .20);
//    getJetCorrections(pickyJetReader, .25);
//    getJetCorrections(pickyJetReader, .3);
    getJetCorrections(pickyJetReader, .35);
//    getJetCorrections(pickyJetReader, .40);

  }


  void out(TString outputPath){
    plotter->write(outputPath);
  }
};




#endif



void pickyJetValidationPlots(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", string outName = "", bool isMCTree = true) {


  Analyze a(fname, treeName, isMCTree, 0);

  a.analyze(100000,500000);
  a.out(outName);
}
