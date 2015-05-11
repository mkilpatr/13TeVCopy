#include "AnalysisMethods/PlotUtils/interface/Plot.hh"

int Plot::counter = 0;
TString Plot::outputdir = ".";

Plot::Plot()
{
  TString name = "plot"; 
  name += TString(to_string(counter));
  Plot(name,"","",""); 
}

Plot::Plot(TString name, TString title, TString xtitle, TString ytitle):
  fStack(0),
  fName(name),
  fTitle(title),
  fXTitle(xtitle),
  fYTitle(ytitle),
  fXmin(0),
  fXmax(0),
  fYmin(0),
  fYmax(0),
  fLogx(0),
  fLogy(0),
  fGridx(0),
  fGridy(0),
  fRebin(1),
  fLeg(0),
  fLegX1(0.6),
  fLegY1(0.84),
  fLegX2(0.93),
  fLegY2(0.9),
  fShowStats(0),
  fStatsX(0.68),
  fStatsY(0.90),
  fLumiText("#sqrt{s} = 13 TeV, L = 4 fb^{-1}"),
  fChanText(""),
  fHeaderX(0.54),
  fHeaderY(0.92)
{
  counter++;
}

Plot::~Plot()
{
  clear();
}

void Plot::clear()
{
  fHists1D.clear();
  fHists2D.clear();
  fGraphs.clear();
  fProfiles.clear();
  fTextBoxes.clear();
  fLines.clear();
  fBoxes.clear();
  fFcns.clear();
  fStackEntries.clear();
  fStack=0;
  fLeg=0;
  delete fStack;
  delete fLeg;
}

int Plot::getStackMaxBin()
{

  TH1F* hist0 = (TH1F*)(fStack->GetHists()->First());
  vector<double> bincontents(hist0->GetNbinsX(), 0.0);

  TIter nexthist(fStack->GetHists());
  while(TH1F *hist = (TH1F*)nexthist()) {
    for(int ibin = 0; ibin < hist->GetNbinsX(); ibin++) {
      bincontents.at(ibin) += hist->GetBinContent(ibin);
    }
  }

  double max = 0.0;
  int maxbin = 0;
  for(unsigned int ibin = 0; ibin < bincontents.size(); ibin++) {
    if(bincontents.at(ibin) > max) {
      max = bincontents.at(ibin);
      maxbin = ibin;
    }
  }

  return maxbin;

}

void Plot::addHist(TH1F* item, TString label, TString drawopt, int color, int fillstyle, int linecolor, int linestyle)
{

  if(!item)
    return;

  TH1F* hist = (TH1F*)item->Clone();

  StyleTools::InitHist(hist, fXTitle, fYTitle, color, fillstyle);

  if(linecolor==0)
    hist->SetLineColor(color);
  else
    hist->SetLineColor(linecolor);

  hist->SetLineStyle(linestyle);

  if(!fLeg)
    fLeg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2);
  else
    fLeg->SetY1(fLeg->GetY1()-0.06);

  if(drawopt.CompareTo("E",TString::kIgnoreCase)==0) {
    fLeg->AddEntry(hist,label,"PL");
  } else {
    if(fillstyle > 0) fLeg->AddEntry(hist,label,"F");
    else              fLeg->AddEntry(hist,label,"L");
  }

  fHists1D.push_back(new h1D(hist, drawopt));

}

void Plot::addHist(TFile *f, TString itemname, TString label, TString drawopt, int color, int fillstyle, int linecolor, int linestyle)
{

  if(!f)
    return;

  TH1F* item = (TH1F*)f->FindObjectAny(itemname);

  addHist(item, label, drawopt, color, fillstyle, linecolor, linestyle);

}

void Plot::addHistScaled(TH1F* item, double scaleto, TString label, TString drawopt, int color, int fillstyle, int linecolor, int linestyle)
{

  if(!item)
    return;

  TH1F* hist = (TH1F*)item->Clone();

  hist->Scale(scaleto/hist->Integral(0, hist->GetNbinsX()+1));

  StyleTools::InitHist(hist, fXTitle, fYTitle, color, fillstyle);

  if(linecolor==0)
    hist->SetLineColor(color);
  else
    hist->SetLineColor(linecolor);

  hist->SetLineStyle(linestyle);

  if(!fLeg)
    fLeg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2);
  else
    fLeg->SetY1(fLeg->GetY1()-0.06);

  if(drawopt.CompareTo("E",TString::kIgnoreCase)==0) {
    fLeg->AddEntry(hist,label,"PL");
  } else {
    if(fillstyle > 0) fLeg->AddEntry(hist,label,"F");
    else              fLeg->AddEntry(hist,label,"L");
  }

  fHists1D.push_back(new h1D(hist, drawopt));

}

