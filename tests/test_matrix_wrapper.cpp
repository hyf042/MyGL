#include <gtest/gtest.h>
#include "Core/MatrixWrapper.h"
#undef override

using namespace MyGL;

// ── Initial state ─────────────────────────────────────────────────────────────

TEST(MatrixWrapper, DefaultIsIdentity) {
    MatrixWrapper mw;
    EXPECT_EQ(mw.get_matrix(), Matrix4::Identity());
}

// ── Translate ─────────────────────────────────────────────────────────────────

TEST(MatrixWrapper, Translate) {
    MatrixWrapper mw;
    mw.Translate(1.0f, 2.0f, 3.0f);
    Matrix4 T = Transforms::MakeTranslate(1.0f, 2.0f, 3.0f);
    EXPECT_EQ(mw.get_matrix(), T);
}

TEST(MatrixWrapper, TranslateChaining) {
    MatrixWrapper mw;
    mw.Translate(1.0f, 0.0f, 0.0f).Translate(2.0f, 0.0f, 0.0f);
    Matrix4 expected = Transforms::MakeTranslate(1.0f, 0.0f, 0.0f)
                     * Transforms::MakeTranslate(2.0f, 0.0f, 0.0f);
    EXPECT_EQ(mw.get_matrix(), expected);
}

// ── Scale ─────────────────────────────────────────────────────────────────────

TEST(MatrixWrapper, Scale) {
    MatrixWrapper mw;
    mw.Scale(2.0f, 3.0f, 4.0f);
    Matrix4 S = Transforms::MakeScale(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(mw.get_matrix(), S);
}

// ── LoadIdentity ──────────────────────────────────────────────────────────────

TEST(MatrixWrapper, LoadIdentityResets) {
    MatrixWrapper mw;
    mw.Translate(5.0f, 5.0f, 5.0f);
    mw.LoadIdentity();
    EXPECT_EQ(mw.get_matrix(), Matrix4::Identity());
}

// ── Push / Pop ────────────────────────────────────────────────────────────────

TEST(MatrixWrapper, PushPopRestoresMatrix) {
    MatrixWrapper mw;
    mw.Translate(1.0f, 2.0f, 3.0f);
    Matrix4 saved = mw.get_matrix();

    mw.Push();
    mw.Scale(5.0f);           // modify after push
    EXPECT_NE(mw.get_matrix(), saved);

    mw.Pop();
    EXPECT_EQ(mw.get_matrix(), saved);  // restored
}

TEST(MatrixWrapper, PopEmptyStackThrows) {
    MatrixWrapper mw;
    EXPECT_THROW(mw.Pop(), MyGL::Exception);
}

TEST(MatrixWrapper, MultiplePushPop) {
    MatrixWrapper mw;
    Matrix4 m0 = mw.get_matrix();  // identity

    mw.Push();
    mw.Translate(1.0f, 0.0f, 0.0f);
    Matrix4 m1 = mw.get_matrix();

    mw.Push();
    mw.Translate(2.0f, 0.0f, 0.0f);

    mw.Pop();
    EXPECT_EQ(mw.get_matrix(), m1);

    mw.Pop();
    EXPECT_EQ(mw.get_matrix(), m0);
}

TEST(MatrixWrapper, PopDoesNotLeakState) {
    // Verify that after N pushes and pops, stack is truly empty (pop throws)
    MatrixWrapper mw;
    mw.Push();
    mw.Push();
    mw.Pop();
    mw.Pop();
    EXPECT_THROW(mw.Pop(), MyGL::Exception);
}

// ── Inverse (lazy cache) ──────────────────────────────────────────────────────

TEST(MatrixWrapper, InverseOfIdentity) {
    MatrixWrapper mw;
    Matrix4 inv = mw.get_inverse();
    EXPECT_EQ(inv, Matrix4::Identity());
}

TEST(MatrixWrapper, InverseUpdatesAfterTransform) {
    MatrixWrapper mw;
    mw.Translate(3.0f, -1.0f, 2.0f);
    Matrix4 inv = mw.get_inverse();
    Matrix4 product = mw.get_matrix() * inv;
    // M * M^-1 should be identity
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_NEAR(product[i][j], (i==j)?1.0f:0.0f, 1e-4f);
}

// ── Multiply ──────────────────────────────────────────────────────────────────

TEST(MatrixWrapper, MultiplyAccumulatesLeft) {
    MatrixWrapper mw;
    Matrix4 m = Transforms::MakeScale(2.0f);
    mw.Multiply(m);
    // Internal matrix = I * Scale(2) = Scale(2)
    EXPECT_EQ(mw.get_matrix(), m);
}
