/*
  Resolved top/W tagger

    book / fillResolvedBasicInfo
      basic cand/pass
    bookExtra / fillResolvedExtraInfo
      more cand/pass info (eta etc)
    bookFiltered / fillResolvedFilteredInfo
      filtered event info

    code to use:
      resolved cand = resCands[i] // not ordered
      resolved top = resTops[i] // ordered by descending chi2(W mass, top mass)

    resTops is the collection of custom objects
      use res.j123 for object MomentumF
          res.m3jet for object total mass (=j123.mass())
          res.j1 and j2 for W components
          res.j3 for b component
          res.bjetcsv (= res.j3) for b component
*/

#ifndef RESOLVEDTAGGER_HH
#define RESOLVEDTAGGER_HH

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/DataFormats/interface/CMSTop.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include <vector>
#include "Math/LorentzVector.h"
#include "Math/VectorUtil.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"
//#include "AnalysisTools/KinematicVariables/interface/mt2w.h"
//#include "AnalysisTools/KinematicVariables/interface/Mt2Helper.h"

using namespace ucsbsusy;

struct resolved {
  resolved() : 
              rj1(0), rj2(0), rj3(0),
              bjetcsv(-1.),
              m1(-1.),  m2(-1.),  m3(-1.),
              m12(-1.), m13(-1.), m23(-1.), m3jet(-1.)
              {}
  ~resolved(){}
  MomentumF j1, j2, j3;       // subjet momenta. might not be pt-ordered. see individual algo.
  MomentumF j123;             // object momentum, the sum of subjets' p4
  float     pt,eta;           // of j123 - convenience
  float     bjetcsv;          // csv of bjet (j3)
  bool      hasmbjet;         // does csv > defaults::medium
  RecoJetF *rj1, *rj2, *rj3;  // ptrs to reco subjets' full info
  float     m1, m2, m3;           // subjet masses
  float     m12, m13, m23, m3jet; // subjet pairwise and total mass
  float     drW;              // dr between W products = dr(j1,j2)
  float     drbW;             // dr between b and W = dr(j12,j3)
  float     dr123;            // max dr between all top products = max(dr12, dr13, dr23)
};
std::vector<resolved> resTops; //passing - tighter mass windows, dr
std::vector<resolved> resCands; //candidate - loose mass windows

// mass windows and resolutions
float mW = 80.4;
float mTop = 173.4;
float mWloosemin = mW-40., mWloosemax = mW+40.; // Huilin and I agree on these loose windows
float mToploosemin = mTop-80., mToploosemax = mTop+80.;
//float mWloosemin = 60., mWloosemax = 110.; // candidate windows, 90% eff for filtered tops
//float mToploosemin = 140., mToploosemax = 210.;
float mWpassmin = 65., mWpassmax = 95.; // passing windows - unimportant
float mToppassmin = 150., mToppassmax = 200.;

struct ResTreeFiller {

  ResTreeFiller() {}

  // basic
  size i_resnpasstops;
  size i_resncandtops;
  size i_respasspt;
  size i_respasseta;
  size i_respassm3jet;
  size i_respassm12;
  size i_respassWpt;
  size i_respassdrW;
  size i_respassdrbW;
  size i_respassdr123;
  size i_respassbpt;
  size i_respassbcsv;
  size i_respassj1csv;
  size i_respassj2csv;

  size i_rescandpt;
  size i_rescandeta;
  size i_rescandm3jet;
  size i_rescandm12;
  size i_rescandWpt;
  size i_rescanddrW;
  size i_rescanddrbW;
  size i_rescanddr123;
  size i_rescandbpt;
  size i_rescandbcsv;
  size i_rescandj1csv;
  size i_rescandj2csv;
  size i_rescandchi2;

  // extra
  size i_respassm13;
  size i_respassm23;
  size i_respassm1;
  size i_respassm2;
  size i_respassm3;
  size i_respassj1pt;
  size i_respassj1eta;
  size i_respassj2pt;
  size i_respassj2eta;
  size i_respassj3pt;
  size i_respassj3eta;

  size i_rescandm13;
  size i_rescandm23;
  size i_rescandm1;
  size i_rescandm2;
  size i_rescandm3;
  size i_rescandj1pt;
  size i_rescandj1eta;
  size i_rescandj2pt;
  size i_rescandj2eta;
  size i_rescandj3pt;
  size i_rescandj3eta;

  // filtered
  size i_ngenhadronictops;
  size i_gentopavgpt;
  size i_gendrbdau1; // gen top info
  size i_gendrbdau2;
  size i_gendrdau1dau2;
  size i_gendrbW;
  size i_gendr123;
  size i_genmindrtopproducts;
  size i_genmindrtops;
  size i_gentoppt;
  size i_genWpt;
  size i_genbpt;
  size i_gendau1pt;
  size i_gendau2pt;
  size i_gentopeta;
  size i_genWeta;
  size i_genbeta;
  size i_gendau1eta;
  size i_gendau2eta;

  size i_recobpt;
  size i_recodau1pt;
  size i_recodau2pt;
  size i_recodrbdau1;
  size i_recodrbdau2;
  size i_recodrdau1dau2;
  size i_recodrbW;
  size i_recodr123;
  size i_recobcsv;
  size i_recoj1csv;
  size i_recoj2csv;

