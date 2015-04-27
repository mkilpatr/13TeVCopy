// ---------------------------------------------------------------------
//
//     W Jet Likilihood
//
// ---------------------------------------------------------------------
{
    
    double bins[] = {20,40,60,100,200,400,800};
    RebinToNewBins b(6,bins);

    Plot::cache("*_wJetLikli_plots.root");
        o = new TObjArray();
    pb = new Plot("W_jet__pt$");
    pn = new Plot("O_jet__pt$");
    pn->toUnderOverflow();
    pb->toUnderOverflow();    
    pb->SetTitle("W jets");
    o->Add(pb);
    pn->SetTitle("non top jets");
    o->Add(pn);
    pbr = (Plot*)pb->Clone();
    pbr->divide(*pn);
    o->Add(pbr);
    
    pn = (Plot*)pn->Clone();
    pb = (Plot*)pb->Clone();
    pn->rebin(b);
    pb->rebin(b);
    pbr = (Plot*)pb->Clone();
    pbr = pbr->divide(*pn);
    o->Add(pn);
    o->Add(pb);
    o->Add(pbr);
    
    
    Pint::drawAll(o,"");
    
}
{

    Plot::cache("*_wJetLikli_plots.root");

        TString vars[] = {"ptD","axis1","axis2","mult","area","mass","betastar","mva",""};
        TString sample = "^merged_picky_";
                // TString sample = "^650_325_picky_";
                // TString sample = "^850_100_picky_";
    
    o = new TObjArray;
    oo = new TObjArray;
    ooo = new TObjArray;
    
    for(unsigned int iV = 0; vars[iV][0]; ++iV){
        pb = new Plot(sample+"W_jet__.*__"+ vars[iV]+"$");
        pn = new Plot(sample+"O_jet__.*__"+ vars[iV]+"$");
        pb->toUnderOverflow();
        pn->toUnderOverflow();
        pb->normalize();
        pn->normalize();
        // pn->rebin(5);
        // pb->rebin(5);
        pb->SetTitle("W jet");
        o->Add(pb);
        pn->SetTitle("non top jet");
        oo->Add(pn);
        
        TCanvas * c = pn->makeCutEfficiencies(*pb,"W jet","non top jet");
        ooo->Add(c);        
    }
    Pint::drawAll(o,"W jet","",":mr");
    Pint::drawAll(oo,"non top jet","",":mr");
    Pint::drawAll(ooo,"ROC");

}



root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testWJetLikliVars.C+("sigSamples/T2tt_merged.root","TestAnalyzer/Events","T2tt_merged_wJetLikli_plots.root")'   &
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testWJetLikliVars.C+("sigSamples/T2tt_650_325.root","TestAnalyzer/Events","T2tt_650_325_wJetLikli_plots.root")' &
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testWJetLikliVars.C+("sigSamples/T2tt_850_100.root","TestAnalyzer/Events","T2tt_850_100_wJetLikli_plots.root")'  &

root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/flattenWJetLikliTree.C+("sigSamples/T2tt_merged.root","TestAnalyzer/Events","T2tt_merged_wJetLikli_tree.root"))' &
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/produceWJetLikliMVA.C+("T2tt_merged_wJetLikli_tree.root","Events","mva","T2tt_merged_wJetLikli_disc.root")' &

// ---------------------------------------------------------------------
//
//     W Candidates
//
// ---------------------------------------------------------------------

{
    
    double bins[] = {0,100,200,300,800};
    RebinToNewBins b(4,bins);

    Plot::cache("*_wCand_plots.root");
        o = new TObjArray();
    pb = new Plot("picky_W__pt$");
    pn = new Plot("picky_non_W__pt$");
    pn->toUnderOverflow();
    pb->toUnderOverflow();    
    pb->SetTitle("W");
    o->Add(pb);
    pn->SetTitle("non W");
    o->Add(pn);
    pbr = (Plot*)pb->Clone();
    pbr->divide(*pn);
    o->Add(pbr);
    
    pn = (Plot*)pn->Clone();
    pb = (Plot*)pb->Clone();
    pn->normalize();
    pb->normalize();
    pn->rebin(b);
    pb->rebin(b);
    pbr = (Plot*)pb->Clone();
    pbr = pbr->divide(*pn);
    o->Add(pn);
    o->Add(pb);
    o->Add(pbr);
    
    
    Pint::drawAll(o,"");
    
}

