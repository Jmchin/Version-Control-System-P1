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

using namespace std;

// the current working directory where program starts
// #define PATH_PREFIX fs::initial_path()

void PrintUsage() {
  cout << "Usage: cli_vcs command [<arguments>...]" << endl;
  cout << "\t cli_vcs create <project> <repository>" << endl;
  cout << "\t cli_vcs checkin <project> <repository>" << endl;
  cout << "\t cli_vcs checkout <manifest> <directory>" << endl;
  cout << "\t cli_vcs label <manifest> <label>" << endl;
}

// TODO: Implement actual error handling and argument requirements
int main(int argc, char* argv[]) {

  if (argc < 2) {
    PrintUsage();
    return 1;
  }

  string command = argv[1];

  // REFACTOR
  if(command == "create") {

    if (argc != 4) {
      cerr << "Invalid number of arguments for create repo." << endl;
      return -1;
    }

    string source = argv[2];
    string destination = argv[3];
    string commands = GetArguments(argc, argv);

    // TODO: Do we need to pass the manifest file stream to create?
    Create(source, destination, commands);


  }
  else if(command == "checkin") {
    string source = argv[2];
    string destination = argv[3];
    string commands = GetArguments(argc, argv);

    CheckIn(source, destination, commands);
  }
  else if(command == "checkout") {
    string manifest = argv[2];
    string destination = argv[3];
    string commands = GetArguments(argc, argv);

    CheckOut(manifest, destination);
  }
  else {
    PrintUsage();
  }
  return 0;
}
