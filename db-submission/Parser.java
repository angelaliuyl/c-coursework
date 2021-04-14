import DBException.*;

import java.util.ArrayList;
import java.util.List;

public class Parser {

    DBCmd parsedCommand;
    String databaseName;
    String tableName1;
    String tableName2;
    String alterationType;
    String structure;
    List<String> attributeNames = new ArrayList<>();
    List<String> values = new ArrayList<>();
    DBCondition condition;

    public DBCmd parseCommand(String commandLine) throws DBException{
        //<Command> ::= <CommandType>;
        Tokenizer tokenizer = new Tokenizer(commandLine);
        //<CommandType>
        parseCommandType(tokenizer);
        //;
        Token currentToken = tokenizer.scanNextToken();
        currentToken.assertContentMatches(";");

        return parsedCommand;
    }

    private void parseCommandType(Tokenizer tokenizer) throws DBException {
        //<CommandType> ::= <Use> | <Create> | <Drop> | <Alter> | <Insert> |
        //                  <Select> | <Update> | <Delete> | <Join>
        Token currentToken = tokenizer.scanNextToken();
        switch (currentToken.getTokenContent()) {
            case "USE" -> parseUse(tokenizer);
            case "CREATE" -> parseCreate(tokenizer);
            case "DROP" -> parseDrop(tokenizer);
            case "ALTER" -> parseAlter(tokenizer);
            case "INSERT" -> parseInsert(tokenizer);
            case "SELECT" -> parseSelect(tokenizer);
            case "UPDATE" -> parseUpdate(tokenizer);
            case "DELETE" -> parseDelete(tokenizer);
            case "JOIN" -> parseJoin(tokenizer);
            default -> throw new UnexpectedTermException("implemented command type",
                    currentToken.getTokenContent());
        }
    }

