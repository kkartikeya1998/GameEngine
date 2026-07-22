#pragma once

class InputManager;
class AssetDatabase;
class AnimationSystem;
class EventQueue;

template<typename>
class StateMachine;

class IGameState;


struct GameServices
{
    InputManager& input;
    AssetDatabase& assets;
    StateMachine<IGameState>& states;
    AnimationSystem& animations;
    EventQueue& events;
};