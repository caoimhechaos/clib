#ifndef HAVE_T_TESTSUITE_H
#define HAVE_T_TESTSUITE_H 1

#define	testresult(text, code)	do { int res = code; exitval |= !res;	\
	if (res)							\
		fprintf(stderr, "Test %d: %s: OK\n", ++ntest, text);	\
	else								\
		fprintf(stderr, "Test %d: %s: FAIL (%s)\n", ++ntest,	\
			text, #code); } while(0)

#define	test_expect_int(type, code, value)	do { type res = code;	\
	if (res == value)						\
		fprintf(stderr, "Test %d: %s: %d == %d: OK\n", ++ntest,	\
			#code, res, value);				\
	else {								\
		fprintf(stderr, "Test %d: %s: FAIL (%d != %d)\n",	\
			++ntest, #code, res, value);			\
		exitval |= 1;						\
	} } while (0)

#define	test_expect_str(text, code, value)	do { char *res = code;	\
	if (res == NULL) {						\
		fprintf(stderr, "Test %d: %s: FAIL (%s -> NULL)\n",	\
			++ntest, text, #code);				\
		exitval |= 1;						\
		break;							\
	}								\
	if (!strcmp(res, value))					\
		fprintf(stderr, "Test %d: %s: OK\n", ++ntest, text);	\
	else {								\
		fprintf(stderr, "Test %d: %s: FAIL (\"%s\" != \"%s\")\n",\
			++ntest, text, res, value);			\
		exitval |= 1;						\
	} } while(0)

#endif /* HAVE_T_TESTSUITE_H */