{
    Plot::cache("*_wCand_plots.root");
    
    // TString vars[] = {"mass","pt2opt1","wJetLikli1","wJetLikli2","maxCSV","dr","deta","charge","pullAng1","pullAng2","nWCon",""};
    TString vars[] = {"mass","pt2opt1","wJetLikli1","wJetLikli2","maxCSV","dr","deta","nWCon","mva",""};

    TString sample = "^merged_picky_";
            // TString sample = "^650_325_picky_";
            // TString sample = "^850_100_picky_";
    
    o = new TObjArray;
    oo = new TObjArray;
    ooo = new TObjArray;
    
    for(unsigned int iV = 0; vars[iV][0]; ++iV){
        pb = new Plot(sample + "W__.*__"+ vars[iV]+"$");
        pn = new Plot(sample + "non_W__.*__"+ vars[iV]+"$");
        if(pn->getNumberOfHistograms() != pb->getNumberOfHistograms()) continue;
        pb->toUnderOverflow();
        pn->toUnderOverflow();
        pb->normalize();
        pn->normalize();
        // pn->rebin(5);
        // pb->rebin(5);
        pb->SetTitle("W");
        o->Add(pb);
        pn->SetTitle("non W");
        oo->Add(pn);
        
        TCanvas * c = pn->makeCutEfficiencies(*pb,"W","non W");
        ooo->Add(c);        
    }
    Pint::drawAll(o,"W","",":mr");
    Pint::drawAll(oo,"non W","",":mr");
    Pint::drawAll(ooo,"ROC");

}

{
    Plot::cache("*_wCand_plots.root");
    
// TString sample = "^merged_picky_";
        TString sample = "^650_325_picky_";
        // TString sample = "^850_100_picky_";
    TString mva = "mva";
    o = new TObjArray();
    
    pb = new Plot(sample + "W__.*__"+ mva+"$");
    pn = new Plot(sample + "non_W__.*__"+ mva+"$");
    pb->toUnderOverflow();
    pn->toUnderOverflow();
    pb->SetTitle("W");
    pn->SetTitle("non W");
    
    pbi = (Plot*)pb->Clone();
    pni = (Plot*)pn->Clone();
    double bI =  pbi->at(0)->Integral() ;
    double nI =  pni->at(0)->Integral() ;
    for(unsigned int iH = 0; iH < pbi->getNumberOfHistograms(); ++iH){
       pbi->at(iH)->Scale( 1./bI  );
       pni->at(iH)->Scale( 1./nI  );
    }
    pbi = pbi->makeIntegral(true);
    pni = pni->makeIntegral(true);
    o->Add(pbi);    
    o->Add(pni);
    
    pb->normalize();
    pb = pb->makeIntegral(true);
    o->Add(pb);
    pn->normalize();
    pn = pn->makeIntegral(true);
    o->Add(pn);
    
    pb = new Plot(sample + "W__.*__allWs$");
    pn = new Plot(sample + "non_W__.*__allWs$");
    pb->SetTitle("W");
    pn->SetTitle("non W");
    for(unsigned int iH = 0; iH < pb->getNumberOfHistograms(); ++iH){
        pb->at(iH)->Scale(1./pb->at(iH)->GetBinContent(1));
        pn->at(iH)->Scale(1./pn->at(iH)->GetBinContent(1));
    }
    o->Add(pb);    
    o->Add(pn);
    
    pn = new Plot(sample + "non_W__.*__fakeCategory$");
    pn->normalize();
    o->Add(pn);
    
    pn = new Plot(sample + "non_W__.*_nFakes$");
    pn->normalize();
    o->Add(pn);
    
    Pint::drawAll(o,"","hist",":m");
}


root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testWJetVars.C+("sigSamples/T2tt_merged.root" ,"TestAnalyzer/Events","T2tt_merged_wCand_plots.root")'   &
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testWJetVars.C+("sigSamples/T2tt_650_325.root","TestAnalyzer/Events","T2tt_650_325_wCand_plots.root")' &
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testWJetVars.C+("sigSamples/T2tt_850_100.root","TestAnalyzer/Events","T2tt_850_100_wCand_plots.root")'  &

root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/flattenWTree.C+("sigSamples/T2tt_merged.root","TestAnalyzer/Events","T2tt_merged_wCand_tree.root"))' &
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/produceWMVA.C+("T2tt_merged_wCand_tree.root","Events","mva","T2tt_merged_wCand_disc.root")' &
        



