#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraph2D.h"
#endif

vector<TGraph> DrawContours(TGraph2D &g2, int color, int style,
                    TLegend *leg = 0, const string &name = ""){
  vector<TGraph> out;
  TH2D* hist = g2.GetHistogram();
  TVirtualHistPainter* histptr = hist->GetPainter();
  TList *l = histptr->GetContourList(1.);
  if(!l)
    return out;
  bool added = false;
  int max_points = -1;
  for(int i = 0; i < l->GetSize(); ++i){
    TGraph *g = static_cast<TGraph*>(l->At(i));
    if(g == 0) {
      continue;
    }
    //int n_points = g->GetN();
    out.push_back(*g);
    g->SetLineColor(color);
    g->SetLineStyle(style);
    g->SetLineWidth(5);
    g->Draw("L same");
    if(!added && leg && name != ""){
      leg->AddEntry(g, name.c_str(), "l");
      added = true;
    }
  }
  return out;
}
  
void SetupColors(){
  const unsigned num = 5;
  const int bands = 255;
  int colors[bands];
  double stops[num] = {0.00, 0.34, 0.61, 0.84, 1.00};
  double red[num] = {0.50, 0.50, 1.00, 1.00, 1.00};
  double green[num] = {0.50, 1.00, 1.00, 0.60, 0.50};
  double blue[num] = {1.00, 1.00, 0.50, 0.40, 0.50};
  /*const unsigned num = 6;
 *   double red[num] =   {1.,0.,0.,0.,1.,1.};
 *     double green[num] = {0.,0.,1.,1.,1.,0.};
 *       double blue[num] =  {1.,1.,1.,0.,0.,0.};
 *         double stops[num] = {0.,0.2,0.4,0.6,0.8,1.};*/
  int fi = TColor::CreateGradientColorTable(num,stops,red,green,blue,bands);
  for(int i = 0; i < bands; ++i){
    colors[i] = fi+i;
  }
  gStyle->SetNumberContours(bands);
  gStyle->SetPalette(bands, colors);
}

