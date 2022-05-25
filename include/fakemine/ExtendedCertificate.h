#pragma once

#include "base.h"
#include "Certificate.h"

class ExtendedCertificate {

public:
    class ExtendedCertificate& operator=(class ExtendedCertificate const &) = delete;
    ExtendedCertificate(class ExtendedCertificate const &) = delete;
    ExtendedCertificate() = delete;

public:
    static std::string getIdentityName(Certificate const &);
    static std::string getXuid(Certificate const &);
};