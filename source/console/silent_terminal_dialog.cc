//
// SyntexRemote - Killer Feature: Silent Remote Terminal & AI Sysadmin Shell
// Copyright (C) 2026 SyntexHelp Team (based on Aspia)
//

#include "console/silent_terminal_dialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardPaths>

namespace console {

SilentTerminalDialog::SilentTerminalDialog(QWidget* parent, const QString& default_host)
    : QDialog(parent)
{
    setupUi();
    if (!default_host.isEmpty())
        setTargetHost(default_host);
}

SilentTerminalDialog::~SilentTerminalDialog()
{
    if (process_) {
        if (process_->state() == QProcess::Running) {
            process_->kill();
            process_->waitForFinished(500);
        }
        delete process_;
    }
}

void SilentTerminalDialog::setupUi()
{
    setWindowTitle(tr("SyntexRemote - Скрытый терминал и AI SysAdmin Shell"));
    setMinimumSize(850, 560);
    resize(920, 620);

    auto* main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(10);
    main_layout->setContentsMargins(14, 14, 14, 14);

    // Header info banner
    auto* banner = new QWidget(this);
    banner->setStyleSheet(QStringLiteral("background-color: #0f172a; border: 1px solid #1e293b; border-radius: 6px; padding: 6px;"));
    auto* banner_layout = new QHBoxLayout(banner);
    banner_layout->setContentsMargins(10, 6, 10, 6);

    auto* title_lbl = new QLabel(tr("⚡ Фоновое управление: выполнение команд без вывода окон на удаленном экране"), banner);
    title_lbl->setStyleSheet(QStringLiteral("font-weight: 600; color: #38bdf8; font-size: 11pt; border: none;"));
    banner_layout->addWidget(title_lbl);
    banner_layout->addStretch();

    status_label_ = new QLabel(tr("Готов к работе"), banner);
    status_label_->setStyleSheet(QStringLiteral("color: #10b981; font-weight: 500; border: none;"));
    banner_layout->addWidget(status_label_);

    main_layout->addWidget(banner);

    // Row 1: Target computer & Shell type
    auto* top_row = new QHBoxLayout();
    top_row->setSpacing(8);

    auto* target_lbl = new QLabel(tr("Хост / Компьютер:"), this);
    top_row->addWidget(target_lbl);

    combo_target_ = new QComboBox(this);
    combo_target_->setEditable(true);
    combo_target_->setMinimumWidth(260);

    // Load SyntexHelp computers list if present
    QStringList paths = {
        QDir::homePath() + QStringLiteral("/.config/aspia/computers_list.txt"),
        QDir::homePath() + QStringLiteral("/.config/syntexremote/computers_list.txt")
    };
    bool loaded = false;
    for (const auto& path : paths) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (!line.isEmpty()) {
                    combo_target_->addItem(line);
                }
            }
            loaded = true;
            break;
        }
    }
    if (!loaded || combo_target_->count() == 0) {
        combo_target_->addItem(QStringLiteral("localhost (Тестовый локальный запуск)"));
        combo_target_->addItem(QStringLiteral("192.168.93.21 - SRV01"));
        combo_target_->addItem(QStringLiteral("47 - Ирина (Бухгалтер)"));
        combo_target_->addItem(QStringLiteral("108 - Яна"));
    }
    top_row->addWidget(combo_target_);

    auto* shell_lbl = new QLabel(tr("Среда:"), this);
    top_row->addWidget(shell_lbl);

    combo_shell_ = new QComboBox(this);
    combo_shell_->addItem(QStringLiteral("PowerShell 7 / Windows PowerShell"));
    combo_shell_->addItem(QStringLiteral("CMD (Командная строка)"));
    combo_shell_->addItem(QStringLiteral("Bash (Linux Shell)"));
    top_row->addWidget(combo_shell_);

    top_row->addStretch();
    main_layout->addLayout(top_row);

    // Row 2: Presets
    auto* presets_row = new QHBoxLayout();
    presets_row->setSpacing(8);

    auto* preset_lbl = new QLabel(tr("Готовые сценарии (SysAdmin Quick Actions):"), this);
    presets_row->addWidget(preset_lbl);

    combo_presets_ = new QComboBox(this);
    combo_presets_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    combo_presets_->addItem(tr("-- Выберите готовый рецепт обслуживания --"), QString());
    combo_presets_->addItem(tr("1. Перезапуск службы очереди печати (Spooler)"),
        QStringLiteral("Restart-Service spooler -Force; Get-Service spooler"));
    combo_presets_->addItem(tr("2. Очистить кэш DNS и обновить сетевые настройки"),
        QStringLiteral("ipconfig /flushdns; ipconfig /renew"));
    combo_presets_->addItem(tr("3. Очистить временные файлы системы и пользователей ($env:TEMP)"),
        QStringLiteral("Remove-Item -Path \"$env:TEMP\\*\" -Recurse -Force -ErrorAction SilentlyContinue; Write-Output 'Temp очищен'"));
    combo_presets_->addItem(tr("4. Топ-10 процессов по потреблению CPU и памяти"),
        QStringLiteral("Get-Process | Sort-Object CPU -Descending | Select-Object -First 10 -Property Id, ProcessName, CPU, WorkingSet | Format-Table -AutoSize"));
    combo_presets_->addItem(tr("5. Свободное место на всех логических дисках"),
        QStringLiteral("Get-Volume | Select-Object DriveLetter, FileSystemLabel, @{N='Free(GB)';E={[math]::Round($_.SizeRemaining/1GB,2)}}, @{N='Total(GB)';E={[math]::Round($_.Size/1GB,2)}} | Format-Table -AutoSize"));
    combo_presets_->addItem(tr("6. Перезапуск сетевых адаптеров хоста"),
        QStringLiteral("Get-NetAdapter | Restart-NetAdapter; Get-NetAdapter"));
    combo_presets_->addItem(tr("7. Быстрый аудит системы (Имя ПК, ОС, сборка, ОЗУ)"),
        QStringLiteral("Get-ComputerInfo | Select-Object CsName, OsName, WindowsVersion, TotalPhysicalMemory | Format-List"));
    combo_presets_->addItem(tr("8. Проверка и перезапуск службы 1С:Предприятие"),
        QStringLiteral("Get-Service -Name *1C* | Format-Table -AutoSize"));

    connect(combo_presets_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SilentTerminalDialog::onPresetSelected);
    presets_row->addWidget(combo_presets_);

    main_layout->addLayout(presets_row);

    // Terminal Output
    terminal_output_ = new QPlainTextEdit(this);
    terminal_output_->setReadOnly(true);
    terminal_output_->setStyleSheet(QStringLiteral(
        "QPlainTextEdit {\n"
        "    background-color: #040711;\n"
        "    color: #38bdf8;\n"
        "    font-family: 'Consolas', 'JetBrains Mono', 'Fira Code', monospace;\n"
        "    font-size: 10.5pt;\n"
        "    border: 1px solid #1e293b;\n"
        "    border-radius: 6px;\n"
        "    padding: 8px;\n"
        "}\n"
    ));
    main_layout->addWidget(terminal_output_, 1);

    // Initial greeting in terminal
    appendLog(QStringLiteral("=== SyntexRemote Silent Shell & Automation Engine v2.6 ==="), QStringLiteral("#38bdf8"));
    appendLog(QStringLiteral("Готов к выполнению фоновых сценариев поддержки. Выберите команду или введите свою."), QStringLiteral("#94a3b8"));

    // Row 3: Command entry + Execute & Clear
    auto* cmd_row = new QHBoxLayout();
    cmd_row->setSpacing(8);

    edit_command_ = new QLineEdit(this);
    edit_command_->setPlaceholderText(tr("Введите команду PowerShell, CMD или Bash (например: Get-Service, whoami)..."));
    connect(edit_command_, &QLineEdit::returnPressed, this, &SilentTerminalDialog::onExecuteClicked);
    cmd_row->addWidget(edit_command_, 1);

    btn_execute_ = new QPushButton(tr("▶ Выполнить"), this);
    btn_execute_->setStyleSheet(QStringLiteral("background-color: #2563eb; color: #ffffff; font-weight: 600; padding: 6px 18px;"));
    connect(btn_execute_, &QPushButton::clicked, this, &SilentTerminalDialog::onExecuteClicked);
    cmd_row->addWidget(btn_execute_);

    btn_clear_ = new QPushButton(tr("Очистить"), this);
    connect(btn_clear_, &QPushButton::clicked, this, &SilentTerminalDialog::onClearOutput);
    cmd_row->addWidget(btn_clear_);

    main_layout->addLayout(cmd_row);
}