void Plot::addHistScaled(TFile *f, TString itemname, double scaleto, TString label, TString drawopt, int color, int fillstyle, int linecolor, int linestyle)
{

  if(!f)
    return;

  TH1F* item = (TH1F*)f->FindObjectAny(itemname);

  addHistScaled(item, scaleto, label, drawopt, color, fillstyle, linecolor, linestyle);

}

void Plot::addHist2D(TH2F* item, TString label, TString drawopt, int color, int fillstyle, int linecolor, int linestyle)
{

  if(!item)
    return;

  TH2F* hist = (TH2F*)item->Clone();

  StyleTools::InitHist(hist, fXTitle, fYTitle, color, fillstyle);

  if(linecolor==0)
    hist->SetLineColor(color);
  else
    hist->SetLineColor(linecolor);

  hist->SetLineStyle(linestyle);

  if(!fLeg)
    fLeg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2);
  else
    fLeg->SetY1(fLeg->GetY1()-0.06);

  if(fillstyle > 0) fLeg->AddEntry(hist,label,"F");
  else              fLeg->AddEntry(hist,label,"L");

  fHists2D.push_back(new h2D(hist, drawopt));

}

void Plot::addGraph(TGraph* item, TString label, TString drawopt, int color, int fillstyle, int linecolor, int linestyle)
{

  if(!item)
    return;

  TGraph* gr = (TGraph*)item->Clone();

  StyleTools::InitGraph(item, fTitle, fXTitle, fYTitle, fXmin, fXmax, fYmin, fYmax, color);

  if(linecolor==0)
    gr->SetLineColor(color);
  else
    gr->SetLineColor(linecolor);

  gr->SetLineStyle(linestyle);
  gr->SetMarkerColor(color);
  gr->SetFillColor(color);
  gr->SetFillStyle(fillstyle);

  if(!fLeg)
    fLeg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2);
  else
    fLeg->SetY1(fLeg->GetY1()-0.06);

  if(drawopt.CompareTo("E",TString::kIgnoreCase)==0 || drawopt.CompareTo("P",TString::kIgnoreCase)==0) {
    gr->SetMarkerSize(1.3);
    gr->SetMarkerStyle(20);
    gr->SetLineWidth(3);
    fLeg->AddEntry(gr,label,"PL");
  } else {
    gr->SetLineWidth(3);
    if(fillstyle > 0) fLeg->AddEntry(gr,label,"F");
    else              fLeg->AddEntry(gr,label,"L");
  }

  fGraphs.push_back(new graph(gr, drawopt));

}

void Plot::addProfile(TProfile* item, TString label, TString drawopt, int color, int fillstyle, int linecolor, int linestyle)
{

  if(!item)
    return;

  TProfile* prof = (TProfile*)item->Clone();

  if(linecolor==0)
    prof->SetLineColor(color);
  else
    prof->SetLineColor(linecolor);

  prof->SetLineStyle(linestyle);
  prof->SetFillColor(color);
  prof->SetFillStyle(fillstyle);

  if(!fLeg)
    fLeg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2);
  else
    fLeg->SetY1(fLeg->GetY1()-0.06);

  if(drawopt.CompareTo("E",TString::kIgnoreCase)==0) {
    prof->SetMarkerSize(1.3);
    prof->SetMarkerStyle(20);
    prof->SetLineWidth(3);
    fLeg->AddEntry(prof,label,"PL");
  } else {
    prof->SetLineWidth(3);
    if(fillstyle > 0) fLeg->AddEntry(prof,label,"F");
    else              fLeg->AddEntry(prof,label,"L");
  }

  fProfiles.push_back(new profile(prof, drawopt));

}

void Plot::addToStack(TH1F *h, int color)
{

  if(!h)
    return;
    
  if(!fStack)
    fStack = new THStack(fName+TString("_stack"),"");
  
  fStack->Add(h);
  addHist(h,"",color,1001);

}

