arnab_counter = 0
jami_sum = 0
monir_limit = 5
goto Lcond1
Lbody0:
t0 = jami_sum + arnab_counter
jami_sum = t0
t1 = arnab_counter + 1
arnab_counter = t1
Lcond1:
t2 = arnab_counter < monir_limit
ifTrue t2 goto Lbody0
t3 = jami_sum > 5
muznabin_condition_1 = t3
t4 = jami_sum == 10
jami_relation_1 = t4
ifFalse muznabin_condition_1 goto Lelse2
arnab_inner_scope = 100
print arnab_inner_scope
print jami_sum
goto Lend3
Lelse2:
print 0
Lend3:
print arnab_counter
print jami_sum
