#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisMethods/PlotUtils/interface/SFGetter.hh"
#include "AnalysisTools/QuickRefold/interface/TObjectContainer.h"

#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include<iostream>


using namespace std;


TTree * getTree(TString filename){
  TFile * sf = new TFile(filename,"read");
  TTree * st =0;
  sf->GetObject("Events",st);
  return st;
}


const TString stdWeight = "1.0";
const TString stdMCWeight = "weight*truePUWeight*btagWeight*2.262";

enum TreeType {Data,QCD, NoSmearQCD, TTbarW, Znuu, ttZ};
const TString TreeNames[] = {"Data", "QCD","NoSmearQCD","t#bar{t}W","Z#rightarrow#nu#nu","t#bar{t}Z"};
const StyleTools::Colors TreeColors[] = {color_data, color_qcd,color_qcd,color_ttbar,color_znunu,color_ttZ};
const TString weights[] = {stdWeight,stdMCWeight,stdMCWeight,stdMCWeight,stdMCWeight,stdMCWeight};

void getSTDTreesAndNames(vector<pair<TTree*,TreeType> >& trees){
  trees.emplace_back(getTree("htmht_tree_skimmed.root"),Data);
  trees.emplace_back(getTree("ttbarplusw_tree_skimmed.root"),TTbarW);
  trees.emplace_back(getTree("znunu_tree_skimmed.root"),Znuu);
  trees.emplace_back(getTree("ttZ_tree_skimmed.root"),ttZ);
  trees.emplace_back(getTree("qcd_smearTree_skimmed.root")     ,QCD);
//  trees.emplace_back(getTree("qcd_origTree.root") ,NoSmearQCD);

}
const TString METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && met >= 250 && j2pt >= 75";
const TString ResTailExtraCuts = "(dphij12met < .1 || dphij3met < .1) && nvetolep == 0 && pseudoRespPassFilter == 1";
const TString BaselineExtraCuts = "njets >= 5 && nlbjets>= 2 && nbjets>=1";

void getRegion(){
//  TString presel = TString::Format("%s && nvetolep == 0", METPresel.Data());
  TString presel = TString::Format("%s && (dphij12met < .1 || dphij3met < .1) && nvetolep == 0", METPresel.Data());
//  TString sels[] = {"met >= 200","met >= 250","nbjets >= 1","nbjets >= 1 && nlbjets >= 2","njets >=5 && nbjets >= 1 && nlbjets >= 2",""};
//  TString sels[] = {"met >= 200","njets >=5","nbjets >= 1","njets >=5 && nbjets >= 1",""};
//  TString sels[] = {"dphij12met < .15","dphij12met < .15 && pseudoRespNoFilterRank >= 0 && pseudoRespNoFilterRank < 2","dphij12met < .15 && pseudoRespNoFilterRank >= 0 && pseudoRespNoFilterRank < 2 && pseudoRespNoFilterNextPTRatio < .5",""};
//  TString sels[] = {"njets >=5 && nbjets >= 1 && nlbjets >= 2",""};
//    TString sels[] = {"pseudoResp < .5 && pseudoRespNoFilterRank >= 0","pseudoResp < .3 && pseudoRespNoFilterRank >= 0","pseudoResp < .15 && pseudoRespNoFilterRank >= 0",""};
//    TString sels[] = {"met >= 200","met >= 250",""};
    TString sels[] = {"pseudoRespCSV >= 0.890","pseudoRespCSV >= 0.970","pseudoRespCSV < 0.605",""};


  HistogramGetter histG("pseudoResp","pseudoResp","#it{p}_{T,reco jet} / #it{p}_{T,reco jet} + #slash{#it{E}}_{T}", 40,0,2);
//  HistogramGetter histG("pseudoRespNoFilterNextPTRatio","pseudoRespNoFilterNextPTRatio","pseudoRespNoFilterNextPTRatio", 20,0,1);
//  HistogramGetter histG("met","met","#it{p}_{T,reco jet} / #it{p}_{T,reco jet} + #slash{#it{E}}_{T}", 20,0,1000);
//      HistogramGetter histG("dphij12met","dphij12met","dphij12met", 12,0,.6);
//      HistogramGetter histG("dphij3met","dphij3met","dphij3met", 12,0,.6);


  vector<pair<TTree*,TreeType> > trees;
  getSTDTreesAndNames(trees);

  for(unsigned int iP = 0; sels[iP][0]; ++iP){
    TString title = TString::Format("%s",sels[iP].Data());
    TString name = TString::Format("getRegion_%u",iP);
    Plot * plot = new Plot(name,title,histG.plotInfo->xTitle,"Events");
    TString sel = TString::Format("%s && %s",presel.Data(),sels[iP].Data());
    for(unsigned int iT = 0; iT < trees.size(); ++iT){

      TH1F * hSN = histG.getHistogram(trees[iT].first,sel,weights[trees[iT].second],TString::Format("tree_%u",iT));

//      if(trees[iT].second == Data)
//        plot->addHist(hSN,TreeNames[trees[iT].second],histG.plotInfo->xTitle,colorGetter(iT));
//      else
//        plot->addToStack(hSN,TreeNames[trees[iT].second],colorGetter(iT));

      if(trees[iT].second == Data)
        plot->addHist(hSN,TreeNames[trees[iT].second],"",TreeColors[trees[iT].second]);
      else
        plot->addToStack(hSN,TreeNames[trees[iT].second],TreeColors[trees[iT].second]);

    }
    TCanvas * c = new TCanvas;
    plot->drawRatioStack(c,false,"png");
//    plot->drawRatios(c,1,true,"png");
    cout << title << endl;
  }
}

