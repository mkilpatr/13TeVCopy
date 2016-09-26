/*
  Resolved top/W tagger
  (Ws are suspended -- see resWs and nsubjets commented out)

    ResTreeFiller
    fillResolvedInfo
    resolved top = resTops[i]

    resTops is the collection of custom objects
      use res.j123 for object MomentumF
          res.m3jet for object total mass (=j123.mass())
          res.j1 and j2 for W components
          res.j3 for b component
          res.bjet and res.bjetcsv (= res.j3) for b component

    variable names
      resbestrecoX = resolved tops, best reco top, property X
      resmatchedbestrecoX = resolved tops, best reco top matched to the event's hardest gen top, property X
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
std::vector<resolved> resTops;
std::vector<resolved> resCands; // loose selections, no cleaning 
//std::vector<resolved > resWs;

// mass windows and resolutions
  float mW = 80.4;
  float mWloosemin = mW - 40., mWloosemax = mW + 40.;
  float mWmin = mW - 20., mWmax = mW + 20.;
  float mTop = 173.4;
  float mToploosemin = mTop - 80., mToploosemax = mTop + 80.;
  float mTopmin = mTop - 50., mTopmax = mTop + 50;
  float mWres = 10., mTopres = 20.;


struct ResTreeFiller {

  ResTreeFiller() {}

  // Basic - some properties of best reco top
  size i_resnrecotops;
  size i_resbestrecopt;
  size i_resbestrecoeta;
  size i_resbestrecom3jet;
  size i_resbestrecom12;
  size i_resbestrecoWpt;
  size i_resbestrecodrW;
  size i_resbestrecodrbW;
  size i_resbestrecodr123;

  // Extra vars - more properties of best reco top
  size i_reshardgentoppt;
  size i_reshardgentopeta;
  size i_resbestrecom13;
  size i_resbestrecom23;
  size i_resbestrecom1;
  size i_resbestrecom2;
  size i_resbestrecom3;
  size i_resbestrecoj1pt;
  size i_resbestrecoj1eta;
  size i_resbestrecoj2pt;
  size i_resbestrecoj2eta;
  size i_resbestrecoj3pt;
  size i_resbestrecoj3eta;

  // Extra vars - properties of best reco top matched (dR) to the hardest gen hadronic top
  size i_resmatchedhardgentoppt;
  size i_resmatchedhardgentopeta;
  size i_resmatchednrecotops;
  size i_resmatchedbestrecopt;
  size i_resmatchedbestrecoeta;
  size i_resmatchedbestrecom3jet;
  size i_resmatchedbestrecom12;
  size i_resmatchedbestrecom13;
  size i_resmatchedbestrecom23;
  size i_resmatchedbestrecom1;
  size i_resmatchedbestrecom2;
  size i_resmatchedbestrecom3;
  size i_resmatchedbestrecoj1pt;
  size i_resmatchedbestrecoj1eta;
  size i_resmatchedbestrecoj2pt;
  size i_resmatchedbestrecoj2eta;
  size i_resmatchedbestrecoj3pt;
  size i_resmatchedbestrecoj3eta;

  // Gen - properties of gen partons and matching
  size i_ngenhadronictops;
  size i_gentopavgpt;
  size i_gendrbdau1; // gen top info
  size i_gendrbdau2;
  size i_gendrdau1dau2;
  size i_gendrbW;
  size i_gendr123;
  size i_gendrtops;
  size i_genmindrtopproducts;
  size i_gentoppt;
  size i_genbpt;
  size i_gendau1pt;
  size i_gendau2pt;

  size i_recoismatched;
  size i_recobpt; // matched reco top info
  size i_recodau1pt;
  size i_recodau2pt;
  size i_recodrbdau1;
  size i_recodrbdau2;
  size i_recodrdau1dau2;
  size i_recodrbW;
  size i_recodr123;

  size i_recotoppt;
  size i_recotopeta;
  size i_recoWpt; // extra matched reco top info
  size i_recoWeta;
  size i_recobeta;
  size i_recodau1eta;
  size i_recodau2eta;
  size i_recom1;
  size i_recom2;
  size i_recom3;
  size i_recom12;
  size i_recom3jet;

  size i_twocsvcontainb; // reco pass conditions
  size i_ncorrectcandjets;
  size i_correcttoppassed;
  size i_passwwindow;
  size i_passtopwindow;
  size i_passdr123;

  void book(TreeWriterData* data) {
    i_resnrecotops               = data->add<int  >("","resnrecotops","I",0);
    i_resbestrecopt              = data->add<float>("","resbestrecopt","F",-9.);
    i_resbestrecoeta             = data->add<float>("","resbestrecoeta","F",-9.);
    i_resbestrecom3jet           = data->add<float>("","resbestrecom3jet","F",-9.);
    i_resbestrecom12             = data->add<float>("","resbestrecom12","F",-9.);
    i_resbestrecoWpt             = data->add<float>("","resbestrecoWpt","F",-9.);
    i_resbestrecodrW             = data->add<float>("","resbestrecodrW","F",-9.);
    i_resbestrecodrbW            = data->add<float>("","resbestrecodrbW","F",-9.);
    i_resbestrecodr123           = data->add<float>("","resbestrecodr123","F",-9.);
  }

  void bookExtra(TreeWriterData* data) {
    i_resmatchedhardgentoppt     = data->add<float>("","resmatchedhardgentoppt","F",-9);
    i_resmatchedhardgentopeta     = data->add<float>("","resmatchedhardgentopeta","F",-9);
    i_resmatchednrecotops        = data->add<int  >("","resmatchednrecotops","I",0);
    i_resmatchedbestrecopt       = data->add<float>("","resmatchedbestrecopt","F",-9.);
    i_resmatchedbestrecoeta      = data->add<float>("","resmatchedbestrecoeta","F",-9.);
    i_resmatchedbestrecom3jet    = data->add<float>("","resmatchedbestrecom3jet","F",-9.);
    i_resmatchedbestrecom12      = data->add<float>("","resmatchedbestrecom12","F",-9.);
    i_resmatchedbestrecom13      = data->add<float>("","resmatchedbestrecom13","F",-9.);
    i_resmatchedbestrecom23      = data->add<float>("","resmatchedbestrecom23","F",-9.);
    i_resmatchedbestrecom1       = data->add<float>("","resmatchedbestrecom1","F",-9.);
    i_resmatchedbestrecom2       = data->add<float>("","resmatchedbestrecom2","F",-9.);
    i_resmatchedbestrecom3       = data->add<float>("","resmatchedbestrecom3","F",-9.);
    i_resmatchedbestrecoj1pt     = data->add<float>("","resmatchedbestrecoj1pt","F",-9.);
    i_resmatchedbestrecoj1eta    = data->add<float>("","resmatchedbestrecoj1eta","F",-9.);
    i_resmatchedbestrecoj2pt     = data->add<float>("","resmatchedbestrecoj2pt","F",-9.);
    i_resmatchedbestrecoj2eta    = data->add<float>("","resmatchedbestrecoj2eta","F",-9.);
    i_resmatchedbestrecoj3pt     = data->add<float>("","resmatchedbestrecoj3pt","F",-9.);
    i_resmatchedbestrecoj3eta    = data->add<float>("","resmatchedbestrecoj3eta","F",-9.);

    i_reshardgentoppt            = data->add<float>("","reshardgentoppt","F",-9.);
    i_reshardgentopeta           = data->add<float>("","reshardgentopeta","F",-9.);
    i_resbestrecom13             = data->add<float>("","resbestrecom13","F",-9.);
    i_resbestrecom23             = data->add<float>("","resbestrecom23","F",-9.);
    i_resbestrecom1              = data->add<float>("","resbestrecom1","F",-9.);
    i_resbestrecom2              = data->add<float>("","resbestrecom2","F",-9.);
    i_resbestrecom3              = data->add<float>("","resbestrecom3","F",-9.);
    i_resbestrecoj1pt            = data->add<float>("","resbestrecoj1pt","F",-9.);
    i_resbestrecoj1eta           = data->add<float>("","resbestrecoj1eta","F",-9.);
    i_resbestrecoj2pt            = data->add<float>("","resbestrecoj2pt","F",-9.);
    i_resbestrecoj2eta           = data->add<float>("","resbestrecoj2eta","F",-9.);
    i_resbestrecoj3pt            = data->add<float>("","resbestrecoj3pt","F",-9.);
    i_resbestrecoj3eta           = data->add<float>("","resbestrecoj3eta","F",-9.);
  }

  void bookGen(TreeWriterData* data){
    i_ngenhadronictops              = data->add<int>("","ngenhadronictops","I",0);
    i_gentopavgpt                   = data->add<float>("","gentopavgpt","F",-9.);
    i_gendrbdau1                    = data->add<float>("","gendrbdau1","F",-9.);
    i_gendrbdau2                    = data->add<float>("","gendrbdau2","F",-9.);
    i_gendrdau1dau2                 = data->add<float>("","gendrdau1dau2","F",-9.);
    i_gendrbW                       = data->add<float>("","gendrbW","F",-9.);
    i_gendr123                      = data->add<float>("","gendr123","F",-9.);
    i_gendrtops                     = data->add<float>("","gendrtops","F",-9.);
    i_genmindrtopproducts           = data->add<float>("","genmindrtopproducts","F",-9.);
    i_gentoppt                      = data->add<float>("","gentoppt","F",-9.);
    i_genbpt                        = data->add<float>("","genbpt","F",-9.);
    i_gendau1pt                     = data->add<float>("","gendau1pt","F",-9.);
    i_gendau2pt                     = data->add<float>("","gendau2pt","F",-9.);

    i_recoismatched                 = data->add<bool >("","recoismatched","O",false);
    i_recobpt                       = data->add<float>("","recobpt","F",-9.);
    i_recodau1pt                    = data->add<float>("","recodau1pt","F",-9.);
    i_recodau2pt                    = data->add<float>("","recodau2pt","F",-9.);
    i_recodrbdau1                   = data->add<float>("","recodrbdau1","F",-9.);
    i_recodrbdau2                   = data->add<float>("","recodrbdau2","F",-9.);
    i_recodrdau1dau2                = data->add<float>("","recodrdau1dau2","F",-9.);
    i_recodrbW                      = data->add<float>("","recodrbW","F",-9.);
    i_recodr123                     = data->add<float>("","recodr123","F",-9.);

    i_recotoppt                     = data->add<float>("","recotoppt","F",-9.);
    i_recotopeta                    = data->add<float>("","recotopeta","F",-9.);
    i_recoWpt                       = data->add<float>("","recoWpt","F",-9.);
    i_recoWeta                      = data->add<float>("","recoWeta","F",-9.);
    i_recobeta                      = data->add<float>("","recobeta","F",-9.);
    i_recodau1eta                   = data->add<float>("","recodau1eta","F",-9.);
    i_recodau2eta                   = data->add<float>("","recodau2eta","F",-9.);
    i_recom1                        = data->add<float>("","recom1","F",-9.);
    i_recom2                        = data->add<float>("","recom2","F",-9.);
    i_recom3                        = data->add<float>("","recom3","F",-9.);
    i_recom12                       = data->add<float>("","recom12","F",-9.);
    i_recom3jet                     = data->add<float>("","recom3jet","F",-9.);

    i_twocsvcontainb                = data->add<bool>("","twocsvcontainb","O",false);
    i_ncorrectcandjets              = data->add<int>("","ncorrectcandjets","I",-1);
    i_correcttoppassed              = data->add<bool>("","correcttoppassed","O",false);
    i_passwwindow                   = data->add<bool>("","passwwindow","O",false);
    i_passtopwindow                 = data->add<bool>("","passtopwindow","O",false);
    i_passdr123                     = data->add<bool>("","passdr123","O",false);

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
        cand.dr123 = max( max(PhysicsUtilities::deltaR(cand.j1,cand.j2), PhysicsUtilities::deltaR(cand.j1,cand.j3)), PhysicsUtilities::deltaR(cand.j2,cand.j3));
        resCands.push_back(cand);

      }//for j
    }//for i

  }

  bool isPassingResolvedTop(const resolved cand){
    // strict W and Top mass windows
    if( (cand.m12 < mWmin) || (cand.m12 > mWmax) ) return false;
    if( (cand.m3jet < mTopmin) || (cand.m3jet > mTopmax) ) return false;

    // mass drop conditions - hett
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

    // require dr123 = max(dr12,dr13,dr23) < pi/2 (hemisphere)
    if( cand.dr123 > TMath::PiOver2() ) return false;

    return true;
  }

  void fillResolvedInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, vector<RecoJetF*> jets, vector<resolved> & resTops, vector<resolved> & resCands) {
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

    // assemble some candidate tops
    for(auto b : candb){ findCandW( jets, b, resCands ); }

    // apply passing selection criteria
    for(auto cand : resCands){ if( isPassingResolvedTop(cand) ) resTops.push_back(cand); }

    // sort selected tops by ascending sth
    sort( resTops.begin(), resTops.end(), [&]( const resolved& lhs, const resolved& rhs )
      {
        /* // chi2 ordering
        auto chi2 = [&] (const resolved& top) {
          return pow( (top.m3jet - mTop)/mTopres, 2 ) + pow( (top.m12 - mW)/mWres, 2 );
        };
        return chi2(lhs) < chi2(rhs); 
        */
        // simple top mass ordering
        auto comp = [&](const resolved& top) {
          return abs(top.m3jet - mTop);
        };
        return comp(lhs) < comp(rhs);
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
    data->fill<int  >(i_resnrecotops, resTops.size());

    if(dbg) std::cout << " ** candidate tops: " << std::endl;
    if(dbg){
      for(int i = 0 ; i < resCands.size() ; ++i){
        resolved rj = resCands[i];
        std::cout << rj.j1 << " " << rj.j2 << " " << rj.j3 << std::endl;
        std::cout << rj.j123 << std::endl;
        std::cout << rj.bjetcsv << " " << rj.hasmbjet << std::endl;
        std::cout << rj.rj1 << " " << rj.rj2 << " " << rj.rj3 << std::endl;
        std::cout << rj.m1 << " " << rj.m2 << " " << rj.m3 << " " << rj.m12 << " " << rj.m23 << " " << rj.m13 << " " << rj.m3jet << std::endl;
      }
    }

    if(dbg) std::cout << " ** selected tops: " << std::endl;
    if(dbg){
      for(int i = 0 ; i < resTops.size() ; ++i){
        resolved rj = resTops[i];
        std::cout << rj.j1 << " " << rj.j2 << " " << rj.j3 << std::endl;
        std::cout << rj.j123 << std::endl;
        std::cout << rj.bjetcsv << " " << rj.hasmbjet << std::endl;
        std::cout << rj.rj1 << " " << rj.rj2 << " " << rj.rj3 << std::endl;
        std::cout << rj.m1 << " " << rj.m2 << " " << rj.m3 << " " << rj.m12 << " " << rj.m23 << " " << rj.m13 << " " << rj.m3jet << std::endl;
      }
    }

    if(dbg) std::cout << "***** event had tops : " << resTops.size() << std::endl;

    if(resTops.size() == 0) return;
    data->fill<float>(i_resbestrecopt      ,resTops[0].pt);
    data->fill<float>(i_resbestrecoeta     ,resTops[0].eta);
    data->fill<float>(i_resbestrecom3jet   ,resTops[0].m3jet);
    data->fill<float>(i_resbestrecom12     ,resTops[0].m12);
    data->fill<float>(i_resbestrecoWpt     ,resTops[0].j1.pt()+resTops[0].j2.pt());
    data->fill<float>(i_resbestrecodrW     ,resTops[0].drW);
    data->fill<float>(i_resbestrecodrbW    ,resTops[0].drbW);
    data->fill<float>(i_resbestrecodr123   ,resTops[0].dr123);

  }//fillResolvedInfo


  // --------------------------------------------------------------------------------------------------
  // deprecated by fillResolvedGenInfo
  void fillResolvedExtraInfo(TreeWriterData* data, BaseTreeAnalyzer* ana, vector<resolved> & resTops){

    bool dbg = false;
    if(dbg) std::cout << "n restops " << resTops.size() << std::endl;

    if(resTops.size()==0) return;

    // Extra vars - more properties of best reco top
    data->fill<float>(i_resbestrecom13     ,resTops[0].m13);
    data->fill<float>(i_resbestrecom23     ,resTops[0].m23);
    data->fill<float>(i_resbestrecom1      ,resTops[0].m1);
    data->fill<float>(i_resbestrecom2      ,resTops[0].m2);
    data->fill<float>(i_resbestrecom3      ,resTops[0].m3);
    data->fill<float>(i_resbestrecoj1pt    ,resTops[0].j1.pt());
    data->fill<float>(i_resbestrecoj1eta   ,resTops[0].j1.eta());
    data->fill<float>(i_resbestrecoj2pt    ,resTops[0].j2.pt());
    data->fill<float>(i_resbestrecoj2eta   ,resTops[0].j2.eta());
    data->fill<float>(i_resbestrecoj3pt    ,resTops[0].j3.pt());
    data->fill<float>(i_resbestrecoj3eta   ,resTops[0].j3.eta());

    // Extra vars - properties of best reco top matched (dr) to hardest gen hadronic top
    if(!ana->isMC()) return;

    if(dbg) std::cout << "finding hadr gen top" << std::endl;

    // hardest gen hadronic top
    GenParticleF* hardgen = 0;
    float hardgenpt = -9.;
    for(auto* p : ana->genParts){
      if (!(abs(p->pdgId()) == ParticleInfo::p_t && ParticleInfo::isGenTopHadronic(p) && ParticleInfo::isLastInChain(p))) continue;
      float genpt = p->p4().pt();
      if(genpt < hardgenpt) continue;
      hardgenpt = genpt;
      hardgen = p;
    }
    if(!hardgen) return;
    data->fill<float>(i_reshardgentoppt    ,hardgen->pt());
    data->fill<float>(i_reshardgentopeta   ,hardgen->eta());

    if(dbg) std::cout << "hardest gen hadr top " << hardgen->p4() << std::endl;

    // best reco top matched to hardest gen hadr top
    // not using findNearestDR b/c no Top.p4(), and might expand to chi2(subjet[i], genjet[i]) matching
    int resmatchednrecotops = 0;
    float hardrecopt = -9.;
    resolved * hardreco = 0;
    for(unsigned int i = 0 ; i < resTops.size() ; i++){
      resolved * reco = &resTops[i];
      if (PhysicsUtilities::deltaR(reco->j123, *hardgen) > 0.2) continue; // 2/3 of correct matches were dr < 0.1 in hand analysis
      resmatchednrecotops++;
      float recopt = reco->j123.pt();
      if(recopt < hardrecopt) continue;
      hardrecopt = recopt;
      hardreco = reco;
    }
    data->fill<int  >(i_resmatchednrecotops,resmatchednrecotops);
    if(dbg) std::cout << "n reco top matched: " << resmatchednrecotops << std::endl;

    if(!hardreco) return;
    if(dbg) std::cout << "best reco top matched: " << hardreco << " dR: " << PhysicsUtilities::deltaR(hardreco->j123, *hardgen) << " " << hardreco->j123.p4() << std::endl;

    data->fill<float>(i_resmatchedhardgentoppt    ,hardgen->pt());
    data->fill<float>(i_resmatchedhardgentopeta   ,hardgen->eta());
    data->fill<float>(i_resmatchedbestrecopt      ,hardreco->pt);
    data->fill<float>(i_resmatchedbestrecoeta     ,hardreco->eta);
    data->fill<float>(i_resmatchedbestrecom3jet   ,hardreco->m3jet);
    data->fill<float>(i_resmatchedbestrecom12     ,hardreco->m12);
    data->fill<float>(i_resmatchedbestrecom13     ,hardreco->m13);
    data->fill<float>(i_resmatchedbestrecom23     ,hardreco->m23);
    data->fill<float>(i_resmatchedbestrecom1      ,hardreco->m1);
    data->fill<float>(i_resmatchedbestrecom2      ,hardreco->m2);
    data->fill<float>(i_resmatchedbestrecom3      ,hardreco->m3);
    data->fill<float>(i_resmatchedbestrecoj1pt    ,hardreco->j1.pt());
    data->fill<float>(i_resmatchedbestrecoj1eta   ,hardreco->j1.eta());
    data->fill<float>(i_resmatchedbestrecoj2pt    ,hardreco->j2.pt());
    data->fill<float>(i_resmatchedbestrecoj2eta   ,hardreco->j2.eta());
    data->fill<float>(i_resmatchedbestrecoj3pt    ,hardreco->j3.pt());
    data->fill<float>(i_resmatchedbestrecoj3eta   ,hardreco->j3.eta());
  }//fillResolvedExtraInfo


  // ----------------------------------------------------------------------------------------------------------------------
  void fillResolvedGenInfo(TreeWriterData* data, BaseTreeAnalyzer* ana,vector<resolved> & resTops, vector<resolved> & resCands){
    bool dbg = true;
    if(dbg) std::cout << std::endl << "** filling gen info " << std::endl;

    std::vector<GenJetF*> filteredGenJes;
    for(auto * j : ana->jets){ if(j->genJet()) filteredGenJes.push_back(j->genJet());}
    PartonMatching::PartonEvent * partonEvent = new PartonMatching::PartonEvent(ana->genParticleReader,*(ana->defaultJets),filteredGenJes);
    const PartonMatching::TopDecay * top1 = partonEvent->topDecays.size() > 0 ? &partonEvent->topDecays[0] : 0 ;
    const PartonMatching::TopDecay * top2 = partonEvent->topDecays.size() > 1 ? &partonEvent->topDecays[1] : 0 ;

    int ngen = (top1) ? ((top2) ? 2 : 1) : 0;
    data->fill<int>(i_ngenhadronictops, ngen);

    // require two hadronic tops
    if( (!top1) || (!top2) ) return;
    if(top1->isLeptonic || top2->isLeptonic) return;
    data->fill<float>(i_gentopavgpt, 0.5*(top1->top->pt() + top2->top->pt()) );

    // choose our top
    TRandom3 * rand = ana->getRndGen();
    bool rndtop = (rand->Uniform(0,1) > 0.5);
    const PartonMatching::TopDecay * top = rndtop ? top1 : top2;
    if(dbg) std::cout << "two hadronic tops. ours is " << top->top->p4() << std::endl;

    auto getTopDR = [](const PartonMatching::TopDecay * top)->float {
      return  std::max(PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau1->parton),
                std::max(PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau2->parton),
                         PhysicsUtilities::deltaR(*top->W_dau2->parton,*top->W_dau1->parton)
                        )
                      );
    };

    auto getMinDRbetweenTops = [](const PartonMatching::TopDecay * top1, const PartonMatching::TopDecay * top2)->float {
      vector<const GenParticleF*> parts1, parts2;
      parts1.push_back(top1->b->parton); parts1.push_back(top1->W_dau1->parton); parts1.push_back(top1->W_dau2->parton);
      parts2.push_back(top2->b->parton); parts2.push_back(top2->W_dau1->parton); parts2.push_back(top2->W_dau2->parton);

      float mindr = 99;
      for(unsigned int i = 0 ; i < parts1.size() ; i++){
        for(unsigned int j = 0 ; j < parts2.size() ; j++){
          mindr = std::min( double(mindr), PhysicsUtilities::deltaR(*parts1[i], *parts2[j]) );
        }
      }
      return mindr;
    };

    data->fill<float>(i_gendrbdau1,    PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau1->parton) );
    data->fill<float>(i_gendrbdau2,    PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau2->parton) );
    data->fill<float>(i_gendrdau1dau2, PhysicsUtilities::deltaR(*top->W_dau1->parton,*top->W_dau2->parton) );
    data->fill<float>(i_gendrbW,       PhysicsUtilities::deltaR(*top->b->parton, *top->W) );
    data->fill<float>(i_gendr123,      getTopDR(top));
    data->fill<float>(i_gendrtops,     PhysicsUtilities::deltaR(*top1->top, *top2->top) );
    data->fill<float>(i_genmindrtopproducts, getMinDRbetweenTops(top1, top2) );
    if(dbg) std::cout << "genmindrtopproducts: " << getMinDRbetweenTops(top1,top2) << std::endl;
    data->fill<float>(i_gentoppt,  top->top->pt() );
    data->fill<float>(i_genbpt,    top->b->parton->pt() );
    data->fill<float>(i_gendau1pt, top->W_dau1->parton->pt() );
    data->fill<float>(i_gendau2pt, top->W_dau2->parton->pt() );

    //match to ak4
    auto getMatchedJet = [](const GenParticleF* part, const vector<RecoJetF*>& jets, float matchdr, float& mindr, float& secondmindr)->int {
      vector<float> drs; drs.reserve(jets.size());
      for(auto jet : jets){ drs.push_back(PhysicsUtilities::deltaR(*part, *jet)); }

      vector<float> sorted(drs);
      std::sort(sorted.begin(), sorted.end(), std::less<float>());
      vector<float>::iterator it = find(drs.begin(),drs.end(),sorted[0]);
      auto pos = std::distance(drs.begin(), it);

      mindr = sorted[0]; secondmindr = sorted[1];
      return (sorted[0] < matchdr && sorted[1] > matchdr) ? pos : -1;
    };

    // match partons to ak4
    float mindr = 0., secondmindr = 0.;
    int matchb    = getMatchedJet(top->b->parton,      ana->jets, 0.1, mindr, secondmindr);
    int matchdau1 = getMatchedJet(top->W_dau1->parton, ana->jets, 0.1, mindr, secondmindr);
    int matchdau2 = getMatchedJet(top->W_dau2->parton, ana->jets, 0.1, mindr, secondmindr);
    if(dbg) std::cout << "match to ak4? b, w1, w2: " << matchb << " " << matchdau1 << " " << matchdau2 << std::endl;
    if( (matchb < 0) || (matchdau1 < 0) || (matchdau2 < 0) ) return;
    data->fill<bool>(i_recoismatched,true);
    RecoJetF * recob    = ana->jets[matchb];
    RecoJetF * recodau1 = ana->jets[matchdau1];
    RecoJetF * recodau2 = ana->jets[matchdau2];
    data->fill<float>(i_recobpt, recob->pt() );
    data->fill<float>(i_recodau1pt, recodau1->pt() );
    data->fill<float>(i_recodau2pt, recodau2->pt() );

    if(dbg) std::cout << "b and reco match: " << top->b->parton->p4() << " " << recob->p4() << std::endl;
    if(dbg) std::cout << "dau1 and reco match: " << top->W_dau1->parton->p4() << " " << recodau1->p4() << std::endl;
    if(dbg) std::cout << "dau2 and reco match: " << top->W_dau2->parton->p4() << " " << recodau2->p4() << std::endl;

    auto getRecoTopDR = [](const RecoJetF * b, const RecoJetF * dau1, const RecoJetF * dau2)->float {
      return  std::max(PhysicsUtilities::deltaR(*b,*dau1),
                std::max(PhysicsUtilities::deltaR(*b,*dau2),
                         PhysicsUtilities::deltaR(*dau1, *dau2)
                        )
                      );
    };
    data->fill<float>(i_recodrbdau1,    PhysicsUtilities::deltaR(*recob, *recodau1) );
    data->fill<float>(i_recodrbdau2,    PhysicsUtilities::deltaR(*recob, *recodau2) );
    data->fill<float>(i_recodrdau1dau2, PhysicsUtilities::deltaR(*recodau1, *recodau2) );
    data->fill<float>(i_recodrbW, PhysicsUtilities::deltaR( *recob, (recodau1->p4() + recodau2->p4()) ));
    data->fill<float>(i_recodr123, getRecoTopDR( recob, recodau1, recodau2 ) );

    // fill extra well-matched properties
    data->fill<float>(i_recotoppt, (recodau1->p4() + recodau2->p4() + recob->p4()).pt() );
    data->fill<float>(i_recotopeta, (recodau1->p4() + recodau2->p4() + recob->p4()).eta() );
    data->fill<float>(i_recoWpt,  (recodau1->p4() + recodau2->p4()).pt() );
    data->fill<float>(i_recoWeta, (recodau1->p4() + recodau2->p4()).eta() );
    data->fill<float>(i_recobeta, recob->eta() );
    data->fill<float>(i_recodau1eta, recodau1->eta() );
    data->fill<float>(i_recodau2eta, recodau2->eta() );
    data->fill<float>(i_recom1, recodau1->mass() );
    data->fill<float>(i_recom2, recodau2->mass() );
    data->fill<float>(i_recom3, recob->mass() );
    data->fill<float>(i_recom12, (recodau1->p4() + recodau2->p4()).mass() );
    data->fill<float>(i_recom3jet, (recodau1->p4() + recodau2->p4() + recob->p4()).mass() );

    //sort jets by descending csv
    vector<RecoJetF*> jetsCSVranked(ana->jets); 
    sort( jetsCSVranked.begin(), jetsCSVranked.end(), []( const RecoJetF* lhs, const RecoJetF* rhs ){ return lhs->csv() > rhs->csv(); } );
    RecoJetF *csv1lj=0, *csv2lj=0; // two highest csv loose jets
    for(const auto j : jetsCSVranked){
      if(j->csv()>defaults::CSV_LOOSE){
        if(!csv1lj){ csv1lj = j; }
        else if(!csv2lj){ csv2lj = j; }
      }
    }
    data->fill<bool>(i_twocsvcontainb, ((csv1lj == recob) || (csv2lj == recob)) );
    if(dbg) if(csv1lj && csv2lj) std::cout << "two highest csv: " << csv1lj->csv() << " " << csv2lj->csv() << std::endl;

    // did any cands capture our matched jets ( cands = v loose conditions)
    int ncorrectcandjets = 0;
    resolved * cand = 0;
    for(auto res : resCands) {
      if(dbg) std::cout << "cand: recob and rj3 " << recob << " " << res.rj3 << std::endl;
      if(dbg) std::cout << "   recodau1, recodau2, rj1, rj2 " << recodau1 << " " << recodau2 << " " << res.rj1 << " " << res.rj2 << std::endl;
      if(recob != res.rj3) continue; // didn't capture b as the bjet
      ncorrectcandjets = std::max(ncorrectcandjets,1);
      if( (recodau1 != res.rj1) && (recodau1 != res.rj2) ) continue; // didn't capture dau1
      ncorrectcandjets = std::max(ncorrectcandjets,2);
      if( (recodau2 != res.rj1) && (recodau2 != res.rj2) ) continue; // didn't capture dau2
      ncorrectcandjets = std::max(ncorrectcandjets,3);
      cand = &res;
    }
    data->fill<int>(i_ncorrectcandjets, ncorrectcandjets);
    if(dbg) std::cout << "ncorrectcandjets: " << ncorrectcandjets << std::endl;

    // check if that cand passed selection
    if(cand) data->fill<bool>(i_correcttoppassed, isPassingResolvedTop(*cand));
    if(dbg) if(cand) std::cout << "m12, m3jet: " << cand->m12 << " " << cand->m3jet << std::endl;
    if(dbg) if(cand) std::cout << "it was a cand-> passed? " << isPassingResolvedTop(*cand) << std::endl;

    // extra bools for matched jets passing mass windows and dr
    float recom12 = (recodau1->p4() + recodau2->p4()).mass();
    data->fill<bool>(i_passwwindow, (recom12 > mWmin) && (recom12 < mWmax) );
    float recom3jet = (recodau1->p4() + recodau2->p4() + recob->p4()).mass();
    data->fill<bool>(i_passtopwindow, (recom3jet > mTopmin) && (recom3jet < mTopmax) );
    float dr123 = getRecoTopDR( recob, recodau1, recodau2 );
    data->fill<bool>(i_passdr123, dr123 < TMath::PiOver2());
    if(dbg) std::cout << "extra bools for our reco guy... recom12, recom3jet, dr123 " << recom12 << " " << recom3jet << " " << dr123 << std::endl;

  }//fillResolvedGenInfo


};//ResTreeFiller

#endif
