/**
 * @file test_main.cpp
 * @brief Main test runner
 */

#include <iostream>

// Forward declarations of test functions
void test_process();
void test_schedulers();

int main(int argc, char **argv) {
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          CPU SCHEDULER SIMULATOR - TEST SUITE                ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n\n";
    
    int failures = 0;
    
    try {
        std::cout << "Running Process tests...\n";
        test_process();
        std::cout << "✓ Process tests passed\n\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Process tests failed: " << e.what() << "\n\n";
        failures++;
    }
    
    try {
        std::cout << "Running Scheduler tests...\n";
        test_schedulers();
        std::cout << "✓ Scheduler tests passed\n\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Scheduler tests failed: " << e.what() << "\n\n";
        failures++;
    }
    
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    if (failures == 0) {
        std::cout << "║               ALL TESTS PASSED                               ║\n";
    } else {
        std::cout << "║               SOME TESTS FAILED: " << failures << "                            ║\n";
    }
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    
    return failures;
}
