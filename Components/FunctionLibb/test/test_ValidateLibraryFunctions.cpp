//
// Created by Stefan Annell on 2021-12-16.
//
#include "Functions.h"
#include <gtest/gtest.h>

class LibraryFunctionTests : public ::testing::Test {
protected:
    LibraryFunctionTests() {
    }

    ~LibraryFunctionTests() override {
    }

    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(LibraryFunctionTests, ExpectNotEqual) {
    EXPECT_NE(SpecialNumber(), 0);
}

TEST_F(LibraryFunctionTests, ExpectEqual) {
    EXPECT_EQ(SpecialNumber(), 42);
}

TEST_F(LibraryFunctionTests, PiExpectEqual) {
    EXPECT_DOUBLE_EQ(Pi(), 3.14);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
