# このスクリプトはkの総和を集計する
# 使い方
# python count.py <実行ファイル> <入力ファイルパス>

# 例
# python .\analysis\count_all_k\count.py .\test\a.exe .\testcase\inputs\

import argparse
import os
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument("executable_flie", help="コンパイルされたmainとか、実行するファイル")
parser.add_argument("input_file", help="入力のファイルのパス")

args = parser.parse_args()

cnt = 0
for f in os.listdir(args.input_file):
    output = subprocess.run(f"{args.executable_flie} < {os.path.join(args.input_file, f)}"
                            , shell=True, stdout=subprocess.PIPE).stdout.decode().replace("\r", "")
    print(f, output)
    output_line = output.split("\n")

    if output_line[0] == "NO":
        continue

    cnt += int(output_line[1])

print("sum: ", cnt)
