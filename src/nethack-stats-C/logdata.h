
#ifndef __LOGDATA_H
#define __LOGDATA_H

struct _nethack_version {
	unsigned short	major;
	unsigned short	minor;
	unsigned short	revision;
} typedef NH_Ver;

#define F_LEN 4
#define A_LEN 1024

struct _data {
	NH_Ver		version;
	int		a, b, c, d, e, f, g;
	int		end_date, start_date;
	unsigned short	pid;
	char		class[F_LEN];
	char		race[F_LEN];
	char		gender[F_LEN];
	char		alignment[F_LEN];
	char		name[A_LEN];
	char		excuse[A_LEN];
} typedef Data;

int
getData(Data *data);

void
printData(Data *data);

#endif	/* __LOGDATA_H */
