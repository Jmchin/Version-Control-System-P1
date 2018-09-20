// 362 Software Engineering Project 1
// Authors:
//     Kevin Molina-Heineck - kevin.molina@csu.fullerton.edu
//     Justin Chin - jmchin@csu.fullerton.edu
//     @@Add your names here@@
//     @@@@@@@@@@@@@@@@@@@@@@@
//
// Description:
//     TODO

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>

#define WEIGHT_ONE 1
#define WEIGHT_TWO 3
#define WEIGHT_THREE 7
#define WEIGHT_FOUR 11
#define WEIGHT_FIVE 17
// #define INT_MAX 2147483647

namespace fs = boost::filesystem;

std::string CheckSum(std::string fileName);
void Create();
void DeepCopyDir(fs::path src, fs::path des);
std::string FormatFileName(long long sum, int count, std::string extension);
void FolderifyLeaf(std::string filePath);

int main() {
    bool f = true;
    char input;

    do {
        std::cout << "******** Team CLI's VCS System ********" << std::endl;
        std::cout << std::endl;
        std::cout << "The following commands are available:" << std::endl;
        std::cout << std::endl;
        std::cout << "C - Create a repository for the given project source tree" << std::endl;
        std::cout << "and all its files, including their folder paths within the project" << std::endl;
        std::cout << std::endl;
        std::cout << "Q - Quit the program" << std::endl << std::endl;

        std::cin >> input;

        switch(toupper(input)){
            case 'C':
                Create();
                break;
            default:
                break;
        }

    } while (toupper(input) != 'Q');

    return 0;
}

// recursively copies file-directory structure
void DeepCopyDir(fs::path src, fs::path des) {

  // TODO: Proper error handling and checks
  if (!fs::exists(src) || !fs::is_directory(src)) {
    exit(-1);
  }

  for (auto& file : fs::recursive_directory_iterator(src)) {
    auto& file_path = file.path();
    auto relative_path = file_path.string();
    boost::replace_first(relative_path, src.string(), "");
    fs::copy(file_path, des / relative_path);
  }
}

void Create() {
    std::string source;
    std::string destination;

    std::cout << "Enter source folder path: ";
    std::cin >> source;
    std::cout << "Enter destination folder path: ";
    std::cin >> destination;

    fs::path src(source.c_str());
    fs::path des(destination.c_str());

    // copy the source directory
    DeepCopyDir(src, des);

    // Does it all in directory po.
    for(fs::directory_entry& p: fs::recursive_directory_iterator(des)) {
      if(fs::is_regular_file(p.path())) {
          std::cout << "folderifying: " << p.path().string() << std::endl;
          FolderifyLeaf(p.path().string());
      }
    }

    // ADD - Copy directory S into destination directory. Should literally be a call to
    // Boost's copy_file or copy_directory or copy_path (or something)
    // Look at my code/look into the path file type

}

// ****** CheckSum ******
//
// DESCRIPTION:
// Creates a rolling 5-byte weighted checksum of all the
// characters (ASCII bytes) in the file (i.e., the bytes
// in the file's contents) followed by a hyphen and an
// “L” and the integer file size, followed by the file's
// extension. The 5 weights by which each 5 character
// group are multiplied are 1, 3, 7, 11, and, 17
//
// INPUT: File name
// OUTPUT: string representing checksum to be used as filename
std::string CheckSum(std::string fileName) {

  std::ifstream myFile(fileName.c_str());

  if(myFile.fail()) {
    // Don't have to handle this
    // Should probably moduralize this too
    std::cout << "Error opening file";
  }
  else {
    char ch;
    int count = 0;
    long long sum = 0;

    while(EOF != (ch = myFile.get())) {
      switch(count % 5) {
      case 0:
        sum += WEIGHT_ONE * ch;
        sum = sum % INT_MAX;
        break;
      case 1:
        sum += WEIGHT_TWO * ch;
        sum = sum % INT_MAX;
        break;
      case 2:
        sum += WEIGHT_THREE * ch;
        sum = sum % INT_MAX;
        break;
      case 3:
        sum += WEIGHT_FOUR * ch;
        sum = sum % INT_MAX;
        break;
      case 4:
        sum += WEIGHT_FIVE * ch;
        sum = sum % INT_MAX;
        break;
      default:
        break;
      }
      count++;
    }

    myFile.close();
    fs::path myPath(fileName.c_str());
    std::string ext = myPath.extension().string();
    return FormatFileName(sum, count, ext);
  }

  exit(-1);

}


// Convience method to format the filenames.
// Pass in sum, count, and extension <e.g. '.txt'>
std::string FormatFileName(long long sum, int count, std::string extension) {
  std::string output;
  std::stringstream ss;
  ss << sum << "-L" << count << extension;

  return ss.str();

}

// Description:
//     Converts a 'leaf' file into a folder w/ 'Checksum' naming
//     Then moves the old file into the new folder, handling cleanup
//
// INPUT: Fully qualified path to file (e.g. /home/kevin/Desktop/foo.cpp)
// OUTPUT: None
void FolderifyLeaf(std::string filePath) {
  fs::path p(filePath);

  try {
    if(fs::exists(p)) {
      // This is a leaf file!
      if(fs::is_regular_file(p)) {
        // TODO - Handle case where folder exists with
        // Name from CheckSum output already (weird edge case)
        // Never gunna happen, but should be handled eventually
        std::string checkSum = CheckSum(filePath);

        // Get the parent path/dir from qualified path to file
        fs::path newFilePath = p.parent_path();
        newFilePath += "/" + checkSum;

        // Rename file with checksum
        // Check for rename funct in boost
        fs::copy_file(p, newFilePath);
        remove(p);
        fs::create_directory(p);

        fs::path finalDest = p.string() + "/" + checkSum;
        fs::copy_file(newFilePath, finalDest);
        fs::remove(newFilePath);
        // Append / + filename to new path (Hacky)
        // Dont need?
        //newFilePath += "/" + p.filename().string();
        // TODO - Remove old file

      }
      else {
        // Use qualified path instead? Haven't tested p
        std::cout << p << "exists, but is not a leaf file";
      }
    }
    else {
      std::cout << p << "Doesn't exist";
    }
  }
  catch (const fs::filesystem_error& ex) {
    std::cout << ex.what() << std::endl;
  }

  return;
}
