#include "Dictionary.h"
//g++ dictionaryTest.cpp Dictionary.cpp -o tests

#include <iostream>
#include <string>
#include <memory>

using namespace std;

//write some test functions

void test_newDictionary(shared_ptr<diction_table> dict) {
    int check = 0;
    if(dict -> capacity == 13) {
        check++;
    }
    if(dict ->occupied == 0) {
        check++;
    }
    if(dict ->size == 0) {
        check++;
    }
    if(check != 3) {
        cout << "Some or all tests failed for test_newDictionary" << endl;
    } else {
        cout << "All tests for test_newDictionary passed" << endl;
    }
};

void test_addDictionary(shared_ptr<diction_table> dict, string colltest) {
    int check = 0;
    if(dict -> size == 5) {
        check++;
    }
    if(dict ->occupied == 5) {
        check++;
    }
    unsigned int hashcode = dict ->hash_func("WL.num");
    unsigned int bucket_idx = dict -> bucket_func(hashcode, dict -> capacity);
    if(dict -> table ->at(bucket_idx) -> def == "Lab identification system") {
        check++;
    }
    unsigned int hashcode1 = dict ->hash_func("Full.name");
    unsigned int bucket_idx1 = dict -> bucket_func(hashcode1, dict -> capacity);
    if(dict -> table ->at(bucket_idx1) -> def == "Full sample name") {
        check++;
    }
    unsigned int hashcode2 = dict ->hash_func("Tissue");
    unsigned int bucket_idx2 = dict -> bucket_func(hashcode2, dict -> capacity);
    if(dict -> table ->at(bucket_idx2) -> def == "Tissue type of the sample such as spleen or sputum") {
        check++;
    }
    unsigned int hashcode3 = dict ->hash_func("Providing_Lab_Key");
    unsigned int bucket_idx3 = dict -> bucket_func(hashcode3, dict -> capacity);
    if(dict -> table ->at(bucket_idx3) -> def == "Identification system from collaborators") {
        check++;
    }
    //can't test directly because tissue will have same index
    //but want to check that Providing_Lab did not get overwritten
    //when attempting to add twice
    if(colltest == "Origin of the sample") {
        check++;
    }
    if(check != 7) {
        cout << "Some or all tests for addDictionary failed." << endl;
    } else {
        cout << "All tests for addDictionary passed" << endl;
    }
    
};


void test_searchDictionary_T(string search1, string search2) {
    if(search1 == "Origin of the sample") {
        cout << "test_searchDictionary_T part1 passed" << endl;

    } else {
        cout << "test_searchDictionary_T part1 failed" << endl;
    }
    if(search2 == "Error: no matching term found") {
        cout << "test_searchDictionary_T part2 passed" << endl;
    } else {
        cout << "test_searchDictionary_T part2 failed" << endl;
    }
};


void test_deleteFrom_Dictionary(bool test1, bool test2, shared_ptr<diction_table> dict){
    int check = 0;
    if(test1 == true) {
        check++;
    }
    if(test2 == false) {
        check++;
    }
    unsigned int hashcode = dict ->hash_func("WL.num");
    unsigned int bucket_idx = dict -> bucket_func(hashcode, dict -> capacity);
    if(dict -> table ->at(bucket_idx) -> deleted == true) {
        check++;
    }

    if(dict -> size == 2) {
        check++;
    }
    if(dict -> occupied == 5) {
        check++;
    }
    if(check == 5) {
        cout << "All tests for deleteFrom_Dictionary passed" << endl;
    } else {
        cout << "Some or all tests for deleteFrom_Dictionary failed" << endl;
    }
};

void test_IncreaseDict(shared_ptr<diction_table> dict, string increaseTest) {
    //add more items should dynamically increase in size
    int check = 0;
    //check that the last element was added
    if(increaseTest == "What treatment regimine was the patient recieving") {
        check++;
    }
    unsigned int newCap = dict -> capacity;
    if(newCap > 13) {
        check++;
    }
    if(dict -> size ==  14) {
        check++;
    }
    if(dict -> occupied == 15) {
        check++;
    }
    if(check == 4) {
        cout << "All tests for IncreaseDict passed." << endl;
    } else {
        cout << "Some or all tests for IncreaseDict failed" << endl;
    }
    //for debugging
    //for(int i = 0; i < dict -> capacity; i++) {
      //  if(dict -> table -> at(i)) {
        //    cout << dict -> table -> at(i) ->term << endl;
        //}
    //}

    

};

