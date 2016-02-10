#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

enum TreeType {Data,QCD, NoSmearQCD, TTbarW, Znuu, ttZ};
const TString TreeNames[] = {"Data", "QCD","NoSmearQCD","t#bar{t}W","Z#rightarrow#nu#nu","t#bar{t}Z"};
const StyleTools::Colors TreeColors[] = {color_data, color_qcd,color_qcd,color_ttbar,color_znunu,color_ttZ};
const TString weights[] = {QCDSupport::stdWeight,QCDSupport::stdMCWeight,QCDSupport::stdMCWeight,QCDSupport::stdMCWeight,QCDSupport::stdMCWeight,QCDSupport::stdMCWeight};

void getSTDTreesAndNames(vector<pair<TTree*,TreeType> >& trees){
  trees.emplace_back(QCDSupport::getTree("pieces/htmht_tree_skimmed.root"),Data);
  trees.emplace_back(QCDSupport::getTree("pieces/ttZ_tZq_skimmed.root"),ttZ);
  trees.emplace_back(QCDSupport::getTree("pieces/znunu_tree_skimmed.root"),Znuu);
  trees.emplace_back(QCDSupport::getTree("pieces/ttbarplusw_tree_skimmed.root"),TTbarW);
  trees.emplace_back(QCDSupport::getTree("pieces/qcd_Tree_skimmed.root")     ,QCD);
//  trees.emplace_back(getTree("qcd_origTree.root") ,NoSmearQCD);
}

void getMotivationPlots() {
//  //Get dPhi plot
  TString preselection = TString::Format("%s && %s",QCDSupport::METPresel.Data(),QCDSupport::BaselineExtraCuts.Data());
  HistogramGetter dphiGetter("dphi","min(dphij12met,dphij3met)","#Delta#phi_{123}",22,0,1.1);
  vector<pair<TTree*,TreeType> > dPhiTrees;
  getSTDTreesAndNames(dPhiTrees);
    Plot * plot = new Plot("dPhi","",dphiGetter.plotInfo->xTitle,"Events");
    TString sel = preselection;
    for(unsigned int iT = 0; iT < dPhiTrees.size(); ++iT){
      if(dPhiTrees[iT].second == Data) continue;
      TH1F * hSN = dphiGetter.getHistogram(dPhiTrees[iT].first,sel,weights[dPhiTrees[iT].second],TString::Format("tree_%u",iT));
      plot->addToStack(hSN,TreeNames[dPhiTrees[iT].second],TreeColors[dPhiTrees[iT].second],1001,1,1,3);
    }
    TCanvas * c = new TCanvas;
//    c->SetLeftMargin  (0.18);
//    c->SetTopMargin   (0.10);
//    c->SetRightMargin (0.07);
//    c->SetBottomMargin(0.03);
    plot->setYRange(65,2000);
    plot->setLogy();
//    plot->setHeader(QCDSupport::header,"");
    plot->setDrawCMSLumi();
    plot->draw(c,true,"pdf");

    //Now do distribution plots
    TTree * qcdTree = QCDSupport::getTree("1_17_version/qcd_resTest_smearWithExtraCut_skimmed.root");
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
        plot->addHist(hSN,selNames[iP],"hist",StyleTools::colorGetter(iP),0,StyleTools::colorGetter(iP));
      }


      TCanvas * c = new TCanvas;
      plot->setLegend(.45,.65,.92,.9);
      plot->setYRange(0.,1.1);