  size i_recotoppt;
  size i_recotopeta;
  size i_recoWpt;
  size i_recoWeta;
  size i_recobeta;
  size i_recodau1eta;
  size i_recodau2eta;
  size i_recom1;
  size i_recom2;
  size i_recom3;
  size i_recom12;
  size i_recom3jet;

  size i_mindrcandtopquarks;
  size i_allmatchedtoreco;
  size i_bmatchedtoreco;
  size i_csvrankofrecob;
  size i_maxnrecojetsinacand;
  size i_recoispassing;
  size i_recopasswwindow;
  size i_recopasstopwindow;
  size i_recopassdr123;

  void book(TreeWriterData* data) {
    i_resnpasstops           = data->add<int  >("","resnpasstops","I",0);
    i_resncandtops           = data->add<int  >("","resncandtops","I",0);
    i_respasspt              = data->addMulti<float>("","respasspt",-9.);
    i_respasseta             = data->addMulti<float>("","respasseta",-9.);
    i_respassm3jet           = data->addMulti<float>("","respassm3jet",-9.);
    i_respassm12             = data->addMulti<float>("","respassm12",-9.);
    i_respassWpt             = data->addMulti<float>("","respassWpt",-9.);
    i_respassdrW             = data->addMulti<float>("","respassdrW",-9.);
    i_respassdrbW            = data->addMulti<float>("","respassdrbW",-9.);
    i_respassdr123           = data->addMulti<float>("","respassdr123",-9.);
    i_respassbpt             = data->addMulti<float>("","respassbpt",-9.);
    i_respassbcsv            = data->addMulti<float>("","respassbcsv",-9.);
    i_respassj1csv           = data->addMulti<float>("","respassj1csv",-9.);
    i_respassj2csv           = data->addMulti<float>("","respassj2csv",-9.);

    i_rescandpt              = data->addMulti<float>("","rescandpt",-9.);
    i_rescandeta             = data->addMulti<float>("","rescandeta",-9.);
    i_rescandm3jet           = data->addMulti<float>("","rescandm3jet",-9.);
    i_rescandm12             = data->addMulti<float>("","rescandm12",-9.);
    i_rescandWpt             = data->addMulti<float>("","rescandWpt",-9.);
    i_rescanddrW             = data->addMulti<float>("","rescanddrW",-9.);
    i_rescanddrbW            = data->addMulti<float>("","rescanddrbW",-9.);
    i_rescanddr123           = data->addMulti<float>("","rescanddr123",-9.);
    i_rescandbpt             = data->addMulti<float>("","rescandbpt",-9.);
    i_rescandbcsv            = data->addMulti<float>("","rescandbcsv",-9.);
    i_rescandj1csv           = data->addMulti<float>("","rescandj1csv",-9.);
    i_rescandj2csv           = data->addMulti<float>("","rescandj2csv",-9.);
    i_rescandchi2            = data->addMulti<float>("","rescandchi2",-9.);
  }

  void bookExtra(TreeWriterData* data) {
    i_respassm13             = data->addMulti<float>("","respassm13",-9.);
    i_respassm23             = data->addMulti<float>("","respassm23",-9.);
    i_respassm1              = data->addMulti<float>("","respassm1",-9.);
    i_respassm2              = data->addMulti<float>("","respassm2",-9.);
    i_respassm3              = data->addMulti<float>("","respassm3",-9.);
    i_respassj1pt            = data->addMulti<float>("","respassj1pt",-9.);
    i_respassj1eta           = data->addMulti<float>("","respassj1eta",-9.);
    i_respassj2pt            = data->addMulti<float>("","respassj2pt",-9.);
    i_respassj2eta           = data->addMulti<float>("","respassj2eta",-9.);
    i_respassj3pt            = data->addMulti<float>("","respassj3pt",-9.);
    i_respassj3eta           = data->addMulti<float>("","respassj3eta",-9.);

    i_rescandm13             = data->addMulti<float>("","rescandm13",-9.);
    i_rescandm23             = data->addMulti<float>("","rescandm23",-9.);
    i_rescandm1              = data->addMulti<float>("","rescandm1",-9.);
    i_rescandm2              = data->addMulti<float>("","rescandm2",-9.);
    i_rescandm3              = data->addMulti<float>("","rescandm3",-9.);
    i_rescandj1pt            = data->addMulti<float>("","rescandj1pt",-9.);
    i_rescandj1eta           = data->addMulti<float>("","rescandj1eta",-9.);
    i_rescandj2pt            = data->addMulti<float>("","rescandj2pt",-9.);
    i_rescandj2eta           = data->addMulti<float>("","rescandj2eta",-9.);
    i_rescandj3pt            = data->addMulti<float>("","rescandj3pt",-9.);
    i_rescandj3eta           = data->addMulti<float>("","rescandj3eta",-9.);
  }

