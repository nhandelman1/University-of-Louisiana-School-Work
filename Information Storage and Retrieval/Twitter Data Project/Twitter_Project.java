package twitter_project;

import java.sql.*;
import java.util.*;
import java.io.*;
/**
 * @author Nick Handelman
 * 
 */
public class Twitter_Project 
{
    final static double DICE_COEFF_THRESHOLD = 0.5;
    final static int NORMALITY_THRESHOLD = 10;
    final static int NUM_HASHTAGS = 10000;
    final static int NUM_TWEETS = 100000;
    
    /**
     * main class
     * @param String[] args the command line arguments
     */
    public static void main(String[] args)
    {   
        Connection conn = dbConnect("jdbc:mysql://localhost:3306/Twitter_DB", "root", "");
        HashSet<HashtagEntity> entities = new HashSet<HashtagEntity>();
        Statement stmt;
        String query;
        ResultSet rs;
        HashtagEntity temp;
        Iterator itr;
        int numEntities = 0;
        int[][] associations; //holds association values

        
        try //form entities in this try block
        {
            stmt = conn.createStatement();
            query = "select * from hashtags limit 0," + NUM_HASHTAGS; //get all hash tags from database table hashtags
            rs = stmt.executeQuery(query);
            
            //determine associations (similar hashtags) within entities
            while(rs.next()) //for each entry in the result set (each hashtag)
            {
                itr = entities.iterator(); //entity iterator
                boolean htAdded = false;
                String curTag = rs.getString(2);
                int curID = rs.getInt(1);
                double maxDiceCoeff = 0;
                HashtagEntity maxEntity = null;
                
                //determine if hashtag belongs to an entity
                while(itr.hasNext() && !htAdded) //for each entity until hashtag found
                {
                    temp = (HashtagEntity)itr.next(); //get next HashtagEntity
                    if(temp.containsTag(curTag))
                    {
                        temp.addID(new Integer(curID)); //add hashtag id, set handles duplicates
                        htAdded = true;
                    }
                }//end while

                //if hashtag does not belong to an entity
                if(!htAdded) 
                {
                    double diceCoeff = 0;
                    itr = entities.iterator(); //entity iterator
                    while(itr.hasNext()) //for each entity
                    {
                        temp = (HashtagEntity)itr.next(); //get entity
                        diceCoeff = diceCoefficient(temp.getDescription(), curTag); //calculate dice coefficient
                        if(diceCoeff > maxDiceCoeff) //if dice coefficient is greater than the maximum computed thus far
                        {
                            maxDiceCoeff = diceCoeff; //dice coefficient becomes the maximum
                            maxEntity = temp; //reference to the best matching entity
                        }//end if
                    }//end while
                    
                    if(maxDiceCoeff > DICE_COEFF_THRESHOLD)
                    {
                        maxEntity.addFullTag(curTag, new Integer(curID));
                        htAdded = true;
                    }//end if
                }//end if
                
                //if hashtag does not fit any entity above dice_coeff_threshold
                if(!htAdded)
                {
                    HashtagEntity newEnt = new HashtagEntity(curTag, numEntities); //create new entity with description of the current tag and a final int id
                    newEnt.addFullTag(curTag, new Integer(curID)); //add the hashtag text and id to the new entity
                    entities.add(newEnt); //add new entity to the set of entities
                    numEntities++;
                }//end if
            }//end while
        }//end try
        catch(Exception e)
        {
            e.printStackTrace();
        }//end catch            
        System.out.println(numEntities);
        associations = new int[numEntities][numEntities]; //initialize 2d array for tracking associations among entities
           
        try //form associations among entities in this try block
        {
            stmt = conn.createStatement();
            query = "select * from tweet_hashtags order by TweetId_FKey limit 0," + NUM_TWEETS; //get table that connects hashtags to tweets
            rs = stmt.executeQuery(query);
            
            //determine associations among entities
            ArrayList<Integer> htID, entityID;
            int curTweet = 0;
            boolean found = false;
            
            while(rs.next()) //for each entry in the result set
            { 
                curTweet = rs.getInt(1); //current tweet id
                htID = new ArrayList<Integer>(); //set of hashtag id's for hashtags found in current tweet
                while(rs.next() && curTweet == rs.getInt(1)) //for all hashtags found in current tweet
                {
                    htID.add(new Integer(rs.getInt(2))); //add hashtag id to set
                }//end while
                rs.previous(); //move cursor back one row

                entityID = new ArrayList<Integer>(); //set of entity id's corresponding to hashtag id's
                for(Integer i: htID) //for each hashtag id in set htID
                {
                    found = false;
                    itr = entities.iterator(); //entity iterator
                    while(itr.hasNext() && !found)
                    {
                        temp = (HashtagEntity)itr.next(); //get next HashtagEntity
                        if(temp.containsID(i)) //if this entity contains the id
                        {
                            entityID.add(temp.getIndexID());
                            found = true;
                        }//end if
                    }//end while
                }//end for
                
                for(Integer i: entityID) //increment associations
                    for(Integer j: entityID)
                        associations[i.intValue()][j.intValue()]++;
            }//end while
        }//end try
        catch(Exception ex){}
        
        //print entities and their associations
        print(entities, associations);
    }//end main
    
