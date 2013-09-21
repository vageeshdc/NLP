#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <unistd.h>    /* for getopt */
#include <string.h>
#include <climits>
#include <math.h>

using namespace std;

//ngram data
unordered_map<string,int> word_list; // 1 gram
unordered_map<string,int> gram2_list;
unordered_map<string,int> gram3_list;
unordered_map<string,int> gram4_list;
unordered_map<string,int> gram5_list;

void build_ngram(string fname,int gram_val); // dont use this!!
void load_serialized_ngram(string fname,int ngram);

void insert_new_word(string name);
void set_word_freq(string name);
void populate_word_list(string fname);
void populate_corpus(string fname);

#define MAX_SIZ 20000

int GT_basic_count[MAX_SIZ];

int main(int argc,char** argv){
  
    populate_word_list(string(argv[1])); // 1gram list
    populate_corpus(string(argv[2]));    // populate 1 gram list
    load_serialized_ngram(argv[3],2);    // 2 gram
    load_serialized_ngram(argv[4],3);    // 3 gram
    load_serialized_ngram(argv[5],4);    // 4 gram
    load_serialized_ngram(argv[6],5);    // 5 gram
    
    //init stage
    for(int i = 0;i < MAX_SIZ;i++){
	GT_basic_count[i] = 0;
    }
    
    //1gram
    unordered_map<string,int>::iterator g1_ite = word_list.begin();
    while(g1_ite != word_list.end()){
	
	if(g1_ite->second < MAX_SIZ-1){
	    GT_basic_count[g1_ite->second] ++;
	}
	else{
	    GT_basic_count[MAX_SIZ-1] ++;
	}
	g1_ite++;
    }
    
    // ngram case
    int i_ = 2;
    while(i_ <6){
      
	unordered_map<string,int>::iterator map_elem;
	unordered_map<string,int>::iterator end_elem;
	
	switch(i_){
	    case 2: map_elem = gram2_list.begin();end_elem = gram2_list.end();break;
	    case 3: map_elem = gram3_list.begin();end_elem = gram3_list.end();break;
	    case 4: map_elem = gram4_list.begin();end_elem = gram4_list.end();break;
	    case 5: map_elem = gram5_list.begin();end_elem = gram5_list.end();break;
	}
	while(map_elem != end_elem){
	    
	    if(map_elem->second < MAX_SIZ-1){
		GT_basic_count[map_elem->second] ++;
	    }
	    else{
		GT_basic_count[MAX_SIZ-1] ++;
	    }
	    map_elem++;
	}
	i_++;
    }
    
    for(int i = 0;i < MAX_SIZ;i++){
	if(GT_basic_count[i] > 0){
	  cout << i << " " << log2(i) << " " << GT_basic_count[i] << " " <<log2(GT_basic_count[i]) << "\n";
	}
    }
    
    return 0;
}


void insert_new_word(string name) {

    //finding the element
    unordered_map<string,int>::iterator got_elem = word_list.find(name);

    if(got_elem == word_list.end()) {
        //element not there
        //inserting
        word_list[name] = 1;
    }
}

void set_word_freq(string name) {

    //finding the element
    unordered_map<string,int>::iterator got_elem = word_list.find(name);

    if(got_elem != word_list.end()) {
        //element is present
        word_list[name] ++;
    }

}

void populate_word_list(string fname) {

    ifstream infile(fname);
    if(infile.is_open()) {
        while(infile.good()) {
            string tmp;
            infile >> tmp;
	    
	    /*
	    char chars_[] = "\'() ,.-;!?\n\"";
	    for (unsigned int i = 0; i < strlen(chars_); ++i)
	    {
		tmp.erase (remove(tmp.begin(), tmp.end(), chars_[i]), tmp.end());
	    }
	    */
	    tmp.erase (remove(tmp.begin(), tmp.end(), '\''),tmp.end());
	    transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	    
            insert_new_word(tmp);
        }
        infile.close();
    }
}

