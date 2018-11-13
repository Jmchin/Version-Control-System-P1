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



void update_version(std::string repo_root) {
    int version_value;
    std::fstream myFile;
    fs::path dir(repo_root);
    fs::path filename = ".latest_version.txt";
    fs::path latest(repo_root / filename);

    // base case, checks to see if file exists, creates it if not and puts value 1.
    myFile.open(latest.string());
    if (!myFile) {
        std::cout << "'.latest_version.txt' file created.\n Version: 1\n";
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
            std::cout << "Version: " << version_value << "\n";
        }
    myFile.close();
}
