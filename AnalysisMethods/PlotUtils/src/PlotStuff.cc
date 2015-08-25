//--------------------------------------------------------------------------------------------------
// 
// Class to ... er ... plot stuff. Nicely.
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"

//using namespace std;

PlotStuff::PlotStuff(TString confFile, TString inputDir, TString outputDir, PlotConfig *myConf, bool doVerbose) :
  config_(myConf ? *myConf : PlotConfig()),
  infile_(0),
  inputdir_(inputDir),
  outputdir_(outputDir),
  canvas_(0),
  outfile_(0),
  verbose_(doVerbose)
{

  config_.conf = confFile;

  PlotTools::initSamples(config_.conf, samples_);

  for(auto* sample : samples_) {
    if(sample->types.at(0) != 0) addData(sample->name);
    else if(sample->name.Contains("T1") || sample->name.Contains("T2")) addSignal(sample->name);
    else addBackground(sample->name);
  }

}

PlotStuff::PlotStuff(TFile* inputFile, TString outputDir, PlotConfig *myConf, bool doVerbose) :
  config_(myConf ? *myConf : PlotConfig()),
  infile_(inputFile),
  outputdir_(outputDir),
  canvas_(0),
  outfile_(0),
  verbose_(doVerbose)
{}

bool PlotStuff::isBackground(TString sname)
{

  for(auto name : config_.backgroundnames) {
    if(sname == name) return true;
  }

  return false;

}

bool PlotStuff::isSignal(TString sname)
{

  for(auto name : config_.signalnames) {
    if(sname == name) return true;
  }

  return false;

}

