#include "doctest.hpp"
#include "../../../Mandatory/Commands/MODE/Mode_parsing.cpp"

TEST_SUITE("Test of Mode_parsing.cpp file")
{
	TEST_CASE("CheckModeFormat_function")
	{
		std::string	Modes;

		SUBCASE("TEST_1 true")
		{
			Modes = "+itkol";
			CHECK(CheckModeFormat(Modes) == true);
		}
		SUBCASE("TEST_2 true")
		{
			Modes = "+i";
			CHECK(CheckModeFormat(Modes) == true);
		}
		SUBCASE("TEST_3 true")
		{
			Modes = "-itkol";
			CHECK(CheckModeFormat(Modes) == true);
		}
		SUBCASE("TEST_4 true")
		{
			Modes = "-i";
			CHECK(CheckModeFormat(Modes) == true);
		}
		SUBCASE("TEST_4 false")
		{
			Modes = "";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_5 false")
		{
			Modes = "-";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_6 false")
		{
			Modes = "+";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_7 false")
		{
			Modes = "+j";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_8 false")
		{
			Modes = "-j";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_9 false")
		{
			Modes = "-itp";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_10 false")
		{
			Modes = "-itk+l";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_11 false")
		{
			Modes = "-itkl+";
			CHECK(CheckModeFormat(Modes) == false);
		}
	}
}
