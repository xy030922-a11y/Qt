#include <QApplication>
#include <QCompleter>
#include <QLineEdit>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // 创建输入框。
    // Create the input box.
    QLineEdit inputLineEdit;
    inputLineEdit.setWindowTitle(
        QStringLiteral("QCompleter 最小示例 / Minimal Example"));
    inputLineEdit.setPlaceholderText(
        QStringLiteral("输入 ap 或 北 / Type ap or 北"));
    inputLineEdit.resize(460, 40);

    // 准备候选词。
    // Prepare completion candidates.
    const QStringList completionStringList{
        QStringLiteral("Apple"),
        QStringLiteral("Application"),
        QStringLiteral("Apricot"),
        QStringLiteral("Banana"),
        QStringLiteral("北京"),
        QStringLiteral("北海"),
        QStringLiteral("上海")
    };

    // 创建补全器并安装到输入框。
    // Create the completer and install it on the input box.
    QCompleter *inputCompleter =
        new QCompleter(completionStringList, &inputLineEdit);
    inputCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    inputCompleter->setFilterMode(Qt::MatchStartsWith);
    inputLineEdit.setCompleter(inputCompleter);

    inputLineEdit.show();
    return application.exec();
}
