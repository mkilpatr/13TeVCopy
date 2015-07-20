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
    void setJES(const std::string s) {jet_scale = s;}
    std::string JES() { return jet_scale;}

    void shiftJES(std::vector<RecoJetF>jets, MomentumF *met);
protected:
    std::string jet_scale;
    std::string interpolation;
    static std::map <std::string, double> JESValues;

};
}