#ifndef QCDPREDICTIONHELPER_H
#define QCDPREDICTIONHELPER_H
#include "TTree.h"
#include<iostream>
#include<regex>
#include<assert.h>
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"



namespace QCDSupport {

TTree * getTree(TString filename){
  TFile * sf = new TFile(filename,"read");
  TTree * st =0;
  sf->GetObject("Events",st);
  return st;
}


void setTitleOffset(TCanvas *c, double xOff = .950, double yOff = 1.400){
  TList * list = c->GetListOfPrimitives();
  for(unsigned int iP = 0; iP < list->GetSize(); ++iP){
    TH1 * h = dynamic_cast<TH1*>(list->At(iP));
    if(h == 0) continue;
    h->GetXaxis()->SetTitleOffset(xOff);
    h->GetYaxis()->SetTitleOffset(yOff);
  }
}


  const TString header = "#sqrt{s} = 13 TeV, L = 2.3 fb^{-1}";
  const TString METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 250 && j2pt >= 75";
  const TString ResTailExtraCuts = "(dphij12met < .1 || dphij3met < .1) && nvetolep == 0 && pseudoRespPassFilter == 1";
  const TString BaselineExtraCuts = "njets >= 5 && nlbjets>= 2 && nbjets>=1";
  const TString stdWeight = "1.0";
  const TString stdMCWeight = "weight*truePUWeight*btagWeight*2.263";
  const TString stdQCDWeight = "weight*truePUWeight*btagWeight*qcdRespTailWeight*2.263";
  const TString topMCWeight = "cttWeight";

  TString processWeight(TString weight, int iCR) {return iCR == 4 ? TString::Format("%s*%s",weight.Data(),topMCWeight.Data()) : weight;}
  const int nMETBins = 5;
  const double metBins[nMETBins+1] = {250,300,400,500,600,700};

//  const int nMETBins = 1;
//  const double metBins[nMETBins+1] = {250,700};
  HistogramGetter * metGetter = new HistogramGetter("met","met","#slash{#it{E}}_{T} [GeV]", nMETBins,metBins);

  struct CRegInfo {
    static const int nCR = 5;
    enum CRReg{ CR_5_1_0_0, CR_7_1_0_0,CR_5_1_0_175, CR_7_1_0_175, CR_5_1_1_175 };
    TString  crRegBinNames[nCR] = { "5_1_0_0", "7_1_0_0","5_1_0_175", "7_1_0_175", "5_1_1_175"};
    TString crSel[nCR] = {
        "mtcsv12met < 175 && (njets == 5 || njets == 6)",
        "mtcsv12met < 175 && njets >= 7",
        "mtcsv12met >= 175 && (njets == 5 || njets == 6) && ncttstd == 0",
        "mtcsv12met >= 175 && njets >= 7 && ncttstd == 0",
        "mtcsv12met >= 175 && ncttstd >= 1"
    };
      TString crSelNames[nCR] = {"#it{m}_{T,b} < 175, nJ 5-6","#it{m}_{T,b}< 175, nJ #geq7",
          "#it{m}_{T,b} #geq 175, nJ 5-6, nT 0","#it{m}_{T,b} #geq 175, nJ #geq7, nT 0",
          "#it{m}_{T,b} #geq 175, nT #geq1"
      };
    TString crPreData        =  TString::Format("%s && %s && nvetotau == 0 && nvetolep == 0 && (dphij12met < .1 || dphij3met < .1)", METPresel.Data(), BaselineExtraCuts.Data());
    TString crPreOther       =  TString::Format("%s && %s && nvetolep==0 && (nvetotau==0 || npromptgentau>0) && (dphij12met < .1 || dphij3met < .1)", METPresel.Data(), BaselineExtraCuts.Data());
    TString crPreQCD         =  TString::Format("%s && %s && (dphij12met < .1 || dphij3met < .1)", METPresel.Data(), BaselineExtraCuts.Data());
    TString crPreQCDWithVeto =  TString::Format("%s && %s && nvetotau == 0 && nvetolep == 0 && (dphij12met < .1 || dphij3met < .1)", METPresel.Data(), BaselineExtraCuts.Data());

