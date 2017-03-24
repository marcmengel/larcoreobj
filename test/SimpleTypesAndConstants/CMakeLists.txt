# ======================================================================
#
# Testing
#
# ======================================================================

cet_test( geo_types_test USE_BOOST_UNIT )
cet_test( readout_types_test USE_BOOST_UNIT )
cet_test( testPhysicalConstants )
cet_test( geo_vectors_test
  LIBRARIES
    ${ROOT_CORE}
    ${ROOT_PHYSICS}
    ${ROOT_GENVECTOR}
  USE_BOOST_UNIT
  )