// ---------------------------------------------------------------------
//
//     T Candidates
//
// ---------------------------------------------------------------------

{
    
    double bins[] = {0,150,300,500,1000};
    RebinToNewBins b(4,bins);

    Plot::cache("*_tCand_plots.root");
        o = new TObjArray();
    pb = new Plot("picky_t__pt$");
    pn = new Plot("picky_not_t__pt$");
    pn->toUnderOverflow();
    pb->toUnderOverflow();    
    pb->SetTitle("t");
    o->Add(pb);
    pn->SetTitle("non t");
    o->Add(pn);
    pbr = (Plot*)pb->Clone();
    pbr->divide(*pn);
    o->Add(pbr);
    
    pn = (Plot*)pn->Clone();
    pb = (Plot*)pb->Clone();
    pn->normalize();
    pb->normalize();
    pn->rebin(b);
    pb->rebin(b);
    pbr = (Plot*)pb->Clone();
    pbr = pbr->divide(*pn);
    o->Add(pn);
    o->Add(pb);
    o->Add(pbr);
    
    
    Pint::drawAll(o,"","",":i");
    
}

{
    Plot::cache("*_tCand_plots.root");
    
    // TString vars[] = {"wPT","tMass","wMass","bPTotPT","bCSV","maxWCSV","bWLikli","wDisc","maxOWDisc","m23om123","m13om12","atan_m13om12","maxjjdr","wbDR","wbDEta","wbDPhi","nTCon","mva","pruned_mva",""};
    TString vars[] = {"tMass","pruned_mva",""};

    TString sample = "^merged_picky_";
    // TString sample = "^ttbar_merged_picky_";
            // TString sample = "^650_325_picky_";
            // TString sample = "^850_100_picky_";
    
    o = new TObjArray;
    oo = new TObjArray;
    ooo = new TObjArray;
    
    for(unsigned int iV = 0; vars[iV][0]; ++iV){
        pb = new Plot(sample + "t__.*__"+ vars[iV]+"$");
        pn = new Plot(sample + "not_t__.*__"+ vars[iV]+"$");
        if(pn->getNumberOfHistograms() != pb->getNumberOfHistograms()) continue;
        pb->toUnderOverflow();
        pn->toUnderOverflow();
        pb->normalize();
        pn->normalize();
        // pn->rebin(5);
        // pb->rebin(5);
        pb->SetTitle("t");
        o->Add(pb);
        pn->SetTitle("not t");
        oo->Add(pn);
        
        TCanvas * c = pn->makeCutEfficiencies(*pb,"t","not t");
        ooo->Add(c);    
    }
    Pint::drawAll(o,"t","",":mr");
    Pint::drawAll(oo,"not t","",":mr");
    Pint::drawAll(ooo,"ROC");

}

{
    Plot::cache("*_tCand_plots.root");
    
TString sample = "^merged_picky_";
        // TString sample = "^650_325_picky_";
        // TString sample = "^850_100_picky_";
    TString mva = "mva";
    o = new TObjArray();
        //
    pb = new Plot(sample + "t__.*__"+ mva+"$");
    pn = new Plot(sample + "not_t__.*__"+ mva+"$");
    pb->toUnderOverflow();
    pn->toUnderOverflow();
    pb->SetTitle("t");
    pn->SetTitle("not t");

    pbi = (Plot*)pb->Clone();
    pni = (Plot*)pn->Clone();
    double bI =  pbi->at(0)->Integral() ;
    double nI =  pni->at(0)->Integral() ;
    for(unsigned int iH = 0; iH < pbi->getNumberOfHistograms(); ++iH){
       pbi->at(iH)->Scale( 1./bI  );
       pni->at(iH)->Scale( 1./nI  );
    }
    pbi = pbi->makeIntegral(true);
    pni = pni->makeIntegral(true);
    o->Add(pbi);
    o->Add(pni);

    pb->normalize();
    pb = pb->makeIntegral(true);
    o->Add(pb);
    pn->normalize();
    pn = pn->makeIntegral(true);
    o->Add(pn);

    pb = new Plot(sample + "t__.*__allTs$");
    pn = new Plot(sample + "not_t__.*__allTs$");
    pb->SetTitle("W");
    pn->SetTitle("non W");
    for(unsigned int iH = 0; iH < pb->getNumberOfHistograms(); ++iH){
        pb->at(iH)->Scale(1./pb->at(iH)->GetBinContent(1));
        pn->at(iH)->Scale(1./pn->at(iH)->GetBinContent(1));
    }
    o->Add(pb);
    o->Add(pn);
    
    pn = new Plot(sample + "not_t__.*__fakeCategory$");
    pn->normalize();
    o->Add(pn);
    
    pn = new Plot(sample + "not_t__.*_nFakes$");
    pn->normalize();
    pn->toUnderOverflow();
    o->Add(pn);
    
    pb = new Plot(sample + "t__.*_realRank$");
    pb->normalize();
    o->Add(pb);
    
    pb = new Plot(sample + "t__.*_nexcl$");
    pb->normalize();
    o->Add(pb);
    
    pb = new Plot(sample + "t__.*_nreal$");
    pb->normalize();
    o->Add(pb);
    
    pb = new Plot(sample + "t__.*_realsubrank$");
    pb->normalize();
    o->Add(pb);
    
    pb = new Plot(sample + "t__nExclPairs$");
    pb->normalize();
    o->Add(pb);
    
    pb = new Plot(sample + "t__realPairRanlk$");
    pb->normalize();
    o->Add(pb);
    
    Pint::drawAll(o,"","hist",":m");
}




