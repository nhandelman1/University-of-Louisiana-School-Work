package propositionallogiccalculator2;

//throw exceptions related to the input EBNF grammar
public class GrammarException extends Exception
{
  GrammarException(String strErrMsg)
  {
    System.err.println(strErrMsg);
    System.exit(0);
  }//end 
}//end GrammarException