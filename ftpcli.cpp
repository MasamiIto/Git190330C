/* $Id: ftp-client.c,v 1.4 2004/05/29 05:36:31 68user Exp $ */
#pragma warning(suppress : 4996)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>

#ifdef NOUSE
#include <windows.h>
//#include <winsock2.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#endif
//#include <sys/socket.h>
//#include <netdb.h>
//#include <netinet/in.h>
//#include <sys/param.h>
//#include <sys/uio.h>
//#include <unistd.h>
//#include <ctype.h>


#define DEFAULT_BUFLEN	512
#define BUF_LEN			256             /* バッファのサイズ */

//#define DEFAULT_PORT "27015"
#define DEFAULT_PORT	"21"

//#define BUF_LEN 256             /* バッファのサイズ */
//#define DEFAULT_PORT "21"
int debug_flg = 1;				/* -d オプションを?けると turn on する */
#pragma warning(disable : 4996)

/*-------------------------------------------------- * ソケットから1 */
char *read_line(SOCKET socket, char *p) {
	char *org_p = p;

	while (1) {
		if (recv(socket, p, 1,0 ) == 0) break;
		if (*p == '\n') break;
		p++;
	}
	*(++p) = '\0';
	return org_p;
}
/*-------------------------------------------------- * レスポンスを??する。^\d\d\d- ならもう1乖?? */
void read_response(SOCKET socket, char *p) {
	do {
		read_line(socket, p);
		if (debug_flg) {
			fprintf(stderr, "<-- %s", p);
		}
	} while (isdigit(p[0]) && isdigit(p[1]) && isdigit(p[2]) && p[3] == '-');
}
/*--------------------------------------------------
 * 回年されたソケット socket に矢机? p を流慨。 * 矢机? p の姜眉は \0 で terminate されている
 * ?妥がある
 */
void write_to_server(SOCKET socket, char *p) {
	if (debug_flg) {
		fprintf(stderr, "--> %s", p);
	}
	send( socket, p, strlen(p) , 0 );
}
void error(const char *message) {
	fprintf(stderr, message);
	exit(1);
}

/****************************************************************************************
*
****************************************************************************************/
int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	const char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	//*********************************************************
	char send_mesg [BUF_LEN];          /* サ?バに流るメッセ?ジ */
	char serverName[32];
	char user      [BUF_LEN];           /* ftp サ?バに流慨するユ?ザ? */
	char passwd    [BUF_LEN];             /* ftp サ?バに流慨するパスワ?ド */
	char buf	   [BUF_LEN];
	char path	   [BUF_LEN];
	SOCKET	data_socket;              /* デ?タ脱ソケット */
	SOCKET	data_waiting_socket;          /* デ?タコネクションの略ち?け脱ソケット */
	int		len;
	struct sockaddr_in		sin;
	short b1, b2;
	unsigned short	b;

	strcpy(serverName, "masami");
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {		printf("WSAStartup failed with error: %d\n", iResult);		return 1;	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	// Resolve the server address and port
	iResult = getaddrinfo(serverName, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,	ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());			WSACleanup();			return 1;		
		}
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);			ConnectSocket = INVALID_SOCKET;			continue;		
		}
		break;
	}
	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");		WSACleanup();		return 1;
	}

	read_response(ConnectSocket, buf);
												/* USER?PASS を流慨 */
	strcpy(user  , "masami");
	strcpy(passwd, "masami");
	strcpy(path, "TEST.txt");

	sprintf(send_mesg, "USER %s\n", user);
	write_to_server(ConnectSocket, send_mesg);
	read_response(ConnectSocket, buf);

	sprintf(send_mesg, "PASS %s\n", passwd);
	write_to_server(ConnectSocket, send_mesg);
	read_response(ConnectSocket, buf);


	for (int i = 0; i < 100000000; i++) {
		printf("--------------------------------------------------------------NO %d \n", i);
		data_waiting_socket = socket(AF_INET, SOCK_STREAM, 0);
		sin.sin_family = AF_INET;
		sin.sin_port = 0;
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(data_waiting_socket, (struct sockaddr *)&sin, sizeof sin) < 0) { error("bind failed.\n"); }
		if (listen(data_waiting_socket, SOMAXCONN) == -1) { error("listen failed.\n"); }
		{
			u_long local_ip;
			len = sizeof(sin);
			if (getsockname(ConnectSocket, (struct sockaddr *)&sin, &len) < 0) { error("getsockname failed.\n"); }
			local_ip = ntohl(sin.sin_addr.s_addr);

			if (getsockname(data_waiting_socket, (struct sockaddr *)&sin, &len) < 0) { error("getsockname failed.\n"); }

			if (i == 0) {
				b1 = (ntohs(sin.sin_port) >> 8) & 0xff;
				b2 = ntohs(sin.sin_port) & 0xff;
				b  = sin.sin_port;
			}
			sprintf(send_mesg, "PORT %d,%d,%d,%d,%d,%d\n",
				(int)(local_ip >> 24) & 0xff,
				(int)(local_ip >> 16) & 0xff,
				(int)(local_ip >> 8) & 0xff,
				(int)(local_ip) & 0xff,
				(ntohs(sin.sin_port) >> 8) & 0xff,
		 		ntohs(sin.sin_port) & 0xff);
//				b1 & 0xff,
//		 		b2 & 0xff);
			write_to_server(ConnectSocket, send_mesg);
			read_response(ConnectSocket, buf);


			sprintf(send_mesg, "STOR %s\n", path);
			write_to_server(ConnectSocket, send_mesg);
			read_response(ConnectSocket, buf);
		}
