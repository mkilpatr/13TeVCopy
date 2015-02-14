//--------------------------------------------------------------------------------------------------
// 
// Class to ... er ... plot stuff. Nicely.
// 
//--------------------------------------------------------------------------------------------------

#ifndef PLOTSTUFF_H
#define PLOTSTUFF_H

#include "TSystem.h"
#include "TString.h"
#include "TFile.h"
#include "TKey.h"
#include "TRegexp.h"
#include "TTree.h"
#include "map"
#include "vector"
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/Sample.hh"
#include "AnalysisMethods/PlotUtils/interface/StyleTools.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"

typedef std::map<TString, TString> SelMap;

using namespace StyleTools;

class PlotStuff {

  public :
    // Types of available plotting options.
    // DATAMC : Standard data-MC plot with backgrounds stacked and signals overlaid
    // COMP : 1-1 comparison of distributions for different samples
    // NORMCOMP : comparison of distributions for different samples, normalized to the first sample
    enum PlotType   {DATAMC, COMP, NORMCOMP};

    // Types of sources for producing plots.
    // HISTS : Plot from a set of "plots.root" files, one per sample, with histograms. All TH1F histograms are plotted
    // TREES : Plot from a set of "tree.root" files, one per sample, with a TTree. Variables to be plotted and selections to be used should be added with addTreeVar
    // HISTSSINGLEFILE : Plot histograms from a file (passed in the specialized constructor). Sets of histograms to be plotted together should be added with addCompSet
    enum PlotSource {HISTS, TREES, HISTSSINGLEFILE};

    // Hold information about tree variables and selections to be used for plotting
    struct PlotTreeVar {

      public :
        TString name;
        TString varname;
        TString selection;
        TString label;
        int     nbins;
        double  xmin;
        double  xmax;

        PlotTreeVar(TString inname, TString invarname, TString inselection, TString inlabel, int innbins, double inxmin, double inxmax) :
          name(inname),
          varname(invarname),
          selection(inselection),
          label(inlabel),
          nbins(innbins),
          xmin(inxmin),
          xmax(inxmax)
        {}

    };

    // Hold information about sets of histograms to be compared in a plot
    struct PlotComp {

      public :
        TString         name;
        vector<TString> compnames;
        vector<TString> complabels;

        PlotComp(TString inname, vector<TString> incompnames, vector<TString> incomplabels) :
          name(inname),
          compnames(incompnames),
          complabels(incomplabels)
        {}

    };

    // Configuration parameters
    struct PlotConfig {

      public :
        TString                conf;
        PlotType               type;
        PlotSource             source;
        TString                format;
        TString                dataname;
        vector<TString>        backgroundnames;
        vector<TString>        signalnames;
        bool                   scalecompto1;
        bool                   writehists;
        TString                outfilename;
        TString                treename;
        TString                wgtvar;
	TString                treefilesuffix;
        TString                plotfilesuffix;
        TString                sqrts;
        TString                lumi;
        TString                channel;
        double                 headerx;
        double                 headery;
        double                 legx1;
        double                 legy1;
        double                 legx2;
        double                 legy2;
        TString                drawopt;
        int                    rebinx;
        int                    sigscale;
        double                 maxscale;
        double                 minlogscale;
        double                 maxlogscale;
        bool                   logy;
        SelMap                 selmap;
        ColorMap               colormap;
        vector<PlotTreeVar>    treevars;
        vector<PlotComp>       compplots;

        PlotConfig() :
          type(DATAMC),
          source(TREES),
          format("png"),
          dataname(""),
          scalecompto1(false),
          writehists(false),
          outfilename("output_plots.root"),
          treename("events"),
          wgtvar("weight"),
          treefilesuffix("_tree.root"),
          plotfilesuffix("_plots.root"),
          sqrts("#sqrt{s} = 13 TeV"),
          lumi("4 fb^{-1}"),
          channel(""),
          headerx(0.61),
          headery(0.92),
          legx1(0),
          legy1(0),
          legx2(0),
          legy2(0),
          drawopt("hist"),
          rebinx(1),
          sigscale(1),
          maxscale(1.3),
          minlogscale(0.2),
          maxlogscale(2000.),
          logy(false),
          colormap(DefaultColors())
        {}

        void print() {
          printf("Plotting configuration:\n");
          printf("Plot type is %s\n",(type==DATAMC ? "data-MC" : (type==NORMCOMP ? "normalized comparison" : (type==COMP ? "comparison" : "Unkown!"))));
          printf("Plotting from %s\n",(source==HISTS ? "histograms, one file provided per sample" : (source==TREES ? "trees, one ntuple provided per sample" : (source==HISTSSINGLEFILE ? "saved histograms" : "Unkown!"))));
          printf("Will save plots in %s format\n",format.Data());
          if(logy) printf("Log scale enabled for y axis\n");
          if(scalecompto1) printf("Histograms will be normalized to 1 for comparisons\n");
          if(writehists) printf("Histograms will be written to %s\n",outfilename.Data());
          if(source == TREES) {
            printf("Using sample settings from %s\n",conf.Data());
            printf("Producing %lu plots using trees named %s from files ending with %s in input directory\n",treevars.size(),treename.Data(),treefilesuffix.Data());
            printf("Will apply weight variable: %s\n",wgtvar.Data());
          }
          if(source == HISTS) {
            printf("Using sample settings from %s\n",conf.Data());
            printf("Plotting all histograms from files ending with %s in input directory\n",plotfilesuffix.Data());
          }
          if(source == HISTSSINGLEFILE) {
            printf("Producing %lu plots from histograms saved in input file\n",compplots.size());
          }
          if(type == DATAMC) {
            if(sigscale < 0) printf("Signal histograms will be scaled to sum of backgrounds\n");
            else if(sigscale != 1) printf("Signal histograms will be scaled by a factor of %d\n",sigscale);
          }
          if(rebinx != 1) printf("Histograms will be rebinned by a factor of %d\n",rebinx);
          if(drawopt != "hist") printf("Using draw option %s\n",drawopt.Data());
        }

    };


