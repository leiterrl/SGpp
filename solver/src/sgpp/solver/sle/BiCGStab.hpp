// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#ifndef BICGSTAB_HPP
#define BICGSTAB_HPP

#include <sgpp/solver/SLESolver.hpp>
#include <sgpp/base/operation/hash/OperationMatrix.hpp>
#include <sgpp/base/datatypes/DataVector.hpp>

#include <sgpp/globaldef.hpp>

#include <iostream>

namespace sgpp {
namespace solver {

class BiCGStab : public SLESolver {
 public:
  /**
   * Std-Constructor
   */
  BiCGStab(size_t imax, double epsilon);

  /**
   * Std-Destructor
   */
  virtual ~BiCGStab();

  /**
   * max_threashold is ignored in this solver
   *
   * Reference:
   * http://www.iue.tuwien.ac.at/phd/heinreichsberger/node70.html
   * http://www.numerik.math.tu-graz.ac.at/kurse/lgs/SIMNET6.pdf
   * http://netlib.org
   */
  virtual void solve(sgpp::base::OperationMatrix& SystemMatrix, sgpp::base::DataVector& alpha,
                     sgpp::base::DataVector& b, bool reuse = false, bool verbose = false,
                     double max_threshold = -1.0);
};

}  // namespace solver
}  // namespace sgpp

#endif /*BICGSTAB_HPP */
