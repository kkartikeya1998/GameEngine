#pragma once

// ---------------------------------------------------------------------------
// PlayerControlComponent — marker tag identifying the player-controlled
// Entity.
//
// REPLACES `using Player = Entity;`. That alias could never express
// "this Entity is also the one driven by live input" as data — it was
// just a second name for the same type, so nothing distinguished a
// Player Entity from an Npc Entity except which variable held it. This
// tag makes "is this the player" a real, queryable fact:
//
//   if (entity.has<PlayerControlComponent>()) { ... }
//
// Empty for now (no fields) — this is intentionally NOT pre-built out
// with input-mapping config, etc. per the linear-curve precedent: add
// fields here only when a second concrete need shows up (today there's
// exactly one player, driven by Game.cpp's WASD polling, so there's
// nothing to parameterize yet).
// ---------------------------------------------------------------------------
struct PlayerControlComponent {};