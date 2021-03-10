
#include "Dictionary.h"



// hash functions
unsigned int DJB2(string term){
  unsigned int hash = 5381;

  for (size_t i=0; i < term.length(); i++) {
    char c = term[i]; 
    hash = ((hash << 5) + hash) + c;

  }

  return hash;
}
//index bucket function
unsigned int modbucket(unsigned int hashcode, unsigned int cap){
  unsigned int b = hashcode % cap;

  return b;
}

//PUBLIC MEMBERS

// constructor, initialize class variables and pointers here if need.
Dictionary::Dictionary(){

}

//deconstructor,
Dictionary::~Dictionary(){
}

shared_ptr<diction_table> Dictionary::newDictionary(){
  shared_ptr<diction_table> ret(new diction_table);
  //initalize cap to prime number 13
  unsigned int cap = 13;
  ret -> capacity = cap;
  ret -> size = 0;
  //includes delete entries
  ret -> occupied = 0;

  ret -> hash_func = DJB2;

  ret -> bucket_func = modbucket;
  ret->table = shared_ptr<dtable>(new dtable(ret -> capacity));


  return ret;
}




bool Dictionary::addToDictionary(shared_ptr<diction_table> dict, string term, string def){
  if(searchDictionary(dict, term)) {
    return false;
  }
  //this is encrypted, needs mod to get index
  unsigned int hashcoded = dict -> hash_func(term);
  //this is the index
  unsigned int search = dict -> bucket_func(hashcoded, dict ->capacity);
  shared_ptr<diction_node> newNode = InitItem(term, hashcoded, def);
  //if the hash table has room for another entry
  //change this to something about the load?
  //load factor is ≥ 0.6.
  if(Load(dict) > 0.6){
      IncreaseDict(dict); 
  }
  if (dict -> table -> at(search)) {
    if ((dict -> table-> at(search) ->hashcode == hashcoded) && (dict -> table-> at(search) ->term == term)) {
      return false; //item already exists
    }
    //otherwise linear search for the next bucket
    else {
      while(dict -> table-> at(search)) {
        search++;
        //if you've reached the end without an "empty since start" bucket
        //loop back to the front of the table
        if(search == dict ->capacity) {
          search = search % (dict -> capacity);
        } 
      }
      dict -> table-> at(search) = newNode;
      dict -> size += 1;
      dict -> occupied += 1;
      return true;
    }
  }
  else { //no node at index yet
    dict -> size += 1;
    dict -> occupied += 1;
    dict -> table-> at(search) = newNode;
    return true;
  }
  
}



string Dictionary::searchDictionary_T(shared_ptr<diction_table> dict, string term){

  unsigned int hash = dict -> hash_func(term);
  for(unsigned int i = 0; i < dict -> capacity; i++) {
    if(dict -> table -> at(i)) {
      if((dict -> table -> at(i) -> hashcode == hash) && !(dict -> table -> at(i) -> deleted)) {
        return dict -> table -> at(i) -> def;
      }
    } 
  }
  return "Error: no matching term found";
}

bool Dictionary::searchDictionary(shared_ptr<diction_table> dict, string term){

  unsigned int hash = dict -> hash_func(term);
  for(unsigned int i = 0; i < dict -> capacity; i++) {
    if(dict -> table -> at(i)) {
      if((dict -> table -> at(i) -> hashcode == hash) && !(dict -> table -> at(i) -> deleted)) {
        return true;
      }
    } 
  }
  return false;
}


bool Dictionary::deleteFrom_Dictionary(shared_ptr<diction_table> dict, string term){
  //hash the term and search through the vecotr
  //update variables if found and return true else false
  unsigned int hash = dict -> hash_func(term);
  for(unsigned int i = 0; i < dict -> capacity; i++) {
    if(dict -> table -> at(i)) {
      if((dict -> table -> at(i) -> hashcode == hash) && !(dict -> table -> at(i) -> deleted)) {
        dict -> table -> at(i) -> term = "";
        dict -> table -> at(i) -> deleted = true;
        dict -> size -= 1;
        return true;
      }
    } 
  }
  return false;
}

string Dictionary::grepDictionary_D(string searchDef, shared_ptr<diction_table>dict) {
  /*-pattern recognition search in the definition portion of the dictionary. Planning on implementing this by placing all unique 
  strings into an array, appending the hash index of each item and then doing a binary search on this array*/
  shared_ptr<vector<grepTable>> grepTabled = makeGrepTable(dict);
  for(unsigned int i = 0; i < grepTabled -> size(); i++) {
    if(grepTabled -> at(i).defText == searchDef) {
      return (grepTabled -> at(i).terms);
      
    }
  }
  return "No results, please try another search";
}

