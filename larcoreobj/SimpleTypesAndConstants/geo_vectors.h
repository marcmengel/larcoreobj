/**
 * @file   larcoreobj/SimpleTypesAndConstants/geo_vectors.h
 * @brief  Definitions of geometry vector data types.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   March 23, 2017
 * 
 * This library depends on ROOT GenVector.
 * In the CET link list in `CMakeLists.txt`, link to `${ROOT_GENVECTOR}`.
 * 
 * Additional utilities are available from `geo_vectors_utils.h`.
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
  
  /**
   * @brief Type for representation of momenta in 3D space.
   * 
   * A vector represents a direction and intensity, or a displacement respect to
   * an unspecified starting point.
   * Vectors can be added or subtracted, resulting in still a vector.
   * Their modulus can also be scaled.
   * 
   */
  using Vector_t = ROOT::Math::DisplacementVector3D
    <ROOT::Math::Cartesian3D<double>, ROOT::Math::GlobalCoordinateSystemTag>;
  
  /// Type used for coordinates and distances. They are measured in centimeters.
  using Length_t = typename Vector_t::Scalar;
  
  /**
   * @brief Type for representation of position in physical 3D space.
   * 
   * A point represents a position in 3D space.
   * As such, it makes no sense to add points, and the difference between two
   * points is not a point any more (it is, in fact, a `geo::Vector_t`).
   * Scaling and norm of a point also have no meaning.
   * 
   * A vector can be added to a point, resulting in another point.
   * 
   * Note that `middlePoint()` function and `MiddlePointAccumulator` class are
   * provided to facilitate the computation of a middle point using any type
   * of vector and in particular `geo::Point_t`.
   * 
   */
  using Point_t = ROOT::Math::PositionVector3D
    <ROOT::Math::Cartesian3D<double>, ROOT::Math::GlobalCoordinateSystemTag>;
  
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
  
  /// Type for representation of space rotations.
  using Rotation_t = ROOT::Math::Rotation3D;
  
  
  //--------------------------------------------------------------------------
  /// Returns a x axis vector of the specified type.
  template <typename Vector = Vector_t>
  constexpr Vector Xaxis() { return { 1.0, 0.0, 0.0 }; }
  
  /// Returns a y axis vector of the specified type.
  template <typename Vector = Vector_t>
  constexpr Vector Yaxis() { return { 0.0, 1.0, 0.0 }; }
  
  /// Returns a z axis vector of the specified type.
  template <typename Vector = Vector_t>
  constexpr Vector Zaxis() { return { 0.0, 0.0, 1.0 }; }
  
  /// Returns a origin position with a point of the specified type.
  template <typename Point = Point_t>
  constexpr Point origin() { return { 0.0, 0.0, 0.0 }; }
  
  
  //--------------------------------------------------------------------------
  /// @{
  /// @name Geometry vector output to stream.
  
  /**
   * @brief Prints a geometry vector into a stream
   * @tparam Stream type of output stream
   * @param out the output stream
   * @param p the 3D point to be printed
   * @return a reference to the output stream
   * 
   * The point is printed with the format: `{ x, y, z }`.
   * No end-of-line character is emitted.
   */
  template <typename Stream>
  Stream& operator<< (Stream&& out, Point_t const& p)
    {
      out << "{ " << p.X() << ", " << p.Y() << ", " << p.Z() << " }";
      return out;
    }
  
  /**
   * @brief Prints a geometry vector into a stream
   * @tparam Stream type of output stream
   * @param out the output stream
   * @param v the 3D vector to be printed
   * @return a reference to the output stream
   * 
   * The vector is printed with the format: `{ x, y, z }`.
   * No end-of-line character is emitted.
   */
  template <typename Stream>
  Stream& operator<< (Stream&& out, Vector_t const& v)
    {
      out << "{ " << v.X() << ", " << v.Y() << ", " << v.Z() << " }";
      return out;
    }
  
  /// @}
  
  //----------------------------------------------------------------------------

} // namespace geo


#endif // LARCOREOBJ_SIMPLETYPESANDCONSTANTS_GEO_VECTORS_H
