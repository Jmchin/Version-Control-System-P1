#include "mapfile.h"

using namespace std;

namespace fs = boost::filesystem;

string getAliasIfExists(string name) {
    ifstream myFile;
    // todo: prefix repo_root

    // FIX THIS: needs qualified path "cwd/repo/.labels"

    stringstream pathtolabel;
    pathtolabel << fs::current_path().string() << "/" << "repo" << "/" << ".labels";

    myFile.open(pathtolabel.str());

    bool found = false;
    string key;
    string value;

    while(!myFile.eof()) {

      // std::cout << "LOOPING";

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
