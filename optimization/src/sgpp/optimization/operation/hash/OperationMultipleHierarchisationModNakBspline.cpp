// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#include <sgpp/globaldef.hpp>

#include <sgpp/optimization/operation/hash/OperationMultipleHierarchisationModNakBspline.hpp>
#include <sgpp/base/operation/hash/OperationEvalModNakBsplineNaive.hpp>
#include <sgpp/base/tools/sle/solver/Auto.hpp>
#include <sgpp/base/tools/sle/system/HierarchisationSLE.hpp>

namespace sgpp {
namespace optimization {

OperationMultipleHierarchisationModNakBspline::
OperationMultipleHierarchisationModNakBspline(
    base::ModNakBsplineGrid& grid)
    : grid(grid) {}

OperationMultipleHierarchisationModNakBspline::
    ~OperationMultipleHierarchisationModNakBspline() {}

bool OperationMultipleHierarchisationModNakBspline::doHierarchisation(
    base::DataVector& nodeValues) {
  base::HierarchisationSLE system(grid);
  base::sle_solver::Auto solver;
  base::DataVector b(nodeValues);
  return solver.solve(system, b, nodeValues);
}

void OperationMultipleHierarchisationModNakBspline::doDehierarchisation(
    base::DataVector& alpha) {
  base::GridStorage& storage = grid.getStorage();
  const size_t d = storage.getDimension();
  base::OperationEvalModNakBsplineNaive opNaiveEval(storage, grid.getDegree());
  base::DataVector nodeValues(storage.getSize());
  base::DataVector x(d, 0.0);

  for (size_t j = 0; j < storage.getSize(); j++) {
    storage.getCoordinates(storage[j], x);
    nodeValues[j] = opNaiveEval.eval(alpha, x);
  }

  alpha.resize(storage.getSize());
  alpha = nodeValues;
}

bool OperationMultipleHierarchisationModNakBspline::doHierarchisation(
    base::DataMatrix& nodeValues) {
  base::HierarchisationSLE system(grid);
  base::sle_solver::Auto solver;
  base::DataMatrix B(nodeValues);
  return solver.solve(system, B, nodeValues);
}

void OperationMultipleHierarchisationModNakBspline::doDehierarchisation(
    base::DataMatrix& alpha) {
  base::GridStorage& storage = grid.getStorage();
  const size_t d = storage.getDimension();
  base::OperationEvalModNakBsplineNaive opNaiveEval(storage, grid.getDegree());
  base::DataVector nodeValues(storage.getSize(), 0.0);
  base::DataVector x(d, 0.0);
  base::DataVector alpha1(storage.getSize(), 0.0);

  for (size_t i = 0; i < alpha.getNcols(); i++) {
    alpha.getColumn(i, alpha1);

    for (size_t j = 0; j < storage.getSize(); j++) {
      storage.getCoordinates(storage[j], x);
      nodeValues[j] = opNaiveEval.eval(alpha1, x);
    }

    alpha.setColumn(i, nodeValues);
  }
}
}  // namespace optimization
}  // namespace sgpp