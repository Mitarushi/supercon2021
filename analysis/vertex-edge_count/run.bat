del output.txt
for %%f in (../../testcase/inputs/*) do (
  call a.exe < ../../testcase/inputs/%%f >> output.txt
)