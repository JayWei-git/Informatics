#!/usr/bin/env python3
"""Integrate N/J/R/W/E event intervals; require complete, ordered lifetimes."""
import argparse
import csv
import sys
from pathlib import Path

p = argparse.ArgumentParser()
p.add_argument("log", type=Path)
p.add_argument("--csv", type=Path)
a = p.parse_args()
processes = {}
last_tick = -1
try:
    for line in a.log.read_text().splitlines():
        start = line.find("TRACE\t")
        if start < 0: continue
        _, pid, state, tick = line[start:].split()
        pid, tick = int(pid), int(tick)
        if tick < last_tick: raise ValueError("global ticks moved backwards")
        last_tick = tick
        if state == "N":
            if pid in processes: raise ValueError("duplicate birth")
            processes[pid] = {"pid": pid, "birth": tick, "last": tick, "state": "N", "waiting": 0, "running": 0, "blocked": 0}
            continue
        row = processes[pid]
        old, previous = row["state"], row["last"]
        transitions = {"N": "J", "J": "R", "R": "JWE", "W": "J"}
        if state not in transitions.get(old, ""): raise ValueError(f"invalid transition {old}->{state} for {pid}")
        key = {"J": "waiting", "R": "running", "W": "blocked"}.get(old)
        if key: row[key] += tick - previous
        row["state"], row["last"] = state, tick
        if state == "E":
            row["exit"] = tick
            row["turnaround"] = tick - row["birth"]
    if not processes or any(x["state"] != "E" for x in processes.values()):
        raise ValueError("missing or unfinished process lifetimes")
except (OSError, ValueError, KeyError) as e:
    sys.exit(f"FAIL: {e}")
columns = ["pid", "birth", "exit", "waiting", "running", "blocked", "turnaround"]
rows = [processes[k] for k in sorted(processes)]
for row in rows:
    if row["waiting"] + row["running"] + row["blocked"] != row["turnaround"]:
        sys.exit("FAIL: lifetime accounting mismatch")
print("\t".join(columns))
for row in rows: print("\t".join(str(row[k]) for k in columns))
span = max(r["exit"] for r in rows) - min(r["birth"] for r in rows)
print(f"average_wait_ticks={sum(r['waiting'] for r in rows)/len(rows):.3f}")
print(f"average_turnaround_ticks={sum(r['turnaround'] for r in rows)/len(rows):.3f}")
print(f"throughput_per_second={len(rows)*100/span:.3f}" if span else "throughput_per_second=undefined")
if a.csv:
    with a.csv.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=columns, extrasaction="ignore")
        writer.writeheader(); writer.writerows(rows)