    TString crWgtOther       =  TString::Format("%s*leptnpweight*lepvetoweight", stdMCWeight.Data());

    TString normSel          = TString::Format("%s && %s && nvetolep > 0 && mtlepmet<100 && dphij12met>0.5 && dphij3met>0.5 && dphij4met>0.5", METPresel.Data(), BaselineExtraCuts.Data());
    TString normMCWT         = TString::Format("%s*leptnpweight", stdMCWeight.Data());



    std::vector<TH1*> otherBKGSFs;
    TTree *otherBKGTree;
    TTree *dataTree;
    TTree *qcdTree;
    void fillOtherBkgSFs(TString dataTreeIn, TString otherBKGTreeIn, TString qcdTreeIn){
      fillOtherBkgSFs(getTree(dataTreeIn),getTree(otherBKGTreeIn),getTree(qcdTreeIn));
    }
    void fillOtherBkgSFs(TTree * dataTreeIn, TTree * otherBKGTreeIn, TTree * qcdTreeIn){
      otherBKGTree = otherBKGTreeIn;
      dataTree = dataTreeIn;
      qcdTree = qcdTreeIn;

      double bins2[] = {250,300,400,700};
      HistogramGetter SFHG("SFmet","met","#slash{#it{E}}_{T} [GeV]", 3,bins2);
      HistogramGetter SFIncHG("SFmet2","met","#slash{#it{E}}_{T} [GeV]", 1, 250, 700);

      for(unsigned int iS = 0; iS < nCR; ++iS){
        TString sel = TString::Format("%s && %s",normSel.Data(),crSel[iS].Data());
        TH1F * dataH  = SFIncHG/*(iS == 4?SFIncHG:SFHG)*/ .getHistogram(dataTree,sel,"1.0","Data");
        TH1F * otherH = SFIncHG/*(iS == 4?SFIncHG:SFHG)*/ .getHistogram(otherBKGTree,sel,processWeight(normMCWT,iS),"Other");
        TH1F * qcdH   = SFIncHG/*(iS == 4?SFIncHG:SFHG)*/ .getHistogram(qcdTree,sel,processWeight(normMCWT,iS),"MC");
        otherH->Add(qcdH);
//        std::cout <<std::endl<< dataH->GetBinContent(1) <<" "<< otherH->GetBinContent(1) <<" "<< dataH->GetBinContent(1)/otherH->GetBinContent(1)<<std::endl;
        dataH->Divide(otherH);

        TH1 * h = new TH1F(TString::Format("ttbarwSF_%u",iS),";SF",nMETBins,metBins);
        for(unsigned int iB = 1; iB <= h->GetNbinsX(); ++iB){
          double metV = h->GetBinCenter(iB);
          double sf = dataH->GetBinContent( dataH->FindFixBin(metV) );
          h->SetBinContent(iB,sf);
        }

//        std::cout <<"ttbar+W norm(" <<iS<<"): ";
//        for(unsigned int iB = 1; iB <= h->GetNbinsX(); ++iB) std::cout << h->GetBinContent(iB)<<"\t";
//        std::cout << std::endl;
        otherBKGSFs.push_back(h);
      }
    }
    std::vector<TH1*> otherBKGScaledYields;
    void fillCorrectedOtherBkgYields(){
      for(unsigned int iS = 0; iS < nCR; ++iS){
        TString sel = TString::Format("%s && %s",crPreOther.Data(),crSel[iS].Data());
        TH1 * h = metGetter->getHistogram(otherBKGTree,sel,processWeight(crWgtOther,iS),TString::Format("tree_%u",0));
        for(unsigned int iB =1; iB <=h->GetNbinsX(); ++iB){
          h->SetBinContent(iB,h->GetBinContent(iB) *otherBKGSFs[iS]->GetBinContent(iB) );
          h->SetBinError(iB,h->GetBinError(iB) *otherBKGSFs[iS]->GetBinContent(iB) );
        }
        otherBKGScaledYields.push_back((TH1*)h->Clone());
      }
    }

