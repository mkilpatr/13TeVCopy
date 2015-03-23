#include "AnalysisMethods/PlotUtils/interface/PlotROCCurves.h"

using namespace StyleTools;
using namespace EffPlotTools;

PlotROCCurves::PlotROCCurves()
{

  SetStyle();

}

void PlotROCCurves::addSignalTree(const TString fname, const TString tname, const TString label)
{

  sigfile_ = TFile::Open(fname, "READ");
  assert(sigfile_);
  sigtree_ = (TTree*)sigfile_->Get(tname);
  assert(sigfile_);
  siglabel_ = label;

}

void PlotROCCurves::addBackgroundTree(const TString fname, const TString tname, const TString label)
{

  bkgfile_ = TFile::Open(fname, "READ");
  assert(bkgfile_);
  bkgtree_ = (TTree*)bkgfile_->Get(tname);
  assert(bkgfile_);
  bkglabel_ = label;

}

void PlotROCCurves::addROCVariable(const TString varname, const TString label, const TString sigsel, const TString bkgsel, const unsigned int color, const int nbins, const float xmin, const float xmax, const bool cutlessthan)
{

  assert(sigtree_);
  TString signame = varname + "_sig";
  TH1F* sighist = new TH1F(signame, "", nbins, xmin, xmax);
  TString drawsig = varname + ">>" + signame;
  sigtree_->Draw(drawsig, sigsel);

  assert(bkgtree_);
  TString bkgname = varname + "_bkg";
  TH1F* bkghist = new TH1F(bkgname, "", nbins, xmin, xmax);
  TString drawbkg = varname + ">>" + bkgname;
  bkgtree_->Draw(drawbkg, bkgsel);

  rocplots_.emplace_back(sighist, bkghist, varname, label, color, cutlessthan);

}

void PlotROCCurves::addCompPlot(const TString compplotname, vector<TString> compvarnames, const double xmin, const double xmax, const double ymin, const double ymax, const bool plotbkgrej, const bool plotsigvsbkg)
{

  TString xlabel = plotsigvsbkg ? (plotbkgrej ? TString::Format("1 - %s",bkglabel_.Data()) : bkglabel_) : siglabel_;
  TString ylabel = plotsigvsbkg ? siglabel_ : (plotbkgrej ? TString::Format("1 - %s",bkglabel_.Data()) : bkglabel_);

  Plot* rocplot = new Plot(compplotname,"",xlabel,ylabel);

  for(auto name : compvarnames) {
    for(auto plot : rocplots_) {
      if(plot.varname == name) {
        TGraph* rocgr = computeROCCurve(plot.sighist, plot.bkghist, "", plot.reversecut, plotbkgrej, plotsigvsbkg);
        rocplot->addGraph(rocgr, plot.label, "C", plot.color, 0, plot.color, 1);
        break;
      }
    }
  }

  rocplot->setXRange(xmin, xmax);
  rocplot->setYRange(ymin, ymax);
  if(plotbkgrej) {
    if(plotsigvsbkg) {
      rocplot->moveLegend(-0.35, -0.4);
      rocplot->setLegend(rocplot->getLegend()->GetX1(), rocplot->getLegend()->GetY1(), rocplot->getLegend()->GetX2()+0.2, rocplot->getLegend()->GetY2());
    } else {
      rocplot->setLegend(rocplot->getLegend()->GetX1()-0.2, rocplot->getLegend()->GetY1(), rocplot->getLegend()->GetX2(), rocplot->getLegend()->GetY2());
    }
  } else {
    if(plotsigvsbkg) {
      rocplot->moveLegend(0.0, -0.4);
      rocplot->setLegend(rocplot->getLegend()->GetX1()-0.2, rocplot->getLegend()->GetY1(), rocplot->getLegend()->GetX2(), rocplot->getLegend()->GetY2());
    }
    else {
      rocplot->moveLegend(-0.35, 0.0);
      rocplot->setLegend(rocplot->getLegend()->GetX1(), rocplot->getLegend()->GetY1(), rocplot->getLegend()->GetX2()+0.2, rocplot->getLegend()->GetY2());
    }
  }
  rocplot->setHeader("","");

  compplots_.push_back(rocplot);

}

void PlotROCCurves::plotAll(TString format)
{

  TCanvas* c = MakeCanvas("c","",600,600);

  c->cd();

  for(auto plot : compplots_) {
    plot->draw(c,true,format);
  }

  delete c;

}

void PlotROCCurves::getInfoForSignalEff(const TString varname, const float eff)
{

  for(auto plot : rocplots_) {
    if(plot.varname == varname) {
      float cut = getCutValueForEfficiency(plot.sighist, eff, plot.reversecut)[0];
      float sigeff = 100.0*getCutValueForEfficiency(plot.sighist, eff, plot.reversecut)[1];
      float bkgeff = 100.0*getEfficiencyForCutValue(plot.bkghist, cut, plot.reversecut)[0];
      printf("%s:\n\tA cut of %4.2f corresponds to\n\tsignal efficiency = %5.3f%%\n\tbackground efficiency = %5.3f%% \n", varname.Data(), cut, sigeff, bkgeff);
      return;
    }
  }

  printf("Variable not found in list\n");

}

void PlotROCCurves::getInfoForBackgroundEff(const TString varname, const float eff)
{

  for(auto plot : rocplots_) {
    if(plot.varname == varname) {
      float cut = getCutValueForEfficiency(plot.bkghist, eff, plot.reversecut)[0];
      float bkgeff = 100.0*getCutValueForEfficiency(plot.bkghist, eff, plot.reversecut)[1];
      float sigeff = 100.0*getEfficiencyForCutValue(plot.sighist, cut, plot.reversecut)[0];
      printf("%s:\n\tA cut of %4.2f corresponds to\n\tsignal efficiency = %5.3f%%\n\tbackground efficiency = %5.3f%% \n", varname.Data(), cut, sigeff, bkgeff);
      return;
    }
  }

  printf("Variable not found in list\n");

}

void PlotROCCurves::getInfoForCut(const TString varname, const float cut)
{

  for(auto plot : rocplots_) {
    if(plot.varname == varname) {
      float sigeff = 100.0*getEfficiencyForCutValue(plot.sighist, cut, plot.reversecut)[0];
      float bkgeff = 100.0*getEfficiencyForCutValue(plot.bkghist, cut, plot.reversecut)[0];
      printf("%s:\n\tA cut of %4.2f corresponds to\n\tsignal efficiency = %5.3f%%\n\tbackground efficiency = %5.3f%% \n", varname.Data(), cut, sigeff, bkgeff);
      return;
    }
  }

  printf("Variable not found in list\n");

}

Plot* PlotROCCurves::getCompPlot(const TString compplotname)
{

  for(auto plot : compplots_) {
    if(plot->getName() == compplotname) return plot;
  }

  return 0;

}

TH1F* PlotROCCurves::getSigHist(const TString varname)
{

  for(auto plot : rocplots_) {
    if(plot.varname == varname) return plot.sighist;
  }

  return 0;

}

TH1F* PlotROCCurves::getBkgHist(const TString varname)
{

  for(auto plot : rocplots_) {
    if(plot.varname == varname) return plot.bkghist;
  }

  return 0;

}
