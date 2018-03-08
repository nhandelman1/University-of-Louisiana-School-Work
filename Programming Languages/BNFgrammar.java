package compilerproject;
import java.util.*;
import java.io.*;

public class BNFgrammar 
{
  private String startSymbol;
  private Set<BNFrule> rules;
  private Set<String> termSyms;
  private Set<String> nonTermSyms;
  private Map<String, Boolean> dLambda_tbl;
  private Map<String,Set<String>>firstSet_tbl;
  
////////////////////////////////////////////////////////////////////////////////
//Constructors                                                                //
////////////////////////////////////////////////////////////////////////////////
  public BNFgrammar()
  {
    startSymbol = "";
    rules = new HashSet<BNFrule>();
    termSyms = new HashSet<String>();
    nonTermSyms = new HashSet<String>();
    dLambda_tbl = new HashMap<String, Boolean>();
    firstSet_tbl = new HashMap<String, Set<String>>();
  }//end BNFgrammar
  
  public BNFgrammar(String s)
  {
    startSymbol = s;
    rules = new HashSet<BNFrule>();
    termSyms = new HashSet<String>();
    nonTermSyms = new HashSet<String>();
    dLambda_tbl = new HashMap<String, Boolean>();
    firstSet_tbl = new HashMap<String, Set<String>>();
  }//end BNFgrammar
    
////////////////////////////////////////////////////////////////////////////////
//Setters and Add methods                                                     //
////////////////////////////////////////////////////////////////////////////////
  public void setStartSym(String s)
  {
    startSymbol = s;
  }//end setStartSymbol
    
  public void addRule(BNFrule r)
  {
    rules.add(r);
  }//end addRule
    
  public void addTermSym(String s)
  {
    termSyms.add(s);
  }//end addTerminalSymbol
    
  public void addNonTermSym(String s)
  {
    nonTermSyms.add(s);
  }//end addNonTermSym

////////////////////////////////////////////////////////////////////////////////
//Getters                                                                     //
////////////////////////////////////////////////////////////////////////////////
  public String getStartSym()
  {
    return startSymbol;
  }//end getStartSym
    
  public Set<BNFrule> getRules()
  {
    return rules;
  }//end getRules
    
  public Set<String> getTermSyms()
  {
    return termSyms;
  }//end getRules
        
  public Set<String> getNonTermSyms()
  {
    return nonTermSyms;
  }//end getRules

////////////////////////////////////////////////////////////////////////////////
//Analyze and validate EBNF input grammar                                     //
////////////////////////////////////////////////////////////////////////////////
  public void analyzeEBNFRule(String[] tokens, int lineNum) 
          throws GrammarException
  {
    BNFrule r = new BNFrule(tokens[0]); //initialize rule with lhs
    
    for(int i = 2; i < tokens.length; i++) //elements of rule rhs
    {
      if(!validRHSToken(tokens[i]))
        throw new GrammarException("ERROR: Incorrect rhs line: " + lineNum + 
                                   ", token: " + (i+1));
      r.addRHSSym(tokens[i]);
    }//end for
    rules.add(r);
  }//end analyzeEBNF
  
  public boolean validRHSToken(String token)
  {
    boolean valid = false;
    
    //check to make sure the token is valid
    if( token.contentEquals("{") || token.contentEquals("}") )
      valid = true;
    else if( termSyms.contains(token) ) //valid terminal symbol
      valid = true;
    else if( nonTermSyms.contains(token) )//valid non terminal symbol
      valid = true;
    
    return valid;
  }//end validRHS
  
  public boolean validNonTerm(String s)
  {
    boolean valid = true;
    
    if( s.length() <= 2 ) //string not long enough
      valid = false;
    else if( s.charAt(0) != '<' ) //non terminal symbol starts improperly
      valid = false;
    else if( s.charAt(s.length()-1) != '>')//non terminal symbol ends improperly
      valid = false;
    else //characters within the < and > range from A-Z or a-z
    {
      for(int i = 1; i < s.length()-1; i++)
      { //if not between A-Z or between a-z
        if(!( (s.charAt(i) >= 'A' && s.charAt(i) <= 'Z') || //between A-Z OR
              (s.charAt(i) >= 'a' && s.charAt(i) <= 'z') )) //between a-z
        {
          valid = false;
          break;
        }//end if
      }//end for
    }//end else
      
    return valid;
  }//end validNonTerm
  
////////////////////////////////////////////////////////////////////////////////
//Create table of first sets                                                  //
////////////////////////////////////////////////////////////////////////////////
  public void initializeTables()
  {
    //initialize derives lambda map to false for all symbols: terminal and non
    for( String sym : nonTermSyms )                                 //terminal
      dLambda_tbl.put(sym, Boolean.valueOf(false));
    for( String sym : termSyms )
      dLambda_tbl.put(sym, Boolean.valueOf(false));
    
    //initialize first set table
    for( String nonTerm : nonTermSyms )
      firstSet_tbl.put(nonTerm, new HashSet<String>());
    for( String term : termSyms )
      firstSet_tbl.put(term, new HashSet<String>());
  }//end initializeTables
  
