package distributedsharedbufferapp;

import java.util.ArrayList;
import java.util.List;
/**
 * @author Arthur, Nick, & Xiangbo
 */
public class Main
{
    final int NUM_INPUT_PORTS = 5;   // Number of ports
    final int NUM_INPUT_BUFFERS = NUM_INPUT_PORTS << 1; // Number of input buffers
    final int NUM_INPUT_VC = 2;     // Number of virtual channels per input port
    final int FLITS_PER_INPUT_VC = 4;// Number of flits per buffer (4 from paper)
    final int NUM_OUTPUT_PORTS = 5; // Number of output portS
    final int NUM_CYCLES = 1000;     // Number of cycles to simulate
    final int NUM_MM_BUFFERS = 5;//2 * NUM_INPUT_PORTS - 1;
    final int NUM_FLITS_PER_MM = 20; // Number flits per middle memory buffer

    Buffer[] inputbuffers;          // Buffers for each input port
    Storage outputbuffers;         // Buffer for each output port
    Storage MM;
    MiddleMemoryReservationTable MMRT;
    ConflictResolutioner CR;        // CR stage object
    TimeStamper RC_TS;              // RC_TS stage object
    FlitGenerator traffic;          // Object that creates flits (traffic)
    int[] buffersathost;            // Number of available buffers at hosts
    List<Flit> winningflits;        // Flits that are to be chosen by XBar1
    int droppedflits = 0;            // Track amount of flits dropped
    int clockcycle = -1;             // Monitor the clock cycle
    int receivedflits = 0;           // Number of created flits
    int headflits = 0;              // Number of head flits
    int minlatency = 100, maxlatency = 0;//min/max latency for a flit
    int sumlatency = 0, divlatency = 0;

    // Statistics variables and controls
    final double INPUT_FLIT_PROB = 0.46; //probability that an input port will
                                        //receive a flit in a given cycle
    final boolean GET_OUTPUT_TS = false;//Get timestamp from output buffer
    final boolean GET_NUM_DROPPED_FLITS = true;//Get number of dropped flits
    final boolean GET_NUM_RECEIVED_FLITS = true;
    final boolean GET_NUM_OF_HEAD_FLITS = true;
    final boolean GET_THROUGHPUT = true;
    final boolean GET_BODY_TAIL_FLITS = true;
    final boolean GET_MIN_LATENCY = true;
    final boolean GET_MAX_LATENCY = true;
    final boolean GET_AVG_LATENCY = true;


