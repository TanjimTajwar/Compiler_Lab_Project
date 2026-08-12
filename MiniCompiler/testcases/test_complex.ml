int arnab_counter;
int jami_sum;
bool muznabin_final_condition;

arnab_counter = 0;
jami_sum = 0;

while (arnab_counter < 5)
{
    jami_sum = jami_sum + arnab_counter;
    arnab_counter = arnab_counter + 1;
}

muznabin_final_condition = jami_sum > 10;

if (muznabin_final_condition)
{
    print(jami_sum);
}
else
{
    print(0);
}