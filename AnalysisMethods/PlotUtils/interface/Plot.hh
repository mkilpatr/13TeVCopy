//--------------------------------------------------------------------------------------------------
//
//   Class to manage a plot. Member methods are provided to facilitate properties of the plot
//   (e.g. styles, colors, combining histograms or graphs, etc.)
//
//--------------------------------------------------------------------------------------------------

#ifndef PLOT_HH
#define PLOT_HH

#include <TROOT.h>
#include <TSystem.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TMath.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TPaveStats.h>
#include <TPaveText.h>
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TProfile.h>
#include <TLine.h>
#include <THStack.h>
#include <TList.h>
#include <TBox.h>
#include <TLatex.h>
#include <TRegexp.h>
#include <TRandom3.h>
#include <Math/QuantFuncMathCore.h>
#include <vector>
#include <assert.h>
#include "AnalysisMethods/PlotUtils/interface/StyleTools.hh"

using namespace std;
using namespace StyleTools;

template <class T>
class PlotElement
{

  public :
    PlotElement(T* inmember, TString inopt) : member(inmember), opt(inopt) {}

    ~PlotElement()		{ delete member;		}

    const char* classname()	{ return member->ClassName();	}

    T* member;

    TString opt;

};

typedef PlotElement<TH1F> h1D;
typedef PlotElement<TH2F> h2D;
typedef PlotElement<TGraph> graph;
typedef PlotElement<TProfile> profile;

class Plot {
  public:
    Plot();
    Plot(TString name, TString title, TString xtitle, TString ytitle);
    ~Plot();

    void clear();

    static TString outputdir;  // output directory
  
    // Draw ratio of 2 histograms
    void drawRatio(TCanvas *c, TH1F *h1, TH1F *h2, bool doSave=false, TString format="png");

    // Draw ratio of data/MC, with MC constituents stacked
    void drawRatioStack(TCanvas *c, bool doSave=false, TString format="png");
    // Draw ratio of data/MC, with MC constituents stacked
    void drawRatioStack(TCanvas *c, TH1F *hData, TH1F *hMC, bool doSave=false, TString format="png");

    // Draw 1D histograms and ratios wrt base histogram
    void drawRatios(TCanvas *c, unsigned int baseIndex=0, bool doSave=false, TString format="png");

    // Draw the plot to a given canvas
    void draw(TCanvas *c, bool doSave=false, TString format="png");
  
    // Adding a histogram/graph to the plot. Overflow options: 0=no overflow, 1=add overflow content to last bin, 2=add extra bin with overflow contents
    void addHist(TH1F* item, TString label, TString drawopt="", int color=0, int fillstyle=0, int linecolor=1, int linestyle=1, unsigned int plotoverflow=0, int linewidth=3);
    void addHist(TFile *f, TString itemname, TString label, TString drawopt="", int color=0, int fillstyle=0, int linecolor=1, int linestyle=1, unsigned int plotoverflow=0, int linewidth=3);
    void addHistScaled(TH1F* item, double scaleto, TString label, TString drawopt="", int color=0, int fillstyle=0, int linecolor=1, int linestyle=1, unsigned int plotoverflow=0, int linewidth=3);
    void addHistScaled(TFile *f, TString itemname, double scaleto, TString label, TString drawopt="", int color=0, int fillstyle=0, int linecolor=1, int linestyle=1, unsigned int plotoverflow=0, int linewidth=3);
    void addHistForRatio(TH1F* item, TString label, TString drawopt="", int color=0, int fillstyle=0, int linecolor=1, int linestyle=1, unsigned int plotoverflow=0, int linewidth=3, bool onlyplotratio=false);
    void addHist2D(TH2F* item, TString label, TString drawopt="", int color=0, int fillstyle=0, int linecolor=1, int linestyle=1);
    void addHist2DScaled(TH2F* item, double scaleto, TString label, TString drawopt="", int color=0, int fillstyle=0, int linecolor=1, int linestyle=1);
    void addGraph(TGraph* item, TString label, TString drawopt="", int color=0, int fillstyle=0, int linecolor=1, int linestyle=1);
    void addProfile(TProfile* item, TString label, TString drawopt="", int color=0, int fillstyle=0, int linecolor=1, int linestyle=1);

