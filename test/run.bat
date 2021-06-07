del output.txt
for %%f in (./../testcase/inputs/*) do (
  echo %%f >> output.txt
  call a.exe < ./../testcase/inputs/%%f >> output.txt
)