//		if (i != 0) {
//			sin.sin_port=b;
//			closesocket(data_socket);
//			printf("Sleep\n");
//			Sleep(10000);
//			continue;
//			break;
//		}
		len = sizeof(sin);
		data_socket = accept(data_waiting_socket, (struct sockaddr *)&sin, &len);
		if (data_socket == -1) { error("accept failed.\n"); }

		memset(send_mesg, 'A', BUF_LEN);
		send_mesg[253] = 0x0d;
		send_mesg[254] = 0x0a;
		send_mesg[255] = 0;
		write_to_server(data_socket, send_mesg);

		closesocket(data_socket);
		read_response(ConnectSocket, buf);


		Sleep(1000);
	}


//	close(data_waiting_socket);
	write_to_server(ConnectSocket, (char*)"QUIT\n");
	read_response(ConnectSocket, buf);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
#ifdef NOUSE
// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
//	iResult = shutdown(ConnectSocket, SD_SEND);
//	if (iResult == SOCKET_ERROR) {
//		printf("shutdown failed with error: %d\n", WSAGetLastError());		closesocket(ConnectSocket);		WSACleanup();		return 1;
//	}
	// Receive until the peer closes the connection
//	do {
//		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
//		if (iResult > 0)
//			printf("Bytes received: %d\n", iResult);
//		else if (iResult == 0)
//			printf("Connection closed\n");
//		else
//			printf("recv failed with error: %d\n", WSAGetLastError());
//
//	} while (iResult > 0);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
#endif

#ifdef NOUSE
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	int err;
	int command_socket;           /* コマンド脱ソケット */
	int data_socket;              /* デ?タ脱ソケット */
	int data_waiting_socket;          /* デ?タコネクションの略ち?け脱ソケット */
	struct hostent *servhost;         /* ホスト?とIPアドレスを?うための菇?? */
	struct sockaddr_in server;        /* ソケットを?うための菇?? */
	struct sockaddr_in sin;
	int len;
	int iResult;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	char send_mesg[BUF_LEN];          /* サ?バに流るメッセ?ジ */
	char user[BUF_LEN];           /* ftp サ?バに流慨するユ?ザ? */
	char passwd[BUF_LEN];             /* ftp サ?バに流慨するパスワ?ド */
	char host[BUF_LEN];               /* 儡?するホスト? */
	char path[BUF_LEN];               /* 妥?するパス */
	char buf[BUF_LEN];

	debug_flg = 1;
	strcpy( user    , "masami" );
	strcpy(passwd   , "masami");
