import DBException.*;

public class CmdUse extends DBCmd {
    private final String databaseName;

    public CmdUse(String databaseName) {
        this.databaseName = databaseName;
    }

    @Override
    public String query(DBServer server) throws DBException {
        try {
            if(!DBFileIO.databaseExists(this.databaseName)) {
                throw new DoesNotExistException("database");
            }
            server.setCurrentDatabase(this.databaseName);
            return "[OK]";
        } catch (DBException dbe) {
            throw new QueryException("USE", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "Database name :" + this.databaseName;
    }
}
