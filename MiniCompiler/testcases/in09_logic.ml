// 09: logical AND / OR / NOT
bool a;
bool b;
bool c;
a = true;
b = false;
c = a && !b;
if (c || b) {
    print(1);
} else {
    print(0);
}
