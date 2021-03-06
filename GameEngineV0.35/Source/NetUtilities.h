#pragma once // Make sure this header is only included once.

namespace Framework
{
  ///Maximum message size per packet.
  const unsigned MAX_MSG_SIZE = 1024 * 4;

  ///Typedefs to make life easier...
  typedef std::string     EMessage;
  typedef std::string     IPAddress;
  typedef unsigned short  Port;

  ///Wrapper for the sockaddr_in struct used to connect to TCP sockets and UDP sockets.
  struct NetAddress
  {
    ///Underlying data. Has no methods itself...
    sockaddr_in address;
    std::string ip_;
    unsigned port_;
  public:
    NetAddress( void );
    NetAddress( const NetAddress &rhs )
    {
      ip_ = rhs.ip_; port_ = rhs.port_;
      memcpy(&address, &rhs.address, sizeof(address));
    }

    NetAddress( const IPAddress &ip, Port port = 0 );

    ///Implicit conversion for cleanlyness.
    operator sockaddr*( void ) { return reinterpret_cast<sockaddr*>(&address); }
    operator const sockaddr*( void ) const { return reinterpret_cast<const sockaddr*>(&address); }

    ///Returns the sizeof the actual sockaddr_in
    int Size() const { return sizeof(address); }

    ///Clears the memory in the sockaddr_in struct to keep data safe...
    void ClearMemory( void ) { SecureZeroMemory( &address, sizeof(address) ); }

    void ResetData( void )
    {

      ip_.assign(inet_ntoa(address.sin_addr));
      port_ = ntohs(address.sin_port);
    }

    ///Comparison operators because there aren't any for sockaddr_in or sockaddr.
    bool operator==( const NetAddress &rhs ) const;
    bool operator!=( const NetAddress &rhs ) const;

    bool operator<( const NetAddress &rhs ) const
    {
      if (ip_ != rhs.ip_)
        return ip_ < rhs.ip_;
      else
        return port_ < rhs.port_;
    }

    NetAddress & operator=( const NetAddress &rhs )
    {
      ip_ = rhs.ip_; port_ = rhs.port_;
      memcpy(&address, &rhs.address, sizeof(address));

      return *this;
    }

    ///Sets the sin_family of the socket.
    void SetFamily( unsigned family ) { address.sin_family = family; }

    ///Sets the port on the address. Can break if in mid use...
    void SetPort( Port port )
    { port_ = port; address.sin_port = htons( port ); }

    ///Sets the IP address. Can break if in mid use...
    void SetIP( const IPAddress &ip )
    { ip_ = ip; address.sin_addr.s_addr = inet_addr( ip.c_str() ); }

  };

  ///NetAPI exception class. Formats error codes into human readable text.
  struct Error : public std::exception
  {
    enum ErrorCode
    {
      E_Unknown = -1,   ///< Unknown error occured.
      E_NoMemory = 0,   ///< Failed to create some memory.
      E_NetworkInit,    ///< Failed to initialize our Network.
      E_SocketError     ///< Socket error while performing some operation.
    };    // enum ERROR

    Error( ErrorCode code, const EMessage &what ) throw() : code_(code), error_(0), what_(what) {;}

    virtual const char* what( void ) const throw() { return what_.c_str(); }

    ErrorCode code_;  ///< Error code given to this exception object by the user stating why it was thrown.
    int error_;       ///< Error number received from GetLastError()
    EMessage what_;    ///< String discribing what happened at the time this object was thrown.
  };    // struct Error

  /// Creates and Error exception from a code.
  Error CreateError(Error::ErrorCode code);

  /// Creates and throws an Error exception from the code given.
  void ThrowError(Error::ErrorCode code) throw (Error);
}
