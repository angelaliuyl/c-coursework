import DBException.*;

public class DBCondition {
    private DBCondition left;
    private DBCondition right;
    private CondOp operator;
    private String attributeName;
    private String valueString;
    private TokenType valueType;


    public DBCondition(DBCondition left, String opString, DBCondition right) {
        this.left = left;
        this.right = right;
        this.operator = stringToCondOp(opString);
    }

    public DBCondition(String attributeName, String opString, String value, TokenType valueType) {
        this.attributeName = attributeName;
        this.operator = stringToCondOp(opString);
        this.valueString = value;
        this.valueType = valueType;
    }

    public boolean evaluateCondition(DBTable table, int rowNumber) throws DBException {
        if (this.operator == CondOp.AND || this.operator == CondOp.OR) {
            boolean leftCond = this.left.evaluateCondition(table, rowNumber);
            boolean rightCond = this.right.evaluateCondition(table, rowNumber);
            if (this.operator == CondOp.AND) {
                return leftCond && rightCond;
            }
            else {
                return leftCond || rightCond;
            }
        }
        String value = table.getValue(this.attributeName, rowNumber);
        try {
            return switch(this.valueType) {
                case STR -> evaluateStrings(value);
                case INT -> evaluateIntegers(value);
                case FLT -> evaluateFloats(value);
                case BOL -> evaluateBooleans(value);
                default -> false;
            };
        } catch (NumberFormatException nfe) {
            throw new MismatchException("format of attribute " + attributeName,
                    "operator " + operator.toString());
        }
    }

    private CondOp stringToCondOp(String opString) {
        return switch (opString) {
            case "AND" -> CondOp.AND;
            case "OR" -> CondOp.OR;
            case ">" -> CondOp.GT;
            case ">=" -> CondOp.GE;
            case "==" -> CondOp.EQ;
            case "<" -> CondOp.LT;
            case "<=" -> CondOp.LE;
            case "!=" -> CondOp.NE;
            case "LIKE" -> CondOp.LIKE;
            default -> null;
        };
    }

    private boolean evaluateStrings(String tableValue) throws DBException {
        if (this.operator == CondOp.EQ) {
            return this.valueString.equals(tableValue);
        }
        else if (this.operator == CondOp.NE) {
            return !this.valueString.equals(tableValue);
        }
        else if (this.operator == CondOp.LIKE) {
            return tableValue.contains(this.valueString);
        }
        else {
            throw new MismatchException("operator type " +
                    operator.toString(), "value type (string)");
        }
    }

    private boolean evaluateIntegers(String tableValue) throws NumberFormatException, DBException {
        int condValue = Integer.parseInt(this.valueString);
        int value = Integer.parseInt(tableValue);
        switch (this.operator) {
            case GT -> {
                return (value > condValue);
            }
            case GE -> {
                return (value >= condValue);
            }
            case EQ -> {
                return (value == condValue);
            }
            case LT -> {
                return (value < condValue);
            }
            case LE -> {
                return (value <= condValue);
            }
            case NE -> {
                return (value != condValue);
            }
            default -> throw new MismatchException("operator type" + operator.toString(), "value type (int)");
        }
    }

    private boolean evaluateFloats(String tableValue) throws NumberFormatException, DBException {
        float condValue = Float.parseFloat(this.valueString);
        float value = Float.parseFloat(tableValue);
        boolean equal = (Float.compare(value, condValue) == 0);
        switch (this.operator) {
            case GT -> {
                return (value > condValue);
            }
            case GE -> {
                return (value >= condValue);
            }
            case EQ -> {
                return equal;
            }
            case LT -> {
                return (value < condValue);
            }
            case LE -> {
                return (value <= condValue);
            }
            case NE -> {
                return !equal;
            }
            default -> throw new MismatchException("operator type" + operator.toString(), "value type (float)");
        }
    }

    private boolean evaluateBooleans(String tableValue) throws DBException {
        boolean condValue = Boolean.parseBoolean(this.valueString);
        boolean value = Boolean.parseBoolean(tableValue);
        if (this.operator == CondOp.EQ) {
            return value == condValue;
        }
        else if (this.operator == CondOp.NE) {
            return value != condValue;
        }
        else {
            throw new MismatchException("operator type " + operator.toString(), "value type (boolean)");
        }
    }

    private enum CondOp {
        AND, OR,
        GT {
            @Override
            public String toString() {
                return ">";
            }
        }, GE {
            @Override
            public String toString() {
                return ">=";
            }
        }, EQ {
            @Override
            public String toString() {
                return "==";
            }
        }, LT {
            @Override
            public String toString() {
                return "<";
            }
        }, LE {
            @Override
            public String toString() {
                return "<=";
            }
        }, NE {
            @Override
            public String toString() {
                return "!=";
            }
        },
        LIKE
    }

}
