#include "Checksum.hpp"

using namespace Walrus;

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
                          };

std::string Checksum::hexToString(const std::string hex)
{
    const int len = hex.length();
    std::string newString;
    for (int i = 0; i < len; i += 2)
    {
        const std::string byte = hex.substr(i, 2);
        const char chr = (char)(int)strtol(byte.c_str(), nullptr, 16);
        newString.push_back(chr);
    }

    return newString;
}

std::string Checksum::bytesToHex(const unsigned char *data, const int len)
{
    std::string s(len * 2, ' ');
    for (int i = 0; i < len; ++i)
    {
        s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}

std::string Checksum::generate(const std::string body, const std::string salt)
{
    // Convert hex to utf8 and create buffer to hold everything
    const std::string saltBin = hexToString(salt);
    unsigned char* buffer = new unsigned char[body.length() + saltBin.length()];

    // Copy body and saltBin into the buffer
    memcpy(buffer, body.data(), body.length());
    memcpy(buffer + body.length(), saltBin.data(), saltBin.length());

    // Get MD5 hash
    unsigned char digest[MD5_DIGEST_LENGTH];
    unsigned char *hash = MD5(buffer, body.length() + saltBin.length(), &(*digest));

    // Convert the bytes to a hex string
    delete[] buffer;
    return bytesToHex(hash, sizeof(digest));
}