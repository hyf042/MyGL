#include <gtest/gtest.h>
#include "Core/Structures/Matrix.h"
#undef override

using namespace MyGL;

// ── 工具函数 ──────────────────────────────────────────────────────────────────

static bool MatNear(const Matrix4& a, const Matrix4& b, float eps = 1e-4f) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (std::abs(a[i][j] - b[i][j]) > eps) return false;
    return true;
}

// ── Identity ──────────────────────────────────────────────────────────────────

TEST(Matrix4, Identity) {
    Matrix4 I = Matrix4::Identity();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_FLOAT_EQ(I[i][j], (i == j) ? 1.0f : 0.0f);
}

TEST(Matrix4, DefaultIsZero) {
    Matrix4 m;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_FLOAT_EQ(m[i][j], 0.0f);
}

// ── Multiply ──────────────────────────────────────────────────────────────────

TEST(Matrix4, MultiplyByIdentity) {
    Matrix4 I = Matrix4::Identity();
    Matrix4 m = Matrixs::CreateMatrix4x4(
        1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16);
    EXPECT_TRUE(MatNear(m * I, m));
    EXPECT_TRUE(MatNear(I * m, m));
}

TEST(Matrix4, MultiplyKnownResult) {
    // Translation(1,0,0) * Translation(2,0,0) = Translation(3,0,0)
    Matrix4 T1 = Matrixs::CreateMatrix4x4(
        1,0,0,1,  0,1,0,0,  0,0,1,0,  0,0,0,1);
    Matrix4 T2 = Matrixs::CreateMatrix4x4(
        1,0,0,2,  0,1,0,0,  0,0,1,0,  0,0,0,1);
    Matrix4 expected = Matrixs::CreateMatrix4x4(
        1,0,0,3,  0,1,0,0,  0,0,1,0,  0,0,0,1);
    EXPECT_TRUE(MatNear(T1 * T2, expected));
}

// ── Matrix-Vector multiply ────────────────────────────────────────────────────

TEST(Matrix4, MultiplyVector4) {
    Matrix4 I = Matrix4::Identity();
    Vector4 v(1.0f, 2.0f, 3.0f, 1.0f);
    Vector4 result = I * v;
    EXPECT_FLOAT_EQ(result.x(), 1.0f);
    EXPECT_FLOAT_EQ(result.y(), 2.0f);
    EXPECT_FLOAT_EQ(result.z(), 3.0f);
    EXPECT_FLOAT_EQ(result.w(), 1.0f);
}

TEST(Matrix4, TranslateAppliedToPoint) {
    // Translate(tx, ty, tz) * point(x,y,z,1) = (x+tx, y+ty, z+tz, 1)
    Matrix4 T = Matrixs::CreateMatrix4x4(
        1,0,0,5,  0,1,0,-3,  0,0,1,7,  0,0,0,1);
    Vector4 p(1.0f, 2.0f, 3.0f, 1.0f);
    Vector4 result = T * p;
    EXPECT_FLOAT_EQ(result.x(), 6.0f);
    EXPECT_FLOAT_EQ(result.y(), -1.0f);
    EXPECT_FLOAT_EQ(result.z(), 10.0f);
    EXPECT_FLOAT_EQ(result.w(), 1.0f);
}

// ── Transpose ─────────────────────────────────────────────────────────────────

TEST(Matrix4, Transpose) {
    Matrix4 m = Matrixs::CreateMatrix4x4(
        1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16);
    Matrix4 t = m.Transpose();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_FLOAT_EQ(t[i][j], m[j][i]);
}

TEST(Matrix4, TransposeOfIdentityIsIdentity) {
    Matrix4 I = Matrix4::Identity();
    EXPECT_TRUE(MatNear(I.Transpose(), I));
}

// ── Inverse ───────────────────────────────────────────────────────────────────

TEST(Matrix4, InverseOfIdentityIsIdentity) {
    Matrix4 I = Matrix4::Identity();
    Matrix4 inv = Matrixs::Inverse(I);
    EXPECT_TRUE(MatNear(inv, I));
}

