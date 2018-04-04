package pkg_Assign1_Q3;
import java.util.*;
import java.io.*;

public class Main 
{

    final static int NUM_DOCS = 4;
    final static int NUM_TERMS = 13;
    
public static void main(String[] args)
{
    //map terms to integers, map integers to terms
    Map<String,Integer> term_int_map = new HashMap<String,Integer>();
    Map<Integer,String> int_term_map = new HashMap<Integer,String>(); 
    int td_freq[][] = new int[NUM_DOCS][NUM_TERMS]; //term-doc frequency table
    float td_norm_freq[][] = new float[NUM_DOCS][NUM_TERMS]; 
                                    //^term-document normalized frequency table
    
    //build map of expected terms, used for indexing 2d array
    buildMap(term_int_map, int_term_map);
    
    //initialize arrays
    for(int r = 0; r < NUM_DOCS; r++)
        for(int c = 0; c < NUM_TERMS; c++)
        {
            td_freq[r][c] = 0;
            td_norm_freq[r][c] = 0;
        }
    
    //read data into 2d arrays for all documents
    build2darrays(td_freq, td_norm_freq, term_int_map);
    
    //compute RSV values for an input query
    computeRSVvalues(td_norm_freq, int_term_map);
    
    
    //output inverted index
    printInvertedIndex(td_freq, int_term_map);
}//end main

static void buildMap(Map<String,Integer> tm1, Map<Integer, String> tm2)
{
    tm1.put("algorithm", new Integer(0));
    tm1.put("comput", new Integer(1));
    tm1.put("data", new Integer(2));
    tm1.put("field", new Integer(3));
    tm1.put("inform", new Integer(4));
    tm1.put("java", new Integer(5));
    tm1.put("languag", new Integer(6));
    tm1.put("mine", new Integer(7));
    tm1.put("popular", new Integer(8));
    tm1.put("retriev", new Integer(9));
    tm1.put("scienc", new Integer(10));
    tm1.put("softwar", new Integer(11));
    tm1.put("written", new Integer(12));
    
    tm2.put(new Integer(0), "algorithm" );
    tm2.put(new Integer(1), "comput" );
    tm2.put(new Integer(2), "data" );
    tm2.put(new Integer(3), "field");
    tm2.put(new Integer(4), "inform");
    tm2.put(new Integer(5), "java");
    tm2.put(new Integer(6), "languag");
    tm2.put(new Integer(7), "mine");
    tm2.put(new Integer(8), "popular");
    tm2.put(new Integer(9), "retriev");
    tm2.put(new Integer(10), "scienc");
    tm2.put(new Integer(11), "softwar");
    tm2.put(new Integer(12), "written");
    
}//end buildMap

static void build2darrays(int array1[][], float array2[][], 
        Map<String, Integer> mp)
{
    for(int x = 0; x < NUM_DOCS; x++)
    {   
        BufferedReader br;
        String l = "", doc = "", term = "";
        String[] line;
        int freq = 0;
        float norm_freq = 0;
        
        //build document name
        doc = "DocVector_d";
        doc = doc.concat(String.valueOf(x+1));
        doc = doc.concat(".txt");

        try
        {
            br = new BufferedReader(new FileReader(doc));
            l = br.readLine(); //read first line
            
            while(br.ready()) //read consecutive lines
            {
                l = br.readLine(); 
                line = l.split(" "); //split l into 3 strings
                term = line[0]; //get term
                freq = Integer.valueOf(line[1]); //get freq 
                norm_freq = Float.valueOf(line[2]); //get norm_freq
                
                //store data into 2d arrays
                array1[x][mp.get(term).intValue()] = freq;
                array2[x][mp.get(term).intValue()] = norm_freq;
            }
        }//end try
        catch(FileNotFoundException e)
        {
            System.err.println("ERROR: File '" + doc + "' not found.");
            System.exit(0);  
        }//end catch
        catch(IOException e)
        {
        System.err.println("ERROR: File '" + doc + "' could not be processed.");
            System.exit(0);
        }//end catch
    }//end for
}//end build2darrays

static void print2darray(float array1[][])
{
    for(int r = 0; r < NUM_DOCS; r++)
    {
        for(int c = 0; c < NUM_TERMS; c++)
            System.out.print( array1[r][c] + " ");
        System.out.println();
    }
    System.out.println();
}//end print2darrays

static void computeRSVvalues(float array1[][], Map<Integer, String> mp)
{
    Scanner kb = new Scanner(System.in);
    float query[] = new float[13];
    float RSV[] = new float[4];
    float dot_prod = 0, doc_norm = 0, query_norm;
    
    //get term weights for query
    for(int i = 0; i < NUM_TERMS; i++) 
    {
        System.out.print("Enter a query value for the term '" + 
                mp.get(new Integer(i)) + "': ");
        query[i] = kb.nextFloat();
    }

    //compute RSV values
    query_norm = computeNorm(query); //calculate norm of the query one time
    for(int i = 0; i < NUM_DOCS; i++)
    {
        dot_prod = computeDotProduct(array1[i], query);
        doc_norm = computeNorm(array1[i]);
        RSV[i] = dot_prod/(doc_norm*query_norm);
    }
    
    //output RSV values for each document
    System.out.println("\nRSV values for each document:");
    for(int i = 0; i < 4; i++)
        System.out.println("Document " + (i+1) + ": " + RSV[i]);
    System.out.println();
}//end computeRSVvalues

static float computeDotProduct(float d[], float q[])
{
    float result = 0;
    
    for(int i = 0; i < NUM_TERMS; i++)
        result += d[i]*q[i];
    
    return result;
}//end computeDotProduct

static float computeNorm(float v[])
{
    float result = 0;
    
    for(int i = 0; i < NUM_TERMS; i++)
        result += v[i]*v[i];
    
    result = (float)Math.sqrt(result);
    
    return result;
}//end computeNorm

static void printInvertedIndex(int array1[][], Map<Integer, String> mp1)
{
    //use int to term map to list keywords in alphabetical order
    //use term frequency 2d array to list term frequency for each document
    System.out.println("Inverted Index:");
    for(int t = 0; t < NUM_TERMS; t++)
    {
        System.out.printf("%9s", mp1.get(new Integer(t))); //print keyword
        for(int d = 0; d < NUM_DOCS; d++)
            if(array1[d][t] != 0.0) //if term is in the document
                System.out.print(" " + (d+1) + ":" + array1[d][t]); 
        System.out.println();                              //^print docid:freq
    }
}//end printInvertedIndex
}//end class main

