#include "creationOrder.h"

using namespace std;
namespace fs = boost::filesystem;

vector<CreationObject> GetCreationOrder(string manifestFile) {
  vector<string> myLines = GetPaths(manifestFile);
  vector<string> parsed;
  vector<CreationObject> orderedObjects;
  set<CreationObject> createdObjs;


  string regx = "^";  // matches the beginning of a line
  regx.append(fs::current_path().string());
  regex pattern(regx);

  for(string s: myLines) {
    parsed.push_back(regex_replace(s, pattern, ""));
  }

  int maxSlashes = 0;
  for(string s: parsed) {
    int curItemCount = count(s.begin(), s.end(), '/');
    if (curItemCount > maxSlashes) {
      maxSlashes = curItemCount;
    }
  }

  for(int i = 1;i <= maxSlashes;i++) {
    for(string s: parsed) {
      int slashCount = count(s.begin(), s.end(), '/');
      if (slashCount == i) {
        vector<string> split = Split(s,"/");
        string newPath = "";
        for(int j = 0;j <i-1;j++) {
          newPath.append(split[j]);
          newPath.append("/");
        }

        newPath.append(split[slashCount-1]);
        CreationObject newCreationObj = CreationObject(s, newPath, false);
        if(createdObjs.count(newCreationObj) == 0) {
          orderedObjects.push_back(newCreationObj);
          createdObjs.insert(newCreationObj);
        }

      }
      else if (slashCount > i) {
        vector<string> split = Split(s, "/");
        string thisDeep = "";
        for(int j = 0; j < i;j++) {
          thisDeep.append(split[j]);
          thisDeep.append("/");
        }

        CreationObject newCreationObj = CreationObject(thisDeep, thisDeep, true);
        if(createdObjs.count(newCreationObj) == 0) {
          orderedObjects.push_back(newCreationObj);
          createdObjs.insert(newCreationObj);
        }
      }
      else {
        continue;
      }
    }
  }
  return orderedObjects;
}

vector<string> GetPaths(string manifestFile) {
  ifstream myFile;
  myFile.open(manifestFile);
  string line;
  vector<string> pathList;

  // Gross skipping of first 2 lines:
  getline(myFile, line);
  getline(myFile, line);


  while(getline(myFile, line)) {
    pathList.push_back(line);
  }

  myFile.close();
  return pathList;
}


vector<string> Split(string str,string sep) {
  char* cstr=const_cast<char*>(str.c_str());
  char* current;
  vector<string> arr;
  current=strtok(cstr,sep.c_str());
  while(current!=NULL){
    arr.push_back(current);
    current=strtok(NULL,sep.c_str());
  }
  return arr;
}
