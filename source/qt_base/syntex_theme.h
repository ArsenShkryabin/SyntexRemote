//
// SyntexRemote - Redesign for SyntexHelp RMM Project
// Copyright (C) 2026 SyntexHelp Team (based on Aspia)
//

#ifndef QT_BASE_SYNTEX_THEME_H
#define QT_BASE_SYNTEX_THEME_H

#include <QString>

class QApplication;

namespace qt_base {

class SyntexTheme {
public:
    static void apply(QApplication* app);
    static QString stylesheet();
};

} // namespace qt_base

#endif // QT_BASE_SYNTEX_THEME_H
