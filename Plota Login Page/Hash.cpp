#include "Hash.h"


#include <sstream>
#include <iomanip>
#include <stdexcept>

// namespace Crypto {
namespace SHA256 {
     namespace {
          const uint32_t K[64] = {
               0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
               0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
               0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
               0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
               0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
               0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
               0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
               0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
               0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
               0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
               0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
               0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
               0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
               0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
               0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
               0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
          };

          inline uint32_t rotr(uint32_t x, uint32_t n) {
               return (x >> n) | (x << (32 - n));
          }
        
          inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
               return (x & y) ^ (~x & z);
          }
        
          inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
               return (x & y) ^ (x & z) ^ (y & z);
          }
        
          inline uint32_t sigma0(uint32_t x) {
               return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
          }
        
          inline uint32_t sigma1(uint32_t x) {
               return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
          }
        
          inline uint32_t gamma0(uint32_t x) {
               return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
          }
        
          inline uint32_t gamma1(uint32_t x) {
               return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
          }
        
          void process_block(const uint8_t* block, uint32_t state[8]) {
               uint32_t W[64];
            
               for (int i = 0; i < 16; i++) {
                    W[i] = (block[i*4] << 24) | (block[i*4 + 1] << 16) | (block[i*4 + 2] << 8) | (block[i*4 + 3]);
               }
            
               for (int i = 16; i < 64; i++) {
                    W[i] = gamma1(W[i-2]) + W[i-7] + gamma0(W[i-15]) + W[i-16];
               }
            
               uint32_t a = state[0];
               uint32_t b = state[1];
               uint32_t c = state[2];
               uint32_t d = state[3];
               uint32_t e = state[4];
               uint32_t f = state[5];
               uint32_t g = state[6];
               uint32_t h = state[7];
            
               for (int i = 0; i < 64; i++) {
                    uint32_t temp1 = h + sigma1(e) + ch(e, f, g) + K[i] + W[i];
                    uint32_t temp2 = sigma0(a) + maj(a, b, c);
                    h = g;
                    g = f;
                    f = e;
                    e = d + temp1;
                    d = c;
                    c = b;
                    b = a;
                    a = temp1 + temp2;
               }
            
               state[0] += a;
               state[1] += b;
               state[2] += c;
               state[3] += d;
               state[4] += e;
               state[5] += f;
               state[6] += g;
               state[7] += h;
          }
        
          std::string state_to_hex(const uint32_t state[8]) {
               std::stringstream ss;
               ss << std::hex << std::setfill('0');
            
               for (int i = 0; i < 8; i++) {
                    ss << std::setw(2) << ((state[i] >> 24) & 0xFF);
                    ss << std::setw(2) << ((state[i] >> 16) & 0xFF);
                    ss << std::setw(2) << ((state[i] >> 8) & 0xFF);
                    ss << std::setw(2) << (state[i] & 0xFF);
               }
            
               return ss.str();
          }
        
          void sha256_internal(const uint8_t* data, size_t length, uint32_t state[8]) {
               state[0] = 0x6a09e667;
               state[1] = 0xbb67ae85;
               state[2] = 0x3c6ef372;
               state[3] = 0xa54ff53a;
               state[4] = 0x510e527f;
               state[5] = 0x9b05688c;
               state[6] = 0x1f83d9ab;
               state[7] = 0x5be0cd19;
            
               std::vector<uint8_t> message(data, data + length);
               uint64_t original_bit_len = length * 8;
            
               message.push_back(0x80);
            
               while ((message.size() % 64) != 56) {
                    message.push_back(0x00);
               }
            
               for (int i = 7; i >= 0; i--) {
                    message.push_back((original_bit_len >> (i * 8)) & 0xFF);
               }
            
               for (size_t i = 0; i < message.size(); i += 64) {
                    process_block(&message[i], state);
               }
          }
     }
    
     std::string hash(const std::string& input) {
          return hash(reinterpret_cast<const uint8_t*>(input.data()), input.size());
     }
    
     std::string hash(const uint8_t* data, size_t length) {
          uint32_t state[8];
          sha256_internal(data, length, state);
          return state_to_hex(state);
     }
    
     std::string hash(const char* input) {
          return hash(std::string(input));
     }
    
     std::string hash(const std::vector<uint8_t>& data) {
          return hash(data.data(), data.size());
     }
    
     bool verify(const std::string& input, const std::string& expected_hash) {
        return hash(input) == HashUtils::to_lowercase(expected_hash);
     }
    
     // bool verify(const uint8_t* data, size_t length, const std::string& expected_hash) {
     //    return hash(data, length) == HashUtils::to_lowercase(expected_hash);
     // }
    
}

namespace HashUtils {
    
     std::string to_lowercase(const std::string& hex) {
          std::string result = hex;
          for (char& c : result) {
               if (c >= 'A' && c <= 'F') {
                    c = c - 'A' + 'a';
               }
          }
          return result;
     }
    
     std::string to_uppercase(const std::string& hex) {
          std::string result = hex;
          for (char& c : result) {
               if (c >= 'a' && c <= 'f') {
                    c = c - 'a' + 'A';
               }
          }
          return result;
     }
    
     bool is_valid_hex(const std::string& hex, size_t expected_length) {
          if (hex.length() != expected_length) return false;
        
          for (char c : hex) {
               if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
                    return false;
               }
          }
          return true;
     }
    
     std::vector<uint8_t> hex_to_bytes(const std::string& hex) {
          if (!is_valid_hex(hex, hex.length())) {
               throw std::invalid_argument("Invalid hex string");
          }
        
          std::vector<uint8_t> bytes;
          bytes.reserve(hex.length() / 2);
        
          for (size_t i = 0; i < hex.length(); i += 2) {
               std::string byte_string = hex.substr(i, 2);
               bytes.push_back(static_cast<uint8_t>(std::stoul(byte_string, nullptr, 16)));
          }
        
          return bytes;
     }
    
     std::string bytes_to_hex(const std::vector<uint8_t>& bytes) {
          std::stringstream ss;
          ss << std::hex << std::setfill('0');
        
          for (uint8_t byte : bytes) {
               ss << std::setw(2) << static_cast<int>(byte);
          }
        
          return ss.str();
     }
    
}
