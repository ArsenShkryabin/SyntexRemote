//
// SyntexRemote - Killer Feature: Silent Remote Terminal & AI Sysadmin Shell
// Copyright (C) 2026 SyntexHelp Team (based on Aspia)
//

#ifndef CONSOLE_SILENT_TERMINAL_DIALOG_H
#define CONSOLE_SILENT_TERMINAL_DIALOG_H

#include <QDialog>
#include <QProcess>

class QComboBox;
class QLineEdit;
class QPushButton;
class QPlainTextEdit;
class QLabel;

namespace console {

class SilentTerminalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SilentTerminalDialog(QWidget* parent = nullptr, const QString& default_host = QString());
    ~SilentTerminalDialog() override;

    void setTargetHost(const QString& host);

private slots:
    void onExecuteClicked();
    void onPresetSelected(int index);
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessReadyRead();
    void onProcessError(QProcess::ProcessError error);
    void onClearOutput();

private:
    void setupUi();
    void appendLog(const QString& text, const QString& color = QStringLiteral("#e2e8f0"));

    QComboBox* combo_target_ = nullptr;
    QComboBox* combo_shell_ = nullptr;
    QComboBox* combo_presets_ = nullptr;
    QLineEdit* edit_command_ = nullptr;
    QPushButton* btn_execute_ = nullptr;
    QPushButton* btn_clear_ = nullptr;
    QPlainTextEdit* terminal_output_ = nullptr;
    QLabel* status_label_ = nullptr;

    QProcess* process_ = nullptr;
};

} // namespace console

#endif // CONSOLE_SILENT_TERMINAL_DIALOG_H
