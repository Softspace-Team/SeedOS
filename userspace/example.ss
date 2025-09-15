echo "hello seed" | upper > out.txt

read out.txt | upper >> out.txt

echo "OUT.TXT = $(read out.txt | upper)"