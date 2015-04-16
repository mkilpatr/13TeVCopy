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

void PlotStuff::addTreeVar(TString plotname, TString varname, TString selection, TString label, int nbins, double xmin, double xmax)
{

  PlotTreeVar treevar(plotname, varname, selection, label, nbins, xmin, xmax);
  config_.treevars.push_back(treevar);
  plotnames_.push_back(plotname);

}

void PlotStuff::addCompSet(TString compplotname, vector<TString> plots, vector<TString> labels)
{

  assert(plots.size() == labels.size());

  PlotComp comp(compplotname, plots, labels);
  config_.compplots.push_back(comp);
  plotnames_.push_back(compplotname);

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

      TFile *infile = 0;
      TTree *intree = 0;

      bool first = true;

      for(auto* sample : samples_) {
         tmphistsv.clear();

         TString filename = inputdir_ + "/" + sample->name + config_.treefilesuffix;
         infile = TFile::Open(filename);
         assert(infile);

         intree = (TTree*)infile->Get(config_.treename);
         assert(intree);

         for(auto var : config_.treevars) {
          TString histname = var.name + "_" + sample->name;
          TH1F* hist = new TH1F(histname, TString("; " + var.label + "; Events"), var.nbins, var.xmin, var.xmax);
          hist->Sumw2();
          TString drawstr = var.varname + ">>" + histname;
          TString cutstr = config_.wgtvar + "*(" + var.selection + ")";
          intree->Draw(drawstr.Data(), cutstr.Data());
          tmphistsv.push_back(hist);
          tmphistsv.back()->SetName(TString::Format("%s_%s",var.name.Data(), sample->name.Data()));
        }

        unsigned int nhist = 0;
        for(vector<TH1F*>::iterator ihist = tmphistsv.begin(); ihist != tmphistsv.end(); ++ihist) {
          if(first) {
            vector<TH1F*> tmpvec(ihist, ihist+1);
            hists_.push_back(tmpvec);
          } else {
            assert(hists_.size() > nhist);
            hists_[nhist].push_back(*ihist);
          }
          nhist++;
        }

        if(first) first = false;
      }

      break;

    }

    case HISTS : {

      vector<TH1F*> tmphistsv;

      TFile *infile = 0;

      bool first = true;

      for(auto* sample : samples_) {
        tmphistsv.clear();

        if(sample->filenames.size() > 1) {
          TString cmd = TString::Format("hadd -f %s/%s%s %s/%s_*%s", inputdir_.Data(), sample->name.Data(), config_.plotfilesuffix.Data(), inputdir_.Data(), sample->name.Data(), config_.plotfilesuffix.Data());
          gSystem->Exec(cmd.Data());
        }

        TString filename = inputdir_ + "/" + sample->name + config_.plotfilesuffix;
        infile = TFile::Open(filename);
        assert(infile);

        TIter nextkey(gDirectory->GetListOfKeys());
        while(TKey *key = (TKey*)nextkey()) {
          TObject *obj = key->ReadObj();
          if(obj->IsA() == TH1F::Class()) {
            tmphistsv.push_back((TH1F*)obj);
            if(first) plotnames_.push_back(obj->GetName());
            tmphistsv.back()->SetName(TString::Format("%s_%s",obj->GetName(), sample->name.Data()));
          }
        }

        unsigned int nhist = 0;
        for(vector<TH1F*>::iterator ihist = tmphistsv.begin(); ihist != tmphistsv.end(); ++ihist) {
          if(first) {
            vector<TH1F*> tmpvec(ihist, ihist+1);
            hists_.push_back(tmpvec);
          } else {
            assert(hists_.size() > nhist);
            hists_[nhist].push_back(*ihist);
          }
          nhist++;
        }
     
        if(first) first = false;
      }

      break;

    }

    case HISTSSINGLEFILE : {

      assert(infile_);

      infile_->cd();

      vector<TH1F*> tmphistsv;

      for(auto compplot : config_.compplots) {
        tmphistsv.clear();

        for(auto name : compplot.compnames) {
          TH1F* hist = (TH1F*)infile_->Get(name);
          assert(hist);
          tmphistsv.push_back(hist);
        }

        hists_.push_back(tmphistsv);
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
         infile = TFile::Open(filename);
         assert(infile);

         intree = (TTree*)infile->Get(config_.treename);
         assert(intree);

         for(auto sel : config_.tablesels) {
          TString drawstr = config_.wgtvar + ">>htmp";
          TString cutstr = config_.wgtvar + "*(" + sel + ")";
          intree->Draw(drawstr.Data(), cutstr.Data());
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

void PlotStuff::makePlot(TString name, TString title, TString xtitle, TString ytitle, vector<TH1F*> hists)
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

      int isam = -1;

      for(auto* sample : samples_) {
        isam++;

        TString sname = sample->name;

        if(config_.rebinx != 1)
          hists[isam]->Rebin(config_.rebinx);

        if(nbins==0)
          nbins = hists[isam]->GetNbinsX();

        if(hists[isam]->GetMaximum() > max) {
          if(isData(sname) || isBackground(sname) || (isSignal(sname) && config_.sigscale == 1)) {
            max = hists[isam]->GetMaximum();
            maxbin = hists[isam]->GetMaximumBin();
          }
        }

        if(isData(sname)) {
          plot->addHist(hists[isam], sample->label, "E");
        } else if(isSignal(sname)) {
          if(config_.sigscale < 0) {
            plot->addHistScaled(hists[isam], bkgtot, config_.addsigscaletxt ? sample->label + " (scaled to #sum(bkg))" : sample->label, "hist", config_.colormap[sname], 0, config_.colormap[sname]);
            if(bkgtot*hists[isam]->GetMaximum()/hists[isam]->Integral(0, nbins+1) > max) {
              max = bkgtot*hists[isam]->GetMaximum()/hists[isam]->Integral(0, nbins+1);
              maxbin = hists[isam]->GetMaximumBin();
            }
          } else if (config_.sigscale != 1) {
            plot->addHistScaled(hists[isam], config_.sigscale*hists[isam]->Integral(0, nbins+1), config_.addsigscaletxt ? TString::Format("%dx %s",config_.sigscale, sample->label.Data()) : sample->label, "hist", config_.colormap[sname], 0, config_.colormap[sname]);
            if(config_.sigscale*hists[isam]->GetMaximum() > max) {
              max = config_.sigscale*hists[isam]->GetMaximum();
              maxbin = hists[isam]->GetMaximumBin();
            }
          } else {
            plot->addHist(hists[isam], sample->label, "hist", 0, 0, config_.colormap[sname]);
          }
        } else {
          bkgtot += hists[isam]->Integral(0, hists[isam]->GetNbinsX()+1);
          plot->addToStack(hists[isam], sample->label, config_.colormap[sname], 1001, 1, 1, 3);
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
          plot->addHistScaled(hists[isam], 1.0, sample->label, config_.drawopt, config_.colormap[colorname], 0, config_.colormap[colorname],1);
          if(hists[isam]->GetMaximum()/hists[isam]->Integral(0,nbins+1) > max) {
            max = hists[isam]->GetMaximum()/hists[isam]->Integral(0,nbins+1);
            maxbin = hists[isam]->GetMaximumBin();
          }
        } else {
          plot->addHist(hists[isam], sample->label,config_.drawopt, config_.colormap[colorname], 0, config_.colormap[colorname],1);
          if(hists[isam]->GetMaximum() > max) {
            max = hists[isam]->GetMaximum();
            maxbin = hists[isam]->GetMaximumBin();
          }
        }
        if(config_.make_integral) {
      	  if(hists[isam]->Integral(0,nbins+1) > max) {
      		max = hists[isam]->Integral(0,nbins+1);
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

        plot->addHist(hists[isam], sample->label, config_.drawopt, config_.colormap[colorname], 0, config_.colormap[colorname],1);

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
  loadPlots();

  SetStyle();

  assert(plotnames_.size() == hists_.size());

  canvas_ = MakeCanvas("plotc","plotc",600,600);

  for(auto& histvec : hists_) {
    auto ihist = &histvec - &hists_[0];
    TH1F* hist0 = histvec.at(0);
    assert(hist0);

    if(config_.source == HISTSSINGLEFILE) {
      samples_.clear();
      for(auto& compname : config_.compplots.at(ihist).compnames) {
        auto iname = &compname - &(config_.compplots.at(ihist).compnames[0]);
        samples_.push_back(new Sample());
        samples_.back()->name = compname;
        samples_.back()->label = config_.compplots.at(ihist).complabels[iname];
      }
    }

    assert(histvec.size() == samples_.size());

    makePlot(plotnames_[ihist], hist0->GetTitle(), hist0->GetXaxis()->GetTitle(), hist0->GetYaxis()->GetTitle(), histvec);

    if(config_.writehists) {
      outfile_->cd();
      for(auto* hist : hists_[ihist]) {
        hist->Write();
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
