#!/usr/bin/env bash
set -e

SRC_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DIST_DIR="$SRC_DIR/../../dist/windows"
mkdir -p "$DIST_DIR"

echo "=== Compiling Windows Resource File with MinGW windres ==="
x86_64-w64-mingw32-windres "$SRC_DIR/syntexremote.rc" -O coff -o "$DIST_DIR/syntexremote_res.o"

echo "=== Building syntexremote_console.exe ==="
x86_64-w64-mingw32-g++ -O3 -Wall -std=c++17 \
    "$SRC_DIR/syntexremote_console_win.cc" \
    "$DIST_DIR/syntexremote_res.o" \
    -o "$DIST_DIR/syntexremote_console.exe" \
    -mwindows -lcomctl32 -lshlwapi -lshell32 -lole32 -ladvapi32 -luser32

echo "=== Building syntexremote_client.exe ==="
x86_64-w64-mingw32-g++ -O3 -Wall -std=c++17 \
    "$SRC_DIR/syntexremote_client_win.cc" \
    "$DIST_DIR/syntexremote_res.o" \
    -o "$DIST_DIR/syntexremote_client.exe" \
    -mwindows -lcomctl32 -lshlwapi -lshell32 -lole32 -ladvapi32 -luser32

echo "=== Building syntexremote_host.exe ==="
x86_64-w64-mingw32-g++ -O3 -Wall -std=c++17 \
    "$SRC_DIR/syntexremote_host_win.cc" \
    "$DIST_DIR/syntexremote_res.o" \
    -o "$DIST_DIR/syntexremote_host.exe" \
    -mwindows -lcomctl32 -lshlwapi -lshell32 -lole32 -ladvapi32 -luser32

echo "=== Building syntexremote_host_service.exe ==="
x86_64-w64-mingw32-g++ -O3 -Wall -std=c++17 \
    "$SRC_DIR/syntexremote_host_service_win.cc" \
    "$DIST_DIR/syntexremote_res.o" \
    -o "$DIST_DIR/syntexremote_host_service.exe" \
    -ladvapi32 -luser32

# Copy desktop agent or companion
cp -f "$DIST_DIR/syntexremote_host.exe" "$DIST_DIR/syntexremote_desktop_agent.exe"

# Create Aspia backward compatibility symlinks / copies
cp -f "$DIST_DIR/syntexremote_console.exe" "$DIST_DIR/aspia_console.exe"
cp -f "$DIST_DIR/syntexremote_client.exe" "$DIST_DIR/aspia_client.exe"
cp -f "$DIST_DIR/syntexremote_host.exe" "$DIST_DIR/aspia_host.exe"
cp -f "$DIST_DIR/syntexremote_host_service.exe" "$DIST_DIR/aspia_host_service.exe"

rm -f "$DIST_DIR/syntexremote_res.o"

echo "=== Windows Build Successful! ==="
ls -lh "$DIST_DIR"
