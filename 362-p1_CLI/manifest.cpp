#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include "manifest.h"

namespace fs = boost::filesystem;

/* input: Manifest& m   - a manifest object to add label to
          String label - the label to append to m's LabelList

   output: New manifest m' - a copy of :Manifest m with :String label appended
                             to the tail of the LabelList

   NOTE: The return type is Manifest, meaning that this function returns a new
         Manifest object, seperate from the one passed in as argument. If you
         want to label a Manifest, you will need to assign the result of this
         function to some variable.
*/

/* input: Manifest& m - the manifest object to write out
          String path - the path to write the manifest object to

   output: None

   Side Effects: Writes the contents of Manifest m as a string to a file named
                 by the path argument.
*/
void WriteManifestToPath(const Manifest& m, std::string path) {
  std::ofstream file;

  file.open(path);
  file << ManifestToString(m);
  file.close();

}


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

std::string ManifestToString(const Manifest& m) {
  std::stringstream buf;
  buf << m.command << "\n";
  buf << m.timestamp << "\n";
  buf << m.user << "\n";
  // buf << mFilesToString(m);

  return buf.str();

}

// writes a line to an existing manifest file
void LogToManifest(std::string log, std::ofstream& manifest) {
  manifest << log << "\n";
}

// std::string mFilesToString(const Manifest& m) {
//   std::stringstream buf;

//   for (auto lines: m.files) {
//     for (auto file_artifact : lines) {
//       buf << file_artifact << " ";
//     }
//     buf << "\n";
//   }

//   return buf.str();
// }

// int main() {
//   // test timestamp
//   std::string ts = timestamp();
//   std::cout << "Timestamp Test: " << ts << std::endl;

//   FileList* files = new FileList;
//   std::string file_artifact1 = "home/jc/school/cpsc362/foo.cpp 13908.cpp";

//   files->push_back(file_artifact1);

//   std::cout << "Creating manifest object..." << std::endl;

//   Manifest manifest = { "create", timestamp(), "jc", *files };
//   std::cout << ManifestToString(manifest) << std::endl;

//   // test manifest labelling
//   std::cout << "Labeling manifest..." << std::endl;
//   std::cout << std::endl;

//   // show final representation of manifest after transformations
//   std::cout << "Manifest object after labeling:" << std::endl;
//   std::cout << ManifestToString(manifest) << std::endl;


//   std::cout << "Writing manifest to disk..." << std::endl;
//   WriteManifestToPath(manifest, "MANIFEST");

//   return 0;
// }
