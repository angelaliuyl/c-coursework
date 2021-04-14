import OXOExceptions.*;

class OXOController
{
    OXOModel gameModel;
    private int row;
    private int col;

    public OXOController(OXOModel model)
    {
        gameModel = model;
    }

    public void handleIncomingCommand(String command) throws OXOMoveException
    {
        // Setting up players for first move of game
        if(gameModel.getCurrentPlayer() == null) {
            gameModel.setCurrentPlayer(gameModel.getPlayerByNumber(0));
        }
        // Check game is not won or drawn
        if ((!gameModel.isGameDrawn()) && (gameModel.getWinner() == null)) {
            validateInput(command);
            gameModel.setCellOwner(row, col, gameModel.getCurrentPlayer());
            updateState();
            changePlayer();
        }
    }

    private void validateInput(String command) throws OXOMoveException
    {
        // Gets row and column info from command
        if (command.length() != 2) {
            throw new InvalidIdentifierLengthException(command.length());
        }
        char rChar = command.toLowerCase().charAt(0);
        char cChar = command.toLowerCase().charAt(1);

        if (!Character.isLetter(rChar)) {
            throw new InvalidIdentifierCharacterException(command.charAt(0), RowOrColumn.ROW);
        }
        else if (!Character.isDigit(cChar)) {
            throw new InvalidIdentifierCharacterException(command.charAt(1), RowOrColumn.COLUMN);
        }

        int row = (int) rChar - (int) 'a';
        int col = (int) cChar - (int) '1';

        if (row < 0 || row >= gameModel.getNumberOfRows()) {
            throw new OutsideCellRangeException(row, RowOrColumn.ROW);
        }
        else if (col < 0 || col >= gameModel.getNumberOfColumns()) {
            throw new OutsideCellRangeException(col, RowOrColumn.COLUMN);
        }

        if (gameModel.getCellOwner(row, col) != null) {
            throw new CellAlreadyTakenException(row, col);
        }

        this.row = row;
        this.col = col;
    }

    private void changePlayer()
    {
        if (gameModel.getCurrentPlayerNumber() >= gameModel.getNumberOfPlayers() - 1) {
            gameModel.setCurrentPlayer(gameModel.getPlayerByNumber(0));
        }
        else {
            gameModel.incrementCurrentPlayer();
        }

    }

    private void updateState()
    {
        int cellCount = 0;
        for (int r = 0; r < gameModel.getNumberOfRows(); r++) {
            for (int c = 0; c < gameModel.getNumberOfColumns(); c++) {
                OXOPlayer curCell = safeRead(r, c);
                if (curCell != null) {
                    cellCount++;
                    if (checkEast(curCell, r, c) ||
                        checkSEast(curCell, r, c) ||
                        checkSouth(curCell, r, c) ||
                        checkSWest(curCell, r, c))
                    {
                        gameModel.setWinner(gameModel.getCurrentPlayer());
                        return;
                    }
                }
            }
        }
        if (cellCount == gameModel.getNumberOfColumns() * gameModel.getNumberOfRows()) {
            gameModel.setGameDrawn();
        }
    }

    private boolean checkEast(OXOPlayer cur, int row, int col)
    {
        for (int step = 1; step < gameModel.getWinThreshold(); step++) {
            if (safeRead(row, col+step) != cur) {
                return false;
            }
        }
        return true;
    }

    private boolean checkSouth(OXOPlayer cur, int row, int col)
    {
        for (int step = 1; step < gameModel.getWinThreshold(); step++) {
            if (safeRead(row+step, col) != cur) {
                return false;
            }
        }
        return true;
    }

    private boolean checkSEast(OXOPlayer cur, int row, int col)
    {
        for (int step = 1; step < gameModel.getWinThreshold(); step++) {
            if (safeRead(row+step, col+step) != cur) {
                return false;
            }
        }
        return true;
    }

    private boolean checkSWest(OXOPlayer cur, int row, int col)
    {
        for (int step = 1; step < gameModel.getWinThreshold(); step++) {
            if (safeRead(row+step, col-step) != cur) {
                return false;
            }
        }
        return true;
    }

    private OXOPlayer safeRead(int row, int col)
    {
        if (row < 0 || row >= gameModel.getNumberOfRows()) {
            return null;
        }
        else if (col < 0 || col >= gameModel.getNumberOfColumns()) {
            return null;
        }
        else {
            return gameModel.getCellOwner(row, col);
        }
    }

}




