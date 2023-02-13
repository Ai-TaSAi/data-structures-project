#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "miniGit.hpp"
#include <vector>

// Constructor simply erases the old minigit repo and creates a new, fresh one.
MiniGit::MiniGit() {
    fs::remove_all(".minigit");
    fs::create_directory(".minigit");
}

MiniGit::~MiniGit() {   
    // Any postprocessing that may be required
    
    // While cleanup programs are not needed for functionality, they are still implemented as 
    // per usual on all class assignments.

    if (hasInitialized){
        // If the hash table has been initialized, then dismantle it.  Otherwise, there is not need to.
        ht->dismantleHash();
    } else {
        cout << "[CLNUP]: No hash table to dismantle." << endl;
    }

    if (commitHead != NULL){
        // If the DLL has content in it, erase any and all content in it.  Otherwise leave it be.
        dismantleBranchNodes();
    } else {
        cout << "[CLNUP]: No branch nodes to deconstruct." << endl;
    }
}

// Unecessary function that was solved by a boolean.
bool MiniGit::memorySafeGuard(bool initialized){
    return initialized;
}

// Initializer function that isn't the actual constructor.  
void MiniGit::init(int hashtablesize) {
   commits = 0;

    // Creates the DLL head node and populates it with the basic content.
   commitHead = new BranchNode();
   commitHead->commitID = commits;
   commitHead->fileHead = NULL;
   commitHead->next = NULL;
   commitHead->previous = NULL;
   commitHead->commitMessage = "";

    // Set the tail node to the head node.
   commitTail = commitHead;

    // Construct a new hash table.
   ht = new HashTable(hashtablesize);

   hasInitialized = true;
}

// Add function, input fileName, added to the SLL of the DLL node if it exists.
void MiniGit::add(string fileName) {
   bool existsInRepo = false;
    string toSearch = fileName;

    // Loop searches for a matching file in the repository_folder.  If it exists, proceeds.
   while (!existsInRepo) {
       if (fs::exists(toSearch)) {
           existsInRepo = true;
       } else {
           // If the file cannot be found, then the user is prompted to enter a file name that actually works.
           cout << "[ERROR]: File does not exist in repository. Please enter a file name to search: \n[PRMPT] > ";
           cin >> toSearch;
       }
   }

    // Makes sure that the file isn't already added to the SLL in a previous operation.
   if (!checkForSLLMatch(toSearch)) {
       addToSLL(toSearch);
   }

    // Prints the SLL to allow the user to verify that the file and its corresponding version number has been added.
    printSLL();

}

// Checks the working commit to make sure the file hasn't already been added to the SLL.
bool MiniGit::checkForSLLMatch(string fileName){
    // If the fileHead is null, then add it.
    if (commitTail->fileHead == NULL) {
        return false;
    } else {
        if (findFile(fileName) != NULL) {
            // If the file is found, then return true.
            return true;
        }
    }
    // If the previous two checks both fail, then the file should be added.
    return false;
}

// General helper to return the filenode that the key corresponds to.
FileNode* MiniGit::findFile(string fileName){
    if (commitTail->fileHead == NULL){
        return NULL;
    } else {
        FileNode* navigator = commitTail->fileHead;
        while (navigator != NULL) {
            if (navigator->name == fileName){
                return navigator;
            }
            navigator = navigator->next;
        }
        return NULL;
    }
}

// Find the SLL node that is before the requested file name.  Returns null if the file doesn't exist or is the head.
FileNode* MiniGit::findPreviousFile(string fileName){
    if (commitTail->fileHead == NULL){
        return NULL;
    } else if (commitTail->fileHead->name == fileName) {
        return NULL;
    } else {
        FileNode* navigator = commitTail->fileHead;
        while (navigator->next != NULL) {
            if (navigator->next->name == fileName){
                return navigator;
            }
            navigator = navigator->next;
        }
        return NULL;
    }
}

