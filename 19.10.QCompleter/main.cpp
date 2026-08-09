#include <QApplication>
#include <QCompleter>
#include <QLineEdit>
#include <QStringList>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环和全局资源。
    // QApplication manages the event loop and global resources of a Widgets app.
    QApplication app(argc, argv);

    // QLineEdit 是接收用户输入并显示补全结果的目标控件。
    // QLineEdit receives user input and displays the accepted completion.
    QLineEdit input;
    input.setWindowTitle(QStringLiteral("QCompleter 最小示例 / Minimal Example"));
    input.setPlaceholderText(
        QStringLiteral("输入 ap 或 北 / Type ap or 北"));
    input.resize(460, 40);

    // QStringList 是最简单的候选数据源；实际项目也可以使用任意项模型。
    // QStringList is the simplest source; real projects may use any item model.
    const QStringList words{
        QStringLiteral("Apple（苹果）"),
        QStringLiteral("Application（应用程序）"),
        QStringLiteral("Apricot（杏）"),
        QStringLiteral("Banana（香蕉）"),
        QStringLiteral("北京（Beijing）"),
        QStringLiteral("北海（Beihai）"),
        QStringLiteral("上海（Shanghai）")
    };

    // 把 input 设为父对象，窗口销毁时 completer 会被自动销毁。
    // Using input as the parent deletes completer automatically with the window.
    auto *completer = new QCompleter(words, &input);

    // 使用弹出列表显示候选项，这是默认模式，此处显式写出便于学习。
    // Show candidates in a popup; this is the default, stated explicitly for clarity.
    completer->setCompletionMode(QCompleter::PopupCompletion);

    // 英文字母匹配时忽略大小写，例如输入 "ap" 可以匹配 "Apple"。
    // Ignore case, so typing "ap" can match "Apple".
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    // 只匹配候选项开头；可改为 Qt::MatchContains 测试包含匹配。
    // Match prefixes only; try Qt::MatchContains for substring matching.
    completer->setFilterMode(Qt::MatchStartsWith);
    completer->setMaxVisibleItems(6);

    // 将补全器安装到输入框；用户可用方向键选择，按 Enter 接受。
    // Install it on the editor; use arrows to choose and Enter to accept.
    input.setCompleter(completer);

    input.show();
    return app.exec();
}
