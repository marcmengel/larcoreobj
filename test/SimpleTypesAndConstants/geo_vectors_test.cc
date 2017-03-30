/**
 * @file   geo_vectors_test.cc
 * @brief  Test of geo_vectors_test.h utilities.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   March 23, 2017
 */

// Boost libraries
#define BOOST_TEST_MODULE ( geo_vectors_test )
#include <cetlib/quiet_unit_test.hpp> // BOOST_AUTO_TEST_CASE()
#include <boost/test/test_tools.hpp> // BOOST_CHECK(), BOOST_CHECK_CLOSE()

// LArSoft libraries
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

// ROOT libraries
#include "TVector3.h"


//------------------------------------------------------------------------------
template <typename PointA, typename PointB>
void CheckPoint(PointA const& test, PointB const& ref, std::string tag = "")
{
  const double tol = 0.001; // percent!
  
  if (!tag.empty()) BOOST_TEST_CHECKPOINT(tag);
  
  BOOST_CHECK_CLOSE(test.X(), ref.X(), tol);
  BOOST_CHECK_CLOSE(test.Y(), ref.Y(), tol);
  BOOST_CHECK_CLOSE(test.Z(), ref.Z(), tol);
  
} // CheckPoint()


//------------------------------------------------------------------------------
void test_MiddlePointAccumulator_defaultConstructor() {
  
  geo::Point_t expected(2., 4., 6.);
  
  std::vector<geo::Point_t> points {
    geo::Point_t(1., 2., 3.),
    geo::Point_t(2., 4., 6.),
    geo::Point_t(3., 6., 9.)
  };
  TVector3 another(expected.X(), expected.Y(), expected.Z());
  
  //
  // default construction, then bulk addition
  //
  geo::MiddlePointAccumulator acc;
  BOOST_CHECK(acc.empty());
  BOOST_CHECK_CLOSE(acc.weight(), 0.0, 0.001);
  // add a single point
  acc.add(another);
  BOOST_CHECK(!acc.empty());
  BOOST_CHECK_CLOSE(acc.weight(), 1.0, 0.001);
  CheckPoint(acc.middlePoint(), expected, "Single add");
  // add many points
  acc.add(points.begin(), points.end());
  BOOST_CHECK(!acc.empty());
  BOOST_CHECK_CLOSE(acc.weight(), 1.0 + points.size(), 0.001);
  CheckPoint(acc.middlePoint(), expected, "Single add plus sequence");
  
  //
  // clear test
  //
  acc.clear();
  BOOST_CHECK(acc.empty());
  acc.add(geo::Point_t{ expected.X() + 1.0, expected.Z(), expected.Y() });
  CheckPoint(
    acc.middlePoint(),
    geo::Point_t{ expected.X() + 1.0, expected.Z(), expected.Y() },
    "clear test"
    );
  
  //
  // start over (same accumulator)
  //
  acc.clear();
  // add many points
  acc.add(points.begin(), points.end());
  BOOST_CHECK(!acc.empty());
  CheckPoint(acc.middlePoint(), expected, "Sequence add");
  // add another one
  acc.add(another);
  BOOST_CHECK(!acc.empty());
  CheckPoint(acc.middlePoint(), expected, "Sequence add plus single point");
  
} // test_MiddlePointAccumulator_defaultConstructor()


void test_MiddlePointAccumulator_sequenceConstructor() {
  
  geo::Point_t expected(2., 4., 6.);
  
  std::vector<geo::Point_t> points {
    geo::Point_t(1., 2., 3.),
    geo::Point_t(2., 4., 6.),
    geo::Point_t(3., 6., 9.)
  };
  TVector3 another(expected.X(), expected.Y(), expected.Z());
  
  //
  // sequence constructor
  //
  geo::MiddlePointAccumulator acc(points.begin(), points.end());
  BOOST_CHECK(!acc.empty());
  CheckPoint(acc.middlePoint(), expected, "Sequence construction");
  // add another one
  acc.add(another);
  BOOST_CHECK(!acc.empty());
  CheckPoint(acc.middlePoint(), expected, "Sequence construction plus single");
  
} // test_MiddlePointAccumulator_sequenceConstructor()


void test_MiddlePointAccumulator_documentation_class() {
  
  geo::Point_t expected { 0.0, 1.0, 0.0 };
  /*
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
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
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */
  std::array<geo::Point_t, 4> const points = {
    geo::Point_t{ 0.0,  1.0,  2.0 },
    geo::Point_t{ 0.0, -1.0,  2.0 },
    geo::Point_t{ 0.0,  1.0, -2.0 },
    geo::Point_t{ 0.0, -1.0, -2.0 }
  };
  
  geo::MiddlePointAccumulator pointsAboveGround;
  for (auto const& point: points)
    if (point.Y() > 0.0) pointsAboveGround.add(point);
  
  if (pointsAboveGround.empty())
    throw std::runtime_error("No point above ground!");
  
  auto middleAboveGround = pointsAboveGround.middlePoint();
  
  
  static_assert(std::is_same<decltype(middleAboveGround), geo::Point_t>::value,
    "unexpected return type for geo::MiddlePointAccumulator::middlePoint()");
  CheckPoint
    (middleAboveGround, expected, "MiddlePointAccumulator::middlePoint()");
  
} // test_MiddlePointAccumulator_documentation_middlePointAs()


