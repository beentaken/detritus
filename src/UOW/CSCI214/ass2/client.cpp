#include "wrappers.h"

#define GRID_LOOP(x)	for (x = 0; x < GRID_SZ; x++)

const int	max_score = 5;

pos		player, p_torpedo, s_torpedo;

int		player_score = 0, server_score = 0;

void
print_grid_line(int line) {
	int	i;

	assert(line < GRID_SZ);

	GRID_LOOP(i) {
		if (player.y == line && player.x == i) {
			putchar('P');
		} else if (p_torpedo.y != -1 && p_torpedo.y == line && p_torpedo.x == i) {
			putchar('p');
		} else if (s_torpedo.y != -1 && s_torpedo.y == line && s_torpedo.x == i) {
			putchar('c');
		} else {
			putchar('+');
		}

		putchar(' ');
	}
	printf("  ");
}

void
display(void) {
	int	i = 0;

	print_grid_line(i++);
	printf("      SCORES\n");
	print_grid_line(i++);
	printf("  Player  Computer\n");
	print_grid_line(i++);
	printf("%5d%9d\n", player_score, server_score);
	print_grid_line(i++);
	printf("\n");
	print_grid_line(i++);
	printf("P = Player's Position\n");
	print_grid_line(i++);
	printf("p = Players's Torpedo\n");
	print_grid_line(i++);
	printf("c = Computer's Torpedo\n");
	printf("\n");
	printf("(f)ire (l)eft (r)ight (u)p (d)own (s)urrender\n");
	printf("Enter your move => ");
}

int main(int argc, char **argv) {
	int	command = -1;

	int	playing = 1;

	int	sock;

	unsigned int	addrlen = sizeof(struct sockaddr_in);
	unsigned int	port = 40642;

	sockaddr_in	addr;

	char	move = EOF;

	common_init(argc, argv, port, addr.sin_addr.s_addr);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	sock = Socket(PF_INET, SOCK_STREAM, 0);

	Connect(sock, (struct sockaddr *)&addr, addrlen);

	Read(sock, &player.x, sizeof(int));
	Read(sock, &player.y, sizeof(int));

	p_torpedo.x = p_torpedo.y = -1;
	s_torpedo.x = s_torpedo.y = -1;

	while (playing) {
		display();

		do {
			move = getchar();
		} while (move != EOF && !isalpha(move));

		switch(move) {
			case 'r':	player.x = (player.x + 1) % GRID_SZ;
					command = MOVE;
					break;

			case 'l':	player.x = (player.x + GRID_SZ - 1) % GRID_SZ;
					command = MOVE;
					break;

			case 'd':	player.y = (player.y + 1) % GRID_SZ;
					command = MOVE;
					break;

			case 'u':	player.y = (player.y + GRID_SZ - 1) % GRID_SZ;
					command = MOVE;
					break;

			case 'f':	printf("Enter target position (x y) => ");
					command = FIRE;
					scanf("%d %d", &p_torpedo.x, &p_torpedo.y);
					break;

			case 's':	printf("\n***** GAME OVER - You have surrendered! *****\n");
			case EOF:	command = SURRENDER;
					break;

			default:	printf("Sorry, I don't understand the command '%c'.\n", move);
					printf("Enter your move => ");
		}

		Write(sock, &command, sizeof(int));

		if (command == FIRE) {
			Write(sock, &p_torpedo, sizeof(pos));
		} else if (command == SURRENDER) {
			break;
		}

		Read(sock, &command, sizeof(int));

		if (command == HIT) {
			player_score++;
		}

		Read(sock, &command, sizeof(int));

		switch (command) {
			case	FIRE:	Read(sock, &s_torpedo, sizeof(pos));
					if (s_torpedo.x == player.x && s_torpedo.y == player.y) {
						command = HIT;
						server_score++;
					} else {
						command = MISS;
					}

					break;

			case	MOVE:	command = ACK;
		}

		Write(sock, &command, sizeof(int));

		if (player_score == max_score || server_score == max_score) {
			command = SURRENDER;
			Write(sock, &command, sizeof(int));
			playing = 0;
		}
	}


	if (player_score == max_score) {
		printf("You have won!\n");
	} else if (server_score == max_score) {
		printf("You have lost!\n");
	}

	return 0;
}
