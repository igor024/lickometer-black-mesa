#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

: "${ARDUINO_LIB_DIR:=$HOME/Arduino/libraries}"
: "${ARDUINO_DATA_DIR:=$HOME/.arduino15}"
: "${ARDUINO_BOARD:=uno}"
: "${ARDUINO_CPU:=atmega328p}"
: "${ARDUINO_F_CPU:=16000000L}"
: "${ARDUINO_VERSION:=10819}"
: "${ARDUINO_ARCH:=AVR}"

export ARDUINO_LIB_DIR
export ARDUINO_DATA_DIR
export ARDUINO_BOARD
export ARDUINO_CPU
export ARDUINO_F_CPU
export ARDUINO_VERSION
export ARDUINO_ARCH

python3 - "$script_dir" <<'PY'
import json
import os
import re
import shlex
import sys
from pathlib import Path

project = Path(sys.argv[1]).resolve()
arduino_lib_dir = Path(os.environ["ARDUINO_LIB_DIR"]).expanduser()
arduino_data_dir = Path(os.environ["ARDUINO_DATA_DIR"]).expanduser()
board = os.environ["ARDUINO_BOARD"]
cpu = os.environ["ARDUINO_CPU"]
f_cpu = os.environ["ARDUINO_F_CPU"]
arduino_version = os.environ["ARDUINO_VERSION"]
arduino_arch = os.environ["ARDUINO_ARCH"]
warning_prefix = "\033[33mwarning:\033[0m" if sys.stderr.isatty() else "warning:"

source_exts = {".c", ".cc", ".cpp", ".cxx", ".ino"}
header_exts = {".h", ".hh", ".hpp", ".hxx"}
project_exts = source_exts | header_exts
ignored_dirs = {
    ".git",
    ".build",
    ".cache",
    ".agents",
    ".codex",
    "cmake",
}
std_headers = {
    "algorithm", "array", "atomic", "bitset", "cassert", "cctype", "cerrno",
    "cfloat", "chrono", "cinttypes", "climits", "cmath", "cstddef", "cstdint",
    "cstdio", "cstdlib", "cstring", "ctime", "deque", "exception", "fstream",
    "functional", "initializer_list", "iomanip", "ios", "iosfwd", "iostream",
    "istream", "iterator", "limits", "list", "map", "memory", "new",
    "numeric", "ostream", "queue", "set", "sstream", "stdarg.h", "stdbool.h",
    "stddef.h", "stdint.h", "stdio.h", "stdlib.h", "string", "string.h",
    "ctype.h", "fcntl.h", "inttypes.h", "limits.h", "math.h", "type_traits",
    "utility", "vector",
}
ignored_missing_headers = {
    "Tick.h",
    "WProgram.h",
    "xc.h",
}
include_re = re.compile(r'^\s*#\s*include\s*([<"])([^>"]+)[>"]')


def latest_version_dir(base: Path) -> Path | None:
    if not base.is_dir():
        return None

    def key(path: Path):
        parts = []
        for part in re.split(r"([0-9]+)", path.name):
            parts.append(int(part) if part.isdigit() else part)
        return parts

    dirs = [p for p in base.iterdir() if p.is_dir()]
    return sorted(dirs, key=key)[-1] if dirs else None


avr_root = latest_version_dir(arduino_data_dir / "packages" / "arduino" / "hardware" / "avr")
tool_root = latest_version_dir(arduino_data_dir / "packages" / "arduino" / "tools" / "avr-gcc")
compiler = "avr-g++"
if tool_root:
    candidate = tool_root / "bin" / "avr-g++"
    if candidate.exists():
        compiler = str(candidate)

core_dir = avr_root / "cores" / "arduino" if avr_root else None
variant_dir = avr_root / "variants" / "standard" if avr_root else None


def library_include_dir(root: Path) -> Path:
    src = root / "src"
    return src if src.is_dir() else root


def library_name(root: Path) -> str:
    props = root / "library.properties"
    if props.exists():
        for line in props.read_text(errors="ignore").splitlines():
            if line.startswith("name="):
                return line.split("=", 1)[1].strip()
    return root.name


class HeaderIndex:
    def __init__(self):
        self.headers = {}
        self.file_to_include_dir = {}
        self.file_to_library = {}

    def add_root(self, root: Path, name: str):
        inc = library_include_dir(root)
        if not inc.is_dir():
            return
        for path in inc.rglob("*"):
            if path.is_file() and path.suffix.lower() in header_exts:
                rel = path.relative_to(inc).as_posix()
                for key in {rel, path.name}:
                    self.headers.setdefault(key, path)
                self.file_to_include_dir[path] = inc
                self.file_to_library[path] = name


lib_index = HeaderIndex()
for library_root in [arduino_lib_dir, arduino_data_dir / "libraries"]:
    if library_root.is_dir():
        for root in sorted(p for p in library_root.iterdir() if p.is_dir()):
            lib_index.add_root(root, library_name(root))

if avr_root:
    builtins = avr_root / "libraries"
    if builtins.is_dir():
        for root in sorted(p for p in builtins.iterdir() if p.is_dir()):
            lib_index.add_root(root, library_name(root))

