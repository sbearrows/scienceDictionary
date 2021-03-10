#ifndef DICTIONARY_H__
#define DICTIONARY_H__

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <fstream>

using namespace std;


//used in the grep function to hold the definition and the matching terms
struct grepTable {
  //will have only one word here
  string defText;
  //append to this each time followed by a space
  string terms;
};


//this is a node in the dictionary table (shared_ptr<dtable> table)
//holds the term and definition and hashcode of each item
//also holds info about status of the node (deleted)
struct diction_node {
  bool deleted;

  //term name
  string term;

  unsigned int hashcode;

  //this is the definition of the term
  string def;
};

//this is the actual vector of diction nodes
//pointed to by shared_ptr<dtable> table; in the diction_table struct
//points to the diction_node struct
typedef vector<shared_ptr<diction_node>> dtable;

//holds the actual dictionary hash table and other metadata
struct diction_table {
  //max items allowed
  unsigned int capacity;

  // number of actual real items
  unsigned int size;

  // number of non-null buckets including deleted
  unsigned int occupied;

  shared_ptr<dtable> table;

  //initialized to DJB2 in newDictionary function
  unsigned int (*hash_func)(string);
  //initialized to modulenucketfunc in newDictionary function
  unsigned int (*bucket_func)(unsigned int hashcode, unsigned int capacity); 
};

//this is the hash function
unsigned int DJB2(string term);
//this assigns items to indexes in the table
unsigned int modbucket(unsigned int hashcode, unsigned int cap);



class Dictionary {
public:
  Dictionary();

  ~Dictionary();
  //creates a new empty dictionary
  shared_ptr<diction_table> newDictionary();

  //once user creates dictionary they can directly add new terms with definitions using this function
  //this function also checks if the capacity of the hash table is getting too small
  // and dynamically will resize (load > 0.6)
  bool addToDictionary(shared_ptr<diction_table> dict, string term, string def);

  //they can search for specific terms and this function will return the definition of that term
  string searchDictionary_T(shared_ptr<diction_table> dict, string term);

  bool searchDictionary(shared_ptr<diction_table> dict, string term);


  //they can remove items from the dictionary using this function
  bool deleteFrom_Dictionary(shared_ptr<diction_table> dict, string term);

  //to get all terms that contain a certain word in their definition they can use this function
  //will return all terms as a string
  string grepDictionary_D(string searchDef, shared_ptr<diction_table>dict);

  //they can write their entire dictionary to a csv file using this function
  //terms are sorted alphabetically by default
  void write_csv(shared_ptr<diction_table> dict);



private:
  //used in the addToDictionary function
  //initializes a new dictionary item
  shared_ptr<diction_node> InitItem(string term, unsigned int hashcode, string val);

  //this dynamically increases the size of the table when load > 0.6
  //this way the user does not have to specify a size for their dictionary or keep track
  //of the size
  void IncreaseDict(shared_ptr<diction_table> dict);

  //use to check if dictionary needs to be resized
  float Load(shared_ptr<diction_table> dict);

  //get the next primer number to increase the size of the dictionary
  //used in the increaseDict function
  int nextPrime(int size);

  //checks if a number is prime
  //used in the nextPrime function
  bool primeCheck(int data);

  //makes the vector for the grep lookup
  //organized as a vector where each element has a string of a word in a def
  //and then all the terms that have that word in their definitions
  //does a brute force search in the grep function to find matches
  shared_ptr<vector<grepTable>> makeGrepTable(shared_ptr<diction_table>dict);


  //alphabetize the term and def vectors for the write_csv function
  //first sorts the terms vector then matches the terms in the dict to the definitions
  //so that there are two vectors at matching index
  //could also have used the struct grepTable to make a vector with two elements per node
  void alphabetize_Dict(vector<string>& terms, vector<string>& defs, shared_ptr<diction_table>dict);
};
#endif  // DICTIONARY_H__