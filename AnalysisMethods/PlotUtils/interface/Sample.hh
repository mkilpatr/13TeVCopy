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

    TString		label;
    unsigned int	color;
    vector<TString>	filenames;
    vector<float>	xsecs;
    vector<float>	weights;
    vector<int>		types;
    vector<TString>	jsons;
    
};

#endif
