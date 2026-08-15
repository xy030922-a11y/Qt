#include <QAction>
#include <QApplication>
#include <QKeySequence>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环。
    // QApplication manages the event loop of a Widgets application.
    QApplication application(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle(
        QStringLiteral("快捷键与状态提示 / Shortcuts and Status Tips"));

    // 使用标准按键序列可自动适配不同操作系统。
    // A standard key sequence adapts automatically to each operating system.
    QAction *saveAction = new QAction(
        QStringLiteral("保存 / Save"), &mainWindow);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(
        QStringLiteral("保存当前文档 / Save the current document"));

    QMenu *fileMenu = mainWindow.menuBar()->addMenu(
        QStringLiteral("文件(&F) / &File"));
    fileMenu->addAction(saveAction);
    QToolBar *fileToolBar = mainWindow.addToolBar(
        QStringLiteral("文件 / File"));
    fileToolBar->addAction(saveAction);

    QLabel *resultLabel = new QLabel(
        QStringLiteral("按下保存快捷键 / Press the Save shortcut"));
    resultLabel->setAlignment(Qt::AlignCenter);
    mainWindow.setCentralWidget(resultLabel);

    QStatusBar *mainStatusBar = mainWindow.statusBar();
    mainStatusBar->showMessage(QStringLiteral("就绪 / Ready"));

    // 菜单、工具栏和快捷键都会触发同一个 QAction。
    // The menu, toolbar, and shortcut all trigger the same QAction.
    QObject::connect(saveAction, &QAction::triggered,
                     resultLabel, [resultLabel, mainStatusBar] {
        resultLabel->setText(QStringLiteral("文档已保存 / Document saved"));
        mainStatusBar->showMessage(
            QStringLiteral("保存成功 / Saved successfully"), 2000);
    });

    mainWindow.resize(520, 300);
    mainWindow.show();
    return application.exec();
}
