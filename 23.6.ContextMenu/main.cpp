#include <QAction>
#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QMenu>
#include <QWidget>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环。
    // QApplication manages the event loop of a Widgets application.
    QApplication application(argc, argv);

    QLabel contextLabel(
        QStringLiteral("在此处单击右键 / Right-click here"));
    contextLabel.setWindowTitle(
        QStringLiteral("上下文菜单最小示例 / Context Menu Example"));
    contextLabel.setAlignment(Qt::AlignCenter);
    contextLabel.setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    // CustomContextMenu 让控件通过信号报告右键位置。
    // CustomContextMenu makes the widget report the right-click position by signal.
    contextLabel.setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(&contextLabel, &QWidget::customContextMenuRequested,
                     &contextLabel, [&contextLabel](const QPoint &position) {
        QMenu contextMenu(&contextLabel);
        QAction *copyAction = contextMenu.addAction(
            QStringLiteral("复制 / Copy"));
        QAction *resetAction = contextMenu.addAction(
            QStringLiteral("重置 / Reset"));

        // exec() 需要全局坐标，并返回用户选择的操作。
        // exec() expects global coordinates and returns the selected action.
        QAction *selectedAction = contextMenu.exec(
            contextLabel.mapToGlobal(position));
        if (selectedAction == copyAction) {
            contextLabel.setText(QStringLiteral("已选择复制 / Copy selected"));
        } else if (selectedAction == resetAction) {
            contextLabel.setText(
                QStringLiteral("在此处单击右键 / Right-click here"));
        }
    });

    contextLabel.resize(420, 220);
    contextLabel.show();
    return application.exec();
}
