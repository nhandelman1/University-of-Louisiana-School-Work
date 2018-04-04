/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package twitter_project;
import java.util.*;

/**
 *
 * @author Nick
 */
public class HashtagEntity 
{
    private String description; //description of entity
    private final int indexID;
    private HashSet<String> hashTags; //set of hashtags included in this entity
    private HashSet<Integer> hashIDs; //set of hashtag id's included in this entity

    HashtagEntity()
    {
        indexID = 0;
        description = "";
        hashTags = new HashSet<String>();
        hashIDs = new HashSet<Integer>();
    }//end default constructor
    
    HashtagEntity(String desc, int id)
    {
        description = desc;
        indexID = id;
        hashTags = new HashSet<String>();
        hashIDs = new HashSet<Integer>();
    }//end parameterized constructor
    
    public void setDescription(String desc)
    {
        description = desc;
    }//end setDescription
    
    public String getDescription()
    {
        return description;
    }//end getDescription
    
    public HashSet<String> getSetOfTags()
    {
        return hashTags;
    }//end getListOfTags
    
    public HashSet<Integer> getSetOfIDs()
    {
        return hashIDs;
    }//end getSetOfIDs
    
    public int getIndexID()
    {
        return indexID;
    }//end getIndexID
    
    public boolean addID(Integer id)
    {
        return hashIDs.add(id);
    }//end addID
    
    public boolean addFullTag(String tag, Integer id)
    {   
        boolean added = false;
        
        added = hashTags.add(tag); //returns false if id already in set
        added = hashIDs.add(id); //return false if id already in set
        
        return added;
    }//end addTag
    
    public boolean containsTag(String tag)
    {
        return hashTags.contains(tag);
    }//end containsTag
    
    public boolean containsID(Integer id)
    {
        return hashIDs.contains(id);
    }//end containsID
    
    public void print(boolean includeHTs)
    {
        System.out.println("Entity description: " + description + "   Entity ID: " + indexID);
        if(includeHTs)
        {
            for(String s: hashTags)
                System.out.print(s + " ");
            System.out.println();
           
        }//end if
    }

}//end class