core_headers = {}
for root in [core_dir, variant_dir]:
    if root and root.is_dir():
        for path in root.rglob("*"):
            if path.is_file() and path.suffix.lower() in header_exts:
                core_headers.setdefault(path.name, path)
                try:
                    core_headers.setdefault(path.relative_to(root).as_posix(), path)
                except ValueError:
                    pass

project_files = []
project_include_dirs = {project, project / "src"}
for path in project.rglob("*"):
    if not path.is_file() or path.suffix.lower() not in project_exts:
        continue
    rel_parts = path.relative_to(project).parts
    if any(part in ignored_dirs or part.startswith("arduino-") for part in rel_parts[:-1]):
        continue
    project_files.append(path)
    project_include_dirs.add(path.parent)

project_files = sorted(project_files)
project_headers = {}
for path in project_files:
    if path.suffix.lower() in header_exts:
        try:
            rel = path.relative_to(project).as_posix()
            project_headers[rel] = path
        except ValueError:
            pass
        project_headers.setdefault(path.name, path)

project_include_dirs = sorted(p for p in project_include_dirs if p.exists())


def read_includes(path: Path):
    try:
        text = path.read_text(errors="ignore")
    except OSError:
        return []
    includes = []
    for line in text.splitlines():
        match = include_re.match(line)
        if match:
            includes.append((match.group(1), match.group(2)))
    return includes


def resolve_local(header: str, including_file: Path) -> Path | None:
    direct = (including_file.parent / header).resolve()
    if direct.exists():
        return direct
    if including_file in lib_index.file_to_include_dir:
        resolved = resolve_angle(header)
        if resolved:
            return resolved
    for inc in project_include_dirs:
        candidate = (inc / header).resolve()
        if candidate.exists():
            return candidate
    return project_headers.get(header)


def resolve_angle(header: str) -> Path | None:
    if header in lib_index.headers:
        return lib_index.headers[header]
    if header in core_headers:
        return core_headers[header]
    return project_headers.get(header)


def include_closure(start: Path):
    needed_dirs = set()
    needed_libs = {}
    missing = set()
    seen = set()
    stack = [start]

    while stack:
        path = stack.pop()
        path = path.resolve()
        if path in seen:
            continue
        seen.add(path)

        for kind, header in read_includes(path):
            resolved = resolve_local(header, path) if kind == '"' else resolve_angle(header)
            if not resolved:
                if (
                    kind == "<"
                    and header not in std_headers
                    and header not in ignored_missing_headers
                    and not header.startswith(("avr/", "util/"))
                ):
                    missing.add(header)
                continue

            resolved = resolved.resolve()
            if resolved in lib_index.file_to_include_dir:
                inc_dir = lib_index.file_to_include_dir[resolved]
                needed_dirs.add(inc_dir)
                needed_libs[lib_index.file_to_library[resolved]] = inc_dir
            elif core_dir and core_dir in resolved.parents:
                needed_dirs.add(core_dir)
            elif variant_dir and variant_dir in resolved.parents:
                needed_dirs.add(variant_dir)

            if resolved.suffix.lower() in header_exts:
                stack.append(resolved)

    return needed_dirs, needed_libs, missing


base_flags = [
    f"-mmcu={cpu}",
    f"-DF_CPU={f_cpu}",
    f"-DARDUINO={arduino_version}",
    f"-DARDUINO_AVR_{board.upper()}",
    f"-DARDUINO_ARCH_{arduino_arch}",
    "-Os",
    "-ffunction-sections",
    "-fdata-sections",
    "-MMD",
    "-MP",
    "-flto",
    "-Wall",
    "-Wextra",
    "-std=gnu++11",
    "-fpermissive",
    "-fno-exceptions",
    "-fno-threadsafe-statics",
]

common_include_dirs = []
for path in [core_dir, variant_dir]:
    if path and path.is_dir():
        common_include_dirs.append(path)
common_include_dirs.extend(project_include_dirs)

commands = []
report = []
for path in project_files:
    needed_dirs, needed_libs, missing = include_closure(path)
    include_dirs = []
    seen_dirs = set()
    for inc in common_include_dirs + sorted(needed_dirs):
        inc = inc.resolve()
        if inc not in seen_dirs:
            seen_dirs.add(inc)
            include_dirs.append(inc)

    flags = list(base_flags)
    if path.suffix.lower() in header_exts:
        flags.extend(["-x", "c++-header"])
    elif path.suffix.lower() == ".ino":
        flags.extend(["-x", "c++"])
    flags.extend(f"-I{inc}" for inc in include_dirs)

    rel = path.relative_to(project).as_posix()
    obj = project / ".build" / "compile_commands" / f"{rel}.o"
    command_parts = [compiler, *flags, "-c", str(path), "-o", str(obj)]
    commands.append({
        "directory": str(project),
        "file": str(path),
        "command": " ".join(shlex.quote(part) for part in command_parts),
    })

    report.append((rel, needed_libs, missing))

(project / "compile_commands.json").write_text(json.dumps(commands, indent=2) + "\n")

print(f"wrote {project / 'compile_commands.json'}")
for rel, needed_libs, missing in report:
    if needed_libs:
        libs = ", ".join(f"{name} ({path})" for name, path in sorted(needed_libs.items()))
        print(f"{rel}: {libs}")
    if missing:
        print(f"{warning_prefix} {rel}: missing headers: {', '.join(sorted(missing))}", file=sys.stderr)
PY
