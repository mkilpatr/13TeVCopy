//--------------------------------------------------------------------------------------------------
//
// Simple class to hold information about samples needed for plotting.
//
//--------------------------------------------------------------------------------------------------

#ifndef SAMPLE_HH
#define SAMPLE_HH

#include <iostream>
#include <vector>
#include <TString.h>

class Sample
{
  public:
    Sample(){}
    ~Sample(){}

    TString      name;
    TString      label;
    unsigned int color;
    std::vector<TString> filenames;
    std::vector<double>  xsecs;
    std::vector<double>  weights;
    std::vector<int>     types;
    std::vector<TString> jsons;
    
};

#endif
