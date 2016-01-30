#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisMethods/PlotUtils/interface/SFGetter.hh"
#include "AnalysisTools/QuickRefold/interface/TObjectContainer.h"

#include "TreeReadingHelper.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include<iostream>
#include<regex>


using namespace std;


TTree * getTree(TString filename){
  TFile * sf = new TFile(filename,"read");
  TTree * st =0;
  sf->GetObject("Events",st);
  return st;
}

void setTitleOffset(TCanvas *c, double xOff = .950, double yOff = 1.400){
  TList * list = c->GetListOfPrimitives();
  for(unsigned int iP = 0; iP < list->GetSize(); ++iP){
    TH1 * h = dynamic_cast<TH1*>(list->At(iP));
    if(h == 0) continue;
    h->GetXaxis()->SetTitleOffset(xOff);
    h->GetYaxis()->SetTitleOffset(yOff);
  }
}

const TString stdWeight = "1.0";
const TString stdMCWeight = "weight*truePUWeight*btagWeight*2.262";
const TString stdQCDWeight = "weight*truePUWeight*btagWeight*qcdRespTailWeight*2.262";

enum TreeType {Data,QCD, NoSmearQCD, TTbarW, Znuu, ttZ};
const TString TreeNames[] = {"Data", "QCD","NoSmearQCD","t#bar{t}W","Z#rightarrow#nu#nu","t#bar{t}Z"};
const StyleTools::Colors TreeColors[] = {color_data, color_qcd,color_qcd,color_ttbar,color_znunu,color_ttZ};
const TString weights[] = {stdWeight,stdMCWeight,stdMCWeight,stdMCWeight,stdMCWeight,stdMCWeight};

void getSTDTreesAndNames(vector<pair<TTree*,TreeType> >& trees){
  trees.emplace_back(getTree("pieces/htmht_tree_skimmed.root"),Data);
  trees.emplace_back(getTree("pieces/ttZ_tZq_skimmed.root"),ttZ);
  trees.emplace_back(getTree("pieces/znunu_tree_skimmed.root"),Znuu);
  trees.emplace_back(getTree("pieces/ttbarplusw_tree_skimmed.root"),TTbarW);
  trees.emplace_back(getTree("pieces/qcd_Tree_skimmed.root")     ,QCD);
//  trees.emplace_back(getTree("qcd_origTree.root") ,NoSmearQCD);
}
const TString METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && met >= 250 && j2pt >= 75";
const TString ResTailExtraCuts = "(dphij12met < .1 || dphij3met < .1) && nvetolep == 0 && pseudoRespPassFilter == 1";
const TString BaselineExtraCuts = "njets >= 5 && nlbjets>= 2 && nbjets>=1";
const TString header = "#sqrt{s} = 13 TeV, L = 2.3 fb^{-1}";

void getMotivationPlots() {
//  //Get dPhi plot
  TString preselection = TString::Format("%s && %s",METPresel.Data(),BaselineExtraCuts.Data());
  HistogramGetter dphiGetter("dphi","min(dphij12met,dphij3met)","#Delta#phi_{123}",22,0,1.1);
  vector<pair<TTree*,TreeType> > dPhiTrees;
  getSTDTreesAndNames(dPhiTrees);
    Plot * plot = new Plot("dPhi","",dphiGetter.plotInfo->xTitle,"Events");
    TString sel = preselection;
    for(unsigned int iT = 0; iT < dPhiTrees.size(); ++iT){
      if(dPhiTrees[iT].second == Data) continue;
      TH1F * hSN = dphiGetter.getHistogram(dPhiTrees[iT].first,sel,weights[dPhiTrees[iT].second],TString::Format("tree_%u",iT));
      plot->addToStack(hSN,TreeNames[dPhiTrees[iT].second],TreeColors[dPhiTrees[iT].second]);
    }
    TCanvas * c = new TCanvas;
//    c->SetLeftMargin  (0.18);
//    c->SetTopMargin   (0.10);
//    c->SetRightMargin (0.07);
//    c->SetBottomMargin(0.03);
    plot->setYRange(65,2000);
    plot->setLogy();
    plot->setHeader(header,"");
    plot->draw(c,true,"png");

    //Now do distribution plots
    TTree * qcdTree = getTree("1_17_version/qcd_resTest_smearWithExtraCut_skimmed.root");
    TString distPre = "met >= 250 && nJ >= 5 && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt";
    TString sels[] = {"met >= 300  && met < 400 && min(dPhi,dPhi3) < .1","met >= 500  && met < 600 && min(dPhi,dPhi3) < .1","met >= 300  && met < 400 && min(min(dPhi,dPhi3),dPhi4) >= .5","met >= 500  && met < 600 && min(min(dPhi,dPhi3),dPhi4) >= .5",""};
    TString selNames[] = {"#slash{#it{E}}_{T} 300-400 GeV, #Delta#phi_{123} < 0.1","#slash{#it{E}}_{T} 500-600 GeV, #Delta#phi_{123} < 0.1","#slash{#it{E}}_{T} 300-400 GeV, #Delta#phi_{1234} > 0.5","#slash{#it{E}}_{T} 500-600 GeV, #Delta#phi_{1234} > 0.5",""};

//    TString sels[] = {"met >= 400  && met < 500 && min(dPhi,dPhi3) < .1","met >= 400  && met < 500 && min(min(dPhi,dPhi3),dPhi4) >= .5",""};
//    TString selNames[] = {"#Delta#phi_{123} < 0.1","#Delta#phi_{1234} > 0.5",""};

    vector<HistogramGetter *> histGs;
    histGs.push_back(new HistogramGetter("leadLoss_recorank","leadLoss_recorank +1","Reconstructed rank",6,0.5,6.5));
    histGs.push_back(new HistogramGetter("leadLoss_rank","leadLoss_rank +1","Generated rank",6,0.5,6.5));
//    histGs.push_back(new HistogramGetter("leadLoss_pt","leadLoss_pt","#it{p}_{T,gen} [GeV]",50,200,2000));
    histGs.push_back(new HistogramGetter("leadLoss_res","leadLoss_res","#it{r}_{jet}",11,0,1.1));


    for(unsigned int iH = 0; iH < histGs.size(); ++iH){
      TString name = TString::Format("extrapolatedDists_%u",iH);
      Plot * plot = new Plot(name,"",histGs[iH]->plotInfo->xTitle,"Arbitrary units");
      for(unsigned int iP = 0; sels[iP][0]; ++iP){
        TString sel = TString::Format("%s && %s",distPre.Data(),sels[iP].Data());
        TH1F * hSN = histGs[iH]->getHistogram(qcdTree,sel,"weight","QCD");
        PlotTools::normalize(hSN);
        plot->addHist(hSN,selNames[iP],"hist",colorGetter(iP),0,colorGetter(iP));
      }


      TCanvas * c = new TCanvas;
      plot->setLegend(.45,.65,.92,.9);
      plot->setYRange(0.,1.0);
      plot->setHeader(header,"");
      plot->draw(c,true,"png");
      setTitleOffset(c);
      c->SaveAs(plot->getName() + TString(".png"));
    }



    //Now do b-integration
    TString numpre = TString::Format("%s && %s && (dphij12met > .5 && dphij3met > .5 && dphij4met > .5)", METPresel.Data(), BaselineExtraCuts.Data());
    TString denpre = TString::Format("%s && %s && (dphij12met < .1 || dphij3met < .1)", METPresel.Data(), BaselineExtraCuts.Data());
    TString selNum[] = {"nbjets == 1",
                        "nbjets >= 2",
                        ""};
    TString selDen[] = {"nbjets == 1",
        "nbjets >= 2",
                        ""};
    TString metSelNames[] = {"# of b-jets = 1","# of b-jets #geq 2",""};
    double bins[] = {250,300,400,500,600,700};
    HistogramGetter metHistG("met","met","#slash{#it{E}}_{T} [GeV]", 5,bins);
    TTree * qcdTree2 = getTree("pieces/qcd_tree_skimmed_baseline.root");

    Plot * metplot = new Plot("bIntegration","",metHistG.plotInfo->xTitle,"SR/CR");

    for(unsigned int iS1 = 0; selNum[iS1][0]; ++iS1){
      TString title = TString::Format("%s",selNum[iS1].Data());
      TString name = TString::Format("checkTF_%u",iS1);
      TString denSelelction = TString::Format("%s && %s",denpre.Data(),selDen[iS1].Data());
      TString numSelelection = TString::Format("%s && %s",numpre.Data(),selNum[iS1].Data());
      metHistG.nBootStraps = 50;
      TH1F *  hSN = metHistG.getTFAndCov(qcdTree2,numSelelection,stdMCWeight,denSelelction,stdMCWeight,TString::Format("ratio_%u",iS1));
      int color = colorGetter(iS1 >= 3 ? iS1 -3 : iS1 );
      metplot->addHist(hSN,metSelNames[iS1],"",color,0,color,iS1 >= 3 ? 3 : 1);
      }
    metplot->setYRange(0.,0.15);
      TCanvas * c2 = new TCanvas;
      metplot->setHeader(header,"");
      metplot->draw(c2,true,"png");
      setTitleOffset(c2);
      c2->SaveAs(metplot->getName() + TString(".png"));


}

