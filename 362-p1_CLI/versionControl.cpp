// 362 Software Engineering Project 1
// Authors:
//     Kevin Molina-Heineck - kevin.molina@csu.fullerton.edu
//     @@Add your names here@@
//     @@@@@@@@@@@@@@@@@@@@@@@
// 
// Description:
//     TODO

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

#define WEIGHT_ONE 1
#define WEIGHT_TWO 3
#define WEIGHT_THREE 7
#define WEIGHT_FOUR 11
#define WEIGHT_FIVE 17
#define INT_MAX 2147483647

string CheckSum(string fileName);
void Create();
string FormatFileName(long long sum, int count, string extension);
void FolderifyLeaf(string qualfiedPath);

int main() {
    bool f = true;
    char input;
    FolderifyLeaf("/home/kevin/Desktop/test1/Foo/yolooo");
    return 0;
    do {
        cout << "******** Team CLI's VCS System ********" << endl;
        cout << endl;
        cout << "The following commands are available:" << endl;
        cout << endl;
        cout << "C - Create a repository for the given project source tree" << endl;
        cout << "and all its files, including their folder paths within the project" << endl;
        cout << endl;
        cout << "Q - Quit the program" << endl << endl;

        cin >> input;

        switch(toupper(input)){
            case 'C':
                Create();
                break;
            default:
                break;
        }

    } while (toupper(input) != 'Q');
    
    return 0;
}

void Create() {
    string source;
    string destination;

    cout << "Enter source folder path: ";
    cin >> source;
    cout << "Enter destination folder path: ";
    cin >> destination;

    // TODO
    // Remove hard-coded stub test code here
    // Figure out how he wants us to do directory stuff
    // e.g. use a library like Boost or manually recurse
    // Idm doing it - Kevin
    CheckSum("foo.txt");

}

// ****** CheckSum ******
// 
// DESCRIPTION:
// Creates a rolling 5-byte weighted checksum of all the
// characters (ASCII bytes) in the file (i.e., the bytes 
// in the file's contents) followed by a hyphen and an
// “L” and the integer file size, followed by the file's 
// extension. The 5 weights by which each 5 character
// group are multiplied are 1, 3, 7, 11, and, 17
//
// INPUT: File name
// OUTPUT: string representing checksum to be used as filename
string CheckSum(string fileName) {

    ifstream myFile(fileName.c_str());
    if(myFile.fail()) {
        // Don't have to handle this
        // Should probably moduralize this too
        cout << "Error opening file";
    }
    else {
        char ch;
        int count = 0;
        long long sum = 0;

        while(EOF != (ch = myFile.get())) {
            switch(count % 5) {
                case 0:
                    sum += WEIGHT_ONE * ch;
                    sum = sum % INT_MAX;
                    break;
                case 1:
                    sum += WEIGHT_TWO * ch;
                    sum = sum % INT_MAX;
                    break;
                case 2:
                    sum += WEIGHT_THREE * ch;
                    sum = sum % INT_MAX;
                    break;
                case 3:
                    sum += WEIGHT_FOUR * ch;
                    sum = sum % INT_MAX;
                    break;
                case 4:
                    sum += WEIGHT_FIVE * ch;
                    sum = sum % INT_MAX;
                    break;
                default:
                    break;
            }
            count++;
        }
        
        myFile.close();
        path myPath(fileName.c_str());
        string ext = myPath.extension().string();
        return FormatFileName(sum, count, ext); 
    }
    

}



// Convience method to format the filenames. 
// Pass in sum, count, and extension <e.g. '.txt'>
string FormatFileName(long long sum, int count, string extension) {
    string output;
    stringstream ss;
    ss << sum << "-L" << count << extension;

    return ss.str();

}

// Description:
//     Converts a 'leaf' file into a folder w/ 'Checksum' naming
//     Then moves the old file into the new folder, handling cleanup
// 
// INPUT: Fully qualified path to file (e.g. /home/kevin/Desktop/foo.cpp)
// OUTPUT: None
void FolderifyLeaf(string qualfiedPath) {
    path p(qualfiedPath);

    try {
        if(exists(p)) {
            // This is a leaf file!
            if(is_regular_file(p)) {
                // TODO - Handle case where folder exists with 
                // Name from CheckSum output already (weird edge case)
                // Never gunna happen, but should be handled eventually
                string checkSum = CheckSum(qualfiedPath);

                // Get the parent path/dir from qualified path to file
                path newFilePath = p.parent_path();
                newFilePath += "/" + checkSum;

                // Rename file with checksum
                // Check for rename funct in boost
                copy_file(p, newFilePath);
                remove(p);
                create_directory(p);

                path finalDest = p.string() + "/" + checkSum;
                copy_file(newFilePath, finalDest);
                remove(newFilePath);
                // Append / + filename to new path (Hacky)
                // Dont need?
                //newFilePath += "/" + p.filename().string();
                // TODO - Remove old file

            }
            else {
                // Use qualified path instead? Haven't tested p
                cout << p << "exists, but is not a leaf file";
            }
        }
        else {
            cout << p << "Doesn't exist";
        }
    }
    catch (const filesystem_error& ex) {
            cout << ex.what() << endl;
    }

    return;
}