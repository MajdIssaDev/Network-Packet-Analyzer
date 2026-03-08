#include <iostream>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

int main() {
    // 1. Create the Raw Socket
    int raw_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (raw_sock < 0) {
        std::cerr << "Error: Could not open raw socket. (Did you run as sudo?)" << std::endl;
        return 1;
    }

    std::cout << "Successfully connected to the Network Card!" << std::endl;
    std::cout << "Listening for live packets... (Press Ctrl+C to stop)" << std::endl;

    // 2. Setup a buffer for incoming data (65,536 is the max MTU size)
    std::vector<unsigned char> buffer(65536);
    struct sockaddr saddr;
    socklen_t saddr_size = sizeof(saddr);

    int packet_count = 0;

    // 3. The Live Capture Loop
    while (true) {
        // recvfrom blocks until a packet arrives
        ssize_t data_size = recvfrom(raw_sock, buffer.data(), buffer.size(), 0, &saddr, &saddr_size);

        if (data_size < 0) {
            std::cerr << "Error: Failed to capture packet." << std::endl;
            break;
        }

        packet_count++;
        std::cout << "[" << packet_count << "] Captured Packet: " << data_size << " bytes" << std::endl;
    }

    close(raw_sock);
    return 0;
}