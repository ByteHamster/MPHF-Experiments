cmake_minimum_required(VERSION 3.25...4.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
project(MPHF-Experiments)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif ()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-march=native>)
endif()

###################### Competitor setup ######################
add_library(PerfectHashingCompetitors INTERFACE)
target_compile_features(PerfectHashingCompetitors INTERFACE cxx_std_20)
target_include_directories(PerfectHashingCompetitors SYSTEM INTERFACE extlib/BBHash)
target_include_directories(PerfectHashingCompetitors SYSTEM INTERFACE extlib/fch/include)
target_link_libraries(PerfectHashingCompetitors INTERFACE dl)
target_include_directories(PerfectHashingCompetitors INTERFACE contenders)

add_subdirectory(extlib/tlx SYSTEM)
target_link_libraries(PerfectHashingCompetitors INTERFACE tlx)

add_subdirectory(extlib/util)
target_link_libraries(PerfectHashingCompetitors INTERFACE ByteHamsterUtil)

file(GLOB cmphSources extlib/cmph/src/**.c)
list(FILTER cmphSources EXCLUDE REGEX ".*cmph/src/(bdz_gen_lookup_table|bm_numbers|sdbm_hash|fnv_hash|djb2_hash|hashtree|buffer_manage|main).c$")
add_library(Cmph SHARED ${cmphSources})
target_include_directories(Cmph SYSTEM INTERFACE extlib/cmph/src)

target_link_libraries(PerfectHashingCompetitors INTERFACE Cmph)

file(GLOB_RECURSE wbpmSources extlib/mphf-wbpm/**.c)
list(FILTER wbpmSources EXCLUDE REGEX ".*(test|cli|example|xxh_x86dispatch).*")
add_library(MphfWbpm SHARED ${wbpmSources})
target_include_directories(MphfWbpm SYSTEM PRIVATE extlib/mphf-wbpm/include)
target_include_directories(MphfWbpm SYSTEM PRIVATE extlib/mphf-wbpm/lib/c_list_types/include)
target_include_directories(MphfWbpm SYSTEM PRIVATE extlib/mphf-wbpm/lib/XORSATFilter/include)
target_include_directories(MphfWbpm SYSTEM PRIVATE extlib/mphf-wbpm/lib/XORSATFilter/lib/bitvector/include)
target_include_directories(MphfWbpm SYSTEM PRIVATE extlib/mphf-wbpm/lib/XORSATFilter/lib/xxHash)
target_include_directories(MphfWbpm SYSTEM PRIVATE extlib/mphf-wbpm/lib/weighted-bipartite-perfect-matching/include)
target_include_directories(MphfWbpm SYSTEM PUBLIC extlib/mphf-wbpm/include)
target_link_libraries(PerfectHashingCompetitors INTERFACE MphfWbpm)

add_subdirectory(extlib/ShockHash)
target_link_libraries(PerfectHashingCompetitors INTERFACE ShockHash)
if(TARGET ShockHashSIMD)
    target_link_libraries(PerfectHashingCompetitors INTERFACE ShockHashSIMD)
endif()

add_subdirectory(extlib/gpurecsplit)
target_link_libraries(PerfectHashingCompetitors INTERFACE RecSplit)
if(TARGET SIMDRecSplit)
    target_link_libraries(PerfectHashingCompetitors INTERFACE SIMDRecSplit)
endif()

add_subdirectory(extlib/sichash)
target_link_libraries(PerfectHashingCompetitors INTERFACE SicHash)

add_subdirectory(extlib/morphishash)
target_link_libraries(PerfectHashingCompetitors INTERFACE MorphisHash)

add_subdirectory(extlib/fips)
target_link_libraries(PerfectHashingCompetitors INTERFACE FiPS)

if(NOT TARGET Sux)
    add_library(Sux INTERFACE)
    target_include_directories(Sux SYSTEM INTERFACE extlib/sux)
endif()
target_link_libraries(PerfectHashingCompetitors INTERFACE Sux)

file(GLOB_RECURSE RustSourceFiles "${CMAKE_SOURCE_DIR}/rust/src/**.rs")
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/target/release/libmphf_experiments.a
    COMMAND bash -c "RUSTFLAGS='-C target-cpu=native -C linker=${CMAKE_C_COMPILER}' \
                     CARGO_TARGET_DIR='${CMAKE_CURRENT_BINARY_DIR}/target' \
                     cargo build --release"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/rust
    DEPENDS ${RustSourceFiles})
add_custom_target(RustCompilation DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/target/release/libmphf_experiments.a")
add_library(RustCompetitors STATIC IMPORTED)
set_property(TARGET RustCompetitors PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/target/release/libmphf_experiments.a")
add_dependencies(RustCompetitors RustCompilation)
target_link_libraries(PerfectHashingCompetitors INTERFACE RustCompetitors)

find_package(Vulkan)
if (Vulkan_FOUND AND Vulkan_glslc_FOUND)
    add_subdirectory(extlib/phobic-gpu)
    target_link_libraries(PerfectHashingCompetitors INTERFACE GpuPTHash)
    target_compile_definitions(PerfectHashingCompetitors INTERFACE HAS_VULKAN)
endif()

add_subdirectory(extlib/pthash)
target_link_libraries(PerfectHashingCompetitors INTERFACE PTHASH)

add_subdirectory(extlib/consensus)
target_link_libraries(PerfectHashingCompetitors INTERFACE ConsensusRecSplit)

#################### CUDA Competitor setup ####################
if (CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    add_library(PerfectHashingCompetitorsCuda INTERFACE)
    target_link_libraries(PerfectHashingCompetitorsCuda INTERFACE GPURecSplit ByteHamsterUtil)
    target_include_directories(PerfectHashingCompetitorsCuda INTERFACE lib)
    target_include_directories(PerfectHashingCompetitorsCuda INTERFACE contenders)
endif()

add_library(Contender SHARED lib/Contender.cpp lib/RustContender.cpp)
target_include_directories(Contender PUBLIC lib)
target_link_libraries(Contender PRIVATE ByteHamsterUtil tlx RustCompetitors)

add_library(PerfectHashingCompetitorParetoRunners INTERFACE)
file(GLOB_RECURSE contendersCpp "${CMAKE_SOURCE_DIR}/contenders/**.cpp")
foreach(filename IN LISTS contendersCpp)
    get_filename_component(name "${filename}" NAME_WLE)
    if (name STREQUAL "GpuRecSplitContender")
        continue()
    endif()
    if (name STREQUAL "GpuPhobicContender" AND NOT TARGET GpuPTHash)
        continue()
    endif()
    add_library(${name} SHARED ${filename})
    target_link_libraries(${name} PRIVATE PerfectHashingCompetitors Contender)
    target_link_libraries(PerfectHashingCompetitorParetoRunners INTERFACE ${name})
endforeach()
target_link_libraries(PerfectHashingCompetitors INTERFACE Contender)
if (CMAKE_CUDA_COMPILER)
    target_link_libraries(PerfectHashingCompetitorsCuda INTERFACE Contender)
endif()

################### Benchmark build targets ###################
add_executable(Comparison src/comparison.cpp)
target_link_libraries(Comparison PRIVATE PerfectHashingCompetitors PerfectHashingCompetitorParetoRunners Sux)
target_compile_features(Comparison PRIVATE cxx_std_20)

if (CMAKE_CUDA_COMPILER)
    add_executable(ComparisonCuda src/comparisonCuda.cu)
    target_link_libraries(ComparisonCuda PRIVATE PerfectHashingCompetitorsCuda Sux)
    set_target_properties(ComparisonCuda PROPERTIES CUDA_SEPERABLE_COMPILATION ON)
    set_target_properties(ComparisonCuda PROPERTIES CUDA_ARCHITECTURES "50;61;72")
endif()

add_executable(ScalingThesisLehmann src/scalingThesisLehmann.cpp)
target_link_libraries(ScalingThesisLehmann PRIVATE PerfectHashingCompetitors Sux)
target_compile_features(ScalingThesisLehmann PRIVATE cxx_std_20)

add_executable(ScalingSurvey src/scalingSurvey.cpp)
target_link_libraries(ScalingSurvey PRIVATE PerfectHashingCompetitors Sux)
target_compile_features(ScalingSurvey PRIVATE cxx_std_20)

add_executable(TablePhobic src/tablePhobic.cpp)
target_link_libraries(TablePhobic PRIVATE PerfectHashingCompetitors Sux)
target_compile_features(TablePhobic PRIVATE cxx_std_20)

add_executable(TableThesisLehmann src/tableThesisLehmann.cpp)
target_link_libraries(TableThesisLehmann PRIVATE PerfectHashingCompetitors Sux)
target_compile_features(TableThesisLehmann PRIVATE cxx_std_20)

add_executable(TableShockHash src/tableShockHash.cpp)
target_link_libraries(TableShockHash PRIVATE PerfectHashingCompetitors Sux)
target_compile_features(TableShockHash PRIVATE cxx_std_20)

add_executable(TablePtrHash src/tablePtrHash.cpp)
target_link_libraries(TablePtrHash PRIVATE PerfectHashingCompetitors Sux)
target_compile_features(TablePtrHash PRIVATE cxx_std_20)

add_executable(TableSurvey src/tableSurvey.cpp)
target_link_libraries(TableSurvey PRIVATE PerfectHashingCompetitors Sux)
target_compile_features(TableSurvey PRIVATE cxx_std_20)

add_executable(TableMorphisHash src/tableMorphisHash.cpp)
target_link_libraries(TableMorphisHash PRIVATE PerfectHashingCompetitors Sux)
target_compile_features(TableMorphisHash PRIVATE cxx_std_20)

add_executable(TablePHast src/tablePHast.cpp)
target_link_libraries(TablePHast PRIVATE PerfectHashingCompetitors Sux)
target_compile_features(TablePHast PRIVATE cxx_std_20)
