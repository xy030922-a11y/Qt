#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QKeySequence>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QStyle>
#include <QTextCursor>
#include <QTextEdit>
#include <QToolBar>
#include <QWidgetAction>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环和全局资源。
    // QApplication manages the event loop and global resources of a Widgets app.
    QApplication application(argc, argv);

    // QMainWindow 原生支持菜单栏、工具栏、中央控件和状态栏。
    // QMainWindow natively supports a menu bar, toolbars, a central widget, and a status bar.
    QMainWindow mainWindow;
    mainWindow.setWindowTitle(
        QStringLiteral("菜单、工具栏与操作 / Menus, Toolbars, and Actions"));

    QTextEdit *editorTextEdit = new QTextEdit(&mainWindow);
    editorTextEdit->setPlainText(QStringLiteral(
        "欢迎使用 Qt 文本编辑器示例。\n"
        "Welcome to the Qt text editor example.\n\n"
        "请尝试菜单、工具栏、快捷键和右键菜单。\n"
        "Try the menus, toolbar, shortcuts, and context menu."));
    mainWindow.setCentralWidget(editorTextEdit);

    QStatusBar *mainStatusBar = mainWindow.statusBar();
    mainStatusBar->showMessage(QStringLiteral("就绪 / Ready"));

    // 每条命令只创建一个 QAction，再复用到菜单、工具栏和右键菜单中。
    // Create each command once as a QAction, then reuse it in menus, toolbars, and context menus.
    QAction *newAction = new QAction(
        mainWindow.style()->standardIcon(QStyle::SP_FileIcon),
        QStringLiteral("新建(&N) / New"), &mainWindow);
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(QStringLiteral("清空编辑器 / Clear the editor"));

    QAction *saveAction = new QAction(
        mainWindow.style()->standardIcon(QStyle::SP_DialogSaveButton),
        QStringLiteral("保存(&S) / Save"), &mainWindow);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(QStringLiteral("模拟保存当前内容 / Simulate saving the current content"));

    QAction *exitAction = new QAction(QStringLiteral("退出(&X) / Exit"), &mainWindow);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip(QStringLiteral("关闭应用程序 / Close the application"));

    QAction *cutAction = new QAction(QStringLiteral("剪切(&T) / Cut"), &mainWindow);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(QStringLiteral("剪切选中的文本 / Cut the selected text"));
    cutAction->setEnabled(false);

    QAction *copyAction = new QAction(QStringLiteral("复制(&C) / Copy"), &mainWindow);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(QStringLiteral("复制选中的文本 / Copy the selected text"));
    copyAction->setEnabled(false);

    QAction *pasteAction = new QAction(QStringLiteral("粘贴(&P) / Paste"), &mainWindow);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(QStringLiteral("粘贴剪贴板文本 / Paste text from the clipboard"));

    QAction *aboutAction = new QAction(QStringLiteral("关于(&A) / About"), &mainWindow);
    aboutAction->setShortcut(QKeySequence::HelpContents);
    aboutAction->setStatusTip(QStringLiteral("查看示例说明 / Show information about this example"));

    QObject::connect(newAction, &QAction::triggered, editorTextEdit, &QTextEdit::clear);
    QObject::connect(saveAction, &QAction::triggered, &mainWindow, [mainStatusBar]() {
        mainStatusBar->showMessage(QStringLiteral("内容已保存（演示）/ Content saved (demo)"), 2500);
    });
    QObject::connect(exitAction, &QAction::triggered, &mainWindow, &QWidget::close);
    QObject::connect(cutAction, &QAction::triggered, editorTextEdit, &QTextEdit::cut);
    QObject::connect(copyAction, &QAction::triggered, editorTextEdit, &QTextEdit::copy);
    QObject::connect(pasteAction, &QAction::triggered, editorTextEdit, &QTextEdit::paste);
    QObject::connect(editorTextEdit, &QTextEdit::copyAvailable, cutAction, &QAction::setEnabled);
    QObject::connect(editorTextEdit, &QTextEdit::copyAvailable, copyAction, &QAction::setEnabled);
    QObject::connect(aboutAction, &QAction::triggered, &mainWindow, [&mainWindow]() {
        QMessageBox::about(
            &mainWindow,
            QStringLiteral("关于 / About"),
            QStringLiteral("本例演示 QMenu、QMenuBar、QToolBar、QAction、"
                           "QActionGroup、QWidgetAction 和右键菜单。\n\n"
                           "This example demonstrates menus, toolbars, actions, "
                           "action groups, widget actions, and context menus."));
    });

    // QActionGroup 让三个可选操作保持互斥。
    // QActionGroup keeps the three checkable actions mutually exclusive.
    QActionGroup *alignmentActionGroup = new QActionGroup(&mainWindow);
    alignmentActionGroup->setExclusive(true);

    QAction *leftAlignAction = new QAction(
        QStringLiteral("左对齐 / Align Left"), alignmentActionGroup);
    leftAlignAction->setCheckable(true);
    leftAlignAction->setChecked(true);
    leftAlignAction->setData(static_cast<int>(Qt::AlignLeft));
    leftAlignAction->setStatusTip(QStringLiteral("文本左对齐 / Align text to the left"));

    QAction *centerAlignAction = new QAction(
        QStringLiteral("居中 / Align Center"), alignmentActionGroup);
    centerAlignAction->setCheckable(true);
    centerAlignAction->setData(static_cast<int>(Qt::AlignHCenter));
    centerAlignAction->setStatusTip(QStringLiteral("文本居中 / Center the text"));

    QAction *rightAlignAction = new QAction(
        QStringLiteral("右对齐 / Align Right"), alignmentActionGroup);
    rightAlignAction->setCheckable(true);
    rightAlignAction->setData(static_cast<int>(Qt::AlignRight));
    rightAlignAction->setStatusTip(QStringLiteral("文本右对齐 / Align text to the right"));

    QObject::connect(
        alignmentActionGroup, &QActionGroup::triggered, editorTextEdit,
        [editorTextEdit](QAction *selectedAction) {
            editorTextEdit->setAlignment(
                static_cast<Qt::AlignmentFlag>(selectedAction->data().toInt()));
        });

    // QMenuBar 放置顶层菜单，QMenu 组织可复用的 QAction。
    // QMenuBar holds top-level menus, while QMenu organizes reusable QActions.
    QMenuBar *mainMenuBar = mainWindow.menuBar();
    QMenu *fileMenu = mainMenuBar->addMenu(QStringLiteral("文件(&F) / File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu *editMenu = mainMenuBar->addMenu(QStringLiteral("编辑(&E) / Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);

    QMenu *formatMenu = mainMenuBar->addMenu(QStringLiteral("格式(&O) / Format"));
    QMenu *alignmentMenu = formatMenu->addMenu(QStringLiteral("对齐 / Alignment"));
    alignmentMenu->addActions(alignmentActionGroup->actions());

    QMenu *helpMenu = mainMenuBar->addMenu(QStringLiteral("帮助(&H) / Help"));
    helpMenu->addAction(aboutAction);

    // 同一个 QAction 加入工具栏后，会与菜单项共享状态和触发逻辑。
    // Adding the same QAction to a toolbar shares its state and trigger logic with the menu item.
    QToolBar *mainToolBar = new QToolBar(
        QStringLiteral("主工具栏 / Main Toolbar"), &mainWindow);
    mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
    mainToolBar->setMovable(true);
    mainWindow.addToolBar(mainToolBar);
    mainToolBar->addAction(newAction);
    mainToolBar->addAction(saveAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(cutAction);
    mainToolBar->addAction(copyAction);
    mainToolBar->addAction(pasteAction);
    mainToolBar->addSeparator();
    mainToolBar->addActions(alignmentActionGroup->actions());

    // QWidgetAction 把搜索输入框作为一个操作放入工具栏，并接管其所有权。
    // QWidgetAction places a search field in the toolbar as an action and takes ownership of it.
    QWidgetAction *searchWidgetAction = new QWidgetAction(&mainWindow);
    QLineEdit *searchLineEdit = new QLineEdit;
    searchLineEdit->setPlaceholderText(QStringLiteral("查找文本 / Find text"));
    searchLineEdit->setClearButtonEnabled(true);
    searchLineEdit->setMaximumWidth(220);
    searchWidgetAction->setDefaultWidget(searchLineEdit);
    mainToolBar->addSeparator();
    mainToolBar->addAction(searchWidgetAction);

    QObject::connect(searchLineEdit, &QLineEdit::returnPressed, &mainWindow,
                     [editorTextEdit, searchLineEdit, mainStatusBar]() {
        const QString searchText = searchLineEdit->text();
        if (searchText.isEmpty()) {
            mainStatusBar->showMessage(QStringLiteral("请输入查找内容 / Enter text to find"), 2500);
            return;
        }

        if (editorTextEdit->find(searchText)) {
            mainStatusBar->showMessage(QStringLiteral("已找到 / Match found"), 2500);
            return;
        }

        QTextCursor startTextCursor = editorTextEdit->textCursor();
        startTextCursor.movePosition(QTextCursor::Start);
        editorTextEdit->setTextCursor(startTextCursor);
        const bool foundFromStart = editorTextEdit->find(searchText);
        mainStatusBar->showMessage(
            foundFromStart
                ? QStringLiteral("已从开头找到 / Match found from the beginning")
                : QStringLiteral("未找到 / No match found"),
            2500);
    });

    // CustomContextMenu 信号提供点击位置；临时 QMenu 复用已有编辑操作。
    // CustomContextMenu provides the click position; a temporary QMenu reuses existing edit actions.
    editorTextEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(
        editorTextEdit, &QWidget::customContextMenuRequested, editorTextEdit,
        [editorTextEdit, cutAction, copyAction, pasteAction](const QPoint &position) {
            QMenu contextMenu(editorTextEdit);
            contextMenu.addAction(cutAction);
            contextMenu.addAction(copyAction);
            contextMenu.addAction(pasteAction);
            contextMenu.exec(editorTextEdit->mapToGlobal(position));
        });

    mainWindow.resize(860, 520);
    mainWindow.show();
    return application.exec();
}
