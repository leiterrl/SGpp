/******************************************************************************
* Copyright (C) 2011 Technische Universitaet Muenchen                         *
* This file is part of the SG++ project. For conditions of distribution and   *
* use, please see the copyright notice at http://www5.in.tum.de/SGpp          *
******************************************************************************/
// @author Alexander Heinecke (Alexander.Heinecke@mytum.de)

#include "tools/MPI/SGppMPITools.hpp"

#include "algorithm/pde/BlackScholesParabolicPDESolverSystemEuropeanParallelMPI.hpp"
#include "exception/algorithm_exception.hpp"
#include "grid/generation/SurplusCoarseningFunctor.hpp"
#include "grid/generation/SurplusRefinementFunctor.hpp"
#include "algorithm/pde/StdUpDown.hpp"
#include "algorithm/pde/UpDownOneOpDim.hpp"
#include "algorithm/pde/UpDownTwoOpDims.hpp"

#ifdef _OPENMP
#include "omp.h"
#endif

namespace sg
{
namespace parallel
{

BlackScholesParabolicPDESolverSystemEuropeanParallelMPI::BlackScholesParabolicPDESolverSystemEuropeanParallelMPI(Grid& SparseGrid, DataVector& alpha, DataVector& mu,
			DataVector& sigma, DataMatrix& rho, double r, double TimestepSize, std::string OperationMode,
			bool bLogTransform, bool useCoarsen, double coarsenThreshold, std::string adaptSolveMode,
			int numCoarsenPoints, double refineThreshold, std::string refineMode, size_t refineMaxLevel) : BlackScholesParabolicPDESolverSystemEuropean(SparseGrid, alpha, mu, sigma, rho,
			r, TimestepSize, OperationMode, bLogTransform, useCoarsen, coarsenThreshold, adaptSolveMode, numCoarsenPoints, refineThreshold, refineMode, refineMaxLevel)
{}

BlackScholesParabolicPDESolverSystemEuropeanParallelMPI::~BlackScholesParabolicPDESolverSystemEuropeanParallelMPI()
{
}

void BlackScholesParabolicPDESolverSystemEuropeanParallelMPI::applyLOperatorInner(DataVector& alpha, DataVector& result)
{
	result.setAll(0.0);

	std::vector<size_t> algoDims = this->InnerGrid->getStorage()->getAlgorithmicDimensions();
	size_t nDims = algoDims.size();
#ifdef _OPENMP
	omp_lock_t DeltaMutex;
	omp_lock_t GammaMutex;
	omp_init_lock(&DeltaMutex);
	omp_init_lock(&GammaMutex);
#endif
	DataVector DeltaResult(result);
	DataVector GammaResult(result);

	// Apply the riskfree rate
	if (0 % myGlobalMPIComm->getNumRanks() == myGlobalMPIComm->getMyRank())
	{
		#pragma omp task shared(alpha, result)
		{
			if (this->r != 0.0)
			{
				DataVector myResult(result.getSize());

				/// @todo (heinecke) discuss methods in order to avoid this cast
				((StdUpDown*)(this->OpLTwoInner))->multParallelBuildingBlock(alpha, myResult);

				// no semaphore needed
				result.axpy((-1.0)*this->r, myResult);
			}
		}
	}

	// Apply the delta method
	for (size_t i = 0; i < nDims; i++)
	{
		if ((i+1) % myGlobalMPIComm->getNumRanks() == myGlobalMPIComm->getMyRank())
		{
			#pragma omp task firstprivate(i) shared(alpha, DeltaMutex, DeltaResult, result, algoDims)
			{
				DataVector myResult(result.getSize());

				/// @todo (heinecke) discuss methods in order to avoid this cast
				((UpDownOneOpDim*)(this->OpDeltaInner))->multParallelBuildingBlock(alpha, myResult, algoDims[i]);

				// semaphore
#ifdef _OPENMP
				omp_set_lock(&DeltaMutex);
#endif
				DeltaResult.add(myResult);
#ifdef _OPENMP
				omp_unset_lock(&DeltaMutex);
#endif
			}
		}
	}

	// Apply the gamma method
	for (size_t i = 0; i < nDims; i++)
	{
		for (size_t j = 0; j < nDims; j++)
		{
			size_t job = (((i*(i+1))/2)+j)+(nDims);

			if (job % myGlobalMPIComm->getNumRanks() == myGlobalMPIComm->getMyRank())
			{
				// symmetric
				if (j <= i)
				{
					#pragma omp task firstprivate(i, j) shared(alpha, GammaMutex, GammaResult, result, algoDims)
					{
						DataVector myResult(result.getSize());

						/// @todo (heinecke) discuss methods in order to avoid this cast
						((UpDownTwoOpDims*)(this->OpGammaInner))->multParallelBuildingBlock(alpha, myResult, algoDims[i], algoDims[j]);

						// semaphore
#ifdef _OPENMP
						omp_set_lock(&GammaMutex);
#endif
						GammaResult.add(myResult);
#ifdef _OPENMP
						omp_unset_lock(&GammaMutex);
#endif
					}
				}
			}
		}
	}

	#pragma omp taskwait

#ifdef _OPENMP
	omp_destroy_lock(&GammaMutex);
	omp_destroy_lock(&DeltaMutex);
#endif

	// sum up
	result.add(DeltaResult);
	result.sub(GammaResult);
}

void BlackScholesParabolicPDESolverSystemEuropeanParallelMPI::applyLOperatorComplete(DataVector& alpha, DataVector& result)
{
	result.setAll(0.0);

	std::vector<size_t> algoDims = this->InnerGrid->getStorage()->getAlgorithmicDimensions();
	size_t nDims = algoDims.size();

#ifdef _OPENMP
	omp_lock_t DeltaMutex;
	omp_lock_t GammaMutex;
	omp_init_lock(&DeltaMutex);
	omp_init_lock(&GammaMutex);
#endif
	DataVector DeltaResult(result);
	DataVector GammaResult(result);

	// Apply the riskfree rate
	if (0 % myGlobalMPIComm->getNumRanks() == myGlobalMPIComm->getMyRank())
	{
		#pragma omp task shared(alpha, result)
		{
			if (this->r != 0.0)
			{
				DataVector myResult(result.getSize());

				/// @todo (heinecke) discuss methods in order to avoid this cast
				((StdUpDown*)(this->OpLTwoBound))->multParallelBuildingBlock(alpha, myResult);

				// no semaphore needed
				result.axpy((-1.0)*this->r, myResult);
			}
		}
	}

	// Apply the delta method
	for (size_t i = 0; i < nDims; i++)
	{
		if ((i+1) % myGlobalMPIComm->getNumRanks() == myGlobalMPIComm->getMyRank())
		{
			#pragma omp task firstprivate(i) shared(alpha, DeltaMutex, DeltaResult, result, algoDims)
			{
				DataVector myResult(result.getSize());

				/// @todo (heinecke) discuss methods in order to avoid this cast
				((UpDownOneOpDim*)(this->OpDeltaBound))->multParallelBuildingBlock(alpha, myResult, algoDims[i]);

				// semaphore
#ifdef _OPENMP
				omp_set_lock(&DeltaMutex);
#endif
				DeltaResult.add(myResult);
#ifdef _OPENMP
				omp_unset_lock(&DeltaMutex);
#endif
			}
		}
	}

	// Apply the gamma method
	for (size_t i = 0; i < nDims; i++)
	{
		for (size_t j = 0; j < nDims; j++)
		{
			size_t job = (((i*(i+1))/2)+j)+(nDims);

			if (job % myGlobalMPIComm->getNumRanks() == myGlobalMPIComm->getMyRank())
			{
				// symmetric
				if (j <= i)
				{
					#pragma omp task firstprivate(i, j) shared(alpha, GammaMutex, GammaResult, result, algoDims)
					{
						DataVector myResult(result.getSize());

						/// @todo (heinecke) discuss methods in order to avoid this cast
						((UpDownTwoOpDims*)(this->OpGammaBound))->multParallelBuildingBlock(alpha, myResult, algoDims[i], algoDims[j]);

						// semaphore
#ifdef _OPENMP
						omp_set_lock(&GammaMutex);
#endif
						GammaResult.add(myResult);
#ifdef _OPENMP
						omp_unset_lock(&GammaMutex);
#endif
					}
				}
			}
		}
	}

	#pragma omp taskwait

#ifdef _OPENMP
	omp_destroy_lock(&GammaMutex);
	omp_destroy_lock(&DeltaMutex);
#endif

	// sum up
	result.add(DeltaResult);
	result.sub(GammaResult);
}

void BlackScholesParabolicPDESolverSystemEuropeanParallelMPI::applyMassMatrixInner(DataVector& alpha, DataVector& result)
{
	DataVector temp(alpha.getSize());
	result.setAll(0.0);
	size_t nDims = this->InnerGrid->getStorage()->getAlgorithmicDimensions().size();
	size_t jobs = (((nDims*nDims)+3*nDims)/2)+2;

	if ((jobs-1) % myGlobalMPIComm->getNumRanks() == myGlobalMPIComm->getMyRank())
	{
		((StdUpDown*)(this->OpLTwoInner))->multParallelBuildingBlock(alpha, temp);

		result.add(temp);
	}
}

void BlackScholesParabolicPDESolverSystemEuropeanParallelMPI::applyMassMatrixComplete(DataVector& alpha, DataVector& result)
{
	DataVector temp(alpha.getSize());
	result.setAll(0.0);
	size_t nDims = this->InnerGrid->getStorage()->getAlgorithmicDimensions().size();
	size_t jobs = (((nDims*nDims)+3*nDims)/2)+2;

	if ((jobs-1) % myGlobalMPIComm->getNumRanks() == myGlobalMPIComm->getMyRank())
	{
		((StdUpDown*)(this->OpLTwoBound))->multParallelBuildingBlock(alpha, temp);

		result.add(temp);
	}
}

void BlackScholesParabolicPDESolverSystemEuropeanParallelMPI::mult(DataVector& alpha, DataVector& result)
{
	// distribute the current grid coefficients
	myGlobalMPIComm->broadcastGridCoefficientsFromRank0(alpha);

	result.setAll(0.0);

	if (this->tOperationMode == "ExEul")
	{
		applyMassMatrixInner(alpha, result);
	}
	else if (this->tOperationMode == "ImEul")
	{
		DataVector temp(result.getSize());
		DataVector temp2(result.getSize());

		#pragma omp parallel shared(alpha, result)
		{
			#pragma omp single nowait
			{
				#pragma omp task shared (alpha, temp)
				{
					applyMassMatrixInner(alpha, temp);
				}

				#pragma omp task shared (alpha, temp2)
				{
					applyLOperatorInner(alpha, temp2);
				}

				#pragma omp taskwait
			}
		}

		result.add(temp);
		result.axpy((-1.0)*this->TimestepSize, temp2);
	}
	else if (this->tOperationMode == "CrNic")
	{
		DataVector temp(result.getSize());
		DataVector temp2(result.getSize());

		#pragma omp parallel shared(alpha, result)
		{
			#pragma omp single nowait
			{
				#pragma omp task shared (alpha, temp)
				{
					applyMassMatrixInner(alpha, temp);
				}

				#pragma omp task shared (alpha, temp2)
				{
					applyLOperatorInner(alpha, temp2);
				}

				#pragma omp taskwait
			}
		}

		result.add(temp);
		result.axpy((-0.5)*this->TimestepSize, temp2);
	}
	else if (this->tOperationMode == "AdBas")
	{
		result.setAll(0.0);

		applyMassMatrixInner(alpha, result);
	}
	else
	{
		throw new algorithm_exception(" BlackScholesParabolicPDESolverSystemEuropeanParallelOMP::mult : An unknown operation mode was specified!");
	}

	// aggregate all results
	myGlobalMPIComm->reduceGridCoefficientsOnRank0(result);
}

DataVector* BlackScholesParabolicPDESolverSystemEuropeanParallelMPI::generateRHS()
{
	// distribute the current grid coefficients
	myGlobalMPIComm->broadcastGridCoefficientsFromRank0(*(this->alpha_complete));

	DataVector rhs_complete(this->alpha_complete->getSize());

	if (this->tOperationMode == "ExEul")
	{
		rhs_complete.setAll(0.0);

		DataVector temp(rhs_complete.getSize());
		DataVector temp2(rhs_complete.getSize());
		DataVector myAlpha(*this->alpha_complete);

		#pragma omp parallel shared(myAlpha, temp, temp2)
		{
			#pragma omp single nowait
			{
				#pragma omp task shared (myAlpha, temp)
				{
					applyMassMatrixComplete(myAlpha, temp);
				}

				#pragma omp task shared (myAlpha, temp2)
				{
					applyLOperatorComplete(myAlpha, temp2);
				}

				#pragma omp taskwait
			}
		}

		rhs_complete.add(temp);
		rhs_complete.axpy(this->TimestepSize, temp2);
	}
	else if (this->tOperationMode == "ImEul")
	{
		rhs_complete.setAll(0.0);

		applyMassMatrixComplete(*this->alpha_complete, rhs_complete);
	}
	else if (this->tOperationMode == "CrNic")
	{
		rhs_complete.setAll(0.0);

		DataVector temp(rhs_complete.getSize());
		DataVector temp2(rhs_complete.getSize());
		DataVector myAlpha(*this->alpha_complete);

		#pragma omp parallel shared(myAlpha, temp, temp2)
		{
			#pragma omp single nowait
			{
				#pragma omp task shared (myAlpha, temp)
				{
					applyMassMatrixComplete(myAlpha, temp);
				}

				#pragma omp task shared (myAlpha, temp2)
				{
					applyLOperatorComplete(myAlpha, temp2);
				}

				#pragma omp taskwait
			}
		}

		rhs_complete.add(temp);
		rhs_complete.axpy((0.5)*this->TimestepSize, temp2);
	}
	else if (this->tOperationMode == "AdBas")
	{
		rhs_complete.setAll(0.0);

		DataVector temp(this->alpha_complete->getSize());
		DataVector myAlpha(*this->alpha_complete);
		DataVector myOldAlpha(*this->alpha_complete_old);

		applyMassMatrixComplete(*this->alpha_complete, temp);

		#pragma omp parallel shared(myAlpha, temp)
		{
			#pragma omp single nowait
			{
				#pragma omp task shared (myAlpha, temp)
				{
					applyLOperatorComplete(myAlpha, temp);
				}

				#pragma omp taskwait
			}
		}

		rhs_complete.add(temp);
		temp.mult((2.0)+this->TimestepSize/this->TimestepSize_old);

		DataVector temp_old(this->alpha_complete->getSize());

		applyMassMatrixComplete(*this->alpha_complete_old, temp_old);

		#pragma omp parallel shared(myOldAlpha, temp_old)
		{
			#pragma omp single nowait
			{
				#pragma omp task shared (myOldAlpha, temp_old)
				{
					applyLOperatorComplete(myOldAlpha, temp_old);
				}

				#pragma omp taskwait
			}
		}

		temp_old.mult(this->TimestepSize/this->TimestepSize_old);
		temp.sub(temp_old);
		rhs_complete.axpy((0.5)*this->TimestepSize, temp);
	}
	else
	{
		throw new algorithm_exception("BlackScholesParabolicPDESolverSystemEuropeanParallelOMP::generateRHS : An unknown operation mode was specified!");
	}

	// aggregate all results
	myGlobalMPIComm->reduceGridCoefficientsOnRank0(rhs_complete);

	// Now we have the right hand side, lets apply the riskfree rate for the next timestep
	this->startTimestep();

	// Now apply the boundary ansatzfunctions to the inner ansatzfunctions
	DataVector result_complete(this->alpha_complete->getSize());
	DataVector alpha_bound(*this->alpha_complete);

	result_complete.setAll(0.0);

	this->BoundaryUpdate->setInnerPointsToZero(alpha_bound);

	// apply CG Matrix
	if (this->tOperationMode == "ExEul")
	{
		applyMassMatrixComplete(alpha_bound, result_complete);
	}
	else if (this->tOperationMode == "ImEul")
	{
		DataVector temp(alpha_bound.getSize());
		DataVector temp2(alpha_bound.getSize());

		#pragma omp parallel shared(alpha_bound, temp, temp2)
		{
			#pragma omp single nowait
			{
				#pragma omp task shared (alpha_bound, temp)
				{
					applyMassMatrixComplete(alpha_bound, temp);
				}

				#pragma omp task shared (alpha_bound, temp2)
				{
					applyLOperatorComplete(alpha_bound, temp2);
				}

				#pragma omp taskwait
			}
		}

		result_complete.add(temp);
		result_complete.axpy((-1.0)*this->TimestepSize, temp2);
	}
	else if (this->tOperationMode == "CrNic")
	{
		DataVector temp(alpha_bound.getSize());
		DataVector temp2(alpha_bound.getSize());

		#pragma omp parallel shared(alpha_bound, temp, temp2)
		{
			#pragma omp single nowait
			{
				#pragma omp task shared (alpha_bound, temp)
				{
					applyMassMatrixComplete(alpha_bound, temp);
				}

				#pragma omp task shared (alpha_bound, temp2)
				{
					applyLOperatorComplete(alpha_bound, temp2);
				}

				#pragma omp taskwait
			}
		}

		result_complete.add(temp);
		result_complete.axpy((-0.5)*this->TimestepSize, temp2);
	}
	else if (this->tOperationMode == "AdBas")
	{
		applyMassMatrixComplete(alpha_bound, result_complete);
	}
	else
	{
		throw new algorithm_exception("BlackScholesParabolicPDESolverSystemEuropeanParallelOMP::generateRHS : An unknown operation mode was specified!");
	}

	// aggregate all results
	myGlobalMPIComm->reduceGridCoefficientsOnRank0(result_complete);

	rhs_complete.sub(result_complete);

	if (this->rhs != NULL)
	{
		delete this->rhs;
	}

	this->rhs = new DataVector(this->alpha_inner->getSize());

	if (myGlobalMPIComm->getMyRank() == 0)
	{
		this->GridConverter->calcInnerCoefs(rhs_complete, *this->rhs);
	}
	else
	{
		this->rhs->setAll(0.0);
	}

	return this->rhs;
}

void BlackScholesParabolicPDESolverSystemEuropeanParallelMPI::finishTimestep(bool isLastTimestep)
{
	// Adaptivity stuff is done on rank 0 only
	if (myGlobalMPIComm->getMyRank() == 0)
	{
		// Replace the inner coefficients on the boundary grid
		this->GridConverter->updateBoundaryCoefs(*this->alpha_complete, *this->alpha_inner);

	#ifndef NOBOUNDARYDISCOUNT
		// Adjust the boundaries with the riskfree rate
		if (this->r != 0.0)
		{
			if (this->tOperationMode == "ExEul" || this->tOperationMode == "AdBas")
			{
				this->BoundaryUpdate->multiplyBoundary(*this->alpha_complete, exp(((-1.0)*(this->r*this->TimestepSize))));
			}
		}
	#endif

		// add number of Gridpoints
		this->numSumGridpointsInner += this->InnerGrid->getSize();
		this->numSumGridpointsComplete += this->BoundGrid->getSize();

		if (this->useCoarsen == true && isLastTimestep == false)
		{
			///////////////////////////////////////////////////
			// Start integrated refinement & coarsening
			///////////////////////////////////////////////////

			size_t originalGridSize = this->BoundGrid->getStorage()->size();

			// Coarsen the grid
			base::GridGenerator* myGenerator = this->BoundGrid->createGridGenerator();

			//std::cout << "Coarsen Threshold: " << this->coarsenThreshold << std::endl;
			//std::cout << "Grid Size: " << originalGridSize << std::endl;

			if (this->adaptSolveMode == "refine" || this->adaptSolveMode == "coarsenNrefine")
			{
				size_t numRefines = myGenerator->getNumberOfRefinablePoints();
				base::SurplusRefinementFunctor* myRefineFunc = new base::SurplusRefinementFunctor(this->alpha_complete, numRefines, this->refineThreshold);
				if (this->refineMode == "maxLevel")
				{
					myGenerator->refineMaxLevel(myRefineFunc, this->refineMaxLevel);
					this->alpha_complete->resizeZero(this->BoundGrid->getStorage()->size());
				}
				if (this->refineMode == "classic")
				{
					myGenerator->refine(myRefineFunc);
					this->alpha_complete->resizeZero(this->BoundGrid->getStorage()->size());
				}
				delete myRefineFunc;
			}

			if (this->adaptSolveMode == "coarsen" || this->adaptSolveMode == "coarsenNrefine")
			{
				size_t numCoarsen = myGenerator->getNumberOfRemoveablePoints();
				base::SurplusCoarseningFunctor* myCoarsenFunctor = new base::SurplusCoarseningFunctor(this->alpha_complete, numCoarsen, this->coarsenThreshold);
				myGenerator->coarsenNFirstOnly(myCoarsenFunctor, this->alpha_complete, originalGridSize);
				delete myCoarsenFunctor;
			}

			delete myGenerator;

			///////////////////////////////////////////////////
			// End integrated refinement & coarsening
			///////////////////////////////////////////////////
		}
	}

	// only communicate if adaptivity is switched on
	if (this->useCoarsen == true && isLastTimestep == false)
	{
		// Communicate new grid
		if (myGlobalMPIComm->getMyRank() == 0)
		{
			std::string bound_grid_storage = this->BoundGrid->getStorage()->serialize();

			myGlobalMPIComm->broadcastGridStorage(bound_grid_storage);

			// rebuild the inner grid + coefficients
			this->GridConverter->rebuildInnerGridWithCoefs(*this->BoundGrid, *this->alpha_complete, &this->InnerGrid, &this->alpha_inner);
		}
		else
		{
			std::string bound_grid_storage = "";

			myGlobalMPIComm->receiveGridStorage(bound_grid_storage);

			this->BoundGrid->getStorage()->emptyStorage();
			this->BoundGrid->getStorage()->unserialize_noAlgoDims(bound_grid_storage);
			this->alpha_complete->resize(this->BoundGrid->getStorage()->size());

			// rebuild the inner grid + coefficients
			this->GridConverter->rebuildInnerGridWithCoefs(*this->BoundGrid, *this->alpha_complete, &this->InnerGrid, &this->alpha_inner);
		}

		myGlobalMPIComm->Barrier();
	}
}

}
}