package compilerproject;
import java.util.*;
import java.io.*;

public class CompilerProject 
{
public static void main(String[] args) 
{
  BNFgrammar EBNFinput = new BNFgrammar();
  BNFgrammar BNFoutput = new BNFgrammar();
  BufferedReader br;
  Scanner kb = new Scanner(System.in);
  int lineNum = 0;
  String fileName = "", line = "";
  String[] tokens;
  boolean startSymFound = false;
  
  //read in terminal symbols, non-terminal symbols, EBNF rules and validation
  try 
  { //read in set of terminal symbols  
    System.out.print("Enter the name of the terminals input file: ");
    fileName = kb.nextLine();
    br = new BufferedReader(new FileReader(fileName));
    
    do //read in terminal symbol (one per line), store in EBNFinput and
    {  //BNFoutput
      lineNum++;
      line = br.readLine();
      EBNFinput.addTermSym(line);
      BNFoutput.addTermSym(line);
    }while(!line.contentEquals("$"));
    
    br.close();
    
    //read in set of non terminal symbols from EBNF file
    lineNum = 0;
    System.out.print("Enter the name of the EBNF grammar input file: ");
    fileName = kb.nextLine();
    br = new BufferedReader(new FileReader(fileName));
    line = br.readLine();
    
    while( line != null ) //read in non terminal symbols (one per line), store 
    {                     //in EBNFinput and BNFoutput, check for required ::= 
      lineNum++;          //symbol, check for start symbol
      tokens = line.split("\\s");
      
      if( !EBNFinput.validNonTerm(tokens[0]) )
        throw new GrammarException("ERROR: Incorrect lhs line: " + lineNum);
      if( !tokens[1].contentEquals("::=") )
        throw new GrammarException("ERROR: '::=' not found on line: " +lineNum);
      
      EBNFinput.addNonTermSym(tokens[0]);
      BNFoutput.addNonTermSym(tokens[0]);
      
      if(tokens[tokens.length-1].contentEquals("$"))//look for start symbol
      {
        EBNFinput.setStartSym(tokens[0]);
        BNFoutput.setStartSym(tokens[0]);
        startSymFound = true;
      }//end if
      line = br.readLine();
    }//end while
    
    br.close();
    
    if(!startSymFound)
      throw new GrammarException("ERROR: Start symbol not found.");
  
    //read in and validate the set of rules from EBNF file
    lineNum = 0;
    br = new BufferedReader(new FileReader(fileName));
    line = br.readLine();
    
    while( line != null ) //read in EBNF grammar rules
    {
      lineNum++;
      tokens = line.split("\\s");
      EBNFinput.analyzeEBNFRule(tokens, lineNum);
      line = br.readLine();
    }//end while
    
    br.close();
  }//end try
  catch(FileNotFoundException e)
  {
    System.err.println("ERROR: File '" + fileName + "' not found.");
    System.exit(0);
  }//end catch
  catch(IOException e)
  {
   System.err.println("ERROR: Line number: "+lineNum+"could not be processed.");
    System.exit(0);
  }//end catch
  catch(GrammarException e){}//end catch

  //convert EBNF to BNF
  Iterator<BNFrule> iterator = EBNFinput.getRules().iterator();
    
  while(iterator.hasNext()) //iterate through all EBNF ruless
  {
    BNFrule r = iterator.next();
    List<String> rhs = r.getRHS();
    
    if( rhs.contains("{") ) //EBNF rule needs to be rewritten in BNF form
    {
      String t = "", s = "";
      BNFrule rule1, rule2;
      int i;
      
      //make new non-terminal symbol to replace the EBNF tokens within braces  
      t = r.getLHS();
      s = t.replace(">", "Mid>");
      BNFoutput.addNonTermSym(s);
      
      //rule 1 has lhs = string t (above), its rhs is the same as EBNF rule r,
      //but all tokens within the curly braces are replaced by the new 
      //non-terminal symbol
      //rule 2 has lhs = string s (above), its rhs contains all tokens within
      //the curly braces of the EBNF rule r
      rule1 = new BNFrule(t); 
      rule2 = new BNFrule(s);
      
      //add all tokens up to first curly brace to BNF rule1
      for(i = 0; ; i++)
      {
        if( !rhs.get(i).contentEquals("{") )
          rule1.addRHSSym(rhs.get(i));
        else { i++; break;}
      }//end for
      
      rule1.addRHSSym(s); //add new non-terminal symbol to rhs of rule1
      
      //add all tokens within the curly braces to BNF rule2
      for( ; ; i++ )
      {             
        if( !rhs.get(i).contentEquals("}") )
          rule2.addRHSSym(rhs.get(i));
        else {i++; break;}
      }//end for
      
      rule2.addRHSSym(s);//add new non-terminal symbol to rhs of rule 2
      
      //add all symbols after last curly brace to BNF rule1
      for( ; i < rhs.size() - 1; i++)
        rule1.addRHSSym(rhs.get(i)); 
      
      BNFoutput.addRule(rule1);
      BNFoutput.addRule(rule2);
      
      //add new rule with lhs = string s (above) and empty rhs
      rule1 = new BNFrule(s);
      BNFoutput.addRule(rule1);
      
    }//end if
    else //EBNF rule is in acceptable BNF form
    {
      BNFoutput.addRule(r);
    }//end else
  }//end while

  BNFoutput.writeBNFgrammar();
  
  //the following four methods complete and display the table of first sets 
  BNFoutput.initializeTables();
  BNFoutput.markLambda();
  BNFoutput.fillFirstSet();
  BNFoutput.writeFirstSets();
}//end main
}//end CompilerProject