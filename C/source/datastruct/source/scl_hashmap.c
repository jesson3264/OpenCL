/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#include "scl_private.h"

//#include "scl_general.h"
#include "c_sharemem.h"
//#include "scl_time.h"
#include "scl_hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define scl_palloc  ShareAllocate
/*
 * The internal form of a hash table.
 *
 * The table is an array indexed by the hash of the key; collisions
 * are resolved by hanging a linked list of hash entries off each
 * element of the array. Although this is a really simple design it
 * isn't too bad given that pools have a low allocation overhead.
 */
typedef struct scl_hash_entry_t scl_hash_entry_t;

struct scl_hash_entry_t {
    scl_hash_entry_t *next;
    unsigned int      hash;
    const void       *key;
    ssize_t           klen;
    const void       *val;
};

/*
 * Data structure for iterating through a hash table.
 *
 * We keep a pointer to the next hash entry here to allow the current
 * hash entry to be freed or otherwise mangled between calls to
 * scl_hash_next().
 */
struct scl_hash_index_t {
    scl_hash_t         *ht;
    scl_hash_entry_t   *this, *next;
    unsigned int        index;
};

/*
 * The size of the array is always a power of two. We use the maximum
 * index rather than the size so that we can use bitwise-AND for
 * modular arithmetic.
 * The count of hash entries may be greater depending on the chosen
 * collision rate.
 */
struct scl_hash_t {
    CShareMemPool      *pool;
    scl_hash_entry_t   **array;
    scl_hash_index_t     iterator;  /* For scl_hash_first(NULL, ...) */
    unsigned int         count, max, seed;
    scl_hashfunc_t       hash_func;
    scl_hash_entry_t    *free;  /* List of recycled entries */
};

#define INITIAL_MAX 15 /* tunable == 2^n - 1 */

static scl_hash_entry_t **alloc_array(scl_hash_t *ht, unsigned int max)
{
    // 分配  max+1 个 scl_hash_entry_t
   void* p = scl_palloc(ht->pool, sizeof(*ht->array) * (max + 1));
   memset(p, 0, sizeof(*ht->array) * (max + 1));
   return (scl_hash_entry_t**)p;
}

/*
 * Hash creation functions.
 */
scl_hash_t* scl_hash_make(CShareMemPool *pool)
{
    scl_hash_t *ht;
    //scl_time_t now = scl_time_now();

    ht = scl_palloc(pool, sizeof(scl_hash_t));
    ht->pool = pool;
    ht->free = NULL;
    ht->count = 0;
    ht->max = INITIAL_MAX;
    // 为什么要搞个种子
    ht->seed = 100;//(unsigned int)((now >> 32) ^ now ^ (scl_uintptr_t)pool ^
                //              (scl_uintptr_t)ht ^ (scl_uintptr_t)&now) - 1;

    ht->array = alloc_array(ht, ht->max);
    ht->hash_func = NULL;
    // 这里没有为 free 进行分配
    return ht;
}

/**
 * 自定义 hash 函数分配 hash 表
*/ 
scl_hash_t* scl_hash_make_custom(CShareMemPool *pool,
                                               scl_hashfunc_t hash_func)
{
    scl_hash_t *ht = scl_hash_make(pool);
    ht->hash_func = hash_func;
    return ht;
}


/*
 * Hash iteration functions.
 */
scl_hash_index_t * scl_hash_next(scl_hash_index_t *hi)
{
    hi->this = hi->next;
    while (!hi->this) {
        if (hi->index > hi->ht->max)
            return NULL;

        hi->this = hi->ht->array[hi->index++];
    }
    hi->next = hi->this->next;
    return hi;
}

scl_hash_index_t * scl_hash_first(CShareMemPool *p, scl_hash_t *ht)
{
    scl_hash_index_t *hi;
    if (p)
        hi = scl_palloc(p, sizeof(*hi));
    else
        hi = &ht->iterator;

    hi->ht = ht;
    hi->index = 0;
    hi->this = NULL;
    hi->next = NULL;
    return scl_hash_next(hi);
}

void scl_hash_this(scl_hash_index_t *hi,
                                const void **key,
                                ssize_t *klen,
                                void **val)
{
    if (key)  *key  = hi->this->key;
    if (klen) *klen = hi->this->klen;
    if (val)  *val  = (void *)hi->this->val;
}

