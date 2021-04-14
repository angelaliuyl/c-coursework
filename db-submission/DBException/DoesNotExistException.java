package DBException;

public class DoesNotExistException extends DBException {
    String nonexistentItem;

    public DoesNotExistException(String nonexistentItem) {
        this.nonexistentItem = nonexistentItem;
    }

    @Override
    public String toString() {
        return this.getClass().getName() +
                ": " + nonexistentItem + " does not exist.";
    }
}
