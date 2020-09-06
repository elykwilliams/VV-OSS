//
// Created by kylew on 9/2/20.
//

#ifndef NSE_SYSTEM_H
#define NSE_SYSTEM_H

#include <deal.II/base/index_set.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/dofs/dof_tools.h>
#include <deal.II/dofs/dof_renumbering.h>
#include <deal.II/lac/generic_linear_algebra.h>

#include "fe_handler.h"


using namespace dealii;

namespace Trilinos{
    using namespace LinearAlgebraTrilinos::MPI;
}

using std::vector, std::map, std::unique_ptr;

template<int dim>
class BlockPartitioning{
	// This class manages the partitioning of degrees of freedom amoung several
	// several processors. It also provides the blocking structure needed
	// when creating a distributed::BlockVector.
	// dijoint_blocks will create blocks with disjoint sets of dofs.
	// relevant_blocks will create blocks with localally owned dofs + ghosted DoFs
public:
	BlockPartitioning() = default;

	BlockPartitioning(DoFHandler<dim>& dof_handler, const FeHandlerBase<dim>& fe_handler)
	: dofs_per_block(fe_handler.n_blocks)
	{
		AssertThrow(dof_handler.get_fe_collection().n_components() == fe_handler.size(), ExcInternalError());
		AssertThrow(dofs_per_block.size() > 0, ExcNotInitialized());
		AssertThrow(dof_handler.n_dofs() > 0, ExcNotInitialized());

		DoFRenumbering::component_wise(dof_handler, fe_handler.components_to_blocks);

		DoFTools::count_dofs_per_block(dof_handler, dofs_per_block,	fe_handler.components_to_blocks);

		locally_owned_dofs = dof_handler.locally_owned_dofs();
		disjoint = split_by_block(locally_owned_dofs);

		locally_relevant_dofs.clear();
		DoFTools::extract_locally_relevant_dofs(dof_handler, locally_relevant_dofs);
		ghosted = split_by_block(locally_relevant_dofs);
	}

	IndexSet locally_owned_dofs;
    IndexSet locally_relevant_dofs;
    vector<IndexSet> disjoint;
    vector<IndexSet> ghosted;
private:
	vector<types::global_dof_index>  dofs_per_block;

	vector<IndexSet>split_by_block(const IndexSet& local_dofs)
	{
		// partitioned[i] is the indices of the dofs owned/relevant to this
		// processor and belonging to block i. I.e. it takes the visible dofs
		// and splits them into blocks.
		// Assumes that local_dofs has been renumbered by block/component
		vector<IndexSet> partitioned(dofs_per_block.size());

		types::global_dof_index start = 0;
		for (unsigned int i = 0; i < dofs_per_block.size(); ++i)
		{
			const unsigned int block_size{dofs_per_block[i]};
			partitioned[i] = local_dofs.get_view(start, start + block_size);
			start += block_size;
		}

		AssertThrow(start == local_dofs.size(), ExcInternalError());

		return partitioned;
	}
};


template<int dim>
class Solution {
    // This class will hold the current solution, and previous time step solution,
    // and previous nonlinear solution. It will also be responsible for updating this solution
    // There is generally no need to access non-ghosted versions of the previous solutions
public:
    Solution() = default;

    explicit Solution(BlockPartitioning<dim> partitioning, MPI_Comm mpi_comm):
            current_dist(partitioning.disjoint, mpi_comm),
            current_ghost(partitioning.ghosted, mpi_comm),
            prev_time_ghost(partitioning.ghosted, mpi_comm),
            prevx2_time_ghost(partitioning.ghosted, mpi_comm),
            prev_nonlinear_ghost(partitioning.ghosted, mpi_comm){ }

    void update_time(){
        // Update
        prevx2_time_ghost = prev_time_ghost;
        prev_time_ghost = current_ghost;
    }
    void update_nonlinear(Trilinos::BlockVector& solution){
        prev_nonlinear_ghost = current_ghost;
        current_ghost = solution;
        current_dist = solution;
    }

    Trilinos::BlockVector get_linearization_point(double dt, int degree = 0){
        // This function extrapolates the solution to the next timestep using a polynomial of a given degree
//        switch(degree) {
//            case 0:
//                return current_ghost;
//            case 1:
//                return 2 * current_ghost - prev_time_ghost;
//            default:
//                return current_ghost;
//        }
        return current_ghost;
    }

    Trilinos::BlockVector current_dist;
    Trilinos::BlockVector current_ghost;
    Trilinos::BlockVector prev_time_ghost;
    Trilinos::BlockVector prevx2_time_ghost;
    Trilinos::BlockVector prev_nonlinear_ghost;
};


#endif //NSE_SYSTEM_H
