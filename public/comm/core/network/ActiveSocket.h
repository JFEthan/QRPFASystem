/*---------------------------------------------------------------------------*/

 /**----------------------------------------------------------------------------*/
#ifndef __ACTIVESOCKET_H__
#define __ACTIVESOCKET_H__

#include "SimpleSocket.h"

namespace accu {
namespace core {
namespace network {

class CPassiveSocket;

/// Provides a platform independent class to create an active socket.
/// An active socket is used to create a socket which connects to a server.
/// This type of object would be used when an application needs to send/receive
/// data from a server.
class CActiveSocket : public CSimpleSocket {
 public:
  friend class CPassiveSocket;

  explicit CActiveSocket(CSocketType type = SocketTypeTcp);
  virtual ~CActiveSocket() {
    Close();
  };

  /// Established a connection to the address specified by pAddr.
  /// Connection-based protocol sockets (CSocket::SocketTypeTcp) may
  /// successfully call open() only once, however; connectionless protocol
  /// sockets (CSocket::SocketTypeUdp) may use Open() multiple times to
  /// change their association.
  ///  @param pAddr specifies the destination address to connect.
  ///  @param nPort specifies the destination port.
  ///  @return true if successful connection made, otherwise false.
  virtual bool Open(const char *pAddr, uint16_t nPort);

 private:
  /// Utility function used to create a TCP connection, called from Open().
  ///  @return true if successful connection made, otherwise false.
  bool ConnectTCP(const char *pAddr, uint16_t nPort);

  /// Utility function used to create a UDP connection, called from Open().
  ///  @return true if successful connection made, otherwise false.
  bool ConnectUDP(const char *pAddr, uint16_t nPort);

  /// Utility function used to create a RAW connection, called from Open().
  ///  @return true if successful connection made, otherwise false.
  bool ConnectRAW(const char *pAddr, uint16_t nPort);

 private:
  struct hostent *m_pHE;

};

}//namespace socket
}//namespace core
}//namespace accu

#endif /*  __ACTIVESOCKET_H__  */

