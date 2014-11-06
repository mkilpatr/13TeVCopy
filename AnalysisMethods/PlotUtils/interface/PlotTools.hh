#ifndef TOOLS_HH
#define TOOLS_HH

#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <TString.h>
#include "AnalysisMethods/PlotUtils/interface/Sample.hh"

namespace PlotTools {

  //void initSamples(TString conf, vector<TString> &snamev, vector<Sample*> &samplev)
  void initSamples(TString conf, vector<Sample*> &samplev)
  {
    ifstream ifs;
    ifs.open(conf.Data());
    assert(ifs.is_open());

    string line;
    Int_t state=0;

    while(getline(ifs,line)) {
      if(line[0]=='#') continue;
      if(line[0]=='%') {
	state++;
	continue;
      }
      if(line[0]=='$') {
	samplev.push_back(new Sample());
	stringstream ss(line);
	string chr;
	//string sname;
	string name;
	Int_t color;
	ss >> chr >> name >> color;
	string label = line.substr(line.find('@')+1);
	//snamev.push_back(sname);
	samplev.back()->name  = name;
	samplev.back()->label = label;
	samplev.back()->color = color;
	continue;
      }
  
      if(state==0) {  // define data sample
	string fname;
	string json;
	Int_t type;
	stringstream ss(line);
	ss >> fname >> type >> json;
	samplev.back()->filenames.push_back(fname);
	samplev.back()->types.push_back(type);
	samplev.back()->xsecs.push_back(0);
	samplev.back()->jsons.push_back(json);
      } else if(state==1) {  // define MC samples
	string fname;
	Double_t xsec;
	stringstream ss(line);
	ss >> fname >> xsec;
	samplev.back()->filenames.push_back(fname);
	samplev.back()->types.push_back(0);
	samplev.back()->xsecs.push_back(xsec);
      }                                                                                                                  
    }                                                                                                                    
    ifs.close(); 
  } 
  
}

#endif
