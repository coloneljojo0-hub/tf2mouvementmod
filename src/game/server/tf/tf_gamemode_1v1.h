#pragma once
//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Custom 1v1 Gamemode Logic
//
//=============================================================================
#ifndef TF_GAMEMODE_1V1_H
#define TF_GAMEMODE_1V1_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "convar.h"

class C1v1Gamemode
{
public:
    C1v1Gamemode();

    void StartMode(int iScoreLimit);
    void StopMode();
    bool IsActive() const { return m_bIsActive; }

    int GetScoreLimit() const { return m_iScoreLimit; }

    // Event Hook Handlers
    void OnPlayerKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker);
    void OnPlayerSpawn(CBasePlayer* pPlayer);

private:
    void ResetPlayerScores();
    void EndMatch(CBasePlayer* pWinner);

    bool m_bIsActive;
    int m_iScoreLimit;
};

extern C1v1Gamemode g_1v1Gamemode;

#endif // TF_GAMEMODE_1V1_H