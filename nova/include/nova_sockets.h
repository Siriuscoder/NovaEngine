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
THE SOFTWARE. 
*/

// (c) Ivan Gagis 2008
// e-mail: igagis@gmail.com

// Description:
//          cross platfrom C++ Sockets wrapper
//
#pragma once

/// \file nova_sockets.h
/// \brief Сетевой функционал
///
/// Тут у нас несколько классов, которые помогу нам для общения с сетью.
/// Эти классы составляют основу работы любых серверов и клиентов.
/// Copyright (c) 2008 Ivan Gagis, Sirius


/*
Большое спасибо Ивану, что предоставил мне свой не сложный сетевой враппер. Я немного его доработал. 
Copyright (c) 2008 Ivan Gagis, SIRIUS
*/
#include "nova_error.h"

#ifdef WIN_BUILD
typedef SOCKET T_Socket;
#else
typedef int T_Socket;
#endif

#define ERR_DISCONNECTED	-1
#define ERR_SOCK_SYSTEM		-2

namespace nova {


#if !defined(DOXYGEN_IGNORE)
class NOVA_EXPORT CTCPServerSocket;
#endif

/// \class CIPAddress
/// \brief IP адрес
class NOVA_EXPORT CIPAddress : public CBase
{
private:
    //parse IP address from string
	static uint ParseString(cchar * ip) throw(nova::NovaExp);

public:
/// \brief IP адрес
///
/// Хранится в целочисленной переменной 4 байта. (172.10.0.1)
    uint host;
/// \brief Порт
///
/// Хранится в целочисленной переменной 2 байта. 
    word port;
    
/// \brief Конструктор
    CIPAddress();
    
/// \brief Конструктор
///
/// Создает некий (узел сети) по 
/// \param h Ай пи адрес 
/// \param p порт
    CIPAddress(uint h, word p);
    
/// \brief Конструктор
///
/// Создает некий (узел сети) по 
/// \param h1, h2, h3, h4 Байтовые составляющие ай пи адреса 
/// \param p порт
    CIPAddress(byte h1, byte h2, byte h3, byte h4, word p);
    
/// \brief Конструктор
///
/// Создает некий (узел сети) по 
/// \param ip Ай пи адрес в строковой записи "127.0.0.1"
/// \param p порт
    CIPAddress(cchar* ip, word p);
    
/// \brief Оператор равенства
///
/// Проверяет равны ли узлы или нет.
    bool operator == (const CIPAddress & ip);
};

/// \class CSockEnviroment
/// \brief Класс отвечающий за внешнее сетевое окружение
///
/// Зачем же он нужен.. Все просто перед использованием сокетов
/// (это я еще опишу, когда дойду до игрового сервера)
/// вам не обходимо создать объект этого класса, так как инициализация
/// сокетов происходит в его конструкторе, ну и деинициализация в деструкторе.
class NOVA_EXPORT CSockEnviroment : public CBase
{
private:
    static void InitSockets() throw(nova::NovaExp);
    static void DeinitSockets();
    static CSockEnviroment *instance;

public:
/// \brief Конструктор
	CSockEnviroment() throw(nova::NovaExp);
/// \brief Конструктор
    ~CSockEnviroment();
    
/// \brief Cылка на этот объект этого класса
///
/// Вообще говоря у нас в программе может быть только один объект такого класса
/// при попытке создать еще один, у вас ничего не выйдет..
    static CSockEnviroment & Inst();
    
/// \brief Взять численный айпи адрес по имени хоста. 
///
/// Работа с DNS.
/// \param hostName имя хоста (ya.ru, google.ru)
/// \param port нужный порт, он встанет в результат
/// \return \see CIPAddress
    CIPAddress GetHostByName(cchar * hostName, word port);
};

/// \class CSocket
/// \brief Класс реализующий базовый сокет
class NOVA_EXPORT CSocket : public CBase
{    
public:
    //this type will hold system specific socket handle.
    //this buffer should be large enough to hold socket handle in different systems.
    //sizeof(int) looks enough so far.
#ifndef DOC_DONT_EXTRACT //direction to doxygen not to extract this class
    struct SystemIndependentSocketHandle
	{
        byte buffer[sizeof(int)];
    };
#endif//~DOC_DONT_EXTRACT

protected:
/// \brief Флаг готовности к передаче.
    bool isReady;
    
/// \brief Идентификатор сокета
    T_Socket socket;
    
/// \brief Конструктор 
    CSocket();
    
/// \brief Оператор копирования.
/// 
/// Сокет источник после присваивания использовать нельзя.
    CSocket & operator = (const CSocket & s);
    
public:
/// \brief Виртуальный деструктор
    virtual ~CSocket();

/// \brief Конструктор
	CSocket(char * _name);
    
/// \brief Проверка сокета
///
/// \return True если открыт, false если закрыт.
    bool IsValid() const;
    
/// \brief Готовность сокета к передаче.
///
/// \return True если готов, false если не готов.
    bool IsReady() const;
    
/// \brief Закрытие сокета.
    void Close();
};

/// \class CTCPSocket
/// \brief Класс TCP сокетов.
class NOVA_EXPORT CTCPSocket : public CSocket
{
    friend class CTCPServerSocket;
private:
    void DisableNaggle() throw(nova::NovaExp);

public:
/// \brief Конструктор
	inline CTCPSocket() : CSocket((char*)"CTCPSocket") {}
    
/// \brief Конструктор копирования
///
/// При копировании, создается точно такой же сокет как и источник, 
/// и источник уничтожается.
    CTCPSocket(const CTCPSocket & s);
    
/// \brief Конструктор
///
/// Открывает сокет и связывает его с конкретным ай пи адресом.
/// \param ip Ай пи адрес.
/// \param disableNaggle разрешить/запретить TCP_DELAY (Naggle algorithm)
    CTCPSocket(const CIPAddress & ip, bool disableNaggle = false) throw(nova::NovaExp);
    
/// \brief Оператор копирования
///
/// Создает точную копию сокета источника, после этого источник уничтожается.
    CTCPSocket & operator = (const CTCPSocket & s);
    
/// \brief Открытие сокета
///
/// Открывает сокет и связывает его с конкретным ай пи адресом.
/// \param ip Ай пи адрес.
/// \param disableNaggle разрешить/запретить TCP_DELAY (Naggle algorithm)
    void Open(const CIPAddress & ip, bool disableNaggle = false) throw(nova::NovaExp);
    
/// \brief Пересылка данных
///
/// После установки TCP соединения можно пересылать данные между узлами
/// \param data указатель на буфер данных
/// \param size размер буфера данных
/// \return количество переданных байт, ERR_DISCONNECTED соединение разорванно, 
/// ERR_SOCK_SYSTEM системная ошибка сокета
    uint Send(const byte * data, uint size);
    
/// \brief Прием данных
///
/// После установки TCP соединения можно принимать данные с удаленного узла.
/// \param buf указатель на буфер данных
/// \param maxSize размер буфера данных
/// \return количество принятых байт, ERR_DISCONNECTED соединение разорванно, 
/// ERR_SOCK_SYSTEM системная ошибка сокета
    uint Recv(byte * buf, uint maxSize);
};

/// \class CTCPServerSocket
/// \brief Класс TCP сервер сокетов.
///
/// Для создания сервера обычно создают поток в котором 
/// сервер слушает определенный хост на предмет 
/// запросов на соединение, так как эта процедура
/// стопит нить выполнения. Ну если конечно у нас всего 1 возможный 
/// клиент то поток можно не создавать)
class NOVA_EXPORT CTCPServerSocket : public CSocket
{
    bool disableNaggle;//this flag indicates if accepted sockets should be created with disabled Naggle
public:
/// \brief Конструктор
    CTCPServerSocket();
    
/// \brief Конструктор копирования
///
/// При копировании, создается точно такой же сокет как и источник, 
/// и источник уничтожается.
    CTCPServerSocket(const CTCPServerSocket & s);
    
/// \brief Оператор копирования
///
/// Создает точную копию сокета источника, после этого источник уничтожается.
    CTCPServerSocket & operator=(const CTCPServerSocket & s);
    
/// \brief Конструктор
///
/// Создает TCP сервер на текущей машине, связывает его с портом
/// и начинает прослушивание хоста.
/// \param port Порт
/// \param disableNaggle разрешить/запретить TCP_DELAY (Naggle algorithm)
    CTCPServerSocket(word port, bool disableNaggle = false);
    
/// \brief Создание сервера.
///
/// Создает TCP сервер на текущей машине, связывает его с портом
/// и начинает прослушивание хоста.
/// \param port Порт
/// \param disableNaggle разрешить/запретить TCP_DELAY (Naggle algorithm)
    void Open(word port, bool disableNaggle = false);
    
/// \brief Создание соединения.
///
/// Когда сервер услышит новый запрос на соединение нужно вызвать эту 
/// функцию для установления соединения, в случае удачного соединения мы
/// получаем объект типа CTCPSocket, который инициирует соединение со стороны сервера,
/// далее пользоваться функуиями Send/Recv для получения/передачи данных.
/// \return объект CTCPSocket
    CTCPSocket Accept();
};

/// \class CUDPSocket
/// \brief Класс UDP сокетов.
class NOVA_EXPORT CUDPSocket : public CSocket
{
public:
/// \brief Конструктор
    CUDPSocket();
    
/// \brief Деструктор
    ~CUDPSocket();
    
/// \brief Открытие UDP сервер сокета
///
/// Открывает сокет для прослушки(процесс не стопит) на определенный порт
/// текущей машины. Далее можно принимать данные.
/// \param port Порт
    void Open(word port); 

/// \brief Открытие UDP клиент сокета
///
/// Открывает UDP сокет, Далее можно посылать данные по сети на определенный адрес.
    void Open();

/// \brief Пересылка данных.
///
/// \param buf указатель на буффер данных
/// \param size размер буфера данных
/// \param destinationIP цель.
/// \return количество отосланных байт, если < 0, то ошибка передачи.
    uint Send(const byte* buf, word size, CIPAddress destinationIP);
    
/// \brief Прием данных.
///
/// \param buf указатель на буффер данных 
/// \param maxSize максимальный размер буффера
/// \param out_SenderIP адрес с которого пришли данные.
/// \return количество принятых байт, если < 0, то ошибка приема.
    uint Recv(byte* buf, word maxSize, CIPAddress &out_SenderIP);
};



}

