struct item 	*OrderedCreateItem(struct item *);
struct item 	*OrderedInsertItem(struct item *, char *);
struct item 	*OrderedDeleteItem(struct item *, char *);
char		*GetSmallestItem(struct item *);
char		*GetLargestItem(struct item *);
int		IsMemberOfOrderedList(struct item *, char *);
void		OrderedPrintItem(struct item *);
