#include <string>
#include <iostream>

#include "../native-tear/tear.h"

using namespace std;
using namespace boost::filesystem;
using namespace CryptoPP;

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

	return 0;
}