void Plot::addToStack(TH1F *h, TString label, int color, int fillstyle, int linecolor, int linestyle, int linewidth)
{

  if(!h)
    return;
  
  TH1F* hist = (TH1F*)h->Clone();

  StyleTools::InitHist(hist, fXTitle, fYTitle, color, fillstyle);

  if(linecolor==0)
    hist->SetLineColor(color);
  else
    hist->SetLineColor(linecolor);

  hist->SetLineStyle(linestyle);
  hist->SetLineWidth(linewidth);
  hist->SetLabelSize(0.05);

  fHists1D.push_back(new h1D(hist, "hist"));

  if(!fStack)
    fStack = new THStack(fName+TString("_stack"),"");
  
  if(!fLeg)
    fLeg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2);
  else
    fLeg->SetY1(fLeg->GetY1()-0.06);  
    
  // make legend entries appear in reverse of the order the histograms are added
  if(label.Length()>0) {
    fStackEntries.push_back(fLeg->AddEntry(hist,label,"F"));
    for(Int_t ientry=(fStackEntries.size()-2); ientry>=0; ientry--) {
      TObject* hh = fStackEntries[ientry]->GetObject();
      TString ll  = fStackEntries[ientry]->GetLabel();
      fStackEntries[ientry+1]->SetObject(hh);
      fStackEntries[ientry+1]->SetLabel(ll);
    }
    fStackEntries[0]->SetObject(hist);
    fStackEntries[0]->SetLabel(label);
  }
     
  fStack->Add(hist);  

}

void Plot::addToStack(TFile *f, TString histname, int color)
{

  if(!f)
    return;
  
  TH1F *h = (TH1F*)f->FindObjectAny(histname);
  addToStack(h,color);

}

void Plot::addToStack(TFile *f, TString histname, TString label, int color, int fillstyle, int linecolor, int linestyle, int linewidth)
{

  if(!f)
    return;
  
  TH1F *h = (TH1F*)f->FindObjectAny(histname);
  addToStack(h,label,color,fillstyle,linecolor,linestyle,linewidth);

}  
  
void Plot::addTextBox(TString text, double x1, double y1, double x2, double y2, int bordersize, int textcolor, int fillcolor)
{

  TPaveText *tb = new TPaveText(x1,y1,x2,y2,"NDC");
  tb->SetTextColor(textcolor);

  if(fillcolor==-1)
    tb->SetFillStyle(0);
  else
    tb->SetFillColor(fillcolor);

  tb->SetBorderSize(bordersize);
  tb->AddText(text);
  fTextBoxes.push_back(tb);

}

void Plot::addLine(double x1, double y1, double x2, double y2, int color, int style)
{

  TLine *line = new TLine(x1,y1,x2,y2);
  line->SetLineColor(color);
  line->SetLineStyle(style);
  line->SetLineWidth(3);
  fLines.push_back(line);

}

void Plot::addLine(double x1, double y1, double x2, double y2, int color, int style, TString label)
{

  TLine *line = new TLine(x1,y1,x2,y2);
  line->SetLineColor(color);
  line->SetLineStyle(style);
  line->SetLineWidth(3);
  fLines.push_back(line);

  if(!fLeg)
    fLeg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2);
  else
    fLeg->SetY1(fLeg->GetY1()-0.06);

  fLeg->AddEntry(line,label,"L");

}

void Plot::addBox(double x1, double y1, double x2, double y2, int linecolor, int linesty, int fillcolor)
{

  TBox *box = new TBox(x1,y1,x2,y2);
  box->SetLineColor(linecolor);
  box->SetLineStyle(linesty);

  if(fillcolor==-1)
    box->SetFillStyle(0);
  else
    box->SetFillColor(fillcolor);

  box->SetLineWidth(2);
  fBoxes.push_back(box);

}

void Plot::addBox(double x1, double y1, double x2, double y2, int linecolor, int linesty, int fillcolor, TString label)
{

  TBox *box = new TBox(x1,y1,x2,y2);
  box->SetLineColor(linecolor);
  box->SetLineStyle(linesty);

  if(fillcolor==-1)
    box->SetFillStyle(0);
  else
    box->SetFillColor(fillcolor);

  box->SetLineWidth(2);
  fBoxes.push_back(box);

  if(!fLeg)
    fLeg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2);
  else
    fLeg->SetY1(fLeg->GetY1()-0.06);
  
  if(fillcolor<0) fLeg->AddEntry(box,label,"L");
  else            fLeg->AddEntry(box,label,"F");

}

