// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
// #include "../code_1/<HEADER FILE>.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <array>
#include <vector>
#include "test_helpers.hpp"

using namespace std;

class test_x : public ::testing::Test {
protected:
	// This function runs only once before any TEST_F function
	static void SetUpTestCase(){
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)0;
			outgrade.close();
		}
	}

	// This function runs after all TEST_F functions have been executed
	static void TearDownTestCase(){
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)std::ceil(100*total_grade/max_grade);
			outgrade.close();
			std::cout << "Total Grade is : " << (int)std::ceil(100*total_grade/max_grade) << std::endl;
		}
	}

	void add_points_to_grade(double points){
		if(!::testing::Test::HasFailure()){
			total_grade += points;
		}
	}

	// this function runs before every TEST_F function
	void SetUp() override {}

	// this function runs after every TEST_F function
	void TearDown() override {
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)std::ceil(100*total_grade/max_grade);
			outgrade.close();
		}
	}
	
	static double total_grade;
	static double max_grade;
};

double test_x::total_grade = 0;
double test_x::max_grade = 100;



/////////////////////////////////////////
// Tests start here
/////////////////////////////////////////

// Tests single word inserts into the hash table.
TEST_F(test_x, TestInsert){
    
    
    string result;
    string expected;
       
    int len = 5;
    
    string commits[] = {"computer", "science", "fun", "difficult", "science"};
    int tabSize = 5;
    
    result = test_insert(commits, len, tabSize);
    
    expected = "0|| science(1,4,)\n1|| \n2|| \n3|| \n4|| computer(0,)-->fun(2,)-->difficult(3,)\n";
    
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

}

// Tests that the hash table is able to break down multi word inputs into the necessary single, hashable, keys.
TEST_F(test_x, TestInsertBreakdown){
    
    
    string result;
    string expected;
       
    int len = 5;
    
    string commits[] = {"strings of pointer", "strings pointer", "pointer of and", "and or of", "theory of strings"};
    int tabSize = 5;
    
    result = test_insert(commits, len, tabSize);
    
    expected = "0|| or(3,)\n1|| \n2|| and(2,3,)-->theory(4,)\n3|| strings(0,1,4,)-->of(0,2,3,4,)\n4|| pointer(0,1,2,)\n";
    
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

}

// Another test similar to the one above.
TEST_F(test_x, TestInsertMultiple){
    
    
    string result;
    string expected;
       
    int len = 5;
    
    string commits[] = {"string", "string pointer", "pointer", "string theory", "theory of everything"};
    int tabSize = 5;
    
    result = test_insert(commits, len, tabSize);
    
    expected = "0|| \n1|| \n2|| theory(3,4,)\n3|| string(0,1,3,)-->of(4,)-->everything(4,)\n4|| pointer(1,2,)\n";
    
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

}

// Makes sure that the hash table returns a blank, but initialized hash table when instructed to do so.
TEST_F(test_x, TestInsertBlank){
    
    
    string result;
    string expected;
       
    int len = 0;
    
    string commits[] = {};
    int tabSize = 5;
    
    result = test_insert(commits, len, tabSize);
    
    expected = "0|| \n1|| \n2|| \n3|| \n4|| \n";
    
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

}

// An unecessary test, but one that makes sure the hash table can handle messages greater than 3 words.
TEST_F(test_x, TestInsertGreaterThanThree){
    
    
    string result;
    string expected;
       
    int len = 4;
    
    string commits[] = {"encyclopedia of knowlege", "the great encyclopedia knows nearly everything", "everything is knowlege and stored in encyclopedia", "knows nearly great"};
    int tabSize = 5;
    
    result = test_insert(commits, len, tabSize);
    
    expected = "0|| knowlege(0,2,)-->is(2,)-->in(2,)\n1|| the(1,)-->great(1,3,)-->nearly(1,3,)\n2|| knows(1,3,)-->and(2,)-->stored(2,)\n3|| of(0,)-->everything(1,2,)\n4|| encyclopedia(0,1,2,)\n";
    
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

}

// Tests that the correct response is given for a hash table with nothing in it.
TEST_F(test_x, TestSearchNullTable){
    
    
    string result;
    string expected;
       
    int len = 0;
    
    string commits[] = {};
    int tabSize = 5;
    
    result = test_search(commits, len, tabSize, "node");
    
    expected = "[ERROR]: Invalid key.\n";
    
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

}

// Tests that nothing will be returned if the key doesn't exist.
TEST_F(test_x, TestSearchWrongKey){
    
    
    string result;
    string expected;
       
    int len = 5;
    
    string commits[] = {"string", "string pointer", "pointer", "string theory", "theory of everything"};
    int tabSize = 5;
    
    result = test_search(commits, len, tabSize, "stack");
    
    expected = "[ERROR]: Invalid key.\n";
    
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

}

// Tests to make sure the table returns an error when a multiword message is entered.
TEST_F(test_x, TestSearchMultiWord){
    
    
    string result;
    string expected;
       
    int len = 5;
    
    string commits[] = {"string", "string pointer", "pointer", "string theory", "theory of everything"};
    int tabSize = 5;
    
    result = test_search(commits, len, tabSize, "stack of tables");
    
    expected = "[ERROR]: Invalid key.\n";
    
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

}

// Makes sure an error is return when attempting to search with nothing as the argument.
TEST_F(test_x, TestSearchBlank){
    
    
    string result;
    string expected;
       
    int len = 5;
    
    string commits[] = {"string", "string pointer", "pointer", "string theory", "theory of everything"};
    int tabSize = 5;
    
    result = test_search(commits, len, tabSize, "");
    
    expected = "[ERROR]: Invalid key.\n";
    
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

}

// Tests a standard search with a correct key.
TEST_F(test_x, TestSearchOne){
    
    
    string result;
    string expected;
       
    int len = 5;
    
    string commits[] = {"string", "string pointer", "pointer", "string theory", "theory of everything"};
    int tabSize = 5;
    
    result = test_search(commits, len, tabSize, "string");
    
    expected = "[SYSTM]: Commits containing the key \"string\": 0, 1, 3\n";
    
    ASSERT_EQ(expected, result);
    add_points_to_grade(5);

}

// Tests another standard search with a correct key.
TEST_F(test_x, TestSearchTwo){
    
    
    string result;
    string expected;
       
    int len = 5;
    
    string commits[] = {"string", "string pointer", "pointer", "string theory", "theory of everything"};
    int tabSize = 5;
    
    result = test_search(commits, len, tabSize, "theory");
    
    expected = "[SYSTM]: Commits containing the key \"theory\": 3, 4\n";
    
    ASSERT_EQ(expected, result);
    add_points_to_grade(5);

}