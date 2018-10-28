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

/* input: Manifest m   - a manifest object to add label to
          String label - the label to append to m's LabelList

   output: New manifest m' - a copy of :Manifest m with :String label appended
                             to the front of the LabelList

   NOTE: The return type is Manifest, meaning that this function returns a new
         Manifest object, seperate from the one passed in as argument. If you
         want to label a Manifest, you will need to assign the result of this
         function to some variable.
*/
Manifest LabelManifest(Manifest m, std::string label) {
  LabelList labels = m.labels;
  labels.push_back(label);
  Manifest manifest = { m.command, m.timestamp, m.user, labels, m.files };

  return manifest;

}

void PrintManifest(Manifest& m) {
  std::cout << m.command << std::endl;
  std::cout << m.timestamp << std::endl;
  std::cout << m.user << std::endl;
  PrintLabels(m);
  std::cout << std::endl;
  PrintFiles(m);
}

/* Helper function to test LabelManifest */
void PrintLabels(Manifest& m) {
  for (auto label : m.labels) {
    std::cout << label << " ";
  }
}

void PrintFiles(Manifest& m) {
  for (auto lines : m.files) {
    for (auto file_artifact : lines) {
      std::cout << file_artifact << " ";

    }
    std::cout << std::endl;
  }

}

void WriteManifestToPath(Manifest m, std::string path) {

}

int main() {
  // test timestamp
  std::string ts = timestamp();
  std::cout << "Timestamp Test: " << ts << std::endl;

  // test manifest object creation and display representation
  LabelList labels;
  labels.push_back("foo");

  FileList files;
  std::vector<std::string> file_artifact1;
  file_artifact1.push_back("foo.cpp");
  file_artifact1.push_back("13850.cpp");

  files.push_back(file_artifact1);


  std::cout << "Creating manifest object..." << std::endl;

  Manifest manifest = { "create", timestamp(), "jc", labels, files };
  PrintManifest(manifest);

  std::cout << std::endl << std::endl;

  // test manifest labelling
  std::cout << "Previous manifest labels: ";
  PrintLabels(manifest);
  std::cout << std::endl;

  std::cout << "Labeling manifest..." << std::endl;

  manifest = LabelManifest(manifest, "bar");
  std::cout << "After labeling: ";
  PrintLabels(manifest);
  std::cout << std::endl << std::endl;

  // show final representation of manifest after transformations
  std::cout << "Manifest object after labelling:" << std::endl;
  PrintManifest(manifest);
  std::cout << std::endl;

  return 0;
}