  public void markLambda()
  { 
    //complete setup of derives lambda map to indicate whether a non-terminal
    //of type string can derive lambda
    boolean changes = false;
    do
    {
      changes = false;
      for( BNFrule rule : rules )
      {
        if( dLambda_tbl.get(rule.getLHS()).booleanValue() == false )
        {
          if( rule.getRHS().isEmpty())
          {
            changes = true;
            dLambda_tbl.put(rule.getLHS(), Boolean.valueOf(true));
            continue;
          }//end if
          boolean rhsDerivesLambda = true;
          for( String sym : rule.getRHS() )
          {
            rhsDerivesLambda &= dLambda_tbl.get(sym).booleanValue();
            if( !rhsDerivesLambda )
              break;
          }//end for
          if( rhsDerivesLambda )
          {
            changes = true;
            dLambda_tbl.put(rule.getLHS(), Boolean.valueOf(true));
          }//end if
        }//end if
      }//end for
    }while(changes);
  }//end MarkLambda
  
  public void fillFirstSet()
  { 
    //first loop initializes non terminal symbols to null or "lambda" in 
    //firstSet_tbl
    for( String nonTerm : nonTermSyms )
      if( dLambda_tbl.get(nonTerm).booleanValue() )
        firstSet_tbl.get(nonTerm).add("lambda");
    printFirstSets(1);
    
    //second loop processes terminal symbols
    for( String term : termSyms )
    {
      firstSet_tbl.get(term).add(term);
      for( BNFrule rule : rules )
        if( !rule.getRHS().isEmpty() && rule.getRHS().get(0).equals(term) )
          firstSet_tbl.get(rule.getLHS()).add(term);
    }//end for
    printFirstSets(2);
      
    //third loop initializes first sets and updates them until quiescence
    boolean changes = false;
    do
    {
      changes = false;
      for( BNFrule rule : rules)
      {
        Set<String> oldFirstSet = new HashSet<String>();
        for( String sym : firstSet_tbl.get(rule.getLHS()))
          oldFirstSet.add(sym);
        Set<String> changeSet = computeFirstRHS(rule.getRHS());
        Set<String> firstSet = firstSet_tbl.get(rule.getLHS());
        for( String sym : changeSet )
          firstSet.add(sym);
        for( String sym : firstSet )
          if( oldFirstSet.contains(sym) == false)
            changes = true;
      }//end for
      printFirstSets(3);
    }while(changes);
  }//end fillFirstSet
  
  public Set<String> computeFirstRHS(List<String> alpha)
  { 
      Set<String> firstSetAlpha = new HashSet<String>();
      if( alpha.isEmpty() )
        firstSetAlpha.add("lambda");
      else
      {
        for( String s : firstSet_tbl.get(alpha.get(0)) )
          firstSetAlpha.add(s);
        int i = 1;
        for(i = 1; i < alpha.size() && 
            firstSet_tbl.get(alpha.get(i-1)). contains("lambda"); i++)
        {
          Set<String> first_set = new HashSet<String>();
          for( String s : firstSet_tbl.get(alpha.get(i)))
            first_set.add(s);
          first_set.remove("lambda");
          for( String s : first_set)
            firstSetAlpha.add(s);
        }//end for
        if( (i == alpha.size()) && 
             firstSet_tbl.get(alpha.get(i-1)).contains("lambda"))
          firstSetAlpha.add("lambda");
      }//end else
      
      return firstSetAlpha;
  }//end computeFirstRHS

////////////////////////////////////////////////////////////////////////////////
//Display terminal symbols, non-terminal symbols, EBNF rules and BNF rules    //
//and first sets for the grammar                                              //
////////////////////////////////////////////////////////////////////////////////
  public void printTerm()
  {
    for( String t : termSyms )
      System.out.println(t);  
  }//end printTerm

  public void printNonTerm()
  {
    for( String t : nonTermSyms )
      System.out.println(t);
  }//end printNonTerm
  
  public void printRules()
  {
    for( BNFrule r : rules )
      r.printRule();
  }//end printRules
  
  public void printFirstSets(int loopNum)
  {
    //From: http://www.java-samples.com/showtutorial.php?tutorialid=369
    System.out.println("Loop Number: " + loopNum);
    Set set = firstSet_tbl.entrySet();
    Iterator i = set.iterator();
    
    while( i.hasNext() )
    {
        Map.Entry me = (Map.Entry)i.next();
        System.out.println(me.getKey() + " : " + me.getValue());
    }
    System.out.println();
  }//end printFirstSets
  
////////////////////////////////////////////////////////////////////////////////
//Write BNF grammar to file "BNFgrammar.txt"                                  //
//Write first sets for the grammar to file "FirstSets.txt"                    //
////////////////////////////////////////////////////////////////////////////////
  public void writeBNFgrammar()
  {
    BufferedWriter bw;
    List<String> list;
    
    try
    {
      bw = new BufferedWriter(new FileWriter("BNFgrammar.txt"));
      
      for(BNFrule r : rules)
      {
        bw.write(r.getLHS()); //write lhs
        bw.write(" ::= "); //write assignment token
        
        list = r.getRHS(); //get list of rhs tokens
        for( String s : list ) //write rhs tokens
          bw.write(s + " ");
        
        bw.newLine();
      }//end for
      
      bw.close();
    }//end try
    catch(Exception e){}   
  }//end writeBNFgrammar
  
  public void writeFirstSets()
  {
    BufferedWriter bw;
    Set set = firstSet_tbl.entrySet();
    Iterator i = set.iterator();
    
    try
    {
      bw = new BufferedWriter(new FileWriter("FinalFirstSets.txt"));
    
      //From: http://www.java-samples.com/showtutorial.php?tutorialid=369
      while( i.hasNext() ) //for each entry in the first set table
      {
        Map.Entry me = (Map.Entry)i.next();
        bw.write(me.getKey() + " : " + me.getValue()); // key : first set syms
        bw.newLine();
      }//end while
      
      bw.close();
    }//end try
    catch(Exception e){}
  }//end writeFirstSets
  
}//end BNFgrammar