/*
 * 
 * Code to generate N-Grams and their frequencies, given the corpus.
 * Author: C.Eshwar
 * 
 */

import java.io.File; 
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;
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
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class getNgram {

	/**
	 * @param args
	 * @throws FileNotFoundException 
	 * @throws RTRException 
	 */
	public Map<String, Integer> keyword2count = new HashMap<String , Integer>();
	int N = 2;		//size of the N-gram; N = 0/1/2/3/4?

	public void generateNGrams() throws FileNotFoundException
	{
		Set<String> words = new HashSet<String>();
		//specify the Regex here!
		String pattern = "[a-zA-Z0-9]+\\s";
		
		String match = "hello world how* are. you! ! world hello";
		
		String inputfilename = "wikipedia2text.txt";	
		String outputfilename = "config_files/distinctOutput.txt";	
		
		FileOutputStream out=new FileOutputStream(outputfilename);
		
		Pattern compile = Pattern.compile(pattern);
		
		Scanner in= new Scanner(new File(inputfilename));
	
		while(in.hasNextLine())
		{
			LinkedList<String> previous = new LinkedList<String>();
			
			match = in.nextLine();
			//String tmp = in.nextLine();//this has the POS info pack it!!
			
			Matcher matcher = compile.matcher(match);
			
			while(matcher.find()){
						
				String group = matcher.group().toLowerCase();;					
				group = group.substring(0, group.length()-1);
				String groupcopy = "";
				
				if(previous.contains(group))
				{
					//don't allow duplicates within the N gram
					continue;
				}
				else if(previous.size() < N)
				{
					//list is empty
					previous.add(group);
					continue;
				}
				
				else if(previous.size() == N)
				{
					//create copies
					groupcopy = group;
					//we have a N-Gram!
					String[] prev = {}; 
					prev = previous.toArray(prev);
					
					//create the N gram into a string with text in alphabetical order separated by &
					
					String tempgroup = "";
			
					int ind1;
					
					for(ind1 = 0; ind1 < N -1; ind1++)
					{
						tempgroup += prev[ind1] + "&";
			
					}
					
					tempgroup += prev[ind1];
					group = tempgroup;
				}
			
				boolean	add = words.add(group);
				
				if(add)
				{
					// implies that a new word has been added to the group
					int score = 1;
					keyword2count.put(group,score);
				}
				else
				{
					int count = (int)keyword2count.get(group);
					count = count + 1;
					keyword2count.remove(group);	
					keyword2count.put(group, count);
				}
				
				//update the previous lists
				if(previous.size() == 1)
					continue;
				
				previous.remove();
			    
				if(groupcopy == "")
					continue;
				
				previous.add(groupcopy);	
			}
		}
		
		/*
		 * Entire Corpus has been gone through! 
		 * keyword2count has the mapping between the N-Grams and their counts
		*/
	
		keyword2count = sortByValue(keyword2count);
		Set<String> temp1 = keyword2count.keySet();
		
		String[] temp = new String[temp1.size()];
		temp1.toArray(temp);
		
		int top_limit = 0;
		//int limit = 100;
		
		for(int l=0; l<keyword2count.size(); l ++)
		{	
			
			top_limit = top_limit + 1;
			
			String temp2 = temp[l];
			
			@SuppressWarnings("resource")
			PrintStream p=new PrintStream(out);
			
			//Write the frequency 
			p.print(keyword2count.get(temp[l]) + " " );
			System.out.print(keyword2count.get(temp[l]) + " ");
			
			int index = temp2.indexOf('&');
			
			//Print the N-Gram
			String sam1 = "",sam2 = temp2;
			
			while(index != -1)
			{
				sam1 = sam2.substring(0,index);
				sam2 = sam2.substring(index + 1);
				
				p.print(sam1 + " ");
				System.out.print(sam1 + " ");
				
				index = sam2.indexOf('&');
			}
			p.println(sam2);
			System.out.println(sam2);

		}
	}
	
	public static void main(String[] args) throws FileNotFoundException, RuntimeException {
		// TODO Auto-generated method stub
		getNgram temp = new getNgram();
		temp.generateNGrams();
	}
	
	   public static <K, V extends Comparable<? super V>> Map<K, V> 
	        sortByValue( Map<K, V> map )
	    {
	        List<Map.Entry<K, V>> list =
	            new LinkedList<Map.Entry<K, V>>( map.entrySet() );
	        Collections.sort( list, new Comparator<Map.Entry<K, V>>()
	        {
	            public int compare( Map.Entry<K, V> o1, Map.Entry<K, V> o2 )
	            {
	                return (o2.getValue()).compareTo( o1.getValue() ); 		//for descending order
	                //return (o1.getValue()).compareTo( o2.getValue() );	//for ascending order
	            }
	        } );

	        Map<K, V> result = new LinkedHashMap<K, V>();
	        for (Map.Entry<K, V> entry : list)
	        {
	            result.put( entry.getKey(), entry.getValue() );
	        }
	        return result;
	    }
	}
