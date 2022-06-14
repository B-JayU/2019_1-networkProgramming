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
	temp->cID = client ��ȣ;
	temp->row = row;
	temp->column = column;

	if (client[client ��ȣ] == NULL)
	client[client ��ȣ] = temp;
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
	char message1[] = "�¼��� �����Ϸ��� 'R', �����Ϸ��� 'Q' : ";
	char message2[] = "�¼��� �����Ϸ��� 'R', ��ȯ�Ϸ��� 'E', �� �¼� ���� : 'S', �����Ϸ��� 'Q' : ";

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

	FD_ZERO(&reads);//select �Լ��� ���� ����� �Ǵ� ��ũ���͵��� 0���� �ʱ�ȭ
	//hServSock�� ������ �����̹Ƿ� �����û�� ������ Ȯ���ϱ� ����
	FD_SET(hServSock, &reads);//hServSock�� ����������� ���

	while (1)
	{
		//select �Լ��� ȣ���� ������ ��ȭ�� �߻��� ��ũ���͸� ������ �������� 0����
		//���� ��ũ���� �������� ����
		cpyReads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		//select�Լ� error ó��
		//�����쿡���� ��������� 
		if ((fdNum = select(0, &cpyReads, 0, 0, 0)) == SOCKET_ERROR)
			break;
		//readset �� � ��ũ����(�迭 ����)���� ��ȭ�� ������ �ǹ�
		//���� Ÿ�Ӿƿ� �缳���ϰ� ��ȣ���ϱ� ���� continue
		if (fdNum == 0)
			continue;
		//���� ��ũ���� ���� ���°� ����Ǿ��� ���
		//(���� ��û �Ǵ� ������ ����)
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
									send(reads.fd_array[i], "OȮ��? : ", 10, 0);
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
						//	printf("buf�� O�� ��� : %d ", loc);

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
