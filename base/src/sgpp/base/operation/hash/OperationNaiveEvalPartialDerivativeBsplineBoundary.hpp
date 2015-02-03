// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#ifndef OPERATIONEVALPARTIALDERIVATIVEBSPLINEBOUNDARY_HPP
#define OPERATIONEVALPARTIALDERIVATIVEBSPLINEBOUNDARY_HPP

#include <sgpp/globaldef.hpp>
#include <sgpp/base/operation/hash/OperationNaiveEvalPartialDerivative.hpp>
#include <sgpp/base/grid/GridStorage.hpp>
#include <sgpp/base/operation/hash/common/basis/BsplineBoundaryBasis.hpp>
#include <sgpp/base/datatypes/DataVector.hpp>

namespace SGPP {
  namespace base {

    /**
     * Operation for evaluating partial derivatives of B-spline
     * linear combinations on Boundary grids.
     */
    class OperationNaiveEvalPartialDerivativeBsplineBoundary :
      public OperationNaiveEvalPartialDerivative {
      public:
        /**
         * Constructor.
         *
         * @param storage   storage of the sparse grid
         * @param degree    B-spline degree
         */
        OperationNaiveEvalPartialDerivativeBsplineBoundary(GridStorage* storage, size_t degree) :
          storage(storage), base(degree) {
        }

        /**
         * Virtual destructor.
         */
        virtual ~OperationNaiveEvalPartialDerivativeBsplineBoundary() {
        }

        /**
         * @param alpha     coefficient vector
         * @param point     evaluation point
         * @param deriv_dim dimension in which the partial derivative should be taken
         * @return          value of the partial derivative of the linear combination
         */
        virtual double evalPartialDerivative(DataVector& alpha,
                                             const std::vector<double>& point,
                                             size_t deriv_dim);

      protected:
        /// storage of the sparse grid
        GridStorage* storage;
        /// 1D B-spline basis
        SBsplineBoundaryBase base;
    };

  }
}

#endif /* OPERATIONEVALPARTIALDERIVATIVEBSPLINEBOUNDARY_HPP */
