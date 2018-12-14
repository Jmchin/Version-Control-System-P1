#include "hierarchy.h"



// Modifies line 1 of manifest file with parent creation heirarchy
void WriteCreationHierarchy(const std::string filePath, const std::string manifestName) {
    std::ifstream readFile;
    readFile.open(filePath);

    // Open first file, grab first line (parent heirarchy)
    std::string firstLine;
    if (readFile.good()) {
        std::getline(readFile, firstLine);
    }

    // Grab the rest of the lines
    std::string line;
    std::vector<std::string> v;
    while(std::getline(readFile, line)) {
        v.push_back(line);
    }

    readFile.close();

    // Modify first line of manifest file
    firstLine.append(manifestName + "/");



    std::ofstream writeFile;
    writeFile.open(filePath);

    // After modification, dump modified first line and write out rest of file
    writeFile << firstLine << std::endl;
    for (int i = 0;i < v.size();i++){
        writeFile << v[i] << std::endl;
    }


}

// Returns back the lowest common parent of two repos
std::string FindMinimalCommonParent(std::string parentListOne, std::string parentListTwo) { char DELIMITER = '/';
    std::string str;

    // Break first string (list) apart by delimiter
    std::stringstream listOneSS(parentListOne);
    std::vector<std::string> listOneVector;
    while (std::getline(listOneSS, str, DELIMITER)) {
        listOneVector.push_back(str);
    }

    // Break second string (list) apart by delimiter
    std::stringstream listTwoSS(parentListTwo);
    std::vector<std::string> listTwoVector;
    while (std::getline(listTwoSS, str, DELIMITER)) {
        listTwoVector.push_back(str);
    }

    std::unordered_set<std::string> listOneSet;

    // Hash one list for more optimal check later on O(N) vs O(N^2)
    for (int i = 0; i < listOneVector.size(); i++) {
        listOneSet.insert(listOneVector[i]);
    }

    // Identify minimal parent or return empty str if not found
    std::string minimalParent = "";
    for (int i = listTwoVector.size() - 1; i >= 0; i--) {
        if (listOneSet.count(listTwoVector[i])) {
            minimalParent = listTwoVector[i];
            break;
        }
    }

    return minimalParent;

}
