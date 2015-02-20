#include "AnalysisMethods/PlotUtils/interface/StyleTools.hh"

using namespace StyleTools;

TCanvas* StyleTools::MakeCanvas(const char* name, const char *title, int dX, int dY)
{
  // Start with a canvas
  TCanvas *canvas = new TCanvas(name,title,0,0,dX,dY);

  // General overall stuff
  canvas->SetFillColor      (0);
  canvas->SetBorderMode     (0);
  canvas->SetBorderSize     (10);

  // Set margins to reasonable defaults
  canvas->SetLeftMargin     (0.18);
  canvas->SetRightMargin    (0.05);
  canvas->SetTopMargin      (0.08);
  canvas->SetBottomMargin   (0.15);
  
  // Setup a frame which makes sense
  canvas->SetFrameFillStyle (0);
  canvas->SetFrameLineStyle (0);
  canvas->SetFrameLineWidth (3);
  canvas->SetFrameBorderMode(0);
  canvas->SetFrameBorderSize(10);
  canvas->SetFrameFillStyle (0);
  canvas->SetFrameLineStyle (0);
  canvas->SetFrameBorderMode(0);
  canvas->SetFrameBorderSize(10);

  return canvas;
}

void StyleTools::InitSubPad(TPad* pad, int i)
{
  pad->cd(i);
  TPad *tmpPad = (TPad*) pad->GetPad(i);
  tmpPad->SetLeftMargin  (0.18);
  tmpPad->SetTopMargin   (0.05);
  tmpPad->SetRightMargin (0.07);
  tmpPad->SetBottomMargin(0.15);
  return;
}

void StyleTools::InitHist(TH1 *hist, const char *xtit, const char *ytit, int color, int style)
{
  hist->SetXTitle(xtit);
  hist->SetYTitle(ytit);
  hist->SetLineColor(kBlack);
  hist->SetLineWidth(    3.);
  hist->SetFillColor(color );
  hist->SetFillStyle(style );
  hist->SetTitleSize  (0.055,"Y");
  hist->SetTitleOffset(1.600,"Y");
  hist->SetLabelOffset(0.014,"Y");
  hist->SetLabelSize  (0.040,"Y");
  hist->SetLabelFont  (62   ,"Y");
  hist->SetTitleSize  (0.055,"X");
  hist->SetTitleOffset(1.300,"X");
  hist->SetLabelOffset(0.014,"X");
  hist->SetLabelSize  (0.040,"X");
  hist->SetLabelFont  (62   ,"X");
  hist->SetMarkerStyle(20);
  hist->SetMarkerColor(color);
  hist->SetMarkerSize (1.3);
  hist->GetYaxis()->SetTitleFont(62);
  hist->GetXaxis()->SetTitleFont(62);
  hist->SetTitle("");  
  return;
}

void StyleTools::SetLegendStyle(TLegend* leg)
{
  leg->SetFillStyle (0);
  leg->SetFillColor (0);
  leg->SetBorderSize(0);
}

