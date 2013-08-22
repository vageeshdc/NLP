/*
 * Author: vageesh D C
 * Role: NLP testing for hash usage
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

using namespace std;

int hash_func(string word);

struct char_vals{

	char letter[26];
	int counts[26];

};

int del_arr[26][26];
int swap_arr[26][26];
int add_arr[26][26];
int subs_ar[26][26];

int err_arr[4][26][26];

enum ERRORS {SUBS = 0,ADD,DEL,SWAP};

struct error_pairs{
	
	char p1;
	char p2;
	ERRORS err_type;
};

unordered_map<string,int> freq_list;
unordered_map<string,int> word_list;

//vector<string> name_list;

int main(){
	return 0;
}

void insert_new_word(string name){

	//finding the element
	unordered_map<string,int>::iterator got_elem = word_list.find(name);
	
	if(got_elem == word_list.end()){
		//element not there
		//inserting
		word_list[name] = hash_func(name);
		
	}
}

void set_word_freq(string name){

	//finding the element
	unordered_map<string,int>::iterator got_elem = word_list.find(name);
	
	if(got_elem != word_list.end()){
		//element is present
		freq_list[name] ++;
	}

}



double get_score(error_pairs* word_err_list,string name){

	error_pairs* err_ite = word_err_list;
	double prob = 1.0;
	int word_count = freq_list[name];

	/*
	 * Lets assume a naive bayes setting
	 */
	while(err_ite != 0){

		prob *= 1.0*(err_arr[err_ite->err_type][err_ite->p1][err_ite->p2])/word_count;

		*err_ite++;
	}

	return prob;
}


void generate_variants(string name){
	
	/*
	 * THis generates all names
	 * perform the various operations and see if they are in the main name list
	 *
	 * TODO: make this idea of changes very scalable
	 * implement the idea 0f smoothing the data and the counts
	 * have a heuristic on how much can you actually change a word to get a new word
	 * do this for all four variations
	 * get the u/p format for words then for training text_corpus and testing the confusion mat
	 *
	 */
	

	//deletion and swap
	string tmp_name,cpy_name;
	int i = 0;
	error_pairs	elem;

	while(i < name.length()){
		
		tmp_name = name;
		tmp_name.erase(tmp_name.begin()+i);

		elem.p2 = name[i];
		
		if(i == 0){
			elem.p1 = '@';
		}
		else{
			elem.p1 = name[i-1];
		}

		elem.err_type = ERRORS::DEL;

		//do something
	
		cpy_name = name;
		char tmp_char = cpy_name.at(i);

		if(i < name.length()-1){
			cpy_name[i] = cpy_name[i+1];
			cpy_name[i+1] = tmp_char;
			//do something
			elem.p1 = name[i];
			elem.p2 = name[i+1];
			elem.err_type = ERRORS::SWAP;
		}
		
		i++;
	}

}

void check_variants_and_comp(string name,error_pairs epair){

	//finding the element
	unordered_map<string,int>::iterator got_elem = word_list.find(name);
	
	if(got_elem != word_list.end()){

		

	}


}

int hash_func(string word){

	hash<string> hash_val;
	return hash_val(word);
}
