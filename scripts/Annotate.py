#!/usr/bin/env python3

import re
import sys
from collections import defaultdict

SHOW_FIRST_N = 3 # [flops]: extra variable

def read_log(path: str) -> dict[int, list[int]]:
    values = defaultdict(list)
    with open(path) as f:
        for line_no, raw in enumerate(f, 1):
            parts = raw.split()
            if len(parts) != 2:
                continue

            try:
                node_id = int(parts[0])
                value = int(parts[1])

            except ValueError:
                print(f"warning: bad line {line_no}: {raw!r}", file = sys.stderr)
                continue
            
            values[node_id].append(value)

    return values


def format_values(vals: list[int]) -> str:
    if not vals:
        return "values: (not executed)"

    shown = vals                    
    rest = len(vals) - len(shown) # FIXME[flops]: rest is always 0: rest = len(vals) - len(vals) = 0
    text = "values: " + ", ".join(str(v) for v in shown)
    if rest > 0:
        text += f" ... (+{rest} more)"
    
    return text


NODE_RE = re.compile(r'^(\s*n(\d+)\s*\[label=")([^"]*)(".*)$')

# FIXME[flops]: Use pygraphiz/networkx to parse dot files and generate images from it
def annotate_line(line: str, values: dict[int, list[int]]) -> str:
    m = NODE_RE.match(line)
    if not m:
        return line

    prefix, node_id_str, label_body, suffix = m.groups()
    node_id = int(node_id_str)

    addition = format_values(values.get(node_id, []))
    new_label = f"{label_body}\\n{addition}"
    return f"{prefix}{new_label}{suffix}\n" if not line.endswith("\n") else \
           f"{prefix}{new_label}{suffix}"


def main():
    values = read_log("runtime_log.txt")

    with open("dots/graph.dot") as f_in, open("dots/graph_annotated.dot", "w") as f_out:
        for line in f_in:
            f_out.write(annotate_line(line, values))

    print(f"Written dots/graph_annotated.dot ({sum(len(v) for v in values.values())} value records)")

if __name__ == "__main__":
    main()
