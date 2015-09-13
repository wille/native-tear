#include <string>
#include <iostream>

#include "../native-tear/tear.h"

using namespace std;
using namespace boost::filesystem;
using namespace CryptoPP;

crypt_data* get(string key, string iv);
void iterate(crypt_data* d, const path& parent);
void process(crypt_data* d, const path& parent);
void decrypt(crypt_data* d, const path& file);

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

	iterate(d, ".");

	return 0;
}

crypt_data* get(string key, string iv) {
	crypt_data* d = new crypt_data;

	HexDecoder decoder;
	decoder.Put((byte*) key.data(), key.size());
	decoder.MessageEnd();
	decoder.Get(d->key, sizeof(d->key));

	HexDecoder decoder1;
	decoder1.Put((byte*) iv.data(), iv.size());
	decoder1.MessageEnd();
	decoder1.Get(d->iv, sizeof(d->iv));

	return d;
}

void iterate(crypt_data* d, const path& parent) {
	string path;
	directory_iterator end_itr;

	for (directory_iterator itr(parent); itr != end_itr; ++itr) {
		path = itr->path().string();

		if (is_directory(itr->status()) && !symbolic_link_exists(itr->path())) {
			iterate(d, path);
		} else {
			process(d, path);
		}
	}
}

void process(crypt_data* d, const path& path) {
#ifdef DEBUG
	cout << "Iterating " << path.string() << endl;
#endif

	if (path.extension() == LOCKED_EXTENSION) {
		decrypt(d, path);
	}
}

void decrypt(crypt_data* d, const path& file) {
#ifdef DEBUG
	cout << "Decrypting " << file.string() << endl;
#endif

	string cipher;
	FileSource(file.string().c_str(), true, new StringSink(cipher));

	string decrypted_name;

#ifdef DEBUG
	decrypted_name = file.string() + "_decrypted";
#else
	decrypted_name = file.string().substr(0, file.string().length() - strlen(LOCKED_EXTENSION));
#endif

	string decrypted;
	CBC_Mode<AES>::Decryption dk;
	dk.SetKeyWithIV(d->key, sizeof(d->key), d->iv);

	StringSource ss(cipher, true, new StreamTransformationFilter(dk, new StringSink(decrypted)));

	//StringSource ss(cipher, true, new StreamTransformationFilter(dk, new FileSink(decrypted_name.c_str())));

	cout << "Decrypted text: " << decrypted << endl;
}
