//See MS Word file "Logic Calculation Algorithm 1" for a description of how
//this program works
package propositionallogiccalculator1;
import java.util.*;
import java.io.*;
import java.lang.Math;

public class PropositionalLogicCalculator1
{

public enum LastCharType { NEGATION, VARCONST, OPERATION }
static final int CHAR_OFFSET_LETTER = 65;
static final int CHAR_OFFSET_NUMBER = 48;
static final int BRAIN_ROWS = 10;
static final int BRAIN_COLUMNS = 10;

public static void main(String[] args)
{
    //variables and initializations
    String fileName = "", proposition = "";
    char[] varConstList = new char[100];
    char curr = ' ';
    LastCharType lct = LastCharType.OPERATION;
    int numInputTotal = 0, numInputDistinct = 0, numOps = 0;
    int[] distinctVarConst = new int[26];
    NeuralNetwork[][] brain = new NeuralNetwork[BRAIN_ROWS][BRAIN_COLUMNS];
    int[][] truthTable;
    int ttRows = 0, temp = 0, inputVal = 0;
   
    for(int i = 0; i < 26; i++)
        distinctVarConst[i] = 0;
    
    for(int i = 0; i < BRAIN_ROWS; i++)
        for(int g = 0; g < BRAIN_COLUMNS; g++)
            brain[i][g] = buildNeuralNetwork("NULL");
    
    //read in the file name and the proposition from the file
    fileName = getFile();
    proposition = getProposition(fileName);
    
    //parse the proposition and create neural network to represent the 
    //proposition
    try
    {   
        for(int i = 0; i < proposition.length(); i++)
        {
            curr = proposition.charAt(i);
            
            if( curr == ' ' )
                continue;
            else if( lct == LastCharType.NEGATION)
            {
                if( curr >= 'A' && curr <= 'Z' )
                {
                    lct = LastCharType.VARCONST;
                    varConstList[numInputTotal] = curr;
                    brain[numInputTotal][0] = buildNeuralNetwork("NEGATION");
                    numInputTotal++;
                    if(isDistinctVarConst(curr, distinctVarConst))
                        numInputDistinct++;
                }//end if
                else throw new GrammarException("Error at Character: " + i);
                
            }//end else if NEGATION
            else if( lct == LastCharType.OPERATION)
            {
                if( curr == '!' )
                    lct = LastCharType.NEGATION;
                else if( curr >= 'A' && curr <= 'Z')
                {
                    lct = LastCharType.VARCONST;
                    brain[numInputTotal][0] = buildNeuralNetwork("NULL");
                    varConstList[numInputTotal] = curr;
                    numInputTotal++;
                    if(isDistinctVarConst(curr, distinctVarConst))
                        numInputDistinct++;
                }//end else if
                else throw new GrammarException("Error at Character: " + i);
            }//end else if OPERATION
            else if( lct == LastCharType.VARCONST)
            {                
                if( curr == '&' || curr == '*' || curr == 'v' || curr == 'b')
                {
                    lct = LastCharType.OPERATION;
                    if(curr == '&')
                        brain[numInputTotal][numInputTotal] = buildNeuralNetwork("AND");
                    else if( curr == '*')
                        brain[numInputTotal][numInputTotal] = buildNeuralNetwork("NAND");
                    else if( curr == 'v')
                        brain[numInputTotal][numInputTotal] = buildNeuralNetwork("OR");
                    else brain[numInputTotal][numInputTotal] = buildNeuralNetwork("NOR");
                }//end if
                else if( curr == '-' )
                {
                    lct = LastCharType.OPERATION;
                    brain[numInputTotal][numInputTotal] = buildNeuralNetwork("CONDITIONAL");
                    i++;
                }//end else if
                else if( curr == 'x' || curr == '<')
                {
                    lct = LastCharType.OPERATION;
                    if(curr == 'x')
                        brain[numInputTotal][numInputTotal] = buildNeuralNetwork("XOR");
                    else brain[numInputTotal][numInputTotal] = buildNeuralNetwork("BICONDITIONAL");
                    i += 2;
                }//end else if
                else throw new GrammarException("Error at Character: " + i);
                
                numOps++;
            }//end else if
        }//end for i
        
        if(numInputTotal != (numOps + 1))
            throw new GrammarException("Too many variables or too many "
                    + "operations");
 
    }//end try
    catch(GrammarException e){}//end catch
    
    //compile truth table with all possible input variations
    ttRows = (int)Math.pow(2, numInputDistinct);
    temp = ttRows;
    inputVal = 0;
    truthTable = new int[ttRows][numInputDistinct + 1];//add output column

    for(int c = 0; c < numInputDistinct; c++ )
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
    }//end for c
    
    //run the neural network for all possible input variations
    int[] inputRow = new int[numInputTotal];
    char var = ' ';

