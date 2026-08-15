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
