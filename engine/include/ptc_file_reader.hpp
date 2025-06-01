#ifndef PTC_FILE_READER_HPP
#define PTC_FILE_READER_HPP

#include "glm/fwd.hpp"
#include <cstddef>
#include <string>
#include <vector>
class FileReader
{
public:
    static std::string Read(std::string path); 
    static std::vector<glm::uint8_t> ReadBytes(std::string path); 
    template <class T>
    static std::vector<T> ProcessBytes(std::vector<glm::uint8_t> bytes)
    {
        const T* ptr = reinterpret_cast<const T*>(bytes.data());
        return std::vector<T>(ptr, ptr + bytes.size());
    }
};

#endif