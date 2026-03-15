#include <gtest/gtest.h>
#include "Core/Structures/Vector.h"
#undef override

using namespace MyGL;

// ── Vector2 ───────────────────────────────────────────────────────────────────

TEST(Vector2, ConstructAndAccess) {
    Vector2 v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.x(), 3.0f);
    EXPECT_FLOAT_EQ(v.y(), 4.0f);
}

TEST(Vector2, DefaultIsZero) {
    Vector2 v;
    EXPECT_FLOAT_EQ(v.x(), 0.0f);
    EXPECT_FLOAT_EQ(v.y(), 0.0f);
}

TEST(Vector2, Arithmetic) {
    Vector2 a(1.0f, 2.0f), b(3.0f, 4.0f);
    Vector2 sum = a + b;
    EXPECT_FLOAT_EQ(sum.x(), 4.0f);
    EXPECT_FLOAT_EQ(sum.y(), 6.0f);

    Vector2 diff = b - a;
    EXPECT_FLOAT_EQ(diff.x(), 2.0f);
    EXPECT_FLOAT_EQ(diff.y(), 2.0f);

    Vector2 scaled = a * 2.0f;
    EXPECT_FLOAT_EQ(scaled.x(), 2.0f);
    EXPECT_FLOAT_EQ(scaled.y(), 4.0f);
}

TEST(Vector2, Dot) {
    Vector2 a(1.0f, 0.0f), b(0.0f, 1.0f);
    EXPECT_FLOAT_EQ(Dot(a, b), 0.0f);

    Vector2 c(1.0f, 2.0f), d(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(Dot(c, d), 11.0f);  // 1*3 + 2*4
}

TEST(Vector2, Cross) {
    Vector2 a(1.0f, 0.0f), b(0.0f, 1.0f);
    EXPECT_FLOAT_EQ(Cross(a, b), 1.0f);
    EXPECT_FLOAT_EQ(Cross(b, a), -1.0f);
}

TEST(Vector2, Length) {
    Vector2 v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.Length(), 5.0f);
    EXPECT_FLOAT_EQ(v.LengthSquare(), 25.0f);
}

TEST(Vector2, Normalize) {
    Vector2 v(3.0f, 4.0f);
    v.Normalize();
    EXPECT_NEAR(v.Length(), 1.0f, 1e-6f);
    EXPECT_NEAR(v.x(), 0.6f, 1e-6f);
    EXPECT_NEAR(v.y(), 0.8f, 1e-6f);
}

TEST(Vector2, Unit) {
    Vector2 v(0.0f, 5.0f);
    Vector2 u = v.Unit();
    EXPECT_NEAR(u.x(), 0.0f, 1e-6f);
    EXPECT_NEAR(u.y(), 1.0f, 1e-6f);
}

