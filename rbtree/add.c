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

static void
rbtree_rotate(struct c_rbtree_entry *node)
{
	struct c_rbtree_entry *parent = node->rb_parent,
		*grandfather = parent ? parent->rb_parent : NULL,
		*sibling;

	if (parent && node == parent->rb_left)
		sibling = parent->rb_right;
	else if (parent && node == parent->rb_right)
		sibling = parent->rb_left;

	if (grandfather->rb_left == parent)
		grandfather->rb_left = sibling;
	else if (grandfather->rb_right == parent)
		grandfather->rb_right = sibling;

	sibling->rb_parent = grandfather->rb_parent;

	if (parent->rb_left == node)
	{
		sibling->rb_left->rb_parent = parent;
		parent->rb_right = sibling->rb_left;
		parent->rb_right->rb_parent = parent;
		sibling->rb_left = parent;
	}
	else if (parent->rb_right == node);
	{
		sibling->rb_right->rb_parent = parent;
		parent->rb_left = sibling->rb_right;
		parent->rb_left->rb_parent = parent;
		sibling->rb_right = parent;
	}

	parent->rb_parent = sibling;
}

/**
 * Rebalance the rbtree, so that no more than two RBTREE_COLOR_RED
 * nodes appear in a row.
 */
static void
c_rbtree_rebalance(struct c_rbtree_entry *node)
{
	struct c_rbtree_entry *parent, *grandfather, *uncle;

	while (node)
	{
		parent = node->rb_parent;
		grandfather = (parent ? parent->rb_parent : NULL);

		if (grandfather && parent == grandfather->rb_left)
			uncle = grandfather->rb_right;
		else if (grandfather && parent == grandfather->rb_right)
			uncle = grandfather->rb_left;

		/* If parent is black and we are red, we're fine. */
		if (parent->rb_color == RBTREE_COLOR_BLACK)
			return;

		if (uncle && uncle->rb_color == RBTREE_COLOR_RED)
		{
			parent->rb_color = RBTREE_COLOR_BLACK;
			uncle->rb_color = RBTREE_COLOR_BLACK;
			grandfather->rb_color = RBTREE_COLOR_RED;
			node = grandfather;
		}
		else
		{
			if (node == parent->rb_right &&
				parent == grandfather->rb_left)
			{
				rbtree_rotate(parent);
				node = node->rb_left;
				parent = node ? node->rb_parent : NULL;
				grandfather = parent ? parent->rb_parent : NULL;
			}
			else if (node == parent->rb_left &&
				parent == grandfather->rb_right)
			{
				rbtree_rotate(parent);
				node = node->rb_right;
				parent = node ? node->rb_parent : NULL;
				grandfather = parent ? parent->rb_parent : NULL;
			}

			node->rb_parent->rb_color = RBTREE_COLOR_BLACK;
			grandfather->rb_color = RBTREE_COLOR_RED;

			if (node == parent->rb_left &&
				parent == grandfather->rb_left)
				rbtree_rotate(grandfather);
			else if (node == parent->rb_right &&
				parent == grandfather->rb_right)
				rbtree_rotate(grandfather);

			return;
		}
	}

	return;
}

/**
 * Insert a new record into the rbtree. If an entry with the same key
 * already exists, the currently specified entry is prepended before
 * it.
 */
int
c_rbtree_insert(struct c_rbtree *t, const void *key, const void *value)
{
	struct c_rbtree_entry *newval, *parent, *next;
	int comparison = 0;

	if (!t)
		return 0;

	newval = malloc(sizeof(struct c_rbtree_entry));
	if (!newval)
		return 0;
	
	memset(newval, 0, sizeof(struct c_rbtree_entry));
	
	newval->rb_left = newval->rb_right = NULL;
	newval->rb_key = key;
	newval->rb_value = value;
	newval->rb_color = RBTREE_COLOR_RED;

	/* First, just add the entry to the tree. */
	next = parent = t->rb_head;

	while (next != NULL)
	{
		parent = next;
		comparison = t->rb_compare(key, next->rb_key);

		/* We don't special-case 0 here because insert doesn't
		   care if the entry exists. The rebalancing will take
		   care of the rest. */
		if (comparison < 0)
			next = next->rb_left;
		else
			next = next->rb_right;
	}

	/* Not sure if it's good to have this here on top
	   performance-wise since it's the very definition of
	   an unlikely corner case. */
	if (t->rb_head == NULL)
	{
		newval->rb_color = RBTREE_COLOR_BLACK;
		newval->rb_parent = NULL;
		t->rb_head = newval;
		return 1;
	}

	if (t->rb_compare(key, parent->rb_key) < 0)
	{
		assert(parent->rb_left == NULL);
		parent->rb_left = newval;
		newval->rb_parent = parent;
	}
	else
	{
		assert(parent->rb_right == NULL);
		parent->rb_right = newval;
		newval->rb_parent = parent;
	}

	c_rbtree_rebalance(newval);

	return 1;
}

/**
 * Replace the first rbtree entry currently associated with the current
 * key with the specified entry. If no key is found it is simply added.
 */
int
c_rbtree_replace(struct c_rbtree *t, const void *key, const void *value)
{
	struct c_rbtree_entry *newval, *parent, *next;
	int comparison = 0;

	if (!t)
		return 0;

	newval = malloc(sizeof(struct c_rbtree_entry));
	if (!newval)
		return 0;
	
	memset(newval, 0, sizeof(struct c_rbtree_entry));
	
	newval->rb_left = newval->rb_right = NULL;
	newval->rb_key = key;
	newval->rb_value = value;
	newval->rb_color = RBTREE_COLOR_RED;

	/* First, just add the entry to the tree. */
	next = parent = t->rb_head;

	while (next != NULL)
	{
		parent = next;
		comparison = t->rb_compare(key, next->rb_key);

		if (comparison < 0)
			next = next->rb_left;
		else if (comparison == 0)
		{
			free(newval);
			next->rb_value = value;
			t->rb_keydestr(key);
		}
		else
			next = next->rb_right;
	}

	/* Not sure if it's good to have this here on top
	   performance-wise since it's the very definition of
	   an unlikely corner case. */
	if (t->rb_head == NULL)
	{
		newval->rb_color = RBTREE_COLOR_BLACK;
		newval->rb_parent = NULL;
		t->rb_head = newval;
		return 1;
	}

	if (t->rb_compare(key, parent->rb_key) < 0)
	{
		assert(parent->rb_left == NULL);
		parent->rb_left = newval;
		newval->rb_parent = parent;
	}
	else
	{
		assert(parent->rb_right == NULL);
		parent->rb_right = newval;
		newval->rb_parent = parent;
	}

	c_rbtree_rebalance(newval);
	return 1;
}
