default: test_en-morse

#Not so good test..
test_en-morse:
	echo Testing turning stuff into morse; \
	../en-morse readme.markdown > /tmp/en-morse_test_output \
	diff /tmp/en-morse_test_output morsed_readme > result/en-morse; \
	cd result; \
	echo $(USER)" `git status |wc -l` `git log |head -n 1` time `date +%s` `wc -l en-morse`" > /tmp/sdl_bad_utils_test_tmp;\
	cat run_list >> /tmp/en-morse_test_tmp;\
	mv /tmp/en-morse_test_tmp run_list; \
	head -n 1 run_list
