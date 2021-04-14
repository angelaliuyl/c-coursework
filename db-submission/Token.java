import DBException.*;

public class Token {
    private final TokenType type;
    private final String tokenContent;

    public Token(TokenType type, String tokenContent) {
        this.type = type;
        this.tokenContent = tokenContent;
    }

    public TokenType getType() {
        return this.type;
    }

    public String getTokenContent() {
        return this.tokenContent;
    }

    public void assertContentMatches(String expectedContent) throws DBException {
        if (!this.tokenContent.equals(expectedContent)) {
            throw new UnexpectedTermException(expectedContent, this.tokenContent);
        }
    }

    public void assertTypeMatches(TokenType expectedType, String expectedContent) throws DBException {
        if (this.type != expectedType) {
            throw new UnexpectedTermException(expectedContent, this.type.toString());
        }
    }

    public void assertTypeIsValue() throws DBException {
        if (this.getType() != TokenType.STR
                && this.getType() != TokenType.INT
                && this.getType() != TokenType.FLT
                && this.getType() != TokenType.BOL) {
            throw new UnexpectedTermException("'<StringLiteral>' | " +
                    "<IntegerLiteral> | <FloatLiteral> | <BooleanLiteral>",
                    this.getType().toString());
        }
    }

    @Override
    public String toString() {
        return "Token{" +
                "type=" + type +
                ", tokenContent='" + tokenContent + '\'' +
                '}';
    }
}
