// 362 Software Engineering Project 1
// Authors:
//     Kevin Molina-Heineck - kevin.molina@csu.fullerton.edu
// 
// 
// 
// Description:
//     TODO

using namespace std;
#include <iostream>
#include <fstream>

#define WEIGHT_ONE 1
#define WEIGHT_TWO 3
#define WEIGHT_THREE 7
#define WEIGHT_FOUR 11
#define WEIGHT_FIVE 17

int main() {
    bool f = true;
    char input;

    do {
        cout << "******** Team CLI's VCS System ********" << endl;
        cout << endl;
        cout << "The following commands are available:" << endl;
        cout << endl;
        cout << "C - Create a repository for the given project source tree" << endl;
        cout << "and all its files, including their folder paths within the project" << endl;
        cout << endl;
        cout << "Q - Quit the program" << endl;

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
// INPUT: File reference
// OUTPUT: string representing checksum to be used as filename
string CheckSum() {

}