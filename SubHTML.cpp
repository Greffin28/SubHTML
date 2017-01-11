#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

std::string tDIR = "temp";
std::vector<std::string> files;


void processFile(std::string destPath, std::string filePath) {
	std::ifstream is, is2;
	std::ofstream os;
	is.open(filePath.c_str());
	if (is.is_open()) {
		os.open((destPath + '/' + filePath).c_str());

		std::string line;
		while (getline(is, line)) {
			if (line.at(0) == '[' && line.at(line.length() - 1) == ']') {
				line = line.substr(1, line.length() - 2);
				processFile(tDIR, line);
				is2.open((tDIR + '/' + line).c_str());
				if (is2.is_open()) {
					while (getline(is2, line)) {
						os << line << std::endl;
					}
					is2.close();
				} else {
					std::cout << "Error on opening file: " << line << std::endl;
				}
			} else {
				os << line << std::endl;
			}
		}

		is.close();
		os.close();
	} else {
		std::cout << "Error on opening file: " << filePath << std::endl;
	}
}

int main(int argc, const char * args[]) {
	std::string fDIR = ".Empty fDIR.";
	
	for (int i = 1; i < argc; ++i) {
		if (!strcmp(args[i], "-fDIR")) {
			fDIR = args[++i];
		} else {
			files.push_back(args[i]);
		}
	}
	
	if (fDIR != ".Empty fDIR.") {
		for (int i = 0; i < files.size(); ++i) {
			processFile(fDIR, files.at(i));
		}
	} else {
		std::cout << "Please specify the final directory with \"-fDIR\"" << std:: endl;
	}

	return 0;
}