    // Set the uncertainty histogram. The histogram should have both the central values and the uncertainties.
    void setUncertaintyHist(TH1F *unc) { fHistUnc = (TH1F*)unc->Clone(); }

    // Adding a 1D histogram to a histogram stack
    void addToStack(TH1F *h, int color);
    void addToStack(TH1F *h, TString label, int color, int fillstyle=1001, int linecolor=1, int linestyle=1, int linewidth=0, unsigned int plotoverflow=0);
    void addToStack(TFile *f, TString histname, int color);
    void addToStack(TFile *f, TString histname, TString label, int color, int fillstyle=1001, int linecolor=1, int linestyle=1, int linewidth=0, unsigned int plotoverflow=0);

    // Integrate all the 1D histograms. Default direction: (value > [cut])
    void integrateHists(bool reversecutdir = false);

    // Get version of histogram with overflow bin added
    static TH1F* addOverFlow(TH1F* h, unsigned int overflowopt=1);

    // Adding a text box to the plot
    void addTextBox(TString text, double x1, double y1, double x2, double y2, int bordersize=1, int textcolor=0, int fillcolor=0, int textalign=11, float textangle=0.0);
  
    // Add a line between two points (x1,y1) and (x2,y2)
    void addLine(double x1, double y1, double x2, double y2, int color=1, int style=1);
    void addLine(double x1, double y1, double x2, double y2, int color, int style, TString label);
  
    // Add a box with coordinates: bottom left (x1,y1), top right (x2,y2)
    void addBox(double x1, double y1, double x2, double y2, int linecolor=1, int linesty=1, int fillcolor=0);
    void addBox(double x1, double y1, double x2, double y2, int linecolor, int linesty, int fillcolor, TString label);
  
    // Add a 1D function
    void addFcn(TF1* fcn, int color=1, int linesty=1);
    void addFcn(TF1* fcn, TString label, int color=1, int linesty=1); 
    
    // Set legend position
    void setLegend(double x1, double y1, double x2, double y2) {
      assert(fLeg);
      fLeg->SetX1(x1); fLeg->SetY1(y1); fLeg->SetX2(x2); fLeg->SetY2(y2);
    }

    // Translate legend box
    void moveLegend(double dx, double dy) {
      assert(fLeg);
      fLeg->SetX1(fLeg->GetX1()+dx); fLeg->SetY1(fLeg->GetY1()+dy); 
      fLeg->SetX2(fLeg->GetX2()+dx); fLeg->SetY2(fLeg->GetY2()+dy);
    } 
  
    // Set stats box position
    void setStats(double x, double y) { fStatsX = x; fStatsY = y; }
  
    // Translate stats box
    void transStats(double dx, double dy) { fStatsX += dx; fStatsY += dy; }  

    // Add logo, lumi, etc.
    void header(const char* lumitext, const char* channel, double lowX=0.4, double lowY=0.92);
    
    // Set header with logo, lumi, etc.
    void setHeader(TString lumitext, TString channel, double lowX=0.54, double lowY=0.92) {
      fLumiText = lumitext;
      fChanText = channel;
      fHeaderX = lowX;
      fHeaderY = lowY;
    }
    
    // Set general properties of the plot
    void setName(TString str)                { fName = str; }                // plot name (for output)
    void setTitle(TString str)               { fTitle = str; }               // plot title
    void setXTitle(TString str)              { fXTitle = str; }              // x-axis title
    void setYTitle(TString str)              { fYTitle = str; }              // y-axis title
    void setXRange(double xmin, double xmax) { fXmin = xmin; fXmax = xmax; } // x-axis range
    void setYRange(double ymin, double ymax) { fYmin = ymin; fYmax = ymax; } // y-axis range
    void setRatioRange(double xmin, double xmax) { fRatioMin = xmin; fRatioMax = xmax; } // y-axis range of ratios plots
    void setLogx(int value=1)                { fLogx = value; }              // toggle logscale x-axis
    void setLogy(int value=1)                { fLogy = value; }              // toggle logscale y-axis
    void setGridx(bool value=1)              { fGridx = value; }             // toggle grid lines from x-axis ticks
    void setGridy(bool value=1)              { fGridy = value; }             // toggle grid lines from y-axis ticks
    void rebin(int ngroup)                   { fRebin = ngroup; }            // 1D histogram re-bin
    void showStats(int show=111)             { fShowStats = show; }          // display statistics
    void setUsePoisson()                     { fUsePoisson = true; }         // Poisson statistics for data points
    void setPlotRatioUncertaintyBand()       { fPlotRatioUncertaintyBand = true; } // Add uncertainty band to ratio plot
    void setPlotStackUncertainty()           { fPlotStackUncertainty = true; }
    void setDrawCMSLumi(int iPosX=10, TString extraText = "Preliminary", int iPer=8)   { fDrawCMSLumi = true; fCMSLumiPosX=iPosX; fCMSLumiExtraText = extraText; fCMSLumiPeriod=iPer; } // Draw CMS lumi header (and change its position)
    void setDrawCMSLumi(TString lumi, int iPosX=10, TString extraText = "Preliminary") { fDrawCMSLumi = true; fCMSLumiPosX=iPosX; fCMSLumiExtraText = extraText; fCMSLumiPeriod=-1; fCMSLumiText = lumi; } // Draw CMS lumi header (and change its position)

