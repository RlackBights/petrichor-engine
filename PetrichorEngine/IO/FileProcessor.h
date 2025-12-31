#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace PetrichorEngine::IO {
    class FileProcessor
    {
    public:
        static std::string Read(std::string path); 
        static std::vector<uint8_t> ReadBytes(std::string path); 
        template <class T>
        static std::vector<T> ProcessBytes(std::vector<uint8_t> bytes)
        {
            const T* ptr = reinterpret_cast<const T*>(bytes.data());
            return std::vector<T>(ptr, ptr + bytes.size());
        }
    };
}