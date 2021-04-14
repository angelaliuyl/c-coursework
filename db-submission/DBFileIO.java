import DBException.*;

import java.io.*;
import java.util.*;

public class DBFileIO {
    public static List<String> readLinesFromFile(String fileName) throws DBException {
        File fileToOpen = new File(fileName);
        try {
            FileReader reader = new FileReader(fileToOpen);
            BufferedReader buffReader = new BufferedReader(reader);
            List<String> lines = new ArrayList<>();
            String currentLine;
            try {
                while ((currentLine = buffReader.readLine()) != null) {
                    //System.out.println("line = " + line);
                    lines.add(currentLine);
                }
            } catch (IOException e) {
                throw new FileIOException("Error reading from file.");
            }
            try {
                reader.close();
            } catch (IOException e) {
                throw new FileIOException("Error closing file.");
            }
            return lines;
        } catch (FileNotFoundException e) {
            throw new FileIOException("File does not exist");
        }
    }

    public static DBTable getTableFromFile(String databaseName, String tableName) throws DBException {
        String fileName = "databases" + File.separator + databaseName
                + File.separator + tableName + ".tab";
        List<String> stringLines = readLinesFromFile(fileName);
        String[] columnHeadings = stringLines.get(0).split("\\t");
        DBTable extractedTable = new DBTable(databaseName, tableName, columnHeadings);
        for (int i = 1; i < stringLines.size(); i++) {
            String[] line = stringLines.get(i).split("\\t");
            extractedTable.addNewRow(line);
            //System.out.println(table.getRow(i-1));
        }
        return extractedTable;
    }

    public static void saveTableToFile(DBTable tableToSave) throws DBException {
        String databaseName = tableToSave.getDatabaseName();
        String tableName = tableToSave.getTableName();
        String fileName = "databases" + File.separator
                + databaseName + File.separator + tableName + ".tab";
        File fileToOpen = new File(fileName);
        try {
            FileWriter writer = new FileWriter(fileToOpen);
            writer.write(tableToSave.toFileOutput());
            writer.flush();
            writer.close();
        } catch (FileNotFoundException e) {
            throw new FileIOException("File does not exist");
        } catch (IOException e) {
            throw new FileIOException("Error in accessing file.");
        }
    }

    public static boolean databaseExists(String databaseName) {
        File databaseDirectory = new File("databases" + File.separator
                + databaseName);
        return databaseDirectory.isDirectory();
    }

    public static boolean tableExists(String databaseName, String tableName) {
        File tableFile = new File("databases" + File.separator
                + databaseName + File.separator + tableName + ".tab");
        return tableFile.isFile();
    }

    public static void createDatabase(String databaseName) throws DBException {
        File databaseDirectory = new File("databases" + File.separator
                + databaseName);
        if (databaseExists(databaseName)) {
            throw new FileIOException("Database already exists");
        }
        if (!databaseDirectory.mkdir()) {
            throw new FileIOException("Database could not be created");
        }
    }

    public static void createTable(String tableName, String databaseName) throws DBException {
        if (!databaseExists(databaseName)) {
            throw new FileIOException("Database does not exist");
        }
        if (tableExists(databaseName, tableName)) {
            throw new FileIOException("Table already exists");
        }
        File tableFile = new File("databases" + File.separator
                + databaseName + File.separator + tableName + ".tab");
        try {
            tableFile.createNewFile();
        } catch (IOException ioe) {
            throw new FileIOException("File could not be created");
        }
    }

    public static void assertDatabasesFolderExists() throws DBException {
        File databasesFolder = new File("databases");
        if (!databasesFolder.isDirectory() && !databasesFolder.mkdir()) {
            throw new FileIOException("Error in creating \"databases\" directory");
        }
    }

    public static void deleteDatabase(String databaseName) throws DBException {
        File databaseDirectory = new File("databases" + File.separator + databaseName);
        File[] tableFiles = databaseDirectory.listFiles();
        if (tableFiles != null) {
            for (File tableFile : tableFiles) {
                if (!tableFile.delete()) {
                    throw new FileIOException("Error in deleting files in directory");
                }
            }
        }
        if (!databaseDirectory.delete()) {
            throw new FileIOException("Error in deleting directory");
        }
    }

    public static void deleteTable(String tableName, String databaseName) throws DBException {
        File tableFile = new File("databases" + File.separator
                + databaseName + File.separator + tableName + ".tab");
        if (!tableFile.delete()) {
            throw new FileIOException("Error in deleting table file");
        }
    }
}