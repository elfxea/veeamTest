#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <sys/sysinfo.h>

/**
 * Test case #1, 'File system'.
 *
 * @memberof \b TestSystem
 * @private \b prep() Checks if seconds passed since UNIX epoch begin is even.
 * @private \b run() Lists files from current user's home directory.
 * 
 * @example
 * TestCase1 test;
 * test.execute(); // inherit execute() method from TestSystem class.
 */
class TestCase1 : public TestSystem {
private:
    bool prep() override {
        const auto time_now = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        return time_now % 2;
    }

    void run() override {
        for (auto &file : std::filesystem::directory_iterator(std::getenv("HOME"))) {
            std::cout << file.path() << '\n';
        }
    }

public:
    TestCase1() : TestSystem() {
        tc_id = 1;
        tc_name = "File list";
    }
};

/**
 * Test case #2, 'Random file'.
 *
 * @memberof \b TestSystem
 * @private \b prep() Checks if available operative memory is >= 1 GB.
 * @private \b run() Creates file sized 1 KB with random contents.
 * @private \b clean_up() Deletes created file.
 * 
 * @example
 * TestCase2 test;
 * test.execute(); // inherit execute() method from TestSystem class.
 */
class TestCase2 : public TestSystem {
private:
    bool prep() override {
        struct sysinfo info {};
        sysinfo(&info);
        if (info.freeram/(1024*1024*1024) < 1)
            return false;
        return true;
    }

    void run() override {
        char *content = (char *) malloc(1024);
        std::ofstream file("test.txt");

        if (!file.is_open())
            throw -1;

        for (int i = 0; i < 1024; ++i)
            file << content[i];

        file.close();
    }

    void clean_up() override {
        if (remove("test.txt"))
            throw -1;
    }

public:
    TestCase2() : TestSystem() {
        tc_id = 2;
        tc_name = "Random file";
    }
};

int main() {
    // Tests for test system
    
    TestCase1 test_fs;
    while (true) {
        auto i = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        bool f = i % 2;
        if (!(i % 2)) {
            test_fs.execute(); // Must be failed
            break;
        }
    }

    while (true) {
        auto i = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        bool f = i % 2;
        if (i % 2) {
            test_fs.execute(); // Must be success
            break;
        }
    }

    TestCase2 test_rf;
    test_rf.execute(); // Should be success

    struct sysinfo info {};
    sysinfo(&info);

    // Limit available memory
    long long **limit = (long long **) malloc(16542 * sizeof(long long *));
    for (int i = 0; i < 16542; ++i)
        limit[i] = (long long *) malloc(16777216 * sizeof(long long ));

    unsigned long i = info.freeram/(1024*1024*1024);
    std::cout << sizeof(long long) << '\n';
    std::cout << i << '\n';

    test_rf.execute(); // Should be failed

    TestSystem f;

    for (int i = 0; i < 16542; ++i)
        free(limit[i]);
    free(limit);

    return 0;
}
