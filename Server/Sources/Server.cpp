#include "Server.h"

Server::Server() : UserBase{DataBase<User>("UserBase.txt")}, 
		   MessageBase{DataBase<Message>("MessageBase.txt")}, dataSize(0)
{

	if(this->listen(QHostAddress::Any, 55555)) {
		cout << "Server started" << endl;	
	} else {
		cout << "Error with starting server" << endl;
	}		
}

Server::~Server()
{ }

void Server::sendCode(quint8 code, TcpSocket* socket)
{
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);
	out << code;
	socket->write(Data);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
	currSocket = new TcpSocket(&Sockets);
	currSocket->setSocketDescriptor(socketDescriptor);
	connect(currSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
	connect(currSocket, SIGNAL(disconnected()), currSocket, SLOT(deleteSelf()));

	Sockets.push_back(currSocket);

	sendCode(OK_CODE, currSocket);
	cout << "Client connected" << endl;
}

void Server::SendToClient(Message msg)
{
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);

	out << quint16(0) << msg.time << msg.userName << msg.text;
	out.device()->seek(0);
	out << quint16(Data.size() - sizeof(quint16));

	for(TcpSocket* socket : Sockets)
		socket->write(Data);
}

void Server::slotReadyRead()
{
	currSocket = static_cast<TcpSocket*>(sender());
	Message msg;
	QString name, passHash;
	QDataStream in(currSocket);
	in.setVersion(QDataStream::Qt_5_2);
	dataSize = 0;

	quint16 code {0};

	if(in.status() == QDataStream::Ok) {
		cout << "reading data..." << endl;		

		for(;;) {

			if(dataSize == 0) {
				if(currSocket->bytesAvailable() < 4) {
					cout << "dataSize reading error..." << endl;
					break;		
				}
				in >> dataSize;	
			}

			if(currSocket->bytesAvailable() < dataSize) break;

			in >> code;

			if(code == MESSAGE_CODE) {
				in >> msg.time >> msg.userName >> msg.text;
				SendToClient(msg);
				qDebug() << "msg: " << msg.formated();
			} else if(code == LOGIN_CODE) {
				in >> name >> passHash;
				if(login(name, passHash) == true)
					sendCode(OK_LOGIN_CODE, currSocket);
				else
					sendCode(WRONG_LOGIN_CODE, currSocket);
			} else if(code == AUTH_CODE) {
				in >> name >> passHash;
				if(auth(name, passHash) == true)
					sendCode(OK_AUTH_CODE, currSocket);
				else
					sendCode(WRONG_AUTH_CODE, currSocket);
			}

		}

		cout << "reading finished" << endl;
	}
	else cout << "data reading error..." << endl;
}

bool Server::login(QString name, QString passHash)
{
	User newUser;
	std::string s = name.toUtf8().constData();
	memcpy(newUser.name, s.c_str(), strlen(s.c_str()));
	newUser.passHash = std::stoll(passHash.toUtf8().constData());

	if(UserBase.findObj(&newUser) == true) return false;

	UserBase.writeObj(&newUser);
	qDebug() << name << QString(" logined sucessfully");	

	return true;
}

bool Server::auth(QString name, QString passHash)
{
	User newUser;
	std::string s = name.toUtf8().constData();
	memcpy(newUser.name, s.c_str(), strlen(s.c_str()));
	newUser.passHash = std::stoll(passHash.toUtf8().constData());
	return UserBase.findObj(&newUser);
}





