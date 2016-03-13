// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#ifndef OPERATIONHESTONKLINEARBOUNDARY_HPP
#define OPERATIONHESTONKLINEARBOUNDARY_HPP

#include <sgpp/pde/algorithm/UpDownFourOpDims.hpp>

#include <sgpp/globaldef.hpp>

namespace sgpp {
namespace finance {

/**
 * Implements the Heston K-Operation (corresponds to operator K in Master's thesis), that is needed
 * the solve the multidimensional Heston
 * equation.
 *
 */
class OperationHestonKLinearBoundary : public sgpp::pde::UpDownFourOpDims {
 public:
  /**
   * Constructor
   *
   * @param storage the grid's sgpp::base::GridStorage object
   * @param coef vector that contains the constant coefficients of this operation
   */
  OperationHestonKLinearBoundary(sgpp::base::GridStorage* storage, double***** coef);

  /**
   * Destructor
   */
  virtual ~OperationHestonKLinearBoundary();

 protected:
  /**
   * Up-step in dimension <i>dim</i> for \f$(\phi_i(x),\phi_j(x))_{L_2}\f$.
   * Applies the up-part of the one-dimensional mass matrix in one dimension.
   * Computes \f[\int_{x=0}^1  \phi_i(x) \sum_{j, l_i < l_j} \alpha_j \phi_j(x) dx.\f]
   *
   * @param dim dimension in which to apply the up-part
   * @param alpha vector of coefficients
   * @param result vector to store the results in
   */
  void up(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * Down-step in dimension <i>dim</i> for \f$(\phi_i(x),\phi_j(x))_{L_2}\f$.
   * Applies the down-part of the one-dimensional mass matrix in one dimension.
   * Computes \f[\int_{x=0}^1  \phi_i(x) \sum_{j, l_i\geq l_j} \alpha_j \phi_j(x) dx.\f]
   *
   * @param dim dimension in which to apply the down-part
   * @param alpha vector of coefficients
   * @param result vector to store the results in
   */
  void down(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * down-Gradient step in dimension <i>dim</i> applies the phi dphi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimOne(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * up-Gradient step in dimension <i>dim</i> applies the phi dphi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimOne(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * down-Gradient step in dimension <i>dim</i> applies the sqrt phi phi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimTwo(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * up-Gradient step in dimension <i>dim</i> applies the sqrt phi phi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimTwo(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * down-Gradient step in dimension <i>dim</i> applies the phi dphi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimThree(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * up-Gradient step in dimension <i>dim</i> applies the phi dphi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimThree(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * down-Gradient step in dimension <i>dim</i> applies the sqrt phi phi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimFour(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * up-Gradient step in dimension <i>dim</i> applies the sqrt phi phi operation
   * in one dimension
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimFour(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result, size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimOneAndOpDimTwo(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                               size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimOneAndOpDimTwo(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                             size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimOneAndOpDimThree(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                                 size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimOneAndOpDimThree(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                               size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimOneAndOpDimFour(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                                size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimOneAndOpDimFour(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                              size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimTwoAndOpDimThree(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                                 size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimTwoAndOpDimThree(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                               size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimTwoAndOpDimFour(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                                size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimTwoAndOpDimFour(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                              size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimThreeAndOpDimFour(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                                  size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimThreeAndOpDimFour(sgpp::base::DataVector& alpha, sgpp::base::DataVector& result,
                                size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimOneAndOpDimTwoAndOpDimThree(sgpp::base::DataVector& alpha,
                                            sgpp::base::DataVector& result, size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimOneAndOpDimTwoAndOpDimThree(sgpp::base::DataVector& alpha,
                                          sgpp::base::DataVector& result, size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimOneAndOpDimTwoAndOpDimFour(sgpp::base::DataVector& alpha,
                                           sgpp::base::DataVector& result, size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimOneAndOpDimTwoAndOpDimFour(sgpp::base::DataVector& alpha,
                                         sgpp::base::DataVector& result, size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimOneAndOpDimThreeAndOpDimFour(sgpp::base::DataVector& alpha,
                                             sgpp::base::DataVector& result, size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimOneAndOpDimThreeAndOpDimFour(sgpp::base::DataVector& alpha,
                                           sgpp::base::DataVector& result, size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimTwoAndOpDimThreeAndOpDimFour(sgpp::base::DataVector& alpha,
                                             sgpp::base::DataVector& result, size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimTwoAndOpDimThreeAndOpDimFour(sgpp::base::DataVector& alpha,
                                           sgpp::base::DataVector& result, size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void downOpDimOneAndOpDimTwoAndOpDimThreeAndOpDimFour(sgpp::base::DataVector& alpha,
                                                        sgpp::base::DataVector& result, size_t dim);

  /**
   * This method does nothing (this situation doesn't come up in Heston's PDEs). Needed only to make
   * the class concrete.
   *
   * @param alpha the coefficients of the gridpoints
   * @param result vector with the result of this operation
   * @param dim the dimension in that down-Gradient is applied
   */
  void upOpDimOneAndOpDimTwoAndOpDimThreeAndOpDimFour(sgpp::base::DataVector& alpha,
                                                      sgpp::base::DataVector& result, size_t dim);
};
}  // namespace finance
}  // namespace sgpp

#endif /* OPERATIONHESTONKLINEARBOUNDARY_HPP */