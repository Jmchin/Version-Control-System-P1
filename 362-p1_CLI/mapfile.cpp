#include "mapfile.h"

using namespace std;

namespace fs = boost::filesystem;

/* returns a manifest given a label, if exists*/
string getAliasIfExists(string name, string source) {
    ifstream myFile;
    stringstream pathtolabel;

    pathtolabel << fs::current_path().string()
                << "/"
                << source
                << "/"
                << ".labels";

    myFile.open(pathtolabel.str());

    bool found = false;
    string key;
    string value;

    while(!myFile.eof()) {

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
