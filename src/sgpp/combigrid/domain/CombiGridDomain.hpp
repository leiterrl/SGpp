/*
 * CombiGridDomain.hpp
 *
 *  Created on: Apr 4, 2011
 *      Author: benk
 */

#ifndef COMBIGRIDDOMAIN_HPP_
#define COMBIGRIDDOMAIN_HPP_

#include "combigrid/utils/combigrid_ultils.hpp"
#include "combigrid/domain/AbstractStretchingMaker.hpp"
#include "combigrid/domain/CombiDomain1D.hpp"

namespace combigrid {

/** grid domain*/
class GridDomain {

public:

	/** */
	GridDomain(int dim , const std::vector<int>& levels,
			             const std::vector<double>& min ,
			             const std::vector<double>& max ,
			             const AbstractStretchingMaker& stretchingMaker);

	/** */
	GridDomain(int dim , const std::vector< std::vector<double> >& scalings );

	/** */
	GridDomain(int dim , const std::vector<double>& min ,
			             const std::vector<double>& max );

	virtual ~GridDomain(){;}

	/** transform from real coordinate into unit coordinates
	 * @param coords [IN/OUT]
	 * @param levels_in [IN] the required levels
	 * @param boundaryFlag [IN] for each dimensions if there are boundary points*/
	void transformRealToUnit( std::vector< double >& coords ,
			const std::vector<int>& levels_in ,
			const std::vector<bool>& boundaryFlag) const;

	/** return 1D axis, can be used for back transformation for each dimension
	 * @param d [IN] the dimension */
	const Domain1D& get1DDomain(int d) const { return axisDomains_[d]; }

	void printDomain();

private:

	/** dimension of the domain */
	int dim_;

	/** array to store the maping for each axis */
	std::vector< Domain1D > axisDomains_;
};

}

#endif /* COMBIGRIDDOMAIN_HPP_ */