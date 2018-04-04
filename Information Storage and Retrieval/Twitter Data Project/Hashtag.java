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
public class Hashtag 
{
    private String hashTag;
    private LinkedList<Integer> hashTagID;
    
    Hashtag()
    {
        hashTag = "";
        hashTagID = new LinkedList<Integer>();
    }
    
    Hashtag(String ht, Integer id)
    {
        hashTag = ht;
        hashTagID = new LinkedList<Integer>();
        hashTagID.add(id);
    }
    
    public void addID(Integer id)
    {
        hashTagID.add(id);
    }
    
    public boolean containsID(Integer id)
    {
        boolean found = false;
        
        if(hashTagID.contains(id))
            found = true;
        
        return found;
    }
}//end class