void getSmearComparision() {
//  //Get dPhi plot
  TString preselection = TString::Format("%s && %s ",METPresel.Data(),BaselineExtraCuts.Data());
//  TString sels[] = {"nbjets == 1 && (dphij12met < .1 || dphij3met < .1)","nbjets >= 2 && (dphij12met < .1 || dphij3met < .1)",
//      "nbjets == 1  && (dphij12met > .5)","nbjets >= 2 && (dphij12met > .5)"
//
//
//      ,""};
//  TString selNames[] = {"#Delta#phi_{123} < 0.1, 1 b-tag","#Delta#phi_{123} < 0.1, #geq 2 b-tags","#Delta#phi_{12} > 0.5, 1 b-tag","#Delta#phi_{12} > 0.5,  #geq 2 b-tags",""};

  TString sels[] = {"nbjets == 1","nbjets >= 2",
      "ncttstd >=1","dphij12met > .5"


      ,""};
  TString selNames[] = {"1 b-tag","#geq 2 b-tags","#geq1 top tag","#Delta#phi_{12} > 0.5",""};


  double bins[] = {250,300,400,500,600,700};
  HistogramGetter metHistG("met","met","#slash{#it{E}}_{T} [GeV]", 5,bins);

  TTree * smearQCD = getTree("pieces/qcd_tree_skimmed_baseline.root");
  TTree * origQCD = getTree("pieces/qcd_origtree_skimmed_baseline.root");




      for(unsigned int iP = 0; sels[iP][0]; ++iP){
        TString name = TString::Format("smearComp_%u",iP);
        Plot * plot = new Plot(name,selNames[iP],metHistG.plotInfo->xTitle,"Events");
        TString sel = TString::Format("%s && %s",preselection.Data(),sels[iP].Data());
        metHistG.nBootStraps = 50;TH1F * hSS = metHistG.getHistogramManual(smearQCD,sel,"weight*2.26","QCD");
        metHistG.nBootStraps = 0; TH1F * hSO = metHistG.getHistogram(origQCD,sel,"weight*2.26","QCD2");
        plot->addHist(hSO,"Original QCD MC","",TreeColors[TTbarW],0,TreeColors[TTbarW]);
        plot->addHist(hSS,"Smeared QCD MC","",TreeColors[Znuu],0,TreeColors[Znuu]);


        TCanvas * c = new TCanvas;
        plot->setHeader(header,"");
        plot->setYRange(0.5,700);
        plot->setLogy();

        plot->drawRatios(c,1,true,"png");
        setTitleOffset(c,.850);
        c->SaveAs(plot->getName() + TString(".png"));
      }

}

void getTailSFRegion(){
  TString presel = TString::Format("%s && %s", METPresel.Data(),ResTailExtraCuts.Data());
  TString sels[] = {"pseudoRespCSV < 0.605","pseudoRespCSV >= 0.890",""};
  TString selNames[] = {"Jet is not light b-tagged","Jet is medium b-tagged",""};


  double bins[] = {0,.1,.33,.5,.66,.8,1};
  HistogramGetter histG("pseudoResp","pseudoResp","#it{r}_{pseudo,jet}", 6,bins);

  vector<TString> names;
  vector<TTree*> trees;
  vector<int> colors;
  trees.emplace_back(getTree("pieces/htmht_tree_skimmed.root")); names.push_back("Data"); colors.push_back(1);
  trees.emplace_back(getTree("pieces/nonQCD_tree_skimmed.root")); names.push_back("Non-QCD MC"); colors.push_back(2);
  trees.emplace_back(getTree("pieces/qcd_tree_skimmed_dphi_split_b_gtp50.root"));      names.push_back("b, r >0.5"); colors.push_back(5);
  trees.emplace_back(getTree("pieces/qcd_tree_skimmed_dphi_split_b_ltp50.root"));names.push_back("b, r < 0.5"); colors.push_back(3);
  trees.emplace_back(getTree("pieces/qcd_tree_skimmed_dphi_split_light_gtp66.root"));  names.push_back("l, r > 0.66"); colors.push_back(7);
  trees.emplace_back(getTree("pieces/qcd_tree_skimmed_dphi_split_light_p33top66.root"));  names.push_back("l, r 0.33-0.66"); colors.push_back(6);
  trees.emplace_back(getTree("pieces/qcd_tree_skimmed_dphi_split_light_ltp33.root"));  names.push_back("l, r < 0.33"); colors.push_back(4);


  for(unsigned int iP = 0; sels[iP][0]; ++iP){
    TString name = TString::Format("tailSFReg_%u",iP);
    Plot * plot = new Plot(name,selNames[iP],histG.plotInfo->xTitle,"Events");
    TString sel = TString::Format("%s && %s",presel.Data(),sels[iP].Data());
    for(unsigned int iT = 0; iT < trees.size(); ++iT){
      if(names[iT] == "Data"){
        TH1F * hSN = histG.getHistogram(trees[iT],sel,"1.0",TString::Format("tree_%u",iT));
        plot->addHist(hSN,names[iT],"",colors[iT],0,colors[iT]);
      } else {
        TH1F * hSN = histG.getHistogram(trees[iT],sel,TString::Format("%s",stdMCWeight.Data()),TString::Format("tree_%u",iT));
        if(names[iT] == "Non-QCD MC"){
          if(iP == 0) hSN->Scale(0.886424);
          if(iP == 1) hSN->Scale(0.77299);
        }
        plot->addToStack(hSN,names[iT],colors[iT]);
      }

    }
    TCanvas * c = new TCanvas;
    plot->setHeader(header,"");
//    plot->setLogy();
    if(iP == 1){
      plot->setYRange(0.,1200);
    }
    plot->getLegend()->SetNColumns(2);
    plot->drawRatioStack(c,true,"png");
    setTitleOffset(c,.850);
    c->SaveAs(plot->getName() + TString(".png"));

  }
}


void checkCRContamination(){
//  TString presel = TString::Format("%s && nvetolep == 0", METPresel.Data());
  TString presel = TString::Format("%s && nvetotau == 0 && nvetolep == 0", METPresel.Data());

//  TString sels[] = {"(dphij12met < .1)","(dphij12met < .15)","(dphij12met < .1 || dphij3met < .1)","(dphij12met < .15 || dphij3met < .15)",""};
  TString sels[] = {
      "(dphij12met < .15)","(dphij12met < .1 || dphij3met < .1)",""};
  TString sels2[] = {"mtcsv12met < 175 && (njets == 5 || njets == 6)","mtcsv12met < 175 && njets >= 7",
                     "mtcsv12met >= 175 && (njets == 5 || njets == 6) && ncttstd == 0", "mtcsv12met >= 175 && njets >= 7 && ncttstd == 0", "mtcsv12met >= 175 && ncttstd >= 1",""};


  vector<HistogramGetter*> histGs;
  double bins[] = {250,300,400,500,600,700};
  histGs.push_back(new HistogramGetter("met","met","#slash{#it{E}}_{T}", 5,bins));

  vector<TString> names;
  vector<TTree*> trees;
//  trees.emplace_back(getTree("pieces/htmht_tree_skimmed.root")); names.push_back("Data");
  trees.emplace_back(getTree("pieces/nonQCD_tree_skimmed.root")); names.push_back("Non-QCD MC");
  trees.emplace_back(getTree("pieces/qcd_tree_skimmed.root")); names.push_back("QCD MC");



  for(unsigned int iH = 0; iH < histGs.size(); ++iH)
    for(unsigned int iP = 0; sels[iP][0]; ++iP)
    for(unsigned int iP2 = 0; sels2[iP2][0]; ++iP2){
    TString title = TString::Format("%s, %s",sels[iP].Data(),sels2[iP2].Data());
    TString name = TString::Format("checkCRContamination_%u_%u",iP,iP2);
    Plot * plot = new Plot(name,title,histGs[iH]->plotInfo->xTitle,"Events");
    TString sel = TString::Format("%s && %s && %s",presel.Data(),sels[iP].Data(),sels2[iP2].Data());
    for(unsigned int iT = 0; iT < trees.size(); ++iT){
      TH1F * hSN = histGs[iH]->getHistogram(trees[iT],sel,stdMCWeight,TString::Format("tree_%u",iT));
      plot->addHist(hSN,names[iT],"",colorGetter(iT),0,colorGetter(iT));
    }
    TCanvas * c = new TCanvas;
    plot->drawRatios(c,1,true,"png");

    cout << title << endl;
  }
}
//MET_nJ_nB_nT_mTB!