void Plot::addFcn(TF1* fcn, int color, int linesty)
{
  if(!fcn)
    return;
  
  fcn->SetLineColor(color);
  fcn->SetLineStyle(linesty);
  fFcns.push_back(fcn);

}

void Plot::addFcn(TF1* fcn, TString label, int color, int linesty)
{

  if(!fcn)
    return;
    
  if(!fLeg)
    fLeg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2);
  else
    fLeg->SetY1(fLeg->GetY1()-0.06);

  fLeg->AddEntry(fcn,label,"L");
  
  addFcn(fcn,color,linesty);

}

void Plot::drawRatio(TCanvas *c, TH1F *h1, TH1F *h2, bool doSave, TString format)
{

  c->cd();
  TPad *p1 = new TPad("p1","p1",0,0.3,1,1);
  p1->SetLeftMargin  (0.18);
  p1->SetTopMargin   (0.10);
  p1->SetRightMargin (0.07);
  p1->SetBottomMargin(0.03);
  p1->Draw();
  p1->cd();
  c->SetLogx(fLogx);
  c->SetLogy(fLogy);
  double ymax = h1->GetMaximum();
  if(h2->GetMaximum()>ymax) ymax=h2->GetMaximum();
  h1->SetMarkerSize(1.3);
  h1->SetMarkerStyle(20);
  h1->SetLineWidth(3);
  h2->SetLineWidth(3);
  h2->GetXaxis()->SetTitle(fXTitle);
  h2->GetXaxis()->SetLabelOffset(0.20);
  h2->GetYaxis()->SetTitle(fYTitle);
  h2->GetYaxis()->SetRangeUser(0,1.1*ymax);
  h2->Draw("hist");
  h1->DrawCopy("same");
  if(fLeg) {
    StyleTools::SetLegendStyle(fLeg);
    fLeg->Draw();
  }
  c->cd();
  TPad *p2 = new TPad("p2","p2",0,0,1,0.3);
  p2->SetLeftMargin  (0.18);
  p2->SetTopMargin   (0.00);
  p2->SetRightMargin (0.07);
  p2->SetBottomMargin(0.30);
  p2->SetGridy(1);
  p2->Draw();
  p2->cd();
  h1->Divide(h2);
  h1->SetLineColor(kBlack);
  h1->GetXaxis()->SetTitle(fXTitle);
  h1->SetTitleSize  (0.12,"Y");
  h1->SetTitleOffset(0.60,"Y");
  h1->SetTitleSize  (0.12,"X");
  h1->SetLabelSize  (0.10,"X");
  h1->SetLabelSize  (0.08,"Y");
  h1->GetYaxis()->SetTitleFont(62);
  h1->GetYaxis()->CenterTitle(kTRUE);
  h1->GetXaxis()->SetTitleFont(62);
  h1->GetYaxis()->SetNdivisions(305);
  h1->GetXaxis()->SetTitle(fXTitle);
  h1->GetYaxis()->SetTitle("Ratio");
  h1->GetYaxis()->SetRangeUser(0.0,2.0);
  h1->Draw("EP");
  double xmin = h1->GetXaxis()->GetXmin();
  double xmax = h1->GetXaxis()->GetXmax();
  TLine *l = new TLine(xmin,1,xmax,1);
  l->SetLineWidth(3);
  l->Draw("same");
  c->cd();

  if(doSave) {
    gSystem->mkdir(outputdir,true);
    TString outname = outputdir+TString("/")+fName+TString(".");
    if(format.CompareTo("all",TString::kIgnoreCase)==0) {
      c->SaveAs(outname+TString("png"));
      c->SaveAs(outname+TString("C"));
    } else {
      c->SaveAs(outname+format);
    }
  }

}

void Plot::integrateHists(bool reversecutdir)
{
	if (fHists1D.empty())
		return;

	for (auto h1D: fHists1D){
		TH1F *hist = (TH1F *)h1D->member->Clone("_hnew");
		Double_t integral, error;

		int nbins = hist->GetNbinsX();
		for(int ibin = 0; ibin <= nbins; ibin++) {
			if(reversecutdir) {
				integral = hist->IntegralAndError(0, ibin, error);
			} else {
				integral = hist->IntegralAndError(ibin, nbins+1, error);
			}
			h1D->member->SetBinContent(ibin, integral);
			h1D->member->SetBinError(ibin, error);
		}
		delete hist;
	}
}

