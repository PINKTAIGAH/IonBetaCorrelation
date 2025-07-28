#ifndef UTILS_H
#define UTILS_H

#include <utility>
#include <vector>
#include <string>

#include "TObject.h"

// *************************************************************************************
// ****************************** DEFINE STRUCTS ***************************************
// *************************************************************************************

struct PromptWindow {Long64_t start; Long64_t end;};
struct ClusterRange {int min; int max; Double_t centroid;};
struct ForwardBackwardPair {int forward; int backward;};
struct XYPair {ClusterRange x; ClusterRange y;};

// *************************************************************************************
// ****************************** DEFINE FUNCTIONS *************************************
// *************************************************************************************

bool IsNoisyStrip(std::vector<int> noisyStripVector, ClusterRange decayClusterRange);
bool IsStripOverlaping(int strip, ClusterRange clusterRange);
bool IsOverlapping1D(ClusterRange clusterRange, ClusterRange otherClusterRange, bool allowAjacents=false);
bool AreClustersOverlapping(XYPair cluster, XYPair otherCluster, bool allowAjacents=false);

bool AreCentroidsCorrelated1D(ClusterRange clusterRange, ClusterRange otherClusterRange, int positionThreshold=3);
bool AreCentroidsCorrelated(XYPair cluster, XYPair otherCluster, int positionThreshold=3);

std::string GetClusterStrings(XYPair cluster);

#endif