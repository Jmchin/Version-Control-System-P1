// 362 Software Engineering Project 1
// Authors:
//     Kevin Molina-Heineck - kevin.molina@csu.fullerton.edu
//     Justin Chin - jmchin@csu.fullerton.edu
//     James Paul Mabe - jamespmabe@csu.fullerton.edu
//     Alex Lavrov - alavrov@csu.fullerton.edu
//
//
// Description:
// Creates a repository storing artifactIDs for every
// non-directory file in a given project tree


#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

#include "manifest.h"

#define WEIGHT_ONE 1
#define WEIGHT_TWO 3
#define WEIGHT_THREE 7
#define WEIGHT_FOUR 11
#define WEIGHT_FIVE 17
#define INT_MAX 2147483647

namespace fs = boost::filesystem;

std::string CheckSum(std::string fileName);
void Create(std::string source, std::string destination);
void CheckIn(std::string source, std::string destination);
void DeepCopyDir(fs::path src, fs::path des);
std::string FormatFileName(long long sum, int count, std::string extension);
void FolderifyLeaf(std::string filePath);


// TODO: Generalize this
// void printUsage() {
//   std::cerr << "Usage: "
//             << "vcs"
//             << " -c"
//             << " <source directory>"
//             << " <target directory>"
//             << std::endl;
// }


// TODO: Implement actual error handling and argument requirements
int main(int argc, char* argv[]) {

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if(arg == "-h" || arg == "--help") {
      std::cout << "Help not implemented." << std::endl;
    }
    else if(arg == "-c" || arg == "--create") {
      if (argc < 4) {
        std::cerr << "Invalid number of arguments for create repo." << std::endl;
        return -1;
      }
      std::string source = argv[2];
      std::string destination = argv[3];

      Create(source, destination);

    }
    else if(arg == "-i" || arg == "--check-in") {
      std::string source = argv[2];
      std::string destination = argv[3];

      // Check project directory into a repository, adding new
      // artifactIDs into the appropriate directories

      // TODO: Create a Check-In manifest file
      CheckIn(source, destination);
      std::cout << "Checked-In project..." << std::endl;
      // std::cout << "Not implemented." << std::endl;
    }
    else if(arg == "-o" || arg == "--check-out") {
      std::cout << "Not implemented." << std::endl;
    }
  }
  return 0;
}

void Create(std::string source, std::string destination) {

  fs::path src(source);
  fs::path des(destination);

  // make a copy of the source directory
  DeepCopyDir(src, des);

  // folderify every leaf in the newly copied repo directory
  for(fs::directory_entry& p: fs::recursive_directory_iterator(des)) {
    if(fs::is_regular_file(p.path())) {
      FolderifyLeaf(p.path().string());
    }
  }

  char* username = getlogin();  // NOTE: Not portable, this assumes POSIX functions

// TODO: Build a manifest object, call WriteManifestToPath

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

// recursively copies file-directory structure
void DeepCopyDir(fs::path src, fs::path des) {

  // TODO: Proper error handling and checks
  if (!fs::exists(src) || !fs::is_directory(src)) {
    exit(-1);
  }

  // hacky way to make sure we can continue, even if user didn't clean directory
  if (!fs::exists(des)) {
    fs::create_directory(des);
  }

  for (auto& file : fs::recursive_directory_iterator(src)) {
    auto& file_path = file.path();
    auto relative_path = file_path.string();
    boost::replace_first(relative_path, src.string(), "");
    fs::copy(file_path, des / relative_path);
  }
}

// Convience method to format the filenames.
// Pass in sum, count, and extension <e.g. '.txt'>
std::string FormatFileName(long long sum, int count, std::string extension) {
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
        std::string checkSum = CheckSum(filePath);
        fs::path newFilePath = p.parent_path();     // Get the parent path/dir from filePath
        newFilePath += "/" + checkSum;


        fs::copy_file(p, newFilePath);     // Rename file with checksum
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
    // TODO: we don't actually need to handle this error do we?
    //       this is the behavior we want, unless there's some
    //       unforeseen consequences

    // std::cout << ex.what() << std::endl;
  }

  return;
}
