# Implement a Transactional Key-Value Store

## Problem Description

Design and implement a key-value store that supports basic operations and transaction management. The implementation should provide both basic key-value operations and transactional capabilities with nested transaction support.

## Requirements

### Basic Operations

1. `set(key, value)`
   - Set the key to the given value
   - If the key already exists, update its value

2. `get(key)`
   - Return the value associated with key
   - If the key does not exist, return "NULL"

3. `delete(key)`
   - Remove key from the store
   - If key does not exist, do nothing

### Transaction Support

The key-value store must support transactions with rollback and commit functionality:

1. `begin()`
   - Start a new transaction
   - Creates a new scope for changes

2. `commit()`
   - Commit all changes made within the current transaction
   - Apply changes to the global key-value store

3. `rollback()`
   - Discard all changes made within the current transaction
   - Return "NO TRANSACTION" if no transaction is active

## Implementation Interface

```cpp
class KeyValueStore {
public:
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    void deleteKey(const std::string& key);
    
    void begin();   // Start a new transaction
    bool commit();  // Commit the current transaction
    bool rollback(); // Rollback the current transaction
};
```

## Constraints

- Transactions must support nesting, where each `begin()` creates a new scope of changes
- `rollback()` must return "NO TRANSACTION" when no transaction is active
- `commit()` should do nothing when no transaction is active
- All operations should maintain consistency of the key-value store

## Follow-Up Questions

1. How would you modify this implementation to support multi-threading?
2. Can you optimize read-heavy workloads by using different data structures?
3. What would be the best approach to store persisted transactions to recover from failures?

## Example Usage

```cpp
KeyValueStore store;

store.set("key1", "value1");
store.begin();             // Start transaction 1
store.set("key2", "value2");
store.begin();             // Start nested transaction 2
store.set("key1", "value3");
store.rollback();          // Rollback transaction 2
store.commit();            // Commit transaction 1

assert(store.get("key1") == "value1");
assert(store.get("key2") == "value2");
```
