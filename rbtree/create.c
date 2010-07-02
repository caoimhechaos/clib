/*-
 * Copyright (c) 2010 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Caoimhe Chaos <caoimhechaos@protonmail.com>.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <unistd.h>

#include "clib_internal.h"

/**
 * Create an rbtree and use free() as a destructor. This is generally
 * desirable for things like string -> string hashes.
 */
struct c_rbtree *
c_rbtree_new(c_equalfunc compare)
{
	/* Create an rbtree with free() as destructor. */
	return c_rbtree_new_complex(compare, free, free);
}

/**
 * Create an rbtree.
 */
struct c_rbtree *
c_rbtree_new_complex(c_equalfunc compare, c_destructor key_destructor,
	c_destructor value_destructor)
{
	struct c_rbtree *t = malloc(sizeof(struct c_rbtree));

	if (!t) return NULL;

	memset(t, 0, sizeof(struct c_rbtree));

	t->rb_compare = compare;
	t->rb_keydestr = key_destructor;
	t->rb_valdestr = value_destructor;

	t->rb_head = NULL;

	return t;
}

/**
 * Destroy the rbtree again.
 */
void
c_rbtree_destroy(struct c_rbtree *t)
{
	/**
	 * FIXME: Not sure this is enough, should be checked with a memory
	 * leak tester.
	 */
	c_rbtree_remove_all(t);
	free(t);
}
