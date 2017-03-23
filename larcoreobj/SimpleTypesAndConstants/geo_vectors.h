/**
 * @file   geo_vectors.h
 * @brief  Definitions of geometry vector data types.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   March 23, 2017
 * 
 * This library depends on ROOT GenVector.
 * In the CET link list in `CMakeLists.txt`, link to `${ROOT_GENVECTOR}`.
 * 
 */

#ifndef LARCOREOBJ_SIMPLETYPESANDCONSTANTS_GEO_VECTORS_H
#define LARCOREOBJ_SIMPLETYPESANDCONSTANTS_GEO_VECTORS_H

// ROOT libraries
#include "Math/GenVector/CoordinateSystemTags.h"
#include "Math/GenVector/Cartesian3D.h"
#include "Math/GenVector/PositionVector3D.h"
#include "Math/GenVector/DisplacementVector3D.h"
#include "Math/GenVector/Rotation3D.h"


namespace geo {
  
  /**
   * These types constitute the basic objects the geometry works with.
   * 
   * All interfaces should support them.
   * 
   * @note As this requires some transition, please report any interface missing
   *       support for these types by opening a "necessary maintenance" request
   *       in the LArSoft issue tracker at
   *       https://cdcvs.fnal.gov/redmine/projects/larsoft/issues .
   *       Even if there are plenty.
   * 
   */
  /// @{
  
  /// Type used for coordinates and distances. They are measured in centimeters.
  using Length_t = double;
  
  /**
   * @brief Tag for vectors in the global coordinate system.
   * 
   * A vector tagged as "global" is expected to be represented in the global
   * (or "world") coordinate system.
   * 
   * That system is the one the detector geometry is described in, and it is
   * defined by the GDML detector description.
   * The linear coordinates are described in _centimeters_.
   * 
   */
  using GlobalCoords = ROOT::Math::GlobalCoordinateSystemTag;
  
  /// Type for representation of position in physical 3D space.
  using Point_t = ROOT::Math::PositionVector3D
    <ROOT::Math::Cartesian3D<Length_t>, GlobalCoords>;
  
  /// Type for representation of momenta in 3D space.
  using Vector_t = ROOT::Math::DisplacementVector3D
    <ROOT::Math::Cartesian3D<Length_t>, GlobalCoords>;
  
  /// Type for representation of space rotations.
  using Rotation_t = ROOT::Math::Rotation3D;
  
  /// @}
  
} // namespace geo

#endif // LARCOREOBJ_SIMPLETYPESANDCONSTANTS_GEO_VECTORS_H
