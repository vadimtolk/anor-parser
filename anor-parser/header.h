#include <iostream>
#include <memory>
#include <chrono>
#include <windows.h>
#include <cstdio>
#include <iomanip>
#include <wchar.h>

typedef long double d32;

d32 getFlops();
short getLogicProcessors();
LPVOID memoryAlloc(const size_t& byte_value);

std::pair<d32, LPVOID>* calculateAnor(const size_t& byte_value) {
    LPVOID allocated_memory_ptr = memoryAlloc(byte_value);

    if (allocated_memory_ptr) {
        const short logicalProcessors = getLogicProcessors();
        const d32 flops = getFlops();
        const d32 anor = logicalProcessors * ( (byte_value / 1024 / 1024) + flops);

        return new std::pair(anor, allocated_memory_ptr);
    }

    return nullptr;
}

d32 getFlops() {
    const unsigned long long iterations = 1e9;
    d32 a = 1.0, b = 2.0, c = 3.0, average = 0.0;
    
    for (char i = 0; i < 5; i++) {
        auto start = std::chrono::high_resolution_clock::now();

        for (unsigned long long j = 0; j < iterations; j++) {
            a = b * c;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> chrono_diff = end - start;

        average += iterations / chrono_diff.count();
    }

    return average / 5 / 1024 / 1024;
}

short getLogicProcessors() {
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);

    return static_cast<short>(sys_info.dwNumberOfProcessors);
}

LPVOID memoryAlloc(const size_t& byte_value) {
    LPVOID memory = VirtualAlloc(NULL, byte_value, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    return memory ? memory : nullptr;
}

std::pair<LPVOID, LPVOID>* writeData(const std::string& data, const size_t& data_size, const size_t& free_memory, LPVOID next_free_byte) {
    if (data_size <= free_memory) {
        memcpy(next_free_byte, data.data(), data_size);

        LPVOID start_ptr = next_free_byte;
        next_free_byte = static_cast<char*>(next_free_byte) + data_size;

        return new std::pair(start_ptr, next_free_byte);
    }

    return nullptr;
}