void PlotStuff::assignColor(TString sname)
{

  if(sname.Contains("ttbar",TString::kIgnoreCase) && sname.Contains("1l",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["ttbar1l"];
  else if(sname.Contains("ttbar",TString::kIgnoreCase) && sname.Contains("2l",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["ttbar2l"];
  else if(sname.Contains("ttbar",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["ttbar"];
  else if(sname.Contains("wjets",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["wjets"];
  else if(sname.Contains("ttZ",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["ttZ"];
  else if(sname.Contains("ttW",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["ttW"];
  else if(sname.Contains("tW",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["tW"];
  else if(sname.Contains("top",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["tW"];
  else if(sname.Contains("st",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["tW"];
  else if(sname.Contains("znunu",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["znunu"];
  else if(sname.Contains("znn",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["znunu"];
  else if(sname.Contains("dyjets",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["znunu"];
  else if(sname.Contains("zll",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["znunu"];
  else if(sname.Contains("qcd",TString::kIgnoreCase))
    config_.colormap[sname] = config_.colormap["qcd"];
  else 
    config_.colormap[sname] = config_.colormap["other"];

}

void PlotStuff::addTreeVar(TString plotname, TString varname, TString selection, TString label, int nbinsx, double xmin, double xmax, int nbinsy, double ymin, double ymax)
{

  PlotTreeVar treevar(plotname, varname, selection, label, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  config_.treevars.push_back(treevar);
  if(varname.Contains(":"))
    hist2dplotnames_.push_back(plotname);
  else
    histplotnames_.push_back(plotname);

}

void PlotStuff::addTreeVar(TString plotname, TString varname, TString selection, TString label, int nbinsx, double* xbins)
{

  PlotTreeVar treevar(plotname, varname, selection, label, nbinsx, xbins);
  config_.treevars.push_back(treevar);
  if(varname.Contains(":"))
    hist2dplotnames_.push_back(plotname);
  else
    histplotnames_.push_back(plotname);

}

void PlotStuff::addTreeVar(TString plotname, TString varname, TString selection, TString label, int nbinsx, double* xbins, int nbinsy, double* ybins)
{

  PlotTreeVar treevar(plotname, varname, selection, label, nbinsx, xbins, nbinsy, ybins);
  config_.treevars.push_back(treevar);
  if(varname.Contains(":"))
    hist2dplotnames_.push_back(plotname);
  else
    histplotnames_.push_back(plotname);

}

void PlotStuff::addCompSet(TString compplotname, vector<TString> plots, vector<TString> labels, double ymax, PlotComp::CompPlotType compplottype)
{

  assert(plots.size() == labels.size());

  PlotComp comp(compplotname, plots, labels, ymax, compplottype);
  if(compplottype == PlotComp::HISTCOMP) {
    config_.comphistplots.push_back(comp);
    histplotnames_.push_back(compplotname);
  }
  else if(compplottype == PlotComp::HIST2DCOMP) {
    config_.comphist2dplots.push_back(comp);
    hist2dplotnames_.push_back(compplotname);
  }
  else if(compplottype == PlotComp::GRAPHCOMP) {
    config_.compgraphplots.push_back(comp);
    graphplotnames_.push_back(compplotname);
  }
  else {
    printf("CompPlot type not known! Specify either HISTCOMP, HIST2DCOMP or GRAPHCOMP\n");
    return;
  }

}

void PlotStuff::addSelection(TString label, TString selection)
{

  config_.tablesels.push_back(selection);
  tablelabels_.push_back(label);

}

void PlotStuff::loadPlots()
{

  switch (config_.source) {

    case TREES : {

      vector<TH1F*> tmphistsv;
      vector<TH2F*> tmphists2dv;

      TFile *infile = 0;
      TTree *intree = 0;

      bool firsthist = true;
      bool first2dhist = true;

      for(auto* sample : samples_) {
        tmphistsv.clear();
        tmphists2dv.clear();

        TString filename = inputdir_ + "/" + sample->name + config_.treefilesuffix;

        if(sample->filenames.size() > 1 && gSystem->AccessPathName(filename.Data())) {
          TString cmd = TString::Format("hadd -f %s/%s%s %s/%s_[0-9]*%s", inputdir_.Data(), sample->name.Data(), config_.treefilesuffix.Data(), inputdir_.Data(), sample->name.Data(), config_.treefilesuffix.Data());
          gSystem->Exec(cmd.Data());
        }

        infile = TFile::Open(filename);
        assert(infile);

        intree = (TTree*)infile->Get(config_.treename);
        assert(intree);

        for(auto var : config_.treevars) {
          TString histname = var.name + "_" + sample->name;
          TString drawstr = var.varname + ">>" + histname;
          TString cutstr = config_.wgtvar + "*(" + var.selection + ")";
          if( isData(sample->name) && !config_.dataismc )
            cutstr = "1.0*(" + var.selection + ")";
          if(var.varname.Contains(":")) {
            TH2F* hist2d = 0;
            if(var.xmax > 0 && var.ymax > 0)
              hist2d = new TH2F(histname, TString("; " + var.label + "; Events"), var.nbinsx, var.xmin, var.xmax, var.nbinsy, var.ymin, var.ymax);
            else if (var.xbins && var.ybins)
              hist2d = new TH2F(histname, TString("; " + var.label + "; Events"), var.nbinsx, var.xbins, var.nbinsy, var.ybins);
            hist2d->Sumw2();
            intree->Draw(drawstr.Data(), cutstr.Data());
            tmphists2dv.push_back(hist2d);
            tmphists2dv.back()->SetName(TString::Format("%s_%s",var.name.Data(), sample->name.Data()));
          } else {
            TH1F* hist = 0;
            if(var.xmax > 0)
              hist = new TH1F(histname, TString("; " + var.label + "; Events"), var.nbinsx, var.xmin, var.xmax);
            else if(var.xbins)
              hist = new TH1F(histname, TString("; " + var.label + "; Events"), var.nbinsx, var.xbins);
            hist->Sumw2();
            intree->Draw(drawstr.Data(), cutstr.Data());
            tmphistsv.push_back(hist);
            tmphistsv.back()->SetName(TString::Format("%s_%s",var.name.Data(), sample->name.Data()));
          }
        }

        unsigned int nhist = 0;
        for(vector<TH1F*>::iterator ihist = tmphistsv.begin(); ihist != tmphistsv.end(); ++ihist) {
          if(firsthist) {
            vector<TH1F*> tmpvec(ihist, ihist+1);
            hists_.push_back(tmpvec);
          } else {
            assert(hists_.size() > nhist);
            hists_[nhist].push_back(*ihist);
          }
          nhist++;
        }

        unsigned int nhist2d = 0;
        for(vector<TH2F*>::iterator ihist = tmphists2dv.begin(); ihist != tmphists2dv.end(); ++ihist) {
          if(first2dhist) {
            vector<TH2F*> tmpvec(ihist, ihist+1);
            hists2d_.push_back(tmpvec);
          } else {
            assert(hists2d_.size() > nhist2d);
            hists2d_[nhist2d].push_back(*ihist);
          }
          nhist2d++;
        }

        if(firsthist)   firsthist = false;
        if(first2dhist) first2dhist = false;
      }

      break;

    }

    case HISTS : {

      vector<TH1F*>   tmphistsv;
      vector<TH2F*>   tmphists2dv;
      vector<TGraph*> tmpgraphsv;

      TFile *infile = 0;

      bool firsthist   = true;
      bool first2dhist = true;
      bool firstgraph  = true;

      for(auto* sample : samples_) {
        tmphistsv.clear();
        tmphists2dv.clear();
        tmpgraphsv.clear();

        TString filename = inputdir_ + "/" + sample->name + config_.plotfilesuffix;

        if(sample->filenames.size() > 1 && gSystem->AccessPathName(filename.Data())) {
          TString cmd = TString::Format("hadd -f %s/%s%s %s/%s_*%s", inputdir_.Data(), sample->name.Data(), config_.plotfilesuffix.Data(), inputdir_.Data(), sample->name.Data(), config_.plotfilesuffix.Data());
          gSystem->Exec(cmd.Data());
        }

        infile = TFile::Open(filename);
        assert(infile);

        TIter nextkey(gDirectory->GetListOfKeys());
        while(TKey *key = (TKey*)nextkey()) {
          TObject *obj = key->ReadObj();
          if(obj->IsA() == TH1F::Class()) {
            tmphistsv.push_back((TH1F*)obj);
            if(firsthist) histplotnames_.push_back(obj->GetName());
            tmphistsv.back()->SetName(TString::Format("%s_%s",obj->GetName(), sample->name.Data()));
          }
          else if(obj->IsA() == TH2F::Class()) {
            tmphists2dv.push_back((TH2F*)obj);
            if(first2dhist) hist2dplotnames_.push_back(obj->GetName());
            tmphists2dv.back()->SetName(TString::Format("%s_%s",obj->GetName(), sample->name.Data()));
          }
          else if(obj->InheritsFrom(TGraph::Class())) {
            tmpgraphsv.push_back((TGraph*)obj);
            if(firstgraph) graphplotnames_.push_back(obj->GetName());
            tmpgraphsv.back()->SetName(TString::Format("%s_%s",obj->GetName(), sample->name.Data()));
          }
        }

        unsigned int nhist = 0;
        for(vector<TH1F*>::iterator ihist = tmphistsv.begin(); ihist != tmphistsv.end(); ++ihist) {
          if(firsthist) {
            vector<TH1F*> tmpvec(ihist, ihist+1);
            hists_.push_back(tmpvec);
          } else {
            assert(hists_.size() > nhist);
            hists_[nhist].push_back(*ihist);
          }
          nhist++;
        }

        unsigned int nhist2d = 0;
        for(vector<TH2F*>::iterator ihist = tmphists2dv.begin(); ihist != tmphists2dv.end(); ++ihist) {
          if(first2dhist) {
            vector<TH2F*> tmpvec(ihist, ihist+1);
            hists2d_.push_back(tmpvec);
          } else {
            assert(hists2d_.size() > nhist2d);
            hists2d_[nhist2d].push_back(*ihist);
          }
          nhist2d++;
        }

        unsigned int ngraph = 0;
        for(vector<TGraph*>::iterator igraph = tmpgraphsv.begin(); igraph != tmpgraphsv.end(); ++igraph) {
          if(firstgraph) {
            vector<TGraph*> tmpvec(igraph, igraph+1);
            graphs_.push_back(tmpvec);
          } else {
            assert(graphs_.size() > ngraph);
            graphs_[ngraph].push_back(*igraph);
          }
          ngraph++;
        }

        if(firsthist)   firsthist = false;
        if(first2dhist) first2dhist = false;
        if(firstgraph)  firstgraph = false;
      }

      break;

    }

    case HISTSSINGLEFILE : {

      assert(infile_);

      infile_->cd();

      vector<TH1F*>   tmphistsv;
      vector<TH2F*>   tmphists2dv;
      vector<TGraph*> tmpgraphsv;

      for(auto compplot : config_.comphistplots) {
        tmphistsv.clear();

        for(auto name : compplot.compnames) {
          TH1F* hist = (TH1F*)infile_->Get(name);
          assert(hist);
          tmphistsv.push_back(hist);
        }

        hists_.push_back(tmphistsv);
      }

      for(auto compplot : config_.comphist2dplots) {
        tmphists2dv.clear();

        for(auto name : compplot.compnames) {
          TH2F* hist = (TH2F*)infile_->Get(name);
          assert(hist);
          tmphists2dv.push_back(hist);
        }

        hists2d_.push_back(tmphists2dv);
      }

      for(auto compplot : config_.compgraphplots) {
        tmpgraphsv.clear();

        for(auto name : compplot.compnames) {
          TGraph* graph = (TGraph*)infile_->Get(name);
          assert(graph);
          tmpgraphsv.push_back(graph);
        }

        graphs_.push_back(tmpgraphsv);
      }

      break;

    }

    default : {

      printf("Plot source not known! Specify either TREES or HISTS\n");

    }

  }

}

void PlotStuff::loadTables()
{

  switch (config_.source) {

    case TREES : {

      vector<double> tmpyieldsv;
      vector<double> tmpyielderrsv;

      TFile *infile = 0;
      TTree *intree = 0;

      bool first = true;

      for(auto* sample : samples_) {
        tmpyieldsv.clear();
        tmpyielderrsv.clear();

        TString filename = inputdir_ + "/" + sample->name + config_.treefilesuffix;

        if(sample->filenames.size() > 1 && gSystem->AccessPathName(filename.Data())) {
          TString cmd = TString::Format("hadd -f %s/%s%s %s/%s_*%s", inputdir_.Data(), sample->name.Data(), config_.treefilesuffix.Data(), inputdir_.Data(), sample->name.Data(), config_.treefilesuffix.Data());
          gSystem->Exec(cmd.Data());
        }

        infile = TFile::Open(filename);
        assert(infile);

        intree = (TTree*)infile->Get(config_.treename);
        assert(intree);

        for(auto sel : config_.tablesels) {
          TString drawstr = config_.wgtvar + ">>htmp";
          TString cutstr = config_.wgtvar + "*(" + sel + ")";
          if(isData(sample->name))
            cutstr = "1.0*(" + sel + ")";
          intree->Draw(drawstr.Data(), cutstr.Data(), "e");
          TH1F* htmp = (TH1F*)gPad->GetPrimitive("htmp");
          double tmperr = 0.0;
          tmpyieldsv.push_back(htmp->IntegralAndError(0, htmp->GetNbinsX()+1, tmperr));
          tmpyielderrsv.push_back(tmperr);
        }

        unsigned int nsel = 0;
        for(vector<double>::iterator iy = tmpyieldsv.begin(); iy != tmpyieldsv.end(); ++iy) {
          if(first) {
            vector<double> tmpvec(iy, iy+1);
            yields_.push_back(tmpvec);
          } else {
            assert(yields_.size() > nsel);
            yields_[nsel].push_back(*iy);
          }
          nsel++;
        }

        nsel = 0;
        for(vector<double>::iterator ie = tmpyielderrsv.begin(); ie != tmpyielderrsv.end(); ++ie) {
          if(first) {
            vector<double> tmpvec(ie, ie+1);
            yielderrs_.push_back(tmpvec);
          } else {
            assert(yielderrs_.size() > nsel);
            yielderrs_[nsel].push_back(*ie);
          }
          nsel++;
        }

        if(first) first = false;
      }

      break;

    }

    default : {

      printf("Plot source not known! Specify either TREES or HISTS\n");

    }

  }

}

void PlotStuff::makeHistPlot(TString name, TString title, TString xtitle, TString ytitle, vector<TH1F*> hists)
{

  Plot *plot = new Plot(name, title, xtitle, ytitle);
  plot->outputdir = outputdir_;
  gSystem->mkdir(outputdir_, true);

  double max = 0.0;
  int maxbin = 0;
  int nbins = 0;

  switch (config_.type) {

    case DATAMC : {

      double bkgtot = 0.0;
      double data   = 0.0;

      int isam = -1;

      for(auto* sample : samples_) {
        isam++;
        if(!isData(sample->name) && !isSignal(sample->name))
          bkgtot += hists[isam]->Integral(0, hists[isam]->GetNbinsX()+1);
      }

      isam = -1;

      for(auto* sample : samples_) {
        isam++;

        TString sname = sample->name;

        if(config_.rebinx != 1)
          hists[isam]->Rebin(config_.rebinx);

        if(nbins==0)
          nbins = hists[isam]->GetNbinsX();

        if(hists[isam]->GetMaximum() > max) {
          if(isData(sname) || (isBackground(sname) && !config_.scaletodata) || (isSignal(sname) && config_.sigscale == 1)) {
            max = hists[isam]->GetMaximum();
            maxbin = hists[isam]->GetMaximumBin();
          }
        }

        if(isData(sname)) {
          data += hists[isam]->Integral(0, hists[isam]->GetNbinsX()+1);
          plot->addHist(hists[isam], sample->label, "E", 1, 0, 1, 1, config_.plotoverflow);
        } else if(isSignal(sname)) {
          if(config_.sigscale < 0) {
            double sigscale = config_.scaletodata ? data : bkgtot;
            plot->addHistScaled(hists[isam], sigscale, config_.addsigscaletxt ? sample->label + " (scaled to #sum(bkg))" : sample->label, "hist", config_.colormap[sname], 0, config_.colormap[sname], 1, config_.plotoverflow);
            if(sigscale*hists[isam]->GetMaximum()/hists[isam]->Integral(0, nbins+1) > max) {
              max = sigscale*hists[isam]->GetMaximum()/hists[isam]->Integral(0, nbins+1);
              maxbin = hists[isam]->GetMaximumBin();
            }
          } else if (config_.sigscale != 1) {
            plot->addHistScaled(hists[isam], config_.sigscale*hists[isam]->Integral(0, nbins+1), config_.addsigscaletxt ? TString::Format("%dx %s",config_.sigscale, sample->label.Data()) : sample->label, "hist", config_.colormap[sname], 0, config_.colormap[sname], 1, config_.plotoverflow);
            if(config_.sigscale*hists[isam]->GetMaximum() > max) {
              max = config_.sigscale*hists[isam]->GetMaximum();
              maxbin = hists[isam]->GetMaximumBin();
            }
          } else {
            plot->addHist(hists[isam], sample->label, "hist", 0, 0, config_.colormap[sname], 1, config_.plotoverflow);
          }
        } else {
          if(config_.scaletodata)
            hists[isam]->Scale(data/bkgtot);
          if(config_.colormap.find(sname) == config_.colormap.end())
            assignColor(sname);
          plot->addToStack(hists[isam], sample->label, config_.colormap[sname], 1001, 1, 1, 3, config_.plotoverflow);
        }
      }

      if(plot->getStack()->GetMaximum() > max) {
        max = plot->getStack()->GetMaximum();
        maxbin = plot->getStackMaxBin();
      }

      if(config_.sigscale < 0)
        plot->setName(name+"_sigscalesumbkg");
      else if(config_.sigscale != 1)
        plot->setName(name+TString::Format("_sigscale%dx",config_.sigscale));

      if(config_.ytitle != "")
        plot->setYTitle(config_.ytitle);

      break;

    }

    case COMP : {

      int isam = -1;

      for(auto* sample : samples_) {
        isam++;

        TString colorname = TString::Format("comp%d",isam+1);

        if(config_.rebinx != 1)
          hists[isam]->Rebin(config_.rebinx);

        if(nbins==0)
          nbins = hists[isam]->GetNbinsX();

        if(config_.scalecompto1) {
          plot->addHistScaled(hists[isam], 1.0, sample->label, config_.drawopt, config_.colormap[colorname], 0, config_.colormap[colorname], 1, config_.plotoverflow);
          if(hists[isam]->GetMaximum()/hists[isam]->Integral(0,nbins+1) > max) {
            max = hists[isam]->GetMaximum()/hists[isam]->Integral(0,nbins+1);
            maxbin = hists[isam]->GetMaximumBin();
          }
        } else {
          plot->addHist(hists[isam], sample->label,config_.drawopt, config_.colormap[colorname], 0, config_.colormap[colorname], 1, config_.plotoverflow);
          if(hists[isam]->GetMaximum() > max) {
            max = hists[isam]->GetMaximum();
            maxbin = hists[isam]->GetMaximumBin();
          }
        }
        if(config_.make_integral) {
      	  if(hists[isam]->Integral(0,nbins+1) > max) {
      		max = config_.scalecompto1 ? 1.0 : hists[isam]->Integral(0,nbins+1);
            maxbin = config_.reverse_integral_dir ? nbins : 1;
      	  }
        }
      }

      if(config_.make_integral){
    	plot->integrateHists(config_.reverse_integral_dir);
      }

      if(config_.ytitle != "")
        plot->setYTitle(config_.ytitle);
      else {
        if(config_.scalecompto1)
          plot->setYTitle("Normalized (to 1.) Events");
        else
          plot->setYTitle("Events");
      }

      break;

    }

    case NORMCOMP : {

      plot->setName("comp_"+name);

      TH1F* hist0 = hists.at(0);

      if(config_.rebinx != 1)
        hist0->Rebin(config_.rebinx);

      nbins = hist0->GetNbinsX();

      int isam = -1;

      for(auto* sample : samples_) {
        isam++;
        if(isam==0) continue;

        TString colorname = TString::Format("comp%d",isam+1);

        if(config_.rebinx != 1)
          hists[isam]->Rebin(config_.rebinx);

        hists[isam]->Divide(hist0);

        plot->addHist(hists[isam], sample->label, config_.drawopt, config_.colormap[colorname], 0, config_.colormap[colorname], 1, config_.plotoverflow);

        if(hists[isam]->GetMaximum() > max) {
          max = hists[isam]->GetMaximum();
          maxbin = hists[isam]->GetMaximumBin();
        }

        TString newname = TString(hists[isam]->GetName());
        newname += "over";
        newname += samples_[0]->name;
        hists[isam]->SetName(newname);
      }

      if(config_.ytitle != "")
        plot->setYTitle(config_.ytitle);
      else {
        if(samples_.size()==2)
          plot->setYTitle("dN("+samples_[1]->label+")/dN("+samples_[0]->label+")");
        else
          plot->setYTitle("dN/dN("+samples_[0]->label+")");
      }

      break;

    }

    default : {

      printf("Plot type option not available!\n");
      break;

    }

  }

  bool legonleft = maxbin > 0.5*nbins ? true : false;

  if(config_.logy) {
    plot->setName(plot->getName()+"_log");
    plot->setLogy();
    plot->setYRange(config_.minlogscale, config_.maxlogscale*max);
  } else {
    plot->setYRange(0.0, config_.maxscale*max);
  }

  if(config_.sqrts != "" && config_.lumi != "")
    plot->setHeader(config_.sqrts+", "+config_.lumi, config_.channel, config_.headerx, config_.headery);
  else
    plot->setHeader(config_.sqrts+" "+config_.lumi, config_.channel, config_.headerx, config_.headery);

  if(config_.legx1 != 0)
    plot->setLegend(config_.legx1, config_.legy1, config_.legx2, config_.legy2);

  if(legonleft && plot->getLegend()->GetX1() > 0.5)
    plot->moveLegend(0.2-plot->getLegend()->GetX1(), 0.0);

  if(config_.plotratio && config_.type == DATAMC)
    plot->drawRatioStack(canvas_, true, config_.format);
  else
    plot->draw(canvas_, true, config_.format);

  delete plot;

}

void PlotStuff::makeHist2DPlot(TString name, TString title, TString xtitle, TString ytitle, vector<TH2F*> hists)
{

  int isam = -1;
  for(auto* sample : samples_) {
    isam++;

    TString sname = sample->name;

    if(config_.rebinx != 1)
      hists[isam]->RebinX(config_.rebinx);
    if(config_.rebiny != 1)
      hists[isam]->RebinY(config_.rebiny);

    Plot *plot = new Plot(name+"_"+sname, title, xtitle, ytitle);
    plot->outputdir = outputdir_;
    gSystem->mkdir(outputdir_, true);

    if(config_.scalecompto1)
      plot->addHist2DScaled(hists[isam], 1.0, sample->label, config_.drawopt2d);
    else
      plot->addHist2D(hists[isam], sample->label, config_.drawopt2d);

    plot->setLegend(0,0,0,0);

    plot->draw(canvas_, true, config_.format);
    delete plot;

  }

}

void PlotStuff::makeGraphPlot(TString name, TString title, TString xtitle, TString ytitle, double ymax, vector<TGraph*> graphs)
{

  Plot *plot = new Plot(name, title, xtitle, ytitle);
  plot->outputdir = outputdir_;
  gSystem->mkdir(outputdir_, true);

  TGraph* gr0 = graphs.at(0);
  int npoints = gr0->GetN();
  double xmin = gr0->GetX()[0] - 0.5*(gr0->GetX()[1] - gr0->GetX()[0]);
  double xmax = gr0->GetX()[npoints-1] + 0.5*(gr0->GetX()[npoints-1] - gr0->GetX()[npoints-2]);
  plot->setXRange(xmin, xmax);
  if(ymax > 0.0) plot->setYRange(0.0, ymax);

  double max = 0.0;
  int maxpoint = 0;

  switch (config_.type) {

    case COMP : {

      int isam = -1;

      for(auto* sample : samples_) {
        isam++;

        TString colorname = TString::Format("comp%d",isam+1);

        plot->addGraph(graphs[isam], sample->label,config_.graphdrawopt, config_.colormap[colorname], 0, config_.colormap[colorname],1);
        for(int ipoint = 0; ipoint < graphs[isam]->GetN(); ++ipoint) {
          if(graphs[isam]->GetY()[ipoint] > max) {
            maxpoint = ipoint;
            max = graphs[isam]->GetY()[ipoint];
          }
        }
      }

      break;

    }

    default : {

      printf("Plot type option not available!\n");
      break;

    }

  }

  bool legonleft = maxpoint > 0.5*npoints ? true : false;

  if(config_.sqrts != "" && config_.lumi != "")
    plot->setHeader(config_.sqrts+", "+config_.lumi, config_.channel, config_.headerx, config_.headery);
  else
    plot->setHeader(config_.sqrts+" "+config_.lumi, config_.channel, config_.headerx, config_.headery);

  if(config_.legx1 != 0)
    plot->setLegend(config_.legx1, config_.legy1, config_.legx2, config_.legy2);

  if(legonleft && plot->getLegend()->GetX1() > 0.5)
    plot->moveLegend(0.2-plot->getLegend()->GetX1(), 0.0);

  plot->draw(canvas_, true, config_.format);

  delete plot;

}

void PlotStuff::plot()
{

  if(verbose_)
    config_.print();

  if(config_.writehists && !outfile_)
    outfile_ = new TFile(outputdir_+"/"+config_.outfilename,"RECREATE");

  hists_.clear();
  hists2d_.clear();
  graphs_.clear();
  loadPlots();

  SetStyle();

  assert(histplotnames_.size()   == hists_.size());
  assert(hist2dplotnames_.size() == hists2d_.size());
  assert(graphplotnames_.size()  == graphs_.size());

  canvas_ = MakeCanvas("plotc","plotc",600,600);

  for(auto& histvec : hists_) {
    auto ihist = &histvec - &hists_[0];
    TH1F* hist0 = histvec.at(0);
    assert(hist0);

    if(config_.source == HISTSSINGLEFILE) {
      samples_.clear();
      for(auto& compname : config_.comphistplots.at(ihist).compnames) {
        auto iname = &compname - &(config_.comphistplots.at(ihist).compnames[0]);
        samples_.push_back(new Sample());
        samples_.back()->name = compname;
        samples_.back()->label = config_.comphistplots.at(ihist).complabels[iname];
      }
    }

    assert(histvec.size() == samples_.size());

    makeHistPlot(histplotnames_[ihist], hist0->GetTitle(), hist0->GetXaxis()->GetTitle(), hist0->GetYaxis()->GetTitle(), histvec);

    if(config_.writehists) {
      outfile_->cd();
      for(auto* hist : hists_[ihist]) {
        hist->Write();
      }
    }
  
  }

  for(auto& histvec : hists2d_) {
    auto ihist = &histvec - &hists2d_[0];
    TH2F* hist0 = histvec.at(0);
    assert(hist0);

    if(config_.source == HISTSSINGLEFILE) {
      samples_.clear();
      for(auto& compname : config_.comphist2dplots.at(ihist).compnames) {
        auto iname = &compname - &(config_.comphist2dplots.at(ihist).compnames[0]);
        samples_.push_back(new Sample());
        samples_.back()->name = compname;
        samples_.back()->label = config_.comphist2dplots.at(ihist).complabels[iname];
      }
    }

    assert(histvec.size() == samples_.size());

    makeHist2DPlot(hist2dplotnames_[ihist], hist0->GetTitle(), hist0->GetXaxis()->GetTitle(), hist0->GetYaxis()->GetTitle(), histvec);

    if(config_.writehists) {
      outfile_->cd();
      for(auto* hist : hists2d_[ihist]) {
        hist->Write();
      }
    }
  
  }

  for(auto& graphvec : graphs_) {
    auto igraph = &graphvec - &graphs_[0];
    TGraph* graph0 = graphvec.at(0);
    assert(graph0);

    if(config_.source == HISTSSINGLEFILE) {
      samples_.clear();
      for(auto& compname : config_.compgraphplots.at(igraph).compnames) {
        auto iname = &compname - &(config_.compgraphplots.at(igraph).compnames[0]);
        samples_.push_back(new Sample());
        samples_.back()->name = compname;
        samples_.back()->label = config_.compgraphplots.at(igraph).complabels[iname];
      }
    }

    assert(graphvec.size() == samples_.size());

    makeGraphPlot(graphplotnames_[igraph], graph0->GetTitle(), graph0->GetXaxis()->GetTitle(), graph0->GetYaxis()->GetTitle(), config_.compgraphplots.at(igraph).ymax, graphvec);

    if(config_.writehists) {
      outfile_->cd();
      for(auto* graph : graphs_[igraph]) {
        graph->Write();
      }
    }
  
  }

  if(outfile_) {
    outfile_->Close();
    delete outfile_;
  }

  if(infile_) {
    infile_->Close();
    delete infile_;
  }

  delete canvas_;

}

void PlotStuff::makeTable(TString label, vector<double> yields, vector<double> yielderrs)
{


  double bkgtot = 0.0;
  double bkgerrtotsq = 0.0;

  yieldfile_ << "\\multicolumn{" << samples_.size()+2 << "}{c}{" << label << "} \\\\" << endl;
  yieldfile_ << "\\hline" << endl;
  yieldfile_ << "Events ";

  int isam = -1;
  unsigned int nbkg = 0;
  bool lastbkg = false;
  for(auto* sample : samples_) {
    isam++;
    yieldfile_ << " & " << fixed << setprecision(2) << yields[isam] << " $\\pm$ " << setprecision(2) << yielderrs[isam];
    if(isBackground(sample->name)) {
      nbkg++;
      bkgtot+=yields[isam];
      bkgerrtotsq+=yielderrs[isam]*yielderrs[isam];
    }
    if(nbkg == config_.backgroundnames.size()) {
      if(!lastbkg) yieldfile_ << " & " << fixed << setprecision(2) << bkgtot << " $\\pm$ " << setprecision(2) << sqrt(bkgerrtotsq);
      lastbkg = true;
    }
  }

  yieldfile_ << "\t \\\\" << endl;
  yieldfile_ << "\\hline" << endl;
  yieldfile_ << "Efficiency ";

  isam = -1;
  nbkg = 0;
  lastbkg = false;
  for(auto* sample : samples_) {
    isam++;
    yieldfile_ << " & " << fixed << setprecision(6) << yields[isam]/(sample->xsecs.at(0)*config_.tablelumi);
    if(isBackground(sample->name)) {
      nbkg++;
    }
    if(nbkg == config_.backgroundnames.size()) {
      if(!lastbkg) yieldfile_ << " & ";
      lastbkg = true;
    }
  }

  yieldfile_ << "\t \\\\" << endl;
  yieldfile_ << "\\hline" << endl;

}

void PlotStuff::tabulate()
{

  if(verbose_)
    config_.print();

  yieldfile_.open(config_.yieldfilename);

  yields_.clear();
  yielderrs_.clear();
  loadTables();

  assert(tablelabels_.size() == yields_.size());
  assert(tablelabels_.size() == yielderrs_.size());

  yieldfile_ << "\\begin{tabular}{";
  for(unsigned int isam = 0; isam < samples_.size()+2; isam++) {
    yieldfile_ << "|c";
  }
  yieldfile_ << "|}" << endl;

  yieldfile_ << "\\hline" << endl;
  yieldfile_ << "Process";
  unsigned int nbkg = 0;
  bool lastbkg = false;
  for(auto* sample : samples_) {
    yieldfile_ << "\t & " << sample->name;
    if(isBackground(sample->name)) nbkg++;
    if(nbkg == config_.backgroundnames.size()) {
      if(!lastbkg) yieldfile_ << "\t & Total Bkg";
      lastbkg = true;
    }
  }

  yieldfile_ << "\t \\\\" << endl;
  yieldfile_ << "\\hline" << endl;

  for(auto& yieldvec : yields_) {
    auto isel = &yieldvec - &yields_[0];

    assert(yieldvec.size() == samples_.size());

    makeTable(tablelabels_[isel], yieldvec, yielderrs_[isel]);

  }
  yieldfile_ << "\\end{tabular}" << endl;

}
