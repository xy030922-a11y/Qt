# QLabel 详解 / QLabel Guide

本章使用 **Qt 6 + C++17 + CMake**，通过一个最小程序介绍 `QLabel` 的用途和常见用法。

*This chapter uses **Qt 6 + C++17 + CMake** and introduces `QLabel` through a minimal program.*

## 1. 当前目录 / This Directory

目录中只包含运行本例所需的三个文件：

*The directory contains only the three files required by this example:*

```text
20.1.QLabel/
├── README.md        # 双语教程 / Bilingual tutorial
├── main.cpp         # 最小示例 / Minimal example
└── CMakeLists.txt   # CMake 构建配置 / CMake build configuration
```

- `README.md`：说明 `QLabel` 的用法、业务场景和常见错误。  
  *Explains QLabel usage, business scenarios, and common mistakes.*
- `main.cpp`：创建一个显示中英文富文本的 `QLabel`。  
  *Creates a QLabel that displays bilingual rich text.*
- `CMakeLists.txt`：查找 Qt 6 Widgets，构建并链接示例。  
  *Finds Qt 6 Widgets, then builds and links the example.*

## 2. QLabel 是什么 / What Is QLabel?

`QLabel` 是 Qt Widgets 模块中的只读显示控件，继承自 `QFrame`。它主要用于显示短文本、富文本、数字、静态图片或动画，也常作为输入控件旁边的说明标签。

*`QLabel` is a read-only display widget in the Qt Widgets module and inherits from `QFrame`. It is mainly used for short text, rich text, numbers, static images, or animations, and is also commonly placed beside an input widget as its descriptive label.*

`QLabel` 本身不是文本编辑器。需要用户输入时使用 `QLineEdit`；需要显示或编辑较长、可滚动的文档时使用 `QTextEdit` 或 `QTextBrowser`。

*QLabel is not a text editor. Use `QLineEdit` for user input, and use `QTextEdit` or `QTextBrowser` for long, scrollable documents.*

## 3. QLabel 的用法 / Using QLabel

### 3.1 创建与修改文本 / Creating and Changing Text

可以在构造函数中传入文本，也可以稍后调用 `setText()`：

*Pass text to the constructor or call `setText()` later:*

```cpp
QLabel *titleLabel = new QLabel(
    QStringLiteral("标题 / Title"), parentWidget);

titleLabel->setText(QStringLiteral("新标题 / New title"));
QString currentText = titleLabel->text();
```

传入父对象后，父窗口销毁时会自动销毁标签，通常不需要手动 `delete`。

*When a parent is provided, the label is deleted automatically with its parent and normally needs no manual `delete`.*

### 3.2 纯文本、富文本和 Markdown / Plain Text, Rich Text, and Markdown

`QLabel` 默认使用 `Qt::AutoText` 猜测文本格式。若文本来自用户、网络或文件，建议显式指定格式，避免普通文本被误解析为 HTML。

*QLabel uses `Qt::AutoText` by default and guesses the format. When text comes from a user, network, or file, set the format explicitly so plain text is not accidentally interpreted as HTML.*

```cpp
// 把所有标签当作普通字符显示。
// Display every tag as ordinary characters.
plainTextLabel->setTextFormat(Qt::PlainText);
plainTextLabel->setText(QStringLiteral("<b>不是粗体 / Not bold</b>"));

// 解析 Qt 支持的 HTML 子集。
// Parse the HTML subset supported by Qt.
richTextLabel->setTextFormat(Qt::RichText);
richTextLabel->setText(QStringLiteral("<b>粗体 / Bold</b>"));

// 解析 Markdown。
// Parse Markdown.
markdownLabel->setTextFormat(Qt::MarkdownText);
markdownLabel->setText(QStringLiteral("**粗体 / Bold**"));
```

显示不可信内容时，优先选择 `Qt::PlainText`，或先对允许的富文本进行清理。

*Prefer `Qt::PlainText` for untrusted content, or sanitize rich text before displaying it.*

### 3.3 对齐、换行和边距 / Alignment, Wrapping, and Margins