void makeScanPlots(const TString inputFileName = "results_T2tt.root")
{

  SetupColors();

  TFile* inputFile = new TFile(inputFileName);

  TH2D* hobs = (TH2D*)inputFile->Get("hobs");
  TH2D* hobsdown = (TH2D*)inputFile->Get("hobsdown");
  TH2D* hobsup = (TH2D*)inputFile->Get("hobsup");
  TH2D* hexp = (TH2D*)inputFile->Get("hexp");
  TH2D* hexpdown = (TH2D*)inputFile->Get("hexpdown");
  TH2D* hexpup = (TH2D*)inputFile->Get("hexpup");
  TH2D* hxsecexp = (TH2D*)inputFile->Get("hxsecexp");

  vector<double> mstops, mlsps;
  vector<double> exp, expdown, expup, expxsec;
  vector<double> obs, obsdown, obsup;

  for(int ibinx = 1; ibinx < hexp->GetNbinsX()+1; ++ibinx) {
    for(int ibiny = 1; ibiny < hexp->GetNbinsX()+1; ++ibiny) {
      if(hexp->GetBinContent(ibinx, ibiny) != 0.0) {
        mstops.push_back(hexp->GetXaxis()->GetBinLowEdge(ibinx));
        mlsps.push_back(hexp->GetYaxis()->GetBinLowEdge(ibiny));
        obs.push_back(hobs->GetBinContent(ibinx, ibiny));
        obsdown.push_back(hobsdown->GetBinContent(ibinx, ibiny));
        obsup.push_back(hobsup->GetBinContent(ibinx, ibiny));
        exp.push_back(hexp->GetBinContent(ibinx, ibiny));
        expdown.push_back(hexpdown->GetBinContent(ibinx, ibiny));
        expup.push_back(hexpup->GetBinContent(ibinx, ibiny));
        expxsec.push_back(hxsecexp->GetBinContent(ibinx, ibiny));
        printf("MStop: %4.2f, MLSP: %4.2f, Limit: %4.2f, (+1: %4.2f, -1: %4.2f), Obs Limit: %4.2f (+1 theory: %4.2f, -1 theory: %4.2f), XS Limit: %4.2f\n", mstops.back(), mlsps.back(), exp.back(), expup.back(), expdown.back(), obs.back(), obsup.back(), obsdown.back(), expxsec.back());
      }
    }
  }

  TGraph2D glim("glim", "Cross-Section Limit", expxsec.size(), &mstops.at(0), &mlsps.at(0), &expxsec.at(0));
  TGraph2D gexp("gexp", "Expected Limit", exp.size(), &mstops.at(0), &mlsps.at(0), &exp.at(0));
  TGraph2D gexpdown("gexpdown", "Expected  -1#sigma Limit", expdown.size(), &mstops.at(0), &mlsps.at(0), &expdown.at(0));
  TGraph2D gexpup("gexpup", "Expected  -1#sigma Limit", expup.size(), &mstops.at(0), &mlsps.at(0), &expup.at(0));
  TGraph2D gobs("gobs", "Observed Limit", obs.size(), &mstops.at(0), &mlsps.at(0), &obs.at(0));
  TGraph2D gobsdown("gobsdown", "Observed  -1#sigma (theory) Limit", obsdown.size(), &mstops.at(0), &mlsps.at(0), &obsdown.at(0));
  TGraph2D gobsup("gobsup", "Observed  -1#sigma (theory) Limit", obsup.size(), &mstops.at(0), &mlsps.at(0), &obsup.at(0));
  TGraph dots(mstops.size(), &mstops.at(0), &mlsps.at(0));

  double xmin = *min_element(mstops.cbegin(), mstops.cend());
  double xmax = *max_element(mstops.cbegin(), mstops.cend());
  double ymin = *min_element(mlsps.cbegin(), mlsps.cend());
  double ymax = *max_element(mlsps.cbegin(), mlsps.cend());
  double bin_size = 12.5;

  int nxbins = max(1, min(500, static_cast<int>(ceil((xmax-xmin)/bin_size))));
  int nybins = max(1, min(500, static_cast<int>(ceil((ymax-ymin)/bin_size))));
  printf("XMin: %4.2f, XMax: %4.2f, YMin: %4.2f, YMax: %4.2f, NXBins: %d, NYBins: %d\n", xmin, xmax, ymin, ymax, nxbins, nybins);
  glim.SetNpx(nxbins);
  glim.SetNpy(nybins);

  TH2D *hlim = glim.GetHistogram();
  if(!hlim) throw runtime_error("Could not retrieve histogram");
  hlim->SetTitle(";m_{stop} [GeV];m_{LSP} [GeV]");

  TCanvas c("","",800,800);
  c.SetLogz();
  hlim->SetMinimum(*min_element(expxsec.cbegin(), expxsec.cend()));
  hlim->SetMaximum(*max_element(expxsec.cbegin(), expxsec.cend()));
  glim.Draw("colz");
  TLegend l(gStyle->GetPadLeftMargin(), 1.-gStyle->GetPadTopMargin(),
            1.-gStyle->GetPadRightMargin(), 1.);
  l.SetNColumns(2);
  l.SetBorderSize(0);
  vector<TGraph> cobsup = DrawContours(gobsup, 1, 2, &l, "ObsUp");
  vector<TGraph> cobsdown = DrawContours(gobsdown, 1, 2, &l, "ObsDown");
  vector<TGraph> cobs = DrawContours(gobs, 1, 1, &l, "Observed");
  vector<TGraph> cexpup = DrawContours(gexpup, 2, 2, &l, "ExpUp");
  vector<TGraph> cexpdown = DrawContours(gexpdown, 2, 2, &l, "ExpDown");
  vector<TGraph> cexp = DrawContours(gexp, 2, 1, &l, "Expected");

  l.Draw("same");
  dots.Draw("p same");
  c.Print("limit_scan.pdf");

  TFile file("limit_scan.root","recreate");
  hlim->Write("hXsec_exp_corr");

  TString gname = "graph_smoothed";

  for(unsigned int ilim = 0; ilim < cobs.size(); ++ilim) {
    TString add = ilim > 0 ? "_" + TString(to_string(ilim)) : "";
    cobs.at(ilim).Write(gname + "_Obs" + add);
  }
  for(unsigned int ilim = 0; ilim < cobsup.size(); ++ilim) {
    TString add = ilim > 0 ? "_" + TString(to_string(ilim)) : "";
    cobsup.at(ilim).Write(gname + "_ObsP" + add);
  }
  for(unsigned int ilim = 0; ilim < cobsdown.size(); ++ilim) {
    TString add = ilim > 0 ? "_" + TString(to_string(ilim)) : "";
    cobsdown.at(ilim).Write(gname + "_ObsM" + add);
  }
  for(unsigned int ilim = 0; ilim < cexp.size(); ++ilim) {
    TString add = ilim > 0 ? "_" + TString(to_string(ilim)) : "";
    cexp.at(ilim).Write(gname + "_Exp" + add);
  }
  for(unsigned int ilim = 0; ilim < cexpup.size(); ++ilim) {
    TString add = ilim > 0 ? "_" + TString(to_string(ilim)) : "";
    cexpup.at(ilim).Write(gname + "_ExpP" + add);
  }
  for(unsigned int ilim = 0; ilim < cexpdown.size(); ++ilim) {
    TString add = ilim > 0 ? "_" + TString(to_string(ilim)) : "";
    cexpdown.at(ilim).Write(gname + "_ExpM" + add);
  }

  file.Close();


}
