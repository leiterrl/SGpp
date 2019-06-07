// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <sgpp/datadriven/algorithm/GridFactory.hpp>
#include <vector>

using sgpp::datadriven::GridFactory;

BOOST_AUTO_TEST_SUITE(GridFactoryTest)

BOOST_AUTO_TEST_CASE(HierarchicalParentStencil) {
  GridFactory grid;
  std::vector<std::vector<int64_t>> dimensions = {{3, 3}, {2, 2}};
  std::vector<std::vector<size_t>> expected = {
      {0, 9},  {1, 9},  {1, 10}, {2, 10}, {3, 9},  {3, 11}, {4, 9}, {4, 10}, {4, 11}, {4, 12},
      {5, 10}, {5, 12}, {6, 11}, {7, 11}, {7, 12}, {8, 12}, {0},    {1},     {2},     {3},
      {4},     {5},     {6},     {7},     {8},     {9},     {10},   {11},    {12},    {}};

  auto result = grid.getHierachicalParents(dimensions);

  for (auto interaction : expected)
    BOOST_CHECK(std::find(result.begin(), result.end(), interaction) != result.end());

  for (auto interaction : result)
    BOOST_CHECK(std::find(expected.begin(), expected.end(), interaction) != expected.end());
}

BOOST_AUTO_TEST_CASE(DirectNeigbourStencilSimple) {
  GridFactory grid;

  auto dimensions = std::vector<std::vector<int64_t>>();
  auto tmp = std::vector<int64_t>();
  tmp.push_back(2);
  tmp.push_back(1);
  dimensions.push_back(tmp);

  std::vector<std::vector<size_t>> expected = {{0, 1}, {0}, {1}, {}};

  auto result = grid.getDirectNeighbours(dimensions);

  for (auto interaction : expected)
    BOOST_CHECK(std::find(result.begin(), result.end(), interaction) != result.end());

  for (auto interaction : result)
    BOOST_CHECK(std::find(expected.begin(), expected.end(), interaction) != expected.end());
}

BOOST_AUTO_TEST_CASE(DirectNeigbourStencil) {
  GridFactory grid;

  auto dimensions = std::vector<std::vector<int64_t>>();
  auto tmp = std::vector<int64_t>();
  tmp.push_back(3);
  tmp.push_back(3);
  dimensions.push_back(tmp);

  std::vector<std::vector<size_t>> expected = {{0, 1}, {1, 2}, {3, 4}, {4, 5}, {6, 7}, {7, 8},

                                               {0, 3}, {3, 6}, {1, 4}, {4, 7}, {2, 5}, {5, 8},

                                               {0},    {1},    {2},    {3},    {4},    {5},
                                               {6},    {7},    {8},    {}};

  // auto result = grid.getDirectNeighbours(dimensions);

  // for (auto interaction : expected)
  //   BOOST_CHECK(std::find(result.begin(), result.end(), interaction) != result.end());

  // for (auto interaction : result)
  //   BOOST_CHECK(std::find(expected.begin(), expected.end(), interaction) != expected.end());
}

BOOST_AUTO_TEST_SUITE_END()
