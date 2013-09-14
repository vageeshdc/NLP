/*
 * NLP phrase checker
 * Author: Vageesh D C
 */

#include "hash_test.h"

//ngram data
unordered_map<string,int> gram2_list;
unordered_map<string,int> gram3_list;
unordered_map<string,int> gram4_list;
unordered_map<string,int> gram5_list;

/*
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      if(i == 0){
	int num = atoi(argv[i]);
	printf("\n value %f\n",log2(num));
      }
   }
   printf("\n");
   return 0;
}

int main_sqlite3_test(int argc, char* argv[]){
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called\n";

   // Open database 
   rc = sqlite3_open("2gram.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   // Create SQL statement 
   sql = "SELECT * from ngram";

   // Execute SQL statement 
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   return 0;
}
*/



int main(){
    
    int N;// number of inputs
    
    while(N--){
      
	//the input
	char phrase[1024];// original string
	vector<string> tok_phrase;// tokenized by space strings
	
	//tokenize
	char *ind_tok = strtok(phrase, " ");//getting tokens
	while (ind_tok) {
	    ind_tok = strtok(NULL, " ");
	    tok_phrase.push_back(ind_tok);
	}
	
	//find the error word
	vector<string> possibs;// possible correct strings
	vector<double> scores;// scores of the possibilities
	
	int err_index;//index where the error has occured in phrase
	
	//finding the error!!
	int ix;
	bool found_wrong = false;
	for(ix = 0;((ix < tok_phrase.size())&&(!found_wrong));ix++){
	    solve_test_cases(tok_phrase.at(ix));
	    
	    int i,j = 0;
	    for(i = ranked_list.size()-1; i >= 0; i--) {
		
		err_index = ix;
		found_wrong = true;
		
		if((score_list[i] > REQ_TRESH) || (j < MAX_CHOICE)) {
		    possibs.push_back(ranked_list.at(i));
		    j++;
		}
		else {
		    i = -1;
		    break;
		}
	    }
	}
	
	//finding the correct choices
	int idx;
	for(idx = 0;idx < possibs.size();idx++){
	    tok_phrase.at(err_index) = possibs.at(idx);
	    
	    //generate n grams and get score
	    scores.push_back(generate_score(tok_phrase,err_index));
	}
	
	int max_idx = get_max_element<double>(scores);//??
	
	if(max_idx == -1){
	    //error could not find anything!
	    cout << " !!! no solutions fond :/\n";
	}
	else{
	    //found something!
	    tok_phrase.at(err_index) = possibs.at(max_idx);
	    cout << gen_combined_toks(tok_phrase,0,tok_phrase.size()-1) << "\n";
	}
	
    }
    
    return 0;
}

template <class T>
int get_max_element(vector<T> elems){
    
    if(elems.size() == 0){
	return -1;
    }
    else{
	T f_elem = elems.at(0);
	int idx = 0;
	for(int i = 1;i < elems.size();i++){
	    if(f_elem <= elems.at(i)){
		f_elem = elems.at(i);
		idx = i;
	    }
	}
	return idx;
    }
}

template <class T>
int compare_vals (T *a, T *b){
    //const double *da = (const double *) a;
    //const double *db = (const double *) b;

    return (*a > *b) - (*a < *b);
}

double generate_score(vector<string> new_phase,int err_index){
    
    double score = 0.0;
    int num_tok = new_phase.size();
    
    for(int i = 2;i <=5;i++){
	for(int j = 0;j < i;j++){
	    //check if the indices are in bounds
	    int e_idx = err_index +j;
	    int s_idx = err_index +j -i +1;
	    
	    if(inrange(0,num_tok-1,s_idx) && inrange(0,num_tok-1,e_idx)){
		string q_string = gen_combined_toks(new_phase,s_idx,e_idx);
		
		//retieve string
		double tmp_score = get_score(q_string,i);
		score += (tmp_score < 0)?0.0:tmp_score;
	    }
	}
    }
    
    return score;
}

bool inrange(int start,int end,int q_idx){
    return ((start <= end)&&(start <= q_idx)&&(q_idx<=end))?true:false;
}

string gen_combined_toks(vector<string> new_phrase,int start_idx,int end_idx){
    
    string combined_t;
    
    for(int idx = start_idx;idx <= end_idx;idx++){
	combined_t.append(new_phrase.at(idx));
	
	if(idx < end_idx){
	    combined_t.append(" ");
	}
    }
    
    return combined_t;
}

double get_score(string sub_phrase,int num_tok){
    unordered_map<string,int>::iterator got_elem;// = word_list.find(name);
    unordered_map<string,int>::iterator end_elem;
    double score = -1.0;
    
    switch(num_tok){
        case 2: got_elem = gram2_list.find(sub_phrase);end_elem = gram2_list.end();break;
        case 3: got_elem = gram3_list.find(sub_phrase);end_elem = gram3_list.end();break;
        case 4: got_elem = gram4_list.find(sub_phrase);end_elem = gram4_list.end();break;
        case 5: got_elem = gram5_list.find(sub_phrase);end_elem = gram5_list.end();break;
    }
    
    if(got_elem != end_elem) {

	score = (double)got_elem->second;
	return log2(score);
    }
}

void build_ngram(string fname,int gram_val){

    ifstream map_file(fname);
    
    if(map_file.is_open()) {
        while(map_file.good()) {

            string name;
            int count_word;

            map_file>>count_word;
	    
	    for(int i = 0;i < gram_val;i++){
		string tmp_name;
		map_file >> tmp_name;
		name.append(tmp_name);
		
		if(i < gram_val -1){
		    name.append(" ");
		}
	    }
	    
	    switch(gram_val){
	      case 2: gram2_list.insert(unordered_map<string,int>::value_type(name,count_word));break;
	      case 3: gram3_list.insert(unordered_map<string,int>::value_type(name,count_word));break;
	      case 4: gram4_list.insert(unordered_map<string,int>::value_type(name,count_word));break;
	      case 5: gram5_list.insert(unordered_map<string,int>::value_type(name,count_word));break;
	    }
            
        }
        map_file.close();
    }
}

void serialize_ngram(string fname,int ngram){
    unordered_map<string,int>::iterator map_elem;// = word_list.begin();
    unordered_map<string,int>::iterator end_elem;
    
    switch(ngram){
        case 2: map_elem = gram2_list.begin();end_elem = gram2_list.end();break;
        case 3: map_elem = gram3_list.begin();end_elem = gram3_list.end();break;
        case 4: map_elem = gram4_list.begin();end_elem = gram4_list.end();break;
        case 5: map_elem = gram5_list.begin();end_elem = gram5_list.end();break;
    }
    
    ofstream map_file(fname);

    if(map_file.is_open()) {
        for(; map_elem != end_elem; map_elem++) {
            map_file<<map_elem->second<<" "<<map_elem->first<<"\n";
        }
        map_file.close();
    }
}