#!/usr/bin/env python3
"""Create isolated, buildable exercises from a working reference substrate."""
import argparse
import re
import shutil
from pathlib import Path

p = argparse.ArgumentParser()
p.add_argument("lab", choices=[f"{n:02}" for n in range(1, 10)])
a = p.parse_args()
root = Path(__file__).resolve().parents[1]
destination = root / "work" / f"lab{a.lab}"
if destination.exists():
    if not destination.is_dir():
        raise SystemExit(f"Destination is not a directory: {destination}")
    print(f"Already prepared; preserving every existing file: {destination}")
    raise SystemExit(0)
destination.mkdir(parents=True)
for name in ("Makefile", "linker.ld", ".gitignore", "README.md"):
    shutil.copy2(root / name, destination / name)
for name in ("boot", "bios", "include", "kernel", "scripts", "tools", "host", "labs", "docs"):
    shutil.copytree(root / name, destination / name, ignore=shutil.ignore_patterns("__pycache__", "previous-design"))

# Replace balanced function bodies, leaving all unrelated infrastructure intact.
def stub(file, function, body):
    text = file.read_text()
    match = re.search(r"\b" + re.escape(function) + r"\([^;{}]*\)\s*\{", text)
    if not match: raise ValueError(f"missing function {function}")
    start = match.end() - 1
    depth = 1; end = start + 1
    # Reference bodies contain no braces in strings or comments in selected functions.
    while depth:
        if text[end] == "{": depth += 1
        if text[end] == "}": depth -= 1
        end += 1
    file.write_text(text[:start] + "{\n    /* TODO: implement this lab; consult its task sheet. */\n    " + body + "\n}" + text[end:])

runtime = destination / "kernel/runtime.c"
mapping = {
    "03": [("sys_iam", "(void)address; return -38;"), ("sys_whoami", "(void)address; (void)capacity; return -38;")],
    "04": [("event", "(void)id; (void)state;")],
    "05": [("schedule", "(void)frame; return frame;")],
    "06": [("sem_wait", "(void)id; return -38;"), ("sem_post", "(void)id; return -38;")],
    "07": [("shared_attach", "(void)id; return -38;")],
    "09": [("proc_snapshot", "(void)buffer; return 0;")],
}
for function, body in mapping.get(a.lab, []): stub(runtime, function, body)
if a.lab == "08":
    console = destination / "kernel/console.c"
    stub(console, "console_filter_toggle", "/* Keep the toggle state disabled until implemented. */")
if a.lab == "03":
    # The teaching baseline's name buffer remains reserved for the learner implementation.
    text = runtime.read_text().replace("static char name[24];", "static char name[24] __attribute__((unused));")
    runtime.write_text(text)
(destination / "EXERCISE.md").write_text(
    f"# 实验 {a.lab} 独立工作副本\n\n"
    "这是从根目录参考基线生成的学习副本；相应实验函数已留为 TODO，其他基础设施保留；\n\n"
    f"先阅读 `labs/` 中编号 {a.lab} 的任务书，再修改源码；\n\n"
    f"在此目录执行 `make lab-test LAB={a.lab}` 验证；开始时实验 03～09 的目标测试应失败，"
    "`make test` 只检查启动，不代表实验完成；\n\n"
    "根目录保留参考实现；生成器拒绝覆盖已有副本，不会自动重置或删除学习进度；\n")
print(f"Prepared {destination}; your reference remains unchanged")
