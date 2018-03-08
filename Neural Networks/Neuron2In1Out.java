package propositionallogiccalculator2;

public class Neuron2In1Out 
{
    private double bias;
    private double[] wts;
    private double inputSum;
    private int output;
    
    public Neuron2In1Out()
    {
        bias = 0;
        wts = new double[2];
        inputSum = 0;
        output = 0;
    }//end Neuron2In1Out
    
    public Neuron2In1Out( double b, double[] w)
    {
        bias = b;
        wts = w;
        inputSum = 0;
        output = 0;
    }//end Neuron2In1Out
    
    public void setBias(double b)
    {
        bias = b;
    }//end setBias
    
    public void setWeights(double[] w)
    {
        wts = w;
    }//end setWeights
    
    public double getBias()
    {
        return bias;
    }//end getBias
    
    public double[] getWeights()
    {
        return wts;
    }//end getWeights
    
    public double getInputSum()
    {
        return inputSum;
    }//end getInputSum
    
    public int getOutput()
    {
        return output;
    }//end getOutput
    
    public void sumInput( double in1, double in2)
    {
        inputSum = wts[0] * in1 + wts[1] * in2 + bias;
    }//end sumInput
    
    public void outputFunction()
    {
        if(inputSum < 0)
            output = 0;
        else output = 1;
        //System.out.println(inputSum + " " + output);
    }//end outputFunction
}//end class
