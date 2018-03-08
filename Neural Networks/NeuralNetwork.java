package propositionallogiccalculator2;

public class NeuralNetwork 
{
    private String name;
    private int numNeurons;
    private Neuron2In1Out[][] network;
    private int output;
    
    public NeuralNetwork()
    {
        name = "";
        numNeurons = 0;
        network = new Neuron2In1Out[2][2];
        output = -1;
    }//end NeuralNetwork
    
    public NeuralNetwork(String n, int num, Neuron2In1Out[][] net)
    {
        name = n;
        numNeurons = num;
        network = net;
        output = -1;
    }//end NeuralNetwork
    
    public void setName(String n)
    {
        name = n;
    }//end setName
    
    public void setNeurons(int num, Neuron2In1Out[][] net)
    {
        numNeurons = num;
        network = net;
    }//end setNeurons
    
    public String getName()
    {
        return name;
    }//end getName
    
    public int getNumNeurons()
    {
        return numNeurons;
    }//end getNumNeurons
    
    public Neuron2In1Out[][] getNeurons()
    {
        return network;
    }//end getNeurons
    
    public int getOutput()
    {
        return output;
    }//eng getOutput
    
    public void processNetwork(double in1, double in2)
    {   
        if( numNeurons == 3)
        {
            network[0][0].sumInput(in1, in2); network[0][0].outputFunction();
            network[0][1].sumInput(in1, in2); network[0][1].outputFunction();
            network[1][0].sumInput(network[0][0].getOutput(), network[0][1].getOutput());
            network[1][0].outputFunction();
            output = network[1][0].getOutput();
            
        }//end if
        else if( numNeurons == 1 )
        {
            network[0][0].sumInput(in1, in2); network[0][0].outputFunction();
            output = network[0][0].getOutput();
        }//end else

    }//end processNetwork
    
}//end class