// Adds the file with the corresponding name to the singly linked list.
void MiniGit::addToSLL(string fileName){

    FileNode * temp = new FileNode;
    temp->name = fileName;
    temp->version = 0;
    temp->next = NULL;
    temp->repoName = fileName + "__" + to_string(temp->version); //The repository works as FILENAME.EXT__K.

    if (commitTail->fileHead == NULL){
        commitTail->fileHead = temp;
    } else {
        FileNode* navi = commitTail->fileHead;
        
        while (navi->next != NULL) {
            navi = navi->next;
        }

        navi->next = temp;
    }
}

// Prints the singly linked list of the most recent working commit.
void MiniGit::printSLL(){

    FileNode* navi = commitTail->fileHead;

    if (navi == NULL) {
        cout << "[SYSTM]: [Singly Linked List is Empty]" << endl;
        return;
    }
        
    while (navi != NULL) {

        cout << "[SYSTM]: [OFN: " + navi->name + " / IFN: " + navi->repoName + " / C#: " + to_string(navi->version) + "] --> ";

        navi = navi->next;
    }

    cout << endl;
}

// removes a file from the singly linked list.
void MiniGit::rm(string fileName) {
    FileNode * targetedFile = findFile(fileName);
    if (targetedFile != NULL) {
        FileNode * previousNode = findPreviousFile(fileName);
        if (previousNode == NULL) {
            commitTail->fileHead = targetedFile->next;
            delete targetedFile;
        } else {
            previousNode->next = targetedFile->next;
            delete targetedFile;
        }
    } else {
        //cout << " >| File not found." << endl;
    }

    printSLL();
}


// Prints the hash table in a readable function.
void MiniGit::printSearchTable()
{
     ht->printTable();
}

// Searches the hash table for the key, prints the commit numbers.
void MiniGit::search(string key)
{
    ht->searchItem(key);
}


// The commit function.  Commits the file to the .minigit repository.
string MiniGit::commit(string msg) {

    bool correctSize = true;

    string message = msg;

    // Message verification, both for number of words, and if it already exists.

    do {
        int counter = 0;

        // Makes sure the message has at most three words by counting for 0 .. 2 spaces.
        for (unsigned int i = 0; i < message.length(); i++){
            if (message[i] == ' '){
                counter++;
            }
        }

        if (counter >= 3){
            cout << "[ERROR]: Commit message exceeds word count, please enter new message: ";
            getline(cin, message);
            correctSize = false;
        } else {
            correctSize = true;
        }

        // Scans for the commit message in the DLL, if it already exists, then request a new message.
        if (messageExistsInDLL(message)){
            correctSize = false;
            cout << "[ERROR]: Commit message already exists, please enter new message: ";
            getline(cin, message);
        }

    } while (!correctSize);

    // Setting the working commit's message as this message.

    commitTail->commitMessage = message;

    // Copys the files named in the SLL files over to the mini repository.

    FileNode * navigator = commitTail->fileHead;

    while (navigator != NULL){
        string filePath = ".minigit/" + navigator->repoName;

        if (!fs::exists(filePath)){
            // The file does not exist in the minirepo yet, so it should be copied.
            fileToRepo(navigator->name, filePath);
        } else {
            // The file already exists, which means it needs to be checked for similarities, and the version # incremented.
            if (existChanges(filePath, navigator->name)) {
                navigator->version++;
                filePath = ".minigit/" + navigator->name + "__" + to_string(navigator->version);
                navigator->repoName = navigator->name + "__" + to_string(navigator->version);
                fileToRepo(navigator->name, filePath);
            }
        }

        navigator = navigator->next; // The process is executed with each SLL file node.
    }

    // The message is broken into its individual words, and each word processed into the hash table.
    stringstream sstream(message);
    string word;

    while (sstream >> word){
        ht->insertItem(word, commits);
    }

    // The next branch DLL node is created, and everything necessary copied over.
    BranchNode * nextCommitNode = new BranchNode;

    commits++;
    nextCommitNode->commitID = commits;

    nextCommitNode->previous = commitTail;
    commitTail->next = nextCommitNode;

    nextCommitNode->next = NULL;
    nextCommitNode->commitMessage = "";
    nextCommitNode->fileHead = NULL;

    FileNode * naviNode = commitTail->fileHead;

    // This process creates a deep copy of the SLL from the previous commit.
    
    while (naviNode != NULL){
        FileNode * tempNode = new FileNode;
        tempNode->name = naviNode->name;
        tempNode->repoName = naviNode->repoName;
        tempNode->version = naviNode->version;
        tempNode->next = NULL;

        if (nextCommitNode->fileHead == NULL){
            nextCommitNode->fileHead = tempNode;
        } else {
            FileNode * nextNavi = nextCommitNode->fileHead;
            
            while (nextNavi->next != NULL) {
                nextNavi = nextNavi->next;
            }

            nextNavi->next = tempNode;
        }

        naviNode = naviNode->next;
    }

    // Updates the commit tail.
    commitTail = nextCommitNode;

    // Prints the SLL of the new commit to make sure everything is in order.
    printSLL();

    // Prints the hash table with the newly added messages hashed to the appropriate places.
    ht->printTable();

    return to_string(commits - 1); //should return the commitID of the commited DLL node
}

