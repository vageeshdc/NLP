#ifndef HASH_TEST_H
#define HASH_TEST_H

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

using namespace std;

#define MAX_LEVEL 2
#define MIN_TRESH 1E-6
#define NO_WORD_LEVEL 3

#define REQ_TRESH 1E-6
#define MAX_CHOICE 2

extern double err_arr[4][27][26];

enum ERRORS {SUBS,ADD,DEL,SWAP};

struct error_pairs {

    char p1;
    char p2;
    ERRORS err_type;
};

extern unordered_map<string,int> word_list;

extern double var_val;
extern int tot_word_count;

extern vector<double> score_list;
extern vector<string> name_list;
extern vector<string> ranked_list;

typedef std::vector<int> int_vec_t;
typedef std::vector<std::string> str_vec_t;
typedef std::vector<size_t> index_vec_t;

class SequenceGen {
public:
    SequenceGen (int start = 0) : current(start) { }
    int operator() () {
        return current++;
    }
private:
    int current;
};

class Comp {
    vector<double>& _v;
public:
    Comp(vector<double>& v) : _v(v) {}
    bool operator()(size_t i, size_t j) {
        return _v[i] < _v[j];
    }
};

//ngram data
extern unordered_map<string,int> gram2_list;
extern unordered_map<string,int> gram3_list;
extern unordered_map<string,int> gram4_list;
extern unordered_map<string,int> gram5_list;

//functions
void setup_phase(int argc,char** argv);//sets up the data

double check_variants_and_comp(string name,error_pairs epair);
void generate_variants(string name,int level);
int hash_func(string word);
void solve_test_cases(string name);
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

void serialize_umap(string fname);
void build_umap(string fname);

// NGRAM functions
//static int callback(void *NotUsed, int argc, char **argv, char **azColName);
double generate_score(vector<string> new_phase,int err_index);
string gen_combined_toks(vector<string> new_phrase,int start_idx,int end_idx);
bool inrange(int start,int end,int q_idx);
double get_score(string sub_phrase,int num_tok);
template <class T> int get_max_element(vector<T> elems);
template <class T> int compare_vals (const void *a, const void *b);//for double value comparision
void build_ngram(string fname,int gram_val);
void load_serialized_ngram(string fname,int ngram);
void serialize_ngram(string fname,int ngram);
// the correction i/p here
void phrase_corrector();

#endif