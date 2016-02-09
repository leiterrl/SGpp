// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#ifndef SGPP_OPTIMIZATION_TEST_PROBLEMS_UNCONSTRAINED_INCREASINGPOWER_HPP
#define SGPP_OPTIMIZATION_TEST_PROBLEMS_UNCONSTRAINED_INCREASINGPOWER_HPP

#include <sgpp/globaldef.hpp>

#include <cmath>
#include <sgpp/optimization/test_problems/unconstrained/UnconstrainedTestProblem.hpp>

namespace SGPP {
namespace optimization {
namespace test_problems {

/**
 * Increasing power objective function.
 *
 * Definition:
 * \f[\bar{f}(\bar{\vec{x}}) :=
 * \sum_{t=1}^d |\bar{x}_t - 2^{-t}|^{t+1}\f]
 */
class IncreasingPowerObjective : public TestScalarFunction {
 public:
  /**
   * Constructor.
   *
   * @param d     dimension of the domain
   */
  IncreasingPowerObjective(size_t d);

  /**
   * Destructor.
   */
  ~IncreasingPowerObjective() override;

  /**
   * @param x     point \f$\vec{x} \in [0, 1]^d\f$
   * @return      \f$f(\vec{x})\f$
   */
  virtual float_t evalUndisplaced(const base::DataVector& x)
  override;

  /**
   * @param[out] clone pointer to cloned object
   */
  virtual void clone(std::unique_ptr<ScalarFunction>& clone)
  const override;
};

/**
 * Increasing power unconstrained test problem.
 *
 * * Number of parameters: \f$d\f$
 * * Domain: \f$\bar{\vec{x}} \in [0, 1]^d\f$
 * * Optimal point: \f$\bar{\vec{x}}_{\text{opt}} =
 *   (2^{-t})_{t=1}^d\f$
 * * Optimal function value: \f$\bar{f}(\bar{\vec{x}}_{\text{opt}}) =
 *   0\f$
 */
class IncreasingPower : public UnconstrainedTestProblem {
 public:
  /**
   * Constructor.
   *
   * @param d     dimension of the domain
   */
  IncreasingPower(size_t d);

  /**
   * Destructor.
   */
  ~IncreasingPower() override;

  /**
   * @return  objective function of the test problem
   */
  TestScalarFunction& getObjectiveFunction() override;

  /**
   * @param[out] x minimal point
   *               \f$\vec{x}_\opt \in [0, 1]^d\f$
   * @return       minimal function value
   *               \f$f(\vec{x}_\opt)\f$
   */
  virtual float_t getOptimalPointUndisplaced(base::DataVector& x)
  override;

 protected:
  /// objective function
  IncreasingPowerObjective f;
};

}
}
}

#endif /* SGPP_OPTIMIZATION_TEST_PROBLEMS_UNCONSTRAINED_INCREASINGPOWER_HPP */