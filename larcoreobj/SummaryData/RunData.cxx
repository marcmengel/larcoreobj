////////////////////////////////////////////////////////////////////////
/// \file RunData.cxx
/// 
/// Definition of object to store run related information
/// 
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////

#include "larcoreobj/SummaryData/RunData.h"

namespace sumdata {

  //---------------------------------------------------------
  RunData::RunData() 
  : fDetName("nodetectorname")
  {
  }

  //---------------------------------------------------------
  RunData::RunData(std::string detectorName) 
  : fDetName(detectorName)
  {
  }

}  