struct CRegInfo {
  static const int nCR = 5;
  static const int nMETBins = 5;
  enum CRReg{ CR_5_1_0_0, CR_7_1_0_0,CR_5_1_0_175, CR_7_1_0_175, CR_5_1_1_175 };
  TString  crRegBinNames[nCR] = { "5_1_0_0", "7_1_0_0","5_1_0_175", "7_1_0_175", "5_1_1_175"};
  TString crSel[nCR] = {
      "mtcsv12met < 175 && (njets == 5 || njets == 6)",
      "mtcsv12met < 175 && njets >= 7",
      "mtcsv12met >= 175 && (njets == 5 || njets == 6) && ncttstd == 0",
      "mtcsv12met >= 175 && njets >= 7 && ncttstd == 0",
      "mtcsv12met >= 175 && ncttstd >= 1"
  };
    TString crSelNames[nCR] = {"#it{m}_{T,b} < 175, nJ 5-6","#it{m}_{T,b}< 175, nJ #geq7",
        "#it{m}_{T,b} #geq 175, nJ 5-6, nT 0","#it{m}_{T,b} #geq 175, nJ #geq7, nT 0",
        "#it{m}_{T,b} #geq 175, nT #geq1"
    };
  TString crPreDataOther =   TString::Format("%s && %s && nvetotau == 0 && nvetolep == 0 && (dphij12met < .1 || dphij3met < .1)", METPresel.Data(), BaselineExtraCuts.Data());
  TString crPreQCD =   TString::Format("%s && %s && (dphij12met < .1 || dphij3met < .1)", METPresel.Data(), BaselineExtraCuts.Data());


  double metBins[nMETBins+1] = {250,300,400,500,600,700};
  HistogramGetter * metGetter = new HistogramGetter("met","met","#slash{#it{E}}_{T} [GeV]", nMETBins,metBins);

  vector<TH1*> otherBKGSFs;
  TTree *otherBKGTree;
  TTree *dataTree;
  TTree *qcdTree;
  void fillOtherBkgSFs(TString dataTreeIn, TString otherBKGTreeIn, TString qcdTreeIn){
    fillOtherBkgSFs(getTree(dataTreeIn),getTree(otherBKGTreeIn),getTree(qcdTreeIn));
  }
  void fillOtherBkgSFs(TTree * dataTreeIn, TTree * otherBKGTreeIn, TTree * qcdTreeIn){
    otherBKGTree = otherBKGTreeIn;
    dataTree = dataTreeIn;
    qcdTree = qcdTreeIn;

    double bins2[] = {250,300,400,700};
    HistogramGetter SFHG("SFmet","met","#slash{#it{E}}_{T} [GeV]", 3,bins2);
    HistogramGetter SFIncHG("SFmet2","met","#slash{#it{E}}_{T} [GeV]", 1, 250, 700);
    TString normPre = TString::Format("%s && %s && nvetolep >= 1 && mtlepmet < 100", METPresel.Data(), BaselineExtraCuts.Data());

    for(unsigned int iS = 0; iS < nCR; ++iS){
      TString sel = TString::Format("%s && %s",normPre.Data(),crSel[iS].Data());
      TH1F * dataH  = (iS == 4?SFIncHG:SFHG) .getHistogram(dataTree,sel,"1.0","Data");
      TH1F * otherH = (iS == 4?SFIncHG:SFHG) .getHistogram(otherBKGTree,sel,stdMCWeight,"Other");
      TH1F * qcdH   = (iS == 4?SFIncHG:SFHG) .getHistogram(qcdTree,sel,stdQCDWeight,"MC");
      otherH->Add(qcdH);
      dataH->Divide(otherH);

      TH1 * h = new TH1F(TString::Format("ttbarwSF_%u",iS),";SF",5,metBins);
      for(unsigned int iB = 1; iB <= h->GetNbinsX(); ++iB){
        double metV = h->GetBinCenter(iB);
        double sf = dataH->GetBinContent( dataH->FindFixBin(metV) );
        h->SetBinContent(iB,sf);
      }

      cout <<"ttbar+W norm(" <<iS<<"): ";
      for(unsigned int iB = 1; iB <= h->GetNbinsX(); ++iB) cout << h->GetBinContent(iB)<<"\t";
      cout << endl;
      otherBKGSFs.push_back(h);
    }
  }
  vector<TH1*> otherBKGScaledYields;
  void fillCorrectedOtherBkgYields(){
    for(unsigned int iS = 0; iS < nCR; ++iS){
      TString sel = TString::Format("%s && %s",crPreDataOther.Data(),crSel[iS].Data());
      TH1 * h = metGetter->getHistogram(otherBKGTree,sel,stdMCWeight.Data(),TString::Format("tree_%u",0));
      for(unsigned int iB =1; iB <=h->GetNbinsX(); ++iB){
        h->SetBinContent(iB,h->GetBinContent(iB) *otherBKGSFs[iS]->GetBinContent(iB) );
        h->SetBinError(iB,h->GetBinError(iB) *otherBKGSFs[iS]->GetBinContent(iB) );
      }
      otherBKGScaledYields.push_back(h);
    }
  }

  vector<TH1*> dataYields;
  vector<TH1*> qcdYields;
  vector<TH1*> otherBKGCorr;
  vector<TH1*> otherBKGCorrUnc;
  void fillDataCorr(){
    for(unsigned int iS = 0; iS < nCR; ++iS){
      TString sel = TString::Format("%s && %s",crPreDataOther.Data(),crSel[iS].Data());
      TH1 * hd = metGetter->getHistogram(dataTree,sel,"1.0",TString::Format("tree_%u",0));
      TH1 * hq = metGetter->getHistogram(qcdTree,sel,stdQCDWeight,TString::Format("tree_%u",0));
      TH1 * corr = (TH1*)hd->Clone();
      TH1 * corrU = (TH1*)hd->Clone();
      cout << crSelNames[iS] << endl;
      for(unsigned int iB =1; iB <=hd->GetNbinsX(); ++iB){
        double dV = hd->GetBinContent(iB) < 10 ?  hq->GetBinContent(iB) + otherBKGScaledYields[iS]->GetBinContent(iB): hd->GetBinContent(iB);
        double oV = otherBKGScaledYields[iS]->GetBinContent(iB);
        double cV = 1 - oV/dV;
        corr->SetBinContent(iB,cV );
        corrU->SetBinContent(iB,1 + oV/dV );

        cout << "("<<cV <<","<<1 + oV/dV  <<") ";
      }
      cout << endl;
      dataYields.push_back(hd);
      qcdYields.push_back(hq);
      otherBKGCorr.push_back(corr);
      otherBKGCorrUnc.push_back(corrU);
    }
  }

  float getDataYield(CRReg reg, int metBin){ return dataYields[reg]->GetBinContent(metBin+1);}
  double getDataStatUnc(CRReg reg, int metBin) {
    double dY = getDataYield(reg,metBin);
    if(dY == 0) dY = 1;
    return TMath::Sqrt(dY)/dY + 1;
  };
  float getScaledOtherYield(CRReg reg, int metBin){ return otherBKGScaledYields[reg]->GetBinContent(metBin+1);}
  float getScaledOtherYieldStatUnc(CRReg reg, int metBin){ return otherBKGScaledYields[reg]->GetBinError(metBin+1);}
  float getDataYieldCorr(CRReg reg, int metBin){ return otherBKGCorr[reg]->GetBinContent(metBin+1);}
  float getDataYieldCorrUnc(CRReg reg, int metBin){ return otherBKGCorrUnc[reg]->GetBinContent(metBin+1);}
  TString getBinName (CRReg reg, int metBin) {return  TString::Format("%.0f_%s",metBins[metBin],crRegBinNames[reg].Data());}
  TString getLatexBlock(CRReg reg, int metBin){
    if(metBin+1 == nMETBins)
      return TString::Format("$>$%.0f & %.0f & %.2f $\\pm$ %.2f",metBins[metBin],getDataYield(reg,metBin),getScaledOtherYield(reg,metBin),getScaledOtherYieldStatUnc(reg,metBin));
    else
      return TString::Format("%.0f$-$%.0f & %.0f & %.2f $\\pm$ %.2f",metBins[metBin],metBins[metBin+1],getDataYield(reg,metBin),getScaledOtherYield(reg,metBin),getScaledOtherYieldStatUnc(reg,metBin));
  }
};



