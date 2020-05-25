//
// Created by kylew on 5/25/20.
//

#ifndef NSE_TYPES_H
#define NSE_TYPES_H
#include <deal.II/base/types.h>

enum class TestCase {round_cylinder, channel_flow, manufactured};

namespace BoundaryType {
    enum : dealii::types::boundary_id {inflow, outflow, wall, bluff};
}

#endif //NSE_TYPES_H