    TGraphAsymmErrors* getAsymmErrors(TH1F* hist);
    TGraphAsymmErrors* getRatioAsymmErrors(TH1F* hnum, TH1F* hden);
    void getRatioUpDownErrors(int dN, double mN, double mE, double& eL, double& eH);

    // Accessors
    TString  getName()   { return fName; }
    TLegend* getLegend() { return fLeg; }
    THStack* getStack()  { return fStack; }
    int      getStackMaxBin();
  
  protected:
    vector<h1D*> fHists1D;                // list of 1D hists to be plotted
    vector<h1D*> fRatioHists1D;           // list of 1D hists to be plotted in ratio plots
    vector<h2D*> fHists2D;                // list of 2D hists to be plotted
    vector<graph*> fGraphs;               // list of graphs to be plotted
    vector<profile*> fProfiles;           // list of profile hists to be plotted
    vector<TPaveText*> fTextBoxes;        // list of text boxes
    vector<TLine*> fLines;                // list of lines
    vector<TBox*> fBoxes;                 // list of boxes
    vector<TF1*> fFcns;                   // list of 1D functions
    THStack *fStack;                      // histogram stack
    TH1F *fHistUnc;                       // uncertainty histogram
    TString fName;                        // plot name
    TString fTitle;                       // plot title
    TString fXTitle;                      // x-axis title
    TString fYTitle;                      // y-axis title
    double fXmin, fXmax;                  // x-axis range
    double fYmin, fYmax;                  // y-axis range
    double fRatioMin, fRatioMax;          // y-axis range of ratio plots
    int fLogx, fLogy;                     // logscale axes
    int fGridx, fGridy;                   // grid lines
    int fRebin;                           // grouping for histogram re-bin
    TLegend *fLeg;                        // legend object
    double fLegX1, fLegY1, fLegX2, fLegY2;// x,y coordinates of legend box
    int fShowStats;                       // whether to display statistics
    double fStatsX, fStatsY;              // x,y coordinates of top left corner of stats box
    TString fLumiText, fChanText;         // text to display in header
    double fHeaderX, fHeaderY;            // x,y coordinates of bottom left corner of header box
    bool fUsePoisson;                     // Poisson error bars for data
    bool fPlotRatioUncertaintyBand;       // Add uncertainty band to ratio plot
    bool fPlotStackUncertainty;           // Add uncertainty band to stacked hists
    bool fDrawCMSLumi;                    // Add TDR style CMS lumi header
    int  fCMSLumiPosX;                    // iPosX in void StyleTools::CMS_lumi(TPad* pad, int iPeriod, int iPosX)
    TString  fCMSLumiText;                // passedLumi in void StyleTools::CMS_lumi(TPad* pad, int iPeriod, int iPosX, TString extraText, TString lumi_pass)
    TString  fCMSLumiExtraText;           // extraText in void StyleTools::CMS_lumi(TPad* pad, int iPeriod, int iPosX, TString extraText)
    int  fCMSLumiPeriod;                  // integer to choose lumi text displayed
  
    vector<TLegendEntry*> fStackEntries;  // pointer to legend entry objects for histograms in a stack
  
    static int counter;                   // number of Plot instances
};

#endif
