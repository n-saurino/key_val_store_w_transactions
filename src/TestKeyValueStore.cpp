#include <gtest/gtest.h>
#include "KeyValueStore.hpp" // Include the header file of the implementation

// ✅ **Fixture Class for Setup & Teardown**
class KeyValueStoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        kvs = std::make_unique<KeyValueStore>(); // Create a new KeyValueStore before each test
    }

    void TearDown() override {
        kvs.reset(); // Reset after each test
    }

    std::unique_ptr<KeyValueStore> kvs;
};

// ✅ **Test 1: Basic Get/Set/Delete Operations**
TEST_F(KeyValueStoreTest, BasicOperations) {
    kvs->Set("x", "10");
    EXPECT_EQ(kvs->Get("x"), "10");

    kvs->Set("x", "20");
    EXPECT_EQ(kvs->Get("x"), "20");

    kvs->Delete("x");
    EXPECT_EQ(kvs->Get("x"), "NULL"); // Key should be deleted
}

// ✅ **Test 2: Commit Persists Changes**
TEST_F(KeyValueStoreTest, CommitChangesPersist) {
    kvs->Begin();
    kvs->Set("y", "50");
    EXPECT_EQ(kvs->Get("y"), "50"); // Change is in transaction

    EXPECT_EQ(kvs->Commit(), true);
    EXPECT_EQ(kvs->Get("y"), "50"); // Now in the global store

    EXPECT_EQ(kvs->Rollback(), false); // Rollback should do nothing since there are no transactions
    EXPECT_EQ(kvs->Get("y"), "50"); // Commit should have persisted changes
}

// ✅ **Test 3: Rollback Discards Changes**
TEST_F(KeyValueStoreTest, RollbackDiscardsChanges) {
    kvs->Begin();
    kvs->Set("z", "30");
    EXPECT_EQ(kvs->Get("z"), "30"); // Set inside transaction

    EXPECT_EQ(kvs->Rollback(), true);
    EXPECT_EQ(kvs->Get("z"), "NULL"); // Rollback should discard
}

// ✅ **Test 4: Nested Transactions Should Merge Correctly**
TEST_F(KeyValueStoreTest, NestedTransactionsMergeProperly) {
    kvs->Begin(); // T1
    kvs->Set("a", "5");
    EXPECT_EQ(kvs->Get("a"), "5");

    kvs->Begin(); // T2
    kvs->Set("a", "15"); 
    EXPECT_EQ(kvs->Get("a"), "15");

    EXPECT_EQ(kvs->Commit(), true); // T2 merges into T1
    EXPECT_EQ(kvs->Get("a"), "15");

    EXPECT_EQ(kvs->Commit(), true); // T1 commits to global
    EXPECT_EQ(kvs->Get("a"), "15"); // Final value in global store
}

// ✅ **Test 5: Rollback in Nested Transactions**
TEST_F(KeyValueStoreTest, NestedTransactionsRollback) {
    kvs->Begin(); // T1
    kvs->Set("b", "100");
    EXPECT_EQ(kvs->Get("b"), "100");

    kvs->Begin(); // T2
    kvs->Set("b", "200");
    EXPECT_EQ(kvs->Get("b"), "200");

    EXPECT_EQ(kvs->Rollback(), true); // Rollback T2
    EXPECT_EQ(kvs->Get("b"), "100"); // Should revert to T1's value

    EXPECT_EQ(kvs->Commit(), true); // Commit T1
    EXPECT_EQ(kvs->Get("b"), "100"); // Final committed value
}

// ✅ **Test 6: Delete Within Transactions**
TEST_F(KeyValueStoreTest, TransactionDelete) {
    kvs->Set("c", "500");
    EXPECT_EQ(kvs->Get("c"), "500");

    kvs->Begin();
    kvs->Delete("c");
    EXPECT_EQ(kvs->Get("c"), "NULL"); // Deleted in transaction

    EXPECT_EQ(kvs->Rollback(), true);
    EXPECT_EQ(kvs->Get("c"), "500"); // Should revert to original value
}

// ✅ **Test 7: Commit a Delete**
TEST_F(KeyValueStoreTest, CommitDelete) {
    kvs->Set("d", "900");
    EXPECT_EQ(kvs->Get("d"), "900");

    kvs->Begin();
    kvs->Delete("d");
    EXPECT_EQ(kvs->Get("d"), "NULL"); // Deleted in transaction

    EXPECT_EQ(kvs->Commit(), true);
    EXPECT_EQ(kvs->Get("d"), "NULL"); // Delete should persist after commit
}

// ✅ **Test 8: Rollback on Empty Transaction Should Do Nothing**
TEST_F(KeyValueStoreTest, RollbackEmptyTransaction) {
    EXPECT_EQ(kvs->Rollback(), false); // No transaction to rollback
}

// ✅ **Test 9: Commit on Empty Transaction Should Do Nothing**
TEST_F(KeyValueStoreTest, CommitEmptyTransaction) {
    EXPECT_EQ(kvs->Commit(), false); // No transaction to commit
}

// ✅ **Test 10: Rollback Should Not Affect Parent Transactions**
TEST_F(KeyValueStoreTest, RollbackDoesNotAffectParent) {
    kvs->Set("e", "1000");
    EXPECT_EQ(kvs->Get("e"), "1000");

    kvs->Begin(); // T1
    kvs->Set("e", "2000");
    EXPECT_EQ(kvs->Get("e"), "2000");

    kvs->Begin(); // T2
    kvs->Set("e", "3000");
    EXPECT_EQ(kvs->Get("e"), "3000");

    EXPECT_EQ(kvs->Rollback(), true); // Rollback T2
    EXPECT_EQ(kvs->Get("e"), "2000"); // Should revert to T1's value

    EXPECT_EQ(kvs->Rollback(), true); // Rollback T1
    EXPECT_EQ(kvs->Get("e"), "1000"); // Should revert to global store
}
