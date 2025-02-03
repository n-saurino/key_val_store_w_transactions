Problem Description

You are tasked with designing a key-value store that supports basic operations as well as transaction management.

Your implementation should support the following operations:

Basic Operations
	1.	set(key, value) → Set the key to the given value. If the key already exists, update its value.
	2.	get(key) → Return the value associated with key. If the key does not exist, return "NULL".
	3.	delete(key) → Remove key from the store. If key does not exist, do nothing.

Transaction Support

Your key-value store should support transactions with rollback and commit functionality:
	1.	begin() → Start a new transaction.
	2.	commit() → Commit all changes made within the current transaction and apply them to the global key-value store.
	3.	rollback() → Discard all changes made within the current transaction.

Constraints
	•	Transactions should be nested such that each begin() creates a new scope of changes.
	•	If rollback() is called when no transaction is active, return "NO TRANSACTION".
	•	If commit() is called with no active transaction, do nothing.
    
class KeyValueStore {
public:
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    void deleteKey(const std::string& key);
    
    void begin();   // Start a new transaction
    bool commit();  // Commit the current transaction
    bool rollback(); // Rollback the current transaction
};

Follow-Up Questions
	1.	How would you modify this implementation to support multi-threading?
	2.	Can you optimize read-heavy workloads by using different data structures?
	3.	What would be the best approach to store persisted transactions to recover from failures?