void populate_corpus(string fname) {

    ifstream infile(fname);
    //tot_word_count = 0;

    if(infile.is_open()) {
        while(infile.good()) {
            string tmp;
            infile >> tmp;
	    
	    char chars[] = "() ,.-;!?\n\"";
	    char* tmp_name = (char*)tmp.c_str();
	    char * pch = strtok (tmp_name,chars);
	    
	    while (pch != NULL)
	    {
		pch = strtok (NULL,chars);
		
		if(pch != 0){
		    //printf("%s\n",pch);
		    string tmp_ = string(pch);
		    
		    char chars_[] = "\'() ,.-;!?\n\"";
		    for (unsigned int i = 0; i < strlen(chars_); ++i)
		    {
			tmp_.erase (remove(tmp_.begin(), tmp_.end(), chars_[i]), tmp_.end());
		    }
		    transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
		    set_word_freq(tmp_);
		    //tot_word_count++;
		}
	    }
	    
        }
        infile.close();
    }
}

void build_ngram(string fname,int gram_val){

    ifstream map_file(fname);
    
    unordered_map<string,int>::iterator map_elem;// = word_list.begin();
    unordered_map<string,int>::iterator end_elem;
    unordered_map<string,int> *tmp_map;
    
    switch(gram_val){
        case 2: map_elem = gram2_list.begin();end_elem = gram2_list.end();tmp_map = &gram2_list;break;
        case 3: map_elem = gram3_list.begin();end_elem = gram3_list.end();tmp_map = &gram3_list;break;
        case 4: map_elem = gram4_list.begin();end_elem = gram4_list.end();tmp_map = &gram4_list;break;
        case 5: map_elem = gram5_list.begin();end_elem = gram5_list.end();tmp_map = &gram5_list;break;
    }
    
    if(map_file.is_open()) {
        while(map_file.good()) {

            string name;
            int count_word;

	    for(int i = 0;i < gram_val;i++){
		string tmp_name;
		map_file >> tmp_name;
		
		char chars_[] = "\'() ,.-;!?\n\"";
		for (unsigned int i = 0; i < strlen(chars_); ++i)
		{
		    tmp_name.erase (remove(tmp_name.begin(), tmp_name.end(), chars_[i]), tmp_name.end());
		}
	        transform(tmp_name.begin(), tmp_name.end(), tmp_name.begin(), ::tolower);
		
		name.append(tmp_name);
		
		if(i < gram_val -1){
		    name.append(" ");
		}
	    }
	    
	    map_file>>count_word;
	    //if(gram_val == 4)
	    //  cout << "key ops " << count_word << ":" << name <<"\n";
	    map_elem = tmp_map->find(name);
	    
	    if(map_elem != end_elem){
		(*tmp_map)[name] += count_word;
	    }
	    else{
		tmp_map->insert(unordered_map<string,int>::value_type(name,count_word));
	    }
        }
        map_file.close();
    }
}

void load_serialized_ngram(string fname,int ngram){
    ifstream map_file(fname);
    
    if(map_file.is_open()) {
        while(map_file.good()) {

            string name;
            int count_word;

            map_file>>count_word;
	    //map_file>>name;
	    
	    for(int i = 0;i < ngram;i++){
		string tmp_name;
		map_file >> tmp_name;
		name.append(tmp_name);
		
		if(i < ngram -1){
		    name.append(" ");
		}
	    }
	    
	    //if(ngram == 4)
	    //  cout << "key ops " << count_word << ":" << name <<"\n";
	    
	    switch(ngram){
	      case 2: gram2_list.insert(unordered_map<string,int>::value_type(name,count_word));break;
	      case 3: gram3_list.insert(unordered_map<string,int>::value_type(name,count_word));break;
	      case 4: gram4_list.insert(unordered_map<string,int>::value_type(name,count_word));break;
	      case 5: gram5_list.insert(unordered_map<string,int>::value_type(name,count_word));break;
	    }
            
        }
        map_file.close();
    }
}