    std::vector<TH1*> dataYields;
    std::vector<TH1*> qcdYieldsWithVeto;
    std::vector<TH1*> otherBKGCorr;
    std::vector<TH1*> otherBKGCorrUnc;
    void fillDataCorr(){
      for(unsigned int iS = 0; iS < nCR; ++iS){
        TH1 * hd = metGetter->getHistogram(dataTree,TString::Format("%s && %s",crPreData.Data(),crSel[iS].Data()),"1.0",TString::Format("tree_%u",0));
        metGetter->setNBS(50);
        TH1 * hqwv = metGetter->getHistogramManual(qcdTree,TString::Format("%s && %s",crPreQCDWithVeto.Data(),crSel[iS].Data()),processWeight(stdQCDWeight,iS),TString::Format("tree_%u",0));
        metGetter->setNBS(0);
        TH1 * corr = (TH1*)hd->Clone();
        TH1 * corrU = (TH1*)hd->Clone();
        std::cout << crSelNames[iS] << std::endl;
        for(unsigned int iB =1; iB <=hd->GetNbinsX(); ++iB){
          double dV = hd->GetBinContent(iB) < 10 ?  hqwv->GetBinContent(iB) + otherBKGScaledYields[iS]->GetBinContent(iB): hd->GetBinContent(iB);
          double oV = otherBKGScaledYields[iS]->GetBinContent(iB);
          double cV = 1 - oV/dV;
          corr->SetBinContent(iB,cV );
          corrU->SetBinContent(iB,1 + oV/dV );

          std::cout << "("<<cV <<","<<1 + oV/dV  <<") ";
        }
        std::cout << std::endl;
        dataYields.push_back(hd);
        qcdYieldsWithVeto.push_back(hqwv);
        otherBKGCorr.push_back(corr);
        otherBKGCorrUnc.push_back(corrU);
      }
    }

    float getDataYield(CRReg reg, int metBin){ return dataYields[reg]->GetBinContent(metBin+1);}
    double getDataStatUnc(CRReg reg, int metBin) {
      double dY = getDataYield(reg,metBin);
      if(dY == 0) dY = 1;
      return TMath::Sqrt(dY)/dY + 1;
    };
    float getScaledOtherYield(CRReg reg, int metBin){ return otherBKGScaledYields[reg]->GetBinContent(metBin+1);}
    float getScaledOtherYieldStatUnc(CRReg reg, int metBin){ return otherBKGScaledYields[reg]->GetBinError(metBin+1);}
    float getDataYieldCorr(CRReg reg, int metBin){ return otherBKGCorr[reg]->GetBinContent(metBin+1);}
    float getDataYieldCorrUnc(CRReg reg, int metBin){ return otherBKGCorrUnc[reg]->GetBinContent(metBin+1);}
    TString getBinName (CRReg reg, int metBin) {return  TString::Format("%.0f_%s",metBins[metBin],crRegBinNames[reg].Data());}
    TString getLatexBlock(CRReg reg, int metBin){
      if(metBin+1 == nMETBins)
        return TString::Format("$>$%.0f & %.0f & %.2f $\\pm$ %.2f",metBins[metBin],getDataYield(reg,metBin),getScaledOtherYield(reg,metBin),getScaledOtherYieldStatUnc(reg,metBin));
      else
        return TString::Format("%.0f$-$%.0f & %.0f & %.2f $\\pm$ %.2f",metBins[metBin],metBins[metBin+1],getDataYield(reg,metBin),getScaledOtherYield(reg,metBin),getScaledOtherYieldStatUnc(reg,metBin));
    }
  };


