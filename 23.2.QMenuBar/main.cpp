#include <QAction>
#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环。
    // QApplication manages the event loop of a Widgets application.
    QApplication application(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle(QStringLiteral("QMenuBar 最小示例 / Minimal Example"));

    // QMainWindow::menuBar() 返回由主窗口管理的菜单栏。
    // QMainWindow::menuBar() returns the menu bar managed by the main window.
    QMenuBar *mainMenuBar = mainWindow.menuBar();
    QMenu *fileMenu = mainMenuBar->addMenu(QStringLiteral("文件(&F) / &File"));
    QAction *newAction = fileMenu->addAction(QStringLiteral("新建 / New"));
    QAction *exitAction = fileMenu->addAction(QStringLiteral("退出 / Exit"));

    QLabel *resultLabel = new QLabel(
        QStringLiteral("从菜单栏选择操作 / Select an action from the menu bar"));
    resultLabel->setAlignment(Qt::AlignCenter);
    mainWindow.setCentralWidget(resultLabel);

    // 菜单项仍由 QAction::triggered() 执行业务。
    // Menu commands still run through QAction::triggered().
    QObject::connect(newAction, &QAction::triggered, resultLabel, [resultLabel] {
        resultLabel->setText(QStringLiteral("已新建文档 / New document created"));
    });
    QObject::connect(exitAction, &QAction::triggered,
                     &application, &QApplication::quit);

    mainWindow.resize(480, 260);
    mainWindow.show();
    return application.exec();
}
