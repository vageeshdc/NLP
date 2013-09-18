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

int main(int argc,char** argv){
    int c;
    int digit_optind = 0;
    /*
     *  a -- the dictionary eg /usr/share/dict/words file of dictionary entries
     *  b -- the corpus
     *  c -- update the matrix
     *  d -- dump the hash map to given file
     *  e -- load using given hash table
     *  f -- load the N gram file
     *  g -- load the hash table
     *  h -- dump the hash table
     *
     * an array to find out the state...
     */

    char* file_names[8];
    bool flags[] = {false,false,false,false,false,false,false,false};

    while ( (c = getopt(argc, argv, "a:b:c:d:e:f:g:h:")) != -1) {
        int this_option_optind = optind ? optind : 1;
        switch (c) {
        case 'a':
            file_names[0] = optarg;
            flags[0] = true;
            break;
        case 'b':
            file_names[1] = optarg;
            flags[1] = true;
            break;
        case 'c':
            file_names[2] = optarg;
            flags[2] = true;
            break;
        case 'd':
            file_names[3] = optarg;
            flags[3] = true;
            break;
        case 'e':
            file_names[4] = optarg;
            flags[4] = true;
            break;
	case 'f':
	    file_names[5] = optarg;
            flags[5] = true;
            break;
	case 'g':
	    file_names[6] = optarg;
            flags[6] = true;
            break;
	case 'h':
	    file_names[7] = optarg;
            flags[7] = true;
            break;
        case '?':
            break;
        default:
            printf ("?? getopt returned character code 0%o ??\n", c);
        }
    }

    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        printf ("\n");
    }
    
    if(flags[0] & flags[1]){
        //we have to load from given corpus
        populate_word_list(string(file_names[0]));
	populate_corpus(string(file_names[1]));
    }
    else if(flags[4]){
	build_umap(string(file_names[4]));
    }
    
    if(flags[3]){
	serialize_umap(string(file_names[3]));
    }
    
    if(flags[2]){
	update_confusion_mat(string(file_names[2]));
    }
    
    if(flags[5]){
	//get the file name having n gram and counts
	//cout << "the file name having n gram and counts\n";
	
	ifstream ip_data_file(file_names[5]);
	int _i = 2;
	
	if(ip_data_file.is_open()) {
	    while(ip_data_file.good()) {

		string f_name;
		
		ip_data_file>>f_name;
		build_ngram(f_name,_i);
		_i++;
	    }
	    ip_data_file.close();
	}
    }
    else if(flags[6]){
	// get the name of files with hash table
	//cout << "the name of files with hash table\n";
	ifstream ip_data_file(file_names[6]);
	int _i = 2;
	
	if(ip_data_file.is_open()) {
	    while(ip_data_file.good()) {

		string f_name;
		
		ip_data_file>>f_name;
		load_serialized_ngram(f_name,_i);
		_i++;
	    }
	    ip_data_file.close();
	}
    }
    
    if(flags[7]){
	//get names of files where to dump to
	//cout << "names of files where to dump to\n";
	ifstream ip_data_file(file_names[7]);
	int _i = 2;
	
	if(ip_data_file.is_open()) {
	    while(ip_data_file.good()) {

		string f_name;
		
		ip_data_file>>f_name;
		serialize_ngram(f_name,_i);
		_i++;
	    }
	    ip_data_file.close();
	}
    }
    
    phrase_corrector();
    exit (0);
}

void get_stringip(char* var,int len){
    char c;
    c = getchar(); // safety ops!!
    int idx = 0;
    while(idx < len-1){
	c = getchar();
	if(c != '\n'){
	    var[idx] = c;
	}
	else{
	    var[idx] = 0;
	    break;
	}
	idx++;
    }
    var[idx] = 0;
}

void phrase_corrector(){
    
    cout<<"enter the number strings and enter each string:\n";
    int N;// number of inputs
    cin >> N;
    
    while(N--){
      
	//the input
	char phrase[2048];// = {0};// original string
	string str_phrase;
	//cin.read(phrase,2047);// reading into a fixed buffer
	//cin >> phrase; - this fails
	cout << "New word :: ";
	//cin.clear(); cin.ignore(INT_MAX,'\n'); //clearing the input buffers!!
	
	//getline(cin,str_phrase);
	//strncpy(phrase, str_phrase.c_str(), sizeof(phrase));
	//phrase[sizeof(phrase) - 1] = 0;
	get_stringip(phrase,2048);
	
	if(strlen(phrase) == 0){
	    cout << "Empty word\n";
	    continue;
	}
	
	cout << " : ";
	
	vector<string> tok_phrase;// tokenized by space strings
	
	//tokenize
	char *ind_tok = strtok(phrase, " ");//getting tokens
	do{
	    if(ind_tok != NULL){
		tok_phrase.push_back(string(ind_tok));
	    }
	    ind_tok = strtok(NULL, " ");
	    
	}while (ind_tok != NULL);
	
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
		  
		found_wrong = true;
		err_index = ix;
	      
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
	if(found_wrong){
	    int idx;
	    for(idx = 0;idx < possibs.size();idx++){
		tok_phrase.at(err_index) = possibs.at(idx);
		
		//generate n grams and get score
		scores.push_back(generate_score(tok_phrase,err_index));
	    }
	    
	    int max_idx = get_max_element<double>(scores);//gets one with highest score
	    
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
	else{
	    cout << "Correct phrase\n";
	}
    }
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

// DUMMY no use
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
    double score = 1.0;
    
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
    else {
	return -1E6;
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

void load_serialized_ngram(string fname,int ngram){
    ifstream map_file(fname);
    
    if(map_file.is_open()) {
        while(map_file.good()) {

            string name;
            int count_word;

            map_file>>count_word;
	    map_file>>name;
	    
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