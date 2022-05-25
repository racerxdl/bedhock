#pragma once

#include "base.h"

class NetworkIdentifier {
    char filler[160];
public:
	std::string getIP();
    class NetworkIdentifier& operator=(class NetworkIdentifier const &) = delete;
    NetworkIdentifier(class NetworkIdentifier const &) = delete;
    NetworkIdentifier() = delete;
    // NetworkIdentifier(struct RakNet::RakNetGUID const &);
    NetworkIdentifier(struct sockaddr_in6 const &);
    NetworkIdentifier(struct sockaddr_in const &);
    std::string getAddress() const;
    uint64_t getHash() const;
    bool isUnassigned() const;
    bool operator==(class NetworkIdentifier const &) const;
    std::string toString() const;

private:
    bool equalsTypeData(class NetworkIdentifier const &) const;

};