struct SRegInfo {
  static const int nSR = 10;
  static const int nMETBins = 5;
  enum SRReg{ SR_5_1_0_0, SR_7_1_0_0,SR_5_2_0_0, SR_7_2_0_0,SR_5_1_0_175, SR_7_1_0_175,SR_5_2_0_175, SR_7_2_0_175,SR_5_1_1_175, SR_5_2_1_175, };
  TString  srRegBinNames[nSR] = { "5_1_0_0", "7_1_0_0","5_2_0_0", "7_2_0_0","5_1_0_175", "7_1_0_175","5_2_0_175", "7_2_0_175","5_1_1_175", "5_2_1_175"};
  TString srSel[nSR] = {
      "mtcsv12met < 175 && nbjets == 1 && (njets == 5 || njets == 6)",
      "mtcsv12met < 175 && nbjets == 1 && njets >= 7",
      "mtcsv12met < 175 && nbjets >= 2 && (njets == 5 || njets == 6)",
      "mtcsv12met < 175 && nbjets >= 2 && njets >= 7",
      "mtcsv12met >= 175 && nbjets == 1 && (njets == 5 || njets == 6) && ncttstd == 0",
      "mtcsv12met >= 175 && nbjets == 1 && njets >= 7 && ncttstd == 0",
      "mtcsv12met >= 175 && nbjets >= 2 && (njets == 5 || njets == 6) && ncttstd == 0",
      "mtcsv12met >= 175 && nbjets >= 2 && njets >= 7 && ncttstd == 0",
      "mtcsv12met >= 175 && nbjets == 1 && ncttstd >= 1",
      "mtcsv12met >= 175 && nbjets >= 2  && ncttstd >= 1"
  };
    TString srSelNames[nSR] = {
        "#it{m}_{T,b} < 175, nb=1, nJ 5-6",
        "#it{m}_{T,b}< 175, nb=1, nJ #geq7",
        "#it{m}_{T,b} < 175, nb#geq2, nJ 5-6",
        "#it{m}_{T,b}< 175, nb#geq2, nJ #geq7",
        "#it{m}_{T,b} #geq 175,nb=1, nJ 5-6, nT 0",
        "#it{m}_{T,b} #geq 175,nb=1, nJ #geq7, nT 0",
        "#it{m}_{T,b} #geq 175,nb=2, nJ 5-6, nT 0",
        "#it{m}_{T,b} #geq 175,nb=2, nJ #geq7, nT 0",
        "#it{m}_{T,b} #geq 175,nb=1, nT #geq1",
        "#it{m}_{T,b} #geq 175,nb#geq2, nT #geq1"
    };
  TString srPreQCD =   TString::Format("%s && %s && (dphij12met >= 0.5  && dphij3met >= 0.5 && dphij4met >= 0.5 )", METPresel.Data(), BaselineExtraCuts.Data());

  CRegInfo::CRReg crRegs[nSR] = {
      CRegInfo::CR_5_1_0_0  ,
      CRegInfo::CR_7_1_0_0  ,
      CRegInfo::CR_5_1_0_0  ,
      CRegInfo::CR_7_1_0_0  ,
      CRegInfo::CR_5_1_0_175,
      CRegInfo::CR_7_1_0_175,
      CRegInfo::CR_5_1_0_175,
      CRegInfo::CR_7_1_0_175,
      CRegInfo::CR_5_1_1_175,
      CRegInfo::CR_5_1_1_175
  };




  double metBins[nMETBins+1] = {250,300,400,500,600,700};
  HistogramGetter * metGetter = new HistogramGetter("srmet","met","#slash{#it{E}}_{T} [GeV]", nMETBins,metBins);

  vector<TH1F*> nominalQCD;
  vector<TH1F*> respUp;
  vector<TH1F*> respDown;
  vector<TH1F*> noResp;
  vector<TH1F*> origQCD;
  TTree *qcdTree;
  TTree *origQcdTree;
  void fillTFVariations( TString qcdTreeIn,TString origTreeIn){
    fillTFVariations(getTree(qcdTreeIn),getTree(origTreeIn));

  }
  void fillTFVariations(TTree* qcdTreeIn,TTree * origTreeIn){
    qcdTree = qcdTreeIn;
    origQcdTree = origTreeIn;
    CRegInfo crInfo;
    for(unsigned int iS = 0; iS < nSR; ++iS){
      TString selN = TString::Format("%s && %s",srPreQCD.Data(),srSel[iS].Data());
      TString selD = TString::Format("%s && %s",crInfo.crPreQCD.Data(),crInfo.crSel[crRegs[iS]].Data());

      metGetter->nBootStraps = 50;
      TH1F * nom = metGetter->getTFAndCov(qcdTree,selN,stdQCDWeight,selD,stdQCDWeight,TString::Format("ratio_%u",iS));
      metGetter->nBootStraps = 0;

      TH1F * upN    = metGetter->getHistogram(qcdTree,selN,TString::Format("%s*upTailWeight",stdMCWeight.Data()),"QCD");
      TH1F * downN  = metGetter->getHistogram(qcdTree,selN,TString::Format("%s*downTailWeight",stdMCWeight.Data()),"QCD");
      TH1F * norN   = metGetter->getHistogram(qcdTree,selN,stdMCWeight,"QCD");
      TH1F * orN    = origQcdTree ? metGetter->getHistogram(origQcdTree,selN,stdMCWeight,"QCD") : 0;

      TH1F * upD    = metGetter->getHistogram(qcdTree,selD,TString::Format("%s*upTailWeight",stdMCWeight.Data()),"QCD");
      TH1F * downD  = metGetter->getHistogram(qcdTree,selD,TString::Format("%s*downTailWeight",stdMCWeight.Data()),"QCD");
      TH1F * norD   = metGetter->getHistogram(qcdTree,selD,stdMCWeight,"QCD");
      TH1F * orD    = origQcdTree ? metGetter->getHistogram(origQcdTree,selD,stdMCWeight,"QCD") : 0;

      upN   ->Divide(upD);
      downN ->Divide(downD);
      norN  ->Divide(norD);
      if(orN) orN   ->Divide(orD);

      nominalQCD.push_back(nom);
      respUp    .push_back(upN  );
      respDown  .push_back(downN);
      noResp    .push_back(norN );
      origQCD   .push_back(orN  );
  }
  }
  std::string convertToTemplateName(std::string inBinName, int metV){
    std::regex e ("(\\d+)_(\\d+)_(\\d+)_(\\d+)");
    std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
    bool matched = std::regex_match (inBinName,sm,e);

    assert(matched);
    assert(sm.size() == 5);
//    cout << inBinName << endl;
//    cout << sm.size() << endl;
    for(unsigned int i = 0; i < sm.size(); ++i) cout <<"["<<sm[i]<<"] ";
    cout << endl;;

    return  "template_met" + std::to_string(metV)+"_njets"+sm[1].str() +"_nbjets"+sm[2].str()+"_ncttstd"+sm[3].str()+"_mtcsv12met"+sm[4].str()+".txt_sr_template";
  }

  void getQCDUncertainties(std::string fIn, std::vector<std::string>& uncNames,std::vector<float>& uncs){
    uncs.clear();
    uncNames.clear();

    string line;
    ifstream inFile (fIn);
    if(!inFile.is_open())cout <<"Could not find: " << fIn << endl;
    assert(inFile.is_open());
    std::regex e ("^(\\S+)\\s+lnN\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+(\\S+)\\s*$");
    std::smatch sm;

    while(getline(inFile,line)){
      if (std::regex_match (line,sm,e)){
        assert(sm.size() == 3);
        bool isGood = true;
        float unc = -1;
        try {unc = stof(sm[2]);}catch (const std::invalid_argument& ia) {isGood = false;}
        if(TString(sm[1]).BeginsWith("qcd_")) isGood = false;
        if(isGood){
          uncs.push_back(unc);
          uncNames.push_back(sm[1]);
        }
      }
    }
  }


  vector<vector<double> > totalExtSystUncs = vector<vector<double> >(nSR,vector<double>(nMETBins,0));
  void loadExternUnc(std::string dcDir){
    cout << "Processing ext. uncs: "<< dcDir <<endl;
    int nUncs = -1;
    for(unsigned int iS = 0; iS < nSR; ++iS)
      for(unsigned int iM = 0; iM < nMETBins; ++iM){
        vector<float> uncs;
        vector<std::string> uncNames;
        getQCDUncertainties(dcDir+convertToTemplateName(srRegBinNames[iS].Data(),metBins[iM]),uncNames,uncs); //
        cout << getBinName(SRReg(iS),iM) << endl;
        for(unsigned int iU = 0; iU < uncs.size(); ++iU) cout << uncNames[iU] <<" -> "<< uncs[iU]<<std::endl;
        if(nUncs < 0){
          for(unsigned int iU = 0; iU < uncs.size(); ++iU) cout << uncNames[iU] <<" -> "<< uncs[iU]<<std::endl;
          nUncs = uncs.size();
        }
        assert(nUncs == uncs.size());
        double totalUnc =0;
        for(unsigned int iU = 0; iU < nUncs; ++iU){
          cout << fabs(uncs[iU]-1)*fabs(uncs[iU]-1) <<" ";
          totalUnc += fabs(uncs[iU]-1)*fabs(uncs[iU]-1);
        }
        cout << endl << sqrt(totalUnc) + 1 <<endl;
        totalExtSystUncs[iS][iM]= sqrt(totalUnc) + 1;
      }
  }

