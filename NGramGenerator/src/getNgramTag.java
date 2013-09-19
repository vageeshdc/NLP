/*
 * 
 * Code to generate N-Grams and their frequencies, given the corpus.
 * Author: C.Eshwar,Vageesh D C
 * 
 */

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;

public class getNgramTag {

	/**
	 * @param args
	 * @throws FileNotFoundException
	 * @throws RTRException
	 */

	int N = 2;
	public Map<String, Integer> keyword2count = new HashMap<String, Integer>();
	public Map<String, Integer> keyword_pos2count = new HashMap<String, Integer>();
	public Map<String, String> key2key = new HashMap<String, String>();

	Set<String> words = new HashSet<String>();
	Set<String> words_pos = new HashSet<String>();

	public ArrayList<String> emptyRmStrBuffer(LinkedList<String> previous, Map<String, Integer> map_data) {

		ArrayList<String> pushed_ngrams = new ArrayList<String>();
		String group = null;

		if (previous.size() == N) {
			// create copies
			// we have a N-Gram!
			String[] prev = {};
			prev = previous.toArray(prev);
			String tempgroup = "";

			int ind1;

			for (ind1 = 0; ind1 < N - 1; ind1++) {
				tempgroup += prev[ind1] + " ";

			}

			tempgroup += prev[ind1];
			group = tempgroup;
		}

		boolean add = words.add(group);

		if (add) {
			// implies that a new word has been added to the group
			int score = 1;
			map_data.put(group, score);
			pushed_ngrams.add(group);
		} else {
			int count = (int) map_data.get(group);
			count = count + 1;
			map_data.remove(group);
			map_data.put(group, count);
		}

		previous.remove();
		return pushed_ngrams;
	}

	public int min(int a,int b){
		if(a >b){
			return b;
		}
		else{
			return a;
		}
	}
	
	public ArrayList<String> putToks(LinkedList<String> previous,
			String[] tmp_toks, Map<String, Integer> map_data) {
		// while(matcher.find()){

		ArrayList<String> pushed_ngrams = new ArrayList<String>();

		for (int i = 0; i < tmp_toks.length; i++) {

			String group = tmp_toks[i];// matcher.group().toLowerCase();;
			// group = group.substring(0, group.length()-1);
			String groupcopy = "";
			
			if(group != null){
				
				if (previous.size() < N) {
					previous.add(group);
					continue;
				}
	
				else if (previous.size() == N) {
					// create copies
					groupcopy = group;
					// we have a N-Gram!
					String[] prev = {};
					prev = previous.toArray(prev);
	
					// create the N gram into a string with text in alphabetical
					// order separated by &
	
					String tempgroup = "";
	
					int ind1;
	
					for (ind1 = 0; ind1 < N - 1; ind1++) {
						tempgroup += prev[ind1] + " ";
	
					}
	
					tempgroup += prev[ind1];
					group = tempgroup;
				}
	
				boolean add = words.add(group);
	
				if (add) {
					// implies that a new word has been added to the group
					int score = 1;
					map_data.put(group, score);
					pushed_ngrams.add(group);
				} else {
					
					int count;
					
					try{
					count = (int) map_data.get(group);
					count = count + 1;
					map_data.remove(group);
					map_data.put(group, count);
					}
					catch(Exception e){}
				}
	
				// update the previous lists
				if (previous.size() == 1)
					continue;
	
				previous.remove();
	
				if (groupcopy == "")
					continue;
	
				previous.add(groupcopy);
			}
		}

		return pushed_ngrams;
	}

	public void generateNGrams() throws FileNotFoundException {
		// size of the N-gram; N = 0/1/2/3/4?
		// specify the Regex here!

		String inputfilename = "/home/sriganesha/eclipse/opfile1";
		String outputfilename = "config_files/distinctOutput.txt";

		FileOutputStream out = new FileOutputStream(outputfilename);

		// Pattern compile = Pattern.compile(pattern);

		Scanner in = new Scanner(new File(inputfilename));
		LinkedList<String> previous = new LinkedList<String>();
		LinkedList<String> previous_pos = new LinkedList<String>();
		
		ArrayList<String> tok_mapper = new ArrayList<String>();
		ArrayList<String> tok_mapper_pos = new ArrayList<String>();
		
		while (in.hasNextLine()) {

			String match = in.nextLine();
			String match_pos = in.nextLine();// this has the POS info pack it!!

			String[] tmp_toks = match.split("\\s");
			String[] tmp_toks_pos = match_pos.split("\\s");

			tok_mapper = putToks(previous, tmp_toks, keyword2count);
			tok_mapper_pos = putToks(previous_pos, tmp_toks_pos,keyword_pos2count);
			
			for (int ix_ = 0; ix_ < min(tok_mapper.size(),tok_mapper_pos.size()); ix_++) {
				key2key.put(tok_mapper.get(ix_), tok_mapper_pos.get(ix_));
			}
			tok_mapper.clear();
			tok_mapper_pos.clear();
		}
		
		tok_mapper = emptyRmStrBuffer(previous, keyword2count);
		tok_mapper_pos = emptyRmStrBuffer(previous_pos, keyword_pos2count);

		for (int ix = 0; ix < min(tok_mapper.size(),tok_mapper_pos.size()); ix++) {
			key2key.put(tok_mapper.get(ix), tok_mapper_pos.get(ix));
		}
		/*
		 * Entire Corpus has been gone through! keyword2count has the mapping
		 * between the N-Grams and their counts
		 */

		keyword2count = sortByValue(keyword2count);
		Set<String> temp1 = keyword2count.keySet();

		String[] temp = new String[temp1.size()];
		temp1.toArray(temp);

		int top_limit = 0;
		// int limit = 100;

		for (int l = 0; l < keyword2count.size(); l++) {

			top_limit = top_limit + 1;
			
			@SuppressWarnings("resource")
			PrintStream p=new PrintStream(out);

			String temp2 = temp[l];

			p.print(keyword2count.get(temp[l]) + " " );
			System.out.print(keyword2count.get(temp[l]) + " ");

			// Print the N-Gram
			String sam2 = temp2;

			p.println(sam2);
			System.out.print(sam2 + " ");

			p.println(key2key.get(temp[l]));
			System.out.println(key2key.get(temp[l]));
		}
	}

	public static void main(String[] args) throws FileNotFoundException,
	RuntimeException {
		// TODO Auto-generated method stub
		getNgramTag temp = new getNgramTag();
		temp.generateNGrams();
	}

	public static <K, V extends Comparable<? super V>> Map<K, V> sortByValue(
			Map<K, V> map) {
		List<Map.Entry<K, V>> list = new LinkedList<Map.Entry<K, V>>(
				map.entrySet());
		Collections.sort(list, new Comparator<Map.Entry<K, V>>() {
			public int compare(Map.Entry<K, V> o1, Map.Entry<K, V> o2) {
				return (o2.getValue()).compareTo(o1.getValue()); // for
				// descending
				// order
				// return (o1.getValue()).compareTo( o2.getValue() ); //for
				// ascending order
			}
		});

		Map<K, V> result = new LinkedHashMap<K, V>();
		for (Map.Entry<K, V> entry : list) {
			result.put(entry.getKey(), entry.getValue());
		}
		return result;
	}
}
