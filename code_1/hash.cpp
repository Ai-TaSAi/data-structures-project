// CPP program to implement hashing with chaining
#include<iostream>
#include<sstream>
#include "hash.hpp"


using namespace std;

HashNode* HashTable::createNode(string key, HashNode* next)
{
    HashNode* nw = new HashNode;

    nw->key = key;
    nw->next = NULL;

    return nw;
}

// Constructor, intializes the heap array.
HashTable::HashTable(int bsize)
{
   tableSize = bsize;
   table = new HashNode*[tableSize];

    for (int i = 0; i < tableSize; i++){
        table[i] = NULL;
    }
}

// Function to calculate hash index, as stated on the requirements.
unsigned int HashTable::hashFunction(string s)
{
    int sum = 0, index = 0;
    
    for (string::size_type i = 0; i < s.length(); i++){
        sum += s[i];
    }

    index = sum % tableSize;

    return index;
}

// TODO Complete this function
// Function that searches the index of the hash table based on the string key.
HashNode* HashTable::searchItem(string key)
{
    // If the key is blank, then return as an invalid choice.
    if (key == ""){
        cout << "[ERROR]: Invalid key." << endl;
        return NULL;
    }

    // If the key is a space, also return as an invalid choice.
    for (unsigned int x = 0; x < key.size(); x++){
        if (key[x] == ' '){
            cout << "[ERROR]: Invalid key." << endl;
            return NULL;
        }
    }

    // Calculate the hash of the key.
    int hashIndex = hashFunction(key);

    HashNode * navi = table[hashIndex];

    // Scan the system for the corresponding key node.
    while(navi != NULL){
        if (navi->key == key) {
            cout << "[SYSTM]: Commits containing the key \"" << key << "\": ";

            for (unsigned int i = 0; i < navi->commitNums.size(); i++){
                if (i == 0){
                    cout << to_string(navi->commitNums[i]);
                } else {
                    cout << ", " << to_string(navi->commitNums[i]);
                }
            }

            cout << endl;
            return navi;
        }
        navi = navi->next;
    }

    // If the key isn't found, then declare an error.
    cout << "[ERROR]: Invalid key." << endl;
    return NULL;
    
}

//TODO Complete this function

// Helper function that breaks down any multi-word strings into individual words to process.
void HashTable::breakDownString(string unbrokenString, int cNum){
    stringstream sstream(unbrokenString);
    string word;

    while (sstream >> word){
        insertItem(word, cNum);
    }
}

// Insert function that takes a key and the commit number. 
bool HashTable::insertItem(string key, int cNum)
{
    // Checks to make sure the key is only one word.  If it isn't, break it apart.
    for (unsigned int i = 0; i < key.size(); i++){
        if (key[i] == ' '){
            breakDownString(key, cNum);
            return true;
        }
    }

    // calculate the index based on the key.
    int hashIndex = hashFunction(key);
    int commitNumber = cNum;

    HashNode * navi = table[hashIndex];

    if (navi == NULL) {
        // Table has a null hashnode at the index, make a new node with key and commit.
        HashNode* temp = createNode(key, NULL);
        table[hashIndex] = temp;
        temp->commitNums.push_back(commitNumber);
        return true;
    } else {
        // Table has a hashnode, and the list of keys contains a match.

        while (navi != NULL){
            if (navi->key == key){
                navi->commitNums.push_back(commitNumber);
                return true;
            }
            navi = navi->next;
        }

        // Table has a hashnode, and the list of keys doesn't have a match. Account for Head and Tail.

        HashNode * navi = table[hashIndex];

        if (navi->next == NULL) {
            HashNode* temp = createNode(key, NULL);
            navi->next = temp;
            temp->commitNums.push_back(commitNumber);
            return true;
        } else {
            while (navi->next != NULL){
                navi = navi->next;
            }
            HashNode* temp = createNode(key, NULL);
            navi->next = temp;
            temp->commitNums.push_back(commitNumber);
            return true;
        }
    }

    return false;
}


// function to display hash table //
/* assume the table size is 5. For each bucket it will show the 
** the string key and the commit number (separated by comma) within parenthesis
** e.g. the key is science and commit numbers are 1 and 4. The key science
** is hashed to position 0. So the print format will be-

0|| science(1,4,)
1|| 
2|| 
3|| 
4|| difficult(3,)-->fun(2,)-->computer(0,)

*/

void HashTable::printTable()
{
    for (int i = 0; i < tableSize; i++){
        int counter = 0;
        cout << to_string(i) << "|| ";

        if (table[i] != NULL) {
            HashNode * navi = table[i];

            while (navi != NULL){

                if (counter != 0){
                    cout << "-->";
                }

                cout << navi->key << "(";

                for (unsigned int k = 0; k < navi->commitNums.size(); k++){
                    cout << navi->commitNums[k] << ",";
                }

                cout << ")";

                navi = navi->next;
                counter++;
            }
        }
        cout << endl;
    }
 }

// Function that dismantles the hash table, along with any hash nodes that are stored in its indexes.
void HashTable::dismantleHash(){
    for (int i = 0; i < tableSize; i++){
        while (table[i] != NULL){
            HashNode * deleteNode = table[i];
            table[i] = table[i]->next;
            delete deleteNode;
        }

        cout << "[CLNUP]: Removed all linked list hash nodes from table index " << to_string(i) << "." << endl;
    }

    delete [] table;

    cout << "[CLNUP]: Removed hash table." << endl;
}
