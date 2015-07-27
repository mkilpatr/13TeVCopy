/*************************************************************************
Title   :   Jet Corrections Class
Author  :   Nicholas Morrow <nicholas.morrow@cern.ch>
File    :   JetCorrections.h
Software:   C++ Standard Library            ,
            CMSSW 7.3.1+                    ,
            UCSBSuSy 13TeVAnalysis Framework
Hardware:   LXPLUS SLC6 - AMD64

DESCRIPTION:
    What does this module do? How does it work?

USAGE:


NOTES:
    None


*************************************************************************/

/**********************    INCLUDE DIRECTIVES    ***********************/
#pragma once

#include <map>
#include <string>
#include <vector>
#include "AnalysisTools/DataFormats/interface/Jet.h"



/**************    CONSTANTS, MACROS, & DATA STRUCTURES    ***************/



/***********************    CLASS DEFINITIONS    ***********************/
namespace ucsbsusy
{
class JetCorrector
{
public:
    JetCorrector();
    ~JetCorrector();
    void setJES(const signed int s) {jet_scale = s;}
    signed int JES() { return jet_scale;}

    void shiftJES(std::vector<RecoJetF>& jets, MomentumF *met);
protected:
    enum {
        NOMINAL = 0,
        JES_UP,
        JES_DOWN
    } jet_scale_types;

    signed int jet_scale;
    static const float JESValues[];
    //static std::vector<float> JESValues;

};
}