//      plot->setHeader(QCDSupport::header,"");
      plot->setDrawCMSLumi();
      plot->draw(c,true,"pdf");
      QCDSupport::setTitleOffset(c);
      c->SaveAs(plot->getName() + TString(".pdf"));
    }



    //Now do b-integration
    TString numpre = TString::Format("%s && %s && (dphij12met > .5 && dphij3met > .5 && dphij4met > .5)", QCDSupport::METPresel.Data(), QCDSupport::BaselineExtraCuts.Data());
    TString denpre = TString::Format("%s && %s && (dphij12met < .1 || dphij3met < .1)", QCDSupport::METPresel.Data(), QCDSupport::BaselineExtraCuts.Data());
    TString selNum[] = {"nbjets == 1",
                        "nbjets >= 2",
                        ""};
    TString selDen[] = {"nbjets == 1",
        "nbjets >= 2",
                        ""};
    TString metSelNames[] = {"# of b-jets = 1","# of b-jets #geq 2",""};
    double bins[] = {250,300,400,500,600,700};
    HistogramGetter metHistG("met","met","#slash{#it{E}}_{T} [GeV]", 5,bins);
    TTree * qcdTree2 = QCDSupport::getTree("pieces/qcd_tree_skimmed_baseline.root");

    Plot * metplot = new Plot("bIntegration","",metHistG.plotInfo->xTitle,"SR/CR");

    for(unsigned int iS1 = 0; selNum[iS1][0]; ++iS1){
      TString title = TString::Format("%s",selNum[iS1].Data());
      TString name = TString::Format("checkTF_%u",iS1);
      TString denSelelction = TString::Format("%s && %s",denpre.Data(),selDen[iS1].Data());
      TString numSelelection = TString::Format("%s && %s",numpre.Data(),selNum[iS1].Data());
      metHistG.setNBS(50);
      TH1F *  hSN = metHistG.getTFAndCov(qcdTree2,numSelelection,QCDSupport::stdMCWeight,denSelelction,QCDSupport::stdMCWeight,TString::Format("ratio_%u",iS1));
      int color = StyleTools::colorGetter(iS1 >= 3 ? iS1 -3 : iS1 );
      metplot->addHist(hSN,metSelNames[iS1],"",color,0,color,iS1 >= 3 ? 3 : 1);
      }
    metplot->setYRange(0.,0.15);
      TCanvas * c2 = new TCanvas;
//      metplot->setHeader(QCDSupport::header,"");
      metplot->setDrawCMSLumi();
      metplot->draw(c2,true,"pdf");
      QCDSupport::setTitleOffset(c2);
      c2->SaveAs(metplot->getName() + TString(".pdf"));


}

void getSmearComparision() {
//  //Get dPhi plot
  TString preselection = TString::Format("%s && %s ",QCDSupport::METPresel.Data(),QCDSupport::BaselineExtraCuts.Data());
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

  TTree * smearQCD = QCDSupport::getTree("pieces/qcd_tree_skimmed_baseline.root");
  TTree * origQCD = QCDSupport::getTree("pieces/qcd_origtree_skimmed_baseline.root");




      for(unsigned int iP = 0; sels[iP][0]; ++iP){
        TString name = TString::Format("smearComp_%u",iP);
        Plot * plot = new Plot(name,selNames[iP],metHistG.plotInfo->xTitle,"Events");
        TString sel = TString::Format("%s && %s",preselection.Data(),sels[iP].Data());
        metHistG.setNBS(50);TH1F * hSS = metHistG.getHistogramManual(smearQCD,sel,"weight*2.26","QCD");
        metHistG.setNBS(0); TH1F * hSO = metHistG.getHistogram(origQCD,sel,"weight*2.26","QCD2");
        plot->addHist(hSO,"Original QCD MC","",TreeColors[TTbarW],0,TreeColors[TTbarW]);
        plot->addHist(hSS,"Smeared QCD MC","",TreeColors[Znuu],0,TreeColors[Znuu]);


        TCanvas * c = new TCanvas;
//        plot->setHeader(QCDSupport::header,"");
        plot->setYRange(0.5,5000);
        plot->setLogy();

        plot->setDrawCMSLumi();
        plot->drawRatios(c,1,true,"pdf");
        QCDSupport::setTitleOffset(c,.850);
        c->SaveAs(plot->getName() + TString(".pdf"));
      }

}

