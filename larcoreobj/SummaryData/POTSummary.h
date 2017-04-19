////////////////////////////////////////////////////////////////////////
/// \file POTSummary.h
/// 
/// Definition of object to store pot related information
/// 
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////
#ifndef POTSUM_H
#define POTSUM_H

#include <iostream>

namespace sumdata {

  class POTSummary {
  public: 
    POTSummary();

    double totpot;
    double totgoodpot;

    int totspills;
    int goodspills;


    friend std::ostream& operator<< (std::ostream& o, POTSummary const& a);


  };

}
#endif //POTSUM_H
     

    
    
    