    private void parseUse(Tokenizer tokenizer) throws DBException {
        //<Use> ::=  USE <DatabaseName>
        try {
            //<DatabaseName>
            Token currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID, "<DatabaseName>");
            databaseName = currentToken.getTokenContent();
            parsedCommand = new CmdUse(databaseName);
        } catch (DBException dbe) {
            throw new CommandParseException("USE", dbe.toString());
        }
    }

    private void parseCreate(Tokenizer tokenizer) throws DBException {
        //<Create>  ::= <CreateDatabase> | <CreateTable>
        //DATABASE | TABLE
        Token currentToken = tokenizer.scanNextToken();
        switch (currentToken.getTokenContent()) {
            case "DATABASE" -> parseCreateDatabase(tokenizer);
            case "TABLE" -> parseCreateTable(tokenizer);
            default -> throw new UnexpectedTermException("DATABASE | TABLE",
                    currentToken.getTokenContent());
        }
    }

    private void parseCreateDatabase(Tokenizer tokenizer) throws DBException {
        //<CreateDatabase> ::=  CREATE DATABASE <DatabaseName>
        //<DatabaseName>
        try {
            Token currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID,"<DatabaseName>");
            databaseName = currentToken.getTokenContent();

            parsedCommand = new CmdCreateDatabase(databaseName);
        } catch  (DBException dbe) {
            throw new CommandParseException("CREATE DATABASE", dbe.toString());
        }
    }

    private void parseCreateTable(Tokenizer tokenizer) throws DBException {
        //<CreateTable> ::= CREATE TABLE <TableName> |
        //                  CREATE TABLE <TableName> (<AttributeList>)
        //<TableName>
        try {
            Token currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID, "<TableName>");
            tableName1 = currentToken.getTokenContent();
            //(<AttributeList>)
            //(
            currentToken = tokenizer.lookAtNextToken();
            if (currentToken.getTokenContent().equals("(")) {
                tokenizer.scanNextToken();
                //<AttributeList>
                parseAttributeList(tokenizer);
                //)
                currentToken = tokenizer.scanNextToken();
                currentToken.assertContentMatches(")");
            }

            parsedCommand = new CmdCreateTable(tableName1);
            parsedCommand.setAttributeNames(attributeNames);
        } catch  (DBException dbe) {
            throw new CommandParseException("CREATE TABLE", dbe.toString());
        }
    }

    private void parseDrop(Tokenizer tokenizer) throws DBException {
        //<Drop>    ::=  DROP <Structure> <StructureName>
        try {
            //<Structure>
            Token currentToken = tokenizer.scanNextToken();
            structure = currentToken.getTokenContent();
            if (!structure.equals("TABLE")
                    && !structure.equals("DATABASE")) {
                throw new UnexpectedTermException("DATABASE | TABLE", structure);
            }
            //<StructureName>
            currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID,"<StructureName>");

            if (structure.equals("TABLE")) {
                tableName1 = currentToken.getTokenContent();
                parsedCommand = new CmdDropTable(tableName1);
            }
            else {
                databaseName = currentToken.getTokenContent();
                parsedCommand = new CmdDropDatabase(databaseName);
            }
        } catch (DBException dbe) {
            throw new CommandParseException("DROP", dbe.toString());
        }
    }

    private void parseAlter(Tokenizer tokenizer) throws DBException {
        //<Alter>   ::=  ALTER TABLE <TableName> <AlterationType> <AttributeName>
        try {
            //TABLE
            Token currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("TABLE");
            //<TableName>
            currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID,"<TableName>");
            tableName1 = currentToken.getTokenContent();
            //<AlterationType> ::=  ADD | DROP
            currentToken = tokenizer.scanNextToken();
            alterationType = currentToken.getTokenContent();
            if (!alterationType.equals("ADD") && !alterationType.equals("DROP")) {
                throw new UnexpectedTermException("ADD | DROP", alterationType);
            }
            //<AttributeName>
            currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID, "<AttributeName>");
            attributeNames.add(currentToken.getTokenContent());
            if (alterationType.equals("ADD")) {
                parsedCommand = new CmdAlterAdd(tableName1, attributeNames.get(0));
            }
            else {
                parsedCommand = new CmdAlterDrop(tableName1, attributeNames.get(0));
            }
        } catch (DBException dbe) {
            throw new CommandParseException("ALTER", dbe.toString());
        }
    }

    private void parseInsert(Tokenizer tokenizer) throws DBException {
        //<Insert>  ::= INSERT INTO <TableName> VALUES (<ValueList>)
        try {
            //INTO
            Token currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("INTO");
            //<TableName>
            currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID, "<TableName>");
            tableName1 = currentToken.getTokenContent();
            //VALUES
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("VALUES");
            //(
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("(");
            //<ValueList>
            parseValueList(tokenizer);
            //)
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches(")");
            parsedCommand = new CmdInsert(tableName1, values);
        } catch (DBException dbe) {
            throw new CommandParseException("INSERT", dbe.toString());
        }
    }

    private void parseSelect(Tokenizer tokenizer) throws DBException {
        //<Select>  ::= SELECT <WildAttribList> FROM <TableName> |
        //              SELECT <WildAttribList> FROM <TableName> WHERE <Condition>
        try {
            //<WildAttribList> ::=  <AttributeList> | *
            Token currentToken = tokenizer.lookAtNextToken();
            if (currentToken.getTokenContent().equals("*")) {
                tokenizer.scanNextToken();
            }
            else {
                try {
                    parseAttributeList(tokenizer);
                } catch (DBException e) {
                    throw new UnexpectedTermException("* | <AttributeList>",
                            currentToken.getTokenContent());
                }
            }
            //FROM
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("FROM");
            //<TableName>
            currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID, "<TableName>");
            tableName1 = currentToken.getTokenContent();
            //WHERE <Condition> (optional)
            currentToken = tokenizer.lookAtNextToken();
            if (currentToken.getTokenContent().equals("WHERE")) {
                tokenizer.scanNextToken();
                condition = parseCondition(tokenizer);
            }
            parsedCommand = new CmdSelect(tableName1);
            //Optional
            parsedCommand.setAttributeNames(attributeNames);
            parsedCommand.setCondition(condition);
        } catch (DBException dbe) {
            throw new CommandParseException("SELECT", dbe.toString());
        }
    }

    private void parseUpdate(Tokenizer tokenizer) throws DBException {
        //<Update>  ::= UPDATE <TableName> SET <NameValueList> WHERE <Condition>
        try {
            //<TableName>
            Token currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID, "<TableName>");
            tableName1 = (currentToken.getTokenContent());
            //SET
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("SET");
            //<NameValueList>
            parseNameValueList(tokenizer);
            //WHERE
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("WHERE");
            //<Condition>
            DBCondition condition = parseCondition(tokenizer);

            parsedCommand = new CmdUpdate(tableName1, attributeNames, values);
            parsedCommand.setCondition(condition);
        } catch (DBException dbe) {
            throw new CommandParseException("UPDATE", dbe.toString());
        }
    }

    private void parseDelete(Tokenizer tokenizer) throws DBException {
        //<Delete>  ::= DELETE FROM <TableName> WHERE <Condition>
        try {
            //FROM
            Token currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("FROM");
            //<TableName>
            currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID, "<TableName>");
            tableName1 = currentToken.getTokenContent();
            //WHERE
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("WHERE");
            //<Condition>
            condition = parseCondition(tokenizer);
            parsedCommand = new CmdDelete(tableName1, condition);
        } catch (DBException dbe) {
            throw new CommandParseException("DELETE", dbe.toString());
        }
    }

    private void parseJoin(Tokenizer tokenizer) throws DBException {
        //<Join>    ::=  JOIN <TableName> AND <TableName> ON
        //               <AttributeName> AND <AttributeName>
        try {
            //<TableName> (Table1)
            Token currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID, "<TableName>");
            tableName1 = currentToken.getTokenContent();
            //AND
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("AND");
            //<TableName> (Table2)
            currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID, "<TableName>");
            tableName2 = currentToken.getTokenContent();
            //ON
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("ON");
            //<AttributeName> (attribute1)
            currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID, "<AttributeName>");
            attributeNames.add(currentToken.getTokenContent());
            //AND
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("AND");
            //<AttributeName> (attribute2)
            currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.ID, "<AttributeName>");
            attributeNames.add(currentToken.getTokenContent());

            parsedCommand = new CmdJoin(tableName1, tableName2,
                    attributeNames.get(0), attributeNames.get(1));
        } catch (DBException dbe) {
            throw new CommandParseException("JOIN", dbe.toString());
        }
    }

    private void parseNameValueList(Tokenizer tokenizer) throws DBException {
        //<NameValueList>  ::=  <NameValuePair> | <NameValuePair>,<NameValueList>
        //<NameValuePair>
        parseNameValuePair(tokenizer);
        //,<NameValueList> (optional)
        Token currentToken = tokenizer.lookAtNextToken();
        if (currentToken.getTokenContent().equals(",")) {
            tokenizer.scanNextToken();
            parseNameValueList(tokenizer);
        }
    }

    private void parseNameValuePair(Tokenizer tokenizer) throws DBException {
        //<NameValuePair>  ::=  <AttributeName>=<Value>
        //<AttributeName>
        Token currentToken = tokenizer.scanNextToken();
        currentToken.assertTypeMatches(TokenType.ID, "<AttributeName>");
        attributeNames.add(currentToken.getTokenContent());
        //=
        currentToken = tokenizer.scanNextToken();
        currentToken.assertContentMatches("=");
        //<Value>
        currentToken = tokenizer.scanNextToken();
        currentToken.assertTypeIsValue();
        values.add(currentToken.getTokenContent());
    }

    private void parseValueList(Tokenizer tokenizer) throws DBException {
        //<ValueList>   ::= <Value> | <Value>,<ValueList>
        //<Value>
        Token currentToken = tokenizer.scanNextToken();
        currentToken.assertTypeIsValue();
        values.add(currentToken.getTokenContent());
        //,<ValueList> (optional)
        currentToken = tokenizer.lookAtNextToken();
        if (currentToken.getTokenContent().equals(",")) {
            tokenizer.scanNextToken();
            parseValueList(tokenizer);
        }
    }

    private void parseAttributeList(Tokenizer tokenizer) throws DBException {
        //<AttributeList>  ::=  <AttributeName> |
        //                      <AttributeName>,<AttributeList>
        //<AttributeName>
        Token currentToken = tokenizer.scanNextToken();
        currentToken.assertTypeMatches(TokenType.ID, "<AttributeName>");
        attributeNames.add(currentToken.getTokenContent());
        //,<AttributeList> (optional)
        currentToken = tokenizer.lookAtNextToken();
        if (currentToken.getTokenContent().equals(",")) {
            tokenizer.scanNextToken();
            parseAttributeList(tokenizer);
        }
    }

    private DBCondition parseCondition(Tokenizer tokenizer) throws DBException {
        //<Condition>   ::= (<Condition>) AND (<Condition>)  |
        //                  (<Condition>) OR (<Condition>)   |
        //                  <AttributeName><Operator><Value>
        Token currentToken = tokenizer.lookAtNextToken();
        //<AttributeName><Operator><Value>
        if (currentToken.getType() == TokenType.ID) {
            //<AttributeName>
            currentToken = tokenizer.scanNextToken();
            String attributeName = currentToken.getTokenContent();
            //<Operator>
            currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeMatches(TokenType.OP, "<Operator>");
            String opString = currentToken.getTokenContent();
            //<Value>
            currentToken = tokenizer.scanNextToken();
            currentToken.assertTypeIsValue();
            String strValue = currentToken.getTokenContent();
            TokenType valueType = currentToken.getType();
            return new DBCondition(attributeName, opString,
                    strValue, valueType);
        }
        //(<Condition>) AND (<Condition>)  |
        //(<Condition>) OR (<Condition>)
        else if (currentToken.getTokenContent().equals("(")) {
            //(
            tokenizer.scanNextToken();
            //<Condition>
            DBCondition left = parseCondition(tokenizer);
            //)
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches(")");
            //AND | OR
            currentToken = tokenizer.scanNextToken();
            String keyword = currentToken.getTokenContent();
            if (!keyword.equals("AND") && !keyword.equals("OR")) {
                throw new UnexpectedTermException("AND | OR", keyword);
            }
            String opString = currentToken.getTokenContent();
            //(
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches("(");
            //<Condition>
            DBCondition right = parseCondition(tokenizer);
            //)
            currentToken = tokenizer.scanNextToken();
            currentToken.assertContentMatches(")");
            return new DBCondition(left, opString, right);
        }
        throw new UnexpectedTermException("\"(<Condition>) <AND|OR> (<Condition>)\" " +
                "| \"<AttributeName><Operator><Value\"",
                currentToken.getTokenContent());
    }

}