    public Main()
    {
        // Initialize entities for simulation here
        inputbuffers = new Buffer[NUM_INPUT_PORTS << 1];
        for (int i = 0; i < inputbuffers.length; i++)
            inputbuffers[i] = new Buffer(FLITS_PER_INPUT_VC);
        //technically not part of the DSBNoC, but we are not simulating down
        //stream routers. the output buffers serve simply as a place for
        //flits to go in pipeline stage 5
        outputbuffers = new Storage(NUM_OUTPUT_PORTS, 1);
        buffersathost = new int[NUM_INPUT_PORTS];
        for (int i = 0; i < NUM_INPUT_PORTS; i++)
            buffersathost[i] = FLITS_PER_INPUT_VC;
        winningflits = new ArrayList<Flit>();
        MM = new Storage(NUM_MM_BUFFERS, NUM_FLITS_PER_MM);
        MMRT = new MiddleMemoryReservationTable(NUM_MM_BUFFERS, NUM_FLITS_PER_MM);
        CR = new ConflictResolutioner(MMRT, inputbuffers, winningflits);
        RC_TS = new TimeStamper(inputbuffers);
        traffic = new FlitGenerator("Uniform");

        
        // Simulate
        int cycle = 0;
        while (cycle <= NUM_CYCLES)
        {
            // Update clockcycle and all entities
            clockcycle = (clockcycle + 1) % NUM_FLITS_PER_MM;

            /*Simulate*/

            // Decide what needs to be done based on the cycle
            switch(cycle)
            {
                case 0:
                    break;
                case 1:
                    TimeStampingStage(cycle);
                    break;
                case 2:
                    ConflictResolutionStage();
                    TimeStampingStage(cycle);
                    break;
                case 3:
                    MMWriteXBar1Stage();
                    ConflictResolutionStage();
                    TimeStampingStage(cycle);
                    break;
                case 4:
                    MMReadXBar2Stage(cycle);
                    MMWriteXBar1Stage();
                    ConflictResolutionStage();
                    TimeStampingStage(cycle);
                    break;
                default:
                    LinkTraversalStage(cycle);
                    MMReadXBar2Stage(cycle);
                    MMWriteXBar1Stage();
                    ConflictResolutionStage();
                    TimeStampingStage(cycle);
                    break;
            }

            // Generate new packets and insert them into ports' VCs.
            for (int i = 0; i < NUM_INPUT_BUFFERS; i += 2)
            {
                if (!inputbuffers[i].isFull() || !inputbuffers[i+1].isFull())
                {   
                    // Decide if a flit arrives on the port or not
                    double decision = Math.random();
                    if (decision < INPUT_FLIT_PROB)
                    {
                        receivedflits++;
                        // Create the new flit.
                        Flit flit = traffic.generateFlit(i >>> 1);
                        flit.setMaxPriority(NUM_MM_BUFFERS - 1);
                        flit.setCreatedCycle(cycle);
                        if (flit.isHead())
                        {
                            headflits++;
                        }

                        // Determine which port to put flit in
                        if (inputbuffers[i].getAmountInBuffer() >
                                inputbuffers[i+1].getAmountInBuffer())
                        {
                            inputbuffers[i+1].addInputFlit(flit);
                        }
                        else
                        {
                            inputbuffers[i].addInputFlit(flit);
                        }
                    }
                }
                else
                {
                    // The input buffer is full.
                    double decision = Math.random();
                    if (decision <= INPUT_FLIT_PROB)
                    {
                        // The flit must be dropped.
                        droppedflits++;
                    }
                }
            }

            // Decrement number of cycles to simulate
            cycle++;
        }

        // Output statistics here
        System.out.println("Simulation has finished \n");
        System.out.println("Stats:");
        if (GET_NUM_DROPPED_FLITS)
        {
            System.out.println("Dropped Flits: " + droppedflits);
        }
        if (GET_NUM_RECEIVED_FLITS)
        {
            System.out.println("Received Flits: " + receivedflits);
        }
        if (GET_THROUGHPUT)
        {
            double throughput = ((double) receivedflits) / ((double) receivedflits + droppedflits);
            System.out.println("Throughput: " + throughput);
        }
        if (GET_NUM_OF_HEAD_FLITS)
        {
            System.out.println("Head Flits: " + headflits);
            double percent = ((double) headflits / (double) receivedflits);
            System.out.println("Percent of Head Flits: " + percent);
        }
        if (GET_BODY_TAIL_FLITS)
        {
            int number = receivedflits - headflits;
            System.out.println("Body/Tail Flits: " + number);
        }
        if (GET_MAX_LATENCY)
        {
            System.out.println("Max Latency: " +maxlatency);
        }
        if (GET_MIN_LATENCY)
        {
            System.out.println("Min Latency: " +minlatency);
        }
        if (GET_AVG_LATENCY)
        {
            double avglatency = ((double) sumlatency) / ((double) divlatency);
            System.out.println("Average Latency: " +avglatency);
        }
    }

    private void LinkTraversalStage(int cycle)
    {
        if (GET_OUTPUT_TS)
        {
            System.out.println(cycle);
            if (outputbuffers.getBuffers()[0].getFlitAtIndex(0) != null)
                System.out.println("Buffer 0: " +outputbuffers.getBuffers()[0].getFlitAtIndex(0).getTimeStamp());
            else
                System.out.println("Buffer 0: empty");
            if (outputbuffers.getBuffers()[1].getFlitAtIndex(0) != null)
                System.out.println("Buffer 1: " +outputbuffers.getBuffers()[1].getFlitAtIndex(0).getTimeStamp());
            else
                System.out.println("Buffer 1: empty");
            if (outputbuffers.getBuffers()[2].getFlitAtIndex(0) != null)
                System.out.println("Buffer 2: " +outputbuffers.getBuffers()[2].getFlitAtIndex(0).getTimeStamp());
            else
                System.out.println("Buffer 2: empty");
            if (outputbuffers.getBuffers()[3].getFlitAtIndex(0) != null)
                System.out.println("Buffer 3: " +outputbuffers.getBuffers()[3].getFlitAtIndex(0).getTimeStamp());
            else
                System.out.println("Buffer 3: empty");
            if (outputbuffers.getBuffers()[4].getFlitAtIndex(0) != null)
                System.out.println("Buffer 4: " +outputbuffers.getBuffers()[4].getFlitAtIndex(0).getTimeStamp());
            else
                System.out.println("Buffer 4: empty");
        }

        // Exit Ports
        for(int j = 0; j < NUM_OUTPUT_PORTS; j++)
        {
            Flit temp = outputbuffers.getBuffers()[j].getFlitAtIndex(0);
            if (temp != null)
            {
                // Get latency
                int latency = cycle - temp.getCreatedCycle();
                if (latency > maxlatency)
                {
                    maxlatency = latency;
                }
                if (latency < minlatency)
                {
                    minlatency = latency;
                }

                sumlatency += latency;
                divlatency++;

                // Remove
                int departure = temp.getDeparturePort();
                CR.acknowledgeMoreBuffers(departure, 1);
            }
            
            //reset head of each outport buffer to 0
            //outputbuffers.getBuffers()[j].setHead(0);
            outputbuffers.getBuffers()[j].clear();
        }
    }

