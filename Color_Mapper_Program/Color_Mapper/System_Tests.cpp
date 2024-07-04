#if TESTMODE 1

#include "../../../Libraries/Catch_Testing_Framework/catch.hpp"

#include "../Color_Mapper/System_Scripts/Event.h"

//Event tests
TEST_CASE("Calling events", "[Event.h]")
{

}

bool isCalled = false;
void testFunction(int num1, bool num2, double num3)
{
	REQUIRE(num1 == 1);
	REQUIRE(num2 == 0);
	REQUIRE(num3 == 234234234239.0f);
	isCalled = true;
}

TEST_CASE("Calling function events", "[Event.h]")
{
	FunctionEvent<int, bool, double> testEvent;

	isCalled = false;
	SECTION("Testing edge cases")
	{
		testEvent.Trigger(3, 0, 23.0f);
	}

	isCalled = false;
	SECTION("Calling a test event")
	{
		testEvent += testFunction;
		testEvent.Trigger(1, false, 234234234239.0f);

		REQUIRE(isCalled == true);


		isCalled = false;
		testEvent -= testFunction;
		testEvent.Trigger(1, false, 234234234239.0f);

		REQUIRE(isCalled == false);
	}
}

#endif