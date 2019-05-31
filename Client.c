#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 100

typedef struct node *nodePointer;
typedef struct node {
	int row;
	char col;
	nodePointer next;
};

void printSeat(char *msg, int start);
nodePointer makeNewNode(int row, char col);
void ErrorHandling(char *msg);

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAdr;
	int strLen, i, isSeat, isClose, row, count, select;
	char msg[BUF_SIZE];
	char signal[] = "aaaaaaaaaaaaaaaaaaa";
	char buf, col, set;
	nodePointer mySeat, temp;

	mySeat = NULL;

	/*
	argc = 3;
	strcpy(argv[1], "127.0.0.1");
	strcpy(argv[2], "190528");

	if (argc != 3) {
	printf("Usage : %s <IP> <port>\n", argv[0]);
	exit(1);
	}*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	isSeat = 0;
	isClose = 0;

	hSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(atoi("190528"));

	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");

	strLen = recv(hSock, msg, BUF_SIZE, 0);

	printSeat(msg, 0);

	send(hSock, signal, 20, 0);

	while (!isClose) {
		if (!isSeat) {
			strLen = recv(hSock, msg, BUF_SIZE - 1, 0);
			msg[strLen] = 0;

			fputs(msg, stdout);

			while (1) {
				fgets(msg, BUF_SIZE, stdin);

				if (msg[0] == 'r' || msg[0] == 'R') {
					msg[0] = 'R';
					msg[1] = 0;
					strLen = 1;

					send(hSock, msg, strLen, 0);

					strLen = recv(hSock, msg, BUF_SIZE, 0);

					if (msg[0] != 'R') {
						fprintf(stderr, "예외 처리 필요!");
						exit(1);
					}

					printf("예약할 좌석을 선택하세요( 예. 2,a ) : ");

					scanf("%d%c%c", &row, &buf, &col);
					scanf("%c", &buf);

					sprintf(msg, "R%d,%d", row, col - 'a' + 1);
					strLen = 4;

					send(hSock, msg, strLen, 0);

					strLen = recv(hSock, msg, BUF_SIZE, 0);

					if (msg[0] == 'O') {
						for (i = 1; i < strLen; i++) {
							printf("%c", msg[i]);
						}

						scanf("%c%c", &set, &buf);

						if (set == 'O' || set == 'o') {
							sprintf(msg, "O%d,%d", row, col - 'a' + 1);
							strLen = 4;

							send(hSock, msg, strLen, 0);

							isSeat++;
							if (mySeat)
								mySeat = makeNewNode(row, col);
							else {
								temp = makeNewNode(row, col);
								temp->next = mySeat;
								mySeat = temp;
							}
						}
						else {
							sprintf(msg, "X%d,%d", row, col - 'a' + 1);
							strLen = 4;

							send(hSock, msg, strLen, 0);
						}

						strLen = recv(hSock, msg, BUF_SIZE, 0); \
							printSeat(msg, 0);
					}
					else { // msg[0] == 'X'
						printf("\n이미 예약된 자리입니다.\n");

						printSeat(msg, 1);

						break;
					}

					break;
				}
				else if (msg[0] == 'q' || msg[0] == 'Q') {
					msg[0] = 'Q';
					msg[1] = 0;
					strLen = 1;

					send(hSock, msg, strLen, 0);

					shutdown(hSock, SD_SEND);

					strLen = recv(hSock, msg, BUF_SIZE, 0);

					isClose = 1;

					break;
				}

				printf("잘못된 입력이오니 다시 입력해주세요 : ");
				scanf("%c", &buf);
			}
		}
		else {
			strLen = recv(hSock, msg, BUF_SIZE - 1, 0);
			msg[strLen] = 0;

			fputs(msg, stdout);

			while (1) {
				fgets(msg, BUF_SIZE, stdin);

				if (msg[0] == 'r' || msg[0] == 'R') {
					msg[0] = 'R';
					msg[1] = 0;
					strLen = 1;

					send(hSock, msg, strLen, 0);

					strLen = recv(hSock, msg, BUF_SIZE, 0);

					if (msg[0] != 'R') {
						fprintf(stderr, "예외 처리 필요!");
						exit(1);
					}

					printf("예약할 좌석을 선택하세요( 예. 2,a ) : ");

					scanf("%d%c%c", &row, &buf, &col);
					scanf("%c", &buf);

					sprintf(msg, "R%d,%d", row, col - 'a' + 1);
					strLen = 4;

					send(hSock, msg, strLen, 0);

					strLen = recv(hSock, msg, BUF_SIZE, 0);

					if (msg[0] == 'O') {
						for (i = 1; i < strLen; i++) {
							printf("%c", msg[i]);
						}

						scanf("%c%c", &set, &buf);

						if (set == 'O' || set == 'o') {
							sprintf(msg, "O%d,%d", row, col - 'a' + 1);
							strLen = 4;

							send(hSock, msg, strLen, 0);

							isSeat++;
							if (mySeat)
								mySeat = makeNewNode(row, col);
							else {
								temp = makeNewNode(row, col);
								temp->next = mySeat;
								mySeat = temp;
							}
						}
						else {
							sprintf(msg, "X%d,%d", row, col - 'a' + 1);
							strLen = 4;

							send(hSock, msg, strLen, 0);
						}

						strLen = recv(hSock, msg, BUF_SIZE, 0);
						printSeat(msg, 0);
					}
					else { // msg[0] == 'X'
						printf("\n이미 예약된 자리입니다.\n");

						printSeat(msg, 1);

						break;
					}

					break;
				}
				else if (msg[0] == 'e' || msg[0] == 'E') {
					msg[0] = 'E';
					msg[1] = 0;
					strLen = 1;

					send(hSock, msg, strLen, 0);

					strLen = recv(hSock, msg, BUF_SIZE, 0);

					if (msg[0] != 'E') {
						fprintf(stderr, "예외 처리 필요!");
						exit(1);
					}

					count = 1;
					for (temp = mySeat; temp; temp = temp->next) {
						printf("%d) %d,%c\n", count++, temp->row, temp->col);
					}
					printf("예약한 좌석 중 교환할 좌석의 번호를 입력하세요 : \n");

					scanf("%d%c", &select, &buf);

					printf("교환할 좌석을 입력해주세요( 예. 2,a ) : ");
					scanf("%d%c%c", &row, &buf, &col);
					scanf("%c", &buf);

					temp = mySeat;
					for (i = 0; i < select - 1; i++) {
						temp = temp->next;
					}
					sprintf(msg, "E%d,%d%d,%d", temp->row, temp->col - 'a' + 1, row, col - 'a' + 1);

					send(hSock, msg, 7, 0);

					strLen = recv(hSock, msg, BUF_SIZE, 0);

					if (msg[0] == 'y' || msg[0] == 'Y') {
						printf("교환 완료!\n");
						temp->row = row;
						temp->col = col;
					}
					else {
						printf("교환이 거부되었습니다.\n");
					}

					printSeat(msg, 1);
				}
				else if (msg[0] == 'h' || msg[0] == 'H') {
					while (1) {
						strLen = recv(hSock, msg, BUF_SIZE, 0);

						printf("%c,%c를 %c,%c로 변경 요청이 있습니다. 수락하시겠습니까?(Y/N) : ", msg[4], msg[6] - '1' + 'a', msg[1], msg[3] - '1' + 'a');

						scanf("%c%c", &set, &buf);

						for (i = 0; i < 7; i++) {
							msg[i + 1] = msg[i];
						}
						if (set == 'y' || set == 'Y')
							msg[0] = 'Y';
						if (set == 'n' || set == 'N')
							msg[0] = 'N';

						if (msg[0] == 'Y') {
							for (temp = mySeat; temp; temp = temp->next) {
								if (temp->row == msg[4] - '0' && temp->col == msg[6] - '1' + 'a') {
									break;
								}
							}
							temp->row = msg[1] - '0';
							temp->col = msg[3] - '1' + 'a';
						}

						send(hSock, msg, 8, 0);
					}
				}
				else if (msg[0] == 'q' || msg[0] == 'Q') {
					msg[0] = 'Q';
					msg[1] = 0;
					strLen = 1;

					send(hSock, msg, strLen, 0);

					shutdown(hSock, SD_SEND);

					strLen = recv(hSock, msg, BUF_SIZE, 0);

					isClose = 1;

					break;
				}

				printf("잘못된 입력이오니 다시 입력해주세요 : ");
				scanf("%c", &buf);
			}
		}
		if (!isClose)
			send(hSock, signal, 20, 0);
	}

	closesocket(hSock);
	WSACleanup();

	return 0;
}

void printSeat(char *msg, int start) {
	int i, j;

	printf(" A B  C D \n");
	for (i = 0; i < 8; i++) {
		printf("%d", i + 1);
		for (j = 0; j < 2; j++) {
			if (msg[i * 4 + j + start] == '0')
				printf("□");
			else
				printf("■");
		}
		printf(" ");
		for (j = 2; j < 4; j++) {
			if (msg[i * 4 + j + start] == '0')
				printf("□");
			else
				printf("■");
		}
		printf("\n");
	}
	printf("\n");
}

nodePointer makeNewNode(int row, char col) {
	nodePointer temp;

	temp = (nodePointer)malloc(sizeof(nodePointer));
	temp->row = row;
	temp->col = col;
	temp->next = NULL;

	return temp;
}

void ErrorHandling(char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