    /**
     * Create connection to database
     * 
     * @param String dbURL
     * @param String dbUserID
     * @param String dbPassword
     * @return Connection conn or null
     */
    public static Connection dbConnect(String dbURL, String dbUserID, String dbPassword)
    {
        try
        {
            Class.forName("com.mysql.jdbc.Driver").newInstance();
            Connection conn = DriverManager.getConnection(dbURL, dbUserID, dbPassword);
       
            System.out.println("connected");
            System.out.println();
            return conn;          
        }//end try
        catch (Exception e)
        {
            e.printStackTrace();
            return null;
        }//end catch
    }//end dbConnect
    
    /**
     * From: http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Dice's_coefficient
     * I modified the algorithm to be case insensitive by converting all lowercase characters to uppercase
     * 
     * @param String s1
     * @param String s2
     * @return double (value between 0 and 1)
     */
    public static double diceCoefficient(String s1, String s2)
    {
            Set<String> nx = new HashSet<String>();
            Set<String> ny = new HashSet<String>();

            for (int i=0; i < s1.length()-1; i++) //form character pairs for string s1
            {                                     
                    char x1 = convertUpperCase(s1.charAt(i));
                    char x2 = convertUpperCase(s1.charAt(i+1));
                    String tmp = "" + x1 + x2;
                    nx.add(tmp);
            }//end for
            
            for (int j=0; j < s2.length()-1; j++) //form character pairs for string s2
            {
                    char y1 = convertUpperCase(s2.charAt(j));
                    char y2 = convertUpperCase(s2.charAt(j+1));
                    String tmp = "" + y1 + y2;
                    ny.add(tmp);
            }//end for

            Set<String> intersection = new HashSet<String>(nx);
            intersection.retainAll(ny); //keep only character pairs that are found in both strings
            double totcombigrams = intersection.size();

            return (2*totcombigrams) / (nx.size()+ny.size());
    }//end diceCoefficient
    
    /**
     * Convert lowercase characters to uppercase characters
     * 
     * @param char x
     * @return char x
     */
    public static char convertUpperCase(char x)
    {
        if(x >= 97 && x <= 122) //97 = 'a', 122 = 'z'
            x -= 32;            //65 = 'A', 90 = 'Z'
        
        return x;
    }//end convertUpperCase
    
    /**
     * Print entities and table of associations
     * 
     * @param HashSet<HashtagEntity> ents
     * @param int[][] assctns 
     */
    public static void print(HashSet<HashtagEntity> ents, int[][] assctns)
    {
        String s = "";
        boolean print = false, printTags = true;
        BufferedWriter bw;
        
        //print and write to file all entities and their ids
        try
        {
            bw = new BufferedWriter(new FileWriter("Entity Results.txt"));
            bw.write("Number of Hashtags Considered: " + NUM_HASHTAGS);
            bw.newLine();
            bw.write("Number of Tweets Considered: " + NUM_TWEETS);
            bw.newLine();
            bw.newLine();
            for(HashtagEntity e: ents)
            {
                //print to console
                e.print(printTags); //if false, print entity description and unique ID
                                    //if true, also print associated hashTags
                
                //write to file
                bw.write("Entity Description: " + e.getDescription() + ", Entity ID: " + e.getIndexID() + " ");
                bw.newLine();
                bw.write("     ");
                if(printTags)
                {
                    HashSet<String> t = e.getSetOfTags();
                    for(String str: t)
                    {
                        bw.write(str + ", ");
                    }//end for
                }//end if
                bw.newLine();
            }//end for
            
            bw.close();
        }//end try
        catch(Exception e){}
       
        System.out.println();
        System.out.println();
        
        //print and write to file entity association values
        try
        {
            bw = new BufferedWriter(new FileWriter("Association Results.txt"));
            
            bw.write("Number of Hashtags Considered: " + NUM_HASHTAGS);
            bw.newLine();
            bw.write("Number of Tweets Considered: " + NUM_TWEETS);
            bw.newLine();
            bw.newLine();
            
            //print associations table
            for(int i = 0; i < assctns.length; i++) //for each entity
            {
                print = false;
                s = i + ": ";
                for(int j = 0; j < assctns[i].length; j++) //for each associated entity
                {
                    if(i != j && assctns[i][j] != 0) //if the entity and associated entity are different
                    {                                //and have a nonzero association value
                        s += j + ":" + assctns[i][j] + ", ";
                        print = true;
                    }//end if
                }//end for

                if(print) //only print if association value is nonzero 
                {
                    System.out.println(s);//print to console
                    bw.write(s); //write to file
                    bw.newLine();
                }//end if
                bw.newLine();
            }//end for
            
            bw.close();
        }//end try
        catch(Exception e){}
    }//end print
    
}//end class Twitter_Project