root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTJetVars.C+("sigSamples/T2tt_merged.root" ,"TestAnalyzer/Events","T2tt_merged_tCand_plots.root")'   &
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTJetVars.C+("sigSamples/T2tt_650_325.root","TestAnalyzer/Events","T2tt_650_325_tCand_plots.root")' &
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTJetVars.C+("sigSamples/T2tt_850_100.root","TestAnalyzer/Events","T2tt_850_100_tCand_plots.root")'  &

root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/flattenTTree.C+("sigSamples/T2tt_merged.root","TestAnalyzer/Events","T2tt_merged_tCand_tree.root"))' &
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/produceTMVA.C+("T2tt_merged_tCand_tree.root","Events","mva","T2tt_merged_tCand_disc.root")' &


// ---------------------------------------------------------------------
//
//     Top pairs
//
// ---------------------------------------------------------------------
{
    double bins[] = {0,50,100,150,200,250,300,350,400,450,500,600,750,1500};
    RebinToNewBins b(13,bins);
    
    Plot::cache("*_tPairs_plots.root");
    TString vars[] = {"leadTopPT","subLeadTopPT","avgTopPT",""};
    TString samples[] ={"650_325","850_100","merged",""};
        // TString samples[] ={"merged",""};
        // TString samples[] ={"650_325","merged",""};
    o = new TObjArray;
    oo = new TObjArray;
    
    for(unsigned int iS = 0; samples[iS][0]; ++iS)
        for(unsigned int iV = 0; vars[iV][0]; ++iV){
            p = new Plot(samples[iS] +"_picky_(best_pair|reconstructed|resolved_jets|all)__"+ vars[iV]+"$");
            // p = new Plot(samples[iS] +"_picky_(best_pair|reconstructed)__"+ vars[iV]+"$");
            p->toUnderOverflow();
            p->rebin(b);
            p->SetTitle(samples[iS]);
            
            o->Add(p);
            p = p->makeRatiogram("all");
            p->setMinMax(0.,1.);
            oo->Add(p);
        }
        Pint::drawAll(o,"");
        Pint::drawAll(oo,"");
    
}

{
    Plot::cache("*_tPairs_plots.root");    
    TString sample = "^merged_picky_";
    
    TString pts[] = {"pt_all__","pt_lt150__","pt_eq150to300__","pt_eq300to500__","pt_geq500__",""};
    TString vars[] = {"rank","mva",""};
    o = new TObjArray;
    
    for(unsigned int iV = 0; vars[iV][0]; ++iV){
        for(unsigned int iP = 0; pts[iP][0]; ++iP){
            p = new Plot(sample + ".*__"+pts[iP]+vars[iV]+"$");
            p->SetTitle(Plot::translated(pts[iP]));
            p->normalize();
            o->Add(p);
        }
    }
    Pint::drawAll(o,"","",":i(0,-1)");
    
}

