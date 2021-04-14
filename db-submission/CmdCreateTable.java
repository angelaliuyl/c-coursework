import DBException.*;

public class CmdCreateTable extends DBCmd {

    private final String tableName;

    public CmdCreateTable(String tableName) {
        this.tableName = tableName;
    }

    @Override
    public String query(DBServer server) throws DBException {
        try {
            String databaseName = server.getCurrentDatabase();
            if (databaseName == null) {
                throw new QueryException("CREATE TABLE", "No database currently in use");
            }
            DBFileIO.createTable(this.tableName, databaseName);
            this.attributeNames.add(0, "id");
            String[] columnHeadings =
                    this.attributeNames.toArray(new String[0]);
            DBTable table = new DBTable(databaseName,
                    this.tableName, columnHeadings);
            DBFileIO.saveTableToFile(table);
            return "[OK]";
        } catch (DBException dbe) {
            throw new QueryException("CREATE TABLE", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "\nTable name :" + this.tableName;
    }
}
