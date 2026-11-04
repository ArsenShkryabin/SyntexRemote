//
// SyntexRemote - Redesign for SyntexHelp RMM Project
// Copyright (C) 2026 SyntexHelp Team (based on Aspia)
//

#include "qt_base/syntex_theme.h"

#include <QApplication>
#include <QPalette>
#include <QColor>
#include <QFont>
#include <QStyleFactory>

namespace qt_base {

QString SyntexTheme::stylesheet()
{
    return QStringLiteral(
        "/* SyntexHelp Modern Slate/Steel Design System */\n"
        "QWidget {\n"
        "    background-color: #0b1329;\n"
        "    color: #e2e8f0;\n"
        "    font-family: 'Segoe UI', 'Manrope', 'Inter', sans-serif;\n"
        "    font-size: 10pt;\n"
        "    selection-background-color: #2563eb;\n"
        "    selection-color: #ffffff;\n"
        "}\n"
        "QMainWindow, QDialog {\n"
        "    background-color: #080e1e;\n"
        "}\n"
        "QMenuBar {\n"
        "    background-color: #0f172a;\n"
        "    color: #cbd5e1;\n"
        "    border-bottom: 1px solid #1e293b;\n"
        "    padding: 2px 4px;\n"
        "}\n"
        "QMenuBar::item {\n"
        "    background: transparent;\n"
        "    padding: 6px 12px;\n"
        "    border-radius: 4px;\n"
        "}\n"
        "QMenuBar::item:selected {\n"
        "    background-color: #1e293b;\n"
        "    color: #38bdf8;\n"
        "}\n"
        "QMenu {\n"
        "    background-color: #0f172a;\n"
        "    color: #e2e8f0;\n"
        "    border: 1px solid #1e293b;\n"
        "    border-radius: 6px;\n"
        "    padding: 4px;\n"
        "}\n"
        "QMenu::item {\n"
        "    padding: 6px 24px 6px 14px;\n"
        "    border-radius: 4px;\n"
        "}\n"
        "QMenu::item:selected {\n"
        "    background-color: #2563eb;\n"
        "    color: #ffffff;\n"
        "}\n"
        "QMenu::separator {\n"
        "    height: 1px;\n"
        "    background-color: #1e293b;\n"
        "    margin: 4px 8px;\n"
        "}\n"
        "QToolBar {\n"
        "    background-color: #0f172a;\n"
        "    border-bottom: 1px solid #1e293b;\n"
        "    spacing: 6px;\n"
        "    padding: 4px 8px;\n"
        "}\n"
        "QToolButton {\n"
        "    background: transparent;\n"
        "    border: 1px solid transparent;\n"
        "    border-radius: 6px;\n"
        "    padding: 4px 8px;\n"
        "    color: #cbd5e1;\n"
        "}\n"
        "QToolButton:hover {\n"
        "    background-color: #1e293b;\n"
        "    border: 1px solid #334155;\n"
        "    color: #38bdf8;\n"
        "}\n"
        "QToolButton:pressed {\n"
        "    background-color: #2563eb;\n"
        "    color: #ffffff;\n"
        "}\n"
        "QPushButton {\n"
        "    background-color: #1e293b;\n"
        "    color: #f8fafc;\n"
        "    border: 1px solid #334155;\n"
        "    border-radius: 6px;\n"
        "    padding: 6px 14px;\n"
        "    font-weight: 500;\n"
        "}\n"
        "QPushButton:hover {\n"
        "    background-color: #334155;\n"
        "    border-color: #475569;\n"
        "    color: #38bdf8;\n"
        "}\n"
        "QPushButton:pressed {\n"
        "    background-color: #2563eb;\n"
        "    border-color: #3b82f6;\n"
        "    color: #ffffff;\n"
        "}\n"
        "QPushButton:disabled {\n"
        "    background-color: #0f172a;\n"
        "    color: #64748b;\n"
        "    border-color: #1e293b;\n"
        "}\n"
        "QLineEdit, QComboBox, QSpinBox, QTextEdit, QPlainTextEdit {\n"
        "    background-color: #0f172a;\n"
        "    color: #f8fafc;\n"
        "    border: 1px solid #334155;\n"
        "    border-radius: 6px;\n"
        "    padding: 5px 10px;\n"
        "}\n"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QTextEdit:focus, QPlainTextEdit:focus {\n"
        "    border: 1px solid #38bdf8;\n"
        "    background-color: #111e38;\n"
        "}\n"
        "QComboBox::drop-down {\n"
        "    subcontrol-origin: padding;\n"
        "    subcontrol-position: top right;\n"
        "    width: 20px;\n"
        "    border-left: none;\n"
        "}\n"
        "QComboBox QAbstractItemView {\n"
        "    background-color: #0f172a;\n"
        "    border: 1px solid #1e293b;\n"
        "    selection-background-color: #2563eb;\n"
        "    color: #f8fafc;\n"
        "}\n"
        "QTabWidget::pane {\n"
        "    border: 1px solid #1e293b;\n"
        "    border-radius: 6px;\n"
        "    background-color: #0f172a;\n"
        "    top: -1px;\n"
        "}\n"
        "QTabBar::tab {\n"
        "    background-color: #080e1e;\n"
        "    color: #94a3b8;\n"
        "    border: 1px solid #1e293b;\n"
        "    border-bottom: none;\n"
        "    border-top-left-radius: 6px;\n"
        "    border-top-right-radius: 6px;\n"
        "    padding: 8px 16px;\n"
        "    margin-right: 2px;\n"
        "}\n"
        "QTabBar::tab:selected {\n"
        "    background-color: #0f172a;\n"
        "    color: #38bdf8;\n"
        "    border-bottom: 2px solid #38bdf8;\n"
        "    font-weight: 600;\n"
        "}\n"
        "QTabBar::tab:hover:!selected {\n"
        "    background-color: #1e293b;\n"
        "    color: #e2e8f0;\n"
        "}\n"
        "QTreeView, QListView, QTableView {\n"
        "    background-color: #090e1f;\n"
        "    alternate-background-color: #0f172a;\n"
        "    border: 1px solid #1e293b;\n"
        "    border-radius: 6px;\n"
        "    color: #e2e8f0;\n"
        "    gridline-color: #1e293b;\n"
        "}\n"
        "QTreeView::item:hover, QListView::item:hover {\n"
        "    background-color: #1e293b;\n"
        "    color: #ffffff;\n"
        "}\n"
        "QTreeView::item:selected, QListView::item:selected {\n"
        "    background-color: #2563eb;\n"
        "    color: #ffffff;\n"
        "}\n"
        "QHeaderView::section {\n"
        "    background-color: #0f172a;\n"
        "    color: #94a3b8;\n"
        "    padding: 6px;\n"
        "    border: 1px solid #1e293b;\n"
        "    font-weight: 600;\n"
        "}\n"
        "QStatusBar {\n"
        "    background-color: #080d1a;\n"
        "    color: #94a3b8;\n"
        "    border-top: 1px solid #1e293b;\n"
        "}\n"
        "QScrollBar:vertical {\n"
        "    background-color: #090e1f;\n"
        "    width: 10px;\n"
        "    margin: 0;\n"
        "    border-radius: 5px;\n"
        "}\n"
        "QScrollBar::handle:vertical {\n"
        "    background-color: #334155;\n"
        "    min-height: 20px;\n"
        "    border-radius: 5px;\n"
        "}\n"
        "QScrollBar::handle:vertical:hover {\n"
        "    background-color: #475569;\n"
        "}\n"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
        "    height: 0px;\n"
        "}\n"
        "QGroupBox {\n"
        "    border: 1px solid #1e293b;\n"
        "    border-radius: 6px;\n"
        "    margin-top: 12px;\n"
        "    padding-top: 12px;\n"
        "    font-weight: 600;\n"
        "}\n"
        "QGroupBox::title {\n"
        "    subcontrol-origin: margin;\n"
        "    subcontrol-position: top left;\n"
        "    left: 10px;\n"
        "    padding: 0 4px;\n"
        "    color: #38bdf8;\n"
        "}\n"
        "QProgressBar {\n"
        "    border: 1px solid #1e293b;\n"
        "    border-radius: 4px;\n"
        "    text-align: center;\n"
        "    background-color: #0f172a;\n"
        "    color: #f8fafc;\n"
        "}\n"
        "QProgressBar::chunk {\n"
        "    background-color: #2563eb;\n"
        "    border-radius: 3px;\n"
        "}\n"
    );
}

void SyntexTheme::apply(QApplication* app)
{
    if (!app)
        return;

    // Use Fusion style as base for clean cross-platform widget layout
    if (QStyleFactory::keys().contains(QStringLiteral("Fusion")))
        app->setStyle(QStyleFactory::create(QStringLiteral("Fusion")));

    // Modern Slate Palette
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0x08, 0x0e, 0x1e));
    palette.setColor(QPalette::WindowText, QColor(0xe2, 0xe8, 0xf0));
    palette.setColor(QPalette::Base, QColor(0x0f, 0x17, 0x2a));
    palette.setColor(QPalette::AlternateBase, QColor(0x13, 0x1d, 0x33));
    palette.setColor(QPalette::ToolTipBase, QColor(0x1e, 0x29, 0x3b));
    palette.setColor(QPalette::ToolTipText, QColor(0xf8, 0xfa, 0xfc));
    palette.setColor(QPalette::Text, QColor(0xe2, 0xe8, 0xf0));
    palette.setColor(QPalette::Button, QColor(0x1e, 0x29, 0x3b));
    palette.setColor(QPalette::ButtonText, QColor(0xf8, 0xfa, 0xfc));
    palette.setColor(QPalette::BrightText, QColor(0x38, 0xbd, 0xf8));
    palette.setColor(QPalette::Link, QColor(0x38, 0xbd, 0xf8));
    palette.setColor(QPalette::Highlight, QColor(0x25, 0x63, 0xeb));
    palette.setColor(QPalette::HighlightedText, QColor(0xff, 0xff, 0xff));

    // Disabled states
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(0x64, 0x74, 0x8b));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(0x64, 0x74, 0x8b));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(0x64, 0x74, 0x8b));

    app->setPalette(palette);
    app->setStyleSheet(stylesheet());
}

} // namespace qt_base
