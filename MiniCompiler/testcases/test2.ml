// test2.ml - While loop and if-else control flow

int i;
int sum;

i = 0;
sum = 0;

while (i < 5) {
    sum = sum + i;
    i = i + 1;
}

if (sum > 10) {
    print(sum);
} else {
    print(0);
}
