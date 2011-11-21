/******************************************************************************
 * Copyright (C) 2009 Technische Universitaet Muenchen                         *
 * This file is part of the SG++ project. For conditions of distribution and   *
 * use, please see the copyright notice at http://www5.in.tum.de/SGpp          *
 ******************************************************************************/
// @author Jörg Blank (blankj@in.tum.de), Alexander Heinecke (Alexander.Heinecke@mytum.de), Dirk Pflueger (pflueged@in.tum.de)

#ifndef GRID_HPP
#define GRID_HPP


#include "base/operation/OperationEval.hpp"

#include "base/grid/generation/GridGenerator.hpp"

#include <map>

namespace sg
{
namespace base
{

/**
 * abstract base class for all types grids used in sgpp
 * the class gives pure virtual function definitions that
 * have to be implemented by all types of grids
 */
class Grid
{
public:
	/**
	 * creates a linear grid without boundaries
	 *
	 * @param dim the grid's dimension
	 */
	static Grid* createLinearGrid(size_t dim);

	/**
	 * creates a linear stretched grid without boundaries
	 *
	 * @param dim the grid's dimension
	 */
	static Grid* createLinearStretchedGrid(size_t dim);

	/**
	 * creates a linear boundary grid
	 *
	 * @param dim the grid's dimension
	 */
	static Grid* createLinearBoundaryGrid(size_t dim);

	/**
	 * creates a linear trapezoid boundary grid
	 *
	 * @param dim the grid's dimension
	 */
	static Grid* createLinearTrapezoidBoundaryGrid(size_t dim);

	/**
	 * creates a linearstretched trapezoid boundary grid
	 *
	 * @param dim the grid's dimension
	 */
	static Grid* createLinearStretchedTrapezoidBoundaryGrid(size_t dim);
	/**
	 * creates a mod linear grid
	 *
	 * @param dim the grid's dimension
	 */
	static Grid* createModLinearGrid(size_t dim);

	/**
	 * creates a mod polynomial grid
	 *
	 * @param dim the grid's dimension
	 * @param degree the polynom's max. degree
	 */
	static Grid* createPolyGrid(size_t dim, size_t degree);

	/**
	 * creates a poly grid
	 *
	 * @param dim the grid's dimension
	 * @param degree the polynom's max. degree
	 */
	static Grid* createModPolyGrid(size_t dim, size_t degree);

	/**
	 * creates a mod wavelet grid
	 *
	 * @param dim the grid's dimension
	 */
	static Grid* createModWaveletGrid(size_t dim);

	/**
	 * creates a mod-Bspline grid
	 *
	 * @param dim the grid's dimension
	 */
	static Grid* createModBsplineGrid(size_t dim, size_t degree);

	/**
	 * creates a prewavelet grid
	 *
	 * @param dim the grid's dimension
	 */
	static Grid* createPrewaveletGrid(size_t dim);

	/**
	 * reads a grid out of a string
	 *
	 * @param istr string that contains the grid information
	 */
	/**
	 * creates a square root grid(h-grid)
	 * @param dim the grid's dimension
	 * */
	static Grid* createSquareRootGrid(size_t dim);

	/**
	 * creates a trapezoid boundary grid=contains all the gridpoints of the fullgrids which have \f$|l|<level and li>=l_user\f$
	 *
	 * @param dim the grid's dimension
	 * */
	static Grid* createTruncatedTrapezoidGrid(size_t dim);

	/**
	 * reads a grid out of a string
	 *
	 * @param istr string that contains the grid information
	 */
	static Grid* unserialize(const std::string& istr);

	/**
	 * reads a grid out of a stream
	 * @todo check for empty istream - error message is not very meaningful
	 * @param istr inputstream that contains the grid information
	 */
	static Grid* unserialize(std::istream& istr);

protected:
	/**
	 * This constructor creates a new GridStorage out of the stream.
	 * For derived classes create an own constructor wich takes a std::istream and calls
	 * this function. Add your own static unserialize function and add it in typeMap().
	 *
	 * @param istr inputstream that contains the grid information
	 */
	Grid(std::istream& istr);

	/**
	 * Standard Constructor
	 */
	Grid();

public:
	/**
	 * Desctructor
	 */
	virtual ~Grid();

	/**
	 * gets a pointer to the GridStorage object
	 *
	 * @return pointer to the GridStorage obeject
	 */
	virtual GridStorage* getStorage();