  void bookFiltered(TreeWriterData* data){
    i_ngenhadronictops              = data->add<int>("","ngenhadronictops","I",-1);
    i_genmindrtopproducts           = data->add<float>("","genmindrtopproducts","F",-9.);
    i_genmindrtops                  = data->add<float>("","genmindrtops","F",-9);
    i_gendrbdau1                    = data->addMulti<float>("","gendrbdau1",-9.);
    i_gendrbdau2                    = data->addMulti<float>("","gendrbdau2",-9.);
    i_gendrdau1dau2                 = data->addMulti<float>("","gendrdau1dau2",-9.);
    i_gendrbW                       = data->addMulti<float>("","gendrbW",-9.);
    i_gendr123                      = data->addMulti<float>("","gendr123",-9.);
    i_gentoppt                      = data->addMulti<float>("","gentoppt",-9.);
    i_genWpt                        = data->addMulti<float>("","genWpt",-9.);
    i_genbpt                        = data->addMulti<float>("","genbpt",-9.);
    i_gendau1pt                     = data->addMulti<float>("","gendau1pt",-9.);
    i_gendau2pt                     = data->addMulti<float>("","gendau2pt",-9.);
    i_gentopeta                     = data->addMulti<float>("","gentopeta",-9.);
    i_genWeta                       = data->addMulti<float>("","genWeta",-9.);
    i_genbeta                       = data->addMulti<float>("","genbeta",-9.);
    i_gendau1eta                    = data->addMulti<float>("","gendau1eta",-9.);
    i_gendau2eta                    = data->addMulti<float>("","gendau2eta",-9.);

    i_recobpt                       = data->addMulti<float>("","recobpt",-9.);
    i_recodau1pt                    = data->addMulti<float>("","recodau1pt",-9.);
    i_recodau2pt                    = data->addMulti<float>("","recodau2pt",-9.);
    i_recodrbdau1                   = data->addMulti<float>("","recodrbdau1",-9.);
    i_recodrbdau2                   = data->addMulti<float>("","recodrbdau2",-9.);
    i_recodrdau1dau2                = data->addMulti<float>("","recodrdau1dau2",-9.);
    i_recodrbW                      = data->addMulti<float>("","recodrbW",-9.);
    i_recodr123                     = data->addMulti<float>("","recodr123",-9.);
    i_recobcsv                      = data->addMulti<float>("","recobcsv",-9.);
    i_recoj1csv                     = data->addMulti<float>("","recoj1csv",-9.);
    i_recoj2csv                     = data->addMulti<float>("","recoj2csv",-9.);

    i_recotoppt                     = data->addMulti<float>("","recotoppt",-9.);
    i_recotopeta                    = data->addMulti<float>("","recotopeta",-9.);
    i_recoWpt                       = data->addMulti<float>("","recoWpt",-9.);
    i_recoWeta                      = data->addMulti<float>("","recoWeta",-9.);
    i_recobeta                      = data->addMulti<float>("","recobeta",-9.);
    i_recodau1eta                   = data->addMulti<float>("","recodau1eta",-9.);
    i_recodau2eta                   = data->addMulti<float>("","recodau2eta",-9.);
    i_recom1                        = data->addMulti<float>("","recom1",-9.);
    i_recom2                        = data->addMulti<float>("","recom2",-9.);
    i_recom3                        = data->addMulti<float>("","recom3",-9.);
    i_recom12                       = data->addMulti<float>("","recom12",-9.);
    i_recom3jet                     = data->addMulti<float>("","recom3jet",-9.);

    i_bmatchedtoreco                = data->addMulti<bool>("","bmatchedtoreco",false);
    i_csvrankofrecob                = data->addMulti<int>("","csvrankofrecob",-1);
    i_mindrcandtopquarks            = data->addMulti<float>("","mindrcandtopquarks",-9.);
    i_allmatchedtoreco              = data->addMulti<bool >("","allmatchedtoreco",false);
    i_maxnrecojetsinacand           = data->addMulti<int>("","maxnrecojetsinacand",-1);
    i_recopasswwindow               = data->addMulti<bool>("","recopasswwindow",false);
    i_recopasstopwindow             = data->addMulti<bool>("","recopasstopwindow",false);
    i_recopassdr123                 = data->addMulti<bool>("","recopassdr123",false);
    i_recoispassing                 = data->addMulti<bool>("","recoispassing",false);

  }

