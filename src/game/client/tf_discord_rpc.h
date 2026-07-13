#pragma once
#ifndef TF_DISCORD_RPC_H
#define TF_DISCORD_RPC_H
#ifdef _WIN32
#pragma once
#endif


void DiscordRPC_Init();
void DiscordRPC_RunCallbacks();
void DiscordRPC_Shutdown();

#endif // TF_DISCORD_RPC_H