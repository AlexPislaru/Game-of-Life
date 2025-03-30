build:
   gcc task1.c task1in.c task1.h -o a.out
run:
   ./checker-linux-amd64 -i
clean:
rm -rf a.out