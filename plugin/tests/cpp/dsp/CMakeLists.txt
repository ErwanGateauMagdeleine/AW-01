add_library(kissfft STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/kissfft/kiss_fft.c
    ${CMAKE_CURRENT_SOURCE_DIR}/external/kissfft/kiss_fftr.c
)

add_executable(dspTests
    catch_main.cpp
    EnvelopeFollowerTests.h
    WahFilterTests.h
    kissFFTUtility.cpp
)

target_compile_features(dspTests PRIVATE cxx_std_20)

target_link_libraries(dspTests
    PRIVATE
        Catch2::Catch2WithMain
        kissfft
)

target_include_directories(dspTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/dsp/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/kissfft
)

add_test(NAME DSP_TESTS COMMAND dspTests)
