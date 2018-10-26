#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <fsream>
#include <boost/filesystem.hpp>

#include "manifest.h"

namespace fs = boost::filesystem;

std::string timestamp() {

  // get time object for the current localtime
  std::time_t t = std::time(nullptr);
  std::tm tm = *std::localtime(&t);

  // put_time outputs to a stream, so use a stringstream here
  std::stringstream buf;
  buf << std::put_time(&tm, "%F|%T");

  return buf.str();

}

/* input: Manifest m - a manifest object to add label to
          String label - the label to append to m's LabelList

   output: New manifest m' - a copy of :Manifest m with :String label appended
                             to the front of the LabelList

*/
Manifest LabelManifest(Manifest m, std::string label) {

}

void WriteManifestToDisk(Manifest m) {
  // get path of hidden file detailing latest manifest version (e.g ".441")
  std::string cur_manifest;

  // increment the filename read as an integer and rename
  // TODO: implement logic to rename file by incrementing the integer portion of the name
  try {

  fs::rename(cur_manifest, "");


  } catch (const fs::filesystem_error& ex) {
    // TODO: implement actual error handling

  }

}


int main() {
  std::string ts = timestamp();
  std::cout << ts << std::endl;

  return 0;
}
