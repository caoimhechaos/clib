#ifndef HAVE_T_TESTSUITE_H
#define HAVE_T_TESTSUITE_H 1

#define	testresult(text, code)	do { int res = code; exitval |= res;	\
	if (res)							\
		fprintf(stderr, "Test %d: %s: OK\n", ++ntest, text);	\
	else								\
		fprintf(stderr, "Test %d: %s: FAIL (%s)\n", ++ntest,	\
			text, #code); } while(0)

#endif /* HAVE_T_TESTSUITE_H */
