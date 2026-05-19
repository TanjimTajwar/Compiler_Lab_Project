// 07: inner block with shadowing name
int x;
x = 1;
{
    int x;
    x = 2;
    print(x);
}
print(x);
