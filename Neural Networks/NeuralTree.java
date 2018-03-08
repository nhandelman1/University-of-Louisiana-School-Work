package propositionallogiccalculator2;

public class NeuralTree 
{
    private NeuralTreeNode root;
    static final int CHAR_OFFSET_LETTER = 65;
    
    public NeuralTree()
    {
        root = null;
    }//end Tree
    
    public NeuralTree(NeuralTreeNode r)
    {
        root = r;
    }//end Tree
    
    public void setRoot(NeuralTreeNode r)
    {
        root = r;
    }//end setRoot
    
    public NeuralTreeNode getRoot()
    {
        return root;
    }//end getRoot
    
    public void addNode(NeuralTreeNode ntn)
    {
        
    }//end addNode
    
    public void postOrderTraversal(int[][] tt, int ttRow)
    {
        postOrderTraversal(root, tt, ttRow);
    }//end postOrderTraversal
    
    private void postOrderTraversal(NeuralTreeNode n, int[][] tt, int ttRow)
    {
        char c = ' ';
        
        if(n.getLeftChild() != null) //nodes have either 2 or 0 children
        {
            postOrderTraversal(n.getLeftChild(), tt, ttRow);
            postOrderTraversal(n.getRightChild(), tt, ttRow);
        }//end if
        
        c = n.getName().charAt(0);
        
        if(c == 'T') //truth (1) constant
            n.getNetwork().processNetwork(1, 1);
        else if(c == 'F') //false (0) constant
            n.getNetwork().processNetwork(0, 0);
        else if(c >= 'A' && c <= 'Z') //variable - get value from truth table
            n.getNetwork().processNetwork(tt[ttRow][c-CHAR_OFFSET_LETTER], 0);
        else n.getNetwork().processNetwork(n.getLeftChild().getNetwork().getOutput(),
                n.getRightChild().getNetwork().getOutput());
    }//end postOrderTraversal
}//end class