  double getTF(SRReg reg, int metBin){return nominalQCD[reg]->GetBinContent(metBin+1);}
  double getTFStatUnc(SRReg reg, int metBin){return min(2.0,nominalQCD[reg]->GetBinError(metBin+1)/nominalQCD[reg]->GetBinContent(metBin+1)+1);}
  double getRespUnc(SRReg reg, int metBin){return (respUp[reg]->GetBinContent(metBin+1) - respDown[reg]->GetBinContent(metBin+1) ) /(2*getTF(reg,metBin)) +1;}
  double getPrediction(CRegInfo* crinfo, SRReg reg, int metBin){
    double dY = crinfo->getDataYield(crRegs[reg],metBin) == 0 ? 1: crinfo->getDataYield(crRegs[reg],metBin);
    return dY*crinfo->getDataYieldCorr(crRegs[reg],metBin)*getTF(reg,metBin);
  }
  double getExtUncs(SRReg reg, int metBin){ return totalExtSystUncs[reg][metBin];}
  TString getBinName (SRReg reg, int metBin) {return  TString::Format("%.0f_%s",metBins[metBin],srRegBinNames[reg].Data());}
  double getTotalSystUnc(CRegInfo * crinfo,SRReg reg, int metBin){
    return TMath::Sqrt(   (getTFStatUnc(reg,metBin) -1)*(getTFStatUnc(reg,metBin) -1)
                        + (getRespUnc(reg,metBin) -1)*(getRespUnc(reg,metBin) -1)
                        + (getExtUncs(reg,metBin) -1)*(getExtUncs(reg,metBin) -1)
                        + (crinfo->getDataYieldCorrUnc(crRegs[reg],metBin)-1)*(crinfo->getDataYieldCorrUnc(crRegs[reg],metBin)-1)
                        ) + 1;
  }


  TString getLatexBlock (CRegInfo* crinfo, SRReg reg, int metBin) {
    double tf       = getTF(reg,metBin);
    double tfUnc    = tf*(getTFStatUnc(reg,metBin) - 1);
    double pred     = getPrediction(crinfo,reg,metBin);
    double predstat = pred*(crinfo->getDataStatUnc(crRegs[reg],metBin) - 1);
    double predsyst = pred*(getTotalSystUnc(crinfo,reg,metBin) - 1);

    if(tf < .0005 ) return TString::Format(" $<$0.001 & - ");
    return TString::Format("%.3f $\\pm$ %.3f & %.2f $\\pm$ %.3f (stat.) $\\pm$ %.3f (syst.)",tf,tfUnc,pred,predstat,predsyst );
  }
};


void checkCRRegion(){

  CRegInfo info;
  info.fillOtherBkgSFs("pieces/htmht_tree_skimmed_baseline.root","pieces/nonQCD_tree_skimmed_baseline.root","pieces/qcd_tree_skimmed_baseline.root");
  info.fillCorrectedOtherBkgYields();
  info.fillDataCorr();
  cout << "DONE!"<<endl;


  vector<TString> names;
  vector<TTree*> trees;
  vector<int> colors;

//  trees.emplace_back(getTree("pieces/qcd_tree_skimmed_baseline.root")); names.push_back("MC w/o #it{r}_{jet} corr");colors.push_back(color_znunu);
//  trees.emplace_back(getTree("pieces/qcd_origtree_skimmed_baseline.root")); names.push_back("MC w/o smearing or #it{r}_{jet} corr");colors.push_back(color_tW);
  trees.emplace_back(getTree("pieces/htmht_tree_skimmed_baseline.root")); names.push_back("Data"); colors.push_back(1);
  trees.emplace_back(getTree("pieces/nonQCD_tree_skimmed_baseline.root")); names.push_back("Non-QCD bkg");colors.push_back(color_ttbar);
  trees.emplace_back(getTree("pieces/qcd_tree_skimmed_baseline.root")); names.push_back("QCD MC"); colors.push_back(color_qcd);

  trees.emplace_back(getTree("pieces/T2tt_500_200_tree.root")); names.push_back("T2tt_500_200"); colors.push_back(color_znunu);
  trees.emplace_back(getTree("pieces/T2tt_700_1_tree.root")); names.push_back("T2tt_700_1"); colors.push_back(color_tW);

    for(unsigned int iP = 0; iP < info.nCR; ++iP){
    TString title = TString::Format("%s",info.crSelNames[iP].Data());
    TString name = TString::Format("getRegion_0_%u",iP);
    Plot * plot = new Plot(name,title,info.metGetter->plotInfo->xTitle,"Events");
    TString sel = TString::Format("%s && %s",info.crPreDataOther.Data(),info.crSel[iP].Data());
    for(unsigned int iT = 0; iT < trees.size(); ++iT){
      if(names[iT] == "Data"){
        TH1F * hSN = info.metGetter->getHistogram(trees[iT],sel,"1.0",TString::Format("tree_%u",iT));
        plot->addHist(hSN,names[iT],"",colors[iT],0,colors[iT]);
      }
      else if(names[iT] == "MC w/o #it{r}_{jet} corr"){
        TH1F * hSN = info.metGetter->getHistogram(trees[iT],sel,stdMCWeight,TString::Format("tree_%u",iT));
        if(iP == 4) hSN->Scale(0.7);
        hSN->Add(info.otherBKGScaledYields[iP]);
        plot->addHist(hSN,names[iT],"hist",colors[iT],0,colors[iT]);
      }
      else if(names[iT] == "MC w/o smearing or #it{r}_{jet} corr"){
        TH1F * hSN = info.metGetter->getHistogram(trees[iT],sel,stdMCWeight,TString::Format("tree_%u",iT));
        if(iP == 4) hSN->Scale(0.7);
        hSN->Add(info.otherBKGScaledYields[iP]);
        plot->addHist(hSN,names[iT],"",colors[iT],0,colors[iT]);
      }
      else if (names[iT] == "Non-QCD MC"){
        TH1F * hSN = (TH1F*)info.otherBKGScaledYields[iP]->Clone();
        plot->addToStack(hSN,names[iT],colors[iT]);
      }
      else if (names[iT].BeginsWith("T2tt")){
         TH1F * hSN = info.metGetter->getHistogram(trees[iT],sel,stdMCWeight,TString::Format("tree_%u",iT));
         plot->addHist(hSN,names[iT],"",colors[iT],0,colors[iT]);
       }
      else {
        TH1F * hSN = info.metGetter->getHistogram(trees[iT],sel,TString::Format("%s*qcdRespTailWeight",stdMCWeight.Data()),TString::Format("tree_%u",iT));
        if(iP == 4) hSN->Scale(0.7);
        plot->addToStack(hSN,names[iT],colors[iT]);
      }
    }

    TCanvas * c = new TCanvas;
    plot->setHeader(header,"");
    plot->setYRange(0.1,400);
    plot->setLogy();

    plot->draw(c,true,"png");
    setTitleOffset(c);
    c->SaveAs(plot->getName() + TString(".png"));
  }
}

