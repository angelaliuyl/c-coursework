import DBException.*;

public class CmdCreateDatabase extends DBCmd {

    private final String databaseName;

    public CmdCreateDatabase(String databaseName) {
        this.databaseName = databaseName;
    }

    @Override
    public String query(DBServer server) throws DBException {
        try {
            DBFileIO.createDatabase(this.databaseName);
            return "[OK]";
        } catch (DBException dbe) {
            throw new QueryException("CREATE DATABASE", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "\nDatabase name = " + this.databaseName;
    }
}
