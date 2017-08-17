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

// C/C++ standard library
#include <array>


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
    <ROOT::Math::Cartesian3D<Length_t>, GlobalCoords>;
  
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
    <ROOT::Math::Cartesian3D<Length_t>, GlobalCoords>;
  
  /// Type for representation of space rotations.
  using Rotation_t = ROOT::Math::Rotation3D;
  
  /// @}
  
  
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
  
  /**
   * @brief Helper class to compute the middle point in a point set.
   * 
   * This class accumulates cartesian points and returns their middle point when
   * asked.
   * 
   * In the following example, only the points from a list (`points`) which
   * have _y_ coordinate larger than 0 are averaged, all with the same weight:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * std::array<geo::Point_t, 4> const points = {
   *   geo::Point_t{ 0.0,  1.0,  2.0 },
   *   geo::Point_t{ 0.0, -1.0,  2.0 },
   *   geo::Point_t{ 0.0,  1.0, -2.0 },
   *   geo::Point_t{ 0.0, -1.0, -2.0 }
   * };
   * 
   * geo::MiddlePointAccumulator pointsAboveGround;
   * for (auto const& point: points)
   *   if (point.Y() > 0.0) pointsAboveGround.add(point);
   * 
   * if (pointsAboveGround.empty())
   *   throw std::runtime_error("No point above ground!");
   * 
   * auto middleAboveGround = pointsAboveGround.middlePoint();
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * Note the check to make sure that there are points that fulfil the
   * requirement.
   * 
   */
  class MiddlePointAccumulator {
    std::array<Length_t, 3U> fSums; ///< Sum of each of the point components.
    double fW = 0.0; ///< Total weight.
    
      public:
    /// Default constructor: starts with no accumulated point.
    MiddlePointAccumulator() { fSums.fill(0.); }
    
    /**
     * @brief Constructor: starts with accumulating a sequence of points.
     * @tparam BeginIter type of iterator to a point type compatible with add()
     * @tparam EndIter type of end iterator
     * @param begin iterator to the first point to be added
     * @param end iterator after the last point to be added
     * @see add()
     */
    template <typename BeginIter, typename EndIter>
    MiddlePointAccumulator(BeginIter begin, EndIter end)
      : MiddlePointAccumulator()
      { add(begin, end); }
    
    
    /// @{
    /// @name Result query
    
    /// Returns whether the total weight is zero (usually means no points).
    bool empty() const { return fW == 0.0; }
    
    /// Returns the total weight (number of points if all have weight 1).
    double weight() const { return fW; }
    
    /**
     * @brief Returns the middle point, NaN components if no point.
     * @tparam Point type of the output point
     * 
     * The type of return point must be specified as template argument, e.g.
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * auto mp = accumulator.middlePointAs<TVector3>();
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * The `Point` type is required to have a constructor with the three
     * cartesian components as arguments.
     */
    template <typename Point>
    Point middlePointAs() const
      { return { fSums[0] / fW, fSums[1] / fW, fSums[2] / fW }; }
    
    /// Returns the middle point as a Point_t, NaN components if no point.
    Point_t middlePoint() const
      { return middlePointAs<Point_t>(); }
    
    
    /// @}
    
    /// @{
    /// @name Addition of points.
    
    /**
     * @brief Accumulates a point.
     * @tparam Point point type, required to have X(), Y() and Z() accessors
     * @param p point to be included
     * 
     * The point is added with weight 1.
     */
    template <typename Point>
    void add(Point const& p)
      { fSums[0] += p.X(); fSums[1] += p.Y(); fSums[2] += p.Z(); fW += 1.0; }
    
    /**
     * @brief Accumulates a point.
     * @tparam Point point type, required to have X(), Y() and Z() accessors
     * @param p point to be included
     * @param weight the relative weight of this point
     */
    template <typename Point>
    void add(Point const& p, double weight)
      {
        fSums[0] += weight * p.X();
        fSums[1] += weight * p.Y();
        fSums[2] += weight * p.Z();
        fW += weight;
      }
    
    /**
     * @brief Adds a sequence of points.
     * @tparam BeginIter type of iterator to a point type compatible with add()
     * @tparam EndIter type of end iterator
     * @param begin iterator to the first point to be added
     * @param end iterator after the last point to be added
     * 
     * Each point is added with weight 1.0.
     */
    template <typename BeginIter, typename EndIter>
    void add(BeginIter begin, EndIter end)
      { std::for_each(begin, end, [this](auto const& p){ this->add(p); }); }
    
    /// Resets the status of the object to no accumulated points.
    void clear() { fSums.fill(0.); fW = 0.0; }
    
    /// @}
    
  }; // MiddlePointAccumulator()
  
  
  /// @{
  /// @name Middle point functions
  
  /**
   * @brief Returns the middle of the specified points.
   * @tparam Point cartesian-represented point type with 3-component constructor
   * @tparam BeginIter type of iterator to a point type compatible with add()
   * @tparam EndIter type of end iterator
   * @param begin iterator to the first point to be averaged
   * @param end iterator after the last point to be averaged
   * @return an object of type `Point` with the value of the middle point
   * 
   * Example of usage:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * std::vector<geo::Point_t> points {
   *   geo::Point_t(1., 2., 3.),
   *   geo::Point_t(2., 4., 6.),
   *   geo::Point_t(3., 6., 9.)
   *   };
   * 
   * auto mp = geo::middlePointAs<geo::Vector_t>(points.begin(), points.end());
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * The variable `mp` of the example will be of type `geo::Vector_t` (converted
   * component by components from a `geo::Point_t`).
   * 
   */
  template <typename Point, typename BeginIter, typename EndIter>
  Point middlePointAs(BeginIter begin, EndIter end)
    { return MiddlePointAccumulator(begin, end).middlePointAs<Point>(); }
  
  /**
   * @brief Returns the middle of the specified points.
   * @tparam BeginIter type of iterator to a point type compatible with add()
   * @tparam EndIter type of end iterator
   * @param begin iterator to the first point to be averaged
   * @param end iterator after the last point to be averaged
   * @return an object of type `Point_t` with the value of the middle point
   * 
   * Example of usage:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * std::vector<geo::Point_t> points {
   *   geo::Point_t(1., 2., 3.),
   *   geo::Point_t(2., 4., 6.),
   *   geo::Point_t(3., 6., 9.)
   *   };
   * 
   * auto mp = geo::middlePoint(points.begin(), points.end());
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * The variable `mp` of the example will be of type `geo::Point_t`.
   * 
   */
  template <typename BeginIter, typename EndIter>
  Point_t middlePoint(BeginIter begin, EndIter end)
    { return middlePointAs<Point_t>(begin, end); }
  
  /**
   * @brief Returns the middle of the specified points.
   * @tparam Point cartesian-represented point type with 3-component constructor
   *         and X(), Y() and Z() accessors.
   * @param points the list of points to be included
   * @return the value of the middle point
   * 
   * Example of usage:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * auto mp = geo::middlePoint
   *   ({ geo::Point_t(1., 2., 3.), geo::Point_t(3., 6., 9.) });
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * The variable `mp` will contain the middle point between the two specified
   * in the initializer list.
   * 
   */
  template <typename Point>
  Point middlePoint(std::initializer_list<Point> points)
    {
      return MiddlePointAccumulator(points.begin(), points.end())
        .middlePointAs<Point>();
    }
  
  /// @}
  
} // namespace geo

#endif // LARCOREOBJ_SIMPLETYPESANDCONSTANTS_GEO_VECTORS_H