void checkTF(){
  CRegInfo crinfo;
  crinfo.fillOtherBkgSFs("pieces/htmht_tree_skimmed_baseline.root","pieces/nonQCD_tree_skimmed_baseline.root","pieces/qcd_tree_skimmed_baseline.root");
  crinfo.fillCorrectedOtherBkgYields();
  crinfo.fillDataCorr();

  SRegInfo srinfo;
  srinfo.fillTFVariations("pieces/qcd_tree_skimmed_baseline.root","pieces/qcd_origtree_skimmed_baseline.root");

  for(unsigned int iS = 0; iS < srinfo.nSR; ++iS){
    Plot * plot = new Plot(TString::Format("TF_%u",iS),srinfo.srSelNames[iS],"","SR/CR");

    plot->addHist(srinfo.origQCD[iS],"w/o smearing or #it{r}_{jet} corr","",color_tW,0,color_tW);
    plot->addHist(srinfo.noResp[iS],"w/o #it{r}_{jet} corr","hist",color_znunu,0,color_znunu);
    plot->addHist(srinfo.respUp[iS],"#it{r}_{jet} up variation","hist",color_qcd,0,color_qcd);
    plot->addHist(srinfo.respDown[iS],"#it{r}_{jet} down variation","hist",5,0,5);
    plot->addHist(srinfo.nominalQCD[iS],"Nominal","",1,0,1);

    plot->setLegend(.35,.70,.92,.9);
    plot->setXTitle(srinfo.metGetter->plotInfo->xTitle);

    switch(iS){
    case 0: plot->setYRange(0.0001,3);break;
    case 1: plot->setYRange(0.0001,3);break;
    case 2: plot->setYRange(0.00005,.5);break;
    case 3: plot->setYRange(0.001,.1);break;
    case 4: plot->setYRange(0.0001,10);break;
    case 5: plot->setYRange(0.001,5);break;
    case 6: plot->setYRange(0.001,.5);break;
    case 7: plot->setYRange(0.001,.1);break;
    case 8: plot->setYRange(0.00005,300); break;
    case 9: plot->setYRange(0.0001,0.1);break;
    }
    plot->setLogy();
    plot->getLegend()->SetNColumns(2);
    plot->setHeader("","");
    TCanvas * c = new TCanvas;
    plot->draw(c,true,"png");
    setTitleOffset(c);
    c->SaveAs(plot->getName() + TString(".png"));

  }
}

void printTable() {
  CRegInfo crinfo;
  crinfo.fillOtherBkgSFs("pieces/htmht_tree_skimmed_baseline.root","pieces/nonQCD_tree_skimmed_baseline.root","pieces/qcd_tree_skimmed_baseline.root");
  crinfo.fillCorrectedOtherBkgYields();
  crinfo.fillDataCorr();

  SRegInfo srinfo;
  srinfo.fillTFVariations("pieces/qcd_tree_skimmed_baseline.root","pieces/qcd_origtree_skimmed_baseline.root");
  srinfo.loadExternUnc("extUncs/");

  cout <<"DataCard info!"<<endl;

  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins;++iM) cout <<"bin_"<< srinfo.getBinName(SRegInfo::SRReg(iS),iM) <<"\t" << "TF"<< "\tqcd\t" << srinfo.getTF(SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins;++iM) cout <<"bin_"<< srinfo.getBinName(SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_tfstatunc_"<< srinfo.getBinName(SRegInfo::SRReg(iS),iM)<<"\tqcd\t" << srinfo.getTFStatUnc(SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins;++iM) cout <<"bin_"<< srinfo.getBinName(SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_jetresptailunc"<< "\tqcd\t" << srinfo.getRespUnc(SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins;++iM) cout <<"bin_"<< srinfo.getBinName(SRegInfo::SRReg(iS),iM) <<"\t" << "Other MC yields"<< "\tqcd\t" << crinfo.getScaledOtherYield(srinfo.crRegs[iS],iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins;++iM) cout <<"bin_"<< srinfo.getBinName(SRegInfo::SRReg(iS),iM) <<"\t" << "Other MC yields corr"<< "\tqcd\t" << crinfo.getDataYieldCorr(srinfo.crRegs[iS],iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins;++iM) cout <<"bin_"<< srinfo.getBinName(SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_bkgsubunc_"<< crinfo.getBinName(srinfo.crRegs[iS],iM)<< "\tqcd\t" << crinfo.getDataYieldCorrUnc(srinfo.crRegs[iS],iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins;++iM) cout <<"bin_"<< srinfo.getBinName(SRegInfo::SRReg(iS),iM) <<"\t" << "External Unc"<< "\tqcd\t" << srinfo.getExtUncs(SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins;++iM) cout <<"bin_"<< srinfo.getBinName(SRegInfo::SRReg(iS),iM) <<"\t" << "Total syst"<< "\tqcd\t" << srinfo.getTotalSystUnc(&crinfo,SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins;++iM) cout <<"bin_"<< srinfo.getBinName(SRegInfo::SRReg(iS),iM) <<"\t" << "Data stat"<< "\tqcd\t" <<  crinfo.getDataStatUnc(srinfo.crRegs[iS],iM)  <<endl;
  cout << endl <<endl;

  cout <<"Table info!"<<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins;++iM) cout << crinfo.getLatexBlock(srinfo.crRegs[iS],iM) <<" :: "<<srinfo.getLatexBlock(&crinfo,SRegInfo::SRReg(iS),iM) << endl ;
  cout << endl <<endl;

  cout <<"Output table!"<<endl;

  auto printBigBlock = [&](SRegInfo::SRReg reg1,SRegInfo::SRReg reg2) {
    cout <<"\\hline"<<endl;
    cout <<"\\hline"<<endl;
    for(unsigned int iM = 0; iM < srinfo.nMETBins; ++iM){
      cout << crinfo.getLatexBlock(srinfo.crRegs[reg1],iM) << " & "<< srinfo.getLatexBlock(&crinfo,reg1,iM) <<" & " << srinfo.getLatexBlock(&crinfo,reg2,iM) <<"\\\\"<<endl;
    }
    cout <<"\\hline"<<endl;
  };

  cout << "\\begin{table}[!!htbp]"<<endl;
  cout << "\\begin{center}"<<endl;
//  cout <<"\\tiny"<<endl;
  cout <<"\\resizebox*{1\\textwidth}{!}{"<<endl;
  cout <<"\\begin{tabular}{|l|c|c||c|c||c|c|}"<<endl;
  cout <<"\\hline"<<endl;
  cout <<"& $N_{\\text{data}}$ & $N^{\\text{non-QCD}}_{\\text{MC}}$ & $\\tfqcd$ & $N_\\text{pred}^{\\text{QCD}}$ & $\\tfqcd$ & $N_\\text{pred}^{\\text{QCD}}$ \\\\"<<endl;
  cout <<"\\hline"<<endl;
  cout <<" & & & \\multicolumn{2}{c||}{$\\nb=1$} & \\multicolumn{2}{c|}{$\\nb \\geq2$} \\\\"<<endl;
  cout <<"\\hline"<<endl;
  cout <<" \\met [GeV] & & & \\multicolumn{4}{c|}{$\\mtb < 175$~\\GeV, $5-6$ jets} \\\\";
  printBigBlock(SRegInfo::SR_5_1_0_0,SRegInfo::SR_5_2_0_0);
  cout << "\\met [GeV] & & & \\multicolumn{4}{c|}{$\\mtb < 175$~\\GeV, $\\geq 7$ jets} \\\\"<<endl;
  printBigBlock(SRegInfo::SR_7_1_0_0,SRegInfo::SR_7_2_0_0);
  cout << " \\met [GeV] & & & \\multicolumn{4}{c|}{$\\mtb \\geq 175$~\\GeV, $\\nt = 0$, $5-6$ jets} \\\\"<<endl;
  printBigBlock(SRegInfo::SR_5_1_0_175,SRegInfo::SR_5_2_0_175);
  cout << "\\met [GeV] & & & \\multicolumn{4}{c|}{$\\mtb \\geq 175$~\\GeV, $\\nt = 0$, $\\geq 7$ jets} \\\\"<<endl;
  printBigBlock(SRegInfo::SR_7_1_0_175,SRegInfo::SR_7_2_0_175);
  cout << " \\met [GeV] & & & \\multicolumn{4}{c|}{$\\mtb \\geq 175$~\\GeV, $\\nt \\geq 1$, $\\geq 5$ jets} \\\\"<<endl;
  printBigBlock(SRegInfo::SR_5_1_1_175,SRegInfo::SR_5_2_1_175);
  cout <<"\\end{tabular}"<<endl;
  cout <<"}"<<endl;
  cout <<"\\caption{\\label{tab:0l-qcd-pred}The QCD estimation in the various search regions using the 2.3~\\invfb dataset. The number of data events in the control region with $\\dphijonetwothree<0.1$, which is used as the basis of the prediction in each search region, is also listed, along with the estimated contribution from non-QCD processes in the control region based on simulation. A $100\\%$ uncertainty is assigned on the non-QCD contamination when subtracting it from the data observed in the control region in order to obtain the QCD prediction.}"<<endl;
  cout <<"\\end{center}"<<endl;
  cout <<"\\end{table}"<<endl;
}