    for(int ttr = 0; ttr < ttRows; ttr++) //run through all truth table variations
    {
        //build input column with truth values for this iteration of the neural 
        //network
        for(int nit = 0; nit < numInputTotal; nit++)
        {
            var = varConstList[nit];
            inputRow[nit] = truthTable[ttr][var-CHAR_OFFSET_LETTER];
        }//end for nit

        //run first column of the brain network to handle negations
        for(int nit = 0; nit < numInputTotal; nit++)
        {
            brain[nit][0].processNetwork(inputRow[nit], 0);
            //System.out.print( brain[nit][0].getOutput() + " ");
        }//end for nit
            //System.out.println();
        //process all remaining columns of the brain network
        for(int c = 1; c < numInputTotal; c++) //for each remaining column
        {
            for(int r = c; r < numInputTotal; r++) //for each row in the column
            {                                      //greater than or equal to
                if(brain[r][c].getName().contentEquals("NULL"))  //column value
                    brain[r][c].processNetwork(brain[r][c-1].getOutput(), 0);
                else brain[r][c].processNetwork(brain[r-1][c-1].getOutput(), brain[r][c-1].getOutput());
            }//end for c

        }//end for r
        
        //store final output in last column of truthTable
        truthTable[ttr][numInputDistinct] = 
                brain[numInputTotal-1][numInputTotal-1].getOutput();
    }//end for ttr
    
    //output truth table to file
    writeFile(truthTable, distinctVarConst, ttRows, numInputDistinct + 1, 
            fileName, proposition);
}//end main





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
    else if( n.contentEquals("NEGATION") )
    {
        network[0][0].setBias(0); //neuron layer 1 unit 1
        wt[0] = -1; wt[1] = 0;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if
    else if( n.contentEquals("AND") )
    {
        network[0][0].setBias(-2); //neuron layer 1 unit 1
        wt[0] = 1; wt[1] = 1;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if
    else if( n.contentEquals("NAND") )
    {
        network[0][0].setBias(1); //neuron layer 1 unit 1
        wt[0] = -1; wt[1] = -1;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if
    else if( n.contentEquals("OR") )
    {
        network[0][0].setBias(-1); //neuron layer 1 unit 1
        wt[0] = 1; wt[1] = 1;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if
    else if( n.contentEquals("NOR") )
    {
        network[0][0].setBias(0); //neuron layer 1 unit 1
        wt[0] = -1; wt[1] = -1;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if
    else if( n.contentEquals("CONDITIONAL") )
    {
        network[0][0].setBias(1); //neuron layer 1 unit 1
        wt[0] = -2; wt[1] = 1;
        network[0][0].setWeights(wt);
        numNeur++;
    }//end else if 
    else if( n.contentEquals("BICONDITIONAL") )
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
    else if( n.contentEquals("XOR") )
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



public static NeuralNetwork buildNeuralNetwork(String n, double[] b, double[] w)
{
    Neuron2In1Out[][] network = new Neuron2In1Out[2][2];
    double[] wt = new double[2];
    int numNeur = 0;
    
    //initialize network neurons
    network[0][0] = new Neuron2In1Out();
    network[0][1] = new Neuron2In1Out();
    network[1][0] = new Neuron2In1Out();
    network[1][1] = new Neuron2In1Out();
    
    network[0][0].setBias(b[0]); //neuron layer 1 unit 1
    wt[0] = w[0]; wt[1] = w[1];
    network[0][0].setWeights(wt);
    numNeur++;
    
    if(n.contentEquals("XOR") || n.contentEquals("BICONDITIONAL"))
    {
        network[0][1].setBias(b[1]); //neuron layer 1 unit 2
        wt[0] = w[2]; wt[1] = w[3];
        network[0][1].setWeights(wt);
        numNeur++;
        network[1][0].setBias(b[2]); //neuron layer 2 unit 1
        wt[0] = w[4]; wt[1] = w[5];
        network[1][0].setWeights(wt);
        numNeur++;
    }//end if
    
    return new NeuralNetwork(n, numNeur, network);
}//end build1NeuronNetwork

public static String getFile()
{
    Scanner kb = new Scanner(System.in);
    String f = "";
   
    System.out.print("Enter the name of the proposition file: ");
    f = kb.nextLine();
    
    return f;
}//end getFile

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

public static boolean isDistinctVarConst(char c, int[] array)
{
    boolean ans = false;
    
    if( c != 'T' && c != 'F' && array[c-CHAR_OFFSET_LETTER] == 0 )
        ans = true;
    
    array[c-CHAR_OFFSET_LETTER]++;
    
    return ans;
}//end distinctVarConst

public static void writeFile(int[][] table, int[] distinctChars, int r, int c, 
        String inputFile, String prop)
{
    BufferedWriter bw;

    try
    {
        bw = new BufferedWriter(new FileWriter(inputFile + "TruthTable.txt"));
        
        bw.write(prop); //write proposition to file
        bw.newLine();
        bw.newLine();

        for(int x = 0; x < 26; x++ ) //write variables and 'output' to file
        {
            if( distinctChars[x] > 0 )
                bw.write((char)(x+CHAR_OFFSET_LETTER) + "   ");
        }//end for
        bw. write("Output");
        bw.newLine(); 

        for(int x = 0; x < r; x++)//write truth table values to file
        {
            for(int y = 0; y < c; y++)
                bw.write(table[x][y] + "   ");
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