```cpp
messageLabel->setAlignment(Qt::AlignCenter);
messageLabel->setWordWrap(true);
messageLabel->setMargin(12);
messageLabel->setIndent(4);
```

- `setAlignment()`：设置水平和垂直对齐方式。  
  *Sets horizontal and vertical alignment.*
- `setWordWrap(true)`：空间不足时按单词边界换行。  
  *Wraps text at word boundaries when space is limited.*
- `setMargin()`：设置边框内侧与内容之间的距离。  
  *Sets the distance between the inner frame and the content.*
- `setIndent()`：设置文本缩进。  
  *Sets text indentation.*

对齐标志可以组合：

*Alignment flags can be combined:*

```cpp
messageLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
```

### 3.4 显示图片 / Displaying an Image

```cpp
QPixmap logoPixmap(QStringLiteral(":/images/logo.png"));
imageLabel->setPixmap(logoPixmap);
imageLabel->setAlignment(Qt::AlignCenter);
```

`setPixmap()` 会清除之前显示的文本。实际项目建议把图片放入 Qt Resource System（`.qrc`），避免依赖当前工作目录。

*`setPixmap()` clears previously displayed text. In production, store images in the Qt Resource System (`.qrc`) instead of relying on the current working directory.*

`setScaledContents(true)` 会让图片填满标签，但可能改变宽高比并导致变形。更可控的做法是使用 `QPixmap::scaled()`：

*`setScaledContents(true)` fills the label but may distort the image by changing its aspect ratio. `QPixmap::scaled()` offers more control:*

```cpp
imageLabel->setPixmap(logoPixmap.scaled(
    imageLabel->size(),
    Qt::KeepAspectRatio,
    Qt::SmoothTransformation));
```

### 3.5 显示数字 / Displaying Numbers

`setNum()` 可以把整数或浮点数转换为普通文本：

*`setNum()` converts an integer or floating-point value to plain text:*

```cpp
countLabel->setNum(42);
priceLabel->setNum(19.95);
```

需要单位、精度或本地化格式时，先用 `QString` 格式化通常更清楚：

*For units, precision, or localized formatting, format a QString first:*

```cpp
priceLabel->setText(QStringLiteral("¥%1").arg(price, 0, 'f', 2));
```

### 3.6 允许复制文本 / Allowing Text Selection

`QLabel` 默认主要用于显示。若希望用户用鼠标选中并复制文本，需要开启交互标志：

*QLabel is display-oriented by default. Enable interaction flags when users should select and copy its text:*

```cpp
valueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
```

### 3.7 链接与信号 / Links and Signals

富文本中的链接可以通过 `linkActivated(const QString &)` 和 `linkHovered(const QString &)` 处理：

*Links in rich text can be handled through `linkActivated(const QString &)` and `linkHovered(const QString &)`:*

```cpp
linkLabel->setTextFormat(Qt::RichText);
linkLabel->setText(QStringLiteral(
    "<a href='settings'>打开设置 / Open settings</a>"));
linkLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

connect(linkLabel, &QLabel::linkActivated,
        parentWidget, [](const QString &link) {
    // 根据 link 执行业务操作。
    // Perform the business action represented by link.
});
```

调用 `setOpenExternalLinks(true)` 后，QLabel 会尝试用系统应用打开链接，而不是发出 `linkActivated()`。只应为可信 URL 开启自动外部跳转。

*With `setOpenExternalLinks(true)`, QLabel asks the system to open links instead of emitting `linkActivated()`. Enable automatic external navigation only for trusted URLs.*

### 3.8 Buddy 与键盘助记键 / Buddy and Keyboard Mnemonics

标签文本中 `&` 后的字符可以成为助记键，`setBuddy()` 指定按下该助记键后获得焦点的控件：

*A character after `&` becomes a mnemonic, and `setBuddy()` specifies the widget that receives focus when the mnemonic is activated:*

```cpp
QLabel *nameLabel = new QLabel(
    QStringLiteral("姓名(&N) / &Name:"), parentWidget);
QLineEdit *nameLineEdit = new QLineEdit(parentWidget);
nameLabel->setBuddy(nameLineEdit);
```

