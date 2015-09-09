#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"

using namespace std;

namespace PlotTools {

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
	string name;
	Int_t color;
	ss >> chr >> name >> color;
	string label = line.substr(line.find('@')+1);
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

  void toUnderflow(TH1 * h) {
      h->SetBinContent(1,h->GetBinContent(1) +h->GetBinContent(0) );
      h->SetBinContent(0,0);
      if(h->GetSumw2()->fN){
        (*h->GetSumw2())[1] += (*h->GetSumw2())[0];
        h->SetBinError(0,0);
      }
  }
  void toOverflow(TH1 * h) {
    int nBins = h->GetNbinsX();
      h->SetBinContent(nBins,h->GetBinContent(nBins) +h->GetBinContent(nBins+1) );
      h->SetBinContent(nBins+1,0);
      if(h->GetSumw2()->fN){
        (*h->GetSumw2())[nBins] += (*h->GetSumw2())[nBins +1];
        h->SetBinError(nBins+1,0);
      }

  }
  void normalize(TH1 * h) {
    if(h->GetSumw2()->fN == 0) h->Sumw2();
    int nBins = h->GetNbinsX();
    double norm = h->Integral(0,nBins+1);
    if(norm)
      h->Scale(1/norm);
  }
  TH1* rebin(TH1* h, int n){
    return h->Rebin(n);
  }
  TH1* rebin(TH1* h, int n, double * bins){
    return h->Rebin(n,"",bins);
  }


  
}
