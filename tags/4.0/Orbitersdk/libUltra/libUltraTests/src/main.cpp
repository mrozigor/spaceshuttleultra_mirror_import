#include "gtest/gtest.h"

char debugString[255];

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

// hack to get around link errors (because test files can't use Orbiter libraries)
void oapiWriteLog(char* temp)
{
	// do nothing
	std::cout << temp << std::endl;
}

char* oapiDebugString()
{
	return debugString;
}
