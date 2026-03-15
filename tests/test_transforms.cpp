#include <gtest/gtest.h>
#include "Core/Transforms.h"
#undef override

#include <cmath>
using namespace MyGL;

// ── 工具 ──────────────────────────────────────────────────────────────────────
static bool V4Near(const Vector4& a, const Vector4& b, float eps = 1e-4f) {
    return std::abs(a.x()-b.x()) < eps
        && std::abs(a.y()-b.y()) < eps
        && std::abs(a.z()-b.z()) < eps
        && std::abs(a.w()-b.w()) < eps;
}

static bool MatNear(const Matrix4& a, const Matrix4& b, float eps = 1e-4f) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (std::abs(a[i][j] - b[i][j]) > eps) return false;
    return true;
}

// ── MakeTranslate ─────────────────────────────────────────────────────────────

TEST(Transforms, TranslatePoint) {
    Matrix4 T = Transforms::MakeTranslate(3.0f, -1.0f, 5.0f);
    Vector4 p(1.0f, 2.0f, 3.0f, 1.0f);
    Vector4 result = T * p;
    EXPECT_NEAR(result.x(), 4.0f, 1e-5f);
    EXPECT_NEAR(result.y(), 1.0f, 1e-5f);
    EXPECT_NEAR(result.z(), 8.0f, 1e-5f);
    EXPECT_NEAR(result.w(), 1.0f, 1e-5f);
}

TEST(Transforms, TranslateVector_NoEffect) {
    // Translating a direction vector (w=0) should not change position.
    Matrix4 T = Transforms::MakeTranslate(3.0f, -1.0f, 5.0f);
    Vector4 v(1.0f, 0.0f, 0.0f, 0.0f);
    Vector4 result = T * v;
    EXPECT_NEAR(result.x(), 1.0f, 1e-5f);
    EXPECT_NEAR(result.y(), 0.0f, 1e-5f);
    EXPECT_NEAR(result.z(), 0.0f, 1e-5f);
    EXPECT_NEAR(result.w(), 0.0f, 1e-5f);
}

TEST(Transforms, TranslateInverseIsNegation) {
    Matrix4 T = Transforms::MakeTranslate(5.0f, 3.0f, -2.0f);
    Matrix4 Tneg = Transforms::MakeTranslate(-5.0f, -3.0f, 2.0f);
    EXPECT_TRUE(MatNear(T * Tneg, Matrix4::Identity()));
}

// ── MakeScale ─────────────────────────────────────────────────────────────────

TEST(Transforms, ScaleAxes) {
    Matrix4 S = Transforms::MakeScale(2.0f, 3.0f, 4.0f);
    Vector4 p(1.0f, 1.0f, 1.0f, 1.0f);
    Vector4 result = S * p;
    EXPECT_NEAR(result.x(), 2.0f, 1e-5f);
    EXPECT_NEAR(result.y(), 3.0f, 1e-5f);
    EXPECT_NEAR(result.z(), 4.0f, 1e-5f);
    EXPECT_NEAR(result.w(), 1.0f, 1e-5f);
}

TEST(Transforms, UniformScale) {
    Matrix4 S = Transforms::MakeScale(5.0f);
    Vector4 p(1.0f, 2.0f, 3.0f, 1.0f);
    Vector4 result = S * p;
    EXPECT_NEAR(result.x(), 5.0f, 1e-5f);
    EXPECT_NEAR(result.y(), 10.0f, 1e-5f);
    EXPECT_NEAR(result.z(), 15.0f, 1e-5f);
}

// ── MakeRotation ──────────────────────────────────────────────────────────────

TEST(Transforms, Rotate90AroundZ) {
    // Rotating x-axis 90° around z-axis gives y-axis
    Matrix4 R = Transforms::MakeRotation(90.0f, 0.0f, 0.0f, 1.0f);
    Vector4 x_axis(1.0f, 0.0f, 0.0f, 0.0f);
    Vector4 result = R * x_axis;
    EXPECT_NEAR(result.x(), 0.0f, 1e-5f);
    EXPECT_NEAR(result.y(), 1.0f, 1e-5f);
    EXPECT_NEAR(result.z(), 0.0f, 1e-5f);
}

