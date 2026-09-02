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
import textwrap

args = [a for a in sys.argv[1:] if not a.startswith('--')]
flags = [a for a in sys.argv[1:] if a.startswith('--')]
default = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'playbook.md')
# under --checks the positional argument is a step, not a path
path = default if ('--checks' in flags or not args) else args[0]
text = open(path).read()

# --checks [step]: print the rule/check pairs, which is how you use this
# document while working rather than while editing it.
if '--checks' in flags:
    want = args[0] if args else None
    step, shown = None, 0
    para, in_check = [], False

    def flush():
        global shown
        if para:
            body = ' '.join(' '.join(para).split())
            for k, ln in enumerate(textwrap.wrap(body, 74)):
                print(f'  [{step}] {ln}' if k == 0 else f'         {ln}')
            shown += 1
            para.clear()

    for line in text.split('\n'):
        m = re.match(r'^#{2,3} (\S+?)\.? (.+)$', line)
        if m and not in_check:
            step = m.group(1).rstrip('.')
        if line.startswith('**Check:** '):
            flush()
            in_check = want is None or step == want
            if in_check:
                para.append(line[len('**Check:** '):])
        elif in_check:
            if line.strip():
                para.append(line)
            else:
                flush()
                in_check = False
    flush()
    print(f'\n{shown} check(s)' + (f' for step {want}' if want else ''))
    sys.exit(0)
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

# --- 3. every rule carries a check ----------------------------------------
# The rule is the claim and the prose is the evidence; without a third field
# the reader still has to work out what to *do*. Only 17 of 107 rules ended on
# an imperative before 2026-09-02, so the check was there but buried.
rule_starts = [m.start() for m in re.finditer(r'^\*\*`\[\w+\]` ', text, re.M)]
for i, s in enumerate(rule_starts):
    end = rule_starts[i + 1] if i + 1 < len(rule_starts) else len(text)
    chunk = text[s:end]
    h = re.search(r'^#{1,6} ', chunk, re.M)
    if h:
        chunk = chunk[:h.start()]
    if '**Check:**' not in chunk:
        head = re.match(r'\*\*`\[\w+\]` (.+?)\*\*', chunk, re.S)
        name = ' '.join(head.group(1).split())[:60] if head else '?'
        line = text[:s].count('\n') + 1
        report('check', f'line {line}: rule has no **Check:** -- {name}')

# --- 4. the hazard index covers every step that has rules ------------------
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

# --- 5. rules per step vs rows per step, so drift is visible ---------------
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
