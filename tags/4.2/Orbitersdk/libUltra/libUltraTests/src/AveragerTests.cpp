#include "Averager.h"
#include "gtest/gtest.h"

TEST(AveragerTest, ConstantIntervals) {
	Averager avg(10.0);
	for(int i=0; i<20; i++) avg.NewValue(50.0, static_cast<double>(i));
	avg.NewValue(53.0, 30.0);
	avg.NewValue(59.0, 32.0);
	avg.NewValue(55.9, 32.0);
	avg.NewValue(54.0, 33.0);
	avg.NewValue(56.2, 34.0);
	avg.NewValue(52.0, 35.0);
	avg.NewValue(50.0, 36.0);
	avg.NewValue(52.0, 37.0);
	avg.NewValue(58.0, 38.0);
	avg.NewValue(55.5, 39.0);
	ASSERT_NEAR(avg.GetAvgValue(), 54.56, 1e-5);
}

TEST(AveragerTest, ChangingIntervals) {
	Averager avg(2.5);
	avg.NewValue(14.0, 0.5);
	avg.NewValue(14.0, 0.9);
	avg.NewValue(19.0, 1.4);
	avg.NewValue(12.0, 1.9);
	avg.NewValue(18.0, 2.6);
	avg.NewValue(14.0, 3.4);
	avg.NewValue(17.0, 3.6);
	ASSERT_NEAR(avg.GetAvgValue(), 16.0, 1e-5);
}

TEST(AveragerTest, FewSamples) {
	Averager avg(10.0);
	avg.NewValue(5.6, 0.1);
	avg.NewValue(8.3, 0.2);
	avg.NewValue(4.4, 0.3);
	avg.NewValue(6.0, 0.4);
	avg.NewValue(4.6, 0.5);
	avg.NewValue(9.4, 0.6);
	avg.NewValue(6.5, 0.7);
	ASSERT_NEAR(avg.GetAvgValue(), 6.4, 1e-5);
}

TEST(AveragerTest, OneSample) {
	Averager avg(10);
	avg.NewValue(1.0, 2.0);
	ASSERT_NEAR(avg.GetAvgValue(), 1.0, 1e-5);
}
