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
#include <algorithm>

using namespace std;

#define MAX_LEVEL 2
#define MIN_TRESH 1E-6

#define REQ_TRESH 1E-6
#define MAX_CHOICE 2

double err_arr[4][27][27];

enum ERRORS {SUBS,ADD,DEL,SWAP};

struct error_pairs{
	
	char p1;
	char p2;
	ERRORS err_type;
};

unordered_map<string,int> word_list;

double var_val;
int tot_word_count;

vector<double> score_list;
vector<string> name_list;
vector<string> ranked_list;

typedef std::vector<int> int_vec_t;
typedef std::vector<std::string> str_vec_t;
typedef std::vector<size_t> index_vec_t;

class SequenceGen {
public:
    SequenceGen (int start = 0) : current(start) { }
    int operator() () { return current++; }
private:
    int current;
};

class Comp{
   vector<double>& _v;
 public:
   Comp(vector<double>& v) : _v(v) {}
   bool operator()(size_t i, size_t j){
         return _v[i] < _v[j];
   }
};

double check_variants_and_comp(string name,error_pairs epair);
void generate_variants(string name,int level);
int hash_func(string word);
void solve_test_cases();
void generate_top_opts();
void sort_vector();

void generate_variants_add(string name,int level,double init_val);
void generate_variants_sub(string name,int level,double init_val);
void generate_variants_del(string name,int level,double init_val);
void generate_variants_swap(string name,int level,double init_val);
void generate_variants(string name,int level,double init_val);
double check_variants_and_comp(string name,error_pairs epair);

void insert_new_word(string name);
void set_word_freq(string name);
void populate_word_list(string fname);
void populate_corpus(string fname);
void update_confusion_mat(string fname);

int main(){
	return 0;
}

void solve_test_cases(){
    
    // the number of cases
    int num_cases;
    cin>>num_cases;
    
    int t = 0;
    while(t < num_cases){
        
        string name;
        cin>>name;
        
        //perform the search;
        unordered_map<string,int>::iterator got_elem = word_list.find(name);
	    if(got_elem == word_list.end()){
		    //element not there
		    cout<<name;
		    
		    generate_variants(name,0,1.0);	
		    sort_vector();
		    generate_top_opts();
		    
	    }
        t++;
    }
}

void generate_top_opts(){
    
    //based on confidence..
    int i,j = 0;
    for(i = ranked_list.size()-1;i >= 0;i--){
        if((score_list[i] > REQ_TRESH) || (j < MAX_CHOICE)){
            cout<<" "<<ranked_list[i]<<" "<<score_list[i];
            j++;
        }
        else{
            i = -1;
            break;
        }
        
    }
    cout<<"\n";
}

void sort_vector(){
    
    vector<int> indices(score_list.size());
    std::generate(indices.begin(), indices.end(), SequenceGen(0));

    std::sort(indices.begin(), indices.end(), Comp(score_list));
    
    int i;
    for(i = 0;i < indices.size();i++){
        ranked_list.push_back(name_list[indices[i]]);
    }
    
}

void serialize_umap(string fname){
    unordered_map<string,int>::iterator map_elem = word_list.begin();
    ofstream map_file(fname);
    
    if(map_file.is_open()){
        for(;map_elem != word_list.end();map_elem++){
            map_file<<map_elem->first<<" "<<map_elem->second<<"\n";    
        }
        map_file.close();
    }
}

void build_umap(string fname){
    
    ifstream map_file(fname);
    
    if(map_file.is_open()){
        while(map_file.good()){
            
            string name;
            int count_word;
        
            map_file>>name>>count_word;
            word_list.insert(unordered_map<string,int>::value_type(name,count_word));
        }
        map_file.close();
    }
}

void insert_new_word(string name){

	//finding the element
	unordered_map<string,int>::iterator got_elem = word_list.find(name);
	
	if(got_elem == word_list.end()){
		//element not there
		//inserting
		word_list[name] = 0;	
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
			infile >> tmp;
			
			insert_new_word(tmp);
		}
		infile.close();
	}
}

void populate_corpus(string fname){
	
	ifstream infile(fname);
	tot_word_count = 0;
	
	if(infile.is_open()){
		while(infile.good()){
			string tmp;
			infile >> tmp;
			
			set_word_freq(tmp);
			
			tot_word_count++;
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

	k = k - 'a'+1;

	prob = (err_arr[i][j][k])*(0.5 + word_count)/(double)(word_list.size() + tot_word_count); //smoothing here

	return prob;
}


void generate_variants_add(string name,int level,double init_val){
	
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
				score_list.push_back(var_val*init_val);
			}
				
			if(var_val > MIN_TRESH || var_val < 0)
				generate_variants(tmp_name,level,var_val*init_val);

			//increment
			j++;
		}
		i++;
	}
}

void generate_variants_sub(string name,int level,double init_val){
	
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
				score_list.push_back(var_val*init_val);
			}
	
			if(var_val > MIN_TRESH || var_val < 0)
				generate_variants(tmp_name,level,var_val*init_val);

			//increment
			j++;
		}
		i++;
	}
}

void generate_variants_del(string name,int level,double init_val){
	
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
			score_list.push_back(var_val*init_val);
		}

		if(var_val > MIN_TRESH || var_val < 0)
			generate_variants(tmp_name,level,var_val*init_val);

		//increment
		i++;
	}
}

void generate_variants_swap(string name,int level,double init_val){
	
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
			score_list.push_back(var_val*init_val);
		}
		
		if(var_val > MIN_TRESH || var_val < 0)
			generate_variants(tmp_name,level,var_val*init_val);
		//increment
		i++;
	}
}



void generate_variants(string name,int level,double init_val){
	
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

	generate_variants_add(name,level+1,init_val);
	generate_variants_sub(name,level+1,init_val);
	generate_variants_del(name,level+1,init_val);
	generate_variants_swap(name,level+1,init_val);

	
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