  void findCandW(const vector<RecoJetF*> jets, RecoJetF* b, vector<resolved> & resCands){
    bool dbg = false;
    if(dbg) std::cout << "*** finding W around the bjet " << b->p4() << std::endl;

    // find jets j1 and j2 to make a W=j1+j2
    MomentumF j1, j2, j3, j12, j123;
    j3 = b->p4(); // we were given the b
    int nJets = jets.size();
    for(int i = 0 ; i < nJets-1 ; ++i){
      for(int j = i+1 ; j < nJets ; ++j){
        if( (b->pt() == jets[i]->pt()) || (b->pt() == jets[j]->pt()) ) continue; // j1, j2 shouldn't be the bjet
        if(dbg) std::cout << "checking W candidate: " << jets[i]->pt() << " " << jets[j]->pt() << std::endl;

        // require loose W and Top mass windows
        float m12 = (jets[i]->p4() + jets[j]->p4()).mass();
        float m3jet = (jets[i]->p4() + jets[j]->p4() + b->p4()).mass();
        if( (m12 > mWloosemax) || (m12 < mWloosemin) ) continue;
        if( (m3jet > mToploosemax) || (m3jet < mToploosemin) ) continue;
        if(dbg) std::cout << "   passed loose w and top mass windows" << std::endl;

        // fill candidate
        resolved cand;
        cand.rj1   = jets[i];
        cand.rj2   = jets[j];
        cand.rj3   = b;
        cand.j1    = cand.rj1->p4();
        cand.j2    = cand.rj2->p4();
        cand.j3    = cand.rj3->p4();
        cand.j123  = cand.j1.p4() + cand.j2.p4() + cand.j3.p4();
        cand.pt    = cand.j123.pt();
        cand.eta   = cand.j123.eta();
        cand.m3jet = cand.j123.mass();
        cand.bjetcsv  = cand.rj3->csv();
        cand.hasmbjet = (cand.bjetcsv > defaults::CSV_MEDIUM);
        cand.m1    = (cand.j1.p4()).mass();
        cand.m2    = (cand.j2.p4()).mass();
        cand.m3    = (cand.j3.p4()).mass();
        cand.m12   = (cand.j1.p4()+cand.j2.p4()).mass();
        cand.m23   = (cand.j2.p4()+cand.j3.p4()).mass();
        cand.m13   = (cand.j1.p4()+cand.j3.p4()).mass();
        cand.drW   = PhysicsUtilities::deltaR(cand.j1,cand.j2);
        cand.drbW  = PhysicsUtilities::deltaR(cand.j1.p4()+cand.j2.p4(),cand.j3);
        cand.dr123 = std::max(PhysicsUtilities::deltaR(cand.j3,cand.j1),
                       std::max(PhysicsUtilities::deltaR(cand.j3,cand.j2),
                                PhysicsUtilities::deltaR(cand.j1, cand.j2)));
        resCands.push_back(cand);

      }//for j
    }//for i

  }

  bool isPassingResolvedTop(const resolved cand){
    // strict W and Top mass windows
    bool passWwindow   = cand.m12   > mWpassmin   && cand.m12   < mWpassmax;
    bool passTopwindow = cand.m3jet > mToppassmin && cand.m3jet < mToppassmax;

    // mass drop conditions - from hett
    /*
    // condition (iii) has subconditions A1 A2 B C
    bool condA1 = (0.2 < atan(m13/m12)) &
            (1.3 > atan(m13/m12));

    bool condA2 = (rMin < m23/m3jet) &
            (rMax > m23/m3jet);
    bool condA = condA1 & condA2;

    // condition B
    bool condB = (pow(rMin,2)*(1+pow(m13/m12,2)) < 1-pow(m23/m3jet,2)) &
           (pow(rMax,2)*(1+pow(m13/m12,2)) > 1-pow(m23/m3jet,2)) &
           (m23/m3jet > 0.35);

    // condition C is condition B with jet 2 <-> 3
    bool condC = (pow(rMin,2)*(1+pow(m12/m13,2)) < 1-pow(m23/m3jet,2)) &
           (pow(rMax,2)*(1+pow(m12/m13,2)) > 1-pow(m23/m3jet,2)) &
           (m23/m3jet > 0.35);

    // enforce either A or B or C
    if (!(condA || condB || condC)) return false;
    */

    // require drW < some pt dependence
    //float drWbest = 160./(cand.j1.p4() + cand.j2.p4()).pt();
    //if( abs(cand.drW - drWbest)/drWbest > 0.3) return false;

    // require drbW < some pt dependence
    //if( cand.drbW > 1.5*( 2.*173./j123.pt() ) ) return false;
    //float drbWbest = 270./cand.j123.pt();
    //if( abs(cand.drbW - drbWbest)/drbWbest > 0.3) return false;

    // require dr123 < some pt dependence
    //if( cand.dr123 > 1.5*( 2.*173./j123.pt() ) ) return false; // pt-dependent dr123

    // hemisphere dr123 (max pairwise spread)
    bool passdr123 = cand.dr123 < TMath::PiOver2();

    return (passWwindow && passTopwindow && passdr123);
  }

