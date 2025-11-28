#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <QDebug>
#include <QThread>
#include <iostream>
#include <vector>
#include <string.h>
#include "msg.h"
#include "TcpSocket.h"
#include "User.h"
#include "DataBase.h"

#define OK_CODE			quint8(111)
#define WRONG_AUTH_CODE		quint8(31)
#define OK_AUTH_CODE		quint8(17)
#define OK_LOGIN_CODE		quint8(16)
#define WRONG_LOGIN_CODE	quint8(15)

#define LOGIN_CODE		quint16(65535)
#define AUTH_CODE		quint16(65533)
#define MESSAGE_CODE		quint16(65534)
#define CHECK_DELAY	5

using std::vector;
using std::cout;
using std::endl;


class Server : public QTcpServer
{ 
	Q_OBJECT

	DataBase<User> UserBase;
	DataBase<Message> MessageBase;
	TcpSocket* currSocket;
	vector<TcpSocket*> Sockets;
	QByteArray Data;
	quint16 dataSize;
	bool _connectionFlag;

	void SendToClient(Message msg);
	void sendCode(quint8 code, TcpSocket* socket);
	void sendPrevMessages(TcpSocket* socket);
	bool login(QString name, QString passHash);
	bool auth(QString name, QString passHash);

public:
	Server();
	~Server();

public slots:
	void incomingConnection(qintptr socketDescriptor);
	void slotReadyRead();
};


#endif
