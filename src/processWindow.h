#ifndef __processWindow_H__
#define __processWindow_H__

#include <QWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QMainWindow>
#include <QProcess>
#include <QtGui>
#include <qtcontrols_global.h>


class QTCON_EXPORT processWindow : public QMainWindow
{
    Q_OBJECT

public:
    processWindow(QWidget * = 0, bool display=true, QWidget * = 0);
    ~processWindow();

    bool isRunning();
    void start(QString command);
    QWidget *getProcessCaller();
    Q_PID getProcessID();

public slots:

    bool tryTerminate();

protected slots:
    void processFinished();
    void processStarted();
    void processError(QProcess::ProcessError err);
    void closeButtonClicked();
    void updateError();
    void updateText();

signals:

    void processClose();

protected:
    void closeEvent(QCloseEvent *);

private:
    QTextEdit *outputWindow, *debugWindow;
    QProcess *termProcess;
    QSplitter* splitter;
    bool displayWindow;
    QWidget *thisCaller;
    Q_PID thisPID;
};

#endif