void test_MiddlePointAccumulator_documentation_middlePointAs() {
  
  //
  // middlePointAs()
  //
  geo::MiddlePointAccumulator accumulator;
  accumulator.add(geo::Point_t());
  
  /*
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * auto mp = accumulator.middlePointAs<TVector3>();
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */
  auto mp = accumulator.middlePointAs<TVector3>();
  
  CheckPoint(mp, geo::Point_t(), "MiddlePointAccumulator::middlePointAs()");
  
} // test_MiddlePointAccumulator_documentation_middlePointAs()


void test_MiddlePointAccumulator_documentation() {
  
  test_MiddlePointAccumulator_documentation_class();
  test_MiddlePointAccumulator_documentation_middlePointAs();
  
} // test_MiddlePointAccumulator_documentation()


//------------------------------------------------------------------------------
void test_middlePoint() {
  
  geo::Point_t expected(2., 4., 6.);
  
  std::vector<geo::Point_t> points {
    geo::Point_t(1., 2., 3.),
    geo::Point_t(2., 4., 6.),
    geo::Point_t(3., 6., 9.)
  };
  
  //
  // sequence
  //
  CheckPoint
    (geo::middlePoint(points.begin(), points.end()), expected, "iterators");
  
  
  //
  // points (initializer list)
  //
  CheckPoint(
    geo::middlePoint({ points[0], points[1], points[2] }), expected,
    "initializer list"
    );
  
  
  //
  // middlePointAs() (sequence)
  //
  auto const mp3 = geo::middlePointAs<TVector3>(points.begin(), points.end());
  static_assert(
    std::is_same<std::decay_t<decltype(mp3)>, TVector3>::value,
    "geo::middlePointAs<TVector3> does not return a TVector3!"
    );
  CheckPoint(mp3, expected, "geo::middlePointAs(sequence)");
  
  
} // test_middlePoint()


void test_middlePointAs_documentation() {
  
  /*
   * std::vector<geo::Point_t> points {
   *   geo::Point_t(1., 2., 3.),
   *   geo::Point_t(2., 4., 6.),
   *   geo::Point_t(3., 6., 9.)
   *   };
   * auto mp = geo::middlePointAs<geo::Vector_t>(points.begin(), points.end());
   */
  std::vector<geo::Point_t> points {
    geo::Point_t(1., 2., 3.),
    geo::Point_t(2., 4., 6.),
    geo::Point_t(3., 6., 9.)
    };
  auto mp = geo::middlePointAs<geo::Vector_t>(points.begin(), points.end());
  
  static_assert(std::is_same<std::decay_t<decltype(mp)>, geo::Vector_t>::value,
    "geo::middlePointAs<geo::Vector_t> result is not geo::Vector_t");
  CheckPoint(mp, geo::Vector_t(2., 4., 6.));
  
} // test_middlePointAs_documentation()


void test_middlePoint_iterators_documentation() {
  
  /*
   * std::vector<geo::Point_t> points {
   *   geo::Point_t(1., 2., 3.),
   *   geo::Point_t(2., 4., 6.),
   *   geo::Point_t(3., 6., 9.)
   *   };
   * 
   * auto mp = geo::middlePoint(points.begin(), points.end());
   */
  
  std::vector<geo::Point_t> points {
    geo::Point_t(1., 2., 3.),
    geo::Point_t(2., 4., 6.),
    geo::Point_t(3., 6., 9.)
    };
  
  auto mp = geo::middlePoint(points.begin(), points.end());
  
  static_assert(std::is_same<std::decay_t<decltype(mp)>, geo::Point_t>::value,
    "geo::middlePoint() result is not geo::Point_t");
  CheckPoint(mp, geo::Point_t(2., 4., 6.));
  
} // test_middlePoint_iterators_documentation()


void test_middlePoint_initlist_documentation() {
  
  /*
   * auto mp = geo::middlePoint
   *   ({ geo::Point_t(1., 2., 3.), geo::Point_t(3., 6., 9.) });
   * 
   */
  
  auto mp = geo::middlePoint
    ({ geo::Point_t(1., 2., 3.), geo::Point_t(3., 6., 9.) });
  
  static_assert(std::is_same<std::decay_t<decltype(mp)>, geo::Point_t>::value,
    "geo::middlePoint() result is not geo::Point_t");
  CheckPoint(mp, geo::Point_t(2., 4., 6.));
  
} // test_middlePoint_initlist_documentation()


//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(MiddlePointAccumulator_test) {
  test_MiddlePointAccumulator_defaultConstructor();
  test_MiddlePointAccumulator_sequenceConstructor();
  test_MiddlePointAccumulator_documentation();
}

BOOST_AUTO_TEST_CASE(middlePoint_test) {
  test_middlePoint();
}

BOOST_AUTO_TEST_CASE(middlePoint_documentation_test) {
  test_middlePointAs_documentation();
  test_middlePoint_iterators_documentation();
  test_middlePoint_initlist_documentation();
}