  struct SRegInfo {
    static const int nSR = 10;
    enum SRReg{ SR_5_1_0_0, SR_7_1_0_0,SR_5_2_0_0, SR_7_2_0_0,SR_5_1_0_175, SR_7_1_0_175,SR_5_2_0_175, SR_7_2_0_175,SR_5_1_1_175, SR_5_2_1_175, };
    TString  srRegBinNames[nSR] = { "5_1_0_0", "7_1_0_0","5_2_0_0", "7_2_0_0","5_1_0_175", "7_1_0_175","5_2_0_175", "7_2_0_175","5_1_1_175", "5_2_1_175"};
    TString srSel[nSR] = {
        "mtcsv12met < 175 && nbjets == 1 && (njets == 5 || njets == 6)",
        "mtcsv12met < 175 && nbjets == 1 && njets >= 7",
        "mtcsv12met < 175 && nbjets >= 2 && (njets == 5 || njets == 6)",
        "mtcsv12met < 175 && nbjets >= 2 && njets >= 7",
        "mtcsv12met >= 175 && nbjets == 1 && (njets == 5 || njets == 6) && ncttstd == 0",
        "mtcsv12met >= 175 && nbjets == 1 && njets >= 7 && ncttstd == 0",
        "mtcsv12met >= 175 && nbjets >= 2 && (njets == 5 || njets == 6) && ncttstd == 0",
        "mtcsv12met >= 175 && nbjets >= 2 && njets >= 7 && ncttstd == 0",
        "mtcsv12met >= 175 && nbjets == 1 && ncttstd >= 1",
        "mtcsv12met >= 175 && nbjets >= 2  && ncttstd >= 1"
    };
      TString srSelNames[nSR] = {
          "#it{m}_{T,b} < 175, nb=1, nJ 5-6",
          "#it{m}_{T,b}< 175, nb=1, nJ #geq7",
          "#it{m}_{T,b} < 175, nb#geq2, nJ 5-6",
          "#it{m}_{T,b}< 175, nb#geq2, nJ #geq7",
          "#it{m}_{T,b} #geq 175,nb=1, nJ 5-6, nT 0",
          "#it{m}_{T,b} #geq 175,nb=1, nJ #geq7, nT 0",
          "#it{m}_{T,b} #geq 175,nb=2, nJ 5-6, nT 0",
          "#it{m}_{T,b} #geq 175,nb=2, nJ #geq7, nT 0",
          "#it{m}_{T,b} #geq 175,nb=1, nT #geq1",
          "#it{m}_{T,b} #geq 175,nb#geq2, nT #geq1"
      };
    TString srPreQCD         =   TString::Format("%s && %s && (dphij12met >= 0.5  && dphij3met >= 0.5 && dphij4met >= 0.5 )", METPresel.Data(), BaselineExtraCuts.Data());


    CRegInfo::CRReg crRegs[nSR] = {
        CRegInfo::CR_5_1_0_0  ,
        CRegInfo::CR_7_1_0_0  ,
        CRegInfo::CR_5_1_0_0  ,
        CRegInfo::CR_7_1_0_0  ,
        CRegInfo::CR_5_1_0_175,
        CRegInfo::CR_7_1_0_175,
        CRegInfo::CR_5_1_0_175,
        CRegInfo::CR_7_1_0_175,
        CRegInfo::CR_5_1_1_175,
        CRegInfo::CR_5_1_1_175
    };

    TString processWeight(TString weight, int iSR) {return iSR >= 8 ? TString::Format("%s*%s",weight.Data(),topMCWeight.Data()) : weight;}

