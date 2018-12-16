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

std::vector<CreationObject> GetCreationOrder(std::string manifestFile) {
  std::vector<std::string> myLines = GetPaths(manifestFile);
  std::vector<std::string> parsed;
  std::vector<CreationObject> orderedObjects;
  std::set<CreationObject> createdObjs;


  std::string regx = "^";  // matches the beginning of a line
  regx.append(fs::current_path().string());
  std::regex pattern(regx);

  for(std::string s: myLines) {
    parsed.push_back(std::regex_replace(s, pattern, ""));
  }

  int maxSlashes = 0;
  for(std::string s: parsed) {
    int curItemCount = count(s.begin(), s.end(), '/');
    if (curItemCount > maxSlashes) {
      maxSlashes = curItemCount;
    }
  }

  for(int i = 1;i <= maxSlashes;i++) {
    for(std::string s: parsed) {
      int slashCount = count(s.begin(), s.end(), '/');
      if (slashCount == i) {
        std::vector<std::string> split = Split(s,"/");
        std::string newPath = "";
        for(int j = 0;j <i-1;j++) {
          newPath.append(split[j]);
          newPath.append("/");
        }

        newPath.append(split[slashCount-1]);
        CreationObject newCreationObj = CreationObject(s, newPath, false);
        if(createdObjs.count(newCreationObj) == 0) {
          orderedObjects.push_back(newCreationObj);
          createdObjs.insert(newCreationObj);
        }

      }
      else if (slashCount > i) {
        std::vector<std::string> split = Split(s, "/");
        std::string thisDeep = "";
        for(int j = 0; j < i;j++) {
          thisDeep.append(split[j]);
          thisDeep.append("/");
        }

        CreationObject newCreationObj = CreationObject(thisDeep, thisDeep, true);
        if(createdObjs.count(newCreationObj) == 0) {
          orderedObjects.push_back(newCreationObj);
          createdObjs.insert(newCreationObj);
        }
      }
      else {
        continue;
      }
    }
  }
  return orderedObjects;
}

std::vector<std::string> GetPaths(std::string manifestFile) {
  std::ifstream myFile;
  myFile.open(manifestFile);
  std::string line;
  std::vector<std::string> pathList;

  // Gross skipping of first 2 lines:
  getline(myFile, line);
  getline(myFile, line);


  while(getline(myFile, line)) {
    pathList.push_back(line);
  }

  myFile.close();
  return pathList;
}


std::vector<std::string> Split(std::string str, std::string sep) {
  char* cstr=const_cast<char*>(str.c_str());
  char* current;
  std::vector<std::string> arr;
  current=strtok(cstr,sep.c_str());
  while(current!=NULL){
    arr.push_back(current);
    current=strtok(NULL,sep.c_str());
  }
  return arr;
}

/* returns a manifest given a label, if exists*/
std::string getAliasIfExists(std::string name, std::string source) {
  std::ifstream myFile;
  std::stringstream pathtolabel;

    pathtolabel << fs::current_path().string()
                << "/"
                << source
                << "/"
                << ".labels";

    myFile.open(pathtolabel.str());

    bool found = false;
    std::string key;
    std::string value;

    while(!myFile.eof()) {

      getline(myFile, key);
      getline(myFile, value);

      if(key == name) {
        found = true;
        break;
      }

    }

    myFile.close();

    if(found) {
        return value;
    }
    return name;
}

/* Returns the most recent common ancestor of source and target */
std::string getCommonAncestor(std::vector<std::string> source, std::vector<std::string> target) {
  std::sort(source.begin(), source.end());
  std::sort(target.begin(), target.end());

  std::vector<std::string> intersection;

  std::set_intersection(source.begin(), source.end(), target.begin(), target.end(), std::back_inserter(intersection));

  return intersection.back();
}
