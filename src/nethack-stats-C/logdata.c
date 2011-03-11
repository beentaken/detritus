
#include <stdio.h>
#include "logdata.h"

int
getData(Data *data) {
	unsigned short	*maj = &data->version.major;
	unsigned short	*min = &data->version.minor;
	unsigned short	*rev = &data->version.revision;
	int		*a = &data->a;
	int		*b = &data->b;
	int		*c = &data->c;
	int		*d = &data->d;
	int		*e = &data->e;
	int		*f = &data->f;
	int		*g = &data->g;
	int		*end = &data->end_date;
	int		*start = &data->start_date;
	unsigned short	*pid = &data->pid;
	char		*cla = data->class;
	char		*rac = data->race;
	char		*gen = data->gender;
	char		*ali = data->alignment;
	char		*nam = data->name;
	char		*exc = data->excuse;

	int		count;

	count = scanf("%hu.%hu.%hu %d %d %d %d %d %d %d %d %d %hu %s %s %s %s %[^,],%[^\n]", maj, min, rev, a, b,
			c, d, e, f, g, end, start, pid, cla, rac, gen, ali, nam, exc);
	
	return(count);
}

void
printData(Data *data) {
	unsigned short	*maj = &data->version.major;
	unsigned short	*min = &data->version.minor;
	unsigned short	*rev = &data->version.revision;
	int		*a = &data->a;
	int		*b = &data->b;
	int		*c = &data->c;
	int		*d = &data->d;
	int		*e = &data->e;
	int		*f = &data->f;
	int		*g = &data->g;
	int		*end = &data->end_date;
	int		*start = &data->start_date;
	unsigned short	*pid = &data->pid;
	char		*cla = data->class;
	char		*rac = data->race;
	char		*gen = data->gender;
	char		*ali = data->alignment;
	char		*nam = data->name;
	char		*exc = data->excuse;

	printf("%hu.%hu.%u %d %d %d %d %d %d %d %d %d %hu %s %s %s %s %s,%s\n", *maj, *min, *rev, *a, *b, *c, *d, *e,
		*f, *g, *end, *start, *pid, cla, rac, gen, ali, nam, exc);
}
