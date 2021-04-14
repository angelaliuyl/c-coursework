import DBException.*;

import java.util.regex.*;

public class Tokenizer {
    private final String incomingCommand;
    private int cursor;
    private final String[] commandTypes = { "USE", "CREATE", "DROP", "ALTER",
            "INSERT", "SELECT", "UPDATE", "DELETE", "JOIN" };
    private final String[] keywords = { "DATABASE", "TABLE", "INTO", "VALUES",
            "FROM", "WHERE", "SET", "AND", "ON", "DROP", "ADD" };
    private final String[] regex = {"[ ]*(?<operator>[><]=?|[!=]=|LIKE)[ ]*",
            "[ ]*(?<float>[\\d]+\\.[\\d]+)[ ]*",
            "[ ]*(?<integer>[\\d]+)[ ]*",
            "[ ]*(?<bool>false|true)[ ]*",
            "[ ]*(?<alphanum>[a-zA-Z0-9]+)[ ]*",
            "[ ]*(?:')(?<quotedstring>[^'\\t]+)(?:')[ ]*",
            "[ ]*(?<symbol>[*;,()=])[ ]*"};
    private final String[] regexGroup = { "operator", "float", "integer", "bool",
            "alphanum", "quotedstring", "symbol" };
    private final TokenType[] regexType = { TokenType.OP, TokenType.FLT, TokenType.INT,
            TokenType.BOL,TokenType.ID, TokenType.STR, TokenType.SYM };
    private int scannedLength;


    public Tokenizer(String incomingCommand) throws NullPointerException{
        if (incomingCommand == null) {
            throw new NullPointerException();
        }
        this.incomingCommand = incomingCommand;
        cursor = 0;

    }

    private boolean noMoreTokens() {
        return !(this.cursor < this.incomingCommand.length());
    }

    public Token lookAtNextToken() throws DBException {
        if(this.noMoreTokens()) {
            throw new UnexpectedEndException();
        }

        String unprocessedString = this.incomingCommand.substring(this.cursor);
        String tokenContent;
        TokenType type;
        Pattern p;
        Matcher m;

        for (int i = 0; i < regex.length; i++) {
            p = Pattern.compile(regex[i]);
            m = p.matcher(unprocessedString);
            if (m.lookingAt()) {
                tokenContent = m.group(regexGroup[i]);
                type = regexType[i];
                if (type == TokenType.ID) {
                    if (isCommandType(tokenContent)) {
                        type = TokenType.CT;
                        tokenContent = tokenContent.toUpperCase();
                    }
                    else if (isKeyword(tokenContent)) {
                        type = TokenType.KW;
                        tokenContent = tokenContent.toUpperCase();
                    }
                }
                Token token = new Token(type, tokenContent);
                this.scannedLength += m.group().length();
                //System.out.println("next token: " + token);
                return token;
            }
        }
        throw new BadTokenException(unprocessedString);
    }

    public Token scanNextToken() throws DBException {
        //resets scannedLength
        this.scannedLength = 0;
        Token nextToken = lookAtNextToken();
        //moves cursor
        this.cursor += this.scannedLength;
        //resets scannedLength
        this.scannedLength = 0;
        return nextToken;
    }

    private boolean isCommandType(String tokenContent) {
        String allCappedString = tokenContent.toUpperCase();
        for (String commandType : commandTypes) {
            if (allCappedString.equals(commandType)) {
                return true;
            }
        }
        return false;
    }

    private boolean isKeyword(String tokenContent) {
        String allCappedString = tokenContent.toUpperCase();
        for (String keyword : keywords) {
            if (allCappedString.equals(keyword)) {
                return true;
            }
        }
        return false;
    }
}