    std::vector<TH1F*> nominalQCD;
    std::vector<TH1F*> respUp;
    std::vector<TH1F*> respDown;
    std::vector<TH1F*> noResp;
    std::vector<TH1F*> origQCD;
    TTree *qcdTree;
    TTree *origQcdTree;
    void fillTFVariations( TString qcdTreeIn,TString origTreeIn){
      fillTFVariations(getTree(qcdTreeIn),getTree(origTreeIn));

    }
    void fillTFVariations(TTree* qcdTreeIn,TTree * origTreeIn){
      qcdTree = qcdTreeIn;
      origQcdTree = origTreeIn;
      CRegInfo crInfo;
      for(unsigned int iS = 0; iS < nSR; ++iS){
        TString selN = TString::Format("%s && %s",srPreQCD.Data(),srSel[iS].Data());
        TString selD = TString::Format("%s && %s",crInfo.crPreQCD.Data(),crInfo.crSel[crRegs[iS]].Data());

        metGetter->setNBS(50);
        TH1F * nom = metGetter->getTFAndCov(qcdTree,selN,processWeight(stdQCDWeight,iS),selD,processWeight(stdQCDWeight,iS),TString::Format("ratio_%u",iS));
        metGetter->setNBS(0);

        TH1F * upN    = metGetter->getHistogram(qcdTree,selN,TString::Format("%s*upTailWeight",processWeight(stdMCWeight,iS).Data()),"QCD");
        TH1F * downN  = metGetter->getHistogram(qcdTree,selN,TString::Format("%s*downTailWeight",processWeight(stdMCWeight,iS).Data()),"QCD");
        TH1F * norN   = metGetter->getHistogram(qcdTree,selN,processWeight(stdMCWeight,iS),"QCD");
        TH1F * orN    = origQcdTree ? metGetter->getHistogram(origQcdTree,selN,processWeight(stdMCWeight,iS),"QCD") : 0;

        TH1F * upD    = metGetter->getHistogram(qcdTree,selD,TString::Format("%s*upTailWeight",processWeight(stdMCWeight,iS).Data()),"QCD");
        TH1F * downD  = metGetter->getHistogram(qcdTree,selD,TString::Format("%s*downTailWeight",processWeight(stdMCWeight,iS).Data()),"QCD");
        TH1F * norD   = metGetter->getHistogram(qcdTree,selD,processWeight(stdMCWeight,iS),"QCD");
        TH1F * orD    = origQcdTree ? metGetter->getHistogram(origQcdTree,selD,processWeight(stdMCWeight,iS),"QCD") : 0;

        upN   ->Divide(upD);
        downN ->Divide(downD);
        norN  ->Divide(norD);
        if(orN) orN   ->Divide(orD);

        nominalQCD.push_back(nom);
        respUp    .push_back(upN  );
        respDown  .push_back(downN);
        noResp    .push_back(norN );
        origQCD   .push_back(orN  );
    }
    }
    std::string convertToTemplateName(std::string inBinName, int metV){
      std::regex e ("(\\d+)_(\\d+)_(\\d+)_(\\d+)");
      std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
      bool matched = std::regex_match (inBinName,sm,e);

      assert(matched);
      assert(sm.size() == 5);
  //    cout << inBinName << endl;
  //    cout << sm.size() << endl;
//      for(unsigned int i = 0; i < sm.size(); ++i) std::cout <<"["<<sm[i]<<"] ";
//      std::cout << std::endl;;

      return  "template_met" + std::to_string(metV)+"_njets"+sm[1].str() +"_nbjets"+sm[2].str()+"_ncttstd"+sm[3].str()+"_mtcsv12met"+sm[4].str()+".txt_sr_template";
    }

    void getQCDUncertainties(std::string fIn, std::vector<std::string>& uncNames,std::vector<float>& uncs){
      uncs.clear();
      uncNames.clear();

      std::string line;
      std::ifstream inFile (fIn);
      if(!inFile.is_open())std::cout <<"Could not find: " << fIn << std::endl;
      assert(inFile.is_open());
      std::regex e ("^(\\S+)\\s+lnN\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+(\\S+)\\s*$");
      std::smatch sm;

      while(getline(inFile,line)){
        if (std::regex_match (line,sm,e)){
          assert(sm.size() == 3);
          bool isGood = true;
          float unc = -1;
          try {unc = stof(sm[2]);}catch (const std::invalid_argument& ia) {isGood = false;}
          if(TString(sm[1]).BeginsWith("qcd_")) isGood = false;
          if(isGood){
            uncs.push_back(unc);
            uncNames.push_back(sm[1]);
          }
        }
      }
    }

    double getQCDCardYields(std::string fIn){
      std::string line;
      std::ifstream inFile (fIn);
      if(!inFile.is_open())std::cout <<"Could not find: " << fIn << std::endl;
      assert(inFile.is_open());
      std::regex e ("^(\\S+)\\s+gmN\\s+(\\S+)\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+(\\S+)\\s*$");
      std::smatch sm;

      while(getline(inFile,line)){
        if (std::regex_match (line,sm,e)){
          assert(sm.size() == 3);
          bool isGood = true;
          float data = -1;
          float tf = -1;
          try {data = stof(sm[2]);}catch (const std::invalid_argument& ia) {isGood = false;}
          try {tf = stof(sm[3]);}catch (const std::invalid_argument& ia) {isGood = false;}
          if(!TString(sm[1]).BeginsWith("qcd_")) isGood = false;
          if(isGood){
            return tf*data;
          }
        }
      }
      return -1;
    }


