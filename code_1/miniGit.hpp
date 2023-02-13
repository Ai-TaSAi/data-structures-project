#pragma once

#include<iostream>
#include<vector>
#include "hash.hpp"

using namespace std;


enum GITOP {init, add, rm, commit, checkout, status};

struct FileNode {
    string name;
    string repoName; // Added a repoName (repository name) for easier file search and copying.
    int version;
    FileNode* next;
};

struct BranchNode {
    int commitID;
    string commitMessage;
    BranchNode* next;
    BranchNode* previous;
    FileNode* fileHead;   
};

//*** You can add helper functions if you desire ***//
class MiniGit {
private:    
    BranchNode* commitHead = NULL;
    int commits;
    HashTable* ht;
    BranchNode* commitTail; // Added a commit tail, because it's better than navigating the list each time.
    
    bool checkForSLLMatch(string fileName);
    void addToSLL(string fileName);
    FileNode* findFile(string fileName);
    FileNode* findPreviousFile(string fileName);

    void printSLL();

    bool messageExistsInDLL(string commitName);

    void fileToRepo(string baseFileName, string repoName);
    bool existChanges(string prevFile, string thisFile);

    void dismantleBranchNodes();

    bool hasInitialized = false;

public:
    MiniGit();
    ~MiniGit();

    void init(int hashtablesize);
    void add(string fileName);
    void rm(string fileName);
    void search(string key);
    string commit(string msg);
    void checkout(string commitID);
    void printSearchTable();

    bool memorySafeGuard(bool initialized); // Unecessary.
    
};