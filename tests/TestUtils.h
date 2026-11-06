#ifndef BKMOVE_A2_TESTUTILS_H
#define BKMOVE_A2_TESTUTILS_H

#include <cctype>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#define CHECK(expr) do { \
    if (!(expr)) { \
        std::ostringstream checkMessage; \
        checkMessage << __FILE__ << ':' << __LINE__ << ": CHECK failed: " << #expr; \
        throw std::runtime_error(checkMessage.str()); \
    } \
} while (0)

class PublicTestSuite {
    struct Case {
        std::string title;
        std::function<void()> run;
    };
    std::string question_;
    std::vector<Case> cases_;
public:
    explicit PublicTestSuite(std::string question) : question_(std::move(question)) {}
    void add(const std::string& name, const std::function<void()>& test) {
        cases_.push_back({name, test});
    }
    int run(int argc, char* argv[]) {
        std::size_t selected = 0;
        if (argc > 2) {
            std::cerr << "Usage: [TEST_NUMBER] (or omit for all cases)\n";
            return 2;
        }
        if (argc == 2) {
            const std::string number(argv[1]);
            if (number.empty()) {
                std::cerr << "Invalid test number\n";
                return 2;
            }
            for (char digit : number) {
                if (!(digit >= '0' && digit <= '9')) {
                    std::cerr << "Invalid test number: " << number << '\n';
                    return 2;
                }
            }
            try {
                selected = std::stoul(number);
            } catch (...) {
                std::cerr << "Invalid test number: " << number << '\n';
                return 2;
            }
            if (selected == 0 || selected > cases_.size()) {
                std::cerr << "Test number must be between 1 and " << cases_.size() << '\n';
                return 2;
            }
        }
        std::cout << "== " << question_ << " public tests ==\n";
        std::size_t passed = 0;
        std::size_t runCount = 0;
        for (std::size_t i = 0; i < cases_.size(); ++i) {
            if (selected != 0 && selected != i + 1) continue;
            ++runCount;
            try {
                cases_[i].run();
                ++passed;
                std::cout << "[PASS] " << i + 1 << ". " << cases_[i].title << '\n';
            } catch (const std::exception& error) {
                std::cout << "[FAIL] " << i + 1 << ". " << cases_[i].title
                          << " -- " << error.what() << '\n';
            } catch (...) {
                std::cout << "[FAIL] " << i + 1 << ". " << cases_[i].title
                          << " -- unknown exception\n";
            }
        }
        std::cout << "Result: " << passed << " / " << runCount << " tests passed\n";
        return passed == runCount ? 0 : 1;
    }
};
#endif
