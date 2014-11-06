#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TSystem.h"
#include "TKey.h"
#include "TRegexp.h"
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/Sample.hh"
#include "AnalysisMethods/PlotUtils/interface/StyleTools.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#endif

// Helper function to produce and format the plot. Assumes only MC to be plotted for now, no data
void makePlot(TString outdirName, TString format, TString name, TString title, TString xtitle, TString ytitle, vector<Sample*> samples, vector<TH1F*> histv, bool log = false) {

  TCanvas *c = StyleTools::MakeCanvas("c","c",700,700);

  Plot *plot = new Plot(name,title,xtitle,ytitle);
  plot->outputdir = outdirName;
  gSystem->mkdir(outdirName,true);

  for(unsigned int isam=0; isam<samples.size(); isam++) {
    // scale signal to be visible on plot ... assumes signal is the last thing you plot
    if(samples[isam]->name.Contains("T2tt") || samples[isam]->name.Contains("T1tttt")) {
      double scale = 0.3*plot->getStack()->GetMaximum()/histv[isam]->GetMaximum();          // 0.3 just a random choice
      histv[isam]->Scale(scale);
      plot->add(histv[isam],samples[isam]->label+" (arbitrary scale)","hist",0,0,samples[isam]->color,11);
    } else {
      // add background samples to the histogram stack
      plot->addToStack(histv[isam],samples[isam]->label,samples[isam]->color,1001,1,1,3);
    }
  }

  if(log) {
    plot->setName(name+"_log");
    plot->setLogy();
    plot->setYRange(0.2, 20000.*plot->getStack()->GetMaximum());
  } else {
    plot->setYRange(0.0, 1.3*plot->getStack()->GetMaximum());
  }

  plot->setLegend(0.5,0.65,0.95,0.9);

  plot->draw(c,true,format);

  delete c;
  delete plot;

}

// Parses a configuration file with samples and settings for plotting (colors, labels), and processes the corresponding files with histograms to make pretty plots
void plotAll(const TString conf = "run1lep.conf", const TString inputdir = "run/plots", const TString outputdir = "run/plots")
{

  vector<Sample*> samples;    // data/MC samples

  // load sample names, cross sections, files, etc.
  PlotTools::initSamples(conf, samples);

  TFile *infile = 0;

  // vector to hold all the histograms
  vector<vector<TH1F*> > histsv;

  vector<TH1F*> tmphistsv;

  for(unsigned int isam=0; isam < samples.size(); isam++) {
    tmphistsv.clear();
    // if more than one file was processed for the sample, combine the outputs
    if(samples[isam]->filenames.size() > 1) {
      TString cmd = TString::Format("hadd -f %s/%s_plots.root %s/%s_*_plots.root", inputdir.Data(), samples[isam]->name.Data(), inputdir.Data(), samples[isam]->name.Data());
      gSystem->Exec(cmd.Data());
    }
    TString filename = inputdir + "/" + samples[isam]->name + "_plots.root";
    infile = TFile::Open(filename);
    assert(infile);

    // loop over the contents of the plot file and add them to the list to be plotted
    TIter nextkey(gDirectory->GetListOfKeys());
    while(TKey *key = (TKey*)nextkey()) {
      TObject *obj = key->ReadObj();
      if(obj->IsA() == TH1F::Class()) {
	tmphistsv.push_back((TH1F*)obj);
	if(isam > 0) tmphistsv.back()->SetName(TString::Format("%s_%d",obj->GetName(),isam));
      }
    }
    histsv.push_back(tmphistsv);
  }

  // switch rows/columns -> columns/rows ... surely there's an easier way :P
  vector<vector<TH1F*> > rearrangedhistsv;
  unsigned int nhists = histsv[0].size();

  for(unsigned int ihist = 0; ihist < nhists; ihist++) {
    tmphistsv.clear();
    for(unsigned int isam=0; isam < samples.size(); isam++) {
      tmphistsv.push_back(histsv[isam].at(ihist));
    }
    rearrangedhistsv.push_back(tmphistsv);
  }

  StyleTools::SetStyle();

  for(unsigned int ihist=0; ihist < rearrangedhistsv.size(); ihist++) {
    TH1F* hist0 = rearrangedhistsv[ihist].at(0);
    assert(hist0);

    assert(rearrangedhistsv[ihist].size() == samples.size());

    // and ... plot!
    makePlot(outputdir,
             "png",
             hist0->GetName(),
             hist0->GetTitle(),
             hist0->GetXaxis()->GetTitle(),
             hist0->GetYaxis()->GetTitle(),
             samples,
             rearrangedhistsv[ihist]);

    // log plot
    makePlot(outputdir,
             "png",
             hist0->GetName(),
             hist0->GetTitle(),
             hist0->GetXaxis()->GetTitle(),
             hist0->GetYaxis()->GetTitle(),
             samples,
             rearrangedhistsv[ihist],
             true);

  }

}

