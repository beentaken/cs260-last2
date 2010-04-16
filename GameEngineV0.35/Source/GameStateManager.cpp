///////////////////////////////////////////////////////////////////////////////////////
//
//	Game Logic
//	Simple game logic for demo. A good way to approach understand the game sample
//	is to walk through all the code in game logic and follow the execution through
//	the rest of the engine.
//	
//	Authors: Chris Peters, Benjamin Ellinger
//	Copyright 2009, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "GameStateManager.h"
#include "WindowsSystem.h"
#include "Core.h"
#include "ComponentCreator.h"
#include "PlayerController.h"
#include "Asteroid.h"
#include "ScoreDisplay.h"
#include "BulletController.h"

#include "SinglePlayer.h"
#include "MainMenu.h"
#include "ServerState.h"

#include <ctime>

namespace Framework
{
	//Our global pointer to the game.
	GameStateManager* GSM = NULL;

  ///Initialize these once so they aren't mis-used later...
  const GameStateManager::GameStateID GameStateManager::GS_EXIT("EXIT");
  const GameStateManager::GameStateID GameStateManager::GS_RESTART("RESTART");

  ///Initialize the GSM pointer and check for duplicate creations
  GameStateManager::GameStateManager() : playerShipId_(0)
	{	
		//Set up the global pointer
		ErrorIf(GSM!=NULL,"Logic already initialized");
		GSM = this;

    //Components are explicitly registered in initialize
    RegisterComponent(Transform);
    RegisterComponent(PlayerController);
    RegisterComponent(Asteroid);
    RegisterComponent(ScoreDisplay);
		RegisterComponent(BulletController);

    //GameStates are created and registered here.
    RegisterGameState(SinglePlayer);
    RegisterGameState(MainMenu);
		RegisterGameState(ServerState);

	}

  ///Cleanup all the GameState memory.
	GameStateManager::~GameStateManager()
	{
    ///Cleanup the GameStates
    GameStateMap::iterator b = GameStates.begin(), e = GameStates.end();
    while (b != e)
      delete b++->second;
	}

  ///Registers all the component types and creates the GameStateObjects
  void GameStateManager::Initialize()
  {
    ///Set the initial state.
    Curr = Next = "SinglePlayer";
    //Curr = Next = "MainMenu";
    GameStates[Curr]->Initialize();
  }

  void GameStateManager::Update(float dt)
  {
    if (Next == GS_EXIT)
      2;// TODO: Do something to quit...

    ///Check if we need to change the GameState.
    if (Next != Curr)
    {
      /// TODO: Check if Next exists!!
      GameStates[Curr]->Cleanup();
      GameStates[Next]->Initialize();
      Curr = Next;
    }
    ///Or if we are to restart the state.
    else if (Next == GS_RESTART)
      GameStates[Curr]->Restart();

    ///Update the current state!
    GameStates[Curr]->Update(dt);
  }

  ///Pass the message to the current state.
	void GameStateManager::SendMessage(Message * m )
	{
    if (GameStates.count(Curr))
      GameStates[Curr]->SendMessage( m );
	}

  ///Sets the next GameState to state, returns true for success.
  bool GameStateManager::ChangeState( GameStateID state )
  {
    /// Check if state exists.
    if (GameStates.count(state)) {
      Next = state;
      return true;
    }

    ///state specified doesn't exist.
    return false;
  }

  void GameStateManager::AddGameState( const std::string &name, IGameState *state )
  {
    if (GameStates.count(name))
      return;

    GameStates[name] = state;
  }


  void GameStateManager::AddController( Controller *controller )
  {
    GameStates[Curr]->AddController(controller);
  }

  void GameStateManager::RemoveController( Controller *controller )
  {
    GameStates[Curr]->RemoveController(controller);
  }

	void GameStateManager::RegisterComposition( GOCId id )
	{
		GameStates[Curr]->RegisterComposition(id);
	}

}