    private void MMReadXBar2Stage(int cycle)
    {
        // Pipeline Stage 4 - Middle Memory Read / XBar 2
        Flit tempFlit = null;
        for(int i = MM.getNumBuffers() - 1; i >= 0; i--)
        {
            if (MM.getBuffers()[i].getFlitAtIndex(clockcycle) != null)
            {
                tempFlit = MM.getBuffers()[i].getFlitAtIndex(clockcycle);
                    MM.getBuffers()[i].clearFlitAtIndex(clockcycle);
                    MMRT.setTable(i, clockcycle, false);
                    try // Debug for multiple flits being sent to same output buffer
                    {
                        if(tempFlit != null)
                        {
                            outputbuffers.getBuffers()[tempFlit.getDeparturePort()].addOutputFlit(tempFlit);
                        }
                    }
                    catch (java.lang.ArrayIndexOutOfBoundsException ex)
                    {
                        System.out.println("* * * * * * * * * * * ERROR * * * * * * * * * ");
                        System.out.println("Clockcycle: " +cycle);
                        System.out.println("Conflict Depart: " +tempFlit.getDeparturePort());
                        System.out.println("Conflict Ts: " +tempFlit.getTimeStamp());
                        System.out.println("Conflict Priority: " +tempFlit.getPriority());
                        System.out.println("-----------------------------------------------");
                        if (outputbuffers.getBuffers()[0].getFlitAtIndex(0) != null)
                        {
                            System.out.print("Buffer 0: " +outputbuffers.getBuffers()[0].getFlitAtIndex(0).getTimeStamp());
                            System.out.println(" Priority: " +outputbuffers.getBuffers()[0].getFlitAtIndex(0).getPriority());
                        }
                        if (outputbuffers.getBuffers()[1].getFlitAtIndex(0) != null)
                        {
                            System.out.print("Buffer 1: " + outputbuffers.getBuffers()[1].getFlitAtIndex(0).getTimeStamp());
                            System.out.println(" Priority: " +outputbuffers.getBuffers()[1].getFlitAtIndex(0).getPriority());
                        }
                        if (outputbuffers.getBuffers()[2].getFlitAtIndex(0) != null)
                        {
                            System.out.print("Buffer 2: " + outputbuffers.getBuffers()[2].getFlitAtIndex(0).getTimeStamp());
                            System.out.println(" Priority: " +outputbuffers.getBuffers()[2].getFlitAtIndex(0).getPriority());
                        }
                        if (outputbuffers.getBuffers()[3].getFlitAtIndex(0) != null)
                        {
                            System.out.print("Buffer 3: " + outputbuffers.getBuffers()[3].getFlitAtIndex(0).getTimeStamp());
                            System.out.println(" Priority: " +outputbuffers.getBuffers()[3].getFlitAtIndex(0).getPriority());
                        }
                        if (outputbuffers.getBuffers()[4].getFlitAtIndex(0) != null)
                        {
                            System.out.print("Buffer 4: " + outputbuffers.getBuffers()[4].getFlitAtIndex(0).getTimeStamp());
                            System.out.println(" Priority: " +outputbuffers.getBuffers()[4].getFlitAtIndex(0).getPriority());
                        }
                        System.out.println("* * * * * * * * * * * * * * * * * * * * * * * *");
                        ex.printStackTrace();
                        System.exit(0);
                    }
            }
        }//end for
    }

    private void MMWriteXBar1Stage()
    {
        // Pipeline Stage 3 - XBar 1 / Middle Memory Write
        if (!winningflits.isEmpty())
        {
            for( Flit f : winningflits)
                MM.getBuffers()[f.getMMRow()].addMMFlit(f);
            winningflits.clear();
        }
    }

    private void ConflictResolutionStage()
    {
        // Avoidance
        CR.CheckMiddleMemoryForFlits();
    }

    private void TimeStampingStage(int cycle)
    {
        // Timestamp
        RC_TS.timestampFlits(cycle);
    }

    public static void main(String[] args)
    {
        Main app = new Main();
    }

}

// Nick: nrh8883@louisiana.edu
// Bill: xxl8948@louisiana.edu