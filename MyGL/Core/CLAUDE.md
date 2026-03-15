[根目录](../../CLAUDE.md) > **MyGL/Core**

# MyGL/Core - AI 上下文文档

## 变更记录 (Changelog)

| 版本 | 日期 | 说明 |
|------|------|------|
| v1.0 | 2026-03-15 | 初始化生成 |

---

## 模块职责

`MyGL/Core/` 是整个渲染引擎的核心层，实现了完整的 CPU 软件渲染管线，包含：

- **GL 状态机**：OpenGL 风格的单例渲染 API（`GL` 类）
- **渲染状态快照**：`GLState` 统一保存矩阵、纹理、光源、混合模式等所有可变状态
- **数学基础库**：`Vector2/3/4`、`Matrix3/4`、`Color`、`Buffer` 等基础数据结构
- **图元系统**：`Primitives`（点/线/三角形/四边形等）+ `PrimitivesBuilder` 构建器 + `DrawCall` 批次
- **光栅化器**：`Rasterizer` 将图元转换为像素片段（`Fragment`），含透视正确纹理坐标插值
- **裁剪系统**：`Clipping` 视锥裁剪 + 背面剔除
- **着色器接口**：`Shader` 可继承覆写的顶点/片段着色器
- **光照模型**：`Light`（点光源/平行光/聚光灯）+ `Material`（Phong 材质）
- **纹理系统**：`Texture`（Mipmap 生成、双线性/最近过滤、四种环绕模式）
- **变换工厂**：`Transforms`（平移、旋转、缩放、LookAt、透视/正交投影、视口变换）
- **矩阵栈封装**：`MatrixWrapper`（矩阵栈 Push/Pop + 逆矩阵懒计算缓存）

---

## 入口与启动

Core 层通过 `MyGL/Core/Common.h` 将所有子组件汇聚，上层只需包含该头文件：

```cpp
#include "Core/Common.h"   // 引入全部 Core 组件
#include "Core/GL.h"       // GL 状态机（已包含在 Common.h 中）
```

GL 状态机通过单例获取：

```cpp
GL& gl = GL::Instance();
gl.Init(width, height);  // 必须先调用 Init 分配颜色/深度/模板缓冲
```

---

## 对外接口

### GL 类（核心 API）

| 方法组 | 代表方法 | 说明 |
|--------|---------|------|
| 初始化 | `Init(w, h)` | 分配颜色/深度/模板缓冲 |
| 清除 | `Clear()`, `ClearColor()`, `ClearDepth()` | 清空帧缓冲 |
| 变换 | `LookAt()`, `Perspective()`, `Ortho()`, `Viewport()` | 设置变换矩阵 |
| 矩阵操作 | `Translate()`, `Rotate()`, `Scale()`, `LoadIdentity()` | 链式调用，操作当前矩阵 |
| 图元构建 | `Begin()`, `End()`, `AddVertex()`, `SetColor()`, `SetUV()`, `SetNormal()` | 立即模式图元录入 |
| 批量绘制 | `DrawArrays()`, `DrawElements()` | 顶点数组/索引绘制 |
| 纹理 | `GenTexture()`, `BindTexture()`, `TexImage2D()`, `TexParameter()` | 纹理管理 |
| 光照 | `GenLight()`, `Enable(light)`, `SetLightParameter()`, `SetMaterial()` | 光源配置 |
| 状态标志 | `Enable(flag)`, `Disable(flag)`, `IsEnable(flag)` | GL_DEPTH_TEST, GL_BLEND, GL_LIGHTING, GL_Z_CULLING |
| 混合 | `BlendFunc(src, dst)` | Alpha 混合模式 |
| 深度 | `DepthFunc()` | 深度测试函数 |
| 提交 | `Flush()` | 执行所有 DrawCall，将结果写入 ColorBuffer |
| 读取 | `GetBuffer()` | 获取最终 ColorBuffer（供 Context 层呈现） |

### Rasterizer（光栅化）

```cpp
// 将图元转换为 Fragment 列表
vector<Fragment> Rasterizer::RasterizePoint(v);
vector<Fragment> Rasterizer::RasterizeLine(v1, v2);
vector<Fragment> Rasterizer::RasterizeTriangle(v1, v2, v3);
```

Fragment 包含：屏幕坐标、世界坐标、法线、UV（透视正确插值）、颜色、footprint（用于 Mipmap LOD）。

### Transforms（变换工厂）

```cpp
Matrix4 Transforms::MakeLookAt(eye, at, up);
Matrix4 Transforms::MakePerspectiveFov(fovy, aspect, near, far);
Matrix4 Transforms::MakeOrtho(left, right, bottom, top, near, far);
Matrix4 Transforms::MakeViewport(minX, minY, width, height, minZ, maxZ);
Matrix4 Transforms::MakeTranslate/Scale/MakeRotation/MakeEuler(...);
```

### Shader（着色器接口）

```cpp
class Shader {
    virtual Vertex VertexFunc(const Vertex& vertex);
    virtual Color FragmentFunc(weak_ptr<Texture> texture, const Fragment& fragment);
};
```

可继承 `Shader` 并注入 `GL` 来实现自定义着色逻辑。

---

## 关键依赖与配置