TEST(Vector2, Equality) {
    Vector2 a(1.0f, 2.0f), b(1.0f, 2.0f), c(1.0f, 3.0f);
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

TEST(Vector2, Lerp) {
    Vector2 a(0.0f, 0.0f), b(10.0f, 20.0f);
    Vector2 mid = Vector2::Lerp(a, b, 0.5f);
    EXPECT_FLOAT_EQ(mid.x(), 5.0f);
    EXPECT_FLOAT_EQ(mid.y(), 10.0f);
}

// ── Vector3 ───────────────────────────────────────────────────────────────────

TEST(Vector3, ConstructAndAccess) {
    Vector3 v(1.0f, 2.0f, 3.0f);
    EXPECT_FLOAT_EQ(v.x(), 1.0f);
    EXPECT_FLOAT_EQ(v.y(), 2.0f);
    EXPECT_FLOAT_EQ(v.z(), 3.0f);
}

TEST(Vector3, CrossProduct) {
    Vector3 x(1.0f, 0.0f, 0.0f);
    Vector3 y(0.0f, 1.0f, 0.0f);
    Vector3 z = Cross(x, y);
    EXPECT_NEAR(z.x(), 0.0f, 1e-6f);
    EXPECT_NEAR(z.y(), 0.0f, 1e-6f);
    EXPECT_NEAR(z.z(), 1.0f, 1e-6f);

    // Anti-commutative
    Vector3 neg_z = Cross(y, x);
    EXPECT_NEAR(neg_z.z(), -1.0f, 1e-6f);
}

TEST(Vector3, DotProduct) {
    Vector3 a(1.0f, 2.0f, 3.0f), b(4.0f, 5.0f, 6.0f);
    EXPECT_FLOAT_EQ(Dot(a, b), 32.0f);  // 1*4+2*5+3*6 = 4+10+18
}

TEST(Vector3, Normalize) {
    Vector3 v(1.0f, 0.0f, 0.0f);
    v.Normalize();
    EXPECT_NEAR(v.Length(), 1.0f, 1e-6f);

    Vector3 v2(3.0f, 4.0f, 0.0f);
    v2.Normalize();
    EXPECT_NEAR(v2.Length(), 1.0f, 1e-6f);
}

TEST(Vector3, Arithmetic) {
    Vector3 a(1.0f, 2.0f, 3.0f), b(4.0f, 5.0f, 6.0f);
    Vector3 sum = a + b;
    EXPECT_FLOAT_EQ(sum.x(), 5.0f);
    EXPECT_FLOAT_EQ(sum.y(), 7.0f);
    EXPECT_FLOAT_EQ(sum.z(), 9.0f);
}

TEST(Vector3, Negate) {
    Vector3 v(1.0f, -2.0f, 3.0f);
    Vector3 neg = -v;
    EXPECT_FLOAT_EQ(neg.x(), -1.0f);
    EXPECT_FLOAT_EQ(neg.y(),  2.0f);
    EXPECT_FLOAT_EQ(neg.z(), -3.0f);
}

// ── Vector4 ───────────────────────────────────────────────────────────────────

TEST(Vector4, ConstructAndAccess) {
    Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.x(), 1.0f);
    EXPECT_FLOAT_EQ(v.y(), 2.0f);
    EXPECT_FLOAT_EQ(v.z(), 3.0f);
    EXPECT_FLOAT_EQ(v.w(), 4.0f);
}

TEST(Vector4, OfFromVector3) {
    Vector3 v3(1.0f, 2.0f, 3.0f);
    Vector4 point  = Vector4::Of(v3, false);  // w=1 (point)
    Vector4 vec    = Vector4::Of(v3, true);   // w=0 (vector)
    EXPECT_FLOAT_EQ(point.w(), 1.0f);
    EXPECT_FLOAT_EQ(vec.w(), 0.0f);
}

TEST(Vector4, AsVector3_DividesByW) {
    Vector4 v(2.0f, 4.0f, 6.0f, 2.0f);
    Vector3 v3 = v.AsVector3();
    EXPECT_FLOAT_EQ(v3.x(), 1.0f);
    EXPECT_FLOAT_EQ(v3.y(), 2.0f);
    EXPECT_FLOAT_EQ(v3.z(), 3.0f);
}

TEST(Vector4, StripLast) {
    Vector4 v(1.0f, 2.0f, 3.0f, 99.0f);
    Vector3 v3 = v.StripLast();
    EXPECT_FLOAT_EQ(v3.x(), 1.0f);
    EXPECT_FLOAT_EQ(v3.y(), 2.0f);
    EXPECT_FLOAT_EQ(v3.z(), 3.0f);
}

TEST(Vector4, RectHelpers) {
    Vector4 r = Vector4::MakeRect(10.0f, 20.0f, 100.0f, 50.0f);
    EXPECT_FLOAT_EQ(r.left(),   10.0f);
    EXPECT_FLOAT_EQ(r.bottom(), 20.0f);
    EXPECT_FLOAT_EQ(r.right(),  110.0f);
    EXPECT_FLOAT_EQ(r.top(),    70.0f);
    EXPECT_FLOAT_EQ(r.width(),  100.0f);
    EXPECT_FLOAT_EQ(r.height(), 50.0f);
}

TEST(Vector4, LerpVector4) {
    Vector4 a(0,0,0,0), b(4,4,4,4);
    Vector4 mid = Vector4::Lerp(a, b, 0.25f);
    EXPECT_FLOAT_EQ(mid.x(), 1.0f);
    EXPECT_FLOAT_EQ(mid.w(), 1.0f);
}
