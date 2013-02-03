/*
 * ivykis, an event handling library
 * Copyright (C) 2013 Lennert Buytenhek
 * Dedicated to Marija Kulikova.
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version
 * 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License version 2.1 for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License version 2.1 along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <pthread.h>
#include <signal.h>

#ifdef HAVE_PRAGMA_WEAK
#pragma weak pthread_once
#endif

static inline int pthreads_available(void)
{
	return !!(pthread_once != NULL);
}


#ifdef HAVE_PRAGMA_WEAK
#pragma weak pthread_getspecific
#pragma weak pthread_key_create
#pragma weak pthread_setspecific
#endif

typedef union {
	pthread_key_t	pk;
	const void	*ptr;
} pthr_key_t;

static inline void *pthr_getspecific(pthr_key_t *key)
{
	if (pthreads_available())
		return pthread_getspecific(key->pk);

	return (void *)key->ptr;
}

static inline int pthr_key_create(pthr_key_t *key, void (*destructor)(void*))
{
	if (pthreads_available())
		return pthread_key_create(&key->pk, destructor);

	return 0;
}

static inline int pthr_setspecific(pthr_key_t *key, const void *value)
{
	if (pthreads_available())
		return pthread_setspecific(key->pk, value);

	key->ptr = value;

	return 0;
}