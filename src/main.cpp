#include <iostream>
#include <string>

#include <boost/filesystem/operations.hpp>

using namespace std;
using namespace boost::filesystem;

void iterate(const path& parent);
void process(const path& path);

int main(int argc, char* argv[]) {
	iterate(".");
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
