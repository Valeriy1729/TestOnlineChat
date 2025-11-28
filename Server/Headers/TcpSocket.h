#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;


class TcpSocket : public QTcpSocket {
	Q_OBJECT
	vector<TcpSocket*>* pSockets;
public:
	TcpSocket(vector<TcpSocket*>* _pSocket);
	~TcpSocket();
public slots:
	void deleteSelf();
};


#endif
