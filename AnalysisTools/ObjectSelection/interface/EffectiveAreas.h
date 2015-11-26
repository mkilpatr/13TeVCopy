#ifndef AnalysisTools_ObjectSelection_AnalysisToolsEffectiveAreas_H
#define AnalysisTools_ObjectSelection_AnalysisToolsEffectiveAreas_H

#include <vector>

#include <string>

class AnalysisToolsEffectiveAreas {

public:
  // Constructor, destructor
  AnalysisToolsEffectiveAreas(const std::string& filename);
  ~AnalysisToolsEffectiveAreas();

  // Accessors
  const float getEffectiveArea(float eta) const;

  // Utility functions
  void printEffectiveAreas() const;
  void checkConsistency() const;

private:
  // Data members
  const std::string  filename_;  // effective areas source file name
  std::vector<float> absEtaMin_; // low limit of the eta range
  std::vector<float> absEtaMax_; // upper limit of the eta range
  std::vector<float> effectiveAreaValues_; // effective area for this eta range

};

#endif
