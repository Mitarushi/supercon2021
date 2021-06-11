# このスクリプトはコードが排他出力を可視化することができる
# 使い方
# python visualizer.py <実行ファイル> <入力ファイル>

# 例
# python .\analysis\progress_visualizer\visualizer.py .\test\a.exe .\testcase\inputs\qual_random_01.in

import argparse
import subprocess
import tkinter

parser = argparse.ArgumentParser()
parser.add_argument("executable_flie", help="コンパイルされたmainとか、実行するファイル")
parser.add_argument("input_file", help="入力のファイル")

args = parser.parse_args()

output = subprocess.run(f"{args.executable_flie} < {args.input_file}", shell=True,
                        stdout=subprocess.PIPE).stdout.decode().replace("\r", "")
print(output)
output_line = output.split("\n")
if output_line[0] == "NO":
    exit()

k = int(output_line[1])
use_ops = [int(i) - 1 for i in output_line[2].split(" ")]

with open(args.input_file) as f:
    lines = iter(f.read().split("\n"))

    n = int(next(lines))
    obstacles = []
    for _ in range(n):
        y, x = map(int, next(lines).split(" "))
        obstacles.append((y - 1, x - 1))

    m = int(next(lines))
    ops = []
    for _ in range(m):
        s = next(lines)
        ops.append(s)

edge_map = [[[True] * n for _ in range(n)] for _ in range(k)]
for y, x in obstacles:
    for i in range(k):
        edge_map[i][y][x] = False

op_shift = []
for idx, i in enumerate(use_ops):
    op = ops[i]

    shift_y, shift_x = 0, 0
    min_y, min_x, max_y, max_x = 0, 0, 0, 0
    for j in op:
        if j == "U":
            shift_y -= 1
        elif j == "D":
            shift_y += 1
        elif j == "L":
            shift_x -= 1
        elif j == "R":
            shift_x += 1

        min_y, _, max_y = sorted([min_y, shift_y, max_y])
        min_x, _, max_x = sorted([min_x, shift_x, max_x])

        for y, x in obstacles:
            py, px = y - shift_y, x - shift_x
            if 0 <= py < n and 0 <= px < n:
                edge_map[idx][py][px] = False

    op_shift.append((shift_y, shift_x))
    for y in range(n):
        for x in range(n):
            if not (-min_y <= y < n - max_y and -min_x <= x < n - max_x):
                edge_map[idx][y][x] = False


def get_reachable_map(use):
    visited = [[False] * n for _ in range(n)]
    visited[0][0] = True
    que = [(0, 0)]

    for y, x in que:
        for op_idx in use:
            if edge_map[op_idx][y][x]:
                dy, dx = op_shift[op_idx]
                py, px = y + dy, x + dx
                if not visited[py][px]:
                    visited[py][px] = True
                    que.append((py, px))

    return visited


def plot(canvas, use):
    canvas.delete("all")
    grid_xy = [512 * i // n for i in range(n)] + [512]
    reachable_map = get_reachable_map(use)

    for y in range(n):
        for x in range(n):
            if reachable_map[y][x]:
                canvas.create_rectangle(grid_xy[x], grid_xy[y],
                                        grid_xy[x + 1], grid_xy[y + 1],
                                        fill="green", outline="")

    for y, x in obstacles:
        canvas.create_rectangle(grid_xy[x], grid_xy[y],
                                grid_xy[x + 1], grid_xy[y + 1],
                                fill="red", outline="")


def update(canvas):
    def func(_):
        plot(canvas, range(slider.get()))

    return func


root = tkinter.Tk()
root.geometry("512x576")
slider = tkinter.IntVar()
canvas = tkinter.Canvas(root, width=512, height=512)
canvas.place(x=0, y=64)
plot(canvas, range(k))

scale = tkinter.Scale(
    root,
    orient="h",
    from_=0,
    to_=len(use_ops),
    length=512 - 128,
    variable=slider,
    command=update(canvas)
)
scale.place(x=64, y=0)

root.mainloop()