{
    Plot::cache("*_tPairs_plots.root");
    
    TString sample = "^merged_picky_";
    // TString sample = "^850_100_picky_";
    
    o = new TObjArray();
    oo = new TObjArray();
    // p = new Plot(sample+"all__nFakes$");
    // o->Add(p);
    //
    // p = new Plot(sample + "(t|not_t)__rank$");
    // p->normalize();
    // o->Add(p);
    
    nottcomp = new Plot();
    tcomp = new Plot();
    
    
    p = new Plot(sample + ".*__mva1p2$");    
    p->normalize();
    nottcomp->add(p->at("not_t"),"1+2");
    tcomp->add(p->at("t"),"1+2");
    p = p->makeIntegral(true);
    
    o->Add(p);
    
    p = new Plot(sample + ".*__mva1t2$");
    p->normalize();
    nottcomp->add(p->at("not_t"),"1x2");
    tcomp->add(p->at("t"),"1x2");
    p = p->makeIntegral(true);
    o->Add(p);
    
    // p = new Plot(sample + ".*__mvap2$");
    // p->normalize();
    // nottcomp->add(p->at("not_t"),"2");
    // tcomp->add(p->at("t"),"2");
    // p = p->makeIntegral(true);
    // o->Add(p);
    
    p = new Plot(sample + ".*__mval$");
    p->normalize();
    nottcomp->add(p->at("not_t"),"mval");
    tcomp->add(p->at("t"),"mval");
    p = p->makeIntegral(true);
    o->Add(p);
    
    nottcomp->makeCutEfficiencies(*tcomp,"t","not t");
 
    Pint::drawAll(o,"");
    
}

    
    
        
    root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTopPairs.C+("sigSamples/T2tt_merged.root" ,"TestAnalyzer/Events","T2tt_merged_tPairs_plots.root")'   &
    root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTopPairs.C+("sigSamples/T2tt_650_325.root","TestAnalyzer/Events","T2tt_650_325_tPairs_plots.root")' &
    root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTopPairs.C+("sigSamples/T2tt_850_100.root","TestAnalyzer/Events","T2tt_850_100_tPairs_plots.root")'  &        
        
        
/// TRY adding ttbar to tTraining
        
        root -b -q 'CORRAL/CORRALTrainingSkimmer.C+("/eos/uscms/store/user/vdutta/13TeV/310315/merged/ttbar_ntuple.root")' &
        root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/flattenTTree.C+("sigSamples/ttbar_ntuple_skimmed.root","Events","ttbar_onlyGood_tCand_tree.root",true,true)' &

        root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/produceTMVA.C+("T2tt_ttbar_merged_tCand_tree.root","Events","mva","T2tt_ttbar_merged_tCand_disc.root")' &



            root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTJetVars.C+("sigSamples/ttbar_ntuple_skimmed.root" ,"Events","ttbar_merged_ttbar_tCand_plots.root")'   &
            root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTJetVars.C+("sigSamples/T2tt_merged.root" ,"TestAnalyzer/Events","T2tt_merged_ttbar_tCand_plots.root")'   &
            root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTJetVars.C+("sigSamples/T2tt_650_325.root","TestAnalyzer/Events","T2tt_650_325_ttbar_tCand_plots.root")' &
            root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTJetVars.C+("sigSamples/T2tt_850_100.root","TestAnalyzer/Events","T2tt_850_100_ttbar_tCand_plots.root")'  &


                root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTopPairs.C+("sigSamples/T2tt_merged.root" ,"TestAnalyzer/Events","T2tt_merged_ttbar_tPairs_plots.root")'   &
                root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTopPairs.C+("sigSamples/T2tt_650_325.root","TestAnalyzer/Events","T2tt_650_325_ttbar_tPairs_plots.root")' &
                root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testTopPairs.C+("sigSamples/T2tt_850_100.root","TestAnalyzer/Events","T2tt_850_100_ttbar_tPairs_plots.root")'  &
   
   

                    root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testCORRAL.C+("sigSamples/T2tt_850_100.root" ,"TestAnalyzer/Events","T2tt_850_100_testCORRAL_plots.root")'   &
                    root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testCORRAL.C+("sigSamples/T2tt_650_325.root" ,"TestAnalyzer/Events","T2tt_650_325_testCORRAL_plots.root")'   &
                    root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_3_1/src/AnalysisMethods/macros/CORRAL/testCORRAL.C+("sigSamples/ttbar_metgeq200.root" ,"TestAnalyzer/Events","ttbar_testCORRAL_plots.root")'   &
   

        