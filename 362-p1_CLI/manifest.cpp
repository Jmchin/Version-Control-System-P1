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
