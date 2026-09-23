#include "shape.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Constructors") {

    Sphere s;

    REQUIRE_THAT(s.getCenter().x(), Catch::Matchers::WithinAbs(0, 0.0001));
    REQUIRE_THAT(s.getCenter().y(), Catch::Matchers::WithinAbs(0, 0.0001));
    REQUIRE_THAT(s.getCenter().z(), Catch::Matchers::WithinAbs(-1, 0.0001));
    REQUIRE_THAT(s.getRadius(), Catch::Matchers::WithinAbs(1, 0.0001));


    vec3 center(1,1,-1);

    Sphere z(center, 15.5);

    REQUIRE_THAT(z.getCenter().x(), Catch::Matchers::WithinAbs(1, 0.0001));
    REQUIRE_THAT(z.getCenter().y(), Catch::Matchers::WithinAbs(1, 0.0001));
    REQUIRE_THAT(z.getCenter().z(), Catch::Matchers::WithinAbs(-1, 0.0001));
    REQUIRE_THAT(z.getRadius(), Catch::Matchers::WithinAbs(15.5, 0.0001));
}

TEST_CASE("Ray directly hits sphere") {

    vec3 origin(0, 0, 0);
    vec3 direction(0, 0, -1);

    ray r(origin, direction);

    Sphere s(vec3(0, 0, -100), 15.0);

    REQUIRE(s.intersect(r) == true);
}


TEST_CASE("Ray misses sphere") {

    vec3 origin(0, 0, 0);
    vec3 direction(0, 1, 0);

    ray r(origin, direction);

    Sphere s(vec3(0, 0, -100), 15.0);

    REQUIRE(s.intersect(r) == false);
}


TEST_CASE("Ray starts inside sphere") {

    Sphere s(vec3(0, 0, 0), 15.0);

    vec3 origin(0, 0, 0);
    vec3 direction(1, 0, 0);

    ray r(origin, direction);

    REQUIRE(s.intersect(r) == true);
}
