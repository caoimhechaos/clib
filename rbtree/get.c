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

#include "clib_internal.h"

static int
c_rbtree_foreach_sub_dfs(struct c_rbtree_entry *node, c_htcallback cb,
	const void *userdata);
static int
c_rbtree_foreach_sub_bfs(struct c_rbtree_entry *node, c_htcallback cb,
	const void *userdata);

/**
 * Retrieve the first record with the specified key. Otherwise, return NULL.
 */
void *
c_rbtree_lookup(struct c_rbtree *t, const void *key)
{
	struct c_rbtree_entry *node;
	int eq;

	if (!t)
		return NULL;

	node = t->rb_head;

	while (node)
	{
		eq = t->rb_compare(node->rb_key, key);

		if (eq == 0)
			return node->rb_value;
		else if (eq < 0)
			node = node->rb_left;
		else
			node = node->rb_right;
	}

	/* Nothing found, too bad. */
	return NULL;
}

/**
 * Iterate over all rbtree entries matching the specified key and call
 * the callback with the given data.
 */
int
c_rbtree_lookup_cb(struct c_rbtree *t, const void *key, c_htcallback cb,
	const void *userdata)
{
	struct c_rbtree_entry *node;
	int eq;

	if (!t)
		return 0;

	node = t->rb_head;

	while (node)
	{
		eq = t->rb_compare(node->rb_key, key);

		if (eq == 0 && cb(node->rb_value, userdata))
			return 1;
		else if (eq < 0)
			node = node->rb_left;
		else
			node = node->rb_right;
	}

	return 1;
}

/**
 * Call the given callback for each member in the entire rbtree in
 * depth-first-search order.
 */
int
c_rbtree_foreach_dfs(struct c_rbtree *t, c_htcallback cb,
	const void *userdata)
{
	if (!t)
		return 0;

	return c_rbtree_foreach_sub_dfs(t->rb_head, cb, userdata);
}

/**
 * Iterate through the trees. There's surely a non-recursive way to do
 * it but it is recursive for now.
 */
static int
c_rbtree_foreach_sub_dfs(struct c_rbtree_entry *node, c_htcallback cb,
	const void *userdata)
{
	int ret = 0;

	if (!node)
		return 0;

	if (node->rb_left)
		ret |= c_rbtree_foreach_sub_dfs(node->rb_left, cb, userdata);

	if (!ret)
		ret |= cb(node->rb_value, userdata);

	if (!ret && node->rb_right)
		ret |= c_rbtree_foreach_sub_dfs(node->rb_right, cb, userdata);

	return ret;
}

/**
 * Call the given callback for each member in the entire rbtree in
 * breadth-first-search order.
 */
int
c_rbtree_foreach_bfs(struct c_rbtree *t, c_htcallback cb,
	const void *userdata)
{
	struct c_rbtree_entry *node;

	if (!t)
		return 0;

	node = t->rb_head;

	if (!node)
		return 0;

	if (cb(node->rb_value, userdata))
		return 1;

	return c_rbtree_foreach_sub_bfs(t->rb_head, cb, userdata);
}

/**
 * Iterate through the trees. There's surely a non-recursive way to do
 * it but it is recursive for now.
 */
static int
c_rbtree_foreach_sub_bfs(struct c_rbtree_entry *node, c_htcallback cb,
	const void *userdata)
{
	int ret = 0;

	if (!node)
		return 0;

	if (node->rb_left)
		ret |= cb(node->rb_left->rb_value, userdata);

	if (!ret && node->rb_right)
		ret |= cb(node->rb_right->rb_value, userdata);

	if (!ret && node->rb_left)
		ret |= c_rbtree_foreach_sub_bfs(node->rb_left, cb, userdata);

	if (!ret && node->rb_right)
		ret |= c_rbtree_foreach_sub_bfs(node->rb_right, cb, userdata);

	return ret;
}
