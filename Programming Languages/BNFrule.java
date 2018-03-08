package compilerproject;
import java.util.*;

public class BNFrule 
{
    private String lhs;
    private List<String> rhsSymbols;
    
////////////////////////////////////////////////////////////////////////////////
//Constructors                                                                //
////////////////////////////////////////////////////////////////////////////////
    public BNFrule()
    {
        lhs = "";
        rhsSymbols = new ArrayList<String>();
    }//end BNFrule
    
    public BNFrule(String lhs)
    {
        this.lhs = lhs;
        rhsSymbols = new ArrayList<String>();
    }//end BNFrule
    
////////////////////////////////////////////////////////////////////////////////
//Setters and add methods                                                     //
////////////////////////////////////////////////////////////////////////////////
    public void setLHS(String lhs)
    {
        this.lhs = lhs;
    }//end setLHS
    
    public void addRHSSym(String s)
    {
        rhsSymbols.add(s);
    }//end addRHSSym
    
////////////////////////////////////////////////////////////////////////////////
//Getters                                                                     //
////////////////////////////////////////////////////////////////////////////////
    public String getLHS()
    {
        return lhs;
    }//end getLHS
    
    public List<String> getRHS()
    {
        return rhsSymbols;
    }//end getRHS
    
////////////////////////////////////////////////////////////////////////////////
//Print rules                                                                 //
////////////////////////////////////////////////////////////////////////////////
    public void printRule()
    {
        System.out.print(lhs + " ::= ");
        for( String s : rhsSymbols)
            System.out.print(s + " ");
        System.out.println();
    }//end printRule
}//end BNFrule