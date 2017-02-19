#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

std::vector<std::string> files;
std::string inFolder = "";

std::string folderPath(std::string &filePath) {
	int foundAt = filePath.rfind('/');
	if (foundAt >= filePath.length()) return "";
	else return filePath.substr(0, foundAt);
}

std::string fileNameE(std::string &filePath) {
	int folderLength = folderPath(filePath).length();
	if (folderLength > 0) return filePath.substr(folderLength + 1, filePath.length() - folderLength - 1);
	else return filePath;
}

std::string fileNameNE(std::string &filePath) {
	int folderLength = folderPath(filePath).length();
	if (folderLength > 0) return filePath.substr(folderLength + 1, filePath.rfind('.') - folderLength - 1);
	else return filePath.substr(0, filePath.rfind('.'));
}

void fixFolderFileName(std::string &folder, std::string &fileName) {
	if (folderPath(fileName).length() > 0) {
		std::string separator = "";
		if (folder.length() > 0) separator = "/";

		folder += separator + folderPath(fileName);
		fileName = fileNameE(fileName);
	}
}

void processFile(std::string folder, std::string fileName, std::string postfix) {
	std::ifstream is, is2;
	std::ofstream os;

	fixFolderFileName(folder, fileName);

	std::string separator = "";
	if (folder.length() > 0) separator = "/";

	is.open((folder + separator + fileName).c_str());
	if (is.is_open()) {
		os.open((folder + separator + fileNameNE(fileName) + postfix).c_str());
		if (os.is_open()) {
			std::string line;
			while (getline(is, line)) {
				if (line.length() > 2 && line.at(0) == '[' && line.at(line.length() - 1) == ']') {
					line = line.substr(1, line.length() - 2);

					std::string relativeFolder = folder;

					fixFolderFileName(relativeFolder, line);

					std::string separator = "";
					if (relativeFolder.length() > 0) separator = "/";

					processFile(relativeFolder, line, ".temp");
					is2.open((relativeFolder + separator + fileNameNE(line) + ".temp").c_str());
					if (is2.is_open()) {
						while (getline(is2, line)) {
								os << line << std::endl;
						}
						is2.close();
					} else {
							std::cout << "Error on opening file: " << relativeFolder + separator + fileNameNE(line) + ".temp" << std::endl;
					}
				} else {
					os << line << std::endl;
				}
			}
			os.close();
		} else {
			std::cout << "File path doesn't exist: " << folder + separator + fileName + postfix << std::endl;
		}
		is.close();
	} else {
		std::cout << "Error on opening file: " << folder + separator + fileName << std::endl;
	}
}

int main() {
	std::ifstream fileList("subhtml.txt");

	if (fileList.is_open()) {
		std::string line;
		while (getline(fileList, line)) {
			if (line == "" || line.at(0) == '#') {
				// Do Nothing
			} else if (line != "" && line.at(0) == '>') {
				inFolder = line.substr(1, line.length() - 1);
			} else {
				files.push_back(inFolder + line);
			}
		}

		for (int i = 0; i < files.size(); ++i) {
			processFile(folderPath(files.at(i)), fileNameE(files.at(i)), ".html");
		}
		
		fileList.close();
	} else {
		std::cout << "No subhtml.txt file exist!" << std::endl;
	}	

	return 0;
}
