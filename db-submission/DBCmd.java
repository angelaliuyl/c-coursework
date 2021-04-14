import DBException.*;

import java.util.*;

public abstract class DBCmd {
    List<String> attributeNames = new ArrayList<>();
    DBCondition condition;

    public void setCondition(DBCondition condition) {
        this.condition = condition;
    }

    public void setAttributeNames(List<String> attributeNames) {
        this.attributeNames = attributeNames;
    }

    abstract public String query(DBServer server) throws DBException;

    @Override
    abstract public String toString();

}
