#include "TcpSocket.h"

TcpSocket::TcpSocket(vector<TcpSocket*>* _pSockets)
{
	pSockets = _pSockets;
}

TcpSocket::~TcpSocket()
{ }

void TcpSocket::deleteSelf()
{
	for(size_t i {0}; i < pSockets->size(); ++i) {
		if((*pSockets)[i] == this) {
			(*pSockets)[i] = (*pSockets)[pSockets->size() - 1];
			pSockets->pop_back();
			break;
		}	
	}	
	cout << "User disconnected && Socket deleted" << endl;
}