//  for(unsigned int iS1 = 0; selNum[iS1][0]; ++iS1){
//    TString name = TString::Format("TF_%u",iS1);
//    TString denSelelction = TString::Format("%s && %s",denpre.Data(),selDen[iS1].Data());
//    TString numSelelection = TString::Format("%s && %s",numpre.Data(),selNum[iS1].Data());
//
//    Plot * plot = new Plot(name,selNames[iS1],"","SR/CR");
//
////    for(unsigned int iB =0; iB < nBinsX; ++iB){
////      binNames.push_back(TString::Format("bin_%.0f_%s",bins[iB],selNumName[iS1].Data()));
////      binDenoms.push_back(binDenomMap[iS1]);
////      binMets.push_back(iB+1);
////    }
////
////    for(unsigned int iT = 0; iT < names.size(); ++iT){
////      TString newWeight = TString::Format("%s*%s",stdMCWeight.Data(),weights[iT].Data());
////      TH1F * hSN = 0;
////      TTree * tree = trees[iT];
////      if(doBootsrap[iT] ){
////        histG.nBootStraps = 50;
////        hSN = histG.getTFAndCov(tree,numSelelection,newWeight,denSelelction,newWeight,TString::Format("ratio_%u",iT));
////      } else {
////        histG.nBootStraps = 0;
////        hSN = histG.getHistogram(tree,numSelelection,newWeight,TString::Format("num_%u",iT));
////        TH1F * hSD = histG.getHistogram(tree,denSelelction,newWeight,TString::Format("num_%u",iT));
////        hSN->Divide(hSD);
////      }
////      plot->addHist(hSN,names[iT],showErrors[iT] == false ? "hist": "",colors[iT],0,colors[iT]);
////
////      if(names[iT] == "Nominal")
////        for(unsigned int iB =0; iB < nBinsX; ++iB){
////          TF.push_back(hSN->GetBinContent(iB+1));
////          TFUnc.push_back(min(2.0,(hSN->GetBinError(iB+1)/hSN->GetBinContent(iB+1)) + 1  ));
////        }
////      if(names[iT] == "#it{r}_{jet} up variation")
////        for(unsigned int iB =0; iB < nBinsX; ++iB){
////          jetTailUncUp.push_back(hSN->GetBinContent(iB+1));
////        }
////      if(names[iT] == "#it{r}_{jet} down variation")
////        for(unsigned int iB =0; iB < nBinsX; ++iB){
////          jetTailUncDown.push_back(hSN->GetBinContent(iB+1));
////        }
////
////
////    }
//    plot->setLegend(.35,.70,.92,.9);
//    plot->setXTitle(histG.plotInfo->xTitle);
//
//    switch(iS1){
//    case 0: plot->setYRange(0.0001,3);break;
//    case 1: plot->setYRange(0.0001,3);break;
//    case 2: plot->setYRange(0.00005,.5);break;
//    case 3: plot->setYRange(0.001,.1);break;
//    case 4: plot->setYRange(0.0001,10);break;
//    case 5: plot->setYRange(0.001,5);break;
//    case 6: plot->setYRange(0.001,.5);break;
//    case 7: plot->setYRange(0.001,.1);break;
//    case 8: plot->setYRange(0.00005,300); break;
//    case 9: plot->setYRange(0.0001,0.1);break;
//    }
//    plot->setLogy();
//    plot->getLegend()->SetNColumns(2);
//    plot->setHeader("","");
//    TCanvas * c = new TCanvas;
//    plot->draw(c,true,"png");
//    setTitleOffset(c);
//    c->SaveAs(plot->getName() + TString(".png"));
//  }

//
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "TF"<< "\tqcd\t" << TF[iB] <<endl;
//      cout << endl <<endl;
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t"  << "qcd_tfstatunc_"<< binNames[iB] << "\tqcd\t" << TFUnc[iB] <<endl;
//      cout << endl <<endl;
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "qcd_jetresptailunc"<< "\tqcd\t" << ((jetTailUncUp[iB] - jetTailUncDown[iB])  /(2*TF[iB]) + 1 ) <<endl;
//      cout << endl <<endl;
//
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "..."<< "\tqcd\t" << binDenoms[iB] <<" "<< binMets[iB] <<endl;
//      cout << endl <<endl;
//
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "Other MC yields"<< "\tqcd\t" << info.otherBKGScaledYields[binDenoms[iB]]->GetBinContent(binMets[iB]) <<endl;
//      cout << endl <<endl;
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "Other MC yields corr"<< "\tqcd\t" << info.otherBKGCorr[binDenoms[iB]]->GetBinContent(binMets[iB]) <<endl;
//      cout << endl <<endl;
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "qcd_bkgsubunc_"<< binDenoms[iB] <<"_"<< binMets[iB]  << "\tqcd\t"  <<  info.otherBKGCorrUnc[binDenoms[iB]]->GetBinContent(binMets[iB]) <<endl;
//      cout << endl <<endl;
//



//}

