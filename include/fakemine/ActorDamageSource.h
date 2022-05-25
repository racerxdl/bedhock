#pragma once

#include "base.h"

class ActorDamageSource {
   public:
    class ActorDamageSource &operator=(class ActorDamageSource const &) = delete;
    ActorDamageSource(class ActorDamageSource const &) = delete;
    ActorDamageSource() = delete;

    virtual ~ActorDamageSource();
    virtual bool isEntitySource() const;
    virtual bool isChildEntitySource() const;
    virtual bool isBlockSource() const;
    virtual bool isFire() const;
    virtual bool isReducedByArmorReduction() const;
    virtual bool isFallingBlockDamage() const;
    virtual bool isFallDamage() const;
    virtual struct std::pair<std::string, std::vector<std::string>> getDeathMessage(std::string, class Actor *) const;
    virtual bool getIsCreative() const;
    virtual bool getIsWorldBuilder() const;
    virtual void __unk_vfn_11();
    virtual void __unk_vfn_12();
    virtual enum ActorCategory getEntityCategories() const;
    virtual bool getDamagingEntityIsCreative() const;
    virtual bool getDamagingEntityIsWorldBuilder() const;
    virtual struct ActorUniqueID getDamagingEntityUniqueID() const;
    virtual enum ActorType getDamagingEntityType() const;
    virtual enum ActorCategory getDamagingEntityCategories() const;
    virtual std::unique_ptr<class ActorDamageSource> clone() const;

    ActorDamageSource(enum ActorDamageCause);
    enum ActorDamageCause getCause() const;
    void setCause(enum ActorDamageCause);
    static std::vector<struct std::pair<std::string, enum ActorDamageCause>> generateDamageCauseCommandEnum();
    ActorType getEntityType() const;
    static enum ActorDamageCause lookupCause(std::string const &);
    static std::string const &lookupCauseName(enum ActorDamageCause);
};