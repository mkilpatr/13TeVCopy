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
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <TString.h>

namespace cfgSet {

  class JSONProcessing {

    public :

      typedef std::pair<unsigned int, unsigned int> RunLumiPair;
      typedef std::tuple<unsigned int, unsigned int, unsigned int> RunLumiEvtTriple;
      typedef std::map<unsigned int,std::vector<RunLumiPair> > RunLumiMap;
      typedef std::set<RunLumiPair> RunLumiSet;
      typedef std::set<RunLumiEvtTriple> RunLumiEvtSet;

      JSONProcessing() : debug(false) {};
      JSONProcessing(const TString jsonfile, bool setdebug = false);

      void addJSONFile(const TString jsonfile);

      void addRunLumiEventFile(const TString txtfile);

      void dumpJSONFile(const TString filepath);

      void fillRunLumiSet(const RunLumiSet &rlset);

      bool hasRunLumi(const RunLumiPair rlpair) const;

      bool hasRunLumiEvent(const RunLumiEvtTriple rletrip) const;

      bool hasRunLumi(unsigned int run, unsigned int lumi) { return hasRunLumi(RunLumiPair(run, lumi)); }

      bool hasRunLumiEvent(unsigned int run, unsigned int lumi, unsigned int evt) { return hasRunLumiEvent(RunLumiEvtTriple(run, lumi, evt)); }

      void addRunLumi(const RunLumiPair &rlpair) { runlumiset.insert(rlpair); }

      void addRunLumiEvent(const RunLumiEvtTriple &rletrip) { runlumievtset.insert(rletrip); }

      void addRunLumi(unsigned int run, unsigned int lumi) { runlumiset.insert(RunLumiPair(run, lumi)); }

      void addRunLumiEvent(unsigned int run, unsigned int lumi, unsigned int evt) { runlumievtset.insert(RunLumiEvtTriple(run, lumi, evt)); }

      void setDebug() { debug = true; }

      const RunLumiSet &runLumiSet()  const { return runlumiset; }

      const RunLumiEvtSet &runLumiEvtSet()  const { return runlumievtset; }

    protected :
      RunLumiMap runlumimap;
      RunLumiSet runlumiset;
      RunLumiEvtSet runlumievtset;
      bool       debug;

  };

}

#endif
