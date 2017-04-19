////////////////////////////////////////////////////////////////////////
/// \file POTSummary.cxx
/// 
/// Definition of object to store pot related information
/// 
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////


#include "larcoreobj/SummaryData/POTSummary.h"

namespace sumdata{

  //----------------------------------------------------------------------------
  POTSummary::POTSummary()
    : totpot    (0.0)
    , totgoodpot(0.0)
    , totspills (0)
    , goodspills(0)
  {}
  
  //----------------------------------------------------------------------------
  std::ostream& operator<< (std::ostream &o, POTSummary const& a)
  {
    o << "This sub run has " << a.totspills 
      << " total spills with an exposure of "
      << a.totpot << " POT" <<std::endl
      <<" with cuts on beam quality, there are " << a.goodspills 
      <<" good spills with an exposure of " << a.totgoodpot 
      <<std::endl;

    return o;
  }

}// end namespace
