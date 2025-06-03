#define CATCH_CONFIG_RUNNER
#include "catch_amalgamated.hpp"
#include "../src/gameMath.h"


TEST_CASE("Vector Addition", "[gameMath]") {
    Vector2D v1{1.0f,1.0f};
    Vector2D v2{1.0f,1.0f};
    Vector2D vresult{2.0f,2.0f};
    Vector2D test = v1+v2; 
    REQUIRE(test == vresult);

}