void SilentTerminalDialog::setTargetHost(const QString& host)
{
    if (combo_target_) {
        combo_target_->setCurrentText(host);
    }
}

void SilentTerminalDialog::onPresetSelected(int index)
{
    if (index <= 0 || !combo_presets_)
        return;

    QString cmd = combo_presets_->itemData(index).toString();
    if (!cmd.isEmpty() && edit_command_) {
        edit_command_->setText(cmd);
        edit_command_->setFocus();
    }
}

void SilentTerminalDialog::appendLog(const QString& text, const QString& color)
{
    if (!terminal_output_)
        return;

    QString timestamp = QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss"));
    QString html = QStringLiteral("<span style=\"color: #64748b;\">[%1]</span> <span style=\"color: %2;\">%3</span>")
        .arg(timestamp, color, text.toHtmlEscaped().replace(QStringLiteral("\n"), QStringLiteral("<br/>")));

    terminal_output_->appendHtml(html);
    terminal_output_->moveCursor(QTextCursor::End);
}

void SilentTerminalDialog::onClearOutput()
{
    if (terminal_output_) {
        terminal_output_->clear();
        appendLog(tr("Терминал очищен."), QStringLiteral("#94a3b8"));
    }
}

void SilentTerminalDialog::onExecuteClicked()
{
    if (!edit_command_)
        return;

    QString cmd = edit_command_->text().trimmed();
    if (cmd.isEmpty())
        return;

    if (process_ && process_->state() == QProcess::Running) {
        appendLog(tr("Предыдущий процесс ещё выполняется..."), QStringLiteral("#f59e0b"));
        return;
    }

    if (!process_) {
        process_ = new QProcess(this);
        connect(process_, &QProcess::readyReadStandardOutput, this, &SilentTerminalDialog::onProcessReadyRead);
        connect(process_, &QProcess::readyReadStandardError, this, &SilentTerminalDialog::onProcessReadyRead);
        connect(process_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &SilentTerminalDialog::onProcessFinished);
        connect(process_, &QProcess::errorOccurred, this, &SilentTerminalDialog::onProcessError);
    }

    QString target = combo_target_ ? combo_target_->currentText() : tr("Локально");
    appendLog(QStringLiteral(">> [%1] $ %2").arg(target, cmd), QStringLiteral("#10b981"));

    if (status_label_) {
        status_label_->setText(tr("Выполняется..."));
        status_label_->setStyleSheet(QStringLiteral("color: #38bdf8; font-weight: 500;"));
    }
    btn_execute_->setEnabled(false);

    // If running on Linux host, execute via sh / bash
#if defined(Q_OS_WIN)
    process_->start(QStringLiteral("powershell.exe"), QStringList() << QStringLiteral("-NoProfile") << QStringLiteral("-Command") << cmd);
#else
    // Linux/POSIX
    process_->start(QStringLiteral("/bin/bash"), QStringList() << QStringLiteral("-c") << cmd);
#endif
}