在常见桌面环境中，用户可使用 `Alt+N` 把焦点移到输入框。要显示真正的 `&` 字符，应写成 `&&`。

*In common desktop environments, `Alt+N` moves focus to the input. Write `&&` to display a literal ampersand.*

## 4. 示例程序 / Example Program

下面就是本目录 `main.cpp` 的最小示例。它只创建一个顶层 `QLabel`，展示双语富文本、居中、自动换行和内容边距。

*The following is the minimal `main.cpp` in this directory. It creates one top-level QLabel and demonstrates bilingual rich text, centering, word wrapping, and a content margin.*

```cpp
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环和全局资源。
    // QApplication manages the event loop and global resources of a Widgets app.
    QApplication application(argc, argv);

    // QLabel 可以直接作为顶层窗口显示文本；栈对象会在 main() 结束时自动销毁。
    // QLabel can display text as a top-level window; this stack object is destroyed automatically.
    QLabel messageLabel;
    messageLabel.setWindowTitle(QStringLiteral("QLabel 最小示例 / Minimal Example"));

    // 显式指定富文本格式，避免 QLabel 自动猜测文本格式。
    // Select rich text explicitly so QLabel does not have to guess the text format.
    messageLabel.setTextFormat(Qt::RichText);
    messageLabel.setText(QStringLiteral(
        "<h2>你好，QLabel！</h2>"
        "<p>Hello, QLabel!</p>"
        "<p>用于显示文本和图片。 / Used to display text and images.</p>"));

    // 文本居中、允许换行，并在内容周围保留边距。
    // Center the text, enable word wrapping, and add space around the content.
    messageLabel.setAlignment(Qt::AlignCenter);
    messageLabel.setWordWrap(true);
    messageLabel.setMargin(16);
    messageLabel.resize(420, 220);

    messageLabel.show();
    return application.exec();
}
```

### 构建和运行 / Build and Run

使用 Qt Creator 时，打开 `CMakeLists.txt`，选择 Qt 6 Desktop Kit，然后构建并运行 `QLabelExample`。

*In Qt Creator, open `CMakeLists.txt`, select a Qt 6 Desktop Kit, then build and run `QLabelExample`.*

当前机器也可以使用以下 PowerShell 命令：

*The current machine can also use these PowerShell commands:*

```powershell
$env:Path = "C:\Qt\6.5.3\mingw_64\bin;C:\Qt\Tools\mingw1120_64\bin;C:\Qt\Tools\Ninja;$env:Path"

cmake -S . -B build -G Ninja `
  -DCMAKE_PREFIX_PATH=C:/Qt/6.5.3/mingw_64 `
  -DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw1120_64/bin/g++.exe `
  -DCMAKE_MAKE_PROGRAM=C:/Qt/Tools/Ninja/ninja.exe

cmake --build build
.\build\QLabelExample.exe
```

编译器必须和 Qt 套件匹配。MinGW 版 Qt 应使用兼容的 MinGW 编译器，不能直接换成 MSVC。

*The compiler must match the Qt kit. A MinGW Qt build requires a compatible MinGW compiler and cannot simply be replaced by MSVC.*

## 5. 实际业务场景 / Real-World Business Scenarios

### 5.1 表单字段说明 / Form Field Captions

在登录、注册和设置页面中，QLabel 常放在 `QLineEdit`、`QComboBox` 等输入控件旁边，并通过 `setBuddy()` 提供键盘访问。

*On login, registration, and settings pages, QLabel commonly appears beside inputs such as `QLineEdit` and `QComboBox`, with `setBuddy()` providing keyboard access.*

### 5.2 状态和结果显示 / Status and Result Display

```cpp
statusLabel->setText(success
    ? QStringLiteral("保存成功 / Saved successfully")
    : QStringLiteral("保存失败 / Save failed"));