  public :
    // Constructor to be used with a .conf file specifying a list of samples, labels, etc. Use with TREES or HISTS
    PlotStuff(TString confFile, TString inputDir, TString outputDir, PlotConfig *myConf = 0, bool doVerbose = false);
    // Constructor to be used with a single input file with histograms to be plotted. Use with HISTSSINGLEFILE
    PlotStuff(TFile* inputFile, TString outputDir, PlotConfig *myConf = 0, bool doVerbose = false);

    virtual ~PlotStuff() {}

    // Add a plot to be made from the provided TTrees: use with TREES
    void     addTreeVar(TString plotname, TString varname, TString selection, TString label, int nbins, double xmin, double xmax);
    // Add a set of names of histograms to be compared on a single plot: use with HISTSSINGLEFILE
    void     addCompSet(TString compplotname, vector<TString> plots, vector<TString> labels);
    // Get name of outputfile to which histograms are saved if writehists option is chosen in config
    TString  outfileName() { assert(outfile_); return TString(outputdir_+"/"+config_.outfilename); }

    // Plot everything according to the provided configurations
    void     plot();

    // Set plot configuration parameters
    // Conf file to be processed with list of samples
    void     setConfFile(TString conf) {config_.conf = conf; }
    // Type of plot (DATAMC/COMP/NORMCOMP)
    void     setPlotType(PlotType type) {config_.type = type; }
    // Source for making plots (HISTS/TREES/HISTSSINGLEFILE)
    void     setPlotSource(PlotSource source) {config_.source = source; }
    // Format to save plots in (e.g., png, pdf)
    void     setFormat(TString format) { config_.format = format; }
    // Whether or not to scale histograms to 1 for comparisons
    void     setUnitScale() { config_.scalecompto1 = true; }
    // Whether or not to write histograms to output file
    void     setWriteHists() { config_.writehists = true; }
    // Name of output file for writing histograms
    void     setOutfile(TString outfilename) { config_.outfilename = outfilename; }
    // Name of TTree to be used with TREES option
    void     setTree(TString treename) { config_.treename = treename; }
    // Name of variable corresponding to weight to be applied for TTree plots
    void     setWgtVar(TString wgtvar) { config_.wgtvar = wgtvar; }
    // Suffix to be added to sample name to get path to tree files
    void     setTreeFileSuffix(TString treefilesuffix) { config_.treefilesuffix = treefilesuffix; }
    // Suffix to be added to sample name to get path to plot files
    void     setPlotFileSuffix(TString plotfilesuffix) { config_.plotfilesuffix = plotfilesuffix; }
    // Text to be displayed in header (com energy, lumi, channel)
    void     setHeaderText(TString sqrts, TString lumi, TString channel) { config_.sqrts = sqrts; config_.lumi = lumi; config_.channel = channel; }
    // x, y coordinates of lower left corner of header box
    void     setHeaderPosition(double headerx, double headery) { config_.headerx = headerx; config_.headery = headery; }
    // Change coordinates of legend box, only if default behavior is not desired
    void     setLegend(double legx1, double legy1, double legx2, double legy2) { config_.legx1 = legx1; config_.legy1 = legy1; config_.legx2 = legx2; config_.legy2 = legy2; }
    // Draw option for histograms in comparison plots (doesn't apply to DATAMC)
    void     setDrawOption(TString drawopt) { config_.drawopt = drawopt; }
    // Rebin factor for histograms
    void     setRebinX(int rebinx) { config_.rebinx = rebinx; }
    // Factor by which to scale signal. -1 implies signal will be scaled to sum of all backgrounds
    void     setSigScale(int sigscale) { config_.sigscale = sigscale; }
    // Proportion of y axis maximum to maximum bin content of histograms for linear plots
    void     setMaxScale(double maxscale) { config_.maxscale = maxscale; }
    // Absolute y axis minimum for log plots
    void     setMinLogScale(double minlogscale) { config_.minlogscale = minlogscale; }
    // Proportion of y axis maximum to maximum bin content of histograms for log plots
    void     setMaxLogScale(double maxlogscale) { config_.maxlogscale = maxlogscale; }
    // Make log plots
    void     setLogy() { config_.logy = true; }
    // Customize color for a particular sample
    void     setColor(TString sname, unsigned int color) { config_.colormap[sname] = color; }

  // Helper functions
  private :
    // Add a data sample
    void     addData(TString dataname) { config_.dataname = dataname; }
    // Add a background sample
    void     addBackground(TString bkgname) { config_.backgroundnames.push_back(bkgname); }
    void     addSignal(TString signame) { config_.signalnames.push_back(signame); }
    // Fill the list of histograms to be plotted
    void     loadPlots();
    // Plot the given histograms on a canvas
    void     makePlot(TString name, TString title, TString xtitle, TString ytitle, vector<TH1F*> hists);
    // Check if a given name corresponds to a data sample
    bool     isData(TString sname) { return config_.dataname == sname; }
    // Check if a given name corresponds to a background sample
    bool     isBackground(TString sname);
    // Check if a given name corresponds to a signal sample
    bool     isSignal(TString sname);

  protected :
    PlotConfig             config_;
    vector<vector<TH1F*> > hists_;
    vector<TString>        plotnames_;
    vector<Sample*>        samples_;
    TFile*                 infile_;
    TString                inputdir_;
    TString                outputdir_;
    TCanvas*               canvas_;
    TFile*                 outfile_;
    bool                   verbose_;

};

#endif