void test_grepDictionary_D(shared_ptr<diction_table> dict, string grepped, int test){
    if(test == 1) {
        if(grepped == "Experiment_Date_ddpcr, Extractor_ddpcr") {
            cout << "All checks passed for part one of test_grepDictionary" << endl;
        } else {
            cout << "Some or all tests failed for part one of test_grepDictionary" << endl;
        }
    }
    if(test == 2) {
        if(grepped == "Providing_Lab, CFU") {
            cout << "All checks passed for part two of test_grepDictionary" << endl;
        } else {
            cout << "Some or all tests failed for part two of test_grepDictionary" << endl;
        }
    }

};



int main () {

    Dictionary newDict;
    shared_ptr<diction_table> dict = newDict.newDictionary();

    test_newDictionary(dict);

    newDict.addToDictionary(dict, "WL.num", "Lab identification system");
    newDict.addToDictionary(dict, "Full.name", "Full sample name");
    newDict.addToDictionary(dict, "Tissue", "Tissue type of the sample such as spleen or sputum");
    newDict.addToDictionary(dict, "Providing_Lab_Key", "Identification system from collaborators");
    newDict.addToDictionary(dict, "Providing_Lab", "Origin of the sample");
    //check if this will be overwritten (same hash different definition)
    //it should NOT OVERWRITE
    //users will need to delete entries and remake them if they wish to overwrite
    newDict.addToDictionary(dict, "Providing_Lab", "Origin of the patients sample");    

    string collTest = newDict.searchDictionary_T(dict, "Providing_Lab");
    //tests for add to dictionary
    //size and ocp should be 5
    test_addDictionary(dict, collTest);



    //search for definitions of the following
    string search1 = newDict.searchDictionary_T(dict, "Providing_Lab");
    //should through an error
    string search2 = newDict.searchDictionary_T(dict, "Experiment_Date_qpcr");
    test_searchDictionary_T(search1, search2);



    //delete three items
    bool learn1 = newDict.deleteFrom_Dictionary(dict, "WL.num");
    //size = 4, ocp = 5
    bool learn2 = newDict.deleteFrom_Dictionary(dict, "Experiment_Date_qpcr");
    //size = 4, ocp = 5 and learn2 = false
    newDict.deleteFrom_Dictionary(dict, "Full.name");
    //size = 3, ocp = 5
    newDict.deleteFrom_Dictionary(dict, "Tissue");
    //size = 2, ocp = 5
    test_deleteFrom_Dictionary(learn1, learn2, dict);
    
    //check dynamic resizing
    newDict.addToDictionary(dict, "Experiment_Date_qpcr", "Experiment date for qpcr");
    newDict.addToDictionary(dict, "Extractor_qpcr", "Wet lab member who completed qpcr");
    newDict.addToDictionary(dict, "CT_1", "CT value for replicate 1");
    newDict.addToDictionary(dict, "CT_2", "CT value for replicate 2");
    newDict.addToDictionary(dict, "CT_3", "CT value for replicate 3");
    newDict.addToDictionary(dict, "BestResult_qpcr", "Will be no if the experiment needs to be redone and a yes otherwise");
    newDict.addToDictionary(dict, "Experiment_Date_ddpcr", "Experiment date for ddpcr");
    newDict.addToDictionary(dict, "Extractor_ddpcr", "Wet lab member who completed ddpcr");
    newDict.addToDictionary(dict, "AssayNumber", "Assay number of the experiment since samples will have more than one assay completed");
    newDict.addToDictionary(dict, "Ratio", "Final value for the samples");
    newDict.addToDictionary(dict, "CFU", "Colony forming units for the sample");
    newDict.addToDictionary(dict, "Mtb_Burden", "Mtb burden of the patient");
    newDict.addToDictionary(dict, "DrugTreatment", "What treatment regimine was the patient recieving");
    //at the end cap > 13
    //size = 15
    //cap = 15 since items that were deleted were removed

    newDict.deleteFrom_Dictionary(dict, "AssayNumber");
    string increaseCheck = newDict.searchDictionary_T(dict, "DrugTreatment");
    //now size should be 14 and cap should be 15
    test_IncreaseDict(dict, increaseCheck);

    
    //test grep
    //should equal "ddpcr_experiment_Date, ddpcr_extractor"
    
    string grepTest = newDict.grepDictionary_D("ddpcr", dict);
    int test = 1;
    test_grepDictionary_D(dict, grepTest, test);

    string grepTest2 = newDict.grepDictionary_D("sample", dict);
    test = 2;
    test_grepDictionary_D(dict, grepTest2, test);


    //check output
    newDict.write_csv(dict);
    
    return 0;
    
}

