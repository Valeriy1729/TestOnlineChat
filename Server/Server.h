#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <QDebug>
#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;


class Server : public QTcpServer
{ 
	Q_OBJECT

	QTcpSocket* currSocket;
	vector<QTcpSocket*> Sockets;
	QByteArray Data;
	void SendToClient(QString msg);

public:
	Server();
	~Server();

public slots:
	void incomingConnection(qintptr socketDescriptor);
	void slotReadyRead();
};


#endif
