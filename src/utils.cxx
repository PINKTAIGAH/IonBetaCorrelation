#include <utility>

#include "utils.hh"
#include "Logger.hh"
#include "ArgumentParser.hh"

bool IsStripOverlaping(int strip, ClusterRange clusterRange){
  return strip >= clusterRange.min && strip < clusterRange.max;
}

bool IsNoisyStrip(std::vector<int> noisyStripVector, ClusterRange decayClusterRange){
  // Function which will loop over vector containing noisy strips and check
  // if current event occured in a noisy strip

  bool isNoisy = false; // Flag telling if event is from noisy strip

  // Loop over vector if not empty
  if (!noisyStripVector.empty()){
    // Loop for each noisy strip
    for (auto& noisyStrip : noisyStripVector){
      // If event strip corresponds to a noisy strip, change flag and break
      if ( IsStripOverlaping(noisyStrip, decayClusterRange) ){
        isNoisy = true;
        if ( ArgumentParser::Instance().HasFlag("v") ) Logger::Log("Found noisy strip: " + std::to_string(noisyStrip) + " Cluster: " + "(" + std::to_string(decayClusterRange.min) + "," + std::to_string(decayClusterRange.max) + ")", Logger::Level::DEBUG);
        break;
      }
    }
  }

  return isNoisy;
}

bool IsOverlapping1D(ClusterRange clusterRange, ClusterRange otherClusterRange, bool allowAjacents){

  if (allowAjacents) return clusterRange.max >= otherClusterRange.min && otherClusterRange.max >= clusterRange.min;
  else return clusterRange.max > otherClusterRange.min && otherClusterRange.max > clusterRange.min;
  
}

bool AreClustersOverlapping(XYPair cluster, XYPair otherCluster, bool allowAjacents){

  return IsOverlapping1D(cluster.x, otherCluster.x, allowAjacents) && IsOverlapping1D(cluster.y, otherCluster.y, allowAjacents);
}

std::string GetClusterStrings(XYPair cluster){
  return "X(" + std::to_string(cluster.x.min) + "," + std::to_string(cluster.x.max) + ") Y("+ std::to_string(cluster.y.min) + "," + std::to_string(cluster.y.max) + ") ";
}