    std::vector<std::vector<double> > totalExtSystUncs = std::vector<std::vector<double> >(nSR,std::vector<double>(nMETBins,0));
    void loadExternUnc(std::string dcDir){
      std::cout << "Processing ext. uncs: "<< dcDir <<std::endl;
      int nUncs = -1;
      for(unsigned int iS = 0; iS < nSR; ++iS)
        for(unsigned int iM = 0; iM < nMETBins; ++iM){
          std::vector<float> uncs;
          std::vector<std::string> uncNames;
          getQCDUncertainties(dcDir+convertToTemplateName(srRegBinNames[iS].Data(),metBins[iM]),uncNames,uncs); //
//          std::cout << getBinName(SRReg(iS),iM) << std::endl;
          for(unsigned int iU = 0; iU < uncs.size(); ++iU) std::cout << uncNames[iU] <<" -> "<< uncs[iU]<<std::endl;
          if(nUncs < 0){
            for(unsigned int iU = 0; iU < uncs.size(); ++iU) std::cout << uncNames[iU] <<" -> "<< uncs[iU]<<std::endl;
            nUncs = uncs.size();
          }
          assert(nUncs == uncs.size());
          double totalUnc =0;
          for(unsigned int iU = 0; iU < nUncs; ++iU){
            std::cout << fabs(uncs[iU]-1)*fabs(uncs[iU]-1) <<" ";
            totalUnc += fabs(uncs[iU]-1)*fabs(uncs[iU]-1);
          }
//          std::cout << std::endl << sqrt(totalUnc) + 1 <<std::endl;
          totalExtSystUncs[iS][iM]= sqrt(totalUnc) + 1;
        }
    }

    double getTF(SRReg reg, int metBin){return nominalQCD[reg]->GetBinContent(metBin+1);}
    double getTFStatUnc(SRReg reg, int metBin){return std::min(2.0,nominalQCD[reg]->GetBinError(metBin+1)/nominalQCD[reg]->GetBinContent(metBin+1)+1);}
    double getRespUnc(SRReg reg, int metBin){return (respUp[reg]->GetBinContent(metBin+1) - respDown[reg]->GetBinContent(metBin+1) ) /(2*getTF(reg,metBin)) +1;}
    double getPrediction(CRegInfo* crinfo, SRReg reg, int metBin){
      double dY = crinfo->getDataYield(crRegs[reg],metBin) == 0 ? 1: crinfo->getDataYield(crRegs[reg],metBin);
      return dY*crinfo->getDataYieldCorr(crRegs[reg],metBin)*getTF(reg,metBin);
    }
    double getExtUncs(SRReg reg, int metBin){ return totalExtSystUncs[reg][metBin];}
    TString getBinName (SRReg reg, int metBin) {return  TString::Format("%.0f_%s",metBins[metBin],srRegBinNames[reg].Data());}
    double getTotalSystUnc(CRegInfo * crinfo,SRReg reg, int metBin){
      return TMath::Sqrt(   (getTFStatUnc(reg,metBin) -1)*(getTFStatUnc(reg,metBin) -1)
                          + (getRespUnc(reg,metBin) -1)*(getRespUnc(reg,metBin) -1)
                          + (getExtUncs(reg,metBin) -1)*(getExtUncs(reg,metBin) -1)
                          + (crinfo->getDataYieldCorrUnc(crRegs[reg],metBin)-1)*(crinfo->getDataYieldCorrUnc(crRegs[reg],metBin)-1)
                          ) + 1;
    }


    TString getLatexBlock (CRegInfo* crinfo, SRReg reg, int metBin) {
      double tf       = getTF(reg,metBin);
      double tfUnc    = tf*(getTFStatUnc(reg,metBin) - 1);
      double pred     = getPrediction(crinfo,reg,metBin);
      double predstat = pred*(crinfo->getDataStatUnc(crRegs[reg],metBin) - 1);
      double predsyst = pred*(getTotalSystUnc(crinfo,reg,metBin) - 1);

      if(tf < .0005 ) return TString::Format(" $<$0.001 & - ");
      return TString::Format("%.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f (stat.) $\\pm$ %.3f (syst.)",tf,tfUnc,pred,predstat,predsyst );
    }
  };



};






#endif
