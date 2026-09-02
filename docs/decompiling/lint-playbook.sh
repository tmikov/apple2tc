#!/usr/bin/env python3
"""Check the playbook against itself.

The document makes three claims about its own structure, and all three had
drifted by 2026-09-01: a tag was in use that the vocabulary did not declare,
the hazard index was missing rows for rules added two days earlier, and a
recipe in a code block did not run. The first two are checkable; this checks
them, so the next one is caught by a script rather than by a reviewer.

  lint-playbook.sh [path]     default: docs/decompiling/playbook.md

Exit 0 clean, 1 with findings.
"""
import os
import re
import sys

path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(
    os.path.dirname(os.path.abspath(__file__)), 'playbook.md')
text = open(path).read()
lines = text.split('\n')
findings = []


def report(kind, msg):
    findings.append(f'{kind}: {msg}')


# --- 1. every tag in use is declared ---------------------------------------
declared = set(re.findall(r'^\| `\[(\w+)\]` \| ', text, re.M))
# A scope tag opens a bolded rule, or labels a whole section. Anything else in
# brackets is a check name ([warn], [exports], [yield]) and not a tag at all.
used = set(re.findall(r'^\*\*`\[(\w+)\]` ', text, re.M))
used |= set(re.findall(r'^\*`\[(\w+)\]`\. ', text, re.M))
for tag in sorted(used - declared):
    report('tag', f'`[{tag}]` is used but not in the scope-tag table')
for tag in sorted(declared - used):
    report('tag', f'`[{tag}]` is declared but never used')

# --- 2. every internal link resolves ---------------------------------------
def slug(heading):
    s = heading.strip().lower()
    s = re.sub(r'`|\*\*|\*|\.|,|;|:|\'|"|\(|\)|/|\\|\+', '', s)
    s = re.sub(r'[^\w\s-]', '', s)
    return re.sub(r'\s+', '-', s).strip('-')


anchors = {slug(m.group(2)) for m in re.finditer(r'^(#{1,6}) (.+)$', text, re.M)}
for m in re.finditer(r'\]\(#([^)]+)\)', text):
    if m.group(1) not in anchors:
        line = text[:m.start()].count('\n') + 1
        report('link', f'line {line}: #{m.group(1)} matches no heading')

# --- 3. the hazard index covers every step that has rules ------------------
try:
    index_at = next(i for i, l in enumerate(lines) if l.startswith('# Hazard index'))
except StopIteration:
    report('index', 'no "# Hazard index" section')
    index_at = len(lines)

body, index = '\n'.join(lines[:index_at]), '\n'.join(lines[index_at:])

# steps are "## N. Title" or "### N.M Title"
body_steps = set(re.findall(r'^#{2,3} (\d+(?:\.\d+)?|\d+[a-z])[\. ]', body, re.M))
row_steps = set()
for row in re.findall(r'^\| ([^|]+) \|', index, re.M):
    s = row.strip()
    if s and s not in ('Step', '---'):
        row_steps.add(s)
# 'T' files a hazard that belongs to the tooling rather than to a step.
for s in sorted(row_steps - body_steps - {'—', '-', 'T'}):
    report('index', f'a row is filed under step {s}, which is not a section')

# --- 4. rules per step vs rows per step, so drift is visible ---------------
# Not an error: not every rule is a hazard. But a step whose rule count has
# grown well past its row count is where the index rots, and it rots silently.
def step_of(pos):
    seen = None
    for m in re.finditer(r'^#{2,3} (\d+(?:\.\d+)?|\d+[a-z])[\. ]', body[:pos], re.M):
        seen = m.group(1)
    return seen


rules = {}
for m in re.finditer(r'^\*\*`\[\w+\]` ', body, re.M):
    s = step_of(m.start())
    if s:
        rules[s] = rules.get(s, 0) + 1
rows = {}
for row in re.findall(r'^\| ([^|]+) \|', index, re.M):
    s = row.strip()
    if s in body_steps:
        rows[s] = rows.get(s, 0) + 1

gaps = [(s, rules[s], rows.get(s, 0)) for s in sorted(rules)
        if rules[s] > rows.get(s, 0) + 2]
if gaps:
    print('note: steps where tagged rules outnumber index rows by more than 2.')
    print('      Not an error -- not every rule is a hazard -- but this is where')
    print('      the index rots, and it rots silently.')
    for s, nr, nw in gaps:
        print(f'      step {s}: {nr} rules, {nw} rows')

for f in findings:
    print(f)
print(f'{"FAIL" if findings else "PASS"} [playbook]: {len(findings)} finding(s) '
      f'over {len(anchors)} headings, {len(used)} tags in use')
sys.exit(1 if findings else 0)
