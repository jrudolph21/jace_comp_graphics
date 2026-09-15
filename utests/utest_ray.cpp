#include "ray.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>


TEST_CASE("Testing constructors") {

    ray d_ray;

    ray c_ray(vec3(1,2,3), vec3(4,5,6));

    REQUIRE_THAT(c_ray.getOrigin().x(), Catch::Matchers::WithinAbs(1, 0.0001));
    REQUIRE_THAT(c_ray.getOrigin().y(), Catch::Matchers::WithinAbs(2, 0.0001));
    REQUIRE_THAT(c_ray.getOrigin().z(), Catch::Matchers::WithinAbs(3, 0.0001));

    REQUIRE_THAT(c_ray.getDirection().x(), Catch::Matchers::WithinAbs(4, 0.0001));
    REQUIRE_THAT(c_ray.getDirection().y(), Catch::Matchers::WithinAbs(5, 0.0001));
    REQUIRE_THAT(c_ray.getDirection().z(), Catch::Matchers::WithinAbs(6, 0.0001));

    REQUIRE_THAT(d_ray.getOrigin().x(), Catch::Matchers::WithinAbs(0, 0.0001));
    REQUIRE_THAT(d_ray.getOrigin().x(), Catch::Matchers::WithinAbs(0, 0.0001));
    REQUIRE_THAT(d_ray.getOrigin().x(), Catch::Matchers::WithinAbs(0, 0.0001));

    REQUIRE_THAT(d_ray.getDirection().x(), Catch::Matchers::WithinAbs(0, 0.0001));
    REQUIRE_THAT(d_ray.getDirection().x(), Catch::Matchers::WithinAbs(0, 0.0001));
    REQUIRE_THAT(d_ray.getDirection().x(), Catch::Matchers::WithinAbs(0, 0.0001));

}

TEST_CASE("Testing the at function") {

    vec3 A(1,2,3);
    vec3 b(1,1,1);

    double t = 0.5;

    ray test_ray(A, b);

    vec3 test_vec = test_ray.at(t);

    REQUIRE_THAT(test_vec.x(), Catch::Matchers::WithinAbs(1.5, 0.0001));
    REQUIRE_THAT(test_vec.y(), Catch::Matchers::WithinAbs(2.5, 0.0001));
    REQUIRE_THAT(test_vec.z(), Catch::Matchers::WithinAbs(3.5, 0.0001));

}