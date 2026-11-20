"""Compile the actual sketch with mocks. Does not compile for ESP32."""
from pathlib import Path
import shutil
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
compiler = shutil.which("g++")
if compiler is None:
    raise SystemExit("g++ is required; install a C++17 compiler to run host tests.")
with tempfile.TemporaryDirectory(prefix="green-corridor-test-") as temp:
    work = Path(temp)
    # Work on a disposable copy: no credentials/config.h written to the real repo.
    shutil.copytree(root / "firmware", work / "firmware")
    shutil.copytree(root / "tests", work / "tests")
    config = work / "firmware/green_corridor/config.h"
    shutil.copyfile(config.with_name("config.example.h"), config)
    binary = work / "controller-test"
    subprocess.run([compiler, "-std=c++17", "-Wall", "-Wextra",
                    "-Wno-sign-compare", "-Wno-unused-parameter",
                    "-I", str(work / "tests/stubs"),
                    str(work / "tests/controller_test.cpp"), "-o", str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