void getTailSFRegion(){
  TString presel = TString::Format("%s && %s", QCDSupport::METPresel.Data(),QCDSupport::ResTailExtraCuts.Data());
  TString sels[] = {"pseudoRespCSV < 0.605","pseudoRespCSV >= 0.890",""};
  TString selNames[] = {"Jet is not light b-tagged","Jet is medium b-tagged",""};


  double bins[] = {0,.1,.33,.5,.66,.8,1};
  HistogramGetter histG("pseudoResp","pseudoResp","#it{r}_{pseudo,jet}", 6,bins);

  vector<TString> names1;
  vector<TTree*> trees1;
  vector<int> colors1;
  trees1.emplace_back(QCDSupport::getTree("pieces/htmht_tree_skimmed.root")); names1.push_back("Data"); colors1.push_back(1);
  trees1.emplace_back(QCDSupport::getTree("pieces/nonQCD_tree_skimmed.root")); names1.push_back("Non-QCD MC"); colors1.push_back(StyleTools::color_ttbar);
  trees1.emplace_back(QCDSupport::getTree("pieces/qcd_tree_skimmed_dphi_split_b_inc.root"));      names1.push_back("b"); colors1.push_back(StyleTools::color_znunu);
  trees1.emplace_back(QCDSupport::getTree("pieces/qcd_tree_skimmed_dphi_split_light_gtp66.root"));  names1.push_back("l, r > 0.66"); colors1.push_back(821);
  trees1.emplace_back(QCDSupport::getTree("pieces/qcd_tree_skimmed_dphi_split_light_p33top66.root"));  names1.push_back("l, r 0.33-0.66"); colors1.push_back(811);
  trees1.emplace_back(QCDSupport::getTree("pieces/qcd_tree_skimmed_dphi_split_light_ltp33.root"));  names1.push_back("l, r < 0.33"); colors1.push_back(812);


  vector<TString> names2;
  vector<TTree*> trees2;
  vector<int> colors2;
  trees2.emplace_back(QCDSupport::getTree("pieces/htmht_tree_skimmed.root")); names2.push_back("Data"); colors2.push_back(1);
  trees2.emplace_back(QCDSupport::getTree("pieces/nonQCD_tree_skimmed.root")); names2.push_back("Non-QCD MC"); colors2.push_back(StyleTools::color_ttbar);
  trees2.emplace_back(QCDSupport::getTree("pieces/qcd_tree_skimmed_dphi_split_light_inc.root"));  names2.push_back("l"); colors2.push_back(StyleTools::color_znunu);
  trees2.emplace_back(QCDSupport::getTree("pieces/qcd_tree_skimmed_dphi_split_b_gtp50.root"));      names2.push_back("b, r >0.5"); colors2.push_back(811);
  trees2.emplace_back(QCDSupport::getTree("pieces/qcd_tree_skimmed_dphi_split_b_ltp50.root"));names2.push_back("b, r < 0.5"); colors2.push_back(812);




  for(unsigned int iP = 0; sels[iP][0]; ++iP){
    vector<TString>* names  = iP == 0 ? &names1 : &names2;
    vector<TTree* >* trees  = iP == 0 ? &trees1 : &trees2;
    vector<int    >* colors = iP == 0 ? &colors1 : &colors2;

    TString name = TString::Format("tailSFReg_%u",iP);
    Plot * plot = new Plot(name,selNames[iP],histG.plotInfo->xTitle,"Events");
    TString sel = TString::Format("%s && %s",presel.Data(),sels[iP].Data());
    for(unsigned int iT = 0; iT < trees->size(); ++iT){
      if((*names)[iT] == "Data"){
        TH1F * hSN = histG.getHistogram((*trees)[iT],sel,"1.0",TString::Format("tree_%u",iT));
        plot->addHist(hSN,(*names)[iT],"",(*colors)[iT],0,(*colors)[iT]);
      } else {
        TH1F * hSN = histG.getHistogram((*trees)[iT],sel,TString::Format("%s",QCDSupport::stdMCWeight.Data()),TString::Format("tree_%u",iT));
        if((*names)[iT] == "Non-QCD MC"){
          if(iP == 0) hSN->Scale(0.886424);
          if(iP == 1) hSN->Scale(0.77299);
        }
        plot->addToStack(hSN,(*names)[iT],(*colors)[iT],1001,1,1,3);
      }

    }
    TCanvas * c = new TCanvas;
    plot->setHeader(QCDSupport::header,"");
//    plot->setLogy();
    if(iP == 1){
      plot->setYRange(0.,1000);
    } else {
      plot->setYRange(0.,4500);
    }
    plot->setLegend(.5,.65,.92,.87);
    plot->getLegend()->SetNColumns(2);
    plot->setDrawCMSLumi();
    plot->drawRatioStack(c,true,"pdf");
    QCDSupport::setTitleOffset(c,.850);
    c->SaveAs(plot->getName() + TString(".pdf"));

  }
}

#endif

void MakeExtraANPlots(){
  StyleTools::SetTDRStyle();
  gStyle->SetTitleOffset(1.400,"Y");
  gStyle->SetTitleOffset(0.950,"X");
  gStyle->SetPadTopMargin   (0.08);
  getMotivationPlots();
  getSmearComparision();
  getTailSFRegion();
}
