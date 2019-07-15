#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>


using namespace std;

bool check_exit(const char *msg)
{
	for (int i = 0; i < strlen(msg); ++i)
	{
		if (msg[i] == '#')
			return true;
	}
	return false;
}

int main(int argc, char *argv[])
{

	if (argc!=2)
	{
		cout<<"Please pass port number!"<<endl;
		return 0;
	}

	int client, server;

	int portnum = stoi(argv[1]);

	int bufsize = 1024;

	bool isExit = false;

	char buffer[bufsize];



	struct sockaddr_in serv_addr;									// This is a structure describing the internet socket address

	client = socket(AF_INET, SOCK_STREAM, 0);						// Create a new socket. (AF_INET = internet domain, SOCK_STREAM = socket of type stream, 0 = operating system will choose the protocol )
	if (client < 0)
	{
		cout << "\nERROR establishing socket...";
		exit(1);
	}

	cout << "\n--> Socket server created..\n";					 	// If all of the above passes, the socket is created

	serv_addr.sin_port = htons(portnum);							// port number converted to network byte order. htons() converts port number in host byte order to a port number in network byte order
	serv_addr.sin_family = AF_INET;									// Code for address family
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);					// IP address of the machine on which server is running. INADDR_ANY gets this address


	if (bind(client, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)		// This binds socket to an address
	{
		cout << "--> ERROR binding connection ...\n";
		return -1;
	}


	socklen_t size;
	size = sizeof(serv_addr);

	cout << "--> Looking for clients..." << endl;
	listen(client, 1);												// Listen on the socket for connections.
																	// 1 = the number of connections that can be waiting while the process is handling a particular connection. Max is typically 5 in most systems
																	// If the first argument is a valid socket, this call cannot fail, and so the code doesn't check for errors

	server = accept(client, (struct sockaddr*)&serv_addr, &size);	//  Blocks process until a client connects to the server. Wakes up the process when a connection from a client has been successfully established
																	//  It returns a new file descriptor, and all communication on this connection should be done using the new file descriptor("server" here).
	if (server < 0)
        cout << "--> Error on accepting..." << endl;


	while (server > 0)
	{
		strcpy(buffer, "--> Server connected...\n");
		send(server, buffer, bufsize, 0);							// Sends bufsize bytes from *buffer to socket - server

		cout << "--> Connected to the client" << endl;
		cout << "\nEnter # to end the connection\n\n" << endl;


		cout << "Client: ";
		recv(server, buffer, bufsize, 0);							// Reveives bufsize bytes from socket - server into *buffer.
		cout << buffer << endl;

		isExit = check_exit(buffer);


		while (!isExit)
		{
			memset(buffer, 0, bufsize);								// Reset buffer
			cout << "Server: ";
			cin.getline(buffer, bufsize);

			send(server, buffer, bufsize, 0);

			if (check_exit(buffer))
				break;
		}

		cout << "\nDisconnected..." << endl;
		isExit = false;
		exit(1);
	}

	return 0;
}
