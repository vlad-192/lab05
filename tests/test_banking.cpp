#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Account.h"
#include "Transaction.h"

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}

    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
    MOCK_METHOD(void, ChangeBalance, (int diff), (override));
};

TEST(AccountTest, Constructor)
{
    Account account(1, 100);
    EXPECT_EQ(account.id(), 1);
    EXPECT_EQ(account.GetBalance(), 100);
}

TEST(AccountTest, ChangeBalance)
{
    Account account(1, 100);
    account.ChangeBalance(50);
    EXPECT_EQ(account.GetBalance(), 150);
}

TEST(AccountTest, LockUnlock)
{
    Account account(1, 100);
    account.Lock();
    account.Unlock();
    SUCCEED();
}

TEST(TransactionTest, Constructor)
{
    Transaction transaction;
    EXPECT_EQ(transaction.fee(), 1);
}

TEST(TransactionTest, SetFee)
{
    Transaction transaction;
    transaction.set_fee(10);
    EXPECT_EQ(transaction.fee(), 10);
}

TEST(TransactionTest, MakeSuccess)
{
    MockAccount from(1, 100);
    MockAccount to(2, 50);

    Transaction transaction;
    transaction.set_fee(5);

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);

    EXPECT_CALL(from, ChangeBalance(-55)).Times(1);
    EXPECT_CALL(to, ChangeBalance(50)).Times(1);

    EXPECT_TRUE(transaction.Make(from, to, 50));
}

TEST(TransactionTest, MakeNotEnoughMoney)
{
    MockAccount from(1, 40);
    MockAccount to(2, 50);

    Transaction transaction;
    transaction.set_fee(5);

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);

    EXPECT_CALL(from, ChangeBalance(testing::_)).Times(0);
    EXPECT_CALL(to, ChangeBalance(testing::_)).Times(0);

    EXPECT_FALSE(transaction.Make(from, to, 50));
}

TEST(TransactionTest, MakeNegativeSum)
{
    MockAccount from(1, 100);
    MockAccount to(2, 50);

    Transaction transaction;

    EXPECT_FALSE(transaction.Make(from, to, -10));
}

TEST(TransactionTest, MakeZeroSum)
{
    MockAccount from(1, 100);
    MockAccount to(2, 50);

    Transaction transaction;

    EXPECT_FALSE(transaction.Make(from, to, 0));
}

TEST(TransactionTest, MakeSameAccount)
{
    MockAccount account(1, 100);

    Transaction transaction;

    EXPECT_FALSE(transaction.Make(account, account, 10));
}
