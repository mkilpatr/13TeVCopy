/************************************************************************
Title   :   Jet Corrections Class
Author  :   Nicholas Morrow <nicholas.morrow@cern.ch>
File    :   JetCorrections.cc
Software:   C++ Standard Library            ,
            CMSSW 7.3.1+                    ,
            UCSBSuSy 13TeVAnalysis Framework
Hardware:   LXPLUS SLC6 - AMD6



************************************************************************/

/**********************    INCLUDE DIRECTIVES    ***********************/

#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "AnalysisBase/TreeAnalyzer/interface/JetCorrections.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"

/*********************    CONSTANTS AND MACROS    **********************/
#define DEBUG 0

using namespace std;
using namespace ucsbsusy;

/***********************    VARIABLE INITIALIZATIONS    ************************/

const float JetCorrector::JESValues[]={0.0,1.0,-1.0};

/*******************    METHOD IMPLEMENTATIONS    ********************/

/*************************************************************************
 * Function :   JetCorrector::JetCorrector()
 * Purpose  :   Default constructor
 ************************************************************************/
JetCorrector::JetCorrector() : jet_scale(NOMINAL)
{
}

/*************************************************************************
 * Function :   JetCorrector::JetCorrector()
 * Purpose  :   Default destructor
 *************************************************************************/
JetCorrector::~JetCorrector()
{
}

/*************************************************************************
 * Function :   JetCorrector::shiftJES()
 * Purpose  :   Shifts jet energy scale - Scales jet pt values by an amount
 *              specified by scaleFactor
 * Input    :   std::vector<recoJetF *> jets - Vector of pointers to jets
 *                                              to be scaled.
 *              MomentumF *met               - Pointer to MET vector
 * Returns  :   void
 *************************************************************************/
void JetCorrector::shiftJES(std::vector<RecoJetF>& jets, MomentumF *const met)
{
    float JEC_scale_factor;

#if DEBUG
    float pt_raw_old = 0, pt_cor_old = 0;
    cout << "\nEVENT CONTAINS " << jets.size() << " JETS." << endl;
    cout << "ORIGINAL MET: " << met->pt() << endl;
    cout << "JET INDEX\t\tORIGINAL RAW PT\t\tUNSCALED CORR PT\tSCALED RAW PT\t\tSCALED CORR PT\t\tETA\t\tCORR FACTOR\t\t\
RAW REL ADJ" << endl;
    if (JESValues[jet_scale]) {
        for ( RecoJetF i : jets) {
            /*  Loop over all jets in vector and scale PT by scale factor.  */
            #if !DEBUG 
                if (!(i.uncertainty())) {continue;}
            #endif
            if (i.uncertainty() < 0) {
                cout << "JetCorrector::shiftJEC(): Possible bad data. Uncertainty value of " <<
                     i.uncertainty() << " in jet " << i << "failed sanity check." << endl;
            }
            if(i.uncertainty() == 0.0) continue;
#if DEBUG
            pt_raw_old = i.pt_raw(); pt_cor_old = i.pt();
            cout << "MET was: " << met->pt() << endl;
#endif
            /* Remove unshifted jet PT from MET vector */
            met->setP4((i.p4()) + (met->p4()));
            /* Calculate JEC scaling factor and shift by JEC uncertainty */
            JEC_scale_factor = JESValues[jet_scale] * (1 + (i.uncertainty() / i.pt()));
            /* Apply shifted correction factor to jet PT  */
            i.setP4(JEC_scale_factor * (i.p4()));
            /* Update MET with scaled and corrected PT */
            met->setP4((met->p4()) - (i.p4()));
#if DEBUG
            cout << i.index() << "\t\t\t" << pt_raw_old  << "\t\t\t"
                 << pt_cor_old << "\t\t\t" << i.pt_raw() << "\t\t\t"
                 << i.pt()    << "\t\t\t" << i.eta()    << "\t\t"
                 << ((i.uncertainty()) ? i.uncertainty() : JESValues[jet_scale]) << "\t\t\t"
                 << (int)(0.5f + (100.0f * ((i.pt_raw() - pt_raw_old) / pt_raw_old )))
                 << "%"  << endl;
            cout << "MET is: " << met->pt() << endl;
#endif
        }
    }
    else {return;}
#if DEBUG
      cout << "NEW MET: " << met->pt() << endl;
#endif
}
