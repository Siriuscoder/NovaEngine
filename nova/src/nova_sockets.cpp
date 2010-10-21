/* The MIT License:

Copyright (c) 2008 Ivan Gagis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

// (c) Ivan Gagis 2008
// e-mail: igagis@gmail.com

// Description:
//          cross platfrom C++ Sockets wrapper
//

/*
Большое спасибо Ивану что предоставил мне свой не сложный сетевой враппер. Я немного его переделал. 
Copyright (c) 2008 Ivan Gagis, SIRIUS
*/
#include "nova_stable_precompiled_headers.h"

#include "nova_sockets.h"

//system specific defines, typedefs and includes
#ifdef WIN_BUILD

#define M_INVALID_SOCKET INVALID_SOCKET
#define M_SOCKET_ERROR SOCKET_ERROR
#define M_EINTR WSAEINTR
#define M_FD_SETSIZE FD_SETSIZE

typedef int socklen_t;

#else //assume linux/unix

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#define M_INVALID_SOCKET (-1)
#define M_SOCKET_ERROR (-1)
#define M_EINTR EINTR
#define M_FD_SETSIZE FD_SETSIZE

#endif


namespace nova {

CSockEnviroment * CSockEnviroment::instance = NULL;

CIPAddress::CIPAddress() : host(INADDR_ANY), port(0), CBase((char*)"CIPAddress") {}

CIPAddress::CIPAddress(uint h, nUInt16 p) : host(h), port(p), CBase((char*)"CIPAddress") {}

CIPAddress::CIPAddress(byte h1, byte h2, byte h3, byte h4, nUInt16 p) :
	host(uint(h1) + (uint(h2)<<8) + (uint(h3)<<16) + (uint(h4)<<24)), 
		port(p), CBase((char*)"IPAddress") {}

CIPAddress::CIPAddress(cchar * ip, nUInt16 p) : 
	host(CIPAddress::ParseString(ip)), port(p), CBase((char*)"CIPAddress") {}

bool CIPAddress::operator==(const CIPAddress & ip)
{
	return (this->host == ip.host) && (this->port == ip.port);
}

uint CIPAddress::ParseString(cchar * ip) throw(nova::NovaExp)
{
    if(!ip)
        throw NovaExp("IPAddress::ParseString(): pointer passed as argument is 0", MEM_ERROR);
    
    //subfunctions
    struct sf
	{
        static void ThrowInvalidIP(){
            throw NovaExp("IPAddress::ParseString(): string is not a valid IP address", BAD_OPERATION);
        };
    };
    
    uint h = 0;//parsed host
    cchar *curp = ip;
    for(uint t = 0; t < 4; ++t)
	{
        uint dights[3];
        uint numDgts;
        for(numDgts = 0; numDgts < 3; ++numDgts)
		{
            if( *curp == '.' || *curp == 0 )
			{
                if(numDgts==0)
                    sf::ThrowInvalidIP();
                break;
            }
			else
			{
                if(*curp < '0' || *curp > '9')
                    sf::ThrowInvalidIP();
                dights[numDgts] = uint(*curp) - uint('0');
            }
            ++curp;
        }
        
        if(t < 3 && *curp != '.')//unexpected delimiter or unexpected end of string
            sf::ThrowInvalidIP();
        else if(t == 3 && *curp != 0)
            sf::ThrowInvalidIP();
        
        uint xxx = 0;
        for(uint i = 0; i < numDgts; ++i)
		{
            uint ord = 1;
            for(uint j = 1; j < numDgts-i; ++j)
               ord *= 10;
            xxx+=dights[i]*ord;
        }
        if(xxx > 255)
            sf::ThrowInvalidIP();
        
        h |= (xxx << (8*t));
        
        ++curp;
    }
    return h;
}

void CSockEnviroment::InitSockets()	throw(nova::NovaExp)
{
#ifdef WIN_BUILD
    nUInt16 versionWanted = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(versionWanted, &wsaData) != 0 )
        throw NovaExp("InitSockets(): Winsock 2.2 initialization failed", BAD_OPERATION);
#else //assume linux/unix
    // SIGPIPE is generated when a remote socket is closed
    void (*handler)(int);
    handler = signal(SIGPIPE, SIG_IGN);
    if(handler != SIG_DFL)
        signal(SIGPIPE, handler);
#endif
}

