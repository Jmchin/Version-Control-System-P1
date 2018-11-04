#include "manifest.h"

namespace fs = boost::filesystem;

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

  return buf.str();

}

/* writes a line to an existing manifest file */
void LogToManifest(std::string log, std::ofstream& manifest) {
  manifest << log << "\n";
}