void Plot::drawRatioStack(TCanvas *c, TH1F *hData, TH1F *hMC, bool doSave, TString format)
{
  c->cd();

  TPad *p1 = new TPad("p1","p1",0,0.3,1,1);
  p1->SetLeftMargin  (0.18);
  p1->SetTopMargin   (0.10);
  p1->SetRightMargin (0.07);
  p1->SetBottomMargin(0.03);  
  p1->Draw();
  if(fLogy) p1->SetLogy();
  p1->cd();

  double ymax = hData->GetMaximum();
  if(hMC->GetMaximum()>ymax) ymax=hMC->GetMaximum();
  hData->SetMarkerSize(1.3);
  hData->SetMarkerStyle(20);
  hData->SetLineWidth(3);

  TH1F *h00 = (TH1F*)hData->Clone("data0");

  if(fStack) {
    if(!fLogy) {
      if(fYmin < fYmax) {
        fStack->SetMaximum(fYmax);
        fStack->SetMinimum(fYmin);
      } else {
        fStack->SetMaximum(1.1*ymax);
        fStack->SetMinimum(0);
      }
    }
    fStack->SetTitle(fTitle);
    fStack->Draw("hist");
    fStack->GetXaxis()->SetLabelOffset(0.20);
    fStack->GetYaxis()->SetTitle(fYTitle);
    if(fYmin < fYmax) fStack->GetYaxis()->SetRangeUser(fYmin,fYmax);
    else fStack->GetYaxis()->SetRangeUser(0,1.1*ymax);
    fStack->GetYaxis()->SetTitleSize(0.055);
    fStack->Draw("histsame");
  } 

  h00->Draw("same");


  if(fLeg) {
    fLeg->SetFillStyle(0);
    fLeg->SetBorderSize(0);
    fLeg->Draw();
  }

  c->cd();
  TPad *p2 = new TPad("p2","p2",0,0,1,0.3);
  p2->SetLeftMargin  (0.18);
  p2->SetTopMargin   (0.00);
  p2->SetRightMargin (0.07);
  p2->SetBottomMargin(0.30);
  p2->SetGridy(1);
  p2->Draw();
  p2->cd();

  TH1F *h3 = (TH1F*)hData->Clone("data3"); 

  h3->SetTitleSize  (0.12,"Y");
  h3->SetTitleOffset(0.60,"Y");
  h3->SetTitleSize  (0.12,"X");
  h3->SetLabelSize  (0.10,"X");
  h3->SetLabelSize  (0.08,"Y");
  h3->GetYaxis()->SetTitleFont(62);
  h3->GetYaxis()->CenterTitle(kTRUE);
  h3->GetXaxis()->SetTitleFont(62);
  h3->GetYaxis()->SetRangeUser(-0.5,0.5);
  h3->GetYaxis()->SetNdivisions(305);
  h3->GetXaxis()->SetTitle(fXTitle);
  h3->GetYaxis()->SetTitle("#frac{N_{obs}}{N_{exp}} - 1");

  double xmin = hData->GetXaxis()->GetXmin();
  double xmax = hData->GetXaxis()->GetXmax();
  TLine *l = new TLine(xmin,0,xmax,0);
  l->SetLineWidth(3);
  l->SetLineColor(kBlack);

  h3->Add(hMC, -1.);
  h3->Divide(hMC);
  h3->GetYaxis()->SetRangeUser(-0.4,0.4);

  h3->DrawCopy("P");  
  h3->Draw("Psame");
  l->Draw("same");

  c->cd();

  if(doSave) {
    gSystem->mkdir(outputdir,true);
    TString outname = outputdir+TString("/")+fName+TString(".");
    if(format.CompareTo("all",TString::kIgnoreCase)==0) {
      c->SaveAs(outname+TString("png"));
      c->SaveAs(outname+TString("C"));
    } else {
      c->SaveAs(outname+format);
    }
  }

}