// The general helper function that copies the baseFile into the repository file.
void MiniGit::fileToRepo(string baseFileName, string repoName){
    ofstream fileObj(repoName);
    ifstream baseFile (baseFileName);
    string fileLine;
    
    // File transcription system that doesn't use fs::copy
    if(baseFile && fileObj){

        int counter = 0;

        while(getline(baseFile,fileLine)){
            if (counter != 0) {
                fileObj << endl << fileLine;
            } else {
                fileObj << fileLine;
            }
            counter++;
        }

    } 

    fileObj.close();
}

// Checks two files for any differences.  If there are differences, return true;
bool MiniGit::existChanges(string prevFile, string thisFile) {

    string prevLine;
    string newLine;

    ifstream previousFile {prevFile};
    ifstream newFile {thisFile};

    if(previousFile && newFile){
        while (getline(previousFile, prevLine) && getline(newFile, newLine)){
            if (prevLine != newLine){
                return true;
            }
        }

        if (getline(previousFile, prevLine) || getline(newFile, newLine)){
            return true;
        }
    }

    return false;
}

// Finds out if the given commit message exists in the DLL.
bool MiniGit::messageExistsInDLL(string commitName) {
    BranchNode * navi = commitHead;

    if (navi == NULL){
        return false;
    } else {
        while (navi != NULL){
            if (navi->commitMessage == commitName){
                return true;
            }

            navi = navi->next;
        }
    }

    return false;
}

// Checkout function that takes a commitID and replaces the working files.
void MiniGit::checkout(string commitID) {
    int searchID = stoi(commitID);

    BranchNode * navi = commitHead;
    BranchNode * commitNode = nullptr;

    while (navi != NULL){
        if (navi->commitID == searchID){
            commitNode = navi;
        }
        navi = navi->next;
    }

    if (commitNode == NULL){
        cout << "[ERROR]: Invalid commit ID." << endl;
        return;
    } else {

        FileNode * fileNavi = commitNode->fileHead;
        
        while (fileNavi != NULL){

            cout << "[SYSTM]: Removing: " << fileNavi->name << " and replacing with " << ".minigit/" + fileNavi->repoName << endl;

            fs::remove(fileNavi->name);
            fileToRepo(".minigit/" + fileNavi->repoName, fileNavi->name);

            fileNavi = fileNavi->next;
        }
    }

}

// The deconstruction helper function that dismantles the SLL nodes and the branch nodes of the DLL.
void MiniGit::dismantleBranchNodes(){
    BranchNode* branchHead = commitHead;

    while (branchHead != NULL){
        BranchNode * workingNode = branchHead;

        while (workingNode->fileHead != NULL) {
            FileNode * deleteNode = workingNode->fileHead;
            workingNode->fileHead = workingNode->fileHead->next;
            delete deleteNode;
        }

        cout << "[CLNUP]: Finished deleting file nodes for commit ID: " << to_string(workingNode->commitID) << endl;

        int tempID = workingNode->commitID;

        branchHead = branchHead->next;
        delete workingNode;

        cout << "[CLNUP]: Deleted branch node with commit ID: " << to_string(tempID) << endl;
    }
}
