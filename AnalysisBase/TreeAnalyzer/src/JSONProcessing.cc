//--------------------------------------------------------------------------------------------------
// 
// JSONProcessing
// 
// Class to process a json file and apply a lumi mask based on it.
// 
// JSONProcessing.cc created on Mon Jul 20 17:00:38 CEST 2015 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/TreeAnalyzer/interface/JSONProcessing.h"
#include "AnalysisTools/JSONSpirit/src/json_spirit.h"

using namespace cfgSet;

JSONProcessing::JSONProcessing(const TString jsonfile, bool setdebug) :
  debug(false)
{
  runlumimap.clear();
  addJSONFile(jsonfile);
  if(setdebug) setDebug();
}


void JSONProcessing::addJSONFile(const TString jsonfile)
{

  // read json file into boost property tree
  boost::property_tree::ptree jsonTree;
  boost::property_tree::read_json(jsonfile.Data(), jsonTree);

  // loop through boost property tree and fill the RunLumiMap structure with the list of good lumi ranges for each run
  for (boost::property_tree::ptree::const_iterator it = jsonTree.begin(); it!=jsonTree.end(); ++it) {
    unsigned int runNumber = boost::lexical_cast<unsigned int>(it->first);
    RunLumiMap::mapped_type &lumiPairList = runlumimap[runNumber];
    boost::property_tree::ptree lumiPairListTree = it->second;
    for (boost::property_tree::ptree::const_iterator jt = lumiPairListTree.begin(); jt!=lumiPairListTree.end(); ++jt) {      
      boost::property_tree::ptree lumiPairTree = jt->second;                                                          
      if (lumiPairTree.size()==2) {
        unsigned int firstLumi = boost::lexical_cast<unsigned int>(lumiPairTree.begin()->second.data());
        unsigned int lastLumi = boost::lexical_cast<unsigned int>((++lumiPairTree.begin())->second.data());
        lumiPairList.push_back(RunLumiPair(firstLumi,lastLumi));
      }
    }
  }

  // dump run and lumi ranges from RunLumiMap structure to verify correct json parsing
  if(debug) {
    printf("Iterating over parsed JSON:\n");
    for (RunLumiMap::const_iterator it = runlumimap.begin(); it != runlumimap.end(); ++it) {
      printf("  Run %u:\n",it->first);
      for (RunLumiMap::mapped_type::const_iterator jt = it->second.begin(); jt < it->second.end(); ++jt) {
        printf("    Lumis %u - %u\n",jt->first,jt->second);
      }
    }
  }

}

void JSONProcessing::fillRunLumiSet(const RunLumiSet &rlset)
{

  runlumimap.clear();
  unsigned int firstlumi = 0;

  for (RunLumiSet::const_iterator it = rlset.begin(); it!=rlset.end(); ++it) {
    if (firstlumi==0) firstlumi = it->second;
    RunLumiMap::mapped_type &lumiPairList = runlumimap[it->first];
    RunLumiSet::const_iterator itnext = it;
    ++itnext;
    if ( itnext==rlset.end() || itnext->first!=it->first || itnext->second!=(it->second+1) ) {
      lumiPairList.push_back(RunLumiPair(firstlumi,it->second));
      firstlumi = 0;
    }
  }

}

void JSONProcessing::dumpJSONFile(const TString filepath)
{

  fillRunLumiSet(runLumiSet());

  json_spirit::Object jsonTree;

  for (RunLumiMap::const_iterator it = runlumimap.begin(); it!=runlumimap.end(); ++it) {
    unsigned int runnum  = it->first;
    json_spirit::Array lumiPairListArray;
    const RunLumiMap::mapped_type &lumiPairList = it->second;
    for (RunLumiMap::mapped_type::const_iterator jt = lumiPairList.begin(); jt<lumiPairList.end(); ++jt) {
      json_spirit::Array lumiPairArray;
      lumiPairArray.push_back(int(jt->first));
      lumiPairArray.push_back(int(jt->second));

      lumiPairListArray.push_back(lumiPairArray);
    }
    json_spirit::Pair runPair(boost::lexical_cast<std::string>(runnum), lumiPairListArray);
    jsonTree.push_back(runPair);
  }

  std::ofstream os(filepath.Data());
  json_spirit::write(jsonTree,os);

}

bool JSONProcessing::hasRunLumi(const RunLumiPair rlpair) const
{

  // Check if a given run,lumi pair is included in the mapped lumi ranges

  // check if run is included in the map
  RunLumiMap::const_iterator it = runlumimap.find(rlpair.first);

  if (it!=runlumimap.end()) {
    // check lumis
    const RunLumiMap::mapped_type &lumiPairList = it->second;
    for (RunLumiMap::mapped_type::const_iterator jt = lumiPairList.begin(); jt<lumiPairList.end(); ++jt) {
      if (rlpair.second >= jt->first && rlpair.second <= jt->second) {
        // found lumi in accepted range
        return true;
      }
    }
  }

  return false;

}