	/**
	 * gets a pointer to the GridStorage's BoundingsBox object
	 *
	 * @return pointer to the GridStorage's BoundingsBox object
	 */
	virtual BoundingBox* getBoundingBox();

	/**
	 * gets a pointer to the GridStorage's Stretching object
	 *
	 * @return pointer to the GridStorage's Stretching object
	 */
	virtual Stretching* getStretching();

	/**
	 * sets the GridStorage's BoundingsBox pointer to a BoundingBox object
	 *
	 * @return pointer to the GridStorage's BoundingsBox object
	 */
	virtual void setBoundingBox(BoundingBox& bb);

	/**
	 * sets the GridStorage's Stretching pointer to a Stretching object
	 *
	 * @return pointer to the GridStorage's Stretching object
	 */
	virtual void setStretching(Stretching& bb);

	/**
	 * gets a pointer to GridGenerator object
	 *
	 * @return pointer to the GrdGenerator object
	 */
	virtual GridGenerator* createGridGenerator() = 0;

	/**
	 * gets a pointer to OperationMultipleEval object
	 *
	 * @param dataset the dataset that should be evaluated on the sparse grid
	 *
	 * @return pointer to the OperationB object
	 */
	//virtual OperationMultipleEval* createOperationMultipleEval(DataMatrix* dataset) = 0;

	/**
	 * gets a pointer to OperationBVectorized object
	 *
	 * @param VecType Type of Vectorization used: Currently supported: SSE, AVX
	 * @param dataset the dataset that should be evaluated on the sparse grid
	 *
	 * @return pointer to the OperationB object
	 */
//	virtual OperationMultipleEvalVectorized* createOperationMultipleEvalVectorized(
//			const std::string& VecType, DataMatrix* dataset) = 0;

	/**
	 * gets a pointer to OperationBVectorizedSP object
	 *
	 * @param VecType Type of Vectorization used: Currently supported: SSE, AVX
	 * @param dataset the dataset that should be evaluated on the sparse grid
	 *
	 * @return pointer to the OperationBSP object
	 */
//	virtual OperationMultipleEvalVectorizedSP* createOperationMultipleEvalVectorizedSP(
//			const std::string& VecType, DataMatrixSP* dataset) = 0;

	/**
	 * gets a pointer to OperationConvert object
	 *
	 * @return pointer to the OperationConvert object
	 */
	//virtual OperationConvert* createOperationConvert() = 0;

	/**
	 * gets a pointer to OperationEval object
	 *
	 * @return pointer to the OperationEval object
	 */
	//virtual OperationEval* createOperationEval() = 0;

	/**
	 * gets a pointer to OperationTest object
	 *
	 * @return pointer to the OperationTest object
	 */
	//virtual OperationTest* createOperationTest() = 0;

	/**
	 * gets a pointer to OperationHierarchisation object
	 *
	 * @return pointer to the OperationHierarchisation object
	 */
	//virtual OperationHierarchisation* createOperationHierarchisation() = 0;

	/**
	 * gets a pointer to OperationLaplace (OperationMatrix) object
	 *
	 * @return point to the OperationLaplace object
	 */
	//virtual OperationMatrix* createOperationLaplace() = 0;

	/**
	 * gets a pointer to OperationLTwoDotProduct (OperationMatrix) object
	 *
	 * @return pointer to OperationLTwoDotProduct object
	 */
	//virtual OperationMatrix* createOperationLTwoDotProduct() = 0;
	/*virtual OperationMatrix* createOperationLB() = 0;
	virtual OperationMatrix* createOperationLD() = 0;
	virtual OperationMatrix* createOperationLE() = 0;
	virtual OperationMatrix* createOperationLF() = 0;*/

	/**
	 * this operation allows you to calculate the following bilinear form
	 * needed to solve the multidimensional Black Scholes Equation
	 *
	 * \f$ \int_{\Omega} S_i v(\vec{s}) \frac{\partial u(\vec{s}}{\partial S_i} d \vec{s}\f$
	 *
	 * @param coef reference to a DataVector object that contains the constant coeffecients of this bilinear from
	 */
	//virtual OperationMatrix* createOperationDelta(DataVector& coef) = 0;

