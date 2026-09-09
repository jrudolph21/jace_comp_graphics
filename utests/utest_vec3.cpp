#include "vec3.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Testing vector constructors") {

    vec3 v1;
    vec3 v2(4,5.3,6.01);

    REQUIRE(v1.x() == 0.0);
    REQUIRE(v1.y() == 0.0);
    REQUIRE(v1.z() == 0.0);

    REQUIRE(v2.x() == 4.0);
    REQUIRE(v2.y() == 5.3);
    REQUIRE(v2.z() == 6.01);
}


TEST_CASE("Testing vector addition") {
    vec3 v1(1,2,3);
    vec3 v2(1,2,3);
    vec3 v3 = v1 + v2;

    REQUIRE(v3.x() == 2.0);
    REQUIRE(v3.y() == 4.0); 
    REQUIRE(v3.z() == 6.0);
}

TEST_CASE("Testing compound addition") {
    vec3 v1(1,2,3);
    vec3 v2(4,5,6);

    v1 += v2;

    REQUIRE(v1.x() == 5.0);
    REQUIRE(v1.y() == 7.0);
    REQUIRE(v1.z() == 9.0);
}


TEST_CASE("Testing vector subtraction") {
    vec3 v1(1,2,3);
    vec3 v2(1,2,3);
    vec3 v3 = v1 - v2;

    REQUIRE(v3.x() == 0.0);
    REQUIRE(v3.y() == 0.0); 
    REQUIRE(v3.z() == 0.0);
}

TEST_CASE("Testing vector multiplication by scalar") {
    vec3 v1(1,2,3);
    double s = 3;

    vec3 v3 = v1 * s;

    REQUIRE(v3.x() == 3.0);
    REQUIRE(v3.y() == 6.0); 
    REQUIRE(v3.z() == 9.0);
}

TEST_CASE("Testing compound multiplication by scalar") {
    vec3 v1(1,2,3);
    double s = 3;

    v1 *= s;

    REQUIRE(v1.x() == 3.0);
    REQUIRE(v1.y() == 6.0); 
    REQUIRE(v1.z() == 9.0);
}

TEST_CASE("Testing vector division by scalar") {
    vec3 v1(1,2,3);
    double s = 2;

    vec3 v3 = v1 / s;

    REQUIRE(v3.x() == 0.5);
    REQUIRE(v3.y() == 1.0); 
    REQUIRE(v3.z() == 1.5);
}

TEST_CASE("Testing compound division by scalar") {
    vec3 v1(1,2,3);
    double s = 2;

    v1 /= s;

    REQUIRE(v1.x() == 0.5);
    REQUIRE(v1.y() == 1.0); 
    REQUIRE(v1.z() == 1.5);
}   

TEST_CASE("Testing vector length squared") {
    vec3 v1(3,4,0);
    double len_sq = v1.length_squared();

    REQUIRE(len_sq == 25.0);
}

TEST_CASE("Testing vector length") {
    vec3 v1(3,4,0);
    double len = v1.length();

    REQUIRE(len == 5.0);
}

TEST_CASE("Testing dot product") {
    vec3 v1(1,2,3);
    vec3 v2(4,5,6);

    double dot = v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();

    REQUIRE(dot == 32.0);
}

TEST_CASE ("Testing cross product") {
    vec3 v1(1,2,3);
    vec3 v2(2,3,4);
    
    vec3 v3 = cross(v1, v2);

    REQUIRE(v3.x() == -1.0);
    REQUIRE(v3.y() == 2.0);
    REQUIRE(v3.z() == -1.0);
}

TEST_CASE("Testing unit vector") {
    vec3 v1(3,4,0);
    double len = v1.length();
    vec3 unit_v1 = v1 / len;


    float eps = 1.0e-3; // a tolerance for how close the answer needs to be
    float expected_x = 0.6;
    float expected_y = 0.8;
    float expected_z = 0.0;

    REQUIRE_THAT(unit_v1.x(), Catch::Matchers::WithinAbs(expected_x, eps));
    REQUIRE_THAT(unit_v1.y(), Catch::Matchers::WithinAbs(expected_y, eps));
    REQUIRE_THAT(unit_v1.z(), Catch::Matchers::WithinAbs(expected_z, eps));
}


