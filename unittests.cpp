#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include <interpreter.h>
#include <TSwim.h>


// IMPORTANT NOTE:
// These are just a few examples from my solution and **should be removed**.
// Depending on your code design your class and functions names would differ



TEST_CASE("Test Interpreter with a bad JSON document", "[interpreter]") {
	/*{
		QString tester = "Test1.json";
		interpreter interp;
		interpreter interp3;
		interp = interp.fromJSON(tester);
		interp3 = interp.fromJSON(tester);
		REQUIRE_THROWS(interp.badCamera);
		REQUIRE_THROWS(interp3.badShape);
	}*/
	{
		QString tester = "I don't Exist";
		interpreter interp;
		REQUIRE_THROWS(interp.fromJSON(tester));
	}
}

TEST_CASE("Test the interpreter with a good file", "[interpreter]") {
	QString tester = "Test2.json";
	interpreter interp;
	REQUIRE_NOTHROW(interp.fromJSON(tester));
}

TEST_CASE("Test the Calculate class", "[TSwim]")
{
	QString tester = "Test2.json";
	interpreter interp;
	interp = interp.fromJSON(tester);
	Calculate Calc(interp,65536);
	Calc.run();
	REQUIRE(Calc.pix.pip.r == 0);
	REQUIRE(Calc.pix.pip.g == 0);
	REQUIRE(Calc.pix.pip.b == 0);
}

/*
#include "geometry.hpp"

TEST_CASE( "Test Basic Geometry: Vec3d ", "[geometry]" ) {

  Vec3d a(2,0,0), b(0,4,0);
  
  REQUIRE(a.x == Approx(2));
  REQUIRE(a.y == Approx(0));
  REQUIRE(a.z == Approx(0));

  REQUIRE(b.x == Approx(0));
  REQUIRE(b.y == Approx(4));
  REQUIRE(b.z == Approx(0));

  double adotb = dot(a,b);
  REQUIRE(adotb == Approx(0));

  Vec3d anorm = norm(a);
  REQUIRE(anorm.x == Approx(1));
  REQUIRE(anorm.y == Approx(0));
  REQUIRE(anorm.z == Approx(0));

  Vec3d bnorm = norm(b);
  REQUIRE(bnorm.x == Approx(0));
  REQUIRE(bnorm.y == Approx(1));
  REQUIRE(bnorm.z == Approx(0));
}
*/