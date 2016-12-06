#include "AerodynamicLookupTables.h"
#include "gtest\gtest.h"
#include <iostream>

class FourDLookupTest : public ::testing::Test
{
protected:
	Aerodynamics::FourDLookup table;

	virtual void SetUp()
	{
		table.Init("../TestFiles/FourDLookupTest.csv", true);
	}
};

// test which doesn't require interpolation; exact value is in lookup table
TEST_F(FourDLookupTest, NoInterpolation) {
	double cl, cym, crm;
	table.GetValues(1.0, 20.0, 1.0, 50.0, cl, cym, crm);
	ASSERT_NEAR(cl, 1.1, 1e-6);
	ASSERT_NEAR(cym, 1.1, 1e-6);
	ASSERT_NEAR(crm, 1.1, 1e-6);
}

TEST_F(FourDLookupTest, Interpolation) {
	double cl, cym, crm;
	table.GetValues(1.3, 12.8, 1.5, 61.0, cl, cym, crm);
	ASSERT_NEAR(cl, 1.1, 1.12542);
}
