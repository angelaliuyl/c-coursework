import DBException.*;

public class CmdDropDatabase extends DBCmd {

    private final String databaseName;

    public CmdDropDatabase(String databaseName) {
        this.databaseName = databaseName;
    }

    @Override
    public String query(DBServer server) throws DBException {
        try {
            if(!DBFileIO.databaseExists(this.databaseName)) {
                throw new DoesNotExistException("database");
            }
            if(server.getCurrentDatabase() != null &&
                    server.getCurrentDatabase().equals(this.databaseName)) {
                server.setCurrentDatabase(null);
            }
            DBFileIO.deleteDatabase(this.databaseName);
            return "[OK]";
        } catch (DBException dbe) {
            throw new QueryException("DROP DATABASE", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "\nDatabase name =" + this.databaseName;
    }
}