  void fillResolvedBasicInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, vector<RecoJetF*> jets, vector<resolved> & resTops, vector<resolved> & resCands) {
    bool dbg = false;
    if(dbg) std::cout << std::endl << std::endl <<  "****filling resolved info" << std::endl;

    resTops.clear(); resCands.clear();
    int nJets = jets.size();
    if(dbg) std::cout << "njets " << nJets << std::endl;

    //sort jets by descending csv
    vector<RecoJetF*> jetsCSVranked(jets);
    sort( jetsCSVranked.begin(), jetsCSVranked.end(), []( const RecoJetF* lhs, const RecoJetF* rhs ){ return lhs->csv() > rhs->csv(); } );

    RecoJetF *csv1lj=0, *csv2lj=0; // two highest csv loose jets
    for(const auto j : jetsCSVranked){
      if(j->csv()>defaults::CSV_LOOSE){
        if(!csv1lj){ csv1lj = j; }
        else if(!csv2lj){ csv2lj = j; }
      }
    }

    // require 2 loose jets for the algorithm
    if(csv2lj == 0) return;

    // assemble some candidate bjets
    vector<RecoJetF*> candb;
    candb.push_back(csv1lj);
    candb.push_back(csv2lj);

    auto chi2 = [&] (const resolved& top) {
      return pow( (top.m3jet - mTop)/20., 2 ) + pow( (top.m12 - mW)/10., 2 ); // resolution got from filtered tops
    };

    // assemble some candidate tops
    for(auto b : candb){ findCandW( jets, b, resCands ); }
    data->fill<int  >(i_resncandtops, resCands.size());
    for(auto top : resCands) {
      data->fillMulti<float>(i_rescandpt      ,top.pt);
      data->fillMulti<float>(i_rescandeta     ,top.eta);
      data->fillMulti<float>(i_rescandm3jet   ,top.m3jet);
      data->fillMulti<float>(i_rescandm12     ,top.m12);
      data->fillMulti<float>(i_rescandWpt     ,(top.j1.p4()+top.j2.p4()).pt());
      data->fillMulti<float>(i_rescanddrW     ,top.drW);
      data->fillMulti<float>(i_rescanddrbW    ,top.drbW);
      data->fillMulti<float>(i_rescanddr123   ,top.dr123);
      data->fillMulti<float>(i_rescandbpt     ,top.j3.pt());
      data->fillMulti<float>(i_rescandbcsv    ,top.rj3->csv());
      data->fillMulti<float>(i_rescandj1csv   ,top.rj1->csv());
      data->fillMulti<float>(i_rescandj2csv   ,top.rj2->csv());
      data->fillMulti<float>(i_rescandchi2    ,chi2(top));
    }

    // apply passing selection criteria
    for(auto cand : resCands){ if( isPassingResolvedTop(cand) ) resTops.push_back(cand); }

    // sort selected tops by ascending sth
    sort( resTops.begin(), resTops.end(), [&]( const resolved& lhs, const resolved& rhs )
      {
        return chi2(lhs) < chi2(rhs);
        /* // simple top mass ordering
        auto comp = [&](const resolved& top) {
          return abs(top.m3jet - mTop);
        };
        return comp(lhs) < comp(rhs);
        */
      }
    );

    // clean reco tops wrt each other, keeping best
    for(int i = 0 ; i < nJets ; ++i) {
      int prev = -1, toErase = -1;
      for(int j = 0 ; j < resTops.size() ; ++j) {
        bool isPresent = (jets[i]->pt() == resTops[j].j1.pt()) | // is jet a subjet of this top
			 (jets[i]->pt() == resTops[j].j2.pt()) |
                         (jets[i]->pt() == resTops[j].j3.pt());
        if(isPresent && prev > -1 ) { toErase = max(prev,j); break; }
        else if(isPresent) { prev = j; }
      }
      if(toErase > -1) { // erase
        resTops.erase(resTops.begin() + toErase);
        i = -1; // reset i to zero
      }
    }
    data->fill<int  >(i_resnpasstops, resTops.size());
    if(dbg) std::cout << "***** event had cand, pass tops : " << resCands.size() << " " << resTops.size() << std::endl;

    if(dbg) std::cout << std::endl << " ** candidate tops: " << std::endl;
    if(dbg){
      for(int i = 0 ; i < resCands.size() ; ++i){
        resolved rj = resCands[i];
        std::cout << "chi2: " << chi2(rj) << std::endl;
        std::cout << rj.j1 << " " << rj.j2 << " " << rj.j3 << std::endl;
        std::cout << rj.j123 << std::endl;
        std::cout << rj.bjetcsv << " " << rj.hasmbjet << std::endl;
        std::cout << rj.rj1 << " " << rj.rj2 << " " << rj.rj3 << std::endl;
        std::cout << rj.m1 << " " << rj.m2 << " " << rj.m3 << " " << rj.m12 << " " << rj.m23 << " " << rj.m13 << " " << rj.m3jet << std::endl;
      }
    }

    if(dbg) std::cout << std::endl << " ** selected tops: " << std::endl;
    if(dbg){
      for(int i = 0 ; i < resTops.size() ; ++i){
        resolved rj = resTops[i];
        std::cout << "chi2: " << chi2(rj) << std::endl;
        std::cout << rj.j1 << " " << rj.j2 << " " << rj.j3 << std::endl;
        std::cout << rj.j123 << std::endl;
        std::cout << rj.bjetcsv << " " << rj.hasmbjet << std::endl;
        std::cout << rj.rj1 << " " << rj.rj2 << " " << rj.rj3 << std::endl;
        std::cout << rj.m1 << " " << rj.m2 << " " << rj.m3 << " " << rj.m12 << " " << rj.m23 << " " << rj.m13 << " " << rj.m3jet << std::endl;
      }
    }

    for(auto top : resTops) {
      data->fillMulti<float>(i_respasspt      ,top.pt);
      data->fillMulti<float>(i_respasseta     ,top.eta);
      data->fillMulti<float>(i_respassm3jet   ,top.m3jet);
      data->fillMulti<float>(i_respassm12     ,top.m12);
      data->fillMulti<float>(i_respassWpt     ,(top.j1.p4()+top.j2.p4()).pt());
      data->fillMulti<float>(i_respassdrW     ,top.drW);
      data->fillMulti<float>(i_respassdrbW    ,top.drbW);
      data->fillMulti<float>(i_respassdr123   ,top.dr123);
      data->fillMulti<float>(i_respassbpt     ,top.j3.pt());
      data->fillMulti<float>(i_respassbcsv    ,top.rj3->csv());
      data->fillMulti<float>(i_respassj1csv   ,top.rj1->csv());
      data->fillMulti<float>(i_respassj2csv   ,top.rj2->csv());
    }

  }//fillResolvedBasicInfo


  // --------------------------------------------------------------------------------------------------
  void fillResolvedExtraInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, vector<resolved> & resTops){

    bool dbg = false;
    if(dbg) std::cout << std::endl << "Filling extra ncands, ntops: " << resCands.size() << " " << resTops.size() << std::endl;
    for(auto top : resCands) {
      data->fill<float>(i_rescandm13     ,top.m13);
      data->fill<float>(i_rescandm23     ,top.m23);
      data->fill<float>(i_rescandm1      ,top.m1);
      data->fill<float>(i_rescandm2      ,top.m2);
      data->fill<float>(i_rescandm3      ,top.m3);
      data->fill<float>(i_rescandj1pt    ,top.j1.pt());
      data->fill<float>(i_rescandj1eta   ,top.j1.eta());
      data->fill<float>(i_rescandj2pt    ,top.j2.pt());
      data->fill<float>(i_rescandj2eta   ,top.j2.eta());
      data->fill<float>(i_rescandj3pt    ,top.j3.pt());
      data->fill<float>(i_rescandj3eta   ,top.j3.eta());
    }

    for(auto top : resTops) {
      data->fill<float>(i_respassm13     ,top.m13);
      data->fill<float>(i_respassm23     ,top.m23);
      data->fill<float>(i_respassm1      ,top.m1);
      data->fill<float>(i_respassm2      ,top.m2);
      data->fill<float>(i_respassm3      ,top.m3);
      data->fill<float>(i_respassj1pt    ,top.j1.pt());
      data->fill<float>(i_respassj1eta   ,top.j1.eta());
      data->fill<float>(i_respassj2pt    ,top.j2.pt());
      data->fill<float>(i_respassj2eta   ,top.j2.eta());
      data->fill<float>(i_respassj3pt    ,top.j3.pt());
      data->fill<float>(i_respassj3eta   ,top.j3.eta());
    }
  }//fillResolvedExtraInfo


