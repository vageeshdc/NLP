/*
 * 
 * Sample code to show how to use the Stanford NLP's POS Tagger
 * The .jar is inside "config_files"
 * The .prop and .tagger files are in "taggers"
 * 
 */

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;

import edu.stanford.nlp.tagger.maxent.MaxentTagger;

public class tagText{
	
	public static void main(String[] args) throws IOException, ClassNotFoundException {
	 
		// Initialize the tagger
		MaxentTagger tagger = new MaxentTagger("taggers/left3words-wsj-0-18.tagger");
	
		Scanner in= new Scanner(new File("wikipedia2text.txt"));
		
		while(in.hasNextLine()){
			// The tagged string
			String sample = in.nextLine();
			String[] toks = sample.split("[\\.\\!\\?]");
			getSubidx(tagger,toks,0);
			getSubidx(tagger,toks,1);
		}
		
	}
	
	public static void getSubidx(MaxentTagger tagger,String[] toks,int idx){
		for(int i = 0;i < toks.length;i++){
			String tagged = tagger.tagString(toks[i]);
			//output the tagged sample string onto your console
			String[] sub_tok = tagged.split("\\s");
			for(int j = 0;j < sub_tok.length;j++){
				String vals[] = sub_tok[j].split("/");
				if(vals.length == 2){
					System.out.print(vals[idx]+" ");
				}
			}
			
		}
		System.out.println();
	}
}
