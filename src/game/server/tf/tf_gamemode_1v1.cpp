#include "cbase.h"
#ifdef GAME_DLL   // server only

#include "tf_gamemode_1v1.h"
#include "tf_player.h"
#include "gamerules.h"
#include "engine/IEngineSound.h"

// ConVars for gamemode customization
ConVar sm_1v1_enabled("sm_1v1_enabled", "0", FCVAR_GAMEDLL | FCVAR_NOTIFY, "Toggles 1v1 Mode active state.");
ConVar sm_1v1_score_limit("sm_1v1_score_limit", "20", FCVAR_GAMEDLL | FCVAR_NOTIFY, "Score limit required to win 1v1 mode.");

C1v1Gamemode g_1v1Gamemode;

C1v1Gamemode::C1v1Gamemode()
{
    m_bIsActive = false;
    m_iScoreLimit = 20;
}

void C1v1Gamemode::StartMode(int iScoreLimit)
{
    m_bIsActive = true;
    m_iScoreLimit = (iScoreLimit > 0) ? iScoreLimit : 20;

    sm_1v1_enabled.SetValue(1);
    sm_1v1_score_limit.SetValue(m_iScoreLimit);

    // 1. Disable round timer and limits (Infinite round time)
    ConVarRef mp_timelimit("mp_timelimit");
    mp_timelimit.SetValue(0);

    ConVarRef mp_stalemate_enable("mp_stalemate_enable");
    mp_stalemate_enable.SetValue(0);

    // 2. Disable respawn delay for instant respawning
    ConVarRef mp_respawnwavetime("mp_respawnwavetime");
    mp_respawnwavetime.SetValue(0);

    ConVarRef mp_disable_respawn_times("mp_disable_respawn_times");
    mp_disable_respawn_times.SetValue(1);

    ResetPlayerScores();

    char szMsg[128];
    Q_snprintf(szMsg, sizeof(szMsg), "1v1 MODE STARTED!\nFirst to %d kills wins!", m_iScoreLimit);
    UTIL_ClientPrintAll(HUD_PRINTCENTER, szMsg);

    Msg("[1v1 Mode] Started with Score Limit: %d\n", m_iScoreLimit);
}

void C1v1Gamemode::StopMode()
{
    m_bIsActive = false;
    sm_1v1_enabled.SetValue(0);

    UTIL_ClientPrintAll(HUD_PRINTCENTER, "1v1 MODE ENDED");
}

void C1v1Gamemode::ResetPlayerScores()
{
    for (int i = 1; i <= gpGlobals->maxClients; i++)
    {
        CTFPlayer* pPlayer = ToTFPlayer(UTIL_PlayerByIndex(i));
        if (pPlayer)
        {
            pPlayer->ResetFragCount();
            pPlayer->ResetDeathCount();
        }
    }
}

void C1v1Gamemode::OnPlayerKilled(CBasePlayer* pVictim, CBasePlayer* pAttacker)
{
    if (!m_bIsActive || !pVictim)
        return;

    CTFPlayer* pTFVictim = ToTFPlayer(pVictim);
    CTFPlayer* pTFAttacker = ToTFPlayer(pAttacker);

    // --- ATTACKER LOGIC: Full Health & Ammo Refill ---
    if (pTFAttacker && pTFAttacker != pTFVictim)
    {
        // Add kill score
        pTFAttacker->IncrementFragCount(1);
        int iAttackerScore = pTFAttacker->FragCount(); // Correct method name

        // Fully heal killer
        pTFAttacker->m_iHealth = pTFAttacker->GetMaxHealth();

        // Refill all ammo
        for (int i = 0; i < MAX_WEAPONS; i++)
        {
            CBaseCombatWeapon* pWeapon = pTFAttacker->GetWeapon(i);
            if (pWeapon)
            {
                int iPrimaryAmmoType = pWeapon->GetPrimaryAmmoType();
                if (iPrimaryAmmoType != -1)
                {
                    pTFAttacker->SetAmmoCount(pTFAttacker->GetMaxAmmo(iPrimaryAmmoType), iPrimaryAmmoType);
                }

                if (pWeapon->UsesClipsForAmmo1())
                {
                    pWeapon->m_iClip1 = pWeapon->GetMaxClip1();
                }
            }
        }

        // Announce current score
        char szScoreMsg[128];
        Q_snprintf(szScoreMsg, sizeof(szScoreMsg), "%s scored a kill! [%d / %d]",
            pTFAttacker->GetPlayerName(), iAttackerScore, m_iScoreLimit);
        UTIL_ClientPrintAll(HUD_PRINTTALK, szScoreMsg);

        // --- VICTORY CHECK ---
        if (iAttackerScore >= m_iScoreLimit)
        {
            EndMatch(pTFAttacker);
            return;
        }
    }

    // --- VICTIM LOGIC: Force Immediate Instant Respawn ---
    if (pTFVictim)
    {
        pTFVictim->ForceRespawn();
    }
}

void C1v1Gamemode::OnPlayerSpawn(CBasePlayer* pPlayer)
{
    if (!m_bIsActive || !pPlayer)
        return;

    CTFPlayer* pTFPlayer = ToTFPlayer(pPlayer);
    if (!pTFPlayer)
        return;

    // Ensure full health and ammo when spawning in 1v1
    pTFPlayer->SetHealth(pTFPlayer->GetMaxHealth());

    for (int i = 0; i < MAX_WEAPONS; i++)
    {
        CBaseCombatWeapon* pWeapon = pTFPlayer->GetWeapon(i);
        if (pWeapon)
        {
            int iPrimaryAmmoType = pWeapon->GetPrimaryAmmoType();
            if (iPrimaryAmmoType != -1)
            {
                pTFPlayer->SetAmmoCount(pTFPlayer->GetMaxAmmo(iPrimaryAmmoType), iPrimaryAmmoType);
            }

            if (pWeapon->UsesClipsForAmmo1())
            {
                pWeapon->m_iClip1 = pWeapon->GetMaxClip1();
            }
        }
    }
}

void C1v1Gamemode::EndMatch(CBasePlayer* pWinner)
{
    if (pWinner)
    {
        char szWinCenter[128];
        Q_snprintf(szWinCenter, sizeof(szWinCenter), "WINNER!\n%s wins the 1v1 duel!", pWinner->GetPlayerName());
        UTIL_ClientPrintAll(HUD_PRINTCENTER, szWinCenter);

        char szWinTalk[128];
        Q_snprintf(szWinTalk, sizeof(szWinTalk), "***** %s WON THE 1v1 MATCH! *****", pWinner->GetPlayerName());
        UTIL_ClientPrintAll(HUD_PRINTTALK, szWinTalk);
    }

    StopMode();
}

// ==========================================
// --- CONSOLE COMMAND TO START 1v1 MODE ---
// ==========================================
CON_COMMAND(start_1v1_mode, "Launches 1v1 mode. Usage: start_1v1_mode [score_limit]")
{
    int iLimit = 20; // Default score limit

    if (args.ArgC() > 1)
    {
        iLimit = atoi(args[1]);
    }

    g_1v1Gamemode.StartMode(iLimit);
}

CON_COMMAND(stop_1v1_mode, "Stops the 1v1 mode.")
{
    g_1v1Gamemode.StopMode();
}
#endif // GAME_DLL
