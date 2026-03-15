[根目录](../../CLAUDE.md) > [MyGL](../) > **Context**

# MyGL/Context - AI 上下文文档

## 变更记录 (Changelog)

| 版本 | 日期 | 说明 |
|------|------|------|
| v1.0 | 2026-03-15 | 初始化生成 |

---

## 模块职责

`MyGL/Context/` 负责渲染上下文管理，即**将 GL 渲染结果呈现到操作系统窗口**的这一层抽象。该层将窗口系统与渲染核心完全解耦：

- `RenderingContext`：纯虚抽象基类，定义窗口生命周期与帧呈现接口
- `SFMLContext`：基于 SFML 2.x 的具体实现，负责创建窗口、将 ColorBuffer 转换为 SFML Texture 并渲染，以及从文件加载纹理

---

## 入口与启动

```cpp
#include "Context/SFMLContext.h"

auto context = make_shared<SFMLContext>();
context->CreateWindow(800, 600, "MyGL");

// 每帧渲染循环：
// 1. 调用 GL 渲染指令
// 2. GL::Instance().Flush() 写入 ColorBuffer
// 3. context->SwapBuffers() 将 ColorBuffer 推送到窗口
```

`SFMLContext::CreateWindow` 内部会调用 `GL::Instance().Init(width, height)` 完成缓冲区分配，因此**创建窗口即初始化 GL**。

---

## 对外接口

### RenderingContext（抽象基类）

```cpp
class RenderingContext {
public:
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void CreateWindow(int width, int height, const string& title) = 0;
    virtual void SwapBuffers();        // 调用 GL::Flush() 并将结果推送到窗口
protected:
    virtual void FlushWindow(weak_ptr<const ColorBuffer> buffer) = 0;
};
```

### SFMLContext（SFML 具体实现）

| 方法 | 说明 |
|------|------|
| `CreateWindow(w, h, title)` | 创建 SFML 窗口，初始化 GL 缓冲，创建内部 SFML Texture/Sprite |
| `FlushWindow(buffer)` | 将 ColorBuffer 的原始字节上传到 SFML Texture，并通过 Sprite 绘制到窗口 |
| `GetWindow()` | 返回底层 `sf::RenderWindow` 的 `weak_ptr`（供主循环做事件处理） |
| `LoadTexture(filename)` | 用 SFML 加载图片文件，自动垂直翻转、转换 ABGR->ARGB，返回 `shared_ptr<Texture>` |

---

## 关键依赖与配置

- 依赖 **SFML 2.x**（`SFML/include/SFML/Graphics.hpp`）
- 依赖 `MyGL/Core/GL.h`（通过 `GL::Instance()` 调用 Init/Flush/GetBuffer）
- SFML 内部像素格式为 **ABGR**，`SFMLContext` 在 `_ConvertABGRToARGB` 中完成格式转换
- 纹理加载后自动调用 `image.flipVertically()`，将图片坐标系从屏幕坐标系转换为 UV 坐标系（Y 轴翻转）

---

## 数据模型

`SFMLContext` 内部持有：

```
_window   shared_ptr<sf::RenderWindow>   SFML 窗口句柄
_texture  shared_ptr<sf::Texture>        用于接收 ColorBuffer 字节的 SFML GPU 纹理
_sprite   shared_ptr<sf::Sprite>         全屏渲染 sprite，绑定 _texture
```

---

## 测试与质量

- 无自动化测试；通过每帧 FPS 监测和目视检查窗口输出验证
- `_ConvertABGRToARGB` 的正确性通过纹理显示颜色是否正常来验证

---

## 常见问题 (FAQ)

**Q: 为什么纹理加载后颜色看起来是对的，但如果不调用 flipVertically 图片会上下颠倒？**

A: OpenGL/MyGL 的 UV 坐标原点在左下角，而 SFML/BMP/JPEG 的图片坐标原点在左上角。`flipVertically` 在加载时统一纠正这个差异。

**Q: 为什么需要 ABGR->ARGB 转换？**

A: SFML `Image::getPixelsPtr()` 返回的像素格式为 RGBA（实际字节序为 ABGR little-endian 的 uint32），而 MyGL 内部 `Color` 使用 ARGB 格式的 `uint32`。`_ConvertABGRToARGB` 完成这个格式适配。

**Q: 如何换用其他窗口库？**

A: 继承 `RenderingContext`，实现 `CreateWindow`/`FlushWindow`/`width`/`height` 四个方法即可。不需要修改 Core 层任何代码。

---

## 相关文件清单

| 文件 | 说明 |
|------|------|
| `Context/RenderingContext.h` / `.cpp` | 渲染上下文抽象基类，定义窗口与帧呈现接口 |
| `Context/SFMLContext.h` | SFML 具体实现（全部为 header-only inline 实现） |
