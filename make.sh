SOURCE="pswdfind.c"
EXEC="pf"
gcc $SOURCE -o $EXEC -g -lm -fopenmp -O3
time ./$EXEC
