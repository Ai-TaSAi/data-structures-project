#include <iostream>
#include "../code_1/miniGit.hpp"
#include <filesystem>
using namespace std;

/*
 * Purpose; displays a menu with options
 */
void displayMenu()
{
    cout << "Select a numerical option:" << endl;
    cout << "+=======Main Menu=======+" << endl;
    cout << " 1. init " << endl;
    cout << " 2. add " << endl;
    cout << " 3. rm " << endl;
    cout << " 4. commit " << endl;
    cout << " 5. checkout" << endl;
    cout << " 6. search" << endl;
    cout << " 7. quit " << endl;
    cout << "+-----------------------+" << endl;
    cout << "#> ";
}
int main(int argc, char* argv[]) {
    
    // CHOICE SELECTION

    // Controller booleans make sure repository is initialized, as well as keep the while loop running.
    bool endRun = false;
    bool hasInitialized = false;

    // Various input variables for various different functions that use them.
    string enteredName = "";
    int hashSize = -1;
    int commitNumber;
    string searchKey;
    string commitLine;
    char choiceChar;

    // Two integers for the checkout function.  One keeps track of the most recent commit, 
    // the other the currently viewing one.
    int commitVersion = 0;
    int currentCommitIndex = 0;

    // The minigit.
    MiniGit minGit = MiniGit();

    // While loop to keep the system running.
    while (!endRun) {

        int intChoice = -1;

        displayMenu();

        cin >> intChoice;

        // Most of the functions have two safeguards: The first is to make sure the repository is initialized.
        // The second is to make sure the commit versions are the same before modification.
        switch (intChoice) {

        // Initializes the repository.
        case 1:

            if (hasInitialized) {
                cout << "[ERROR]: Repository already initialized." << endl;
                break;
            } else {
                cout << "[SYSTM]: Initializing .minigit and data structures. Please enter the size of the hash table:\n[PRMPT] > ";
                cin >> hashSize;
                minGit.init(hashSize);
                hasInitialized = true;
            }
            break;

        // Adds a new file to the SLL in the DLL.
        case 2:

            if (currentCommitIndex != commitVersion){
                cout << "[ERROR]: Unable to modify current files when viewing past commits!" << endl;
            } else if (!hasInitialized){
                cout << "[ERROR]: Repository and data structures not initialized! Do that first!" << endl;
            } else {
                cout << "[SYSTM]: Adding a new file to the repository. Please enter the file name:\n[PRMPT] > ";
                cin >> enteredName;
                minGit.add(enteredName);
            }

            break;

        // Removes files from the SLL of the current working commit.
        case 3:

            if (currentCommitIndex != commitVersion){
                cout << "[ERROR]: Unable to modify current files when viewing past commits!" << endl;
            } else if (!hasInitialized){
                cout << "[ERROR]: Repository and data structures not initialized! Do that first!" << endl;
            } else {
                cout << "[SYSTM]: Removing a file from the repository. Please enter the file name:\n[PRMPT] > ";
                cin >> enteredName;
                minGit.rm(enteredName);
            }

            break;

        // Commit function that adds new files to the .minigit folder.
        case 4:

            if (currentCommitIndex != commitVersion){
                cout << "[ERROR]: Unable to modify current files when viewing past commits!" << endl;
            } else if (!hasInitialized){
                cout << "[ERROR]: Repository and data structures not initialized! Do that first!" << endl;
            } else {
                cout << "[SYSTM]: Commiting local changes. Please enter a message:\n[PRMPT] > ";
                cin.ignore();
                getline(cin, commitLine);
                minGit.commit(commitLine);
                commitVersion++;
                currentCommitIndex++;
            }

            break;

        // Checkout.  Prompts the user to confirm their actions, and then reverts the appropriate files back
        // to that version of the repository.
        case 5:
            if (!hasInitialized) {
                cout << "[ERROR]: Repository and data structures not initialized! Do that first!" << endl;
            } else {
                cout << "[SYSTM]: This will delete any local changes you have made. Are you sure you want to continue?\n[PRMPT] [Y/N] > ";

                cin >> choiceChar;

                if (choiceChar == 'N' || choiceChar == 'n') {
                    break;
                } else if (choiceChar == 'Y' || choiceChar == 'y') {
                    cout << "[SYSTM]: Enter a valid version number. Choices are from 0 to " << to_string(commitVersion) << ":\n[PRMPT] > ";
                    cin >> commitNumber;
                    currentCommitIndex = commitNumber;
                    minGit.checkout(to_string(commitNumber));
                } else {
                    cout << "[ERROR]: Invalid character entered. Returning to menu." << endl;
                }
            }

            break;

        // Search function that takes in a word and searches through the commit hash table.
        case 6:
            if (!hasInitialized){
                cout << "[ERROR]: Repository and data structures not initialized! Do that first!" << endl;
            } else {
                cout << "[SYSTM]: Enter a single word to search for:\n[PRMPT] > ";
                cin >> searchKey;
                minGit.search(searchKey);
            }

            break;

        // Quit that also calls deconstructor components.
        case 7:
            cout << "[CLNUP]: Starting cleanup and dismantling procedures." << endl;
            endRun = true;
            break;
        
        // If the option is invalid, the user is prompted to enter a valid choice.
        default:
            cout << "[ERROR]: Not a valid option. Please reselect." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            break;
        }
    }

    return 0;
}