TEST(Transforms, Rotate90AroundY) {
    // Rotating z-axis 90° CCW around +y-axis (right-hand rule): z → +x
    Matrix4 R = Transforms::MakeRotation(90.0f, 0.0f, 1.0f, 0.0f);
    Vector4 z_axis(0.0f, 0.0f, 1.0f, 0.0f);
    Vector4 result = R * z_axis;
    EXPECT_NEAR(result.x(), 1.0f, 1e-5f);   // z-axis rotates to +x
    EXPECT_NEAR(result.y(), 0.0f, 1e-5f);
    EXPECT_NEAR(result.z(), 0.0f, 1e-5f);
}

TEST(Transforms, Rotate360IsIdentity) {
    Matrix4 R = Transforms::MakeRotation(360.0f, 0.0f, 1.0f, 0.0f);
    EXPECT_TRUE(MatNear(R, Matrix4::Identity(), 1e-5f));
}

TEST(Transforms, RotationPreservesLength) {
    Matrix4 R = Transforms::MakeRotation(45.0f, 1.0f, 1.0f, 0.0f);
    Vector4 v(3.0f, 4.0f, 5.0f, 0.0f);
    Vector4 result = R * v;
    float len_before = Vector3(v.x(), v.y(), v.z()).Length();
    float len_after  = Vector3(result.x(), result.y(), result.z()).Length();
    EXPECT_NEAR(len_before, len_after, 1e-4f);
}

// ── MakeLookAt ────────────────────────────────────────────────────────────────

TEST(Transforms, LookAt_OriginBehindCamera) {
    // Camera at (0,0,5) looking at origin, up = world y
    Matrix4 V = Transforms::MakeLookAt(
        Vector3(0.0f, 0.0f, 5.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f));
    Vector4 world_origin(0.0f, 0.0f, 0.0f, 1.0f);
    Vector4 camera_space = V * world_origin;
    // In camera space, origin should be at (0, 0, -5)
    EXPECT_NEAR(camera_space.x(), 0.0f, 1e-4f);
    EXPECT_NEAR(camera_space.y(), 0.0f, 1e-4f);
    EXPECT_NEAR(camera_space.z(), -5.0f, 1e-4f);
    EXPECT_NEAR(camera_space.w(), 1.0f, 1e-4f);
}

TEST(Transforms, LookAt_EyeTransformsToOrigin) {
    // The camera position itself should map to (0,0,0) in camera space
    Vector3 eye(3.0f, 4.0f, -2.0f);
    Matrix4 V = Transforms::MakeLookAt(eye,
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f));
    Vector4 eye_world(eye.x(), eye.y(), eye.z(), 1.0f);
    Vector4 cam = V * eye_world;
    EXPECT_NEAR(cam.x(), 0.0f, 1e-4f);
    EXPECT_NEAR(cam.y(), 0.0f, 1e-4f);
    EXPECT_NEAR(cam.z(), 0.0f, 1e-4f);
}

TEST(Transforms, LookAt_RotatedCamera) {
    // Camera at (5,0,0) looking at origin, up=(0,1,0)
    // In camera space: world origin should be at camera's "back" (positive z)
    Matrix4 V = Transforms::MakeLookAt(
        Vector3(5.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f));
    Vector4 world_origin(0.0f, 0.0f, 0.0f, 1.0f);
    Vector4 cam = V * world_origin;
    // Origin is 5 units away in the camera's backward direction (+z)
    EXPECT_NEAR(cam.x(), 0.0f, 1e-4f);
    EXPECT_NEAR(cam.y(), 0.0f, 1e-4f);
    EXPECT_NEAR(cam.z(), -5.0f, 1e-4f);
}