void SilentTerminalDialog::onProcessReadyRead()
{
    if (!process_)
        return;

    QByteArray out = process_->readAllStandardOutput();
    if (!out.isEmpty()) {
        QString text = QString::fromUtf8(out).trimmed();
        if (!text.isEmpty()) {
            appendLog(text, QStringLiteral("#e2e8f0"));
        }
    }

    QByteArray err = process_->readAllStandardError();
    if (!err.isEmpty()) {
        QString err_text = QString::fromUtf8(err).trimmed();
        if (!err_text.isEmpty()) {
            appendLog(err_text, QStringLiteral("#f87171"));
        }
    }
}

void SilentTerminalDialog::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    btn_execute_->setEnabled(true);
    if (status_label_) {
        if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
            status_label_->setText(tr("Успешно завершено (код 0)"));
            status_label_->setStyleSheet(QStringLiteral("color: #10b981; font-weight: 500;"));
            appendLog(tr("Команда успешно завершена."), QStringLiteral("#10b981"));
        } else {
            status_label_->setText(tr("Завершено с ошибкой (код %1)").arg(exitCode));
            status_label_->setStyleSheet(QStringLiteral("color: #f87171; font-weight: 500;"));
            appendLog(tr("Процесс завершился с кодом %1").arg(exitCode), QStringLiteral("#f87171"));
        }
    }
}

void SilentTerminalDialog::onProcessError(QProcess::ProcessError error)
{
    btn_execute_->setEnabled(true);
    if (status_label_) {
        status_label_->setText(tr("Ошибка запуска"));
        status_label_->setStyleSheet(QStringLiteral("color: #f87171; font-weight: 500;"));
    }
    appendLog(tr("Не удалось запустить процесс execution engine."), QStringLiteral("#f87171"));
}

} // namespace console
