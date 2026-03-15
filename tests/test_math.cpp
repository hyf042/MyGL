// Must include gtest first, then MyGL (which redefines 'override'),
// then #undef override before TEST() macros.
#include <gtest/gtest.h>
#include "Core/Math.h"
#undef override

using namespace MyGL;

TEST(Math, EqualAndIsZero) {
    EXPECT_TRUE(Math::Equal(0.0f, 0.0f));
    EXPECT_TRUE(Math::Equal(1.0f, 1.0f));
    EXPECT_FALSE(Math::Equal(1.0f, 2.0f));
    EXPECT_FALSE(Math::Equal(1.0f, 1.0f + 1e-4f));

    EXPECT_TRUE(Math::IsZero(0.0f));
    EXPECT_FALSE(Math::IsZero(1e-4f));
}

TEST(Math, Comparison) {
    EXPECT_TRUE(Math::Greater(2.0f, 1.0f));
    EXPECT_FALSE(Math::Greater(1.0f, 2.0f));
    EXPECT_FALSE(Math::Greater(1.0f, 1.0f));

    EXPECT_TRUE(Math::Less(1.0f, 2.0f));
    EXPECT_FALSE(Math::Less(2.0f, 1.0f));

    EXPECT_TRUE(Math::GreaterOrEqual(1.0f, 1.0f));
    EXPECT_TRUE(Math::GreaterOrEqual(2.0f, 1.0f));
    EXPECT_FALSE(Math::GreaterOrEqual(1.0f, 2.0f));

    EXPECT_TRUE(Math::InRange(1.5f, 1.0f, 2.0f));
    EXPECT_TRUE(Math::InRange(1.0f, 1.0f, 2.0f));
    EXPECT_FALSE(Math::InRange(3.0f, 1.0f, 2.0f));
    EXPECT_TRUE(Math::InRange(2, 1, 3));
    EXPECT_FALSE(Math::InRange(0, 1, 3));
}

TEST(Math, Clamp) {
    EXPECT_FLOAT_EQ(Math::Clamp(5.0f, 0.0f, 10.0f), 5.0f);
    EXPECT_FLOAT_EQ(Math::Clamp(-1.0f, 0.0f, 10.0f), 0.0f);
    EXPECT_FLOAT_EQ(Math::Clamp(11.0f, 0.0f, 10.0f), 10.0f);
}

TEST(Math, Lerp) {
    EXPECT_FLOAT_EQ(Math::Lerp(0.0f, 10.0f, 0.5f), 5.0f);
    EXPECT_FLOAT_EQ(Math::Lerp(0.0f, 10.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(Math::Lerp(0.0f, 10.0f, 1.0f), 10.0f);
}

TEST(Math, Deg2Rad) {
    EXPECT_NEAR(Math::Deg2Rad(180.0f), Math::Pi, 1e-5f);
    EXPECT_NEAR(Math::Deg2Rad(90.0f),  Math::Pi * 0.5f, 1e-5f);
    EXPECT_NEAR(Math::Deg2Rad(0.0f),   0.0f, 1e-5f);

    EXPECT_NEAR(Math::Rad2Deg(Math::Pi), 180.0f, 1e-4f);
}

TEST(Math, Mod) {
    EXPECT_NEAR(Math::Mod(5.5f, 2.0f), 1.5f, 1e-5f);
    EXPECT_NEAR(Math::Mod(-1.5f, 2.0f), 0.5f, 1e-5f);
    EXPECT_NEAR(Math::Mod(0.0f, 3.0f), 0.0f, 1e-5f);
}

TEST(Math, IsPowerOf2) {
    EXPECT_TRUE(Math::IsPowerOf2(1));
    EXPECT_TRUE(Math::IsPowerOf2(2));
    EXPECT_TRUE(Math::IsPowerOf2(4));
    EXPECT_TRUE(Math::IsPowerOf2(256));
    EXPECT_FALSE(Math::IsPowerOf2(3));
    EXPECT_FALSE(Math::IsPowerOf2(6));
    EXPECT_FALSE(Math::IsPowerOf2(0));
}

TEST(Math, MinMax) {
    EXPECT_FLOAT_EQ(Math::Min(1.0f, 2.0f), 1.0f);
    EXPECT_FLOAT_EQ(Math::Max(1.0f, 2.0f), 2.0f);
    EXPECT_EQ(Math::Min(3, 5), 3);
    EXPECT_EQ(Math::Max(3, 5), 5);
}
