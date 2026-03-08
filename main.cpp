#include <iostream>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <iomanip>

// Professional Hex Dump utility
void print_hex_dump(const unsigned char* data, int size) {
    for (int i = 0; i < size; i++) {
        // Print 16 bytes per line
        if (i != 0 && i % 16 == 0) std::cout << std::endl;

        // Format as 2-digit uppercase hex
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << (int)data[i] << " ";
    }
    std::cout << std::dec << std::endl << std::endl;
}

#pragma pack(push, 1)
struct EthernetHeader {
    unsigned char dest_mac[6];   // 6 bytes for Destination MAC
    unsigned char src_mac[6];    // 6 bytes for Source MAC
    unsigned short ether_type;   // 2 bytes for Protocol (e.g., IPv4)
};
#pragma pack(pop)

int main() {
    int raw_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (raw_sock < 0) {
        std::cerr << "Error: Run as sudo!" << std::endl;
        return 1;
    }

    std::vector<unsigned char> buffer(65536);
    struct sockaddr saddr;
    socklen_t saddr_size = sizeof(saddr);

    std::cout << "Monitoring live hex data... (Ctrl+C to stop)" << std::endl;

    while (true) {
        ssize_t data_size = recvfrom(raw_sock, buffer.data(), buffer.size(), 0, &saddr, &saddr_size);

        if (data_size >= 14) { // 14 bytes is the minimum for an Ethernet header
            // Map the start of the buffer to our struct
            EthernetHeader* eth = (EthernetHeader*)buffer.data();

            std::cout << "\n[ Ethernet Frame ]" << std::endl;

            // Print Source MAC
            std::cout << "  Source: ";
            for(int i=0; i<6; i++)
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)eth->src_mac[i] << (i < 5 ? ":" : "");

            // Print Destination MAC
            std::cout << "\n  Dest:   ";
            for(int i=0; i<6; i++)
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)eth->dest_mac[i] << (i < 5 ? ":" : "");

            // Print Protocol Type (Converted from Network Byte Order)
            std::cout << std::dec << "\n  Type:   0x" << std::hex << ntohs(eth->ether_type) << std::endl;
        }
    }

    close(raw_sock);
    return 0;
}