// ── MakePerspectiveFov ────────────────────────────────────────────────────────

TEST(Transforms, Perspective_StructureCheck) {
    // Verify key structural properties of the perspective matrix
    Matrix4 P = Transforms::MakePerspectiveFov(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
    // m[3][2] should be -1 (homogeneous divide by -z)
    EXPECT_NEAR(P[3][2], -1.0f, 1e-5f);
    // m[3][3] should be 0
    EXPECT_NEAR(P[3][3], 0.0f, 1e-5f);
    // m[0][0] and m[1][1] should be positive (field-of-view scaling)
    EXPECT_GT(P[0][0], 0.0f);
    EXPECT_GT(P[1][1], 0.0f);
}

TEST(Transforms, Perspective_FovAspect) {
    // For a symmetric fov: m[1][1] = cot(fovy/2), m[0][0] = m[1][1] / aspect
    float fovy = 90.0f;
    float aspect = 2.0f;
    Matrix4 P = Transforms::MakePerspectiveFov(fovy, aspect, 1.0f, 100.0f);
    float cot_half = 1.0f / std::tan(Math::Deg2Rad(fovy) * 0.5f);
    EXPECT_NEAR(P[1][1], cot_half, 1e-4f);
    EXPECT_NEAR(P[0][0], cot_half / aspect, 1e-4f);
}

// ── MakeOrtho ─────────────────────────────────────────────────────────────────

TEST(Transforms, Ortho_CenterMapsToOrigin) {
    float left = -10, right = 10, bottom = -5, top = 5, near = 1, far = 100;
    Matrix4 O = Transforms::MakeOrtho(left, right, bottom, top, near, far);
    Vector4 center(0.0f, 0.0f, -(near + far) * 0.5f, 1.0f);
    Vector4 ndc = O * center;
    EXPECT_NEAR(ndc.x(), 0.0f, 1e-4f);
    EXPECT_NEAR(ndc.y(), 0.0f, 1e-4f);
    EXPECT_NEAR(ndc.w(), 1.0f, 1e-5f);
}

TEST(Transforms, Ortho_StructureCheck) {
    Matrix4 O = Transforms::MakeOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
    // Should be a scale-translation matrix with m[3][3] = 1
    EXPECT_NEAR(O[3][3], 1.0f, 1e-5f);
    EXPECT_NEAR(O[3][0], 0.0f, 1e-5f);
    EXPECT_NEAR(O[3][1], 0.0f, 1e-5f);
    EXPECT_NEAR(O[3][2], 0.0f, 1e-5f);
}

// ── MakeViewport ──────────────────────────────────────────────────────────────

TEST(Transforms, Viewport_CenterNDCMapsToScreenCenter) {
    // NDC (0,0) -> screen center (w/2, h/2)
    float w = 800, h = 600;
    Matrix4 VP = Transforms::MakeViewport(0, 0, w, h);
    Vector4 ndc_center(0.0f, 0.0f, 0.5f, 1.0f);
    Vector4 screen = VP * ndc_center;
    EXPECT_NEAR(screen.x(), w * 0.5f, 1e-3f);
    EXPECT_NEAR(screen.y(), h * 0.5f, 1e-3f);
}

TEST(Transforms, Viewport_TopLeftIsYFlipped) {
    // NDC (+x, +y) -> right, UP in NDC = DOWN in screen (y is flipped)
    float w = 800, h = 600;
    Matrix4 VP = Transforms::MakeViewport(0, 0, w, h);
    Vector4 ndc_top_right(1.0f, 1.0f, 0.5f, 1.0f);
    Vector4 screen = VP * ndc_top_right;
    EXPECT_NEAR(screen.x(), w, 1e-3f);  // right edge
    EXPECT_NEAR(screen.y(), 0.0f, 1e-3f);  // top edge (y flipped: NDC +1 -> screen y=0)
}
