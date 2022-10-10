cmake_minimum_required(VERSION 3.16)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
project(MPHF-Experiments)

if(CMAKE_BUILD_TYPE STREQUAL "Release" AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_compile_options(-march=native)
endif()

###################### Competitor setup ######################
add_library(PerfectHashingCompetitors INTERFACE)
target_compile_features(PerfectHashingCompetitors INTERFACE cxx_std_20)
target_include_directories(PerfectHashingCompetitors SYSTEM INTERFACE extlib/pthash/include)
target_include_directories(PerfectHashingCompetitors SYSTEM INTERFACE extlib/BBHash)
target_link_libraries(PerfectHashingCompetitors INTERFACE SicHash)

file(GLOB cmphSources extlib/cmph/src/**.c)
list(FILTER cmphSources EXCLUDE REGEX ".*cmph/src/(bdz_gen_lookup_table|bm_numbers|sdbm_hash|fnv_hash|djb2_hash|hashtree|buffer_manage|main).c$")
add_library(Cmph SHARED ${cmphSources})
target_include_directories(Cmph SYSTEM INTERFACE extlib/cmph/src)
target_link_libraries(PerfectHashingCompetitors INTERFACE Cmph)

add_library(Sux INTERFACE)
target_include_directories(Sux SYSTEM INTERFACE extlib/sux)
target_link_libraries(PerfectHashingCompetitors INTERFACE Sux)

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

add_subdirectory(extlib/gpurecsplit)
target_link_libraries(PerfectHashingCompetitors INTERFACE SIMDRecSplit)

add_subdirectory(extlib/sichash)
target_link_libraries(PerfectHashingCompetitors INTERFACE SicHash)

################### Benchmark build targets ###################
add_executable(Comparison comparison.cpp)
target_link_libraries(Comparison PRIVATE PerfectHashingCompetitors)
target_compile_features(Comparison PRIVATE cxx_std_20)

add_executable(ComparisonN comparisonN.cpp)
target_link_libraries(ComparisonN PRIVATE PerfectHashingCompetitors)
target_compile_features(ComparisonN PRIVATE cxx_std_20)

add_executable(ComparisonThreads comparisonThreads.cpp)
target_link_libraries(ComparisonThreads PRIVATE PerfectHashingCompetitors)
target_compile_features(ComparisonThreads PRIVATE cxx_std_20)
