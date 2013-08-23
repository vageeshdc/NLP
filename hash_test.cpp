/*
 * Author: vageesh D C
 * Role: NLP testing for hash usage
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <fstream>

using namespace std;

#define MAX_LEVEL 2
#define MIN_TRESH 1E-6

int hash_func(string word);

double err_arr[4][27][27];

enum ERRORS {SUBS,ADD,DEL,SWAP};

struct error_pairs{
	
	char p1;
	char p2;
	ERRORS err_type;
};

unordered_map<string,int> word_list;

double var_val;

vector<double> score_list;
vector<string> name_list;

double check_variants_and_comp(string name,error_pairs epair);
void generate_variants(string name,int level);

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
		word_list[name] ++;
	}

}

void populate_word_list(string fname){

	ifstream infile(fname);
	if(infile.is_open()){
		while(infile.good()){
			string tmp;
			getline(infile,tmp,' ');

			if(tmp[tmp.length()-2] == '\n') tmp[tmp.length()-2] = '\0';

			insert_new_word(tmp);
		}
		infile.close();
	}
}

void populate_corpus(string fname){
	
	ifstream infile(fname);
	if(infile.is_open()){
		while(infile.good()){
			string tmp;
			getline(infile,tmp,' ');

			if(tmp[tmp.length()-2] == '\n') tmp[tmp.length()-2] = '\0';

			set_word_freq(tmp);
		}
		infile.close();
	}
}

void update_confusion_mat(string fname){

	ifstream infile(fname);
	if(infile.is_open()){

		while(infile.good()){
		
			int i,j,k;
			for(i = 0;i < 4;i++){
				for(j = 0;j < 27;j++){
					for(k = 0;k < 27;k++){
						infile>>err_arr[i][j][k];
					}
				}
			}

		}

		infile.close();
	}
}

double get_score(error_pairs word_err_list,string name){

	double prob = 1.0;
	int word_count = word_list[name];

	/*
	 * Lets assume a naive bayes setting
	 */

	// incorporate smoothing ...
	char i,j,k;
	i = word_err_list.err_type;
	j = word_err_list.p1;
	k = word_err_list.p2;

	if(j == '@'){
		j = 0;
	}
	else{
		j = j-'a'+1;
	}

	k -= 'a';

	prob = 1.0*(err_arr[i][j][k])/word_count;

	return prob;
}


void generate_variants_add(string name,int level){
	
	/*This is to add a new char*/
	int i = 0;
	string tmp_name;

	while(i <= name.length()){

		char j = 'a';

		while(j <= 'z'){
			tmp_name = name;
			tmp_name.insert(i,1,j);

			error_pairs elms;

			if(i == 0){
				elms.p1 = '@';
			}
			else{
				elms.p1 = tmp_name[i-1];
			}

			elms.p2 = j;
			elms.err_type = ERRORS::ADD;

			// call the validation function
			var_val = check_variants_and_comp(tmp_name,elms);
			if(var_val > -1){
				name_list.push_back(tmp_name);
				score_list.push_back(var_val);
			}
				
			if(var_val > MIN_TRESH || var_val < 0)
				generate_variants(tmp_name,level);

			//increment
			j++;
		}
		i++;
	}
}

void generate_variants_subs(string name,int level){
	
	/*This is to add a new char*/
	int i = 0;
	string tmp_name;

	while(i < name.length()){

		char j = 'a';

		while(j <= 'z'){
			tmp_name = name;
			tmp_name[i]= j;

			error_pairs elms;
			elms.p1 = name[i];
			elms.p2 = j;
			elms.err_type = ERRORS::SUBS;


			// call the validation function
			var_val = check_variants_and_comp(tmp_name,elms);
			if(var_val > -1){
				name_list.push_back(tmp_name);
				score_list.push_back(var_val);
			}
	
			if(var_val > MIN_TRESH || var_val < 0)
				generate_variants(tmp_name,level);

			//increment
			j++;
		}
		i++;
	}
}

void generate_variants_del(string name,int level){
	
	/*This is to add a new char*/
	int i = 0;
	string tmp_name;

	while(i < name.length()){

		tmp_name = name;
		tmp_name.erase(i,1);

		error_pairs elms;

		if(i == 0){
			elms.p1 = '@';
		}
		else{
			elms.p1 = tmp_name[i-1];
		}

		elms.p2 = name[i];
		elms.err_type = ERRORS::DEL;

		// call the validation function
		var_val = check_variants_and_comp(tmp_name,elms);
		if(var_val > -1){
			name_list.push_back(tmp_name);
			score_list.push_back(var_val);
		}

		if(var_val > MIN_TRESH || var_val < 0)
			generate_variants(tmp_name,level);

		//increment
		i++;
	}
}

void generate_variants_swap(string name,int level){
	
	/*This is to add a new char*/
	int i = 0;
	string tmp_name;

	while(i < name.length()-1){

		tmp_name = name;
		char tmp_char = tmp_name[i+1];
		tmp_name[i+1] = tmp_name[i];
		tmp_name[i+1] = tmp_char;

		error_pairs elms;
		elms.p1 = name[i];
		elms.p2 = name[i+1];
		elms.err_type = ERRORS::SWAP;

		// call the validation function
		var_val = check_variants_and_comp(tmp_name,elms);
		if(var_val > -1){
			name_list.push_back(tmp_name);
			score_list.push_back(var_val);
		}
		
		if(var_val > MIN_TRESH || var_val < 0)
			generate_variants(tmp_name,level);
		//increment
		i++;
	}
}



void generate_variants(string name,int level){
	
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
	if((level > MAX_LEVEL)||( name_list.size() != 0 )){
		return;
	}

	generate_variants_add(name,level+1);
	generate_variants_subs(name,level+1);
	generate_variants_del(name,level+1);
	generate_variants_swap(name,level+1);

	
	/*
	//deletion and swap
	string tmp_name,cpy_name;
	int i = 0;
	error_pairs	elem;
	double var_val;

	vector<double> score_list;
	vector<string> name_list;

	while(i < name.length()){
		
		/// performing deletion
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

		//call to the scoring thing
		var_val = check_variants_and_comp(tmp_name,elem);

		//pushing the lists
		if(var_val > -1){
			name_list.push_back(tmp_name);
			score_list.push_back(var_val);
		}
	
		/// Performing swaping
		cpy_name = name;
		char tmp_char = cpy_name.at(i);

		if(i < name.length()-1){
			cpy_name[i] = cpy_name[i+1];
			cpy_name[i+1] = tmp_char;
			elem.p1 = name[i];
			elem.p2 = name[i+1];
			elem.err_type = ERRORS::SWAP;

			//call to scored
			var_val = check_variants_and_comp(cpy_name,elem);

		//pushing the lists
			if(var_val > -1){
				name_list.push_back(tmp_name);
				score_list.push_back(var_val);
			}
		}
		
		i++;
	}
	*/

}



double check_variants_and_comp(string name,error_pairs epair){

	//finding the element
	unordered_map<string,int>::iterator got_elem = word_list.find(name);
	
	if(got_elem != word_list.end()){

		double score = get_score(epair,name);
		return score;
	}

	return -1;
}

int hash_func(string word){

	hash<string> hash_val;
	return hash_val(word);
}
