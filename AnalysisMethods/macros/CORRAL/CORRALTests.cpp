// ---------------------------------------------------------------------
//
//     Test CORRAL
//
// ---------------------------------------------------------------------
{
    Plot::cache("*_testCORRAL_plots.root");
    TString vars[] = {"topPairInvMass","topVecPT","topDPHI","topPairDisc",""};
    // TString vars[] = {"topPairInvMass","leadTopMass","subleadTopMass","topHT","topVecPT","topDPHI","leadTopDisc","subleadTopDisc","topPairDisc",""};
    
    TString presel = "noEMuTau__nTopPairs_geq1__";
    o = new TObjArray;
        for(unsigned int iV = 0; vars[iV][0]; ++iV){
            p = new Plot(presel + vars[iV] + "$");
            p->toUnderOverflow();
            p->normalize();
            p->rebin(4);
            o->Add(p);
        }
        
        Pint::drawAll(o,"");
        
    
}        

{
    Plot::cache("*_testCORRAL_plots.root");
    pI = new Plot("noEMuTau__nTopPairs_incl__nEvents");
    pT = new Plot("noEMuTau__nTopPairs_geq1__nEvents");
    pP = new Plot("noEMuTau__nTopPairs_geq1_pure__nEvents");
    
    pT->divide(*pI);
    new TCanvas();
    pT->draw();
    pP->divide(*pI);
    new TCanvas();
    pP->draw();
    
}

root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testCORRAL.C+("sigSamples/T2tt_850_100.root" ,"TestAnalyzer/Events","T2tt_850_100_testCORRAL_plots.root")'   &
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testCORRAL.C+("sigSamples/T2tt_650_325.root" ,"TestAnalyzer/Events","T2tt_650_325_testCORRAL_plots.root")'   &
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testCORRAL.C+("sigSamples/ttbar_metgeq200.root" ,"TestAnalyzer/Events","ttbar_testCORRAL_plots.root")'   &


// ---------------------------------------------------------------------
//
//     Test CORRAL in SD
//
// ---------------------------------------------------------------------
    
    nohup root -b -q 'CORRAL/CORRALWriter.C+("metskim/ttbar_ntuple_metSkim.root"           ,"Events","corral")' &
    nohup root -b -q 'CORRAL/CORRALWriter.C+("metskim/znunu_ht600toinf_ntuple_metSkim.root","Events","corral")' &
    nohup root -b -q 'CORRAL/CORRALWriter.C+("metskim/znunu_ht400to600_ntuple_metSkim.root","Events","corral")' &
    nohup root -b -q 'CORRAL/CORRALWriter.C+("metskim/znunu_ht200to400_ntuple_metSkim.root","Events","corral")' &
    nohup root -b -q 'CORRAL/CORRALWriter.C+("metskim/znunu_ht100to200_ntuple_metSkim.root","Events","corral")' &
    nohup root -b -q 'CORRAL/CORRALWriter.C+("metskim/T2tt_425_325_ntuple_metSkim.root"    ,"Events","corral")' &
    nohup root -b -q 'CORRAL/CORRALWriter.C+("metskim/ttZ_ntuple_metSkim.root"             ,"Events","corral")' &
    nohup root -b -q 'CORRAL/CORRALWriter.C+("metskim/T2tt_850_100_ntuple_metSkim.root"    ,"Events","corral")' &
    nohup root -b -q 'CORRAL/CORRALWriter.C+("metskim/T2tt_650_325_ntuple_metSkim.root"    ,"Events","corral")' &
    nohup root -b -q 'CORRAL/CORRALWriter.C+("metskim/T2tt_500_325_ntuple_metSkim.root"    ,"Events","corral")' &
        
        root -b -q 'CORRAL/testCORRALInSD.C+("corralskim/SM_ntuple_metSkim_corral.root","Events","SM_corralInSD.root")' &
        root -b -q 'CORRAL/testCORRALInSD.C+("corralskim/T2tt_850_100_ntuple_metSkim_corral.root","Events","T2tt_850_100_corralInSD.root")' &
        root -b -q 'CORRAL/testCORRALInSD.C+("corralskim/T2tt_650_325_ntuple_metSkim_corral.root","Events","T2tt_650_325_corralInSD.root")' &
            

{
    Plot::cache("*_corralInSD.root");
    TString METS[] = {"met_geq200","","met_eq200to300","met_eq300to400","met_geq400",""};
    // TString VARS[] = {"ak4_MT2","picky_MT2","corral_MT2",""}
    TString VARS[] = {"topPairDisc","maxLead","maxSubLead","leadTopPT","subleadTopPT",""}
    o = new TObjArray;
    oo = new TObjArray;
        ooo = new TObjArray;
    for(unsigned int iV = 0; VARS[iV][0]; ++iV){
    for(unsigned int iM = 0; METS[iM][0]; ++iM){
        p = new Plot("_"+METS[iM]+"__nTopPairs_geq1__"+ VARS[iV]);
        p->rebin(4);
        p->toUnderOverflow();
        p->SetTitle(Plot::translated(METS[iM]));
        o->Add(p);
        p =(Plot*)p->Clone();
        p->normalize();
        oo->Add(p);     
        p =(Plot*)p->Clone();           
        p = p->makeIntegral(true);
        ooo->Add(p);
    }
}
    Pint::drawAll(o,"");
    Pint::drawAll(oo,"");
    Pint::drawAll(ooo,"");
    
}

{
    Plot::cache("*_corralInSD.root");
    TString PRC[] = {"SM_ttbar__","SM_znunu__","SM_ttz__","T2tt_850_100_",""};
    // TString presel = "met_geq200__nTopPairs_incl__";
    TString presel = "met_geq200__nTopPairs_geq1__";
    TString VARS[] = {"topPairDiscvak4_MT2","topPairDiscvpicky_MT2","topPairDiscvcorral_MT2",""};


    for(unsigned int iV = 0; VARS[iV][0]; ++iV){
    o = new TObjArray;        
    for(unsigned int iP = 0; PRC[iP][0]; ++iP){
        TH2 * h = (TH2*)Plot::getCache()->findOne(PRC[iP]+presel + VARS[iV]);
        if(h == 0) continue;
        Procedures::toUnderOverflow(h);
        Procedures::normalize(h);
        h->SetTitle(Plot::translated(PRC[iP] + VARS[iV]));
        o->Add(h);
    }
        Pint::drawAll(o,"","COLZ");
}

    
}