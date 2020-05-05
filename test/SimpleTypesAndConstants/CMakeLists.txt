# ======================================================================
#
# Testing
#
# ======================================================================

cet_test( geo_types_test USE_BOOST_UNIT )
cet_test( readout_types_test USE_BOOST_UNIT )
cet_test( geo_types_fhicl_test USE_BOOST_UNIT
  LIBRARIES
    ${FHICLCPP}
    ${CETLIB_EXCEPT}
  )
cet_test( readout_types_fhicl_test USE_BOOST_UNIT
  LIBRARIES
    ${FHICLCPP}
    ${CETLIB_EXCEPT}
  )
cet_test( testPhysicalConstants )
