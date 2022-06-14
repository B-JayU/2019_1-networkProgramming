//server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define A_SIZE 32
#define row 8
#define col 4
#define maxClient 6

typedef struct node *nodePointer;
typedef struct node {
	int row_s;
	int col_s;
	nodePointer next;
}node;

typedef struct {
	SOCKET clientId;
	int seatCount;
	nodePointer seat;
}clientInfo;

nodePointer makeNewNode(int r, int c);

void ErrorHandling(char *message);

int main(int argc, char *argv[])
{

	clientInfo client[maxClient];


	/*   node* temp = malloc(sizeof(node));
	temp->cID = client 번호;
	temp->row = row;
	temp->column = column;

	if (client[client 번호] == NULL)
	client[client 번호] = temp;
	*/

	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	TIMEVAL timeout;///////////////
	fd_set reads, cpyReads;///////////
	char arrLONG[A_SIZE] = "00000000000000000000000000000000";
	int r, c, loc;

	int adrSz;
	int strLen, fdNum, i;
	char buf[A_SIZE];
	int recent, msg1_len, msg2_len;
	char message1[] = "좌석을 예약하려면 'R', 종료하려면 'Q' : ";
	char message2[] = "좌석을 예약하려면 'R', 교환하려면 'E', 내 좌석 보기 : 'S', 종료하려면 'Q' : ";

	for (i = 0; i < maxClient; i++) {
		client[i].seat = NULL;
		client[i].seatCount = 0;
	}

	msg1_len = strlen(message1);
	msg2_len = strlen(message2);
	/*if (argc != 2) {
	printf("Usage : %s <port>\n", argv[0]);
	exit(1);
	}*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi("190528"));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	FD_ZERO(&reads);//select 함수의 관찰 대상이 되는 디스크립터들을 0으로 초기화
	//hServSock은 리스닝 소켓이므로 연결요청이 오는지 확인하기 위해
	FD_SET(hServSock, &reads);//hServSock을 관찰대상으로 등록

	while (1)
	{
		//select 함수의 호출이 끝나면 변화가 발생한 디스크립터를 제외한 나머지는 0으로
		//따라서 디스크립터 정보들을 복사
		cpyReads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		//select함수 error 처리
		//윈도우에서는 관찰대상이 
		if ((fdNum = select(0, &cpyReads, 0, 0, 0)) == SOCKET_ERROR)
			break;
		//readset 즉 어떤 디스크립터(배열 내에)에도 변화가 없음을 의미
		//따라서 타임아웃 재설정하고 재호출하기 위해 continue
		if (fdNum == 0)
			continue;
		//파일 스크립터 내에 상태가 변경되었을 경우
		//(연결 요청 또는 데이터 수신)
		for (i = 0; i < reads.fd_count; i++)
		{
			if (FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				if (reads.fd_array[i] == hServSock)     // connection request!
				{

					adrSz = sizeof(clntAdr);
					hClntSock =
						accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
					FD_SET(hClntSock, &reads);

					printf("connected client: %d \n", hClntSock);
					send(hClntSock, arrLONG, 32, 0);
					client[reads.fd_count - 1].clientId = hClntSock;
					//   client[i].
				}
				else    // read message!
				{
					strLen = recv(reads.fd_array[i], buf, A_SIZE, 0);
					//printf("Strlen : %d ", strLen);
					//printf("%s", buf);
					//recent = i;


					if (strLen == 0)    // close request!
					{
						FD_CLR(reads.fd_array[i], &reads);
						closesocket(cpyReads.fd_array[i]);
						printf("closed client: %d \n", cpyReads.fd_array[i]);
					}
					else
					{
						if (strLen == 20) {


							if (client[i].seatCount == 0) {
								send(reads.fd_array[i], message1, msg1_len, 0);
							}
							else {
								send(reads.fd_array[i], message2, msg2_len, 0);
							}
							//nodePointer temp = (nodePointer)malloc(sizeof(nodePointer));
							//node* temp = malloc(sizeof(node));

							//client[i].seat = temp;

							/*
							nodePointer temp = (nodePointer)malloc(sizeof(nodePointer));
							temp->row_s = r;
							temp->col_s = c;*/
						}
						else if (buf[0] == 'R') {

							if (strLen == 1) {
								send(reads.fd_array[i], buf, strLen, 0);

							}

							else {
								r = buf[1] - '1', c = buf[3] - '1';
								loc = r * col + c;

								if (arrLONG[loc] == '0') {

									//client[i].seatCount++;
									arrLONG[loc] = '1';
									send(reads.fd_array[i], "O확정? : ", 10, 0);
								}
								else {
									char a[34];
									a[0] = 'X';
									a[1] = 0;
									strcat(a, arrLONG);
									send(reads.fd_array[i], a, 34, 0);

									//client[i].seatCount--;
									continue;
								}
							}
						}
						else if (buf[0] == 'E') {

							if (strLen == 1) {
								//   printf("%s\n",buf);
								send(reads.fd_array[i], buf, strLen, 0);
							}

							else {
								r = buf[1] - '1', c = buf[3] - '1';
								//loc = r*col + c;

								int rr, cc;
								SOCKET which = NULL;
								rr = buf[4] - '1', cc = buf[6] - '1';
								int ii;
								for (ii = 0; ii < maxClient; ii++) {
									for (nodePointer search = client[ii].seat; search; search = search->next) {
										if (search->row_s == rr && search->col_s == cc) {
											which = client[ii].clientId;
											break;
										}
									}
									if (which)
										break;
								}


							//	printf("%d\n", which);



								char message_e[100];
								buf[0] = i + '0';
								send(which, buf, 7, 0);


							}

						}
						else if (buf[0] == 'O') {

							r = buf[1] - '1', c = buf[3] - '1';
							loc = r * col + c;
						//	printf("buf가 O일 경우 : %d ", loc);

							nodePointer temp = (nodePointer)malloc(sizeof(nodePointer));
							temp->row_s = r;
							temp->col_s = c;

							if (client[i].seatCount == 0) {
								temp->next = NULL;
								client[i].seat = temp;
							}
							else {
								temp->next = client[i].seat;
								client[i].seat = temp;
							}
							client[i].seatCount++;
							send(reads.fd_array[i], arrLONG, A_SIZE, 0);
						}
						else if (buf[0] == 'X') {

							r = buf[1] - '1', c = buf[3] - '1';
							loc = r * col + c;
							arrLONG[loc] = '0';
							//printf("loc : %d\n", loc);
							send(reads.fd_array[i], arrLONG, A_SIZE, 0);
							continue;
						}
						else if (buf[0] == 'Y') {
							int request_client = buf[1] - '0';

							nodePointer search;
							for (search = client[request_client].seat; search; search = search->next) {
								if (search->row_s == buf[2] - '1' && search->col_s == buf[4] - '1')
									break;
							}

							nodePointer search2;
							for (search2 = client[i].seat; search2; search2 = search2->next) {
								if (search2->row_s == buf[5] - '1' && search2->col_s == buf[7] - '1')
									break;
							}

							int r_temp, c_temp;
							r_temp = search->row_s;
							search->row_s = search2->row_s;
							search2->row_s = r_temp;

							c_temp = search->col_s;
							search->col_s = search2->col_s;
							search2->col_s = c_temp;

							char m[100];
							sprintf(m, "Y%s", arrLONG);
							send(client[request_client].clientId, m, 33, 0);
							send(reads.fd_array[i], arrLONG, A_SIZE, 0);
						}
						else if (buf[0] == 'N') {
							char m[100];
							int request_client = buf[1] - '0';
							sprintf(m, "N%s", arrLONG);
							send(client[request_client].clientId, m, 33, 0);
						}
						else if (buf[0] == 'S') {
							send(reads.fd_array[i], arrLONG, A_SIZE, 0);
						}
						else {
							continue;
							printf("ERROR\n");
						}
						memset(buf, 0, sizeof(buf));
					}
				}
			}
		}
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}