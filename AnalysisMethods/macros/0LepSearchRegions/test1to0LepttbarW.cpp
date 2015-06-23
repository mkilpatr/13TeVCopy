nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/310315/merged/ttbar_ntuple.root"                      ,"ttbar" , 831.76    ,4,-1,"TestAnalyzer/Events","metSkim")' &


nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/ttbar_4_ntuple_wgtxsec.root"         ,"Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/ttbar_7_ntuple_wgtxsec.root"         ,"Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/ttbar_3_ntuple_wgtxsec.root"         ,"Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/ttbar_9_ntuple_wgtxsec.root"         ,"Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/ttbar_2_ntuple_wgtxsec.root"         ,"Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/ttbar_6_ntuple_wgtxsec.root"         ,"Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/ttbar_8_ntuple_wgtxsec.root"         ,"Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/ttbar_5_ntuple_wgtxsec.root"         ,"Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/ttbar_10_ntuple_wgtxsec.root"        ,"Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/ttbar_1_ntuple_wgtxsec.root"         ,"Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/wjets_ht600toInf_ntuple_wgtxsec.root","Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/wjets_ht400to600_ntuple_wgtxsec.root","Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/wjets_ht200to400_ntuple_wgtxsec.root","Events","njskim")' &
nohup root -b -q '0LepSearchRegions/preselectionSkimmer3.C+("/eos/uscms/store/user/vdutta/13TeV/080615/merged/wjets_ht100to200_ntuple_wgtxsec.root","Events","njskim")' &




root -b -q '0LepSearchRegions/test1to0LepttbarW.C+("ttbarW.root")' &
    
{
    o = new TObjArray;
    TString sel[] ={"0LepNoVetoPre__noDPHi__nTops_incl__nBs_geq1__.*__nEvents", "0LepPre__noDPHi__nTops_incl__nBs_geq1__.*__nEvents",
"0LepPre__noMTB__nTops_incl__nBs_geq1__.*__nEvents","0LepPre__nTops_incl__nBs_geq1__.*__nEvents", ""};


for(unsigned int iS = 0; sel[iS][0]; ++iS){
    p = new Plot(sel[iS]);
    o->Add(p);
    
    }
    Pint::drawAll(o,"","",":i");
}    
    

{
// TString pre = "incl";
TString pre = "nTops_incl__nBs_geq1__";
    o = new TObjArray();
    p = new Plot("(0LepPre|1LepPre|1LepTightPre)__" + pre+ "all__met");
    p->sitrep();
    p->toUnderOverflow();
    o->Add(p);
    p = (Plot*)p->Clone();
    p->normalize();
    o->Add(p);
    
    // p = new Plot("1LepPre__"+pre+"__(all|W)__met");
    // // p = new Plot(pre+"__all__met");
    // p->toUnderOverflow();
    // p->sitrep();
    //
    // o->Add(p);
    //  r = p->makeRatiogram(1u,"");
    //  o->Add(r);
    // p = (Plot*)p->Clone();
    // p->normalize();
    // o->Add(p);
    // r = p->makeRatiogram(0u,"");
    // o->Add(r);
    //
    Pint::drawAll(o,"");

}


{
// TString pre = "incl";
TString pre = "otherPre";

TString nTs[] = {"nTops_eq0__","nTops_geq1__",""};
TString nBs[] = {"nBs_eq1__","nBs_gt1__",""};
o = new TObjArray();
oo = new TObjArray();

for(unsigned int iT = 0; nTs[iT][0]; ++iT)
    for(unsigned int iB = 0; nBs[iB][0]; ++iB){
        // p = new Plot("(1LepTightPre__|0LepPre__)" + nTs[iT] + nBs[iB]+ "all__coarse_met");
        
        p = new Plot("1LepTightPre__noMTB__" + nTs[iT] + nBs[iB]+ ".*__coarse_met");
        
        p->toUnderOverflow();
        p->scale(.25);
        p->SetTitle(Plot::translated(nTs[iT] + nBs[iB]));
        p->sitrep();
        o->Add(p);
        // r = p->makeRatiogram("1LepTightPre","");
        r = p->makeRatiogram("all","");
        oo->Add(r);
        
        
    }
    
    Pint::drawAll(o,"");
    Pint::drawAll(oo,"");

}


