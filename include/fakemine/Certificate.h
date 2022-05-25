#pragma once

#include "base.h"

class Certificate {
    char filler[128];

public:
    class Certificate& operator=(class Certificate const &) = delete;
    Certificate() = delete;
    Certificate(class Certificate const &);
    int64_t getExpirationDate() const;
    std::string getIdentityPublicKey() const;
    int64_t getNotBeforeDate() const;
    bool isValid() const;
    std::string toString() const;
    bool validate(int64_t);
    ~Certificate();

private:
    Certificate(class UnverifiedCertificate const &, std::unique_ptr<class Certificate>);

};