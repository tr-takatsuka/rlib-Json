# CMakeList.txt : test の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#
cmake_minimum_required (VERSION 3.8)

project ("test")

# デフォルトは Release ビルド
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING
	"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
	FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (test
	"./main.cpp"
	"./Json_test.cpp"
)

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。
