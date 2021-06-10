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

output = subprocess.run(f"{args.executable_flie} < {args.input_file}", shell=True, stdout=subprocess.PIPE).stdout.decode()
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
    obstacle_map = [[False] * n for _ in range(n)]
    for _ in range(n):
        y, x = map(int, next(lines).split(" "))
        obstacles.append((y - 1, x - 1))
        obstacle_map[y-1][x-1] = True
    
    m = int(next(lines))
    ops = []
    for _ in range(m):
        s = next(lines)
        ops.append(s)

def rachable_map(use):
    visited = [[0] * n for _ in range(n)]
    visited[0][0] = 1
    que = [(0, 0)]

    for y, x in que:
        for op_ids in use:
            y2, x2 = y, x
            flag = True
            for i in ops[op_ids]:
                if i == "U":
                    y2 -= 1
                elif i == "D":
                    y2 += 1
                elif i == "L":
                    x2 -= 1
                elif i == "R":
                    x2 += 1
                
                if not (0 <= y2 < n and 0 <= x2 < n and (not obstacle_map[y2][x2])):
                    flag = False
                    break
            
            if flag and not visited[y2][x2]:
                visited[y2][x2] = True
                que.append((y2, x2))

    return visited


root = tkinter.Tk()

canvas = tkinter.Canvas(root, width=512, height=512)
canvas.pack()

scale = tkinter.Scale(
    root, 
)

root.mainloop()
