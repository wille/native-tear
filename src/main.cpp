#include <iostream>
#include <string>

#include <boost/filesystem/operations.hpp>

#ifdef _WIN32
#	include <windows.h>
#	include <Lmcons.h>
#elif defined(__linux__)
#	include <pwd.h>
#endif

using namespace std;
using namespace boost::filesystem;

void iterate(const path& parent);
void process(const path& path);
string getid();

int main(int argc, char* argv[]) {
	iterate(".");

	cout << "Computer ID: " << getid() << endl;
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

	return to_string(uid);
#endif
}
