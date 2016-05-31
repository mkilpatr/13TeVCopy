#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"

  TH1F * HistogramGetter::getHistogramManual(TTree* tree,TString histSelection,TString histWeight, TString histSampleName){
    selection = histSelection;
    weight = histWeight;
    sampleName = histSampleName;
    return getHistogramManual(tree);
  }

  TH1F * HistogramGetter::getHistogram(TTree* tree,TString histSelection,TString histWeight, TString histSampleName){
    selection = histSelection;
    weight = histWeight;
    sampleName = histSampleName;
    return getHistogram(tree);
  }


  TH1F*  HistogramGetter::getTFAndCov(TTree* tree,TString numSelection,TString numWeight, TString denSelection,TString denWeight, TString histSampleName){

        sampleName = histSampleName;
        TTreeFormula *varFormula = new TTreeFormula("form2",plotInfo->var, tree);

        selection = numSelection;
        weight = numWeight;
        TString nameN = getFullName();
        TString selN = getSelString();
        TH1F * hN = plotInfo->getHistogram(nameN);
        hN->Sumw2();

        TTreeFormula *selNFormula = new TTreeFormula("form1",selN, tree);

        selection = denSelection;
        weight = denWeight;
        TString nameD = getFullName();
        TString selD = getSelString();
        TH1F * hD = plotInfo->getHistogram(nameD);
        hD->Sumw2();

        TTreeFormula *selDFormula = new TTreeFormula("form2",selD, tree);


        vector<TTreeFormula *> bootFormulas(nBootStraps);

        TH2F *hbN = 0;
        TH2F *hbD = 0;

        if(nBootStraps){
          TString nameS = plotInfo->name += "_bN";
          hbN  = plotInfo->getHistogram2D(nameS,nBootStraps);
          nameS = plotInfo->name += "_bD";
          hbD  = plotInfo->getHistogram2D(nameS,nBootStraps);

          for(unsigned int iB = 0; iB < nBootStraps; ++iB){
            bootFormulas[iB] = new TTreeFormula(TString::Format("boostForm_%u",iB),TString::Format("bootstrapWeight[%u]",iB), tree);
          }
        }


        int nEntries = tree->GetEntries();
        for (int i= 0;i<nEntries;i++) {
         tree->GetEntry(i);
         double var      = varFormula->EvalInstance();
         double weightN   = selNFormula->EvalInstance();
         double weightD   = selDFormula->EvalInstance();
         if(weightD == 0 && weightN == 0) continue;

         if(weightN) hN->Fill(var,weightN);
         if(weightD) hD->Fill(var,weightD);

         for(unsigned int iB = 0; iB < nBootStraps; ++iB){
           bootFormulas[iB]->GetNdata();
           double bootWeight = bootFormulas[iB]->EvalInstance();
           if(bootWeight && weightN)
             hbN->Fill(var,iB,weightN*bootWeight);
           if(bootWeight && weightD)
             hbD->Fill(var,iB,weightD*bootWeight);
         }
        }

       delete selNFormula;
       delete selDFormula;
       delete varFormula;
       for(unsigned int iB = 0; iB < nBootStraps; ++iB)
         delete bootFormulas[iB];

       if(underflow){
         PlotTools::toUnderflow(hN);
         PlotTools::toUnderflow(hD);
         if(nBootStraps){
           PlotTools::toUnderflowX(hbN);
           PlotTools::toUnderflowX(hbD);
         }
       }
       if(overflow){
         PlotTools::toOverflow(hN);
         PlotTools::toOverflow(hD);
         if(nBootStraps){
           PlotTools::toOverflowX(hbN);
           PlotTools::toOverflowX(hbD);
         }
       }

       hN->Divide(hD);

       if(nBootStraps){
         hbN->Divide(hbD);
         const unsigned int nBinsX = hbN->GetNbinsX();

         vector<double> means(nBinsX +2,0);
         vector<vector<double> > meanSqs(nBinsX +2, vector<double>(nBinsX +2,0)      );

         for(unsigned int iBS = 0; iBS < nBootStraps; ++iBS){
           for(unsigned int iB = 0; iB <= nBinsX +1; ++iB){
             means[iB] += hbN->GetBinContent(iB,iBS+1);
             for(unsigned int iB2 = 0; iB2 <= nBinsX +1; ++iB2){
               meanSqs[iB][iB2] += hbN->GetBinContent(iB,iBS+1)*hbN->GetBinContent(iB2,iBS+1);
             }
           }
         }

         vector<vector<double> > covMatrix(nBinsX +2, vector<double>(nBinsX +2,0)      );
         for(unsigned int iB = 0; iB <= nBinsX +1; ++iB){
           for(unsigned int iB2 = 0; iB2 <= nBinsX +1; ++iB2){
             covMatrix[iB][iB2] = meanSqs[iB][iB2]/double(nBootStraps) - (means[iB]/double(nBootStraps) )*(means[iB2]/double(nBootStraps)) ;
             cout << covMatrix[iB][iB2] <<" ";
           }
           cout << endl;
         }
         cout << endl <<"Norm!"<<endl;
         for(unsigned int iB = 0; iB <= nBinsX +1; ++iB){
           for(unsigned int iB2 = 0; iB2 <= nBinsX +1; ++iB2){
             cout << TString::Format("%.2f",(covMatrix[iB][iB] == 0 ? 0 : covMatrix[iB][iB2]/covMatrix[iB][iB])) << " ";
           }
           cout << endl;
         }

         cout << endl;



         for(unsigned int iB = 0; iB <= nBinsX +1; ++iB){
           hN->SetBinError(iB,TMath::Sqrt(covMatrix[iB][iB]) );
         }
         delete hbN;
         delete hbD;
       }

       return hN;
      }


  TH1F * HistogramGetter::getHistogram(TTree* tree){
    if(nBootStraps > 0){
      getHistogramManual(tree);
    }

    TString name = getFullName();
    TString sel = getSelString();
    TH1F * h = plotInfo->getHistogram(name);
    h->Sumw2();

    tree->Draw(TString::Format("%s>>+%s",plotInfo->var.Data(),name.Data()),sel,"goff");

    if(underflow)
      PlotTools::toUnderflow(h);
    if(overflow)
      PlotTools::toOverflow(h);

    return h;
  }

  TH1F*  HistogramGetter::getHistogramManual(TTree * tree){

      TString name = getFullName();
      TString sel = getSelString();
      TH1F * h = plotInfo->getHistogram(name);
      h->Sumw2();

      TTreeFormula *selFormula = new TTreeFormula("form1",sel, tree);
      TTreeFormula *varFormula = new TTreeFormula("form2",plotInfo->var, tree);

      vector<TTreeFormula *> bootFormulas(nBootStraps);

      TH2F *hb = 0;

      if(nBootStraps){
        TString nameS = plotInfo->name += "_b";
        hb  = plotInfo->getHistogram2D(nameS,nBootStraps);

        for(unsigned int iB = 0; iB < nBootStraps; ++iB){
          bootFormulas[iB] = new TTreeFormula(TString::Format("boostForm_%u",iB),TString::Format("bootstrapWeight[%u]",iB), tree);
        }
      }


      int nEntries = tree->GetEntries();
      for (int i= 0;i<nEntries;i++) {
       tree->GetEntry(i);
       double var      = varFormula->EvalInstance();
       double weight   = selFormula->EvalInstance();
       if(weight == 0) continue;

       h->Fill(var,weight);

       for(unsigned int iB = 0; iB < nBootStraps; ++iB){
         bootFormulas[iB]->GetNdata();
         double bootWeight = bootFormulas[iB]->EvalInstance();
         if(bootWeight)
           hb->Fill(var,iB,weight*bootWeight);
       }
      }

     delete selFormula;
     delete varFormula;
     for(unsigned int iB = 0; iB < nBootStraps; ++iB)
       delete bootFormulas[iB];

     if(underflow){
       PlotTools::toUnderflow(h);
       if(nBootStraps){
         PlotTools::toUnderflowX(hb);
       }
     }
     if(overflow){
       PlotTools::toOverflow(h);
       if(nBootStraps){
         PlotTools::toOverflowX(hb);
       }
     }

     if(nBootStraps){
       const unsigned int nBinsX = h->GetNbinsX() ;
       for(unsigned int iB = 0; iB <= nBinsX +1; ++iB){
         double s = 0;
         double ss = 0;

         for(unsigned int iBS = 0; iBS < nBootStraps; ++iBS){
           s += hb->GetBinContent(iB,iBS+1);
           ss += hb->GetBinContent(iB,iBS+1)*hb->GetBinContent(iB,iBS+1);
         }
         double stdDev = TMath::Sqrt(nBootStraps*ss - s*s)/nBootStraps;
         h->SetBinError(iB,stdDev);
       }
       delete hb;
     }

     return h;
    }

  TString HistogramGetter::getFullName(){
    if(sampleName != "")
      return TString::Format("%s_%s_%i",sampleName.Data(), plotInfo->name.Data(), plotInfo->getN());
    else
      return TString::Format("%s_%i",plotInfo->name.Data(), plotInfo->getN());
  }

  TString HistogramGetter::getSelString() {
    TString selString("");
    if(weight != ""){
      selString = weight;
      if(selection != "")
        selString += TString::Format("*(%s)",selection.Data());
    } else if (selection != ""){
      selString = selection;
    }

    return selString;

  }


