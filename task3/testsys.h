#include <chrono>
#include <string>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <sys/sysinfo.h>
/**
 * Abstract class for test cases.
 *
 * @protected \b tc_id Test id
 * @protected \b tc_name test name
 * @protected \b prep() Preparation stage of test
 * @protected \b run() Executable stage of test
 * @protected \b clean_up() Cleaning up test temp files
 *
 * @public \b execute() Executing test
 *
 * @example
 * class TestCase1 : public TestSystem {}
 * TestCase1.execute();
 */
class TestSystem {
protected:
    /// Test case id
    long long tc_id;
    /// Test case name
    std::string tc_name;

    /**
     * A preparation test case method.
     *
     * @return \b true if not overridden.
     */
    virtual bool prep() { return true; };

    virtual void run() {};

    virtual void clean_up() {};

public:
    /**
     * A public method to execute test. Made virtual to change it if necessary. Runs prep(), run() and clean_up()
     * protected methods.
     */
    virtual void execute() {
        if (!prep()) {
            std::cerr << "Test #" << tc_id << " '" << tc_name << "' finished on preparation stage.\n";
            return;
        }

        try {
            run();
        } catch (int errcode) {
            switch (errcode) {
                default:
                    std::cerr << "Test #" << tc_id << " '" << tc_name << "' finished with error code " << errcode <<
                              "\n\tUnknown error code.\n";
                    break;
                case -1:
                    std::cerr << "Test #" << tc_id << " '" << tc_name << "' finished with error code " << errcode <<
                              "\n\tNot enough rights to write temp file.\n";
                    break;
            }
            clean_up();
            return;
        }

        try {
            clean_up();
        } catch (int errcode) {
            switch (errcode) {
                default:
                    std::cerr << "Test #" << tc_id << " '" << tc_name << "' threw error code " << errcode <<
                              "\n\tUnknown error code.\n";
                    break;
                case -1:
                    std::cerr << "Test #" << tc_id << " '" << tc_name << "' threw error code " << errcode <<
                              "\n\tNot enough rights to delete temp file.\n";
                    break;
            }
        }
        std::cout << "Test #" << tc_id << " '" << tc_name << "': success.\n";
    }
};
