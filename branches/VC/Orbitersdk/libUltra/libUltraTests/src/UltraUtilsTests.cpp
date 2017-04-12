#include "UltraUtils.h"
#include "gtest/gtest.h"

TEST(ReadCSVLineTest, ReadInts) {
	std::string sVals = "1,-2, 3, 4, 10,11, -12";
	std::vector<int> vVals;

	ReadCSVLine(sVals, vVals);

	ASSERT_EQ(7, vVals.size());
	ASSERT_EQ(1, vVals[0]);
	ASSERT_EQ(-2, vVals[1]);
	ASSERT_EQ(3, vVals[2]);
	ASSERT_EQ(4, vVals[3]);
	ASSERT_EQ(10, vVals[4]);
	ASSERT_EQ(11, vVals[5]);
	ASSERT_EQ(-12, vVals[6]);
}

TEST(ReadCSVLineTest, ReadDoubles) {
	std::string sVals = "0.01512,-0.01900, 0.02520,0.03580, -0.04704,0.05968,0.07843";
	std::vector<double> vVals;

	ReadCSVLine(sVals, vVals);

	ASSERT_EQ(7, vVals.size());
	ASSERT_DOUBLE_EQ(0.01512, vVals[0]);
	ASSERT_DOUBLE_EQ(-0.019, vVals[1]);
	ASSERT_DOUBLE_EQ(0.0252, vVals[2]);
	ASSERT_DOUBLE_EQ(0.0358, vVals[3]);
	ASSERT_DOUBLE_EQ(-0.04704, vVals[4]);
	ASSERT_DOUBLE_EQ(0.05968, vVals[5]);
	ASSERT_DOUBLE_EQ(0.07843, vVals[6]);
}
