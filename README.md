# Data Structures - Final Project

**Authors: Rui Wen**

**Class: CSCI 2750**

### Project Overview
The objective of this project is to use knowlege on doubly-linked lists, singly-linked lists, and hash tables to implement a rudimentary form of GitHub in the CSEL file and coding environment.

In order to try and run the code, a preexisting "repository_folder" has been created with three files inside.  Feel free to add or remove files as you see fit, as long as they exist within the repository_folder.  These files will be used to demonstrate the different features of the minigit, as specified by Phase I.  There is no additional content.

To boot up the minigit, navigate your way into the repository_folder:
```
$ cd repository_folder
```

From there, run the following command to start the minigit.
```
$ ../build/run_app_1
```

### Options in the MiniGit

Upon start, you will be presented with the following menu:
```
Select a numerical option:
+=======Main Menu=======+
 1. init 
 2. add 
 3. rm 
 4. commit 
 5. checkout
 6. search
 7. quit 
+-----------------------+
#>
```
Each option will be explained in the following sections.

#### 1. Initialize (init)

This function initializes the various data structures that serve as the foundation of the minigit.  When selecting this option, the user will be prompted to enter the size of their hash table.  Normal testing uses a table of size 5.

After input, the .minigit folder will be created, as well as the doubly linked branch node list and the hash table.  A node with a commit ID of 0 will be made as the first branch.

If the user does not initialize the minigit, they will be locked from most of the functionality of the rest of the options, besides the quit option.

#### 2. Add (add)

This function prompts the user to enter a valid file name within their chosen repository.  The function then takes this string and checks to make sure such a file indeed exists within the repository_folder.  If it exists, the file, as well as its version (which will always start with a commit ID of 0), is added to the singly linked list of the current working branch node in the DLL.

However, if the file already exists in the singly linked list, or the file name is invalid, the file will not be added.  In the case the file name is invalid, the user will be prompted to reenter a file name until it is valid.

#### 3. Remove (rm)

This function prompts the user to enter a file name.  It will then scan the current working commit's singly linked list for a node whose key matches the entered name.  It will then proceed to remove the node from the singly linked list.  The user can re-add the node if they chose to do so.

#### 4. Commit (commit)

This function commits the changes that have been edited and added to the SLL on the current working node.  The user will be prompted to enter a commit message that **should not exceed 3 words**. After inputting the commit message, the doubly linked list will read its singly linked file node list, and then proceed to do one of two things:
- If the file doesn't exist in the .minigit, it will be copied there.
- If the file does exist, and has changes, the version number will be updated in the DLL, and the new file copied into the .minigit.

The given message will then be broken into its individual words, where each word will then be hashed to an index in the hash table, where it is stored in the table.  If the key already exists, the commit number is added to the key's node.  If the key doesn't exist, a new node containing the key and the commit number is created and appended to the hash table's index pointer.

During this, the Doubly Linked List will created a new tail node, updating all appropriate values to account for the additional node.  This node will become the new working node, where it will make a hard copy (no pointers) of the previous DLL node's file list, as well as increment its commit ID number.

#### 5. Checkout (checkout)

This function allows the user to *check-out* previous commits.  However, this is done by reverting the files outside of the .minigit in the repository, which means that any local changes will be erased.  If the user agrees to this warning, then they are prompted with entering a commit version, starting from 0 and going to the working commit. If they haven't made a commit yet, the function will terminate and nothing will change.  If they have made a commit, then the SLL of that specific commit node will be read, and the repository files from that commit version will overwrite the ones in the repository_folder.  **Note that this only changes the files of that specific commit.** If a file was not added to the SLL in that instance of a commit, but was added and modified in later commits, **it will not be changed**, as it was not added to the repository as a file that needed to be tracked at that moment.

When using this function, file modification and commiting will be completely locked, as it is only intended for file viewing. However, the search and quit functions are still available for use.  If the user wishes to edit and commit again, they have to checkout again, and enter the commit ID of the working commit, or the tail of the doubly linked list.

#### 6. Search (search)

This function simply prompts the user for a key.  The key ideally should be one of the words used in a commit message string.  The minigit then takes this key, and returns a list of all commit IDs that have this word in the commit message.  It does so by hashing the key, and then scanning the index for the appropriate node.  It will return an error if it cannot find it.

#### 7. Quit (quit)

There isn't much to be said about this function.  It exits out of the program.  However, inside this function, there are two helper functions that help dismantle the hash table and the minigit.  

However, if the minigit and/or hash table hasn't been initialized, the helper functions will not run, as the two data structures aren't taking up any heap memory.

Please note that while memory-based storage will be cleared, the .minigit repository itself will not be.  It will still contain a copy of each changed file after each commit, for inspection and convinience.

### Operating and Evaluation Notes
- 11 test cases have been written in the tests folder of the project folder.  They can be quickly run and evaluated if the following inputs are executed when in the project folder:
```
$ cd build
$ ./run_tests
```

*~Rui W.*