void Plot::draw(TCanvas *c, bool doSave, TString format)
{ 

  c->cd();
  
  c->SetLogy(0);
  c->SetLogx(0);
  
  if(!fHists1D.size() && !fHists2D.size() && !fGraphs.size() && !fProfiles.size())
    return;   
  
  // Draw 2D histogram, save if necessary, then exit
  //   Suggested options for:
  //     contour plot -> "CONT4Z"
  //     lego plot    -> "LEGO1 0"
  //     color plot   -> "COLZ"
  //   Default is scatter plot
  if(fHists2D.size()>0) {
    for(uint i=0; i<fHists2D.size(); i++) {
      fHists2D[i]->member->Draw(fHists2D[i]->opt);
      fHists2D[i]->member->SetTitle(fTitle);
      fHists2D[i]->member->GetXaxis()->SetTitle(fXTitle);
      fHists2D[i]->member->GetYaxis()->SetTitle(fYTitle);
    
      // Set log scale if necessary
      c->SetLogx(fLogx);
      c->SetLogy(fLogy);
      
      for(uint k=0; k<fLines.size(); k++)
        fLines[k]->Draw();

      for(uint k=0; k<fBoxes.size(); k++)
        fBoxes[k]->Draw();
      
      for(uint j=0; j<fTextBoxes.size(); j++)
        fTextBoxes[j]->Draw();
            
      if(doSave) {
        gSystem->mkdir(outputdir,true);
        TString outname = outputdir+TString("/")+fName+TString(".");
	if(format.CompareTo("all",TString::kIgnoreCase)==0) {
	  c->SaveAs(outname+TString("pdf"));
	  c->SaveAs(outname+TString("png"));
	  c->SaveAs(outname+TString("C"));
	} else {
	  c->SaveAs(outname+format);
	}
      }
      
      return;
    }
  }    
  
  // Draw 1D histograms
  //   Histograms are cloned so that content and properties 
  //   of the original histograms are not changed
  std::vector<TH1F*> vHists;
  std::vector<TString> vHistOpts;
  if(fHists1D.size()>0) {   
    
    double ymax=0;
    uint ifirst=0;
    
    for(uint i=0; i<fHists1D.size(); i++) {
      if(fStack && fStack->GetHists()->Contains(fHists1D[i]->member)) continue;
      
      TString hname = fName;
      hname += "_h_";
      hname += TString(to_string(i));
    
      TH1F *h;     
      if(fRebin>1)      
        h = (TH1F*)fHists1D[i]->member->Rebin(fRebin,hname);
      else
        h = (TH1F*)fHists1D[i]->member->Clone(hname);
  
      if(fXmin < fXmax) {
        h->GetXaxis()->SetRangeUser(fXmin,fXmax);
      }
      
      if(fYmin < fYmax) { 
        h->GetYaxis()->SetRangeUser(fYmin,fYmax);
      } else {
        if(ymax < h->GetMaximum()) {
	  ymax = h->GetMaximum();
	  ifirst = vHists.size();
	}
      }
      
      vHists.push_back(h);
      vHistOpts.push_back(fHists1D[i]->opt);
    }
    
    if(vHists.size()>0) {
      vHists[ifirst]->SetTitle(fTitle);
      vHists[ifirst]->GetXaxis()->SetTitle(fXTitle);
      vHists[ifirst]->GetYaxis()->SetTitle(fYTitle);
      vHists[ifirst]->SetLineWidth(3);
      vHists[ifirst]->Draw(vHistOpts[ifirst].Data());
    }
   
    // Draw histogram stack
    if(fStack) {
      if(vHists.size()>0) { 
        if(fYmin < fYmax) {
	  fStack->Draw("hist same");
	} else {
	  if(fStack->GetMaximum() > vHists[ifirst]->GetMaximum()) {
	    fStack->SetTitle(fTitle);
	    fStack->Draw();
	    fStack->GetXaxis()->SetTitle(fXTitle);
	    fStack->GetYaxis()->SetTitle(fYTitle);
	    fStack->Draw("hist");
	  } else {	    
	    fStack->Draw("hist same"); 
	  }
        }
	
      } else {
        // NOTE: Must draw first before accessing axes
	fStack->Draw("hist"); 

	if(fXmin < fXmax)
          fStack->GetXaxis()->SetRangeUser(fXmin,fXmax);
	 
	if(fYmin < fYmax) {
          fStack->SetMaximum(fYmax);  
	  fStack->SetMinimum(fYmin);
        }
	 
        fStack->SetTitle(fTitle);
	fStack->GetXaxis()->SetTitle(fXTitle);
	fStack->GetYaxis()->SetTitle(fYTitle);	
        fStack->Draw("hist");	 
      } 
    }
        
    for(uint i=0; i<vHists.size(); i++) {
      TH1F *h = vHists[i];              
      h->SetLineWidth(3);
      char opt[100];
      sprintf(opt,"same%s",vHistOpts[i].Data());
      h->Draw(opt);
      //if(i!=ifirst) h->Draw(opt);
    }
  }  
  c->RedrawAxis();
  
  // Draw graphs
  std::vector<TGraph*> vGraphs;
  std::vector<TString> vGraphOpts;
  if(fGraphs.size()>0) {    
    for(uint i=0; i<fGraphs.size(); i++) {
      if(!TString(fGraphs[i]->classname()).Contains("TGraph")) continue;
    
      TString grName = fName;
      grName += "_gr_";
      grName += i;
      
      TGraph *gr = (TGraph*)fGraphs[i]->member->Clone(grName);
      
      if(fXmin < fXmax)
        gr->GetXaxis()->SetLimits(fXmin,fXmax);
    
      if(fYmin < fYmax)
        gr->GetYaxis()->SetRangeUser(fYmin,fYmax);
	
      vGraphs.push_back(gr);
      vGraphOpts.push_back(fGraphs[i]->opt);
    }
    
    if(vHists.size()==0) {
      vGraphs[0]->SetTitle(fTitle);
      vGraphs[0]->GetXaxis()->SetTitle(fXTitle);
      vGraphs[0]->GetYaxis()->SetTitle(fYTitle);
    }
    
    for(uint i=0; i<vGraphs.size(); i++) {
      TGraph *gr = vGraphs[i];
      char opt[100];
      (i==0 && fHists1D.size()==0) ? sprintf(opt,"A%s",vGraphOpts[i].Data()) : sprintf(opt,"%s",vGraphOpts[i].Data());
      gr->Draw(opt);
    }
  }

  c->RedrawAxis();
  
  // Draw profile histograms
  std::vector<TProfile*> vProfiles;
  std::vector<TString> vProfileOpts;
  if(fProfiles.size()>0) {    
    for(uint i=0; i<fProfiles.size(); i++) {
      if(!TString(fProfiles[i]->classname()).Contains("TProfile")) continue;
    
      TString prName = fName;
      prName += "_pr_";
      prName += i;
      
      TProfile *pr = (TProfile*)fProfiles[i]->member->Clone(prName);
      
      if(fXmin < fXmax)
        pr->GetXaxis()->SetLimits(fXmin,fXmax);
    
      if(fYmin < fYmax)
        pr->GetYaxis()->SetRangeUser(fYmin,fYmax);
	
      vProfiles.push_back(pr);
      vProfileOpts.push_back(fProfiles[i]->opt);
    }
    
    if(vHists.size()==0) {
      vProfiles[0]->SetTitle(fTitle);
      vProfiles[0]->GetXaxis()->SetTitle(fXTitle);
      vProfiles[0]->GetYaxis()->SetTitle(fYTitle);
    }
    
    for(uint i=0; i<vProfiles.size(); i++) {
      TProfile *pr = vProfiles[i];
      char opt[100];
      if(i>0 || fHists1D.size()>0 || fGraphs.size()>0) 
        sprintf(opt,"same%s",vProfileOpts[i].Data());
      else
        sprintf(opt,"%s",vProfileOpts[i].Data());
      pr->Draw(opt);
    }
  }
      
  c->RedrawAxis();
  
  // Draw legend
  if(fLeg) {
    fLeg->SetFillStyle(0);
    fLeg->SetFillColor(0);
    fLeg->SetBorderSize(0);
    fLeg->Draw();
  }
  
  // Draw statistics box
  TLatex *stat=0, *sval=0;
  if(fShowStats) {
    char buffer[20];
    stat = new TLatex[3*vHists.size()];
    sval = new TLatex[3*vHists.size()];
    for(uint i=0; i<vHists.size(); i++) {
      int x = fShowStats;
      
      // number of entries
      if(x / 100) {
        stat[3*i].SetNDC(); stat[3*i].SetTextAlign(13); stat[3*i].SetTextSize(0.03);
        stat[3*i].SetText(fStatsX,fStatsY-0.04*(3*i)-0.005*i,"Entries");
	stat[3*i].SetTextColor(vHists[i]->GetLineColor());
        stat[3*i].Draw();   
        sprintf(buffer,"%i",int(vHists[i]->Integral()));
        sval[3*i].SetNDC(); sval[3*i].SetTextAlign(33); sval[3*i].SetTextSize(0.03);
        sval[3*i].SetText(fStatsX+0.25,fStatsY-0.04*(3*i)-0.005*i,buffer);
	sval[3*i].SetTextColor(vHists[i]->GetLineColor());
        sval[3*i].Draw();
      }
      
      // mean
      x = x % 100;
      if(x / 10) {
        stat[3*i+1].SetNDC(); stat[3*i+1].SetTextAlign(13); stat[3*i+1].SetTextSize(0.03);
        stat[3*i+1].SetText(fStatsX,fStatsY-0.04*(3*i+1)-0.005*i,"Mean");
	stat[3*i+1].SetTextColor(vHists[i]->GetLineColor());
        stat[3*i+1].Draw();   
        sprintf(buffer,"%g",vHists[i]->GetMean());
        sval[3*i+1].SetNDC(); sval[3*i+1].SetTextAlign(33); sval[3*i+1].SetTextSize(0.03);
        sval[3*i+1].SetText(fStatsX+0.25,fStatsY-0.04*(3*i+1)-0.005*i,buffer);
	sval[3*i+1].SetTextColor(vHists[i]->GetLineColor());
        sval[3*i+1].Draw();
      }
      
      // RMS
      x = x % 10;
      if(x) {
        stat[3*i+2].SetNDC(); stat[3*i+2].SetTextAlign(13); stat[3*i+2].SetTextSize(0.03);
        stat[3*i+2].SetText(fStatsX,fStatsY-0.04*(3*i+2)-0.005*i,"RMS");
	stat[3*i+2].SetTextColor(vHists[i]->GetLineColor());
        stat[3*i+2].Draw();   
        sprintf(buffer,"%g",vHists[i]->GetRMS());
        sval[3*i+2].SetNDC(); sval[3*i+2].SetTextAlign(33); sval[3*i+2].SetTextSize(0.03);
        sval[3*i+2].SetText(fStatsX+0.25,fStatsY-0.04*(3*i+2)-0.005*i,buffer);
	sval[3*i+2].SetTextColor(vHists[i]->GetLineColor());
        sval[3*i+2].Draw();
      }
    }
  }
 
  // Draw functions
  for(uint i=0; i<fFcns.size(); i++)
    (i==0 && vHists.size()==0 && vGraphs.size()==0) ? fFcns[i]->Draw() : fFcns[i]->Draw("sameC");
  
  // Draw lines
  for(uint i=0; i<fLines.size(); i++)
    fLines[i]->Draw();
  
  // Draw Boxes
  for(uint i=0; i<fBoxes.size(); i++)
    fBoxes[i]->Draw();
  
  // Draw textboxes
  for(uint i=0; i<fTextBoxes.size(); i++)
    fTextBoxes[i]->Draw();    
  
  // Set log scale if necessary
  c->SetLogx(fLogx);
  c->SetLogy(fLogy);
  
  // Set grid lines if necessary
  c->SetGridx(fGridx);
  c->SetGridy(fGridy);

  // Add header and lumi text
  header(fLumiText.Data(), fChanText.Data(), fHeaderX, fHeaderY);

  // Save plot if necessary
  if(doSave) {
    gSystem->mkdir(outputdir,true);
    TString outname = outputdir+TString("/")+fName+TString(".");
    if(format.CompareTo("all",TString::kIgnoreCase)==0) {
      c->SaveAs(outname+TString("png"));
      c->SaveAs(outname+TString("C"));
    } else {
      c->SaveAs(outname+format);
    }
    
    delete [] stat;
    delete [] sval;
  }
}

void Plot::header(const char* lumitext, const char* channel, double lowX, double lowY)
{
  TPaveText* lumi  = new TPaveText(lowX, lowY, lowX+0.25, lowY+0.04, "NDC");
  lumi->SetBorderSize(   0 );
  lumi->SetFillStyle(    0 );
  lumi->SetTextAlign(   11 );
  lumi->SetTextSize ( 0.045);
  lumi->SetTextColor(    1 );
  lumi->SetTextFont (   62 );
  lumi->AddText(lumitext);
  lumi->Draw();

  TPaveText* chan     = new TPaveText(lowX+0.68, lowY, lowX+0.73, lowY+0.04, "NDC");
  chan->SetBorderSize(   0 );
  chan->SetFillStyle(    0 );
  chan->SetTextAlign(   11 );
  chan->SetTextSize ( 0.045);
  chan->SetTextColor(    1 );
  chan->SetTextFont (   62 );
  chan->AddText(channel);
  chan->Draw();
}