void checkContamination(){
//  TString presel = TString::Format("%s && nvetolep == 0", METPresel.Data());
  TString presel = TString::Format("%s && (dphij12met < .1 || dphij3met < .1) && nvetolep == 0 && pseudoRespNoFilterRank >= 0", METPresel.Data());

    TString sels[] = {"nlbjets == 0","pseudoRespCSV < 0.605","pseudoRespCSV >= 0.890","pseudoRespCSV >= 0.970",""};

  vector<HistogramGetter*> histGs;
  double bins[] = {0,.33,.5,.66,1};
  histGs.push_back(new HistogramGetter("pseudoResp","pseudoResp","#it{p}_{T,reco jet} / #it{p}_{T,reco jet} + #slash{#it{E}}_{T}", 4,bins));

  vector<TString> names;
  vector<TTree*> trees;
  trees.emplace_back(getTree("htmht_tree_skimmed.root")); names.push_back("Data");
  trees.emplace_back(getTree("nonQCD_tree_skimmed.root")); names.push_back("BKG");
  trees.emplace_back(getTree("qcd_smearTree_skimmed_split_light_gtp66.root"));  names.push_back("l, >.66");
  trees.emplace_back(getTree("qcd_smearTree_skimmed_split_light_p33top66.root"));  names.push_back("l, .33-.66");
  trees.emplace_back(getTree("qcd_smearTree_skimmed_split_light_ltp33.root"));  names.push_back("l, <.33");
  trees.emplace_back(getTree("qcd_smearTree_skimmed_split_b_gtp50.root"));      names.push_back("b, >.5");
  trees.emplace_back(getTree("qcd_smearTree_skimmed_split_b_ltp50.root"));names.push_back("b, < .5");


  for(unsigned int iH = 0; iH < histGs.size(); ++iH)
  for(unsigned int iP = 0; sels[iP][0]; ++iP){
    TString title = TString::Format("%s",sels[iP].Data());
    TString name = TString::Format("getRegion_%u",iP);
    Plot * plot = new Plot(name,title,histGs[iH]->plotInfo->xTitle,"Events");
    TString sel = TString::Format("%s && %s",presel.Data(),sels[iP].Data());
    for(unsigned int iT = 0; iT < trees.size(); ++iT){

      TH1F * hSN = histGs[iH]->getHistogram(trees[iT],sel,stdMCWeight,TString::Format("tree_%u",iT));
        plot->addHist(hSN,names[iT],"",colorGetter(iT),0,colorGetter(iT));
    }
    TCanvas * c = new TCanvas;
    plot->draw(c,false,"png");
    cout << title << endl;
  }
}