//void checkTF(){
//
//  TString numpre = TString::Format("%s && %s && (dphij12met > .5 && dphij3met > .5 && dphij4met > .5)", METPresel.Data(), BaselineExtraCuts.Data());
//  TString denpre = TString::Format("%s && %s && (dphij12met < .1 || dphij3met < .1)", METPresel.Data(), BaselineExtraCuts.Data());
//
//  TString selNum[] = {"mtcsv12met < 175 && nbjets == 1 && (njets == 5 || njets == 6)",
//                      "mtcsv12met < 175 && nbjets == 1 && njets >= 7",
//                      "mtcsv12met < 175 && nbjets >= 2 && (njets == 5 || njets == 6)",
//                      "mtcsv12met < 175 && nbjets >= 2 && njets >= 7",
//                      "mtcsv12met >= 175 && nbjets == 1 && (njets == 5 || njets == 6) && ncttstd == 0",
//                      "mtcsv12met >= 175 && nbjets == 1 && njets >= 7 && ncttstd == 0",
//                      "mtcsv12met >= 175 && nbjets >= 2 && (njets == 5 || njets == 6) && ncttstd == 0",
//                      "mtcsv12met >= 175 && nbjets >= 2 && njets >= 7 && ncttstd == 0",
//                      "mtcsv12met >= 175 && nbjets == 1 && ncttstd >= 1",
//                      "mtcsv12met >= 175 && nbjets >= 2  && ncttstd >= 1",
//                      ""};
//  TString selDen[] = {"mtcsv12met < 175  && (njets == 5 || njets == 6)",
//                      "mtcsv12met < 175  && njets >= 7",
//                      "mtcsv12met < 175  && (njets == 5 || njets == 6)",
//                      "mtcsv12met < 175  && njets >= 7",
//                      "mtcsv12met >= 175 && (njets == 5 || njets == 6) && ncttstd == 0",
//                      "mtcsv12met >= 175 && njets >= 7 && ncttstd == 0",
//                      "mtcsv12met >= 175 && (njets == 5 || njets == 6) && ncttstd == 0",
//                      "mtcsv12met >= 175 && njets >= 7 && ncttstd == 0",
//                      "mtcsv12met >= 175 && ncttstd >= 1",
//                      "mtcsv12met >= 175 && ncttstd >= 1",
//                      ""};
//  TString selNames[] = {
//      "#it{m}_{T,b} < 175, nb=1, nJ 5-6",
//      "#it{m}_{T,b}< 175, nb=1, nJ #geq7",
//      "#it{m}_{T,b} < 175, nb#geq2, nJ 5-6",
//      "#it{m}_{T,b}< 175, nb#geq2, nJ #geq7",
//      "#it{m}_{T,b} #geq 175,nb=1, nJ 5-6, nT 0",
//      "#it{m}_{T,b} #geq 175,nb=1, nJ #geq7, nT 0",
//      "#it{m}_{T,b} #geq 175,nb=2, nJ 5-6, nT 0",
//      "#it{m}_{T,b} #geq 175,nb=2, nJ #geq7, nT 0",
//      "#it{m}_{T,b} #geq 175,nb=1, nT #geq1",
//      "#it{m}_{T,b} #geq 175,nb#geq2, nT #geq1",
//      ""
//  };
//
//  TString selNumName[] = {"5_1_0_0",
//                          "7_1_0_0",
//                          "5_2_0_0",
//                          "7_2_0_0",
//                          "5_1_0_175",
//                          "7_1_0_175",
//                          "5_2_0_175",
//                          "7_2_0_175",
//                          "5_1_1_175",
//                          "5_2_1_175",
//                          ""};
//
//  vector<int> binDenomMap;
//  binDenomMap.push_back(CRegInfo::CR_5_1_0_0);
//  binDenomMap.push_back(CRegInfo::CR_7_1_0_0);
//  binDenomMap.push_back(CRegInfo::CR_5_1_0_0);
//  binDenomMap.push_back(CRegInfo::CR_7_1_0_0);
//  binDenomMap.push_back(CRegInfo::CR_5_1_0_175);
//  binDenomMap.push_back(CRegInfo::CR_7_1_0_175);
//  binDenomMap.push_back(CRegInfo::CR_5_1_0_175);
//  binDenomMap.push_back(CRegInfo::CR_7_1_0_175);
//  binDenomMap.push_back(CRegInfo::CR_5_1_1_175);
//  binDenomMap.push_back(CRegInfo::CR_5_1_1_175);
//
//
////  enum CRReg{ CR_5_1_0_0, CR_7_1_0_0,CR_5_1_0_175, CR_7_1_0_175, CR_5_1_1_175 };
//
//  CRegInfo info;
//  info.fillOtherBkgSFs("pieces/htmht_tree_skimmed_baseline.root","pieces/nonQCD_tree_skimmed_baseline.root","pieces/qcd_tree_skimmed_baseline.root");
//  info.fillCorrectedOtherBkgYields();
//  info.fillDataCorr();
//
//
//
//  vector<TString> binNames;
//  vector<int> binDenoms;
//  vector<int> binMets;
//  vector<double> TF;
//  vector<double> TFUnc;
//  vector<double> jetTailUncUp;
//  vector<double> jetTailUncDown;
//
//  vector<HistogramGetter*> histGs;
//  double bins[] = {250,300,400,500,600,700};
//  int nBinsX = 5;
//  HistogramGetter histG("met","met","#slash{#it{E}}_{T}", nBinsX,bins);
//
//  vector<TString> names;
//  vector<TString> weights;
//  vector<bool> doBootsrap;
//  vector<bool> showErrors;
//  vector<int> colors;
//  vector<TTree*> trees;
//  trees.emplace_back(getTree("pieces/qcd_origtree_skimmed_baseline.root")); names.push_back("w/o smearing or #it{r}_{jet} corr"); weights.push_back("1.0");doBootsrap.push_back(false);showErrors.push_back(true);colors.push_back(color_tW);
//  trees.emplace_back(getTree("pieces/qcd_tree_skimmed_baseline.root")); names.push_back("w/o #it{r}_{jet} corr");weights.push_back("1.0");doBootsrap.push_back(false);showErrors.push_back(false);colors.push_back(color_znunu);
//  trees.emplace_back(getTree("pieces/qcd_tree_skimmed_baseline.root")); names.push_back("#it{r}_{jet} up variation");weights.push_back("upTailWeight");doBootsrap.push_back(false);showErrors.push_back(false);colors.push_back(color_qcd);
//  trees.emplace_back(getTree("pieces/qcd_tree_skimmed_baseline.root")); names.push_back("#it{r}_{jet} down variation");weights.push_back("downTailWeight");doBootsrap.push_back(false);showErrors.push_back(false);colors.push_back(5);
//  trees.emplace_back(getTree("pieces/qcd_tree_skimmed_baseline.root")); names.push_back("Nominal");weights.push_back("qcdRespTailWeight");doBootsrap.push_back(true);showErrors.push_back(true);colors.push_back(1.0);
//
//
//
//  for(unsigned int iS1 = 0; selNum[iS1][0]; ++iS1){
//    TString name = TString::Format("TF_%u",iS1);
//    TString denSelelction = TString::Format("%s && %s",denpre.Data(),selDen[iS1].Data());
//    TString numSelelection = TString::Format("%s && %s",numpre.Data(),selNum[iS1].Data());
//
//    Plot * plot = new Plot(name,selNames[iS1],"","SR/CR");
//
//    for(unsigned int iB =0; iB < nBinsX; ++iB){
//      binNames.push_back(TString::Format("bin_%.0f_%s",bins[iB],selNumName[iS1].Data()));
//      binDenoms.push_back(binDenomMap[iS1]);
//      binMets.push_back(iB+1);
//    }
//
//    for(unsigned int iT = 0; iT < names.size(); ++iT){
//      TString newWeight = TString::Format("%s*%s",stdMCWeight.Data(),weights[iT].Data());
//      TH1F * hSN = 0;
//      TTree * tree = trees[iT];
//      if(doBootsrap[iT] ){
//        histG.nBootStraps = 50;
//        hSN = histG.getTFAndCov(tree,numSelelection,newWeight,denSelelction,newWeight,TString::Format("ratio_%u",iT));
//      } else {
//        histG.nBootStraps = 0;
//        hSN = histG.getHistogram(tree,numSelelection,newWeight,TString::Format("num_%u",iT));
//        TH1F * hSD = histG.getHistogram(tree,denSelelction,newWeight,TString::Format("num_%u",iT));
//        hSN->Divide(hSD);
//      }
//      plot->addHist(hSN,names[iT],showErrors[iT] == false ? "hist": "",colors[iT],0,colors[iT]);
//
//      if(names[iT] == "Nominal")
//        for(unsigned int iB =0; iB < nBinsX; ++iB){
//          TF.push_back(hSN->GetBinContent(iB+1));
//          TFUnc.push_back(min(2.0,(hSN->GetBinError(iB+1)/hSN->GetBinContent(iB+1)) + 1  ));
//        }
//      if(names[iT] == "#it{r}_{jet} up variation")
//        for(unsigned int iB =0; iB < nBinsX; ++iB){
//          jetTailUncUp.push_back(hSN->GetBinContent(iB+1));
//        }
//      if(names[iT] == "#it{r}_{jet} down variation")
//        for(unsigned int iB =0; iB < nBinsX; ++iB){
//          jetTailUncDown.push_back(hSN->GetBinContent(iB+1));
//        }
//
//
//    }
//    plot->setLegend(.35,.70,.92,.9);
//    plot->setXTitle(histG.plotInfo->xTitle);
//
//    switch(iS1){
//    case 0: plot->setYRange(0.0001,3);break;
//    case 1: plot->setYRange(0.0001,3);break;
//    case 2: plot->setYRange(0.00005,.5);break;
//    case 3: plot->setYRange(0.001,.1);break;
//    case 4: plot->setYRange(0.0001,10);break;
//    case 5: plot->setYRange(0.001,5);break;
//    case 6: plot->setYRange(0.001,.5);break;
//    case 7: plot->setYRange(0.001,.1);break;
//    case 8: plot->setYRange(0.00005,300); break;
//    case 9: plot->setYRange(0.0001,0.1);break;
//    }
//    plot->setLogy();
//    plot->getLegend()->SetNColumns(2);
//    plot->setHeader("","");
//    TCanvas * c = new TCanvas;
//    plot->draw(c,true,"png");
//    setTitleOffset(c);
//    c->SaveAs(plot->getName() + TString(".png"));
//  }
//
//
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "TF"<< "\tqcd\t" << TF[iB] <<endl;
//      cout << endl <<endl;
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t"  << "qcd_tfstatunc_"<< binNames[iB] << "\tqcd\t" << TFUnc[iB] <<endl;
//      cout << endl <<endl;
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "qcd_jetresptailunc"<< "\tqcd\t" << ((jetTailUncUp[iB] - jetTailUncDown[iB])  /(2*TF[iB]) + 1 ) <<endl;
//      cout << endl <<endl;
//
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "..."<< "\tqcd\t" << binDenoms[iB] <<" "<< binMets[iB] <<endl;
//      cout << endl <<endl;
//
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "Other MC yields"<< "\tqcd\t" << info.otherBKGScaledYields[binDenoms[iB]]->GetBinContent(binMets[iB]) <<endl;
//      cout << endl <<endl;
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "Other MC yields corr"<< "\tqcd\t" << info.otherBKGCorr[binDenoms[iB]]->GetBinContent(binMets[iB]) <<endl;
//      cout << endl <<endl;
//      for(unsigned int iB = 0; iB < TF.size(); ++iB)
//        cout << binNames[iB] <<"\t" << "qcd_bkgsubunc_"<< binDenoms[iB] <<"_"<< binMets[iB]  << "\tqcd\t"  <<  info.otherBKGCorrUnc[binDenoms[iB]]->GetBinContent(binMets[iB]) <<endl;
//      cout << endl <<endl;
//
//
//
//
//}

#endif


void GetQCDPrediction()
{

  StyleTools::SetStyle();
  gStyle->SetTitleOffset(1.400,"Y");
  gStyle->SetTitleOffset(0.950,"X");

//  gStyle->SetTitleOffset(1.00,"Y");
//  gStyle->SetTitleOffset(0.950,"X");
//  gStyle->SetPadBottomMargin(0.5);
//  gStyle->SetPadTopMargin   (0.08);
//  gStyle->SetPadLeftMargin  (0.15);
//  gStyle->SetPadRightMargin (0.05);
  getMotivationPlots();
//  getSmearComparision();
//  getTailSFRegion();
//  checkCRRegion();
//  checkCRContamination();
//  checkCRRegion();
//  checkTF();
//  printTable();
  //  prepareTrees();
//  doFullExtraction();
//  getRegion();
//  checkContamination();
//  getSFs();
//  test();
}
