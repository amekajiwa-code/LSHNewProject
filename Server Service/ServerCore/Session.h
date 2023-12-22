#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

/*���漱��*/
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
	/*Disconnect: ���� ������ ���� ���ڷ� ����*/
	void Disconnect(const WCHAR* cause); 
	shared_ptr<Service> GetService() { return _service.lock(); }
	void SetService(shared_ptr<Service> service) { _service = service; }
public:
	/* ���� ���� */
	void		SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress	GetAddress() { return _netAddress; }
	SOCKET		GetSocket() { return _socket; }
	//IsConnected:������ִ��� Ȯ���ؼ� ��Ŷ�� ��� Recv���ٰ����� ���θ� ��ȯ
	bool IsConnected() { return _connected; }
	//GetSessionRef: SessionRef�� ��ȯ�ϱ� ���� ĳ������ ����
	SessionRef GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

public:
	/* �������̽� ���� */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;
private:
	/* ���� ���� */
	/*Register�� �Ϸ�Ǹ� Process���� �˷���*/
	void RegisterConnect();
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);
	/*Error�ڵ带 ó���ϴ� �Լ�*/
	void HandleError(int32 errorCode);

protected:
	/* ������ �ڵ忡�� �����ε� ����� �Լ�*/
	virtual void OnConnected() {}
	virtual int32 OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void OnSend(int32 len) { }
	virtual void OnDisconnected() { }
public:
	// TEMP
	char _recvBuffer[1000];

private:
	/*���������ε� ������ ��Ȳ�� �˱����� service �����ͷ� ����ֱ�*/
	weak_ptr<Service>	_service;
	SOCKET				_socket = INVALID_SOCKET;
	NetAddress			_netAddress = {};
	Atomic<bool>		_connected = false;
private:
	USE_LOCK;

	/*���� ����*/

	/* �۽� ����*/

	/* IocpEvent ������ �ϱ� ���� ���*/
	RecvEvent _recvEvent;
};

