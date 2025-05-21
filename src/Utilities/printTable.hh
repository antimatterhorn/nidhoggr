#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cassert>

// Forward declaration
template<typename... Fields>
void printTable(size_t size, const Fields&... fields);

// Internal helper to print header
inline void printHeaders() { }

template<typename First, typename... Rest>
void printHeaders(const First& first, const Rest&... rest) {
    std::cout << std::setw(16) << first.getNameString();
    if constexpr (sizeof...(rest) > 0) {
        printHeaders(rest...);
    } else {
        std::cout << "\n";
    }
}

// Internal helper to print each row
inline void printRow(size_t /*i*/) { }

template<typename First, typename... Rest>
void printRow(size_t i, const First& first, const Rest&... rest) {
    std::cout << std::setw(16) << std::scientific << first[i];
    if constexpr (sizeof...(rest) > 0) {
        printRow(i, rest...);
    } else {
        std::cout << "\n";
    }
}

// Main printTable function
template<typename... Fields>
void printTable(size_t size, const Fields&... fields) {
    std::cout << std::fixed << std::setprecision(6);

    printHeaders(fields...);

    for (size_t i = 0; i < size; ++i) {
        printRow(i, fields...);
    }
}
