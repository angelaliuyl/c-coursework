public enum TokenType {
    FLT {
        @Override
        public String toString() {
            return "FloatLiteral";
        }
    },
    INT {
        @Override
        public String toString() {
            return "IntegerLiteral";
        }
    },
    BOL {
        @Override
        public String toString() {
            return "BooleanLiteral";
        }
    },
    ID {
        @Override
        public String toString() {
            return "Name";
        }
    },
    STR {
        @Override
        public String toString() {
            return "StringLiteral";
        }
    },
    OP {
        @Override
        public String toString() {
            return "Operator";
        }
    },
    SYM {
        @Override
        public String toString() {
            return "Symbol";
        }
    },
    CT {
        @Override
        public String toString() {
            return "CommandType";
        }
    },
    KW {
        @Override
        public String toString() {
            return "Keyword";
        }
    }
}
