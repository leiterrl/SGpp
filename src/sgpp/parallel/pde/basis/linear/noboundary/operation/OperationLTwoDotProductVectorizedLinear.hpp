/******************************************************************************
 * Copyright (C) 2013 Technische Universitaet Muenchen                         *
 * This file is part of the SG++ project. For conditions of distribution and   *
 * use, please see the copyright notice at http://www5.in.tum.de/SGpp          *
 ******************************************************************************/
// @author Alexander Heinecke (Alexander.Heinecke@mytum.de)

#ifndef OPERATIONLTWODOTPRODUCTVECTORIZEDLINEAR_HPP
#define OPERATIONLTWODOTPRODUCTVECTORIZEDLINEAR_HPP

#include "base/operation/OperationMatrix.hpp"
#include "base/datatypes/DataMatrix.hpp"
#include "base/grid/Grid.hpp"


#include "parallel/tools/TypesParallel.hpp"

#if defined(__SSE4_2__) || defined(__AVX__)
#include <immintrin.h>
#endif
#if defined(__FMA4__)
#include <x86intrin.h>
#endif

#ifdef __USEAVX128__
#undef __AVX__
#endif

namespace sg {
    namespace parallel {
        
        /**
	 * Implementation for linear functions of LTwoDotProduct Operation, linear grids without boundaries
         *
         * @version $HEAD$
         */
        class OperationLTwoDotProductVectorizedLinear: public sg::base::OperationMatrix {
        private:
            
            sg::base::GridStorage* storage;
            sg::base::DataMatrix* level_;
            sg::base::DataMatrix* level_int_;
            sg::base::DataMatrix* index_;
            sg::base::DataVector* lcl_q_;            
            sg::base::DataVector* alpha_padded_;
            sg::base::DataVector* constants_;
			
			sg::base::DataVector** gradient_temp; 
			sg::base::DataVector** l2dot_temp; 
			
#if defined(STORE_MATRIX)
		sg::base::DataMatrix* operation_result_matrix_;
		bool operation_result_generated_;
#endif

			int process_count;
			int process_index;

			std::vector<int> all_i_start;
			std::vector<int> all_i_size;
			
			std::vector<int> send_start;
			std::vector<int> send_size;
			
			std::vector<int> recv_start;
			std::vector<int> recv_size;


            void init_constants();
			void init_grid_storage();
            
			double l2dot(size_t i, size_t j, size_t dim);
            
        public:
            /**
             * Construtor of OperationLTwoDotProductVectorizedLinear
             *
             * @param storage Pointer to the grid's gridstorage obejct
             */
            OperationLTwoDotProductVectorizedLinear(sg::base::GridStorage* storage);
            
            /**
             * Construtor of OperationLTwoDotProductVectorizedLinear
             *
             * @param storage Pointer to the grid's gridstorage obejct
             * @param lambda Vector which contains pre-factors for every dimension of the operator
             */
            OperationLTwoDotProductVectorizedLinear(sg::base::GridStorage* storage, sg::base::DataVector& lambda);
            
            /**
             * Destructor
             */
            virtual ~OperationLTwoDotProductVectorizedLinear();
            
            virtual void mult(sg::base::DataVector& alpha, sg::base::DataVector& result);
			
			virtual void reset();
        };
        
    }
    
}

#endif /* OPERATIONLAPLACEVECTORIZEDLINEAR_HPP */
