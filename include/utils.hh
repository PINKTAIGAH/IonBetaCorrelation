#ifndef UTILS_H
#define UTILS_H

#include <utility>
#include <vector>

// *************************************************************************************
// ****************************** DEFINE STRUCTS ***************************************
// *************************************************************************************

struct ClusterRange {int min; int max;};
struct ForwardBackwardPair {int forward; int backward;};

// *************************************************************************************
// ****************************** DEFINE FUNCTIONS *************************************
// *************************************************************************************

bool IsNoisyStrip(std::vector<int> noisyStripVector, ClusterRange decayClusterRange);
bool IsStripOverlaping(int strip, ClusterRange clusterRange);
bool IsOverlapping1D(ClusterRange clusterRange, ClusterRange otherClusterRange, bool allowAjacents);
bool AreClustersOverlapping(std::pair<ClusterRange, ClusterRange> cluster, std::pair<ClusterRange, ClusterRange> otherCluster, bool allowAjacents);

#endif