- 无外部依赖（纯 C++11 标准库）
- `PreDefines.h` 定义了所有基础类型别名（`uint8`/`uint32`/`string`/`shared_ptr` 等）和 `override` 宏
- `Consts.h` 定义所有 OpenGL 风格枚举（`PrimitivesType`、`TextureWrapMode`、`GLFlag`、`BlendMode` 等）
- `Exception.h` 定义异常类，`Exception::NotImplementationException()` 用于标记尚未实现的功能

---

## 数据模型

### GLState（渲染状态）

```
modelViewMatrix   MatrixWrapper   模型->视图空间变换矩阵（含栈）
projectionMatrix  MatrixWrapper   视图->裁剪空间投影矩阵（含栈）
viewportMatrix    Matrix4         裁剪->视口空间变换矩阵
vertexColor       Color           当前顶点颜色（立即模式）
vertexUV          Vector2         当前纹理坐标
vertexNormal      Vector3         当前法线
cullFace          CullFaceMask    面剔除模式（GL_BACK 默认）
textureTargets    map<target, Texture>  已绑定纹理
lights            vector<Light>   光源列表（默认 8 个槽位）
material          Material        当前材质（Phong: ambient/diffuse/specular/shininess）
flags             set<GLFlag>     功能开关（depth test 默认开启）
srcBlendMode      BlendMode       源混合因子
dstBlendMode      BlendMode       目标混合因子
depthFunc         DepthFunc       深度比较函数
```

### Vertex / Fragment

```
Vertex:   position(viewport), world_position, uv, normal, color
Fragment: 继承 Vertex，增加 output_x/y（像素坐标）+ footprint（Mipmap LOD 依据）
```

### Buffer<T>

```
模板类，T=uint32 时即 ColorBuffer（ARGB 32位像素缓冲）
T=float 时用作 DepthBuffer
支持：Get/SetData, Clear, Clamp, GetRawBytes
```

### Texture

```
_buffer         ColorBuffer     原始纹理
_mipmap         vector<ColorBuffer>  Mipmap 链（LoadMipmap 自动生成，要求正方形+2的幂次）
_horizWrapMode  TextureWrapMode GL_REPEAT / GL_MIRRORED_REPEAT / GL_CLAMP_TO_EDGE / GL_CLAMP_TO_BORDER
_vertWrapMode   TextureWrapMode 同上
_minFilter      TextureFilterMode   缩小过滤：GL_NEAREST / GL_LINEAR
_maxFilter      TextureFilterMode   放大过滤
_levelFilter    TextureFilterMode   Mipmap 级别插值：GL_NEAREST / GL_LINEAR
```

### Light

```
_position    Vector4   w=0 表示平行光方向，w=1 表示点光源位置
_diffuse     Color     漫反射颜色
_specular    Color     高光颜色
_ambient     Color     环境光颜色
_attenuation Vector3   (常数/线性/二次) 衰减系数
_spot_*               聚光灯参数（方向、内外角、指数）
```

---

## 测试与质量

- 无自动化测试，渲染正确性通过 `main.cpp` TestCase 目视验证
- 数学结构（Vector/Matrix）通过渲染结果间接验证
- 关键 TODO：`Fragment._footprint` 当前为标量，注释说明应改为矩形 footprint 以支持各向异性过滤

---

## 相关文件清单

| 文件 | 说明 |
|------|------|
| `Core/GL.h` / `GL.cpp` | GL 单例状态机，渲染管线主入口 |
| `Core/GLState.h` | 渲染状态快照（所有可变状态集中于此） |
| `Core/Rasterizer.h` / `.cpp` | 点/线/三角形光栅化，Fragment 生成 |
| `Core/Primitives.h` | 图元类型定义（Points/Lines/Triangles/Quads 等） |
| `Core/PrimitivesBuilder.h` | 图元构建器（Begin/AddVertex/Build） |
| `Core/DrawCall.h` | DrawCall 批次，持有 Primitives 列表 |
| `Core/Clipping.h` / `.cpp` | 视锥裁剪 + 背面剔除 |
| `Core/Shader.h` / `.cpp` | 可覆写着色器接口 |
| `Core/Light.h` / `.cpp` | Phong 光照计算（点光源/平行光/聚光灯） |
| `Core/Material.h` | Phong 材质（ambient/diffuse/specular/emission/shininess） |
| `Core/Texture.h` | 纹理采样系统（Mipmap + 过滤 + 环绕） |
| `Core/Transforms.h` / `.cpp` | 所有变换矩阵工厂方法 |
| `Core/MatrixWrapper.h` | 矩阵栈封装 + 逆矩阵懒计算 |
| `Core/Vertex.h` | Vertex 和 Fragment 数据结构 |
| `Core/Structures/Vector.h` / `.cpp` | Vector2/3/4 模板向量 |
| `Core/Structures/Matrix.h` / `.cpp` | Matrix<R,C> 模板矩阵 + Inverse |
| `Core/Structures/Color.h` | ARGB 颜色结构 |
| `Core/Structures/Buffer.h` | 通用二维缓冲模板（ColorBuffer/DepthBuffer） |
| `Core/Consts.h` | 所有枚举常量定义 |
| `Core/PreDefines.h` | 基础类型别名 + override 宏 |
| `Core/Common.h` | Core 层统一导出头文件 |
| `Core/Math.h` / `.cpp` | 数学工具函数（Clamp/Lerp/Log2/IsPowerOf2 等） |
| `Core/Exception.h` | 异常类定义 |
| `Core/Interfaces/IClonable.h` | 克隆接口模板 |
| `Core/Interfaces/IStringify.h` | 字符串化接口 |
