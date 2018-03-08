package propositionallogiccalculator2;
import java.util.*;
import java.io.*;
import java.lang.*;

public class PropositionalLogicCalculator2 
{

public enum LastCharType {NULL, NEG, VC, OP, OPPAR, CLPAR}
static final int MAX_SYMBOLS = 100;
static final int CHAR_OFFSET_LETTER = 65;
static final int CHAR_OFFSET_NUMBER = 48;

public static void main(String[] args)
{
    //variables and initialization
    String fileName = "", infixProp = "", curr = "";
    String[] revInfixProp = new String[MAX_SYMBOLS];
    String[] revPrefixProp = new String[MAX_SYMBOLS];
    String[] prefixProp = new String[MAX_SYMBOLS];
    Stack  s = new Stack();
    int i = 0, arrayTop = 0, numInputTotal = 0, numInputDistinct = 0;
    NeuralTree brain;
    int[] distinctVarConst = new int[26];
    int[][] truthTable; //will be instantiated later
    int ttRows = 0, temp = 0, inputVal = 0;
    int[] inputVals = new int[26];

    for(i = 0; i < 26; i++)
    {
        distinctVarConst[i] = 0;
        inputVals[i] = 0;
    }//end for
    
    //Read in the file name and the infix proposition from the file
    //Reverse the infix proposition to determine the prefix proposition
    fileName = getFile();
    infixProp = getProposition(fileName);
    try
    {
        revInfixProp = reverseInfixString(infixProp);
    }//end try
    catch(GrammarException e){}
    
    //Parse the proposition by converting from infix to prefix
    i = 0;
    while(!revInfixProp[i].contentEquals("end"))
    {
        curr = revInfixProp[i];
        
        if(isVarConst(curr.charAt(0)) || curr.charAt(0) == '!')
        { //curr is operand or negation with attached operand
            revPrefixProp[arrayTop++] = curr;
            numInputTotal++;
            if(isDistinctVariable(curr, distinctVarConst))
                numInputDistinct++;
        }//end if
        else if( curr.contentEquals(")"))
            s.push(curr);
        else if( curr.contentEquals("("))
        {
            if(!s.isEmpty())
            {                   //pop stack and append operators to string array
                while( !s.getTop().contentEquals(")") ) 
                    revPrefixProp[arrayTop++] = s.pop();
                s.pop(); //remove ')' from the stack
            }//end if
        }//end else if
        else 
        {                       //pop stack and append operators to string array
            while(!s.isEmpty() && !s.getTop().contentEquals(")"))
                revPrefixProp[arrayTop++] = s.pop();
            s.push(curr); //push operator onto stack
        }//end else
        
        i++;
    }//end while
    
    while(!s.isEmpty()) //pop the remaining operators on the stack
        revPrefixProp[arrayTop++] = s.pop();
    
    for(int j = arrayTop-1, k = 0; j >= 0; j--, k++) //reverse revPrefixProp
        prefixProp[k] = revPrefixProp[j];
    
    System.out.print("Prefix Proposition: "); //print prefix string
    for(i = 0; i < arrayTop; i++)
        System.out.print(prefixProp[i]);
    
    //Compile truth table with all possible input variations
    ttRows = (int)Math.pow(2, numInputDistinct);
    temp = ttRows;
    inputVal = 0;
    truthTable = new int[ttRows][27];//column 26 is for output
    
    for(int c = 0; c < 26; c++ )
    {
        if(distinctVarConst[c] > 0)
        {
            temp /= 2;
        
            for(int r = 0; r < ttRows; r++)
            {
                if( r % temp == 0 )
                {
                    if(inputVal == 0)
                        inputVal = 1;
                    else inputVal = 0;
                }//end if
                truthTable[r][c] = inputVal;
            }//end for t
        }//end if
    }//end for c
    
    //Create a neural network tree using the prefix proposition
    NeuralTreeNode ntn = new NeuralTreeNode(prefixProp[0], buildNeuralNetwork(prefixProp[0]),
        new NeuralTreeNode(), new NeuralTreeNode());
    brain = new NeuralTree(ntn); //create new tree with its root
    
    for(i = 1; i < arrayTop; i++)//add nodes to tree
    {
        
        
        
    }//end for
        
    //Run the neural network for all possible input variations
    //This is a post-order traversal of the neural network tree
    /*for(i = 0; i < ttRows; i++)
    {
        brain.postOrderTraversal(truthTable, i); 
        truthTable[i][26] = brain.getRoot().getNetwork().getOutput();
    }//end for
    */
    
    //Output truth table to file
    writeFile(truthTable, distinctVarConst, ttRows, fileName, infixProp);
}//end main

/* Function - getFile
 * Arguments: none
 * Returns: String
 * 
 * User inputs the name of the file which holds the proposition.
 * Return file name as a String.
 */
public static String getFile()
{
    Scanner kb = new Scanner(System.in);
    String f = "";
   
    System.out.print("Enter the name of the proposition file: ");
    f = kb.nextLine();
    
    return f;
}//end getFile    

/* Function - getProposition
 * Arguments: String
 * Returns: String
 * Exceptions: FileNotFoundException, IOException
 * 
 * Read the proposition from the file whose name is provided as an argument.
 * Return the proposition as a String.
 */
public static String getProposition(String f)
{
    BufferedReader br;
    String l = "";
    System.out.println(f);
    try
    {
        br = new BufferedReader(new FileReader(f));
        l = br.readLine();
    }//end try
    catch(FileNotFoundException e)
    {
        System.err.println("ERROR: File '" + f + "' not found.");
        System.exit(0);  
    }//end catch
    catch(IOException e)
    {
        System.err.println("ERROR: The proposition could not be processed");
        System.exit(0);
    }//end catch
    
    return l;
}//end getProposition

/* Function - reverseInfixString
 * Arguments: String
 * Returns: String[]
 * Exceptions: GrammarException - caught by caller
 * 
 * Receives a proposition as a String and converts the String into a String[] 
 * such that the String held at each index of the array is an independent symbol 
 * of the proposition. It then reverses the String[].
 * A large portion of the syntax checking occurs here. The rest occurs later 
 * during conversion of the proposition from infix to prefix.
 * Returns the reversed infix string[].
 */
public static String[] reverseInfixString(String in) throws GrammarException
{
    String temp = "";
    String[] array = new String[MAX_SYMBOLS];
    String[] arrayTemp = new String[MAX_SYMBOLS];
    int arrayTop = 0, numOpenParens = 0, numClosedParens = 0;
    int numOps = 0, numVarConst = 0;
    
    //convert the infix string into an array of strings and check syntax
    for(int i = 0; i < in.length(); i++)
    {
        if(in.charAt(i) == ' ')//dont consider spacing in the infix string
            continue;
        
        if(isValidSymbol(in.charAt(i)))
            temp = Character.toString(in.charAt(i));
        else throw new GrammarException("Error at char: " + i);
        
        if(temp.contentEquals("-") || temp.contentEquals("!")) //->, !A
        {
            if(temp.contentEquals("-") && in.charAt(i+1) != '>')
                throw new GrammarException("Operator (->) error at char: " + i);
            if(temp.contentEquals("!") && !isVarConst(in.charAt(i+1)))
                throw new GrammarException("Operator (!) error at char: " + i);
            
            temp = temp.concat(Character.toString(in.charAt(++i)));
        }//end if
        else if(temp.contentEquals("x") || temp.contentEquals("<"))//xor, <->
        {
            if(temp.contentEquals("x") && (in.charAt(i+1) != 'o' || in.charAt(i+2) != 'r'))
                throw new GrammarException("Operator (xor) error at char: " + i);
            if(temp.contentEquals("<") && (in.charAt(i+1) != '-' || in.charAt(i+2) != '>'))
                throw new GrammarException("Operator (<->) error at char: " + i);
            
            temp = temp.concat(Character.toString(in.charAt(++i)));
            temp = temp.concat(Character.toString(in.charAt(++i)));
        }//end else if
        
        array[arrayTop++] = temp;
    }//end for
    
    //check for partially correct syntax
    for(int i = 0; i < arrayTop; i++)
    {
        if(array[i].contentEquals("("))
            numOpenParens++;
        else if(array[i].contentEquals(")"))
            numClosedParens++; 
        else if(isBinaryOperator(array[i].charAt(0)))
            numOps++;
        else if(isVarConst(array[i].charAt(0)) || array[i].charAt(0) == '!')
            numVarConst++;
    }//end for
    
    if(numOpenParens > numClosedParens)
        throw new GrammarException("Error: Too many open parens / Too few closed parens");
    if(numOpenParens < numClosedParens)
        throw new GrammarException("Error: Too many closed parens / Too few open parens");
    if((numOps+1) != numVarConst)
        throw new GrammarException("Error: Number of Binary Operations: " + numOps + 
                ", Number of Non-Distinct Operands: " + numVarConst + ". There must be"
                + "exactly one more operand than binary operations.");
    
    //reverse the array of strings
    for(int i = 0, j = arrayTop-1; i < arrayTop; i++, j--)
        arrayTemp[j] = array[i];
    
    arrayTemp[arrayTop] = "end";
    return arrayTemp;
}//end reverseString

/* Function - isValidSymbol
 * Arguments: char
 * Returns: boolean
 * 
 * Determines if the char argument is a valid symbol according to the syntax.
 * Returns true if char is valid. False otherwise.
 */
public static boolean isValidSymbol(char c)
{
    boolean valid = false;
    
    if(isVarConst(c) || isBinaryOperator(c) || c == '!')
        valid = true;
    else if( c == '(' || c == ')')
        valid = true;
 
    return valid;
}//end isValidSymbol

/* Function - isVarConst
 * Arguments: char
 * Returns: boolean
 * 
 * Determines if the char argument is a valid variable/constant according to the
 * syntax. Any uppercase letter is valid.
 * Returns true if char is valid variable/constant. False otherwise.
 */
public static boolean isVarConst(char c)
{   
    if(c >= 'A' && c <= 'Z')//symbol is varconst
        return true;
    
    return false;
}//end isVarConst

/* Function - isBinaryOperator
 * Arguments: char
 * Returns: boolean
 * 
 * Determines if the char argument is a valid operator according to the syntax.
 * &, v, *, b are operators represented by a single character
 * < (<->), - (->), x (xor) are operators represented by multiple characters.
 * For these, it is sufficient to analyze the first character.
 * Returns true if char is a valid binary operator. False otherwise.
 */
public static boolean isBinaryOperator(char c)
{   
    if( c == '&' || c == 'v' || c == '<' || c == '-' || 
        c == '*' || c == 'b' || c == 'x')
        return true;
    
    return false;
}//end is Operator

/* Function - isDistinctVarConst
 * Arguments: String, int[]
 * Returns: boolean
 * 
 * Determine if the variable contained within the String argument has
 * been used within the proposition before and isn't a constant (T, F).
 * For example, the variable A could be an operand at multiple locations within
 * the proposition.
 * Returns true if variable is distinct. False otherwise.
 */
public static boolean isDistinctVariable(String s, int[] array)
{
    char c = ' ';
    boolean ans = false;
    
    if(s.length() == 1)
        c = s.charAt(0);
    else c = s.charAt(1);
    
    if( c != 'T' && c != 'F' && array[c-CHAR_OFFSET_LETTER] == 0 )
        ans = true;
    
    array[c-CHAR_OFFSET_LETTER]++;
    
    return ans;
}//end distinctVarConst

/* Function - buildNeuralNetwork
 * Arguments: String
 * Returns: NeuralNetwork
 * 
 * Determines the structure of the neural network according to the value of the
 * String argument.
 * Creates and returns the appropriate neural network. 
 */
public static NeuralNetwork buildNeuralNetwork(String n)
{
    Neuron2In1Out[][] network = new Neuron2In1Out[2][2];
    double[] wt = new double[2];
    int numNeur = 0; 
    
    //initialize network neurons
    network[0][0] = new Neuron2In1Out();
    network[0][1] = new Neuron2In1Out();
    network[1][0] = new Neuron2In1Out();
    network[1][1] = new Neuron2In1Out();

    if( n.contentEquals("NULL"))
    {   
        network[0][0].setBias(-1); //neuron layer 1 unit 1
        wt[0] = 1; wt[1] = 0;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end if
    else if( n.contentEquals("!") )
    {
        network[0][0].setBias(0); //neuron layer 1 unit 1
        wt[0] = -1; wt[1] = 0;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if
    else if( n.contentEquals("&") )
    {
        network[0][0].setBias(-2); //neuron layer 1 unit 1
        wt[0] = 1; wt[1] = 1;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if
    else if( n.contentEquals("*") )
    {
        network[0][0].setBias(1); //neuron layer 1 unit 1
        wt[0] = -1; wt[1] = -1;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if
    else if( n.contentEquals("v") )
    {
        network[0][0].setBias(-1); //neuron layer 1 unit 1
        wt[0] = 1; wt[1] = 1;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if
    else if( n.contentEquals("b") )
    {
        network[0][0].setBias(0); //neuron layer 1 unit 1
        wt[0] = -1; wt[1] = -1;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if
    else if( n.contentEquals("->") )
    {
        network[0][0].setBias(1); //neuron layer 1 unit 1
        wt[0] = -2; wt[1] = 1;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if 
    else if( n.contentEquals("<->") )
    {
        network[0][0].setBias(1); //neuron layer 1 unit 1
        wt[0] = 1; wt[1] = -2;
        network[0][0].setWeights(wt);
        numNeur++;
        
        wt = new double[2];
        network[0][1].setBias(1); //neuron layer 1 unit 2
        wt[0] = -2; wt[1] = 1;
        network[0][1].setWeights(wt);
        numNeur++;
        
        wt = new double[2];
        network[1][0].setBias(-2); //neuron layer 2 unit 1
        wt[0] = 1; wt[1] = 1;
        network[1][0].setWeights(wt);
        numNeur++;
    }//end else if
    else if( n.contentEquals("xor") )
    {
        network[0][0].setBias(1); //neuron layer 1 unit 1
        wt[0] = 1; wt[1] = -2;
        network[0][0].setWeights(wt);
        numNeur++;
        
        wt = new double[2];
        network[0][1].setBias(1); //neuron layer 1 unit 2
        wt[0] = -2; wt[1] = 1;
        network[0][1].setWeights(wt);
        numNeur++;
        
        wt = new double[2];
        network[1][0].setBias(1); //neuron layer 2 unit 1
        wt[0] = -1; wt[1] = -1;
        network[1][0].setWeights(wt);
        numNeur++;
    }//end else if
    
    return new NeuralNetwork(n, numNeur, network);
}//end buildNeuralNetwork    

/* Function - writeFile
 * Arguments: int[][], int[], int r, int c, String inputFile, String prop
 * Returns: none
 * 
 * Outputs the truth table created by running the complete neural network for
 * all variations of input variables. 
 */
public static void writeFile(int[][] table, int[] distinctChars, int ttRow, 
        String inputFile, String prop)
{
    BufferedWriter bw;

    try
    {
        bw = new BufferedWriter(new FileWriter(inputFile + "TruthTable.txt"));
        
        bw.write(prop); //write proposition to file
        bw.newLine();
        bw.newLine();

        for(int i = 0; i < 26; i++) //write variables and 'output' to file
        {
            if( distinctChars[i] > 0 )
                bw.write((char)(i+CHAR_OFFSET_LETTER) + "   ");
        }//end for
        bw.write("Output");
        bw.newLine();
        
        for(int r = 0; r < ttRow; r++) //write truth table values to file
        {
            for(int c = 0; c < 26; c++)
            {
                if(distinctChars[c] > 0)
                    bw.write(table[r][c] + "   ");
            }//end for
            bw.write(table[r][26]);
            bw.newLine();
        }//end for
        
        bw.close();
    }//end try
    catch(IOException e)
    {
        System.err.println("ERROR: Output file could not be written.");
        System.exit(0);
    }//end catch
}//end writeFile

}//end class

