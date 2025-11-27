#include "Server.h"

Server::Server()
{
	if(this->listen(QHostAddress::Any, 55555)) {
		cout << "Server started" << endl;	
	} else {
		cout << "Error with starting server" << endl;
	}		
}

Server::~Server()
{ }

void Server::incomingConnection(qintptr socketDescriptor)
{
	currSocket = new QTcpSocket;
	currSocket->setSocketDescriptor(socketDescriptor);
	connect(currSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
	connect(currSocket, SIGNAL(disconnected()), currSocket, SLOT(deleteLater()));

	Sockets.push_back(currSocket);
	cout << "Client connected" << endl;
}

void Server::SendToClient(QString msg)
{
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);

	out << msg;

	for(QTcpSocket* socket : Sockets)
		socket->write(Data);
}

void Server::slotReadyRead()
{
	currSocket = (QTcpSocket*)sender();
	QDataStream in(currSocket);
	in.setVersion(QDataStream::Qt_5_2);

	if(in.status() != QDataStream::Ok) {
		cout << "data reading error" << endl;	
		return;
	}

	cout << "reading..." << endl;
	QString msg;
	in >> msg;
	qDebug() << msg;

	SendToClient(msg);
}





