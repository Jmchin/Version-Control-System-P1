#include "manifest.h"
#include <iostream>

/* ============================================================================ */
/*                              Auxililary Functions                            */
/* ============================================================================ */

/* Current localtime formatted YYYY-MM-DD|HH:MM:SS */
std::string timestamp() {

  // get time object for the current localtime
  std::time_t t = std::time(nullptr);
  std::tm tm = *std::localtime(&t);

  // put_time outputs to a stream, so use a stringstream here
  std::stringstream buf;
  buf << std::put_time(&tm, "%F|%T");

  return buf.str();

}

/* writes a line to an existing manifest file */
void LogToManifest(std::string log, std::ofstream& manifest) {
  manifest << log << "\n";
}

/* Returns a vector of a manifest's history upto the initial repo creation */
std::vector<std::string> GetLinearHistory(std::string manifest, std::string repo) {
  // initialize history vector
  std::vector<std::string> history;

  // std::cout << "getlinearhistory: " << manifest << std::endl;
  // GetParent while not create manifest
  // TODO: read only the manifest file name from the manifest
  std::string child = "";
  for (int i = manifest.length() - 1;i >= 0;i--) {
    if (manifest[i] == '/') {
      break;
    }
    else {
      child = manifest[i] + child;
    }
  }

  std::string childPath = manifest;

  while (child != "1.manifest") {

    std::string parent = GetParent(childPath);
    history.push_back(parent);
    childPath = parent;

    child = "";
    for (int i = childPath.length() - 1;i >= 0;i--) {
      if (manifest[i] == '/') {
        break;
        }
        else {
            child = childPath[i] + child;
        }
     }
  }

  return history;
}

std::string GetParent(std::string manifest) {
  // open the manifest file
  std::ifstream child(manifest);

  // get first line of manifest
  std::string line;
  std::getline(child, line);

  // Parse manifest line 1 into vector of strs using SS
  std::string str;
  char DELIMITER = ' ';
  std::stringstream lineOne(line);
  std::vector<std::string> lineOneVector;
  while (std::getline(lineOne, str, DELIMITER)) {
      lineOneVector.push_back(str);
  }

  // Grab current manifest number for use later
  // Done through SS parsing off a delimiter
  int MANIFEST_NUMBER;
  char MANIFEST_DELIM = '.';
  std::stringstream manifestNumSS(manifest);
  std::vector<std::string> manifestSSVector;
  std::getline(manifestNumSS, str, MANIFEST_DELIM);
  manifestSSVector.push_back(str);
  std::string manifestPath = manifestSSVector[0];
  std::string manNum = "";
  for (int i = manifestPath.length() - 1;i >= 0;i--) {
      if (manifestPath[i] == '/') {
          break;
      }
      else {
          manNum = manifestPath[i] + manNum;
      }
  }

  MANIFEST_NUMBER = std::stoi(manNum);

  // Was called on root create
  if (lineOneVector[1] == "create") {
      return manifest;
  }
  else if (lineOneVector[1] == "checkout") {
      std::string cwd = boost::filesystem::current_path().string();
      return cwd + "/" + lineOneVector[2] + "/" + lineOneVector[3];
  }

  else if (lineOneVector[1] == "checkin") {


    std::string parent_proj = lineOneVector[2];
    // TODO: if checkin manifest, get the project directory name (i.e last argument)
    std::string child_proj_dir = lineOneVector[3];
    child.close();

    // TODO: get list of manifests less recent than the argument
    std::vector<std::string> manifests;
    for(auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(child_proj_dir), {})) {
        if (boost::filesystem::is_regular_file(entry)) {
            std::string str = entry.path().string();
            int subLen = child_proj_dir.length();
            int endLen = str.length() - subLen;
            std::string manifestFile = str.substr(subLen, endLen);
            if (manifestFile.find("manifest") != std::string::npos) {
                manifests.push_back(manifestFile);
            }

        }
    }

    std::unordered_map<int, std::string> mapManifests;
    std::vector<int> manifestNums;

    // Split manifests apart associating #:qualified path
    // Also storing just number for easier searching later
    for (size_t i = 0; i < manifests.size(); i++) {
        std::string str;
        std::stringstream manifestFile(manifests[i]);
        std::vector<std::string> manifestVector;
        std::getline(manifestFile, str, MANIFEST_DELIM);
            manifestVector.push_back(str);
              std::string manNumStr = "";
              std::string manifestSlice = manifestVector[0];
              for (int i =manifestSlice.length() - 1;i >= 0;i--) {
              if (manifestSlice[i] == '/') {
                     break;
                 }
                 else {
                     manNumStr = manifestSlice[i] + manNumStr;
                 }
              }

              int manNum = std::stoi(manNumStr);

            if(manNum < MANIFEST_NUMBER) {
                mapManifests[manNum] = manifests[i];
                manifestNums.push_back(manNum);
            }

    }

    std::vector<std::string> sortedManifests;

    // Sort number of manifest files to make traversal easier
    std::sort(manifestNums.begin(), manifestNums.end());


    // Now sort the manifest files but with fully qualified path
    for (size_t i = 0;i < manifestNums.size(); i++) {
        std::string qualifiedManifest = (child_proj_dir + mapManifests[manifestNums[i]]);
        sortedManifests.push_back(qualifiedManifest);
    }


    // Start from back of the list, find manifest direct parent
    // Spit back parent manifest (Walk up the tree!!)
    std::string checkOutItem = "";
    while (parent_proj != checkOutItem) {
        // open the next potential parent
        std::string currentManifest = sortedManifests.back();
        std::ifstream parent(currentManifest);

        // read first line from parent
        std::string line;
        std::getline(parent, line);
        std::string str;
        std::stringstream manifestLineOne(line);
        std::vector<std::string> manifestLineOneVector;
        while (std::getline(manifestLineOne, str, DELIMITER)) {
        manifestLineOneVector.push_back(str);
        }
        // We're currently seaching for checkin parent,
        // which is a checkout. so find the checkout's last item if it matches
        if (manifestLineOneVector[1] == "checkout" && manifestLineOneVector[4] == parent_proj) {
          checkOutItem = currentManifest;
          break;
        }
        parent.close();
        sortedManifests.pop_back();  }

  std::string cwd = boost::filesystem::current_path().string() + "/" +  checkOutItem;

  return cwd;

  }
  return "";
}
