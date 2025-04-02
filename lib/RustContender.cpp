#include "RustContender.h"

void RustContender::beforeConstruction(const std::vector<std::string> &keys) {
    std::cout << "Converting input for Rust" << std::endl;
    for (size_t i = 0; i < keys.size(); i++) {
        keysAsCString[i] = keys[i].c_str();
    }
    freeKeysWrapper();
    keysRustWrapper = convertToVecSlice(keys.size(), keysAsCString);
}

void RustContender::beforeQueries(const std::span<std::string> &keys) {
    std::cout << "Converting input for Rust" << std::endl;
    for (size_t i = 0; i < keys.size(); i++) {
        keysAsCString[i] = keys[i].c_str();
    }
    freeKeysWrapper();
    keysRustWrapper = convertToVecSlice(keys.size(), keysAsCString);
}
