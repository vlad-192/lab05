#include <gtest/gtest.h>

#include "Account.h"
#include "Transaction.h"

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

TEST(TransactionTest, MakeSuccess)
{
    Account from(1, 1000);
    Account to(2, 100);

    Transaction transaction;
    transaction.set_fee(1);

    EXPECT_TRUE(transaction.Make(from, to, 100));

    EXPECT_EQ(from.GetBalance(), 1000);
    EXPECT_EQ(to.GetBalance(), 99);
}

TEST(TransactionTest, MakeNotEnoughMoney)
{
    Account from(1, 50);
    Account to(2, 100);

    Transaction transaction;
    transaction.set_fee(1);

    EXPECT_TRUE(transaction.Make(from, to, 100));

    EXPECT_EQ(from.GetBalance(), 50);
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
