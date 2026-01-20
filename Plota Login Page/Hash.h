#pragma once

#include <string>
#include <vector>
#include <cstdint>


namespace SHA256 {
     std::string hash(const std::string& input);
     std::string hash(const uint8_t* data, size_t length);
     std::string hash(const char* input);
     std::string hash(const std::vector<uint8_t>& data);
     bool verify(const std::string& input, const std::string& expected_hash);
     // bool verify(const uint8_t* data, size_t length, const std::string& expected_hash);
}
namespace HashUtils {
     std::string to_lowercase(const std::string& hex);
     std::string to_uppercase(const std::string& hex);
     bool is_valid_hex(const std::string& hex, size_t expected_length = 64);
     std::vector<uint8_t> hex_to_bytes(const std::string& hex);
     std::string bytes_to_hex(const std::vector<uint8_t>& bytes);
}