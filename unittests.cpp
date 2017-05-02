#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include <interpreter.h>
#include <TSwim.h>


// IMPORTANT NOTE:
// These are just a few examples from my solution and **should be removed**.
// Depending on your code design your class and functions names would differ

TEST_CASE("Test Camera class constructors", "[camera.cpp]")
{
	{
		camera cam;
		REQUIRE(cam.CamStruct.center.x == 0);
		REQUIRE(cam.CamStruct.center.y == 0);
		REQUIRE(cam.CamStruct.center.z == 0);
		REQUIRE(cam.CamStruct.normal.x == 0);
		REQUIRE(cam.CamStruct.normal.y == 0);
		REQUIRE(cam.CamStruct.normal.z == 0);
		REQUIRE(cam.CamStruct.focus == 0);
		REQUIRE(cam.CamStruct.size[0] == 0);
		REQUIRE(cam.CamStruct.size[1] == 0);
		REQUIRE(cam.CamStruct.resolution[0] == 0);
		REQUIRE(cam.CamStruct.resolution[1] == 0);
	}
	
}
TEST_CASE("Test Light class constructors", "[lights.cpp]")
{
	lights lit;
	REQUIRE(lit.lite.intensity == 0);
	REQUIRE(lit.lite.location.x == 0);
	REQUIRE(lit.lite.location.y == 0);
	REQUIRE(lit.lite.location.z == 0);
}
TEST_CASE("Test Shape class constructors", "[shapes.cpp]")
{
	shapes shape;
	REQUIRE(shape.Shps.center.x == 0);
	REQUIRE(shape.Shps.center.y == 0);
	REQUIRE(shape.Shps.center.z == 0);
	REQUIRE(shape.Shps.color.r == 0);
	REQUIRE(shape.Shps.color.g == 0);
	REQUIRE(shape.Shps.color.b == 0);
	REQUIRE(shape.Shps.lambert == 0);
	REQUIRE(shape.Shps.normal.x == 0);
	REQUIRE(shape.Shps.normal.y == 0);
	REQUIRE(shape.Shps.normal.z == 0);
	REQUIRE(shape.Shps.type == "neither");
	REQUIRE(shape.Shps.radius == 0);
}
TEST_CASE("Test Pixel class constructors", "[PixelStruct.h]") 
{
	Pixall pix;
	REQUIRE(pix.pip.x == 0);
	REQUIRE(pix.pip.y == 0);
	REQUIRE(pix.pip.r == 0);
	REQUIRE(pix.pip.g == 0);
	REQUIRE(pix.pip.b == 0);
	REQUIRE(pix.pip.rgb == 0);
	REQUIRE(pix.shadow.x == 0);
	REQUIRE(pix.shadow.y == 0);
	REQUIRE(pix.shadow.z == 0);
	REQUIRE(pix.shadow.index == 0);
}

TEST_CASE("Test Interpreter with a bad JSON document", "[interpreter]") {
	{
		QString tester = "/vagrant/unttests/Test1.json";
		interpreter interp;
		REQUIRE_THROWS(interp.fromJSON(tester));		
	}
	{
		QString tester = "I don't Exist";
		interpreter interp;
		REQUIRE_THROWS(interp.fromJSON(tester));
	}
}

TEST_CASE("Test the interpreter with a good file", "[interpreter]") {
	QString tester = "/vagrant/unttests/Test2.json";
	interpreter interp;
	REQUIRE_NOTHROW(interp.fromJSON(tester));
	REQUIRE(interp.Cams.CamStruct.center.x == 0);
	REQUIRE(interp.Cams.CamStruct.center.y == 0);
	REQUIRE(interp.Cams.CamStruct.center.z == 0);
	REQUIRE(interp.Cams.CamStruct.normal.x == 0);
	REQUIRE(interp.Cams.CamStruct.normal.y == 0);
	REQUIRE(interp.Cams.CamStruct.normal.z == 1);
	REQUIRE(interp.Cams.CamStruct.focus == 10);
	REQUIRE(interp.Cams.CamStruct.size[0] == 256);
	REQUIRE(interp.Cams.CamStruct.size[1] == 256);
	REQUIRE(interp.Cams.CamStruct.resolution[0] == 0.01);
	REQUIRE(interp.Cams.CamStruct.resolution[1] == 0.01);
	REQUIRE(interp.ShapeList.size() == 1);
	REQUIRE(interp.LightList.size() == 1);
	REQUIRE(interp.LightList[0].lite.intensity == 1);
	REQUIRE(interp.LightList[0].lite.location.x == 5);
	REQUIRE(interp.LightList[0].lite.location.y == -5);
	REQUIRE(interp.LightList[0].lite.location.z == 0);
	REQUIRE(interp.ShapeList[0].Shps.center.x == 0);
	REQUIRE(interp.ShapeList[0].Shps.center.y == 0);
	REQUIRE(interp.ShapeList[0].Shps.center.z == 5);
	REQUIRE(interp.ShapeList[0].Shps.color.r == 255);
	REQUIRE(interp.ShapeList[0].Shps.color.g == 0);
	REQUIRE(interp.ShapeList[0].Shps.color.b == 0);
	REQUIRE(interp.ShapeList[0].Shps.lambert == 1);
	REQUIRE(interp.ShapeList[0].Shps.normal.x == 0);
	REQUIRE(interp.ShapeList[0].Shps.normal.y == 0);
	REQUIRE(interp.ShapeList[0].Shps.normal.z == 0);
	REQUIRE(interp.ShapeList[0].Shps.type == "sphere");
	REQUIRE(interp.ShapeList[0].Shps.radius == 1);
}

TEST_CASE("Test the Calculate class", "[TSwim]")
{
	{
		QString tester = "/vagrant/unttests/Test2.json";
		interpreter interp;
		interp = interp.fromJSON(tester);
		Calculate Calc(interp, 65536);
		Calc.run();
		REQUIRE(Calc.pix.pip.r == 0);
		REQUIRE(Calc.pix.pip.g == 0); 
		REQUIRE(Calc.pix.pip.b == 0);
	}
	{
		QString tester = "/vagrant/unttests/Test2.json";
		interpreter interp;
		interp = interp.fromJSON(tester);
		Calculate Calc(interp, 24755);
		tester = "/vagrant/unttests/Test3.json";
		interp = interp.fromJSON(tester);
		Calculate Calc2(interp, 65536);
		REQUIRE_NOTHROW(Calc.run());
		REQUIRE_NOTHROW(Calc2.run());
		REQUIRE(Calc.inter.ShapeList.size() == 1);
		REQUIRE(Calc2.inter.ShapeList.size() == 2);
		REQUIRE(Calc2.inter.ShapeList[0].Shps.type == "sphere");
		REQUIRE(Calc2.inter.ShapeList[1].Shps.type == "plane");
	}
}

TEST_CASE("Testing the threadsafe queue", "[message_queue]")
{
	message_queue<int> tester;
	int one = 1;
	int two = 2;
	int three = 3;
	bool good = false;
	int hold;
	REQUIRE(tester.empty());
	tester.push(one);
	tester.push(two);
	tester.push(three);
	tester.try_pop(hold);
	REQUIRE(hold == one);
	tester.wait_and_pop(hold);
	REQUIRE(hold == two);
}