void Dictionary::write_csv(shared_ptr<diction_table> dict) {
  /*writes all terms and definitions to a csv file. Scientist mainly work in excel so this type of 
		file will feel more natural. They can then send this file to other collaborators. This uses the function alphabetize_Dict*/
    vector<string> terms;
    vector<string> defs;
    alphabetize_Dict(terms, defs, dict);
    ofstream outFile("Dictionary.csv");
    outFile << "Term, Definition" << endl;
    for(int i = 0; i < terms.size(); i++) {
      outFile << terms.at(i);
      outFile << ",";
      outFile << defs.at(i) << endl;
    }
    outFile.close();
}

//PRIVATE MEMBERS
shared_ptr<diction_node> Dictionary::InitItem(string term, unsigned int hashcode, string def){
  shared_ptr<diction_node> ret(new diction_node);
  ret -> deleted = false;
  ret -> term = term;
  ret -> hashcode = hashcode;
  //change defString to array
  ret -> def = def;
  return ret;
}

void Dictionary::IncreaseDict(shared_ptr<diction_table> dict){
  int size = dict -> capacity;
  int newSize = nextPrime(size);
  //newArray = Allocate new array of size newSize
  shared_ptr<dtable> newTable(new dtable(newSize));
  shared_ptr<dtable> oldTable = dict -> table;
  dict -> capacity = newSize;
  dict -> size = 0;
  dict -> occupied = 0;
  dict -> table = newTable;

  int bucket = 0;
  while(bucket < size) {
    if (oldTable -> at(bucket) != NULL) {
      if(oldTable -> at(bucket) -> deleted == false) {
        string nextTerm = oldTable -> at(bucket) -> term;
        string nextDef = oldTable -> at(bucket) -> def;
        addToDictionary(dict, nextTerm, nextDef);
      }
    }
      bucket++;
  }
}

float Dictionary::Load(shared_ptr<diction_table> dict){
    //calc the load of the hash struct
  float cap = dict ->capacity;
  float occ = dict ->occupied;
  return (occ / cap);
}

int Dictionary::nextPrime(int size) {
  int newSize = size * 2;
  while(!primeCheck(newSize)) {
    newSize += 1;
  }
  return newSize;
}

 bool Dictionary::primeCheck(int data) {
  for(int i = 2; i < data; i++) {
    if(data % i == 0) {
      return false;
    }
  }
  return true;
}

shared_ptr<vector<grepTable>> Dictionary::makeGrepTable(shared_ptr<diction_table>dict) {
    shared_ptr<vector<grepTable>> grepped (new vector<grepTable>);
    string defTemp;
    string wordo;
    grepTable next;
    string termed;
    bool find = false;
    //loop through the dictionary
    for(int i = 0; i < dict -> capacity; i++) {
      //make sure it's non null otherwise keep moving
      if(dict -> table -> at(i) !=NULL) {
        //make sure it wasn't deleted
        if(!dict -> table -> at(i) -> deleted) {
          //get the definition sentence and create individual words by looping through the chars
          defTemp = dict -> table -> at(i) -> def;
          for(int j = 0; j < defTemp.length(); j++) {
            wordo.push_back(defTemp.at(j));
            //if the next char is a space
            if(defTemp[j+1] == ' ' | j == defTemp.length() - 1) {
              //check that it's not already in the vector
              for(int y = 0; y < grepped -> size(); y++) {
                if(grepped -> at(y).defText ==  wordo) {
                  termed = dict -> table -> at(i) -> term;
                  grepped -> at(y).terms.append(", ");
                  grepped -> at(y).terms.append(termed);
                  find = true;
                  wordo = "";
                  j++;
                  break;
                }
              }
              //if it wasn't found add it
              if(!find) {
                next.defText = wordo;
                next.terms = dict -> table -> at(i) -> term;
                grepped -> push_back(next);
                //reinitialize variables
                wordo = "";
                j++;
                next.terms = "";
                next.defText = "";
              }
              find = false;
            }
          }
        }
      }
    }
  //looping through hash table
    //loop through definition parsing the def into a word 
      //loop through vector -> if parse word not in vector
        //add to vector
        //else append term to element already present
  return grepped;
}

void Dictionary::alphabetize_Dict(vector<string>& terms, vector<string>& defs, shared_ptr<diction_table>dict) {
  /*alphabetizes the terms and places them in a vector. It will also place the associated 
		definitions in a separate vector (at the same index)*/
    for(int i = 0; i < dict -> capacity; i++) {
      if(dict -> table -> at(i) !=NULL) {
        if(!dict -> table -> at(i) -> deleted) {
          terms.push_back(dict -> table -> at(i) -> term);
        }
      }
    }
    //sort and then match by index
    sort(terms.begin(), terms.end());
    int j = 0;
    string checkTerm;
    for(int i = 0; i < terms.size(); i++) {
      checkTerm = dict -> table -> at(j) -> term;
      while(terms.at(i) != checkTerm) {
        j++;
        if(dict -> table -> at(j) != NULL) {
          checkTerm = dict -> table -> at(j) -> term;
        }
      }
      defs.push_back(dict -> table -> at(j) -> def);
      j = 0;
    }
}
