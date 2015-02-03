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
    static double param0[] = {-0.853422,-0.687297,-0.701125,-0.408372,-1.04396,-0.964522,-0.895684,-0.756244,-0.6033,-0.559984,-1.01293,-2.17769,-2.91198,-3.33198,-1.76464,-0.201368,-0.783988,-0.566723,-0.698571,-0.543086,-0.474122,-0.63127,-0.87867,-0.826942,-1.04913,-1.22067,-1.23548,-1.45207,-1.08597,-1.52566,-1.37719,-0.969128,-0.893565,-0.916191,-2.12447,-0.963161,-0.891276,-0.785051,-0.946275,-1.66625,-1.03436,-0.0698692,-0.945691,-0.815896,-0.918572,-1.22118,-1.06354,-1.06376,-1.21984,-0.722053,-1.22737,-1.25384,-1.43892,-0.397265,-0.989054,-1.53084,-1.01117,-1.20399,-0.921474,-2.25301,-0.787646,-0.867744,-0.880757,-0.964293,-0.7825,-0.724667,-0.367746,-1.56312,-2.99562,-3.21509,-1.7861,-1.23991,-0.690696,-0.781798,-0.717775,-0.939972,-0.765734,-0.852608,-0.86479,-0.586767,-1.54182,-0.517457};
    static double param1[] = {0.0401,0.149583,0.35515,0.491751,0.606422,0.61164,0.592705,0.615502,0.523822,0.491835,0.452743,0.939132,1.17253,1.12364,0.433483,0.20868,0.237287,0.230766,0.252941,0.281996,0.307402,0.331719,0.40136,0.419098,0.421535,0.412193,0.362387,0.316997,0.401728,0.436882,0.466114,0.446151,0.440838,0.428792,0.502252,0.443348,0.406417,0.413393,0.414318,0.336966,0.422246,0.219009,0.430869,0.424816,0.416131,0.397592,0.463657,0.455693,0.471498,0.408965,0.465353,0.458744,0.451876,0.248335,0.347968,0.428049,0.41202,0.434203,0.414354,0.534201,0.362336,0.316257,0.265997,0.296419,0.275748,0.266418,0.248462,0.468117,1.0872,1.13568,0.899567,0.514995,0.492521,0.554725,0.576221,0.604535,0.606241,0.564705,0.496761,0.369842,0.505659,0.0728927};
    static double param2[] = {5,0.886925,0.164898,0.0782526,0.060632,0.0845584,0.101603,0.0765068,0.12731,0.124337,0.212253,0.0360037,-0.0428459,-0.0320324,0.251734,0.286691,0.160055,0.0909085,0.0520523,0.0109112,0.0155763,0.0542747,0.0438313,0.0550151,0.092354,0.12903,0.197546,0.299799,0.142443,0.12301,0.0819422,0.0667259,0.0671874,0.0741227,0.0869688,0.0640953,0.0909345,0.0790436,0.0938385,0.244788,0.0949267,0.339145,0.0832172,0.0770342,0.0890957,0.130984,0.0569305,0.0646587,0.0619914,0.0883909,0.0705234,0.0799038,0.107584,0.365514,0.211075,0.145157,0.118911,0.0904754,0.0684498,0.0294423,0.0325769,0.0368925,0.0611726,0.0180039,0.0445185,0.107464,0.205708,0.188082,-0.0362241,-0.0307959,0.034685,0.161636,0.128722,0.11583,0.103368,0.0970263,0.0710093,0.0791228,0.100488,0.140536,0.0555723,2.45399};

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
    return origPT * max(0.0001,1.0-area*(p0+(p1*rho)*(1+p2*log(pt)))/pt);

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
    load(ELECTRONS);
    load(MUONS);
//    load(PUPPIJETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
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

    return getCorrectedPT(pt,eta,rho,area,false);
//
//    if(pt > 30)
//      return max(0.,1.08*(pt - rho*area*correction) );
//
//    if(TMath::Abs(eta) < 1.5)
//    return max(0.,1.05*(pt - rho*area*correction*1.43) );
//
//
//    return max(0.,1.10*(pt - rho*area*correction) );


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


  void makeSRPlots(JetReader * reader, TString prefix){
    eventPlots.revert();

    ++eventPlots;

    bool singleLep = (leptons.size() == 1) && leptons[0]->pt() >= 30 && PhysicsUtilities::absDeltaPhi(*leptons[0], *met) > 1;

   eventPlots("1Lep_dPhi__",singleLep);
   eventPlots("0Lep__",leptons.size() == 0);
   eventPlots("inc__",true);

   ++eventPlots;
   eventPlots("nBJets_inc__",true);
   eventPlots("nBJets_geq1__",nBJets >= 1);

   ++eventPlots;
   eventPlots("met_inc__",true);
   eventPlots("met_geq100__",met->pt() >= 100);
   eventPlots("met_geq150__",met->pt() >= 150);
   eventPlots("met_geq200__",met->pt() >= 200);

   static Topness * tNess     = new Topness();
   double minTopness = tNess->findMinTopnessConfiguration(leptons,jets,met);
   eventPlots.fill(minTopness            , 1, "topness"                , ";Topness"                            , 40,  -20,  20, prefix);
   eventPlots.fill(nJets , 1, "num_20"            , ";N. p_{T} #geq 20 jets"                    , 11,  1.5, 12.5, prefix);
   eventPlots.fill(nBJets , 1, "num_bjets"            , ";N.b-Tags p_{T} #geq 20 jets"                    , 5,  -.5, 4.5, prefix);

   for(const auto* j : jets)eventPlots.fill(j->csv()               , 1, TString("csv")         , TString(";csv ")                 , 100,  0, 1, prefix);

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
    eventPlots("",true);
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

//    getJetCorrections(pickyJetReader, 0, true);
//    getJetCorrections(puppiJetsReader,0);
//    getJetCorrections(puppiJetsReader,0);
//    getJetCorrections(puppiJetsReader, , true);
//    getJetCorrections(pickyJetReader, .10);
//    getJetCorrections(pickyJetReader, .15);
//    getJetCorrections(pickyJetReader, .20);
//    getJetCorrections(pickyJetReader, .25);
//    getJetCorrections(pickyJetReader, .35);
//    getJetCorrections(pickyJetReader, 1);
//    getJetCorrections(pickyJetReader, .40);

//    makePlots(ak4Reader.genJets,"ak4Gen_");
//    makePlots(ak4Reader.recoJets,"ak4_");
//    makePlots(pickyJetReader.recoJets,"ak4Raw_");
//    makePlots(pickyJetReader.recoJets,"ak4L1_",0,pickyJetReader.jetarea_);
//    makePlots(puppiJetsReader.recoJets,"ak4Puppi_");

    makeSRPlots(defaultJets,"");


  }


  void out(TString outputPath){
    plotter->write(outputPath);
  }
};




#endif



void pickyJetValidationPlots(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", string outName = "", bool doPicky = false, bool isMCTree = true) {

  ConfigPars pars;
  pars.minJetPt = 20;
  pars.minBJetPt = 20;
  pars.cleanJetsvLeptons_ =true;
//  pars.correctPickyPT =false;
  Analyze a(fname, treeName, isMCTree, &pars);
  a.setDefaultJets( doPicky ? BaseTreeAnalyzer::PICKYJETS : BaseTreeAnalyzer::AK4JETS  );
  a.analyze(100000,500000);
  a.out(outName);
}
