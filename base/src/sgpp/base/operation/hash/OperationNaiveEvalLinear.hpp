// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#ifndef OPERATIONNAIVEEVALLINEAR_HPP
#define OPERATIONNAIVEEVALLINEAR_HPP

#include <sgpp/globaldef.hpp>
#include <sgpp/base/operation/hash/OperationNaiveEval.hpp>
#include <sgpp/base/operation/hash/common/basis/LinearBasis.hpp>
#include <sgpp/base/grid/GridStorage.hpp>
#include <sgpp/base/datatypes/DataVector.hpp>

namespace SGPP {
  namespace base {

    class OperationNaiveEvalLinear : public OperationNaiveEval {
      public:
        /**
         * Constructor.
         *
         * @param storage   storage of the sparse grid
         */
        OperationNaiveEvalLinear(GridStorage* storage) : storage(storage) {
        }

        /**
         * Virtual destructor.
         */
        virtual ~OperationNaiveEvalLinear() {
        }

        /**
         * @param alpha     coefficient vector
         * @param point     evaluation point
         * @return          value of linear combination
         */
        virtual double eval(DataVector& alpha, std::vector<double>& point);

      protected:
        /// storage of the sparse grid
        GridStorage* storage;
        /// 1D linear basis
        SLinearBase base;
    };

  }
}

#endif /* OPERATIONNAIVEEVALLINEAR_HPP */
