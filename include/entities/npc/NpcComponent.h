#pragma once

#include <string>

// ---------------------------------------------------------------------------
// NpcComponent — marker + data tag identifying an NPC Entity.
//
// REPLACES Npc.h entirely. The old Npc class was a thin wrapper holding
// an Entity plus one piece of NPC-only data (typeName) and forwarding
// update()/getDirection()/getRenderX()/getRenderY() straight through to
// the wrapped Entity. Once Entity is an open component bag, that
// wrapper has no remaining purpose — "NPC-ness" is just another
// component you attach, same as PlayerControlComponent marks
// "player-ness":
//
//   if (entity.has<NpcComponent>()) { ... }
//
// typeName is kept (it was the one real field Npc.h had) for whatever
// downstream code identifies/looks up an NPC's sprite or behavior by
// name — same role it played before, just living on the tag component
// instead of the wrapper.
//
// behavior_ / switchMovement() / INpcBehavior are NOT implemented here
// — per existing notes these are still future work (NPCs are currently
// static, no behavior driver exists yet). When that lands, it's an
// ADDITIVE field on this struct (e.g. a behavior strategy pointer) or
// a new sibling component (AiComponent) — not a reason to revisit this
// file now.
// ---------------------------------------------------------------------------
struct NpcComponent {
    std::string typeName;
};