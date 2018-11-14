#include "util.h"

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
  std::stringstream ss;
  ss << sum << "-L" << count << extension;

  return ss.str();

}


// Gets the arguments from the command line as a string
std::string GetArguments(int argc, char* argv[]) {
  std::stringstream buf;
  for (int i = 0; i < argc; ++i) {
    buf << argv[i] << " ";
  }
  return buf.str();
}


// Returns the current manifest version as an integer
int get_current_version(std::string repo_root) {
  int version;
  std::fstream myFile;
  fs::path dir(repo_root);
  fs::path filename = ".latest_version.txt";
  fs::path latest(dir / filename);

  myFile.open(latest.string(), std::fstream::in);
  if (myFile.is_open()) {
    myFile >> version;
  }
  myFile.close();
  return version;
}


void update_version(std::string repo_root) {
    int version_value;
    std::fstream myFile;
    fs::path dir(repo_root);
    fs::path filename = ".latest_version.txt";
    fs::path latest(repo_root / filename);

    // base case, checks to see if file exists, creates it if not and puts value 1.
    myFile.open(latest.string());
    if (!myFile) {
        // std::cout << "'.latest_version.txt' file created.\n Version: 1\n";
        myFile.open(latest.string(), std::fstream::in | std::fstream::out | std::fstream::app);
        myFile << 1;
        return;
    }
    myFile.close();

    // stores current version, increments the version.
    myFile.open(latest.string(), std::fstream::in);
    if (myFile.is_open()) {
        myFile >> version_value;
    }
    myFile.close();
    version_value++;

    // clears contents of the file then writes the new_version int value in.
    myFile.open(latest.string(), std::fstream::out | std::fstream::trunc);
        if (myFile.is_open()) {
            myFile << version_value;
            // std::cout << "Version: " << version_value << "\n";
        }
    myFile.close();
}
