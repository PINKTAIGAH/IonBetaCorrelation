#ifndef TIME_RANGE_MANAGER_H
#define TIME_RANGE_MANAGER_H

#include <vector>

#include "TMath.h"
#include "TObject.h"

#include "utils.hh"

struct TimeRange{
  // Members
  ULong64_t startTime;
  ULong64_t endTime;
  Double_t clusterCentroidX;
  Double_t clusterCentroidY;

  bool Contains(ULong64_t time, double x, double y, double positionRange) const;
  bool operator<(const TimeRange& other) const;

};

class TimeRangeManagerLocal{

  private:
    Double_t positionRange;
    std::vector<TimeRange> timeRanges;

  public:

    TimeRangeManagerLocal(Double_t positionRange);
    void AddRange(ULong64_t startTime, ULong64_t endTime, Double_t clusterCentroidX, Double_t clusterCentroidY);
    bool Contains(uint64_t time, Double_t x, Double_t y);
    const std::vector<TimeRange>& GetRanges();

};

#endif