//	strcpy(host, "192.168.99.103");
	strcpy(host, "masami");
	strcpy(path     , "TEST.txt");

//	servhost = gethostbyname(host);
//	printf("%d\n", errno);
//	if (servhost == NULL) {
//		fprintf(stderr, "Bad hostname [%s]\n", host);
//		exit(1);
//	}
//	servhost->h_addrtype
	memset( (char*)&server,0, sizeof(server));
	server.sin_family    = AF_INET;

//	memcpy( servhost->h_addr, (char *)&server.sin_addr, servhost->h_length);
	server.sin_addr.S_un.S_un_b.s_b1 = 192;
	server.sin_addr.S_un.S_un_b.s_b2 = 168;
	server.sin_addr.S_un.S_un_b.s_b3 = 99;
	server.sin_addr.S_un.S_un_b.s_b4 = 103;
	server.sin_port = 21;// (getservbyname("ftp", "tcp"))->s_port;
	command_socket  = socket(AF_INET, SOCK_STREAM, 0);
	connect(command_socket, (struct sockaddr *)&server, sizeof(server));
											/* welcome response を?? */
	read_response(command_socket, buf);
											/* USER?PASS を流慨 */
	sprintf(send_mesg, "USER %s\n", user);
	write_to_server(command_socket, send_mesg);
	read_response(command_socket, buf);

	sprintf(send_mesg, "PASS %s\n", passwd);
	write_to_server(command_socket, send_mesg);
	read_response(command_socket, buf);

	data_waiting_socket = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_family = AF_INET;
	sin.sin_port = 0;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind   (data_waiting_socket, (struct sockaddr *)&sin, sizeof sin) <   0) {		error("bind failed.\n");	}
	if (listen (data_waiting_socket, SOMAXCONN                          ) == -1) {		error("listen failed.\n");	}

	/* ----------------------------------------- */
	{
		u_long local_ip;
		len = sizeof(sin);
		if (getsockname(command_socket,			(struct sockaddr *)&sin, &len) < 0) {			error("getsockname failed.\n");		}
		local_ip = ntohl(sin.sin_addr.s_addr);

		if (getsockname(data_waiting_socket,(struct sockaddr *)&sin, &len) < 0) {		error("getsockname failed.\n");		}

		sprintf(send_mesg, "PORT %d,%d,%d,%d,%d,%d\n",
			(int)(local_ip >> 24) & 0xff,
			(int)(local_ip >> 16) & 0xff,
			(int)(local_ip >> 8) & 0xff,
			(int)(local_ip) & 0xff,
			(ntohs(sin.sin_port) >> 8) & 0xff,
			ntohs(sin.sin_port) & 0xff);
		write_to_server(command_socket, send_mesg);
		read_response(command_socket, buf);

		sprintf(send_mesg, "STOR %s\n", path);
		write_to_server(command_socket, send_mesg);
	}

	len = sizeof(sin);
	data_socket = accept(data_waiting_socket, (struct sockaddr *)&sin, &len);
	if (data_socket == -1) {		error("accept failed.\n");	}

	while (1) {
		int read_size;
		read_size = read(data_socket, buf, BUF_LEN);
		if (read_size > 0) {
			write(1, buf, read_size);
		}else {
			break;
		}
	}
	read_response(command_socket, buf);
	read_response(command_socket, buf);

	write_to_server(command_socket, (char*)"QUIT\n");
	read_response(command_socket, buf);

	close(data_waiting_socket);
	close(command_socket);

	return 0;
}

#endif