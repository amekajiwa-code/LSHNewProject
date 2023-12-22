#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

class Service;

/*--------------
	Session
---------------*/

class Session : public IocpObject
{
	/*GameServer���� ��Ʈ��ũ�� ���õ�
	�Լ����� private���ִ°� �����ϱ�
	�� �ʿ��� �ֵ鸸 �����Ҽ� �ֵ��� friendŬ���� ���ش�.*/
	friend class Listener;
	friend class IocpCore;
	friend class Service;

public:
	Session();
	virtual ~Session();

public:
	/* �ܺο��� ��� */
	void Send(BYTE* buffer, int32 len);
	bool Connect();

	/*Disconnect: ���� ������ ���� ���ڷ� ����*/
	void				Disconnect(const WCHAR* cause);

	shared_ptr<Service>	GetService() { return _service.lock(); }
	void				SetService(shared_ptr<Service> service) { _service = service; }

public:
						/* ���� ���� */
	void				SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress			GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	//IsConnected:������ִ��� Ȯ���ؼ� ��Ŷ�� ��� Recv���ٰ����� ���θ� ��ȯ
	bool				IsConnected() { return _connected; }
	//GetSessionRef: SessionRef�� ��ȯ�ϱ� ���� ĳ������ ����
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

private:
						/* �������̽� ���� */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
						/* ���� ���� */
	/*Register�� �Ϸ�Ǹ� Process���� �˷���*/
	bool				RegisterConnect();
	bool				RegisterDisconnect();
	void				RegisterRecv();
	void				RegisterSend(SendEvent* sendEvent);

	void				ProcessConnect();
	void				ProcessDisconnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(SendEvent* sendEvent, int32 numOfBytes);
	/*Error�ڵ带 ó���ϴ� �Լ�*/
	void				HandleError(int32 errorCode);

protected:
						/* ������ �ڵ忡�� ������ */
	virtual void		OnConnected() { }
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) { }
	virtual void		OnDisconnected() { }

public:
	// TEMP
	BYTE _recvBuffer[1000];
	// Circular Buffer
	BYTE _sendBuffer[1000];
	int32 _sendLen = 0;

private:
	/*���������ε� ������ ��Ȳ�� �˱����� service �����ͷ� ����ֱ�*/
	weak_ptr<Service>	_service;
	SOCKET				_socket = INVALID_SOCKET;
	NetAddress			_netAddress = {};
	Atomic<bool>		_connected = false;

private:
	USE_LOCK;

	/* ���� ���� */

	/* �۽� ���� */

private:
	/*IocpEvent ������ �ϱ� ���� ��� */
	ConnectEvent		_connectEvent;
	DisconnectEvent		_disconnectEvent;
	RecvEvent			_recvEvent;
};

