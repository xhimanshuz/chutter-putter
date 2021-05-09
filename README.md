# chutter-putter
Asio and Boost.Beast based HTTP and Raw Socket C++ Rendezvous Server to help client locate each and established peer-to-peer connection with TCP NAT Hole Puching technique mentioned in Bryan Ford's Paper's "Peer-to-Peer Communication Across Network Address Translators"

# Build

g++ src/*cpp -Iinclude -std=c++17 -lpthread -O3 -o chutter-putter

# Run

chutter-putter 8000

# Reference
Bryan Ford's Paper's  - 
Peer-to-Peer Communication Across Network Address Translators.
## https://bford.info/pub/net/p2pnat/

# P2P - Client
![alt text](https://github.com/xhimanshuz/chutter-putter-client/blob/main/doc/client.png)
## Github Repo - chutter-putter-client: 
## https://github.com/xhimanshuz/chutter-putter-client
Used this server to locate other clients over network to establised peer-to-peer connection.