const void * scl_hash_this_key(scl_hash_index_t *hi)
{
    const void *key;

    scl_hash_this(hi, &key, NULL, NULL);
    return key;
}

ssize_t scl_hash_this_key_len(scl_hash_index_t *hi)
{
    ssize_t klen;

    scl_hash_this(hi, NULL, &klen, NULL);
    return klen;
}

void * scl_hash_this_val(scl_hash_index_t *hi)
{
    void *val;

    scl_hash_this(hi, NULL, NULL, &val);
    return val;
}

/*
 * Expanding a hash table
 */
static void expand_array(scl_hash_t *ht)
{
    scl_hash_index_t *hi;
    scl_hash_entry_t **new_array;
    unsigned int new_max;

    new_max = ht->max * 2 + 1;
    new_array = alloc_array(ht, new_max);
    for (hi = scl_hash_first(NULL, ht); hi; hi = scl_hash_next(hi)) {
        unsigned int i = hi->this->hash & new_max;
        hi->this->next = new_array[i];
        new_array[i] = hi->this;
    }
    ht->array = new_array;
    ht->max = new_max;
}

static unsigned int hashfunc_default(const char *char_key, ssize_t *klen,
                                     unsigned int hash)
{
    const unsigned char *key = (const unsigned char *)char_key;
    const unsigned char *p;
    ssize_t i;
    
    /*
     * This is the popular `times 33' hash algorithm which is used by
     * perl and also appears in Berkeley DB. This is one of the best
     * known hash functions for strings because it is both computed
     * very fast and distributes very well.
     *
     * The originator may be Dan Bernstein but the code in Berkeley DB
     * cites Chris Torek as the source. The best citation I have found
     * is "Chris Torek, Hash function for text in C, Usenet message
     * <27038@mimsy.umd.edu> in comp.lang.c , October, 1990." in Rich
     * Salz's USENIX 1992 paper about INN which can be found at
     * <http://citeseer.nj.nec.com/salz92internetnews.html>.
     *
     * The magic of number 33, i.e. why it works better than many other
     * constants, prime or not, has never been adequately explained by
     * anyone. So I try an explanation: if one experimentally tests all
     * multipliers between 1 and 256 (as I did while writing a low-level
     * data structure library some time ago) one detects that even
     * numbers are not useable at all. The remaining 128 odd numbers
     * (except for the number 1) work more or less all equally well.
     * They all distribute in an acceptable way and this way fill a hash
     * table with an average percent of approx. 86%.
     *
     * If one compares the chi^2 values of the variants (see
     * Bob Jenkins ``Hashing Frequently Asked Questions'' at
     * http://burtleburtle.net/bob/hash/hashfaq.html for a description
     * of chi^2), the number 33 not even has the best value. But the
     * number 33 and a few other equally good numbers like 17, 31, 63,
     * 127 and 129 have nevertheless a great advantage to the remaining
     * numbers in the large set of possible multipliers: their multiply
     * operation can be replaced by a faster operation based on just one
     * shift plus either a single addition or subtraction operation. And
     * because a hash function has to both distribute good _and_ has to
     * be very fast to compute, those few numbers should be preferred.
     *
     *                  -- Ralf S. Engelschall <rse@engelschall.com>
     */

    if (*klen == SCL_HASH_KEY_STRING) {
        for (p = key; *p; p++) {
            hash = hash * 33 + *p;
        }
        *klen = p - key;
    }
    else {
        for (p = key, i = *klen; i; i--, p++) {
            hash = hash * 33 + *p;
        }
    }

    return hash;
}

unsigned int scl_hashfunc_default(const char *char_key,
                                                      ssize_t *klen)
{
    return hashfunc_default(char_key, klen, 0);
}

/*
 * This is where we keep the details of the hash function and control
 * the maximum collision rate.
 *
 * If val is non-NULL it creates and initializes a new hash entry if
 * there isn't already one there; it returns an updatable pointer so
 * that hash entries can be removed.
 */

