import DBException.*;

import java.util.*;

public class CmdUpdate extends DBCmd {

    private final String tableName;
    private final List<String> attributeNames;
    private final List<String> values;

    public CmdUpdate(String tableName, List<String> attributeNames,
                     List<String> values) throws DBException {
        this.tableName = tableName;
        if (attributeNames.size() != values.size()) {
            throw new MismatchException("number of attributes", "number of values");
        }
        this.attributeNames = attributeNames;
        this.values = values;
    }

    @Override
    public String query(DBServer server) throws DBException {
        try {
            String databaseName = server.getCurrentDatabase();
            String tableName = this.tableName;
            int changeCount = 0;
            if (!DBFileIO.tableExists(databaseName,
                    tableName)) {
                return "[ERROR] Table does not exist";
            }
            DBTable table = DBFileIO.getTableFromFile(databaseName, tableName);
            for (int i = 0; i < table.getNumberOfRows(); i++) {
                if (this.condition.evaluateCondition(table, i)) {
                    for (int j = 0; j < this.values.size(); j++) {
                        table.setValue(this.attributeNames.get(j),
                                i, this.values.get(j));
                        changeCount++;
                    }
                }
            }
            DBFileIO.saveTableToFile(table);
            return "[OK] Updated " + changeCount + " entry(s).";
        } catch (DBException dbe) {
            throw new QueryException("UPDATE", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "\nTable name " + this.tableName +
                "\nAttribute names " + this.attributeNames +
                "\nValues " + this.values;
    }
}
