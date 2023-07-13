#include <iostream>
#include <filesystem>
#include <map>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace fs = std::filesystem;

void mkdir(fs::path path) {
	fs::create_directory(path);
}

void printdir(fs::path path) {
	if (fs::is_directory(path)) {
		for (auto file : (fs::directory_iterator)path) {
			std::cout << file.path().filename().string() << '\n';
		}
	}
	else {
		throw std::invalid_argument("not a directory\n");
	}
}

void rmdir(fs::path path) {
	if (fs::is_directory(path)) {
		fs::remove_all(path);
	}
	else {
		throw std::invalid_argument("not a directory\n");
	}
}

void mkfile(fs::path path) {
	FILE* in = fopen(path.string().c_str(), "w");
}

void printfile(fs::path path) {
	if (fs::is_regular_file(path)) {
		std::ifstream file(path);
		std::string line;
		while (!file.eof()) {
			file >> line;
			std::cout << line;
		}
	}
	else {
		throw std::invalid_argument("not a file\n");
	}
}

void rmfile(fs::path path) {
	if (fs::is_regular_file(path)) {
		fs::remove(path);
	}
	else {
		throw std::invalid_argument("not a file\n");
	}
}

void mkslink(fs::path path) {
	if (fs::is_regular_file(path)) {
		fs::path sLink(fs::current_path());
		sLink /= (path.filename().string() + "_sLink");
		int i = 1;
		std::string str;
		while (fs::exists(sLink)) {
			str = std::to_string(i);
			sLink.replace_filename(path.filename().string() + "_sLink" + str);
			++i;
		}
		fs::create_symlink(path, sLink);
	}
	else {
		throw std::invalid_argument("not a file\n");
	}
}

void printslink(fs::path link) {
	if (fs::is_symlink(link)) {
		fs::path path = fs::read_symlink(link);
		std::cout << path;
	}
	else {
		throw std::invalid_argument("not a symlink\n");
	}
}

void printslinkcontents(fs::path link) {
	if (fs::is_symlink(link)) {
		fs::path path = fs::read_symlink(link);
		printfile(path);
	}
	else {
		throw std::invalid_argument("not a symlink\n");
	}
}

void rmslink(fs::path link) {
	if (fs::is_symlink(link)) {
		fs::remove(link);
	}
	else {
		throw std::invalid_argument("not a symlink\n");
	}
}

void mkhlink(fs::path path) {
	if (fs::is_regular_file(path)) {
		fs::path hLink(fs::current_path());
		hLink /= (path.filename().string() + "_hLink");
		int i = 1;
		std::string str;
		while (fs::exists(hLink)) {
			str = std::to_string(i);
			hLink.replace_filename(path.filename().string() + "_hLink" + str);
			++i;
		}
		fs::create_hard_link(path, hLink);
	}
	else {
		throw std::invalid_argument("not a file\n");
	}
}

void printhlink(fs::path link) {
	printfile(link);
}

void rmhlink(fs::path path) {
	rmfile(path);
}

void printfilemode(fs::path path) {
	if (fs::is_regular_file(path)) {
		fs::file_status status = fs::status(path);
		fs::perms perms = status.permissions();
		auto show = [=](char op, fs::perms perm) {
			std::cout << (fs::perms::none == (perm & perms) ? '-' : op);
		};
		show('r', fs::perms::owner_read);
		show('w', fs::perms::owner_write);
		show('x', fs::perms::owner_exec);
		show('r', fs::perms::group_read);
		show('w', fs::perms::group_write);
		show('x', fs::perms::group_exec);
		show('r', fs::perms::others_read);
		show('w', fs::perms::others_write);
		show('x', fs::perms::others_exec);
	}
	else {
		throw std::invalid_argument("not a file\n");
	}
}

void chmode(fs::path path, std::string str) {
	if (str.size() != 9) {
		throw std::invalid_argument("wrong option arg\n");
	}
	auto comp = [=](char ch, char mode, fs::perms perm) {
		if (ch == mode) {
			fs::permissions(path, perm, fs::perm_options::add);
		}
		else {
			if (ch == '-') {
				fs::permissions(path, perm, fs::perm_options::remove);
			}
			else {
				throw std::invalid_argument("wrong permission options");
			}
		}
	};
	comp(str[0], 'r', fs::perms::owner_read);
	comp(str[1], 'w', fs::perms::owner_write);
	comp(str[2], 'x', fs::perms::owner_exec);
	comp(str[3], 'r', fs::perms::group_read);
	comp(str[4], 'w', fs::perms::group_write);
	comp(str[5], 'x', fs::perms::group_exec);
	comp(str[6], 'r', fs::perms::others_read);
	comp(str[7], 'w', fs::perms::others_write);
	comp(str[8], 'x', fs::perms::others_exec);
}

const static std::map < std::string, void(*)(fs::path)> options = { {
	{"dirmk", &mkdir},
	{"printdir", &printdir},
	{"dirrm", &rmdir},
	{"mkfile", &mkfile},
	{"printfile", &printfile},
	{"rmfile", &rmfile},
	{"mkslink", &mkslink},
	{"printslink", &printslink},
	{"printslinkcontents", &printslinkcontents},
	{"rmslink", &rmslink},
	{"mkhlink", &mkhlink},
	{"printhlink", &printhlink},
	{"rmhlink", &rmhlink}, 
	{"printfilemode", &printfilemode},
} };

void info() {
	std::cout << "myinfo\n";
	for (auto option: options) {
		std::cout << option.first << '\n';
	}
	std::cout << "modech\n";
}

int main(int argc, char* argv[]) {
	try {
		if (argc != 2) {
			if (argc == 1) {
				if (!strcmp(argv[0], "myinfo")) {
					info();
				}
				else {
					std::cout << "wrong option1\n";
				}
			}
			else {
				if (argc == 3) {
					if (!strcmp(argv[0], "modech")) {
						fs::path path(argv[1]);
						chmode(path, argv[2]);
					}
					else {
						std::cout << "wrong option2\n";
					}
				}
				else{
					std::cout << "wrong number of args\n";
				}
				
			}
			return 0;
		}

		fs::path path(argv[1]);
		bool isFound = false;
		for (auto option : options) {
			if (!strcmp(argv[0], option.first.c_str())) {
				option.second(path);
				isFound = true;
			}
		}
		if (isFound == false) {
			std::cout << argv[0] << '\n';
			throw std::invalid_argument("wrong option3\n");
		}
	}
	catch (fs::filesystem_error ex) {
		std::cout << ex.what();
	}
	catch (std::invalid_argument ex) {
		std::cout << ex.what();
	}
}
