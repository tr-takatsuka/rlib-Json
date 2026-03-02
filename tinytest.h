#pragma once

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <functional>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

namespace rlib
{
	struct TinyTest {

		using TestFunc = std::function<void()>;

		static std::vector<std::pair<std::string, TestFunc>>& tests() {
			static std::vector<std::pair<std::string, TestFunc>> tests;
			return tests;
		}
		static int& errorCount() {
			static int errorCount = 0;
			return errorCount;
		}

		TinyTest(const std::string& name, TestFunc fn) {
			tests().emplace_back(name, fn);
		}

		static int run() {
			errorCount() = 0;
			for (const auto& test : tests()) {
				try {
					std::cout << Color::blue << "run : " << test.first << Color::initial << std::endl;
					test.second();
				} catch (...) {
					std::cerr << Color::red << "unknown exception : " << test.first << Color::initial << std::endl;
					errorCount()++;
				}
			}
			const auto n = errorCount();
			if (n > 0) {
				std::cout << Color::red << "test failed " << n << " count" << Color::initial << std::endl;
			} else {
				std::cout << Color::green << "test succeeded" << Color::initial << std::endl;
			}
			return n;
		}

		template<typename... Args> static std::string format(const std::string& fmt, Args... args) {
			auto size = std::snprintf(nullptr, 0, fmt.c_str(), args...);
			if (size <= 0) return {};
			std::unique_ptr<char[]> buf(new char[size + 1]);
			std::snprintf(buf.get(), size + 1, fmt.c_str(), args...);
			return std::string(buf.get(), buf.get() + size);
		}

		struct Color {
			static constexpr const char* red = "\033[31m";
			static constexpr const char* green = "\033[32m";
			static constexpr const char* blue = "\033[34m";
			static constexpr const char* initial = "\033[0m";
		};
	};

}

#define TINYTEST_MESSAGE(msg) \
	do{ \
		std::cout << rlib::TinyTest::Color::initial << msg << std::endl; \
	}while(false)

#define TINYTEST_CHECK(cond) \
	do{ \
		if (!(cond)) { \
			std::cerr << rlib::TinyTest::Color::red << "CHECK failed " << __FILE__ << ":" << __LINE__ << " (" << #cond << ")" << rlib::TinyTest::Color::initial << std::endl; \
			rlib::TinyTest::errorCount()++; \
		} \
	}while(false)

#define TINYTEST_CHECK_SMALL(x, tol) \
	do{ \
		auto _x = (x); \
		auto _tol = (tol); \
		if (std::fabs(_x) > _tol) { \
			std::cerr << rlib::TinyTest::Color::red << "CHECK_SMALL failed " << __FILE__ << ":" << __LINE__ << " (" << #x << ", " << #tol << ")" << rlib::TinyTest::Color::initial << std::endl; \
			rlib::TinyTest::errorCount()++; \
		} \
	}while(false)

#define TINYTEST_CHECK_THROW(expr, exType) \
	do { \
		bool thrown = false; \
		try { expr; } \
		catch (const exType&) { thrown = true; } \
		catch (...) {} \
		if (!thrown) { \
			std::cerr << rlib::TinyTest::Color::red << "CHECK_THROW failed " << __FILE__ << ":" << __LINE__ << " (" << #expr << ", " << #exType << ")" << rlib::TinyTest::Color::initial << std::endl; \
			rlib::TinyTest::errorCount()++; \
		} \
	}while(false)

#define TINYTEST_CHECK_CLOSE(a,b,percent) \
	do{ \
		auto _a = (a); \
		auto _b = (b); \
		auto diff = std::fabs(_a - _b); \
		auto tol = std::fabs(_b) * (percent) / 100.0; \
		if (diff > tol) { \
			std::cerr << rlib::TinyTest::Color::red << "CHECK_CLOSE failed " << __FILE__ << ":" << __LINE__ << " (" << #a << ", " << #b << ", " << #percent <<")" << rlib::TinyTest::Color::initial << std::endl; \
			rlib::TinyTest::errorCount()++; \
		} \
	}while(false)

