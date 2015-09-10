#include <string>
#include <iostream>

#include "../native-tear/tear.h"

using namespace std;
using namespace boost::filesystem;
using namespace CryptoPP;

crypt_data* get(string key, string iv);

int main(int argc, char* argv[]) {
	string hex_key = string(argv[1]);
	string hex_iv = string(argv[2]);

	if (hex_key.length() != KEY_LEN * 2) {
		cerr << "Hex key length must be " << (KEY_LEN * 2) << endl;
		exit(-1);
	} else if (hex_iv.length() != IV_LEN * 2) {
		cerr << "Hex IV length must be " << (IV_LEN * 2) << endl;
		exit(-1);
	}

	crypt_data* d = get(hex_key, hex_iv);

	return 0;
}

crypt_data* get(string key, string iv) {
	crypt_data* d = new crypt_data;

	HexDecoder decoder;
	decoder.Put((byte*) key.data(), key.size());
	decoder.MessageEnd();
	decoder.Get(d->key, sizeof(d->key));

	HexDecoder decoder1;
	decoder1.Put((byte*) key.data(), key.size());
	decoder1.MessageEnd();
	decoder1.Get(d->iv, sizeof(d->iv));

	return d;
}
