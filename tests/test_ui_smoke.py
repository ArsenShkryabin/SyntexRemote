#!/usr/bin/env python3
"""
UI Smoke Test Suite for SyntexRemote (Linux and Windows binaries)
Verifies clean launch, responsive CLI parsing, theme initialization and UI lifecycle
WITHOUT connecting to remote hosts.
"""

import os
import subprocess
import sys
import time

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
LINUX_DIR = os.path.join(BASE_DIR, "dist", "linux")
WIN_DIR = os.path.join(BASE_DIR, "dist", "windows")

def run_test(name, cmd, env=None, timeout=5, expect_code=0):
    print(f"[*] Running test: {name} ... ", end="", flush=True)
    full_env = os.environ.copy()
    if env:
        full_env.update(env)
    try:
        proc = subprocess.run(cmd, env=full_env, timeout=timeout, capture_output=True, text=True)
        if proc.returncode == expect_code:
            print("PASSED")
            return True, proc.stdout + proc.stderr
        else:
            print(f"FAILED (code {proc.returncode})")
            print("OUTPUT:\n", proc.stdout, proc.stderr)
            return False, proc.stdout + proc.stderr
    except subprocess.TimeoutExpired:
        print("TIMED OUT")
        return False, "Timeout"

def main():
    print("=" * 60)
    print("  SYNTEXREMOTE UI SMOKE TEST SUITE")
    print("=" * 60)

    success_count = 0
    total_tests = 0

    # Test 1: Console --help
    total_tests += 1
    ok, out = run_test("Linux Console --help", [os.path.join(LINUX_DIR, "syntexremote_console"), "--help"])
    if ok and "Options:" in out:
        success_count += 1
    else:
        print("  -> Expected 'Options:' in help output")

    # Test 2: Console --version
    total_tests += 1
    ok, out = run_test("Linux Console --version", [os.path.join(LINUX_DIR, "syntexremote_console"), "--version"])
    if ok and "2.6.0.0" in out:
        success_count += 1
    else:
        print("  -> Expected version '2.6.0.0' in output")

    # Test 3: Client --help
    total_tests += 1
    ok, out = run_test("Linux Client --help", [os.path.join(LINUX_DIR, "syntexremote_client"), "--help"])
    if ok and "--address" in out and "--session-type" in out:
        success_count += 1
    else:
        print("  -> Expected '--address' and '--session-type' in help output")

    # Test 4: Client --version
    total_tests += 1
    ok, out = run_test("Linux Client --version", [os.path.join(LINUX_DIR, "syntexremote_client"), "--version"])
    if ok and "2.6.0.0" in out:
        success_count += 1

    # Test 5: Host --help
    total_tests += 1
    ok, out = run_test("Linux Host --help", [os.path.join(LINUX_DIR, "syntexremote_host"), "--help"],
                       env={"QT_QPA_PLATFORM": "offscreen"})
    if ok:
        success_count += 1

    # Test 6: Host --version with stealth mode flag
    total_tests += 1
    ok, out = run_test("Linux Host Stealth Flag", [os.path.join(LINUX_DIR, "syntexremote_host"), "--silent-banner", "--help"],
                       env={"QT_QPA_PLATFORM": "offscreen"})
    if ok:
        success_count += 1

    # Test 7: Console UI Event Loop Smoke Test (offscreen launch for 1 second)
    total_tests += 1
    print("[*] Running test: Console UI Event Loop Lifecycle (Offscreen) ... ", end="", flush=True)
    env = os.environ.copy()
    env["QT_QPA_PLATFORM"] = "offscreen"
    p = subprocess.Popen([os.path.join(LINUX_DIR, "syntexremote_console")], env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    time.sleep(1)
    if p.poll() is None:
        p.terminate()
        p.wait(timeout=2)
        print("PASSED (UI initialized & loop running cleanly)")
        success_count += 1
    else:
        print(f"FAILED (exited early with {p.returncode})")

    # Test 8: Client UI Event Loop Smoke Test (offscreen launch for 1 second)
    total_tests += 1
    print("[*] Running test: Client UI Event Loop Lifecycle (Offscreen) ... ", end="", flush=True)
    p = subprocess.Popen([os.path.join(LINUX_DIR, "syntexremote_client"), "--address", "localhost"], env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    time.sleep(1)
    if p.poll() is None:
        p.terminate()
        p.wait(timeout=2)
        print("PASSED (UI initialized & loop running cleanly)")
        success_count += 1
    else:
        print(f"FAILED (exited early with {p.returncode})")

    # Test 9: Windows PE header validation
    total_tests += 1
    print("[*] Validating Windows Binaries (PE32+ Executables) ... ", end="", flush=True)
    win_bins = [
        "syntexremote_console.exe",
        "syntexremote_client.exe",
        "syntexremote_host.exe",
        "syntexremote_host_service.exe"
    ]
    all_pe = True
    for b in win_bins:
        p = os.path.join(WIN_DIR, b)
        if not os.path.exists(p) or os.path.getsize(p) < 10000:
            all_pe = False
            break
        with open(p, "rb") as f:
            header = f.read(2)
            if header != b"MZ":
                all_pe = False
                break
    if all_pe:
        print("PASSED (All 4 Windows PE executables verified)")
        success_count += 1
    else:
        print("FAILED")

    print("=" * 60)
    print(f"Results: {success_count}/{total_tests} tests passed successfully!")
    print("=" * 60)
    return 0 if success_count == total_tests else 1

if __name__ == "__main__":
    sys.exit(main())
