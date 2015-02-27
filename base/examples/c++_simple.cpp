// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#include <iostream>
// All SG++ headers
//#include "sgpp_base.hpp"

// Or, better!, include only those that are required
#include "base/datatypes/DataVector.hpp"
#include "base/grid/Grid.hpp"
#include "base/grid/GridStorage.hpp"
#include "base/grid/generation/GridGenerator.hpp"
#include "base/operation/OperationEval.hpp"
#include "base/operation/BaseOpFactory.hpp"
#include "pde/operation/PdeOpFactory.hpp"
#include "base/operation/OperationMatrix.hpp"
#include "pde/basis/periodic/operation/OperationMatrixLTwoDotExplicitPeriodic.hpp"

using namespace std;
using namespace SGPP;
using namespace SGPP::base;
using namespace SGPP::pde;

// function to interpolate
float_t f(float_t x0, float_t x1) {
  return 16.0 * (x0 - 1) * x0 * (x1 - 1) * x1;
}

int main() {
  // create a two-dimensional piecewise bi-linear grid
  int dim = 2;
  Grid* grid = Grid::createPeriodicGrid(dim);
  GridStorage* gridStorage = grid->getStorage();
  cout << "dimensionality:         " << gridStorage->dim() << endl;

  // create regular grid, level 3
  int level = 1;
  GridGenerator* gridGen = grid->createGridGenerator();
  gridGen->regular(level);
  cout << "number of grid points:  " << gridStorage->size() << endl;

  // create coefficient vector
  DataVector alpha(gridStorage->size());
  alpha.setAll(0.0);
  cout << "length of alpha-vector: " << alpha.getSize() << endl;


  OperationMatrixLTwoDotExplicitPeriodic op = OperationMatrixLTwoDotExplicitPeriodic(grid);

  alpha[0] = 1;

  DataVector res(alpha.getSize());
  op.mult(alpha, res);

  cout << res.toString() << endl;

  delete grid;
}