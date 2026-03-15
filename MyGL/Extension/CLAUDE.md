[根目录](../../CLAUDE.md) > [MyGL](../) > **Extension**

# MyGL/Extension - AI 上下文文档

## 变更记录 (Changelog)

| 版本 | 日期 | 说明 |
|------|------|------|
| v1.0 | 2026-03-15 | 初始化生成 |

---

## 模块职责

`MyGL/Extension/` 提供基于 Core 层之上的**高层几何体封装**，使调用者无需手动管理顶点数组即可绘制常见形状：

- `Mesh`：通用几何体数据容器，持有顶点/法线/颜色/UV/索引数组，并通过 `GL::DrawArrays` 或 `GL::DrawElements` 一键提交绘制
- `MeshImporter`：内置几何体生成工厂，提供立方体、球体、曲面等标准网格的静态创建方法

---

## 入口与启动

```cpp
#include "Extension/Mesh.h"

// 使用内置几何体工厂创建 Mesh
auto sphere = MeshImporter::SharedSphereMesh(20);   // 20 slices 精细度
auto cube   = MeshImporter::SharedCubeMesh();
auto surf   = MeshImporter::SharedSurfaceMesh();

// 每帧绘制
sphere->Draw();   // 内部调用 GL::Instance().DrawElements(GL_TRIANGLES, ...)
```

---

## 对外接口

### Mesh 类

| 方法 | 说明 |
|------|------|
| `Mesh(vertices, colors, uvs, normals, elements)` | 构造函数，接受完整顶点数据；要求 vertices/colors/uvs/normals 数组等长 |
| `Draw()` | 若存在 elements 索引则调用 `DrawElements`，否则调用 `DrawArrays`；图元类型固定为 `GL_TRIANGLES` |
| `UseIndex()` | 判断是否使用索引缓冲（`elements.size() > 0`） |

### MeshImporter 类

| 方法 | 说明 |
|------|------|
| `SharedCubeMesh()` | 返回单位立方体网格（`shared_ptr<Mesh>`） |
| `SharedSphereMesh(slices)` | 返回 UV 球体网格，`slices` 控制横向/纵向精细度（默认 20） |
| `SharedSurfaceMesh()` | 返回平面曲面网格 |

---

## 关键依赖与配置

- 依赖 `MyGL/Core/GL.h`（调用 `GL::Instance().DrawArrays/DrawElements`）
- `Mesh` 构造时通过 `assert` 强制要求 vertices/colors/uvs/normals 数组长度一致
- 图元类型硬编码为 `GL_TRIANGLES`，不支持其他图元（线/点等需直接使用 Core 层 API）

---

## 数据模型

```
Mesh._vertices   vector<Vector3>   顶点位置（视口空间前）
Mesh._normals    vector<Vector3>   法线向量
Mesh._colors     vector<Color>     顶点颜色
Mesh._uvs        vector<Vector2>   纹理坐标
Mesh._elements   vector<uint32>    可选索引缓冲（空则使用顺序绘制）
```

---

## 测试与质量

- 无自动化测试；`SharedSphereMesh` 和 `SharedCubeMesh` 的正确性通过 `main.cpp` 中的 `SphereMesh` TestCase 目视验证
- `Mesh` 构造时的 `assert` 是唯一的运行时数据校验

---

## 常见问题 (FAQ)

**Q: 如何为 Mesh 添加自定义纹理？**

A: 在调用 `mesh->Draw()` 之前，通过 GL API 绑定纹理：

```cpp
GL::Instance().BindTexture(GL_TEXTURE_2D, myTexture);
mesh->Draw();
```

**Q: Mesh::Draw() 只支持三角形，如何绘制线框？**

A: 需要绕过 `Mesh` 层，直接使用 `GL::Instance().DrawArrays(GL_LINE_STRIP, ...)` 或 `GL::Begin(GL_LINES)` 等接口。

**Q: 如何新增一个内置几何体（例如圆柱体）？**

A: 在 `MyGL/Extension/Mesh.cpp` 中添加 `MeshImporter::SharedCylinderMesh()` 静态方法，手动生成顶点/法线/UV/索引数组，构造并返回 `make_shared<Mesh>(...)` 即可。

---

## 相关文件清单

| 文件 | 说明 |
|------|------|
| `Extension/Mesh.h` | Mesh 数据容器类 + MeshImporter 工厂类声明 |
| `Extension/Mesh.cpp` | MeshImporter 工厂方法实现（立方体/球体/曲面顶点生成逻辑） |