{
// TString pre = "incl";
TString pre = "otherPre";

TString nTs[] = {"nTops_eq0__","nTops_geq1__",""};
TString nBs[] = {"nBs_eq1__","nBs_gt1__",""};
o = new TObjArray();
oo = new TObjArray();
ooo = new TObjArray();

for(unsigned int iT = 0; nTs[iT][0]; ++iT)
    for(unsigned int iB = 0; nBs[iB][0]; ++iB){
        p = new Plot("(1LepTightPre__|0LepPre__)" + nTs[iT] + nBs[iB]+ "all__coarse_met");        
        
        p2 = new Plot("(1LepTightPre__|0LepPre__)nTops_incl__nBs_geq1__all__coarse_met");
        p2->sitrep();
        double zNorm = p2->at("0LepPre")->Integral(0,-1);
        double oNorm = p2->at("1LepTightPre")->Integral(0,-1);
        
        
        
        // p = new Plot("1LepTightPre__" + nTs[iT] + nBs[iB]+ ".*__coarse_met");
        
        p->toUnderOverflow();
        // p->scale(.25);
        p->SetTitle(Plot::translated(nTs[iT] + nBs[iB]));
        p->sitrep();
        o->Add(p);
        r = p->makeRatiogram("1LepTightPre","");
        // r = p->makeRatiogram("all","");
        oo->Add(r);
        
        p = (Plot*)p->Clone();
        p->at("0LepPre")->Scale(1./zNorm);
        p->at("1LepTightPre")->Scale(1./oNorm);
        ooo->Add(p);
        
    }
    
    Pint::drawAll(o,"");
    Pint::drawAll(oo,"");
    Pint::drawAll(ooo,"");

}


{
    Plot::cache("ttbarW*plots.root");
    TString nTs[] = {"nTops_eq0__","nTops_geq1__",""};
    TString nBs[] = {"nBs_eq1__","nBs_gt1__",""};
    o = new TObjArray();
    oo = new TObjArray();
    ooo = new TObjArray();

    for(unsigned int iT = 0; nTs[iT][0]; ++iT)
        for(unsigned int iB = 0; nBs[iB][0]; ++iB){
            p = new Plot("1LepPre__" + nTs[iT] + nBs[iB]+ "all__coarse_met");
            p2 = new Plot("stand_0LepPre__" + nTs[iT] + nBs[iB]+ "all__coarse_met");
            p->add(p2->at(0),"0Lep");
            p->toUnderOverflow();
            p->SetTitle(Plot::translated(nTs[iT] + nBs[iB]));
            p->normalize();
            o->Add(p);                                
    }
    Pint::drawAll(o,"");

    
}


{
    TString lep[] = {"1LepPre__","0LepPre__",""};
    TString reg = "nTops_incl__nBs_geq1__";
    
    TString sel[] ={"wPT","metplep","neutrino_o_w_profile","met_o_metplep_profile",""};
    
    o = new TObjArray();
    
    for(unsigned int iL = 0; lep[iL][0]; ++iL)
    for(unsigned int iS = 0; sel[iS][0]; ++iS){
        p = new Plot(lep[iL] + reg +".*__"+ sel[iS]);
        cout << lep[iL] + reg +".*__"+ sel[iS] <<endl;
        // p->rebin(5);
        p->SetTitle(Plot::translated(lep[iL]));
        if(iS > 1)p->setMinMax(0.,2.);
        o->Add(p);
    }
    Pint::drawAll(o,"");
    
}