void getSFs(){
  TString presel = TString::Format("%s && (dphij12met < .1 || dphij3met < .1) && nvetolep == 0 && pseudoRespNoFilterRank >= 0", METPresel.Data());
//  TString recoSels[] = {"pseudoResp < . 33","pseudoResp >= .33 && pseudoResp < . 66","pseudoResp >= .66",""};
//  TString genSels[] = {"trueResp < . 33","trueResp >= .33 && trueResp < . 66","trueResp >= .66",""};

  TString recoSels[] = {"pseudoResp <  .33 && pseudoRespCSV < 0.605","pseudoResp >= .33 && pseudoResp < .66 && pseudoRespCSV < 0.605","pseudoResp >= .66 && pseudoRespCSV < 0.605",
      "pseudoResp < . 5 && pseudoRespCSV >= 0.890","pseudoResp >= .5 && pseudoRespCSV >= 0.890"
      ,""};
//  TString recoSels[] = {"pseudoResp <  .33 && nlbjets ==0","pseudoResp >= .33 && pseudoResp < .66 && nlbjets ==0","pseudoResp >= .66 && nlbjets ==0",
//      "pseudoResp < . 5 && pseudoRespCSV >= 0.970","pseudoResp >= .5 && pseudoRespCSV >= 0.970"
//      ,""};
  TString genSels[] = {"trueResp < .33 && trueRespFlv !=4","trueResp >= .33 && trueResp < .66 && trueRespFlv !=4","trueResp >= .66 && trueRespFlv !=4",
                       "trueResp < .5 && trueRespFlv ==4","trueResp >= .5 && trueRespFlv ==4",""};

//  TString recoSels[] = {"pseudoResp <  .33 && pseudoRespCSV < 0.605","pseudoResp >= .33 && pseudoResp < .66 && pseudoRespCSV < 0.605","pseudoResp >= .66 && pseudoRespCSV < 0.605",
//      "pseudoResp < . 33 && pseudoRespCSV >= 0.970","pseudoResp >= .33 && pseudoResp < .66 && pseudoRespCSV >= 0.970","pseudoResp >= .66 && pseudoRespCSV >= 0.970"
//      ,""};
//  TString genSels[] = {"trueResp < .33 && trueRespFlv !=4","trueResp >= .33 && trueResp < .66 && trueRespFlv !=4","trueResp >= .66 && trueRespFlv !=4",
//                       "trueResp < .33 && trueRespFlv ==4","trueResp >= .33 && trueResp < .66  && trueRespFlv ==4","trueResp >= .66 && trueRespFlv ==4",""};

cout << presel <<endl;
vector<double> bkgSFs;
bkgSFs.push_back(.8);
bkgSFs.push_back(.8);
bkgSFs.push_back(.8);
bkgSFs.push_back(.8);
bkgSFs.push_back(.8);
//bkgSFs.push_back(.8);

vector<double> bkgUNCs;
bkgUNCs.push_back(.2);
bkgUNCs.push_back(.2);
bkgUNCs.push_back(.2);
bkgUNCs.push_back(.2);
bkgUNCs.push_back(.2);
//bkgUNCs.push_back(.2);

  vector<TString> recoSelStrings;
  vector<TString> genSelStrings;
  for(unsigned int iD = 0; recoSels[iD][0]; ++iD){
    recoSelStrings.push_back(recoSels[iD]);
    genSelStrings.push_back(genSels[iD]);
  }
  TRandom3 * rand = new TRandom3(1234);
  TTree * dataTree = getTree("htmht_tree_skimmed.root");
  TTree * bkgTree = getTree("nonQCD_tree_skimmed.root");
  TTree * qcdTree = getTree("qcd_smearTree_skimmed_dphipresel.root");

  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVector(dataTree,bkgTree,&bkgSFs,&bkgUNCs,presel,"1.0",stdMCWeight,recoSelStrings,rand);

  cout <<"Loading MC!" <<endl;
  SFGetter::PseudoMatrixBootstrapGenerator mcMatrix(qcdTree,presel,stdMCWeight,recoSelStrings,genSelStrings,50,"bootstrapWeight");
//  TTree * qcdTree = getTree("qcd_origTree.root");
//  cout <<"Loading MC!" <<endl;
//  SFGetter::PseudoMatrixGausGenerator mcMatrix(qcdTree,presel,stdMCWeight,recoSelStrings,genSelStrings,rand);

  SFGetter::MatrixSolver solver(&dataVector,&mcMatrix,5);
}



#endif


void GetTailSF()
{

  StyleTools::SetStyle();
//  doFullExtraction();
//  getRegion();
//  checkContamination();
//  getSFs();
//  test();
}