static scl_hash_entry_t **find_entry(scl_hash_t *ht,
                                     const void *key,
                                     ssize_t klen,
                                     const void *val)
{
    scl_hash_entry_t **hep, *he;
    unsigned int hash;
    ///< 先根据 key 获取 hash 值
    if (ht->hash_func)
        hash = ht->hash_func(key, &klen);
    else
        hash = hashfunc_default(key, &klen, ht->seed);

    /* scan linked list */
    for (hep = &ht->array[hash & ht->max], he = *hep;
         he; 
         hep = &he->next, he = *hep) 
    {
        if (he->hash == hash
            && he->klen == klen
            && memcmp(he->key, key, klen) == 0)
            break;
    }

    // 已经存在或者值为空
    if (he || !val)
        return hep;

    ///< 没有找到， 则进行分配
    /* add a new entry for non-NULL values */
    if ((he = ht->free) != NULL)
        ht->free = he->next; /// 这里为什么要做一次 free 的赋值？
    else
        he = scl_palloc(ht->pool, sizeof(*he));
    // 这里 he 要么是从空闲中来，要么是分配来的
    he->next = NULL;
    he->hash = hash;
    he->key  = key;///< 这里直接键赋值，没有为传入的key 分配内存
    he->klen = klen;
    he->val  = val;
    *hep = he;
    ht->count++;
    return hep;
}

scl_hash_t * scl_hash_copy(CShareMemPool *pool,
                                        const scl_hash_t *orig)
{
    scl_hash_t *ht;
    scl_hash_entry_t *new_vals;
    unsigned int i, j;

    ht = scl_palloc(pool, sizeof(scl_hash_t) +
                    sizeof(*ht->array) * (orig->max + 1) +
                    sizeof(scl_hash_entry_t) * orig->count);
    ht->pool = pool;
    ht->free = NULL;
    ht->count = orig->count;
    ht->max = orig->max;
    ht->seed = orig->seed;
    ht->hash_func = orig->hash_func;
    ht->array = (scl_hash_entry_t **)((char *)ht + sizeof(scl_hash_t));

    new_vals = (scl_hash_entry_t *)((char *)(ht) + sizeof(scl_hash_t) +
                                    sizeof(*ht->array) * (orig->max + 1));
    j = 0;
    for (i = 0; i <= ht->max; i++) {
        scl_hash_entry_t **new_entry = &(ht->array[i]);
        scl_hash_entry_t *orig_entry = orig->array[i];
        while (orig_entry) {
            *new_entry = &new_vals[j++];
            (*new_entry)->hash = orig_entry->hash;
            (*new_entry)->key = orig_entry->key;
            (*new_entry)->klen = orig_entry->klen;
            (*new_entry)->val = orig_entry->val;
            new_entry = &((*new_entry)->next);
            orig_entry = orig_entry->next;
        }
        *new_entry = NULL;
    }
    return ht;
}

void * scl_hash_get(scl_hash_t *ht,
                                 const void *key,
                                 ssize_t klen)
{
    scl_hash_entry_t *he;
    he = *find_entry(ht, key, klen, NULL);
    if (he)
        return (void *)he->val;
    else
        return NULL;
}

void scl_hash_set(scl_hash_t *ht,
                               const void *key,
                               ssize_t klen,
                               const void *val)
{
    scl_hash_entry_t **hep;// 
    hep = find_entry(ht, key, klen, val);
    if (*hep) {
        if (!val) {//如果为空
            /* delete entry */
            scl_hash_entry_t *old = *hep;
            *hep = (*hep)->next;
            old->next = ht->free;//放到空闲链表中
            ht->free = old;
            --ht->count;
        }
        else {  
            /* replace entry */
            (*hep)->val = val;//原有的内存需要自己释放
            /* check that the collision rate isn't too high */
            if (ht->count > ht->max) {
                expand_array(ht);
            }
        }
    }
    /* else key not present and val==NULL */
}

unsigned int scl_hash_count(scl_hash_t *ht)
{
    return ht->count;
}

void scl_hash_clear(scl_hash_t *ht)
{
    scl_hash_index_t *hi;
    for (hi = scl_hash_first(NULL, ht); hi; hi = scl_hash_next(hi))
        scl_hash_set(ht, hi->this->key, hi->this->klen, NULL);
}

scl_hash_t* scl_hash_overlay(CShareMemPool *p,
                                          const scl_hash_t *overlay,
                                          const scl_hash_t *base)
{
    return scl_hash_merge(p, overlay, base, NULL, NULL);
}

