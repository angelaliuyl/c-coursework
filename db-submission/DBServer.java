import DBException.*;

import java.io.*;
import java.net.*;

class DBServer
{
    private String currentDatabase;

    public DBServer(int portNumber)
    {
        try {
            ServerSocket serverSocket = new ServerSocket(portNumber);
            System.out.println("Server Listening");
            DBFileIO.assertDatabasesFolderExists();
            while(true) processNextConnection(serverSocket);
        } catch (IOException | DBException ioe) {
            System.err.println(ioe);
        }
    }

    private void processNextConnection(ServerSocket serverSocket)
    {
        try {
            Socket socket = serverSocket.accept();
            BufferedReader socketReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            BufferedWriter socketWriter = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
            System.out.println("Connection Established");
            while(true) processNextCommand(socketReader, socketWriter);
        } catch(IOException ioe) {
            System.err.println(ioe);
        } catch(NullPointerException npe) {
            System.out.println("Connection Lost");
        }
    }

    private void processNextCommand(BufferedReader socketReader, BufferedWriter socketWriter) throws IOException, NullPointerException
    {
        String incomingCommand = socketReader.readLine();
        System.out.println("Received message: " + incomingCommand);
        Parser parser = new Parser();
        try {
            DBCmd command = parser.parseCommand(incomingCommand);
            String outputMessage = command.query(this);
            System.out.println(outputMessage);
            socketWriter.write(outputMessage);
        } catch (DBException dbe) {
            System.err.println(dbe.toString());
            socketWriter.write("[ERROR]: "+ dbe.toString());
        }
        socketWriter.write("\n" + ((char)4) + "\n");
        socketWriter.flush();
    }

    public void setCurrentDatabase(String databaseName) {
        this.currentDatabase = databaseName;
    }

    public String getCurrentDatabase() {
        return this.currentDatabase;
    }

    public static void main(String args[])
    {
        DBServer server = new DBServer(8888);
    }

}
