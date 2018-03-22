package distributedsharedbufferapp;
/**
 * @author Arthur, Nick, & Xiangbo
 */
public class Storage
{
    private Buffer buffers[];
    private int numBuffers;

    public Storage(int numbuffers, int buffersize)
    {
        buffers = new Buffer[numbuffers];
        for (int i = 0; i < numbuffers; i++)
            buffers[i] = new Buffer(buffersize);
        numBuffers = numbuffers;
    }

    public Buffer[] getBuffers()
    {
        return buffers;
    }

    public int getNumBuffers()
    {
        return numBuffers;
    }//end getNumBuffers
}