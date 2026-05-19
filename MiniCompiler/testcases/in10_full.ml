// 10: combined: while + if + arithmetic (sum 0..4 = 10)
int i;
int sum;
bool done;
i = 0;
sum = 0;
done = false;
while (!done) {
    sum = sum + i;
    i = i + 1;
    if (i == 5) {
        done = true;
    }
}
print(sum);