TEST(Matrix4, InverseProperty_AxInvA_IsIdentity) {
    // A * A^-1 == I
    Matrix4 A = Matrixs::CreateMatrix4x4(
        1,0,0,5,  0,1,0,-3,  0,0,1,7,  0,0,0,1);  // simple translation
    Matrix4 Ainv = Matrixs::Inverse(A);
    Matrix4 product = A * Ainv;
    EXPECT_TRUE(MatNear(product, Matrix4::Identity()));
}

TEST(Matrix4, InverseOfTranslation) {
    // Translate(tx, ty, tz)^-1 = Translate(-tx, -ty, -tz)
    Matrix4 T = Matrixs::CreateMatrix4x4(
        1,0,0,3,  0,1,0,2,  0,0,1,-1,  0,0,0,1);
    Matrix4 Tinv = Matrixs::Inverse(T);
    Matrix4 expected = Matrixs::CreateMatrix4x4(
        1,0,0,-3,  0,1,0,-2,  0,0,1,1,  0,0,0,1);
    EXPECT_TRUE(MatNear(Tinv, expected));
}

TEST(Matrix4, InverseOfScaling) {
    // Scale(sx,sy,sz)^-1 = Scale(1/sx, 1/sy, 1/sz)
    Matrix4 S = Matrixs::CreateMatrix4x4(
        2,0,0,0,  0,4,0,0,  0,0,8,0,  0,0,0,1);
    Matrix4 Sinv = Matrixs::Inverse(S);
    Matrix4 expected = Matrixs::CreateMatrix4x4(
        0.5f,0,0,0,  0,0.25f,0,0,  0,0,0.125f,0,  0,0,0,1);
    EXPECT_TRUE(MatNear(Sinv, expected));
}

TEST(Matrix4, SingularMatrixThrows) {
    Matrix4 singular;  // all zeros
    EXPECT_THROW(Matrixs::Inverse(singular), MyGL::Exception);
}

// ── Equality ──────────────────────────────────────────────────────────────────

TEST(Matrix4, EqualityOperator) {
    Matrix4 I = Matrix4::Identity();
    Matrix4 I2 = Matrix4::Identity();
    EXPECT_EQ(I, I2);

    Matrix4 m = Matrixs::CreateMatrix4x4(1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16);
    EXPECT_NE(I, m);
}

// ── GetRowVector / GetColVector ───────────────────────────────────────────────

TEST(Matrix4, GetRowColVectors) {
    Matrix4 m = Matrixs::CreateMatrix4x4(
        1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16);
    Vector4 row0 = m.GetRowVector(0);
    EXPECT_FLOAT_EQ(row0.x(), 1.0f);
    EXPECT_FLOAT_EQ(row0.y(), 2.0f);
    EXPECT_FLOAT_EQ(row0.z(), 3.0f);
    EXPECT_FLOAT_EQ(row0.w(), 4.0f);

    Vector4 col1 = m.GetColVector(1);
    EXPECT_FLOAT_EQ(col1.x(), 2.0f);
    EXPECT_FLOAT_EQ(col1.y(), 6.0f);
    EXPECT_FLOAT_EQ(col1.z(), 10.0f);
    EXPECT_FLOAT_EQ(col1.w(), 14.0f);
}

// ── Matrix3 ───────────────────────────────────────────────────────────────────

TEST(Matrix3, Identity) {
    Matrix3 I = Matrix3::Identity();
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            EXPECT_FLOAT_EQ(I[i][j], (i==j) ? 1.0f : 0.0f);
}

TEST(Matrix3, InverseProperty) {
    Matrix3 A = Matrixs::CreateMatrix3x3(
        2,1,0,  0,3,1,  0,0,4);
    Matrix3 Ainv = Matrixs::Inverse(A);
    // Check A * A^-1 ≈ I
    Matrix3 product = A * Ainv;
    Matrix3 I = Matrix3::Identity();
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            EXPECT_NEAR(product[i][j], I[i][j], 1e-5f);
}
