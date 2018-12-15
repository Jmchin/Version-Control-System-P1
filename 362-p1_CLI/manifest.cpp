#include "manifest.h"

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
std::vector<std::string> GetLinearHistory(std::string manifest) {
  // initialize history vector
  std::vector<std::string> history;

  // GetParent while not create manifest
  std::string child = manifest;
  while (child != "1.manifest") {
    std::string parent = GetParent(child);
    history.push_back(parent);
    child = parent;
  }

  return history;
}

std::string GetParent(std::string manifest) {
  // open the manifest file
  std::ifstream child(manifest);

  // get first line of manifest
  std::string line;
  std::getline(file, line);

  //TODO: if checkin manifest, get the project directory name (i.e last argument)
  std::string child_proj_dir;

  child.close();

  //TODO: get list of manifests less recent than the argument
  std::vector<std::string> manifests;

  //TODO: start from back of the list, find manifest direct parent
  std::string parent_proj_dir;

  while (parent_proj_dir != child_proj_dir) {
    // open the next potential parent
    std::ifstream parent(manifests.back());

    // read first line from parent
    std::string line;
    std::getline(parent, line);

    //TODO: get parent_proj_dir from line

    // close parent
    parent.close();

    // remove last element from manifests
    manifests.pop_back();
  }

  return parent;

}
