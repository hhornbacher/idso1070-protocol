#include <transport/Transport.h>

Transport::Transport() : throttlingTimer_(ioService_, boost::posix_time::milliseconds(0)),
                         timeoutTimer_(ioService_, boost::posix_time::milliseconds(0))
{
}

void Transport::connect(ConnectionHandler connectionHandler)
{
  if (!connected_ && !busy_)
  {
    connectionHandler_ = connectionHandler;
    setupConnection();
  }
  else
  {
    Exception e(busy_ ? "Busy" : "Already connected");
    connectionHandler(&e, connected_);
  }
}

void Transport::disconnect(ConnectionHandler connectionHandler)
{
  if (connected_ && !busy_)
  {
    connectionHandler_ = connectionHandler;
    stopConnection();
  }
  else
  {
    Exception e(busy_ ? "Busy" : "Not connected");
    connectionHandler(&e, connected_);
  }
}

void Transport::transaction(uint8_t *txBuffer, size_t txBufferLength, uint8_t *rxBuffer, size_t rxBufferLength, TransactionHandler transactionHandler)
{
  if (connected_ && !busy_)
  {
    txBuffer_ = txBuffer;
    txBufferLength_ = txBufferLength;
    rxBuffer_ = rxBuffer;
    rxBufferLength_ = rxBufferLength;
    transactionHandler_ = transactionHandler;
    send();
  }
  else
  {
    Exception e(busy_ ? "Busy" : "Not connected");
    transactionHandler(&e);
  }
}