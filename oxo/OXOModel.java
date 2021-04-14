import java.util.*;

class OXOModel
{
    private List<List<OXOPlayer>> cells = new ArrayList<>();
    private List<OXOPlayer> players = new ArrayList<>();
    private OXOPlayer currentPlayer;
    private OXOPlayer winner;
    private boolean gameDrawn;
    private int winThreshold;

    public OXOModel(int numberOfRows, int numberOfColumns, int winThresh)
    {
        winThreshold = winThresh;
        for (int i = 0; i < numberOfRows; i++) {
            List<OXOPlayer> tmp = new ArrayList<>(numberOfColumns);
            for (int j = 0; j < numberOfColumns; j++) {
                tmp.add(null);
            }
            cells.add(tmp);
        }
    }

    public int getNumberOfPlayers()
    {
        return players.size();
    }

    public void addPlayer(OXOPlayer player)
    {
        players.add(player);
    }

    public OXOPlayer getPlayerByNumber(int number)
    {
        return players.get(number);
    }

    public OXOPlayer getWinner()
    {
        return winner;
    }

    public void setWinner(OXOPlayer player)
    {
        winner = player;
    }

    public OXOPlayer getCurrentPlayer()
    {
        return currentPlayer;
    }

    public void setCurrentPlayer(OXOPlayer player)
    {
        currentPlayer = player;
    }

    public int getCurrentPlayerNumber()
    {
        return players.indexOf(currentPlayer);
    }

    public void incrementCurrentPlayer()
    {
        setCurrentPlayer(players.get(players.indexOf(currentPlayer) + 1));
    }

    public int getNumberOfRows()
    {
        return cells.size();
    }

    public int getNumberOfColumns()
    {
        return cells.get(0).size();
    }

    public OXOPlayer getCellOwner(int rowNumber, int colNumber)
    {
        return cells.get(rowNumber).get(colNumber);
    }

    public void setCellOwner(int rowNumber, int colNumber, OXOPlayer player)
    {
        List<OXOPlayer> tmp = cells.get(rowNumber);
        tmp.set(colNumber, player);
        cells.set(rowNumber, tmp);
    }

    public void setWinThreshold(int winThresh)
    {
        winThreshold = winThresh;
    }

    public int getWinThreshold()
    {
        return winThreshold;
    }

    public void setGameDrawn()
    {
        gameDrawn = true;
    }

    public boolean isGameDrawn()
    {
        return gameDrawn;
    }

}
