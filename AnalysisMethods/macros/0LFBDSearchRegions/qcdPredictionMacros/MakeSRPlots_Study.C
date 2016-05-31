#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;
TString output_dir = "study_of_SR_variables";

void go(){
  gSystem->mkdir(output_dir, true);
  QCDSupport::SRegInfo srinfo;
//  srinfo.fillOtherBkgSFs(QCDSupport::inputDir + "/met_tree_skimmed_baseline.root",QCDSupport::inputDir + "/nonQCD_tree_skimmed_baseline.root",QCDSupport::inputDir + "/qcd_tree_skimmed_baseline.root");
//  srinfo.fillCorrectedOtherBkgYields();
  cout << "DONE!"<<endl;

  vector <TString>          variations;
  vector <TString>          var_Names;
  vector <HistogramGetter*> var_HistGs;
  variations.push_back(srinfo.srPreQCD);                                                                                                 var_HistGs.push_back(new HistogramGetter("metoverht", "met/sqrt(ht)", "#slash{#it{E}}_{T}/#sqrt{h_{T}}",     27, 3,  30) ); var_Names.push_back("metoverht_bl_m_metoverht");
  variations.push_back(srinfo.srPreQCD + " && (met/sqrt(ht)>=10)");                                                                      var_HistGs.push_back(new HistogramGetter("dphij3met", "dphij3met",    "|#Delta#phi(j_{3}, #slash{E}_{T})|",  32, 0., 3.2)); var_Names.push_back("dphij3met_bl");
  variations.push_back(srinfo.srPreQCD);                                                                                                 var_HistGs.push_back(new HistogramGetter("dphij3met", "dphij3met",    "|#Delta#phi(j_{3}, #slash{E}_{T})|",  32, 0., 3.2)); var_Names.push_back("dphij3met_bl_m_metoverht");
  variations.push_back(QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts + " && (dphij1met >= 0.5) && (met/sqrt(ht)>=10)"); var_HistGs.push_back(new HistogramGetter("dphij2met", "dphij2met",    "|#Delta#phi(j_{2}, #slash{E}_{T})|",  32, 0., 3.2)); var_Names.push_back("dphij2met_bl_m_dphij2met");
  variations.push_back(QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts + " && (dphij1met >= 0.5)");                       var_HistGs.push_back(new HistogramGetter("dphij2met", "dphij2met",    "|#Delta#phi(j_{2}, #slash{E}_{T})|",  32, 0., 3.2)); var_Names.push_back("dphij2met_bl_m_dphij2met_m_metoverht");
  variations.push_back(QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts + " && (dphij2met >= 0.3) && (met/sqrt(ht)>=10)"); var_HistGs.push_back(new HistogramGetter("dphij1met", "dphij1met",    "|#Delta#phi(j_{1}, #slash{E}_{T})|",  32, 0., 3.2)); var_Names.push_back("dphij1met_bl_m_dphij1met");
  variations.push_back(QCDSupport::METPresel + " && " + QCDSupport::BaselineExtraCuts + " && (dphij2met >= 0.3)");                       var_HistGs.push_back(new HistogramGetter("dphij1met", "dphij1met",    "|#Delta#phi(j_{1}, #slash{E}_{T})|",  32, 0., 3.2)); var_Names.push_back("dphij1met_bl_m_dphij1met_m_metoverht");

  vector <TString> names;
  vector <TTree*>  trees;
  vector <int>     colors;
//  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/nonQCD_tree_skimmed_baseline.root"));       names.push_back("Non-QCD bkg");        colors.push_back(color_ttbar);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_tree_skimmed_baseline.root"));          names.push_back("Smeared QCD MC");     colors.push_back(color_qcd);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/ttbar-mg_tree_skimmed_baseline.root"));     names.push_back("t#bar{t}");           colors.push_back(color_ttbar);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/wjets-ht_tree_skimmed_baseline.root"));     names.push_back("W+jets");             colors.push_back(color_wjets);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/znunu_tree_skimmed_baseline.root"));        names.push_back("Z#rightarrow#nu#nu"); colors.push_back(color_znunu);
//  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_origtree_skimmed_baseline.root"));      names.push_back("With orig. QCD MC");  colors.push_back(color_tW);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_375_295_tree_skimmed_baseline.root")); names.push_back("#Delta M = 20 GeV");  colors.push_back(kBlack);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_375_325_tree_skimmed_baseline.root")); names.push_back("#Delta M = 50 GeV");  colors.push_back(kRed);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_375_355_tree_skimmed_baseline.root")); names.push_back("#Delta M = 80 GeV");  colors.push_back(kBlue);

  for(unsigned int iV = 0; iV < variations.size(); ++iV){
    for(unsigned int iP = 0; iP < srinfo.nSR; ++iP){
      float yMin = 0., yMax = 0.;
      float totMCYield = 0;
      TString title = TString::Format("%s_%s", var_Names[iV].Data(), srinfo.srSelNames[iP].Data());
      TString name  = TString::Format("%s_%u", var_Names[iV].Data(), iP);
      Plot* plot = new Plot(name, title, var_HistGs[iV]->plotInfo->xTitle, "Events");
      TString sel_SR = TString::Format("%s && %s", variations[iV].Data(), srinfo.srSel[iP].Data());
      for(unsigned int iT = 0; iT < trees.size(); ++iT){
        if(names[iT] == "With orig. QCD MC"){
          TH1F* hSN = var_HistGs[iV]->getHistogram(trees[iT], sel_SR, QCDSupport::stdMCWeight, TString::Format("tree_%s_%u", var_Names[iV].Data(), iT));
          totMCYield += hSN->Integral(0, hSN->GetNbinsX() + 1);
          plot->addHist(hSN, names[iT], "hist", colors[iT], 0, colors[iT]);
        }
        else if ( (!names[iT].Contains("QCD")) && (!names[iT].Contains("Delta")) ){
          TH1F* hSN = var_HistGs[iV]->getHistogram(trees[iT], sel_SR, QCDSupport::stdMCWeight, TString::Format("tree_%s_%u", var_Names[iV].Data(), iT));
          totMCYield += hSN->Integral(0, hSN->GetNbinsX() + 1);
          plot->addToStack(hSN, names[iT], colors[iT], 1001, 1, 1, 3);
        }
        else if (names[iT].Contains("Delta")){
          TH1F* hSN = var_HistGs[iV]->getHistogram(trees[iT], sel_SR, QCDSupport::stdMCWeight, TString::Format("tree_%s_%u", var_Names[iV].Data(), iT));
          hSN->Scale(totMCYield / hSN->Integral(0, hSN->GetNbinsX() + 1));
          if(yMax < hSN->GetMaximum()) yMax = hSN->GetMaximum();
          plot->addHist(hSN, names[iT], "hist", colors[iT], 0, colors[iT]);
        }
        else if (names[iT].Contains("QCD")){
          TH1F* hSN = var_HistGs[iV]->getHistogram(trees[iT], sel_SR, QCDSupport::stdQCDWeight, TString::Format("tree_%s_%u", var_Names[iV].Data(), iT));
          totMCYield += hSN->Integral(0, hSN->GetNbinsX() + 1);
          plot->addToStack(hSN, names[iT], colors[iT], 1001, 1, 1, 3);
        }
        else {
          cout << "a sample i didn't count on" << endl;
        }
      }

      TCanvas* c = new TCanvas;
      plot->setDrawCMSLumi();
      plot->setUsePoisson();
      if(var_Names[iV].Contains("dphi")){
        plot->setYRange(0.001, 100. * yMax);
      } else {
        plot->setYRange(0.001, 2. * yMax);
      }
      plot->setTitle(" ");
      plot->setLegend(.6, .58, .90, .88);

      if(var_Names[iV].Contains("dphi")) plot->setLogy();
      c->cd();
      plot->draw(c, false, QCDSupport::format);
      c->cd();
      c->Modified();
      TLatex* tl = new TLatex();
      tl->SetTextSize(0.03);
      tl->DrawLatexNDC(0.2, 0.69, srinfo.srSelLabels[iP]);

      QCDSupport::setTitleOffset(c);
      c->Update();

      c->SaveAs(TString::Format("%s/QCD_SR_%s_%u.%s", output_dir.Data(), var_Names[iV].Data(), iP, QCDSupport::format.Data()));
    }
  }
}
#endif

void MakeSRPlots_Study(){
  StyleTools::SetTDRStyle();
  gStyle->SetTitleOffset(1.400, "Y");
  gStyle->SetTitleOffset(0.950, "X");
  gStyle->SetPadTopMargin(0.08);
  go();
}
