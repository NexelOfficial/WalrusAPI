#pragma once

#include <string>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <iomanip>
#include <sstream>

namespace Walrus
{
class Checksum
{
public:
    static std::string generate(const std::string body, const std::string salt);
private:
    static unsigned char *getMd5Hash(const unsigned char *data, const size_t dataLen, int *mdLen);
    static std::string hexToString(const std::string hex);
    static std::string bytesToHex(const unsigned char *data, const int len);
};
}