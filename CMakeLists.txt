cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_STANDARD 11)

set(ENVSDK $ENV{PLAYDATE_SDK_PATH})

if (NOT ${ENVSDK} STREQUAL "")
	# Convert path from Windows
	file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
	execute_process(
			COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
			COMMAND head -n 1
			COMMAND cut -c9-
			OUTPUT_VARIABLE SDK
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif()

if (NOT EXISTS ${SDK})
	message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
	return()
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Game Name Customization
set(PLAYDATE_GAME_NAME ${GAME_NAME})
set(PLAYDATE_GAME_DEVICE ${GAME_NAME}_DEVICE)

project(${PLAYDATE_GAME_NAME} C ASM)


file(GLOB SOURCE src/*.c)

if (TOOLCHAIN STREQUAL "armgcc")
    add_executable(${PLAYDATE_GAME_DEVICE} ${SDK}/C_API/buildsupport/setup.c ${SOURCE})
else()
    add_library(${PLAYDATE_GAME_NAME} SHARED ${SOURCE})

    enable_testing()
    file(GLOB TEST_SOURCE test/*.c)
    add_executable(test_${PLAYDATE_GAME_NAME} ${TEST_SOURCE})
    add_test(NAME run_test COMMAND test_${PLAYDATE_GAME_NAME})
endif()


include(${SDK}/C_API/buildsupport/playdate_game.cmake)

