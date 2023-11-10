# MAC(LEAKS): // NE NADEZHNO :)
# -----------
# leakmacro () {
#   leaks -quiet -atExit -- $1 $2 $3 $4 $5 $6 $7 $8 $9 > leak.log
#   grep -e Process leak.log
# }

# LINUX(VALGRIND):
# ----------------
leakmacro () {
  valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -q $1 $2 $3 $4 $5 $6 $7 $8 $9 > leak.log
  grep -i errors leak.log 
}

filename=Makefile;
echo ----------------------------------------------*CAT LEAK CHECK 1
leakmacro ./s21_cat -b "$filename"
echo ----------------------------------------------*CAT LEAK CHECK 2
leakmacro ./s21_cat -e "$filename"
echo ----------------------------------------------*CAT LEAK CHECK 3
leakmacro ./s21_cat -n "$filename"
# echo ----------------------------------------------*CAT 4
# leakmacro ./s21_cat "$filename" -n
echo ----------------------------------------------*CAT LEAK CHECK 5
leakmacro ./s21_cat -s "$filename"
echo ----------------------------------------------*CAT LEAK CHECK 6
leakmacro ./s21_cat -t "$filename"
echo ----------------------------------------------*CAT LEAK CHECK 7
leakmacro ./s21_cat "$filename"
echo ----------------------------------------------*Leak tests ended*