//static
void CSockEnviroment::DeinitSockets()
{
#ifdef WIN_BUILD
    // Clean up windows networking
    if(WSACleanup() == M_SOCKET_ERROR)
        if(WSAGetLastError() == WSAEINPROGRESS)
		{
            WSACancelBlockingCall();
            WSACleanup();
        }
#else //assume linux/unix
    // Restore the SIGPIPE handler
    void (*handler)(int);
    handler = signal(SIGPIPE, SIG_DFL);
    if(handler != SIG_IGN)
        signal(SIGPIPE, handler);
#endif
}

CIPAddress CSockEnviroment::GetHostByName(cchar * hostName, nUInt16 port)
{
    if(!hostName)
        throw NovaExp("Sockets::GetHostByName(): pointer passed as argument is 0", MEM_ERROR);
    
    CIPAddress addr;
    addr.host = inet_addr(hostName);
    if(addr.host == INADDR_NONE){
        struct hostent *hp;
        hp = gethostbyname(hostName);
        if(hp)
            memcpy(&(addr.host), hp->h_addr, sizeof(addr.host)/* hp->h_length */);
        else
            throw NovaExp("Sockets::GetHostByName(): gethostbyname() failed", BAD_OPERATION);
    }
    addr.port = port;
    return addr;
}

CSockEnviroment & CSockEnviroment::Inst()
{
	if(!CSockEnviroment::instance)
		throw NovaExp("Sockets::Inst(): singletone object is not created", MEM_ERROR);
	return *CSockEnviroment::instance;
}

CSockEnviroment::CSockEnviroment() throw(nova::NovaExp) : CBase((char*)"CSockEnviroment")
{
    if(CSockEnviroment::instance != NULL)
        throw NovaExp("CSockEnviroment singletone object is already created", MEM_ERROR);
    CSockEnviroment::InitSockets();
    this->instance = this;
}

CSockEnviroment::~CSockEnviroment()
{
    //this->instance should not be null here
    CSockEnviroment::DeinitSockets();
    this->instance = NULL;
}

CSocket::CSocket() : isReady(false), CBase((char*)"CSocket")
{
    this->socket = M_INVALID_SOCKET;
}

CSocket::CSocket(char * _name) : isReady(false), CBase(_name)
{
	this->socket = M_INVALID_SOCKET;
}

bool CSocket::IsValid() const
{
    return (this->socket != M_INVALID_SOCKET);
}

CSocket & CSocket::operator = (const CSocket & s)
{
    this->~CSocket();
    this->socket = s.socket;
    this->isReady = s.isReady;
    const_cast<CSocket&>(s).socket = M_INVALID_SOCKET;//same as std::auto_ptr
    return *this;
}

CSocket::~CSocket()
{
	this->Close();
}

inline bool CSocket::IsReady() const
{
	return this->isReady;
}

void CSocket::Close()
{
    if(this->IsValid())
	{
#ifdef WIN_BUILD
        //Closing socket in Win32.
        //refer to http://tangentsoft.net/wskfaq/newbie.html#howclose for details
        shutdown(this->socket, SD_BOTH);
        closesocket(this->socket);
#else //assume linux/unix
        close(this->socket);
#endif
    }
    this->isReady = false;
    this->socket = M_INVALID_SOCKET;
}

CTCPSocket::CTCPSocket(const CTCPSocket & s)
{
//NOTE: that operator= calls destructor, so this->socket should be invalid, base class constructor takes care about it.
	this->operator=(s);//same as auto_ptr
}

CTCPSocket::CTCPSocket(const CIPAddress & ip, bool disableNaggle) 
	throw(nova::NovaExp) : CSocket((char*)"CTCPSocket")
{
    this->Open(ip, disableNaggle);
}

CTCPSocket & CTCPSocket::operator = (const CTCPSocket & s)
{
    this->CSocket::operator=(s);
    return *this;
}

/* Open a TCP network socket.
   A TCP connection to the remote host and port is attempted.
*/
void CTCPSocket::Open(const CIPAddress & ip, bool disableNaggle) throw(nova::NovaExp)
{
    if(this->IsValid())
        throw NovaExp("CTCPSocket::Open(): socket already opened", BAD_OPERATION);
    
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if(this->socket == M_INVALID_SOCKET)
        throw NovaExp("CTCPSocket::Open(): Couldn't create socket", BAD_OPERATION);

	ulong dwRet = 1;
	setsockopt(this->socket, SOL_SOCKET, SO_KEEPALIVE, reinterpret_cast<char *>(&dwRet),sizeof(ulong));
    
    //Connecting to remote host
    
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = ip.host;
    sockAddr.sin_port = htons(ip.port);

    // Connect to the remote host
    if(connect(this->socket, reinterpret_cast<sockaddr *>(&sockAddr), sizeof(sockAddr)) == M_SOCKET_ERROR)
	{
        this->Close();
        throw NovaExp("CTCPSocket::Open(): Couldn't connect to remote host", BAD_OPERATION);
    }
    
    //Disable Naggle algorithm if required
    if(disableNaggle)
        this->DisableNaggle();
    
    this->isReady = false;
}

