/**
 * List operations copied from the linux kernel source code.
 * */
#ifndef _LIST_H_
#define _LIST_H_


struct list_head {
	struct list_head *next, *prev;
};

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#undef container_of
#define container_of(ptr, type, member) __extension__ ({        \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
        struct list_head name __attribute__((used)) = LIST_HEAD_INIT(name)
static inline void INIT_LIST_HEAD(struct list_head *list)
{
        list->next = list;
        list->prev = list;
}


/**
 * list_entry - get the struct for this entry
 * @ptr:        the &struct list_head pointer.
 * @type:       the type of the struct this is embedded in.
 * @member:     the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)

/**
 * list_for_each - iterate over a list
 * @pos: the &struct list_head to use as a loop cursor.
 * @head: the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)


/**
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:        the &struct list_head to use as a loop cursor.
 * @n:          another &struct list_head to use as temporary storage
 * @head:       the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
        for (pos = (head)->next, n = pos->next; pos != (head); \
                pos = n, n = pos->next)


/**
 * list_for_each_entry  -       iterate over list of given type
 * @pos:        the type * to use as a loop cursor.
 * @head:       the head for your list.
 * @member:     the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)                          \
        for (pos = list_entry((head)->next, typeof(*pos), member);      \
             &pos->member != (head);    \
             pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:        the type * to use as a loop cursor.
 * @n:          another type * to use as temporary storage
 * @head:       the head for your list.
 * @member:     the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)                  \
        for (pos = list_entry((head)->next, typeof(*pos), member),      \
                n = list_entry(pos->member.next, typeof(*pos), member); \
             &pos->member != (head);                                    \
             pos = n, n = list_entry(n->member.next, typeof(*n), member))


/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head *_new,
                              struct list_head *prev,
                              struct list_head *next)
{
        next->prev = _new;
        _new->next = next;
        _new->prev = prev;
        prev->next = _new;
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *_new, struct list_head *head)
{
        __list_add(_new, head->prev, head);
}


/*
 * Delete a list entry by making the prev/next entries
 * point to each other. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
        next->prev = prev;
        prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void __list_del_entry(struct list_head *entry)
{
        __list_del(entry->prev, entry->next);
}

static inline void list_del(struct list_head *entry)
{
        __list_del(entry->prev, entry->next);
	/*entry->next = NULL;
        entry->prev = NULL;*/
}

static inline int list_size(struct list_head *head)
{
	int size = 0;
	struct list_head *tmp = NULL;
	list_for_each(tmp, head) {
		size++;
	}
	return size;
}


/**
 * Iterate over the list and free each struct containing the list
 * and remove it from the list.
 *
 * @head: the list head
 * @pos: temporary pointer of type of the struct this is embedded in.
 * @member: name of the list struct within the struct
 * */
#define free_list_entry(head, pos, member) \
	do { \
		struct list_head *__tmp_pos, *n; \
		list_for_each_safe(__tmp_pos, n, head) { \
			pos = list_entry(__tmp_pos, typeof(*pos), member); \
			list_del(__tmp_pos); \
			free(pos); \
		} \
	} while(0)


#endif /* _LIST_H_ */
