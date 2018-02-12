////////////////////////////////////////////////////////////////////////
/// \file RunData.h
///
/// Definition of object to store run related information
///
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
#ifndef SD_RUNDATA_H
#define SD_RUNDATA_H

#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include <string>

namespace sumdata {

  class RunData{

  public:

    RunData(); // Default constructor

    /// What to do when multiple `RunData` objects are found for the same run.
    /// 
    /// @throws std::runtime_error if `other` has a different `DetName()`
    void aggregate(RunData const& other);
    
  private:

    std::string  fDetName; ///< detector name

  public:
    explicit           RunData(std::string detectorName);
    std::string const& DetName() const;


  };
}


inline std::string const& sumdata::RunData::DetName() const { return fDetName; }


#endif // SD_RUNDATA_H
