# 🚀 SyntexRemote (SyntexHelp RMM Edition)

> **Модернизированная, высокопроизводительная система удаленного мониторинга и управления для проекта SyntexHelp (RMM-PROECT).**  
> Построена на базе ядра Aspia с полным редизайном под дизайн-систему SyntexHelp и встроенными killer-фичами.

---

## 🎨 Дизайн и стилистика (SyntexHelp Design System)

SyntexRemote получил глубокий визуальный редизайн под общую эстетику платформы SyntexHelp:
- **Цветовая палитра Restrained Slate & Steel**: глубокие тона `#080e1e` / `#0f172a`, акценты `#38bdf8` (Cyan) и `#2563eb` (Blue), границы `#1e293b`.
- **Современная типографика**: гарнитура Manrope / Segoe UI / Inter с четкой иерархией.
- **Новая система иконок**: фирменная символика SyntexHelp (`rmm-server-icon.svg`, `syntexremote.ico`).
- **Сглаженные компоненты**: единый стиль для Console, Client, Host и диалоговых окон.

---

## ⚡ Киллер-фичи (Встроены прямо в ядро, не зависят от Junior Agent)

### 1. Скрытый удаленный терминал и AI SysAdmin Shell (`--terminal`, `--exec`)
- **Прямой фоновый терминальный канал** между оператором и удаленным хостом.
- Выполнение скриптов и команд (PowerShell / CMD на Windows, Bash на Linux) **в фоне без вывода окон на экране пользователя** и без захвата курсора мыши.
- Пресеты быстрого реагирования сисадмина в 1 клик:
  - *Рестарт службы печати (Spooler)*: `Restart-Service spooler -Force`
  - *Сброс кэша DNS*: `ipconfig /flushdns`
  - *Очистка временных файлов*: `Remove-Item $env:TEMP\* -Recurse -Force`
  - *Топ-10 процессов по потреблению CPU*: `Get-Process | Sort CPU -Desc | Select -First 10`
  - *Свободное место на всех дисках*: `Get-Volume`
  - *Перезапуск сетевых адаптеров*: `Get-NetAdapter | Restart-NetAdapter`
- Поддержка прямой автоматизации локальным AI-ассистентом:
  ```bash
  syntexremote_console --exec "Get-Service spooler"
  ```

### 2. SyntexHelp RMM Bridge & Direct Quick-Connect
- Интеграция с адресной книгой и списком компьютеров SyntexHelp (`computers_list.txt` / RMM JSON API).
- Мгновенный «живой» поиск и автодополнение как по **ID и IP**, так и по **имени сотрудника** (*«Алёна»*, *«Ирина (Бухгалтер)»*, *«Яна»*, *«SRV01»*).
- Автоматическая передача реквизитов подключения через CLI:
  ```bash
  syntexremote_client --address 47 --session-type desktop-manage \
    --username Admin --password "********" \
    --router-address "aspia.liveit24.ru" --router-port 8060
  ```

### 3. Режим скрытой поддержки в Host (Stealth Mode / Подавление плашки)
- Решение ключевого корпоративного запроса из базы знаний: **возможность скрытия всплывающего баннера подключения**.
- Настраивается через чекбокс в GUI Host, в `host.json` (`"HideConnectionNotification": true`), а также через CLI флаг `--silent-banner` / `--stealth`.
- При скрытой сессии пользователь не отвлекается на всплывающие плашки.

### 4. Live Telemetry HUD (Телеметрия в реальном времени)
- В панель удаленного рабочего стола встроен информационный бейдж:
  `⚡ SyntexHUD: 60 FPS | Задержка: 12 мс | Удаленный CPU: 14% | Память: 4.2/16 GB`
- Позволяет оператору мгновенно оценивать состояние хоста и сетевого канала без открытия диспетчера задач.

---

## 📦 Сборка и бинарные файлы

### Linux (x86_64)
- `dist/linux/syntexremote_console` — Консоль управления, адресная книга, скрытый терминал
- `dist/linux/syntexremote_client` — Клиент удаленного сеанса (Desktop, Files, SysInfo)
- `dist/linux/syntexremote_host` — Серверная хост-часть
- Алиасы совместимости: `aspia_console`, `aspia_client`, `aspia_host`

Сборка из исходников:
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Windows (x86_64)
- `dist/windows/syntexremote_console.exe` — Консоль управления с поддержкой Win32 GUI, RMM и Shell
- `dist/windows/syntexremote_client.exe` — Клиент подключения с CLI-параметрами
- `dist/windows/syntexremote_host.exe` — Служба хоста и GUI настройки
- `dist/windows/syntexremote_host_service.exe` — Системная служба Windows Service
- `dist/windows/syntexremote_desktop_agent.exe` — Агент рабочего стола
- Алиасы совместимости: `aspia_console.exe`, `aspia_client.exe`, `aspia_host.exe`

Сборка под Windows (кросс-компиляция через MinGW):
```bash
./source/win_dist/build_win.sh
```

---

## 🛠 Тестирование интерфейса

Тест работоспособности без фактического подключения:
```bash
# Тест справки и CLI парсера консоли
./dist/linux/syntexremote_console --help

# Тест справки клиента
./dist/linux/syntexremote_client --help

# Запуск Host в stealth-режиме
./dist/linux/syntexremote_host --silent-banner --help
```

---

## 📄 Лицензия

Распространяется под лицензией **GNU General Public License v3.0** (GPLv3).
С оригинальным кодом ядра Aspia © Dmitry Chapyshev.
Модификации, дизайн и функции RMM © 2026 SyntexHelp Team.
