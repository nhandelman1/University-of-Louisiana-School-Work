package distributedsharedbufferapp;
/**
 * @author Arthur, Nick, & Xiangbo
 */
public class MiddleMemoryReservationTable
{
    private boolean[][] table;

    public MiddleMemoryReservationTable(int row, int col)
    {
        table = new boolean[row][col];
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                table[i][j] = false;
    }

    public boolean checkTable(int row, int col)
    {
        if (row >= table.length)
            return table[table.length - 1][col];
        else
            return table[row][col];
    }

    public void setTable(int row, int col, boolean set)
    {
        table[row][col] = set;
    }

    public int getSizeOfBuffer()
    {
        return table[0].length;
    }

}