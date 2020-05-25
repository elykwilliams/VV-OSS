//
// Created by kylew on 5/24/20.
//

#include "catch.hpp"

#include <string>

#include "Base/settings.h"
#include "Base/NSESolver.h"

using std::string;

TEST_CASE("Test Constructor", "[constructor]"){

    extern string filename;
    Settings settings(filename);

    if (settings.get_dim() == 2)
        REQUIRE_NOTHROW(NSESolver<2>(settings));
    else if (settings.get_dim() == 3)
        REQUIRE_NOTHROW(NSESolver<2>(settings));
}