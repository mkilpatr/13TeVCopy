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
        int     nbinsx;
        double  xmin;
        double  xmax;
        double* xbins;
        int     nbinsy;
        double  ymin;
        double  ymax;
        double* ybins;

        PlotTreeVar(TString inname, TString invarname, TString inselection, TString inlabel, int innbinsx, double inxmin, double inxmax, int innbinsy, double inymin, double inymax) :
          name(inname),
          varname(invarname),
          selection(inselection),
          label(inlabel),
          nbinsx(innbinsx),
          xmin(inxmin),
          xmax(inxmax),
          xbins(0),
          nbinsy(innbinsy),
          ymin(inymin),
          ymax(inymax),
          ybins(0)
        {}

        PlotTreeVar(TString inname, TString invarname, TString inselection, TString inlabel, int innbinsx, double* inxbins) :
          name(inname),
          varname(invarname),
          selection(inselection),
          label(inlabel),
          nbinsx(innbinsx),
          xmin(0),
          xmax(0),
          xbins(inxbins),
          nbinsy(0),
          ymin(0),
          ymax(0),
          ybins(0)
        {}

        PlotTreeVar(TString inname, TString invarname, TString inselection, TString inlabel, int innbinsx, double* inxbins, int innbinsy, double* inybins) :
          name(inname),
          varname(invarname),
          selection(inselection),
          label(inlabel),
          nbinsx(innbinsx),
          xmin(0),
          xmax(0),
          xbins(inxbins),
          nbinsy(innbinsy),
          ymin(0),
          ymax(0),
          ybins(inybins)
        {}

    };

    // Hold information about sets of histograms/graphs to be compared in a plot
    struct PlotComp {

      public :
        enum CompPlotType {HISTCOMP, HIST2DCOMP, GRAPHCOMP};
        TString         name;
        vector<TString> compnames;
        vector<TString> complabels;
        double          ymax;
        CompPlotType    compplottype;

        PlotComp(TString inname, vector<TString> incompnames, vector<TString> incomplabels, double inymax, CompPlotType incompplottype=HISTCOMP) :
          name(inname),
          compnames(incompnames),
          complabels(incomplabels),
          ymax(inymax),
          compplottype(incompplottype)
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
        TString                yieldfilename;
        TString                treename;
        TString                wgtvar;
        bool                   dataismc;
        TString                treefilesuffix;
        TString                plotfilesuffix;
        TString                sqrts;
        TString                lumi;
        TString                channel;
        TString                ytitle;
        double                 headerx;
        double                 headery;
        double                 legx1;
        double                 legy1;
        double                 legx2;
        double                 legy2;
        TString                drawopt;
        TString                drawopt2d;
        TString                graphdrawopt;
        int                    rebinx;
        int                    rebiny;
        int                    sigscale;
        bool                   addsigscaletxt;
        bool                   scaletodata;
        double                 maxscale;
        double                 minlogscale;
        double                 maxlogscale;
        bool                   logy;
        double                 tablelumi;
        SelMap                 selmap;
        ColorMap               colormap;
        vector<PlotTreeVar>    treevars;
        vector<PlotComp>       comphistplots;
        vector<PlotComp>       comphist2dplots;
        vector<PlotComp>       compgraphplots;
        vector<TString>        tablesels;
        unsigned int           plotoverflow;
        bool                   make_integral;
        bool                   reverse_integral_dir;
        bool                   plotratio;

        PlotConfig() :
          type(DATAMC),
          source(TREES),
          format("png"),
          dataname(""),
          scalecompto1(false),
          writehists(false),
          outfilename("output_plots.root"),
          yieldfilename("yields.tex"),
          treename("events"),
          wgtvar("weight"),
          dataismc(false),
          treefilesuffix("_tree.root"),
          plotfilesuffix("_plots.root"),
          sqrts("#sqrt{s} = 13 TeV"),
          lumi("4 fb^{-1}"),
          channel(""),
          ytitle(""),
          headerx(0.61),
          headery(0.92),
          legx1(0),
          legy1(0),
          legx2(0),
          legy2(0),
          drawopt("hist"),
          drawopt2d("COLZ"),
          graphdrawopt("P"),
          rebinx(1),
          rebiny(1),
          sigscale(1),
          addsigscaletxt(true),
          scaletodata(false),
          maxscale(1.3),
          minlogscale(0.2),
          maxlogscale(2000.),
          logy(false),
          tablelumi(4000.),
          colormap(DefaultColors()),
          plotoverflow(0),
          make_integral(false),
          reverse_integral_dir(false),
          plotratio(false)
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
            printf("Producing %lu plots from histograms/graphs saved in input file\n",(comphistplots.size()+compgraphplots.size()));
          }
          if(type == DATAMC) {
            if(sigscale < 0) printf("Signal histograms will be scaled to sum of backgrounds\n");
            else if(sigscale != 1) printf("Signal histograms will be scaled by a factor of %d\n",sigscale);
            if(dataismc) printf("'Data' sample is scaled like MC.");
          }
          if(rebinx != 1) printf("Histograms will be rebinned by a factor of in x%d\n",rebinx);
          if(rebiny != 1) printf("Histograms will be rebinned by a factor of in y%d\n",rebiny);
          if(drawopt != "hist") printf("Using histogram draw option %s\n",drawopt.Data());
          if(drawopt2d != "COLZ") printf("Using 2D histogram draw option %s\n",drawopt2d.Data());
          if(graphdrawopt != "P") printf("Using graph draw option %s\n",graphdrawopt.Data());
          if(plotoverflow > 0) printf("Histograms will be plotted with %s\n", plotoverflow == 1 ? "overflow bin contents added to last bin" : "additional bin displaying overflow");
          if(make_integral){
            printf("Histograms will be integrated %s. \n", reverse_integral_dir?"from the left":"to the right");
          }
        }

    };


  public :
    // Constructor to be used with a .conf file specifying a list of samples, labels, etc. Use with TREES or HISTS
    PlotStuff(TString confFile, TString inputDir, TString outputDir, PlotConfig *myConf = 0, bool doVerbose = false);
    // Constructor to be used with a single input file with histograms to be plotted. Use with HISTSSINGLEFILE
    PlotStuff(TFile* inputFile, TString outputDir, PlotConfig *myConf = 0, bool doVerbose = false);

    virtual ~PlotStuff() {}

    // Add a plot to be made from the provided TTrees: use with TREES
    void     addTreeVar(TString plotname, TString varname, TString selection, TString label, int nbinsx, double xmin, double xmax, int nbinsy=0, double ymin=0, double ymax=0);
    // Add a plot to be made from the provided TTrees: use with TREES and variable x binning
    void     addTreeVar(TString plotname, TString varname, TString selection, TString label, int nbinsx, double* xbins);
    // Add a plot to be made from the provided TTrees: use with TREES and variable x/y binning
    void     addTreeVar(TString plotname, TString varname, TString selection, TString label, int nbinsx, double* xbins, int nbinsy, double* ybins);
    // Add a set of names of histograms/graphs to be compared on a single plot: use with HISTSSINGLEFILE. Set compplottype to GRAPHCOMP for graphs
    void     addCompSet(TString compplotname, vector<TString> plots, vector<TString> labels, double ymax=0.0, PlotComp::CompPlotType compplottype=PlotComp::HISTCOMP);
    // Add a selection for which to compute event yields and add to yields table. Use with TREES
    void     addSelection(TString label, TString selection);
    // Get name of outputfile to which histograms are saved if writehists option is chosen in config
    TString  outfileName() { assert(outfile_); return TString(outputdir_+"/"+config_.outfilename); }

    // Plot everything according to the provided configurations
    void     plot();
    // Make yields table
    void     tabulate();

    // Set plot configuration parameters
    // Conf file to be processed with list of samples
    void     setConfFile(TString conf) {config_.conf = conf; }
    // Type of plot (DATAMC/COMP/NORMCOMP)
    void     setPlotType(PlotType type) {config_.type = type; }
    // Source for making plots (HISTS/TREES/HISTSSINGLEFILE)
    void     setPlotSource(PlotSource source) {config_.source = source; }
    // Format to save plots in (e.g., png, pdf)
    void     setFormat(TString format) { config_.format = format; }
    // Name of the data file
    void     setDataName(TString dataname) { config_.dataname = dataname; }
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
    // Set to treat (i.e. scale) the 'data' sample as if it's MC
    void     setDataIsMC(bool dataismc=true) { config_.dataismc = dataismc; }
    // Suffix to be added to sample name to get path to tree files
    void     setTreeFileSuffix(TString treefilesuffix) { config_.treefilesuffix = treefilesuffix; }
    // Suffix to be added to sample name to get path to plot files
    void     setPlotFileSuffix(TString plotfilesuffix) { config_.plotfilesuffix = plotfilesuffix; }
    // Text to be displayed in header (com energy, lumi, channel)
    void     setHeaderText(TString sqrts, TString lumi, TString channel) { config_.sqrts = sqrts; config_.lumi = lumi; config_.channel = channel; }
    // Y-axis title (if different from default)
    void     setYTitle(TString ytitle) { config_.ytitle = ytitle; }
    // x, y coordinates of lower left corner of header box
    void     setHeaderPosition(double headerx, double headery) { config_.headerx = headerx; config_.headery = headery; }
    // Change coordinates of legend box, only if default behavior is not desired
    void     setLegend(double legx1, double legy1, double legx2, double legy2) { config_.legx1 = legx1; config_.legy1 = legy1; config_.legx2 = legx2; config_.legy2 = legy2; }
    // Draw option for histograms in comparison plots (doesn't apply to DATAMC)
    void     setDrawOption(TString drawopt) { config_.drawopt = drawopt; }
    // Draw option for 2D histograms
    void     setDrawOption2D(TString drawopt2d) { config_.drawopt2d = drawopt2d; }
    // Draw option for graphs in comparison plots
    void     setGraphDrawOption(TString graphdrawopt) { config_.graphdrawopt = graphdrawopt; }
    // Rebin factor for histograms
    void     setRebinX(int rebinx) { config_.rebinx = rebinx; }
    // Rebin factor for y-axis for 2D histograms
    void     setRebinY(int rebiny) { config_.rebiny = rebiny; }
    // Factor by which to scale signal. -1 implies signal will be scaled to sum of all backgrounds
    void     setSigScale(int sigscale) { config_.sigscale = sigscale; }
    // Add signal scale factor to legend (default = true)
    void     setAddSigScaleTxt(bool addsigscaletxt) { config_.addsigscaletxt = addsigscaletxt; }
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
    // Set option for plotting overflow bin by default
    void     setPlotOverflow(unsigned int plotoverflow) { config_.plotoverflow = plotoverflow; }
    // Make integral plots. Default integral direction: (value > [cut]).
    void     setIntegral(bool reverse_integral_direction = false) {config_.make_integral = true; config_.reverse_integral_dir = reverse_integral_direction; }
    // Make a ratio plot
    void     setRatioPlot(bool plotratio = true) { config_.plotratio = plotratio; }
    // Scale backgrounds to data (when plotting data vs MC)
    void     setScaleToData(bool scaletodata = true) { config_.scaletodata = scaletodata; }

  // Helper functions
  private :
    // Add a data sample
    void     addData(TString dataname) { config_.dataname = dataname; }
    // Add a background sample
    void     addBackground(TString bkgname) { config_.backgroundnames.push_back(bkgname); }
    // Add a signal sample
    void     addSignal(TString signame) { config_.signalnames.push_back(signame); }
    // Fill the list of histograms to be plotted
    void     loadPlots();
    // Fill yields and uncertainties
    void     loadTables();
    // Plot the given histograms on a canvas
    void     makeHistPlot(TString name, TString title, TString xtitle, TString ytitle, vector<TH1F*> hists);
    // Plot the given 2D histograms separately
    void     makeHist2DPlot(TString name, TString title, TString xtitle, TString ytitle, vector<TH2F*> hists);
    // Plot the given graphs on a canvas
    void     makeGraphPlot(TString name, TString title, TString xtitle, TString ytitle, double ymax, vector<TGraph*> graphs);
    // Make yields table
    void     makeTable(TString label, vector<double> yields, vector<double> yielderrs);
    // Check if a given name corresponds to a data sample
    bool     isData(TString sname) { return config_.dataname == sname; }
    // Check if a given name corresponds to a background sample
    bool     isBackground(TString sname);
    // Check if a given name corresponds to a signal sample
    bool     isSignal(TString sname);

  protected :
    PlotConfig               config_;
    vector<vector<TH1F*> >   hists_;
    vector<vector<TH2F*> >   hists2d_;
    vector<vector<TGraph*> > graphs_;
    vector<vector<double> >  yields_;
    vector<vector<double> >  yielderrs_;
    vector<TString>          histplotnames_;
    vector<TString>          hist2dplotnames_;
    vector<TString>          graphplotnames_;
    vector<TString>          tablelabels_;
    vector<Sample*>          samples_;
    TFile*                   infile_;
    TString                  inputdir_;
    TString                  outputdir_;
    TCanvas*                 canvas_;
    TFile*                   outfile_;
    ofstream                 yieldfile_;
    bool                     verbose_;

};

#endif