uint CTCPSocket::Send(const byte * data, uint size) 
{
    if(!this->IsValid())
        throw NovaExp("CTCPSocket::Send(): socket is not opened", BAD_OPERATION);
    
    int sent = 0, left = int(size);
    
    //Keep sending data until it's sent or an error occurs
    int errorCode = 0;

    int res;
    do
	{
        res = send(this->socket, reinterpret_cast<const char *>(data), left, 0);
        if(res < 0)
		{
#ifdef WIN_BUILD
            errorCode = WSAGetLastError();
#else //linux/unix
            errorCode = errno;
#endif
			break;
        }
		else
		{
            sent += res;
            left -= res;
            data += res;
        }
    }
	while( (left > 0) && ((res != M_SOCKET_ERROR) || (errorCode == M_EINTR)) );
    
    if(res < 0)
        sent = res;
    
    return uint(sent);
}

uint CTCPSocket::Recv(byte * buf, uint maxSize) 
{
    if(!this->IsValid())
        throw NovaExp("CTCPSocket::Send(): socket is not opened", BAD_OPERATION);
    
    int len;
    int errorCode = 0;
    
	while (maxSize > 0) 
	{
		len = recv(this->socket, reinterpret_cast<char *>(buf), maxSize, 0);
		if(len < 0)
		{
			
#ifdef WIN_BUILD
            errorCode = WSAGetLastError();
#else //linux/unix
            errorCode = errno;
#endif
			break;
		} 
		else 
		{
			maxSize -= len;
			buf += len;
		}
    }
        

    this->isReady = false;
	return uint(len);
}

void CTCPSocket::DisableNaggle() throw(nova::NovaExp)
{
    if(!this->IsValid())
        throw NovaExp("CTCPSocket::DisableNaggle(): socket is not opened", BAD_OPERATION);
    
    int yes = 1;
    setsockopt(this->socket, IPPROTO_TCP, TCP_NODELAY, (char *)&yes, sizeof(yes));
}

CTCPServerSocket::CTCPServerSocket() : disableNaggle(false), CSocket((char*)"CTCPServerSocket") {}
CTCPServerSocket::CTCPServerSocket(const CTCPServerSocket& s) : disableNaggle(s.disableNaggle)
{
	//NOTE: that operator= calls destructor, so this->socket should be invalid, base class constructor takes care about it.
	this->operator=(s);//same as auto_ptr
}

CTCPServerSocket & CTCPServerSocket::operator=(const CTCPServerSocket & s)
{
	this->CSocket::operator=(s);
	return *this;
}

CTCPServerSocket::CTCPServerSocket(nUInt16 port, bool disableNaggle) : CSocket((char*)"CTCPServerSocket")
{
    this->Open(port, disableNaggle);
}

void CTCPServerSocket::Open(nUInt16 port, bool disableNaggle) 
{
    if(this->IsValid())
		throw NOVA_EXP("TCPServerSocket::Open(): socket already opened", BAD_OPERATION);
    
    this->disableNaggle = disableNaggle;
    
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if(this->socket == M_INVALID_SOCKET)
        throw NOVA_EXP("TCPServerSocket::Open(): Couldn't create socket", BAD_OPERATION);
    
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    sockAddr.sin_port = htons(port);

    // allow local address reuse
    {
        int yes = 1;
        setsockopt(this->socket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes));
    }

    // Bind the socket for listening
    if(bind(this->socket, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) == M_SOCKET_ERROR)
	{
        this->Close();
        throw NOVA_EXP("TCPServerSocket::Open(): Couldn't bind to local port", BAD_OPERATION);
    }

    if(listen(this->socket, 5) == M_SOCKET_ERROR)
	{
        this->Close();
        throw NOVA_EXP("TCPServerSocket::Open(): Couldn't listen to local port", BAD_OPERATION);
    }

    //Set the socket to non-blocking mode for accept()
#if defined(__BEOS__) && defined(SO_NONBLOCK)
    // On BeOS r5 there is O_NONBLOCK but it's for files only
    {
        long b = 1;
        setsockopt(this->socket, SOL_SOCKET, SO_NONBLOCK, &b, sizeof(b));
    }
