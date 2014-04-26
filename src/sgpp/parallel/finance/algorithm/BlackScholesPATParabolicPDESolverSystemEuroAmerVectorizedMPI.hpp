/* ****************************************************************************
* Copyright (C) 2013 Technische Universitaet Muenchen                         *
* This file is part of the SG++ project. For conditions of distribution and   *
* use, please see the copyright notice at http://www5.in.tum.de/SGpp          *
**************************************************************************** */
// @author Alexander Heinecke (Alexander.Heinecke@mytum.de)
// @author Jacob Jepsen (jepsen@diku.dk)

#ifndef BLACKSCHOLESPATPARABOLICPDESOLVERSYSTEMEUROAMERVECTORIZEDMPI_HPP
#define BLACKSCHOLESPATPARABOLICPDESOLVERSYSTEMEUROAMERVECTORIZEDMPI_HPP

#include "parallel/pde/operation/OperationParabolicPDESolverSystemDirichletCombined.hpp"
#include "parallel/pde/operation/OperationParabolicPDEMatrixCombined.hpp"

#include "base/grid/Grid.hpp"
#include "base/datatypes/DataVector.hpp"
#include "base/datatypes/DataMatrix.hpp"

namespace sg {
  namespace parallel {

    /**
     * This class implements the ParabolicPDESolverSystem for the BlackScholes
     * Equation based on a combined and vectorized application of L and Mass matrix.
     *
     * Here a European and American Options with fix Dirichlet boundaries are solved.
     *
     * In addition a principal axis transformation is performed in order
     * to reduce the computational effort.
     */
    class BlackScholesPATParabolicPDESolverSystemEuroAmerVectorizedMPI : public sg::parallel::OperationParabolicPDESolverSystemDirichletCombined {
      protected:
        /// the Laplace Operation, on boundary grid
        sg::base::OperationMatrix* OpLaplaceBound;
        /// the LTwoDotProduct Operation (Mass Matrix), on boundary grid
        sg::base::OperationMatrix* OpLTwoBound;
        /// the Laplace Operation, on Inner grid
        sg::base::OperationMatrix* OpLaplaceInner;
        /// the LTwoDotProduct Operation (Mass Matrix), on Inner grid
        sg::base::OperationMatrix* OpLTwoInner;
        /// the combination of the LTwoDotProduct Operation (Mass Matrix) and the Laplace Operation, on Inner grid
        sg::parallel::OperationParabolicPDEMatrixCombined* OpLTwoDotLaplaceInner;
        /// the combination of the LTwoDotProduct Operation (Mass Matrix) and the Laplace Operation, on Bound grid
        sg::parallel::OperationParabolicPDEMatrixCombined* OpLTwoDotLaplaceBound;
        /// Pointer to the lambda (coefficients of the needed Laplace operator)
        sg::base::DataVector* lambda;
        /// Eigenvectors of the covariance matrix
        sg::base::DataMatrix* eigenvecs;
        /// Pointer to the mu_hat (transformed drifts and correlation, needed for constraint of American options)
        sg::base::DataVector* mu_hat;
        /// use coarsening between timesteps in order to reduce gridsize
        bool useCoarsen;
        /// adaptive mode during solving Black Scholes Equation: coarsen, refine, coarsenNrefine
        std::string adaptSolveMode;
        /// number of points the are coarsened in each coarsening-step !CURRENTLY UNUSED PARAMETER!
        int numCoarsenPoints;
        /// Threshold used to decide if a grid point should be deleted
        double coarsenThreshold;
        /// Threshold used to decide if a grid point should be refined
        double refineThreshold;
        /// refine mode during solving Black Scholes Equation: classic or maxLevel
        std::string refineMode;
        /// maxLevel max. Level of refinement
        sg::base::GridIndex::level_type refineMaxLevel;
        /// the algorithmic dimensions used in this system
        std::vector<size_t> BSalgoDims;
        /// store number of executed timesteps
        size_t nExecTimesteps;
        /// the strike of the current option
        double dStrike;
        /// the type of the current option
        std::string option_type;
        /// teh riskfree rate
        double r;

        virtual void applyLOperatorInner(sg::base::DataVector& alpha, sg::base::DataVector& result);

        virtual void applyLOperatorComplete(sg::base::DataVector& alpha, sg::base::DataVector& result);

        virtual void applyMassMatrixInner(sg::base::DataVector& alpha, sg::base::DataVector& result);

        virtual void applyMassMatrixComplete(sg::base::DataVector& alpha, sg::base::DataVector& result);

        virtual void applyMassMatrixLOperatorInner(sg::base::DataVector& alpha, sg::base::DataVector& result);

        virtual void applyMassMatrixLOperatorBound(sg::base::DataVector& alpha, sg::base::DataVector& result);

        virtual void setTimestepCoefficientInner(double timestep_coefficient);

        virtual void setTimestepCoefficientBound(double timestep_coefficient);

      public:
        /**
         * Std-Constructor
         *
         * @param SparseGrid reference to the sparse grid
         * @param alpha the ansatzfunctions' coefficients
         * @param lambda reference to the lambdas
         * @param eigenvecs reference to the eigenvectors of the co-variance matrix
         * @param mu_hat reference to transformed drifts and correlation, needed for constraint of American options
         * @param TimestepSize the size of one timestep used in the ODE Solver
         * @param OperationMode specifies in which solver this matrix is used, valid values are: ExEul for explicit Euler,
         *                ImEul for implicit Euler, CrNic for Crank Nicolson solver
         * @param dStrike the strike of the option
         * @param option_type the option to be solved std_amer_put as an special case
         * @param r the riskfree rate
         * @param useCoarsen specifies if the grid should be coarsened between timesteps
         * @param coarsenThreshold Threshold to decide, if a grid point should be deleted
         * @param adaptSolveMode adaptive mode during solving: coarsen, refine, coarsenNrefine
         * @param numCoarsenPoints number of point that should be coarsened in one coarsening step !CURRENTLY UNUSED PARAMETER!
         * @param refineThreshold Threshold to decide, if a grid point should be refined
         * @param refineMode refineMode during solving Black Scholes Equation: classic or maxLevel
         * @param refineMaxLevel max. level of refinement during solving
         */
        BlackScholesPATParabolicPDESolverSystemEuroAmerVectorizedMPI(sg::base::Grid& SparseGrid, sg::base::DataVector& alpha, sg::base::DataVector& lambda,
            sg::base::DataMatrix& eigenvecs, sg::base::DataVector& mu_hat, double TimestepSize, std::string OperationMode,
            double dStrike, std::string option_type, double r,
            bool useCoarsen = false, double coarsenThreshold = 0.0, std::string adaptSolveMode = "none",
            int numCoarsenPoints = -1, double refineThreshold = 0.0, std::string refineMode = "classic", sg::base::GridIndex::level_type refineMaxLevel = 0);

        /**
         * Std-Destructor
         */
        virtual ~BlackScholesPATParabolicPDESolverSystemEuroAmerVectorizedMPI();

        virtual void finishTimestep();

        /**
         * @param isLastTimestep specify if last time step
         */
        virtual void coarsenAndRefine(bool isLastTimestep = false);

        void startTimestep();
    };

  }
}

#endif /* BLACKSCHOLESPATPARABOLICPDESOLVERSYSTEMEUROAMERVECTORIZEDMPI_HPP */