	/**
	 * this operation allows you to calculate the following bilinear form
	 * needed to solve the multidimensional Black Scholes Equation
	 *
	 * \f$ \int_{\Omega} S_i S_j \frac{\partial u(\vec{s}}{\partial S_i} \frac{\partial v(\vec{s}}{\partial S_j} d \vec{s}\f$
	 *
	 * @param coef reference to a DataMatrix object that contains the constant coeffecients of this bilinear from
	 */
	//virtual OperationMatrix* createOperationGamma(DataMatrix& coef) = 0;

	/**
	 * this operation allows you to calculate the following bilinear form
	 * needed to solve the multidimensional log-transformed Black Scholes Equation
	 *
	 * \f$ \int_{\Omega} \frac{\partial u(\vec{s}}{\partial S_i} v(\vec{s}) d \vec{s}\f$
	 *
	 * @param coef reference to a DataVector object that contains the constant coeffecients of this bilinear from
	 */
	//virtual OperationMatrix* createOperationDeltaLog(DataVector& coef) = 0;

	/**
	 * this operation allows you to calculate the following bilinear form
	 * needed to solve the multidimensional log-transformed Black Scholes Equation
	 *
	 * \f$ \int_{\Omega} \frac{\partial u(\vec{s}}{\partial S_i} \frac{\partial v(\vec{s}}{\partial S_j} d \vec{s}\f$
	 *
	 * @param coef reference to a DataVector object that contains the constant coeffecients of this bilinear from
	 */
	//virtual OperationMatrix* createOperationGammaLog(DataMatrix& coef) = 0;

	/**
	 * gets a pointer to OperationIdentity (OperationMatrix) object
	 *
	 * @return point to the OperationIdentity object
	 */
	//OperationMatrix* createOperationIdentity();

	/**
	 * Returns a string that identifies the grid type uniquely
	 *
	 * @return string that identifies the grid type uniquely
	 */
	virtual const char* getType() = 0;

	/**
	 * Serializes grid to a string.
	 * Needed for Python compatibility. Calls serialize(std::ostream&).
	 *
	 * @param ostr string into which the grid is written
	 */
	void serialize(std::string& ostr);

	/**
	 * Serializes the grid.
	 * Override if additional information need to be saved.
	 * Call base function before writing anything!
	 *
	 * @param ostr stream to which the grid is written
	 */
	virtual void serialize(std::ostream& ostr);

	/**
	 * Serializes grid to a string.
	 * Needed for Java compatibility.
	 *
	 * @returns string into which the grid is written
	 */
	std::string serialize();

	/**
	 * Refine grid
	 * Refine the given number of points on the grid according to the vector
	 *
	 * @param vector DataVector vector with errors for each basis function or alpha-vector
	 * @param numOfPoints integer number of points to refine
	 */
	void refine(DataVector* vector, int numOfPoints);

	/**
	 * Evaluate the value of function in the point
	 *
	 * @param alpha DataVector alpha vector of the grid
	 * @param point DataVector point where the function should be evaluated
	 */
	double eval(DataVector& alpha, DataVector& point);

	/**
	 * Insert one point to the grid
	 *
	 * @param dim dimension of the grid
	 * @param levels array with levels of the point
	 * @param indices array with indices of the point
	 * @param isLeaf indicator whether the point is a leaf
	 */
	void insertPoint(size_t dim, unsigned int levels[], unsigned int indices[],
			bool isLeaf);

	/**
	 * Returns the number of points on the grid
	 * @return the number of points on the grid
	 */
	int getSize();

	/**
	 * returns the algorithmic dimensions (the dimensions in which the Up Down
	 * operations should be applied)
	 *
	 * @return the algorithmic dimensions
	 */
	std::vector<size_t> getAlgorithmicDimensions();

	/**
	 * sets the algorithmic dimensions (the dimensions in which the Up Down
	 * operations should be applied)
	 *
	 * @param newAlgoDims std::vector containing the algorithmic dimensions
	 */
	void setAlgorithmicDimensions(std::vector<size_t> newAlgoDims);

protected:
	/// pointer the GridStorage object of the grid
	GridStorage* storage;

	typedef Grid* (*Factory)(std::istream&);
	typedef std::map<std::string, Grid::Factory> factoryMap;

	static Grid* nullFactory(std::istream&);

private:
	/**
	 * This method returns a map with all available grid types for serialization
	 *
	 * @return a map with all available grid types for serialization
	 */
	static factoryMap& typeMap();

	//pointer to the Operation Eval used in Grid.eval()
	static OperationEval* evalOp;
};

}
}

#endif /* GRID_HPP */