
#include "../src/BuildTests.h"

#ifdef BUILD_UNIT_TESTS
#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "../thirdParty/catch2/catch.hpp"
#endif
