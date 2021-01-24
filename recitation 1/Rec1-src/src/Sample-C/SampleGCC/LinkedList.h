struct item 	*CreateItem(struct item *);
struct item 	*InsertItem(struct item *, char *);
struct item 	*DeleteItem(struct item *, char *);
char		*GetFirstItem(struct item *);
char		*GetLastItem(struct item *);
int		IsMemberOfList(struct item *, char *);
void		PrintItem(struct item *);