{
    TString lepSel = "1LepTightPre";
    TString noLepSel = "0LepPre";
    TString nTs[] = {"nTops_eq0__","nTops_geq1__",""};
    TString nBs[] = {"nBs_eq1__","nBs_gt1__",""};
    
    vector<double> dYields;
    vector<double> mYields;
    vector<double> uncs;
    vector<double> uncs1;
    vector<double> uncs2;
    vector<TString> names;
    
    for(unsigned int iB = 1; iB <= 5; ++iB){
            for(unsigned int ibT = 0; nBs[ibT][0]; ++ibT)
            for(unsigned int iT = 0; nTs[iT][0]; ++iT){                
                p = new Plot("(" + lepSel +"|" + noLepSel +")__" + nTs[iT] + nBs[ibT]+ "all__coarse_met" );
                double n1L = p->at(lepSel)->GetBinContent(iB);  if(iB==5) n1L += p->at(lepSel)->GetBinContent(iB +1);                
                double e1L = p->at(lepSel)->GetBinError(iB)*p->at(lepSel)->GetBinError(iB); if(iB==5) e1L += p->at(lepSel)->GetBinError(iB +1)*p->at(lepSel)->GetBinError(iB +1);
                double n0L = p->at(noLepSel)->GetBinContent(iB); if(iB==5) n0L += p->at(noLepSel)->GetBinContent(iB +1);
                double e0L = p->at(noLepSel)->GetBinError(iB)*p->at(noLepSel)->GetBinError(iB); if(iB==5) e0L += p->at(noLepSel)->GetBinError(iB +1)*p->at(noLepSel)->GetBinError(iB +1);
                uncs1.push_back(e1L);
                uncs2.push_back(e0L);
                dYields.push_back(n1L);
                mYields.push_back(n0L);
                uncs.push_back(TMath::Sqrt( e0L/(n1L*n1L) + e1L*n0L*n0L/(n1L*n1L*n1L*n1L)  ));      
                names.push_back( TString::Format("%u\t%s\t%s", iB,nBs[ibT].Data(), nTs[iT].Data() )  );  
            }            
    }
    
    cout << endl;
    for(unsigned int iB = 0; iB < dYields.size(); ++iB){
        cout << names[iB] << "\t" << dYields[iB] <<"\t"<< mYields[iB] <<"\t"<<(1.0 +uncs[iB]/mYields[iB]) << endl;
    }
    
    
}




{
    TString lepSel = "1LepTightPre";
    TString noLepSel = "0LepPre";
    TString nTs[] = {"nTops_eq0__","nTops_geq1__",""};
    TString nBs[] = {"nBs_eq1__","nBs_gt1__",""};
    
    vector<double> dYields;
    vector<double> mYields;
    vector<double> uncs;
    vector<double> uncs1;
    vector<double> uncs2;
    vector<TString> names;
    
    TH1 * h = new TH1F("scales","",20,-.5,19.5);
    
            for(unsigned int ibT = 0; nBs[ibT][0]; ++ibT)
            for(unsigned int iT = 0; nTs[iT][0]; ++iT){         
                for(unsigned int iB = 1; iB <= 5; ++iB){
                       
                p = new Plot("(" + lepSel +"|" + noLepSel +")__" + nTs[iT] + nBs[ibT]+ "all__coarse_met" );
                double n1L = p->at(lepSel)->GetBinContent(iB);  if(iB==5) n1L += p->at(lepSel)->GetBinContent(iB +1);                
                double e1L = p->at(lepSel)->GetBinError(iB)*p->at(lepSel)->GetBinError(iB); if(iB==5) e1L += p->at(lepSel)->GetBinError(iB +1)*p->at(lepSel)->GetBinError(iB +1);
                double n0L = p->at(noLepSel)->GetBinContent(iB); if(iB==5) n0L += p->at(noLepSel)->GetBinContent(iB +1);
                double e0L = p->at(noLepSel)->GetBinError(iB)*p->at(noLepSel)->GetBinError(iB); if(iB==5) e0L += p->at(noLepSel)->GetBinError(iB +1)*p->at(noLepSel)->GetBinError(iB +1);
                uncs1.push_back(e1L);
                uncs2.push_back(e0L);
                dYields.push_back(n1L);
                mYields.push_back(n0L);
                uncs.push_back(TMath::Sqrt( e0L/(n1L*n1L) + e1L*n0L*n0L/(n1L*n1L*n1L*n1L)  ));      
                names.push_back( TString::Format("%u\t%s\t%s", iB,nBs[ibT].Data(), nTs[iT].Data() )  );  
            }            
    }
    
    cout << endl;
    for(unsigned int iB = 0; iB < dYields.size(); ++iB){
        cout << names[iB] << "\t" << dYields[iB] <<"\t"<< mYields[iB] <<"\t"<<(1.0 +uncs[iB]/mYields[iB]) << endl;
        h->SetBinContent(iB+1, mYields[iB]/dYields[iB]);
        h->SetBinError(iB+1, uncs[iB]);
    }
    
    h->Draw();
    
    
}


