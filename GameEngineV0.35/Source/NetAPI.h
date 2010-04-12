/**************************************************************************************************/
/*!
@file   NetAPI.h
@author Robert Onulak
@author Justin Keane
@par    email: robert.onulak@@digipen.edu
@par    email: justin.keane@@digipen.edu

@par    Course: CS260
@par    Assignment #3

----------------------------------------------------------------------------------------------------
@attention � Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/
#ifndef NETAPI__H
#define NETAPI__H

#include "SocketTypes.h"
#include "ConfigReader.hpp"

namespace Framework ///< Networking API namespace
{
  ///The network api class. Deals with creating and destroying sockets.
  ///Also manages setting up Winsock and getting and data from it.
  class NetAPI_
  {
    ///Cleaner interface and holds both types of sockets.
    typedef std::map<SocketID, ASocket*> SocketBank;

    ///Data types for the set of available UDP ports as specified by the config file.
    typedef std::pair<unsigned, bool> PortState;
    typedef std::vector<PortState> PortBank;

	  //Mutex mutex;        ///< Keeps data structures safe.
    PortBank ports;     ///< The set of ports allowed for UDP sockets.
	  SocketBank sockets; ///< All the sockets that have been created.
    IPAddress localIP;  ///< Local IP saved for convience.
	  WSADATA wsData;     ///< The winsock context data.
	  bool init;          ///< Whether or not the system has been initialized.

    ///Singleton class
	  NetAPI_( void );
	  ~NetAPI_( void );
  public:
    ///Singleton getter method.
	  static NetAPI_ *GetInstance( void );

	  ///Initializes WinSock
	  int Init( void );
	  ///Shutdown WinSock
	  void Cleanup( void );

	  ///Returns the local IP address
	  IPAddress LocalIP( void ) const;

	  ///Makes a new TCP socket and returns it.
	  TCPSOCKET NewTCPSocket( void );
    ///Makes a new UDP socket and initializes it.
    UDPSOCKET NewUDPSocket( void );

	  ///Returns the socket with id from the socket_bank.
    TCPSOCKET GetTCPSocket( SocketID id );
	  UDPSOCKET GetUDPSocket( SocketID id );

	  ///Closes and deletes the sockets
	  void CloseSocket( ASocket *socket );
  };

  ///Much easier than explicitly calling the method.
  #define NetAPI NetAPI_::GetInstance()

} // Framework namespace

#endif
