// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#ifndef OPERATIONHESTONCLINEAR_HPP
#define OPERATIONHESTONCLINEAR_HPP

#include <sgpp/pde/algorithm/UpDownTwoOpDims.hpp>

#include <sgpp/globaldef.hpp>

namespace sgpp {
namespace finance {

/**
 * Implements the Heston C-Operation (corresponds to operator C in Master's thesis), that is needed
 * the solve the multidimensional Heston
 * equation, on grids with fix Dirichlet-0-Boundaries.
 *
 */
class OperationHestonCLinear : public sgpp::pde::UpDownTwoOpDims {
 public:
  /**
   * Constructor
   *
   * @param storage the grid's sgpp::base::GridStorage object
   * @param coef vector that contains the constant coefficients of this operation
   */
  OperationHestonCLinear(sgpp::base::GridStorage* storage, sgpp::base::DataMatrix& coef);

  /**
   * Destructor
   */
  virtual ~OperationHestonCLinear();

 protected:
  void mult(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result);

  /**
   * Up-step in dimension <i>dim</i> for \f$(\phi_i(x),\phi_j(x))_{L_2}\f$.
   * Applies the up-part of the one-dimensional mass matrix in one dimension.
   * Computes \f[\int_{x=0}^1  \phi_i(x) \sum_{j, l_i < l_j} \alpha_j \phi_j(x) dx.\f]
   *
   * @param dim dimension in which to apply the up-part
   * @param alpha vector of coefficients
   * @param result vector to store the results in
   */
  virtual void up(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * Down-step in dimension <i>dim</i> for \f$(\phi_i(x),\phi_j(x))_{L_2}\f$.
   * Applies the down-part of the one-dimensional mass matrix in one dimension.
   * Computes \f[\int_{x=0}^1  \phi_i(x) \sum_{j, l_i\geq l_j} \alpha_j \phi_j(x) dx.\f]
   *
   * @param dim dimension in which to apply the down-part
   * @param alpha vector of coefficients
   * @param result vector to store the results in
   */
  virtual void down(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * down-Gradient step in dimension <i>dim</i> applies the phi dphi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  virtual void downOpDimOne(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                            size_t dim);

  /**
   * up-Gradient step in dimension <i>dim</i> applies the phi dphi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that up-Gradient is applied
   */
  virtual void upOpDimOne(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                          size_t dim);

  /**
   * down-Gradient step in dimension <i>dim</i> applies the x dphi phi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  virtual void downOpDimTwo(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                            size_t dim);

  /**
   * up-Gradient step in dimension <i>dim</i> applies the x dphi phi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that up-Gradient is applied
   */
  virtual void upOpDimTwo(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                          size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Only required to
   * make the class complete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimOneAndOpDimTwo(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                               size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Only required to
   * make the class complete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that up-Gradient is applied
   */
  void upOpDimOneAndOpDimTwo(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                             size_t dim);
};
}  // namespace finance
}  // namespace sgpp

#endif /* OPERATIONHESTONCLINEAR_HPP */