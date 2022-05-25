#include <fakemine/fakemine.h>
#include <wrap/Player.h>
#include <wrap/TextPacket.h>

#include <iostream>

void _WrappedPlayer::sendNetworkPacket(const Packet &p) const {
    handler->packetSender->sendToClient(id, &p, 0);
}