/*//save
    // choose our top
    TRandom3 * rand = ana->getRndGen();
    bool rndtop = (rand->Uniform(0,1) > 0.5);
    const PartonMatching::TopDecay * top = rndtop ? top1 : top2;
*/
  // ----------------------------------------------------------------------------------------------------------------------
  void fillResolvedFilteredInfo(TreeWriterData* data, BaseTreeAnalyzer* ana,vector<resolved> & resTops, vector<resolved> & resCands){
    bool dbg = false;
    if(dbg) std::cout << std::endl << "** filling filtered info " << std::endl;

    std::vector<GenJetF*> filteredGenJets;
    for(auto * j : ana->jets){ if(j->genJet()) filteredGenJets.push_back(j->genJet());}
    PartonMatching::PartonEvent * partonEvent = new PartonMatching::PartonEvent(ana->genParticleReader,*(ana->defaultJets),filteredGenJets);
    vector<const PartonMatching::TopDecay*> tops;
    for(unsigned int i = 0 ; i < partonEvent->topDecays.size() ; i++){
      PartonMatching::TopDecay* top = &partonEvent->topDecays[i];
      tops.push_back(top); //NOTE: if condense this loop, see only &partonEvent->topDecays (first top in vector) -> bug
    }

    vector<RecoJetF*> jetsCSVranked(ana->jets); 
    sort( jetsCSVranked.begin(), jetsCSVranked.end(), []( const RecoJetF* lhs, const RecoJetF* rhs ){ return lhs->csv() > rhs->csv(); } );

    // dr between passing tops and gen hadr tops
    for(auto recotop : resTops){
      float mindr = 9.;
      for(auto top : tops){
        if(!top->isLeptonic){
          mindr = std::min( double(mindr), PhysicsUtilities::deltaR(top->top->p4(), recotop.j123.p4()) );
        }
      }
      data->fillMulti<float>(i_mindrcandtopquarks, mindr);
    }

    int ngenhadronictops = 0;
    for(auto top : tops) { if( !top->isLeptonic) ngenhadronictops++; }
    data->fill<int>(i_ngenhadronictops, ngenhadronictops);

    // require > 0 hadronic tops
    if(ngenhadronictops < 1) return;

    // min dr between all quarks
    auto getMinDRbetweenTopProducts = [](const vector<const PartonMatching::TopDecay*> tops)->float {
      vector<const GenParticleF*> parts;
      for(auto top : tops){
        parts.push_back(top->b->parton); parts.push_back(top->W_dau1->parton); parts.push_back(top->W_dau2->parton);
      }
      float mindr = 99;
      for(unsigned int i = 0 ; i < parts.size()-1 ; i++){
        for(unsigned int j = i+1 ; j < parts.size() ; j++){
          mindr = std::min( double(mindr), PhysicsUtilities::deltaR(*parts[i], *parts[j]) );
        }
      }
      return mindr;
    };

    auto getMinDRbetweenTops = [](const vector<const PartonMatching::TopDecay*> tops)->float {
      float mindr = 99;
      for(unsigned int i = 0 ; i < tops.size()-1 ; i++){
        for(unsigned int j = i+1 ; j < tops.size() ; j++){
          mindr = std::min( double(mindr), PhysicsUtilities::deltaR(*tops[i]->top, *tops[j]->top) );
        }
      }
      return mindr;
    };

    //match quark to exactly one ak4
    auto getMatchedJet = [](const GenParticleF* part, const vector<RecoJetF*>& jets)->RecoJetF* {
      float matchdr = 0.4, matchpt = 0.5;
      RecoJetF* jmatch = 0;
      for(int i = 0; i < jets.size() ; i++){
        float drmatch = PhysicsUtilities::deltaR(*part, *jets[i]) < matchdr;
        bool  ptmatch = abs(jets[i]->pt() - part->pt())/part->pt() < matchpt;
        if(drmatch && ptmatch){
          if(jmatch) return 0; // two matches
          jmatch = jets[i];
        }
      }
      return jmatch;
    };

    data->fill<float>(i_genmindrtops, getMinDRbetweenTops(tops));
    data->fill<float>(i_genmindrtopproducts, getMinDRbetweenTopProducts(tops) );

    for(auto top : tops){
      // before hadronic req't: do two highest csv loose jets contain b
      // match b parton to ak4
      RecoJetF * tmpb = getMatchedJet(top->b->parton, ana->jets);
      if(dbg) std::cout << "matched b: " << tmpb << std::endl;
      if(dbg) if(tmpb) std::cout << "    gen, reco: " << top->b->parton->p4() << " " << tmpb->p4() << std::endl;
      data->fillMulti<bool>(i_bmatchedtoreco, (tmpb > 0));
      int brank = -1;
      for(unsigned int i = 0 ; i < jetsCSVranked.size() ; i++) {
        if(tmpb == jetsCSVranked[i]) brank=i;
      }

      data->fillMulti<int>(i_csvrankofrecob, brank);
      if(dbg) std::cout << "csv rank of reco b: " << brank << std::endl;

      // require hadronic
      if(top->isLeptonic) continue;

      auto getTopDR123 = [](const PartonMatching::TopDecay * top)->float {
        return  std::max(PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau1->parton),
                std::max(PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau2->parton),
                         PhysicsUtilities::deltaR(*top->W_dau2->parton,*top->W_dau1->parton)
                        )
                        );
      };

      // gen top properties
      data->fillMulti<float>(i_gendrbdau1,      PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau1->parton) );
      data->fillMulti<float>(i_gendrbdau2,      PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau2->parton) );
      data->fillMulti<float>(i_gendrdau1dau2, PhysicsUtilities::deltaR(*top->W_dau1->parton,*top->W_dau2->parton) );
      data->fillMulti<float>(i_gendrbW,         PhysicsUtilities::deltaR(*top->b->parton, *top->W) );
      data->fillMulti<float>(i_gendr123,      getTopDR123(top));

      data->fillMulti<float>(i_gentoppt,  top->top->pt() );
      data->fillMulti<float>(i_genWpt,    top->W->pt() );
      data->fillMulti<float>(i_genbpt,    top->b->parton->pt() );
      data->fillMulti<float>(i_gendau1pt, top->W_dau1->parton->pt() );
      data->fillMulti<float>(i_gendau2pt, top->W_dau2->parton->pt() );

      data->fillMulti<float>(i_gentopeta,  top->top->eta() );
      data->fillMulti<float>(i_genWeta,    top->W->eta() );
      data->fillMulti<float>(i_genbeta,    top->b->parton->eta() );
      data->fillMulti<float>(i_gendau1eta, top->W_dau1->parton->eta() );
      data->fillMulti<float>(i_gendau2eta, top->W_dau2->parton->eta() );

      // match partons to ak4
      RecoJetF * recob    = getMatchedJet(top->b->parton,      ana->jets);
      RecoJetF * recodau1 = getMatchedJet(top->W_dau1->parton, ana->jets);
      RecoJetF * recodau2 = getMatchedJet(top->W_dau2->parton, ana->jets);
      if(dbg) std::cout << "match to ak4? b, w1, w2: " << recob << " " << recodau1 << " " << recodau2 << std::endl;

      // require matching
      bool allmatchedtoreco = !((recob == 0) || (recodau1 == 0) || (recodau2 == 0));
      data->fillMulti<bool>(i_allmatchedtoreco,allmatchedtoreco);
      if(!allmatchedtoreco) return;

      data->fillMulti<float>(i_recobpt, recob->pt() );
      data->fillMulti<float>(i_recodau1pt, recodau1->pt() );
      data->fillMulti<float>(i_recodau2pt, recodau2->pt() );

      if(dbg) std::cout << "b and reco match: " << top->b->parton->p4() << " " << recob->p4() << std::endl;
      if(dbg) std::cout << "dau1 and reco match: " << top->W_dau1->parton->p4() << " " << recodau1->p4() << std::endl;
      if(dbg) std::cout << "dau2 and reco match: " << top->W_dau2->parton->p4() << " " << recodau2->p4() << std::endl;

      auto getRecoTopDR123 = [](const RecoJetF * b, const RecoJetF * dau1, const RecoJetF * dau2)->float {
        return  std::max(PhysicsUtilities::deltaR(*b,*dau1),
                  std::max(PhysicsUtilities::deltaR(*b,*dau2),
                             PhysicsUtilities::deltaR(*dau1, *dau2)
                          )
                        );
      };

      // fill reco
      data->fillMulti<float>(i_recodrbdau1,    PhysicsUtilities::deltaR(*recob, *recodau1) );
      data->fillMulti<float>(i_recodrbdau2,    PhysicsUtilities::deltaR(*recob, *recodau2) );
      data->fillMulti<float>(i_recodrdau1dau2, PhysicsUtilities::deltaR(*recodau1, *recodau2) );
      data->fillMulti<float>(i_recodrbW, PhysicsUtilities::deltaR( *recob, (recodau1->p4() + recodau2->p4()) ));
      data->fillMulti<float>(i_recodr123, getRecoTopDR123( recob, recodau1, recodau2 ) );
      data->fillMulti<float>(i_recobcsv, recob->csv());
      data->fillMulti<float>(i_recoj1csv, recodau1->csv());
      data->fillMulti<float>(i_recoj2csv, recodau2->csv());
      data->fillMulti<float>(i_recotoppt, (recodau1->p4() + recodau2->p4() + recob->p4()).pt() );
      data->fillMulti<float>(i_recotopeta, (recodau1->p4() + recodau2->p4() + recob->p4()).eta() );
      data->fillMulti<float>(i_recoWpt,  (recodau1->p4() + recodau2->p4()).pt() );
      data->fillMulti<float>(i_recoWeta, (recodau1->p4() + recodau2->p4()).eta() );
      data->fillMulti<float>(i_recobeta, recob->eta() );
      data->fillMulti<float>(i_recodau1eta, recodau1->eta() );
      data->fillMulti<float>(i_recodau2eta, recodau2->eta() );
      data->fillMulti<float>(i_recom1, recodau1->mass() );
      data->fillMulti<float>(i_recom2, recodau2->mass() );
      data->fillMulti<float>(i_recom3, recob->mass() );
      data->fillMulti<float>(i_recom12, (recodau1->p4() + recodau2->p4()).mass() );
      data->fillMulti<float>(i_recom3jet, (recodau1->p4() + recodau2->p4() + recob->p4()).mass() );

      // check some selections on reco
      float recom12 = (recodau1->p4() + recodau2->p4()).mass();
      data->fillMulti<bool>(i_recopasswwindow, (recom12 > mWpassmin) && (recom12 < mWpassmax) );
      float recom3jet = (recodau1->p4() + recodau2->p4() + recob->p4()).mass();
      data->fillMulti<bool>(i_recopasstopwindow, (recom3jet > mToppassmin) && (recom3jet < mToppassmax) );
      float dr123 = getRecoTopDR123( recob, recodau1, recodau2 );
      data->fillMulti<bool>(i_recopassdr123, dr123 < TMath::PiOver2());
      if(dbg) std::cout << "extra bools for our reco guy... recom12, recom3jet, dr123 " << recom12 << " " << recom3jet << " " << dr123 << std::endl;

      // check if reco jets made it into a candidate top, or how many subjets did
      int maxnrecojetsinacand = 0;
      resolved * cand = 0;
      for(auto res : resCands) {
        if(dbg) std::cout << "cand: recob and rj3 " << recob << " " << res.rj3 << std::endl;
        if(dbg) std::cout << "   recodau1, recodau2, rj1, rj2 " << recodau1 << " " << recodau2 << " " << res.rj1 << " " << res.rj2 << std::endl;
        if(recob != res.rj3) continue; // failed to capture b
        maxnrecojetsinacand = std::max(maxnrecojetsinacand,1);
        if( (recodau1 != res.rj1) && (recodau1 != res.rj2) ) continue; // didn't capture dau1
        maxnrecojetsinacand = std::max(maxnrecojetsinacand,2);
        if( (recodau2 != res.rj1) && (recodau2 != res.rj2) ) continue; // didn't capture dau2
        maxnrecojetsinacand = std::max(maxnrecojetsinacand,3);
        cand = &res;
      }
      data->fillMulti<int>(i_maxnrecojetsinacand, maxnrecojetsinacand);
      if(dbg) std::cout << "maxnrecojetsinacand: " << maxnrecojetsinacand << std::endl;

      // if it became a candidate, check if it's passing
      if(cand) data->fillMulti<bool>(i_recoispassing, isPassingResolvedTop(*cand));
      if(dbg) if(cand) std::cout << "m12, m3jet: " << cand->m12 << " " << cand->m3jet << std::endl;
      if(dbg) if(cand) std::cout << "it was a cand-> passed? " << isPassingResolvedTop(*cand) << std::endl;
    }//forTops
  }//fillResolvedFilteredInfo

};//ResTreeFiller

#endif
