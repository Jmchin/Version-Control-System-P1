// 362 Software Engineering Project 1
// Authors:
//     Kevin Molina-Heineck - kevin.molina@csu.fullerton.edu
//     Justin Chin - jmchin@csu.fullerton.edu
//     James Paul Mabe - jamespmabe@csu.fullerton.edu
//     Alex Lavrov - alavrov@csu.fullerton.edu
//
//
// Description:
// Creates a repository storing artifactIDs for every
// non-directory file in a given project tree

#include "manifest.h"
#include "util.h"
#include "repo.h"

// TODO: Implement actual error handling and argument requirements
int main(int argc, char* argv[]) {

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    // REFACTOR
    if(arg == "-h" || arg == "--help") {
      std::cout << "Help not implemented." << std::endl;
    }
    else if(arg == "-c" || arg == "--create") {
      if (argc < 4) {
        std::cerr << "Invalid number of arguments for create repo." << std::endl;
        return -1;
      }
      std::string source = argv[2];
      std::string destination = argv[3];

      // create the initial manifest file
      std::ofstream manifest("MANIFEST");
      std::string commands = GetArguments(argc, argv);
      char* username = getlogin();

      // TODO: Bundle this together to InitializeManifest()
      LogToManifest(commands, manifest);
      LogToManifest(timestamp(), manifest);
      LogToManifest(username, manifest);

      // TODO: Do we need to pass the manifest file stream to create?
      Create(source, destination);

      manifest.close();

    }
    else if(arg == "-i" || arg == "--check-in") {
      std::string source = argv[2];
      std::string destination = argv[3];

      CheckIn(source, destination);
    }
    else if(arg == "-o" || arg == "--check-out") {
      std::string manifest = argv[2];
      std::string destination = argv[3];

      CheckOut(manifest, destination);
    }
  }
  return 0;
}
