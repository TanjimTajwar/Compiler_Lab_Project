int arnab_counter;
int jami_sum;
int monir_limit;

bool muznabin_condition_1;
bool jami_relation_1;

arnab_counter = 0;
jami_sum = 0;
monir_limit = 5;

/* While Loop + Arithmetic + Relational */
while (arnab_counter < monir_limit)
{
    jami_sum = jami_sum + arnab_counter;
    arnab_counter = arnab_counter + 1;
}

/* Relational Expression */
muznabin_condition_1 = jami_sum > 5;

/* Equality Check */
jami_relation_1 = jami_sum == 10;

/* If-Else + Block Scope */
if (muznabin_condition_1)
{
    int arnab_inner_scope;

    arnab_inner_scope = 100;

    print(arnab_inner_scope);
    print(jami_sum);
}
else
{
    print(0);
}

/* Final Prints */
print(arnab_counter);
print(jami_sum);