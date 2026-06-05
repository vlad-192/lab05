#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Account.h"
#include "Transaction.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}

    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int diff), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};

TEST(AccountTest, Constructor)
{
    Account account(1, 100);
    EXPECT_EQ(account.id(), 1);
    EXPECT_EQ(account.GetBalance(), 100);
}

TEST(AccountTest, ChangeBalanceWithLock)
{
    Account account(1, 100);
    account.Lock();
    account.ChangeBalance(50);
    account.Unlock();

    EXPECT_EQ(account.GetBalance(), 150);
}

TEST(AccountTest, ChangeBalanceWithoutLockThrows)
{
    Account account(1, 100);

    EXPECT_THROW(account.ChangeBalance(50), std::runtime_error);
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

TEST(TransactionTest, MakeSuccessDirectTest)
{
    Account from(1, 1000);
    Account to(2, 100);

    Transaction transaction;
    transaction.set_fee(1);

    EXPECT_TRUE(transaction.Make(from, to, 100));

    EXPECT_EQ(from.GetBalance(), 1000);
    EXPECT_EQ(to.GetBalance(), 99);
}

TEST(TransactionTest, MakeNegativeSum)
{
    Account from(1, 100);
    Account to(2, 50);

    Transaction transaction;

    EXPECT_THROW(transaction.Make(from, to, -10), std::invalid_argument);
}

TEST(TransactionTest, MakeZeroSum)
{
    Account from(1, 100);
    Account to(2, 50);

    Transaction transaction;

    EXPECT_THROW(transaction.Make(from, to, 0), std::logic_error);
}

TEST(TransactionTest, MakeSameAccount)
{
    Account account(1, 100);

    Transaction transaction;

    EXPECT_THROW(transaction.Make(account, account, 10), std::logic_error);
}

TEST(TransactionMockTest, SuccessfulTransactionCallsRequiredMethods)
{
    MockAccount from(1, 1000);
    MockAccount to(2, 100);

    Transaction transaction;
    transaction.set_fee(1);

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);

    EXPECT_CALL(to, ChangeBalance(100)).Times(1);
    EXPECT_CALL(to, GetBalance())
        .Times(2)
        .WillOnce(Return(200))
        .WillOnce(Return(99));
    EXPECT_CALL(to, ChangeBalance(-101)).Times(1);

    EXPECT_CALL(from, GetBalance())
        .Times(1)
        .WillOnce(Return(1000));

    EXPECT_CALL(to, Unlock()).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);

    EXPECT_TRUE(transaction.Make(from, to, 100));
}

TEST(TransactionMockTest, FailedDebitRollsBackTransaction)
{
    MockAccount from(1, 1000);
    MockAccount to(2, 100);

    Transaction transaction;
    transaction.set_fee(1);

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);

    EXPECT_CALL(to, ChangeBalance(100)).Times(1);
    EXPECT_CALL(to, GetBalance())
        .Times(2)
        .WillOnce(Return(50))
        .WillOnce(Return(50));
    EXPECT_CALL(to, ChangeBalance(-100)).Times(1);

    EXPECT_CALL(from, GetBalance())
        .Times(1)
        .WillOnce(Return(1000));

    EXPECT_CALL(to, Unlock()).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);

    EXPECT_FALSE(transaction.Make(from, to, 100));
}

TEST(TransactionMockTest, TooSmallSumDoesNotCallAccountMethods)
{
    MockAccount from(1, 1000);
    MockAccount to(2, 100);

    Transaction transaction;

    EXPECT_CALL(from, Lock()).Times(0);
    EXPECT_CALL(to, Lock()).Times(0);
    EXPECT_CALL(from, Unlock()).Times(0);
    EXPECT_CALL(to, Unlock()).Times(0);
    EXPECT_CALL(from, ChangeBalance(_)).Times(0);
    EXPECT_CALL(to, ChangeBalance(_)).Times(0);
    EXPECT_CALL(from, GetBalance()).Times(0);
    EXPECT_CALL(to, GetBalance()).Times(0);

    EXPECT_THROW(transaction.Make(from, to, 50), std::logic_error);
}
