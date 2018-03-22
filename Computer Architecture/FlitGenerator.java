package distributedsharedbufferapp;
/**
 * @author Arthur, Nick, & Xiangbo
 */
public class FlitGenerator 
{
    private final double UNIFORM_HEAD_PROB = .65;   // 65%
    private String traffic; // Defines what time of traffic we want to simulate
    int sequencenum1, sequencenum2, sequencenum3, sequencenum4, sequencenum5;

    public FlitGenerator(String traffictype)
    {
        traffic = traffictype;
        sequencenum1 = sequencenum2 =
                sequencenum3 = sequencenum4 = sequencenum5 = 0;
    }

    public Flit generateFlit(int source)
    {
        if (traffic.equals("Uniform"))
            return generateUniformFlit(source);
        else
            return null;
    }

    private Flit generateUniformFlit(int source)
    {
        double decision = Math.random(); 	// random output port
	int output;                             // outputport for flit
	Flit newflit;				// the new flit

	// Decide what is the output port for flit
        if      (decision < 0.2) output = 0;
	else if (decision < 0.4) output = 1;
	else if (decision < 0.6) output = 2;
	else if (decision < 0.8) output = 3;
	else                     output = 4;

        // Construct the new flit
	newflit = new Flit(output, -1);
        switch(source)
        {
            case 0:
                newflit.setSource('A');
                newflit.setSequenceNum(sequencenum1);
                sequencenum1++;
                break;
            case 1:
                newflit.setSource('B');
                newflit.setSequenceNum(sequencenum2);
                sequencenum2++;
                break;
            case 2:
                newflit.setSource('C');
                newflit.setSequenceNum(sequencenum3);
                sequencenum3++;
                break;
            case 3:
                newflit.setSource('D');
                newflit.setSequenceNum(sequencenum4);
                sequencenum4++;
                break;
            default:
                newflit.setSource('E');
                newflit.setSequenceNum(sequencenum5);
                sequencenum5++;
                break;
        }
        
        // Determine if flit is a head flit or not.
        double probability = Math.random();
        if (probability >= UNIFORM_HEAD_PROB)
            newflit.setIsHead(true);
        else
            newflit.setIsHead(false);

	return newflit;
    }
}