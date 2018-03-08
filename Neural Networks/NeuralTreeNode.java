package propositionallogiccalculator2;

public class NeuralTreeNode 
{
    private String nodeName;
    private NeuralNetwork net;
    private NeuralTreeNode leftChild;
    private NeuralTreeNode rightChild;
    
    public NeuralTreeNode()
    {
        nodeName = "";
        net = null;
        leftChild = null;
        rightChild = null;
    }//end TreeNode
    
    public NeuralTreeNode(String name, NeuralNetwork n, NeuralTreeNode l, NeuralTreeNode r)
    {
        nodeName = name;
        net = n;
        leftChild = l;
        rightChild = r;
    }//end TreeNode
    
    public void setName(String name)
    {
        nodeName = name;
    }//end setName
    
    public void setNetwork(NeuralNetwork n)
    {
        net = n;
    }//end setNetwork
    
    public void setLeftChild(NeuralTreeNode l)
    {
        leftChild = l;
    }//end setLeftChild
    
    public void setRightChild(NeuralTreeNode r)
    {
        rightChild = r;
    }//end setRightChild
    
    public String getName()
    {
        return nodeName;
    }//end getName
    
    public NeuralNetwork getNetwork()
    {
        return net;
    }//end getNetwork
    
    public NeuralTreeNode getLeftChild()
    {
        return leftChild;
    }//end getLeftChild
    
    public NeuralTreeNode getRightChild()
    {
        return rightChild;
    }//end getRightChild
    
}//end class