scl_hash_t * scl_hash_merge(CShareMemPool *p,
                                         const scl_hash_t *overlay,
                                         const scl_hash_t *base,
                                         void * (*merger)(CShareMemPool *p,
                                                     const void *key,
                                                     ssize_t klen,
                                                     const void *h1_val,
                                                     const void *h2_val,
                                                     const void *data),
                                         const void *data)
{
    scl_hash_t *res;
    scl_hash_entry_t *new_vals = NULL;
    scl_hash_entry_t *iter;
    scl_hash_entry_t *ent;
    unsigned int i, j, k, hash;

#if scl_POOL_DEBUG
    /* we don't copy keys and values, so it's necessary that
     * overlay->a.pool and base->a.pool have a life span at least
     * as long as p
     */
    if (!scl_pool_is_ancestor(overlay->pool, p)) {
        fprintf(stderr,
                "scl_hash_merge: overlay's pool is not an ancestor of p\n");
        abort();
    }
    if (!scl_pool_is_ancestor(base->pool, p)) {
        fprintf(stderr,
                "scl_hash_merge: base's pool is not an ancestor of p\n");
        abort();
    }
#endif

    res = scl_palloc(p, sizeof(scl_hash_t));
    res->pool = p;
    res->free = NULL;
    res->hash_func = base->hash_func;
    res->count = base->count;
    res->max = (overlay->max > base->max) ? overlay->max : base->max;
    if (base->count + overlay->count > res->max) {
        res->max = res->max * 2 + 1;
    }
    res->seed = base->seed;
    res->array = alloc_array(res, res->max);
    if (base->count + overlay->count) {
        new_vals = scl_palloc(p, sizeof(scl_hash_entry_t) *
                              (base->count + overlay->count));
    }
    j = 0;
    for (k = 0; k <= base->max; k++) {
        for (iter = base->array[k]; iter; iter = iter->next) {
            i = iter->hash & res->max;
            new_vals[j].klen = iter->klen;
            new_vals[j].key = iter->key;
            new_vals[j].val = iter->val;
            new_vals[j].hash = iter->hash;
            new_vals[j].next = res->array[i];
            res->array[i] = &new_vals[j];
            j++;
        }
    }

    for (k = 0; k <= overlay->max; k++) {
        for (iter = overlay->array[k]; iter; iter = iter->next) {
            if (res->hash_func)
                hash = res->hash_func(iter->key, &iter->klen);
            else
                hash = hashfunc_default(iter->key, &iter->klen, res->seed);
            i = hash & res->max;
            for (ent = res->array[i]; ent; ent = ent->next) {
                if ((ent->klen == iter->klen) &&
                    (memcmp(ent->key, iter->key, iter->klen) == 0)) {
                    if (merger) {
                        ent->val = (*merger)(p, iter->key, iter->klen,
                                             iter->val, ent->val, data);
                    }
                    else {
                        ent->val = iter->val;
                    }
                    break;
                }
            }
            if (!ent) {
                new_vals[j].klen = iter->klen;
                new_vals[j].key = iter->key;
                new_vals[j].val = iter->val;
                new_vals[j].hash = hash;
                new_vals[j].next = res->array[i];
                res->array[i] = &new_vals[j];
                res->count++;
                j++;
            }
        }
    }
    return res;
}

/* This is basically the following...
 * for every element in hash table {
 *    comp elemeny.key, element.value
 * }
 *
 * Like with scl_table_do, the comp callback is called for each and every
 * element of the hash table.
 */
int scl_hash_do(scl_hash_do_callback_fn_t *comp,
                             void *rec, const scl_hash_t *ht)
{
    scl_hash_index_t  hix;
    scl_hash_index_t *hi;
    int rv, dorv  = 1;

    hix.ht    = (scl_hash_t *)ht;
    hix.index = 0;
    hix.this  = NULL;
    hix.next  = NULL;

    if ((hi = scl_hash_next(&hix))) {
        /* Scan the entire table */
        do {
            rv = (*comp)(rec, hi->this->key, hi->this->klen, hi->this->val);
        } while (rv && (hi = scl_hash_next(hi)));

        if (rv == 0) {
            dorv = 0;
        }
    }
    return dorv;
}

//scl_POOL_IMPLEMENT_ACCESSOR(hash)