```

它适合显示连接状态、校验结果、计算值或简短提示。如果状态需要用户操作，可配合按钮或链接信号。

*It is suitable for connection status, validation results, calculated values, or brief hints. Pair it with a button or link signal when the status requires an action.*

### 5.3 图片预览 / Image Preview

头像、商品缩略图、二维码和摄像头快照可以通过 `setPixmap()` 显示。频繁缩放大图时，应缓存缩放结果，避免每次重绘都进行昂贵转换。

*Avatars, product thumbnails, QR codes, and camera snapshots can be displayed with `setPixmap()`. Cache scaled results for frequently redrawn large images instead of performing expensive conversions on every repaint.*

### 5.4 带格式的帮助信息 / Formatted Help Text

少量粗体、颜色和链接可使用富文本 QLabel。内容很长、需要滚动或复杂导航时，应改用 `QTextBrowser`。

*Use a rich-text QLabel for small amounts of bold text, color, and links. Switch to `QTextBrowser` for long, scrollable, or navigation-heavy content.*

### 5.5 动画提示 / Animated Indicator

QLabel 可通过 `setMovie(QMovie *)` 显示 GIF 等动画，适合轻量加载提示。QLabel 不取得 `QMovie` 的所有权，因此必须保证 `QMovie` 的生命周期足够长。

*QLabel can display an animation such as a GIF through `setMovie(QMovie *)`, which is useful for a lightweight loading indicator. QLabel does not take ownership of QMovie, so the movie must remain alive long enough.*

## 6. 常见错误 / Common Mistakes

### 错误 1：让用户编辑 QLabel / Expecting Users to Edit QLabel

QLabel 是显示控件，不是输入控件。单行输入使用 `QLineEdit`，多行编辑使用 `QTextEdit`。

*QLabel is for display, not input. Use `QLineEdit` for single-line input and `QTextEdit` for multiline editing.*

### 错误 2：依赖 AutoText 显示不可信文本 / Using AutoText for Untrusted Content

默认的 `Qt::AutoText` 可能把类似 HTML 的内容作为富文本解析。显示外部数据时应明确使用 `Qt::PlainText`，或先清理富文本。

*The default `Qt::AutoText` may interpret HTML-like content as rich text. Use `Qt::PlainText` for external data, or sanitize rich text first.*

### 错误 3：开启 scaledContents 后图片变形 / Distorted Images with scaledContents

`setScaledContents(true)` 可能忽略原图宽高比。使用 `QPixmap::scaled(size, Qt::KeepAspectRatio, ...)` 可以避免拉伸变形。

*`setScaledContents(true)` can ignore the source aspect ratio. Use `QPixmap::scaled(size, Qt::KeepAspectRatio, ...)` to prevent stretching.*

### 错误 4：忘记开启文本换行 / Forgetting Word Wrap

长文本默认不换行，可能扩大标签的尺寸提示并破坏布局。需要多行显示时调用 `setWordWrap(true)`。

*Long text does not wrap by default and may enlarge the label’s size hint and disrupt the layout. Call `setWordWrap(true)` for multiline display.*

### 错误 5：建立连接后没有初始化界面 / Not Initializing After Connecting

连接模型信号不会自动调用槽函数。建立连接后应主动设置一次 QLabel 的初始文本。

*Connecting a model signal does not invoke the slot immediately. Set QLabel’s initial text explicitly after creating the connection.*

### 错误 6：忽略对象生命周期 / Ignoring Object Lifetime

没有父对象的堆对象需要手动管理；传入父窗口或使用栈对象可让生命周期更清楚。使用 `setMovie()` 时还要单独保证 `QMovie` 存活。

*A heap object without a parent needs manual management. Give it a parent or use a stack object for clearer lifetime. With `setMovie()`, also keep the separate QMovie alive.*

## Qt 官方文档 / Official Qt Documentation

- [QLabel Class](https://doc.qt.io/qt-6/qlabel.html)
- [Qt::TextFormat](https://doc.qt.io/qt-6/qt.html#TextFormat-enum)
- [QPixmap Class](https://doc.qt.io/qt-6/qpixmap.html)
- [QMovie Class](https://doc.qt.io/qt-6/qmovie.html)
- [Rich Text Processing](https://doc.qt.io/qt-6/richtext.html)
