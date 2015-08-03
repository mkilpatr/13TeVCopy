/*
 * PhotonReader.h
 *
 * Class for reading photon object information from TTree.
 *
 * Created on: May 22, 2015
 *      Author: hqu
 */

#ifndef ANALYSISTOOLS_TREEREADER_PHOTONREADER_H_
#define ANALYSISTOOLS_TREEREADER_PHOTONREADER_H_

#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/Photon.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

namespace ucsbsusy {

  class PhotonReader : public BaseReader {

    public :
    enum  Options           {
                              NULLOPT         = 0
                            , LOADRECO        = (1 <<  0)   ///< Load standard photons
                            , LOADEXTRECO     = (1 <<  1)   ///< Load extra info
                            , FILLOBJ         = (1 <<  2)   ///< Fill objects (as opposed to just pointers)
    };
    static const int defaultOptions;

    PhotonReader();
    ~PhotonReader() {}

    void load(TreeReader *treeReader, int options, std::string branchName);
    void refresh();

    public :
      std::vector<float> *    pt;
      std::vector<float> *    eta;
      std::vector<float> *    scEta;
      std::vector<float> *    phi;
      std::vector<float> *    mass;
      std::vector<float> *    r9;
      std::vector<float> *    pfdbetaiso;
      std::vector<bool>  *    isveto;
      std::vector<bool>  *    isloose;
      std::vector<bool>  *    ismedium;
      std::vector<bool>  *    istight;

      PhotonFCollection photons;

    private :
      ;

  };

}




#endif /* ANALYSISTOOLS_TREEREADER_PHOTONREADER_H_ */
