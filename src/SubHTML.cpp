#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

std::vector<std::string> files;
std::vector<std::string> ignoredFiles;

std::string getFolderPath(std::experimental::filesystem::path filePath) {
	return filePath.string().substr(0, filePath.string().find(filePath.filename().string()));
}

std::string getFileNameWithoutExtension(std::experimental::filesystem::path filePath) {
	return filePath.filename().string().substr(0, filePath.filename().string().find(filePath.extension().string()));
}

void processFile(std::experimental::filesystem::path filePath) {
	std::cout << "Processing: " << filePath << std::endl;

	std::ifstream is, is2;
	std::ofstream os;

	std::string folderName = getFolderPath(filePath);
	std::string fileNameNoExtension = getFileNameWithoutExtension(filePath);

	is.open(filePath);
	if (is.is_open()) {

		os.open(folderName + fileNameNoExtension + ".html");
		if (os.is_open()) {
			std::string line;
			while (getline(is, line)) {
				if (line.length() > 2 && line.at(0) == '[' && line.at(line.length() - 1) == ']') {
					auto subFilePath = std::experimental::filesystem::path(line.substr(1, line.length() - 2));
					std::string subFileFolderName = getFolderPath(subFilePath);
					std::string subFileNameNoExtension = getFileNameWithoutExtension(subFilePath);

					processFile(folderName + subFilePath.string());
					is2.open(folderName + subFileFolderName + subFileNameNoExtension + ".html");
					if (is2.is_open()) {
						while (getline(is2, line)) {
								os << line << std::endl;
						}
						is2.close();
					} else {
							std::cout << "Error on opening file: " << folderName << subFileFolderName << subFileNameNoExtension << ".html" << std::endl;
					}
				} else {
					os << line << std::endl;
				}
			}
			os.close();
		} else {
			std::cout << "File path doesn't exist: " << folderName << fileNameNoExtension << ".html" << std::endl;
		}
		is.close();
	} else {
		std::cout << "Error on opening file: " << filePath.string() << std::endl;
	}
}

void includeFile(std::experimental::filesystem::path filePath) {
	if (filePath.extension().string() == ".subhtml") {
		files.push_back(filePath.string());
	} else {
		ignoredFiles.push_back(filePath.string());
	}
}

void includeFolder(std::experimental::filesystem::path folderPath) {
	std::experimental::filesystem::directory_iterator dirIter(folderPath);

	for (auto & fileIter : dirIter) {
		auto filePath = std::experimental::filesystem::path(fileIter);

		if (std::experimental::filesystem::is_directory(filePath)) {
			includeFolder(filePath);
		} else if (std::experimental::filesystem::is_regular_file(filePath)) {
			includeFile(filePath);
		}
	}
}

int main() {
	
	std::ifstream fileList("subhtml.txt");

	if (fileList.is_open()) {
		std::string line;
		while (getline(fileList, line)) {
			if (line == "" || line.at(0) == '#') {
				continue;
			}

			auto path = std::experimental::filesystem::path(line);
			
			if (std::experimental::filesystem::is_regular_file(path)) {
				includeFile(path);
			} else if (std::experimental::filesystem::is_directory(path)) {
				includeFolder(path);
			}
		}

		std::cout << ignoredFiles.size() << " ignored files:" << std::endl;
		for (std::string & filePath : ignoredFiles) {
			std::cout << filePath << std::endl;
		}

		std::cout << std::endl << "================================\n" << files.size() << " processed files:" << std::endl;
		for (std::size_t i = 0; i < files.size(); ++i) {
			processFile(std::experimental::filesystem::path(files.at(i)));
		}
		
		fileList.close();
	} else {
		std::cout << "No subhtml.txt file exist!" << std::endl;
	}	
	
	return 0;
}
