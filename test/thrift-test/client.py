#!/usr/bin/env python


import sys
sys.path.append('./gen-py')

from echo import Echo
from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

# Make socket
transport = TSocket.TSocket('localhost', 9888)

# Buffering is critical. Raw sockets are very slow
transport = TTransport.TBufferedTransport(transport)

# Wrap in a protocol
protocol = TBinaryProtocol.TBinaryProtocol(transport)

# Create a client to use the protocol encoder
client = Echo.Client(protocol)

# Connect!
transport.open()

sum = client.GetInt(100)
print sum 

ss = client.GetString('Hello Python')
print ss

# Close!
transport.close()

