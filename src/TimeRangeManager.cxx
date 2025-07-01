#include <algorithm>

#include "TimeRangeManager.hh"

// Struct

bool TimeRange::Contains(ULong64_t time, double x, double y, double positionRange) const{
  return time >= startTime && time <= endTime && TMath::Abs(x - clusterCentroidX) <= positionRange && TMath::Abs(y - clusterCentroidY) <= positionRange;
}

bool TimeRange::operator<(const TimeRange& other) const{
  return startTime < other.startTime;
} 

// TimeRangeManagerLocal

TimeRangeManagerLocal::TimeRangeManagerLocal(Double_t positionRange){
  this->positionRange = positionRange;
}

void TimeRangeManagerLocal::AddRange(ULong64_t startTime, ULong64_t endTime, Double_t clusterCentroidX, Double_t clusterCentroidY){
  if ( startTime > endTime ) std::swap(startTime, endTime);
  timeRanges.push_back( {startTime, endTime, clusterCentroidX, clusterCentroidY } );
}


bool TimeRangeManagerLocal::Contains(uint64_t time, Double_t x, Double_t y){
  for ( const auto& timeRange : timeRanges ){
    if ( timeRange.Contains(time, x, y, positionRange) ) return true;
  }
  return false;
}

const std::vector<TimeRange>& TimeRangeManagerLocal::GetRanges(){
  return timeRanges;
}