#elif defined(O_NONBLOCK)
    {
        fcntl(this->socket, F_SETFL, O_NONBLOCK);
    }
#elif defined(WIN32)
    {
        ulong mode = 1;
        ioctlsocket(this->socket, FIONBIO, (u_long *)&mode);
    }
#elif defined(__OS2__)
    {
        int dontblock = 1;
        ioctl(this->socket, FIONBIO, &dontblock);
    }
#else
#warning How do we set non-blocking mode on other operating systems?
#endif
}

CTCPSocket CTCPServerSocket::Accept()
{
    if(!this->IsValid())
        throw NOVA_EXP("TCPServerSocket::Accept(): the socket is not opened", BAD_OPERATION);
    
    this->isReady = false;
    
    struct sockaddr_in sockAddr;
    int sock_alen = sizeof(sockAddr);
    CTCPSocket sock;//allocate a new socket object
    
    sock.socket = accept(this->socket, reinterpret_cast<sockaddr*>(&sockAddr),
		reinterpret_cast<socklen_t*>(&sock_alen));
    
    if(sock.socket == M_INVALID_SOCKET)
        return sock;//no connections to be accepted, return invalid socket
    
    //set blocking mode
#ifdef  WIN_BUILD
    {
        /* passing a zero value, socket mode set to block on */
        ulong mode = 0;
        ioctlsocket(sock.socket, FIONBIO, (u_long *)&mode);
    }
#elif defined(O_NONBLOCK)
    {
        int flags = fcntl(sock.socket, F_GETFL, 0);
        fcntl(sock.socket, F_SETFL, flags & ~O_NONBLOCK);
    }
#else
#error do not know how to set blocking mode to socket
#endif //#ifdef WIN32
    
    if(this->disableNaggle)
        sock.DisableNaggle();
    
    return sock;//return a newly created socket
}

CUDPSocket::CUDPSocket() : CSocket((char*)"CUDPSocket") {}
    
CUDPSocket::~CUDPSocket()
{
	this->Close();
}

void CUDPSocket::Open(nUInt16 port)
{
	if(this->IsValid())
        throw NOVA_EXP("UDPSocket::Open(): the socket is already opened", BAD_OPERATION);
    
    this->socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if(this->socket == M_INVALID_SOCKET)
		throw NOVA_EXP("UDPSocket::Open(): ::socket() failed", BAD_OPERATION);
    
    /* Bind locally, if appropriate */
    if(port != 0)
	{
        struct sockaddr_in sockAddr;
        memset(&sockAddr, 0, sizeof(sockAddr));
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_addr.s_addr = INADDR_ANY;
        sockAddr.sin_port = htons(port);
        
        // Bind the socket for listening
        if(bind(this->socket, reinterpret_cast<struct sockaddr*>(&sockAddr), sizeof(sockAddr)) == M_SOCKET_ERROR)
		{
            this->Close();
            throw NOVA_EXP("UDPSocket::Open(): could not bind to local port", BAD_OPERATION);
        }
    }
#ifdef SO_BROADCAST
    //Allow LAN broadcasts with the socket
    {
        int yes = 1;
        setsockopt(this->socket, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));
    }
#endif
    
    this->isReady = false;
}


void CUDPSocket::Open()
{
	this->Open(0);
}

uint CUDPSocket::Send(const byte * buf, nUInt16 size, CIPAddress destinationIP) 
{
    sockaddr_in sockAddr;
    int sockLen = sizeof(sockAddr);
    
    sockAddr.sin_addr.s_addr = destinationIP.host;
    sockAddr.sin_port = destinationIP.port;
    sockAddr.sin_family = AF_INET;
    int res = sendto(this->socket, reinterpret_cast<const char*>(buf), size, 0, reinterpret_cast<struct sockaddr*>(&sockAddr), sockLen);
    
    return res;
}

uint CUDPSocket::Recv(byte * buf, nUInt16 maxSize, CIPAddress & out_SenderIP)
{
    sockaddr_in sockAddr;
    int sockLen = sizeof(sockAddr);
    
    int res = recvfrom(this->socket, reinterpret_cast<char*>(buf), maxSize, 0, 
		reinterpret_cast<sockaddr*>(&sockAddr), reinterpret_cast<socklen_t*>(&sockLen));
    
    out_SenderIP.host = ntohl(sockAddr.sin_addr.s_addr);
    out_SenderIP.port = ntohs(sockAddr.sin_port);
    return res;
}

}
