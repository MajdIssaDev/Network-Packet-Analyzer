#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <unistd.h>

int main() {
    // 1. Create a Raw Socket that listens for ALL Ethernet traffic
    int raw_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (raw_sock < 0) {
        std::cerr << "Error: Could not open raw socket. (Did you run as sudo?)" << std::endl;
        return 1;
    }

    std::cout << "Success! Raw socket is open and listening..." << std::endl;

    // 2. We'll add the capture loop next.
    close(raw_sock);
    return 0;
}