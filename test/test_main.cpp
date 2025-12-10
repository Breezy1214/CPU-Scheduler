/**
 * @file test_main.cpp
 * @brief Main test runner for Google Test
 */

#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char **argv) {
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          CPU SCHEDULER SIMULATOR - TEST SUITE                ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n\n";
    
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    TEST RUN COMPLETE                         ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    
    return result;
}