void StyleTools::SetStyle()
{
  TStyle *MyStyle = new TStyle("New Style","Better than the default style :-)");
  gStyle = MyStyle;

  // Canvas
  MyStyle->SetCanvasColor     (0);
  MyStyle->SetCanvasBorderSize(10);
  MyStyle->SetCanvasBorderMode(0);
  MyStyle->SetCanvasDefH      (700);
  MyStyle->SetCanvasDefW      (700);
  MyStyle->SetCanvasDefX      (100);
  MyStyle->SetCanvasDefY      (100);

  // color palette for 2D temperature plots
  MyStyle->SetPalette(1,0);

  // Pads
  MyStyle->SetPadColor       (0);
  MyStyle->SetPadBorderSize  (10);
  MyStyle->SetPadBorderMode  (0);
  MyStyle->SetPadBottomMargin(0.13);
  MyStyle->SetPadTopMargin   (0.08);
  MyStyle->SetPadLeftMargin  (0.15);
  MyStyle->SetPadRightMargin (0.05);
  MyStyle->SetPadGridX       (0);
  MyStyle->SetPadGridY       (0);
  MyStyle->SetPadTickX       (1);
  MyStyle->SetPadTickY       (1);

  // Frames
  MyStyle->SetLineWidth(3);
  MyStyle->SetFrameFillStyle ( 0);
  MyStyle->SetFrameFillColor ( 0);
  MyStyle->SetFrameLineColor ( 1);
  MyStyle->SetFrameLineStyle ( 0);
  MyStyle->SetFrameLineWidth ( 3);
  MyStyle->SetFrameBorderSize(10);
  MyStyle->SetFrameBorderMode( 0);

  // Histograms
  MyStyle->SetHistFillColor(2);
  MyStyle->SetHistFillStyle(0);
  MyStyle->SetHistLineColor(1);
  MyStyle->SetHistLineStyle(0);
  MyStyle->SetHistLineWidth(3);
  MyStyle->SetNdivisions(505, "X");

  // Functions
  MyStyle->SetFuncColor(1);
  MyStyle->SetFuncStyle(0);
  MyStyle->SetFuncWidth(2);

  // Various
  MyStyle->SetMarkerStyle(20);
  MyStyle->SetMarkerColor(kBlack);
  MyStyle->SetMarkerSize (1.4);

  MyStyle->SetTitleBorderSize(0);
  MyStyle->SetTitleFillColor (0);
  MyStyle->SetTitleX         (0.2);

  MyStyle->SetTitleSize  (0.055,"X");
  MyStyle->SetTitleOffset(1.200,"X");
  MyStyle->SetLabelOffset(0.005,"X");
  MyStyle->SetLabelSize  (0.040,"X");
  MyStyle->SetLabelFont  (62   ,"X");

  MyStyle->SetStripDecimals(kFALSE);
  MyStyle->SetLineStyleString(11,"20 10");

  MyStyle->SetTitleSize  (0.055,"Y");
  MyStyle->SetTitleOffset(1.600,"Y");
  MyStyle->SetLabelOffset(0.010,"Y");
  MyStyle->SetLabelSize  (0.040,"Y");
  MyStyle->SetLabelFont  (62   ,"Y");

  MyStyle->SetTextSize   (0.055);
  MyStyle->SetTextFont   (62);

  MyStyle->SetStatFont   (62);
  MyStyle->SetTitleFont  (62);
  MyStyle->SetTitleFont  (62,"X");
  MyStyle->SetTitleFont  (62,"Y");

  MyStyle->SetOptStat    (0);

  return;
}

ColorMap StyleTools::DefaultColors()
{

  ColorMap colors;

  colors["ttbar"]           = color_ttbar;
  colors["ttbar1l"]         = color_ttbar1l;
  colors["ttbar2l"]         = color_ttbar2l;
  colors["ttZ"]             = color_ttZ;
  colors["wjets"]           = color_wjets;
  colors["other"]           = color_other; 
  colors["znunu"]           = color_znunu;
  colors["tW"]              = color_tW;
  colors["ttW"]             = color_ttW;
  colors["qcd"]             = color_qcd;
  colors["T2tt_850_100"]    = color_T2tt_850_100;
  colors["T2tt_650_325"]    = color_T2tt_650_325;
  colors["T2tt_500_325"]    = color_T2tt_500_325;
  colors["T1tttt_1500_100"] = color_T1tttt_1500_100;
  colors["T1tttt_1200_800"] = color_T1tttt_1200_800;
  colors["comp1"]           = color_comp1;
  colors["comp2"]           = color_comp2;
  colors["comp3"]           = color_comp3;
  colors["comp4"]           = color_comp4;
  colors["comp5"]           = color_comp5;
  colors["comp6"]           = color_comp6;
  colors["ttbar"]           = color_ttbar;

  return colors;

}
