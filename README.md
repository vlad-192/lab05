# Lab05 - Banking Library with GTest and Mock Objects

Lotnik Vladislav IU8-23

## Code Coverage: 100%

| File | Lines | Functions |
|---|---|---|
| `banking/Account.cpp` | 100% | 100% |
| `banking/Transaction.cpp` | 100% | 100% |

## Test Results

- Total tests: 10
- Passed: 10
- Failed: 0

## Build and Test

```bash
git clone --recursive https://github.com/vlad-192/lab05
cd lab05
mkdir build
cd build
cmake .. -DBUILD_TESTS=ON
make
./banking_tests
