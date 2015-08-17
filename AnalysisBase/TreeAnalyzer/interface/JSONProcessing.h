//--------------------------------------------------------------------------------------------------
// 
// JSONProcessing
// 
// Class to process a json file and apply a lumi mask based on it.
// 
// JSONProcessing.h created on Mon Jul 20 17:00:38 CEST 2015 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_TREEANALYZER_JSONPROCESSING_H
#define ANALYSISBASE_TREEANALYZER_JSONPROCESSING_H

#include <vector>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <TString.h>

namespace cfgSet {

  class JSONProcessing {

    public :

      typedef std::pair<unsigned int, unsigned int> RunLumiPair;
      typedef std::map<unsigned int,std::vector<RunLumiPair> > RunLumiMap;

      JSONProcessing() : debug(false) {};
      JSONProcessing(const TString jsonfile, bool setdebug = false);

      void addJSONFile(const TString jsonfile);

      bool hasRunLumi(const RunLumiPair rlpair) const;

      bool hasRunLumi(unsigned int run, unsigned int lumi) { return hasRunLumi(RunLumiPair(run, lumi)); }

      void setDebug() { debug = true; }

    protected :
      RunLumiMap runlumimap;
      bool       debug;

  };

}

#endif
