/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "IndividualProgression.h"
#include "ScriptedGossip.h"

enum class PROGRESSION_SERVICE_TIER : uint32 {
    START,           // (Starting state - nothing to purchase)
    MOLTEN_CORE,     // (Molten Core)
    ONYXIA,          // (Onyxia's Lair)
    BLACKWING_LAIR,  // (Blackwing Lair)
    PRE_AQ,          // (Pre-Ahn'Qiraj / AQ gates quest line)
    AQ_WAR,          // (AQ War Effort)
    AQ,              // (Ahn'Qiraj)
    NAXX40,          // (Naxxramas - Vanilla)
    PRE_TBC,         // (Pre-TBC / Dark Portal - unlocks Karazhan, Gruul, Magtheridon)
    TBC_TIER_1,      // (Karazhan, Gruul's Lair, Magtheridon's Lair)
    TBC_TIER_2,     // (Serpentshrine Cavern, Tempest Keep)
    TBC_TIER_3,     // (Zul'Aman) - TODO: Check the ordering here in IP
    TBC_TIER_4,     // (Hyjal Summit, Black Temple)
    TBC_TIER_5,     // (Sunwell Plateau - unlocks Northrend / WotLK leveling)
    WOTLK_TIER_1,   // (Naxxramas, Eye of Eternity, Obsidian Sanctum)
    WOTLK_TIER_2,   // (Ulduar)
    WOTLK_TIER_3,   // (Trial of the Crusader)
    WOTLK_TIER_4,   // (Icecrown Citadel)
    WOTLK_TIER_5    // (Ruby Sanctum)
};

// Add player scripts
class IndividualProgressionService : public CreatureScript
{
public:
    IndividualProgressionService()
        : CreatureScript("individual_progression_service")
    {
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        // Check if mod is enabled. Stop execution if disabled
        if(!sConfigMgr->GetOption<bool>("IndividualProgressionService.Enable", true))
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Individual Progression Service is disabled. Please enable to talk to this NPC.");
            return false;
        }

        // TODO: Inform player of current progression tier

        // Creating Menu Items
        // TODO: Add pricing config (off by default)
        // AddGossipItemFor(
        //     player,
        //     GOSSIP_ICON_INTERACT_1,
        //     "|T" + GetTierIcon(PROGRESSION_SERVICE_TIER::START) + ":35:35|tProgress to " + GetTierName(PROGRESSION_SERVICE_TIER::START),
        //     GOSSIP_SENDER_MAIN,
        //     static_cast<uint32>(PROGRESSION_SERVICE_TIER::START)
        // );

        // Loop through all items in the enum and display their related icon and message.
        for(uint32 i = static_cast<uint32>(PROGRESSION_SERVICE_TIER::START);
            i <= static_cast<uint32>(PROGRESSION_SERVICE_TIER::WOTLK_TIER_5);
            ++i)
        {
          auto tier = static_cast<PROGRESSION_SERVICE_TIER>(i);

          AddGossipItemFor(
              player,
              GOSSIP_ICON_INTERACT_1,
              "|T" + GetTierIcon(tier) + ":35:35|tProgress through " + GetTierName(tier),
              GOSSIP_SENDER_MAIN,
              i
          );
        }

        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

        return true;
    }

    bool OnGossipSelect( Player* player, Creature* creature, uint32 sender, uint32 action)
    { 
        // If action id is outside of our scope, we skip
        if (action > static_cast<uint32>(PROGRESSION_SERVICE_TIER::WOTLK_TIER_5))
        {
            CloseGossipMenuFor(player);
            return true;
        }

        auto tier = static_cast<PROGRESSION_SERVICE_TIER>(action);

        // TODO: Here we need to check if the player can purchase it, here is where we do the purchase
        // TODO: Make it a purchase and check config to see if it's free or not

        // Apply purchase
        ApplyProgressionTierPurchase(player, tier);

        CloseGossipMenuFor(player);
        return true;
    }

    void ApplyProgressionTierPurchase(Player* player, PROGRESSION_SERVICE_TIER newTier)
    {
        // Handles all progress except for a full reset to 0
        if(newTier != PROGRESSION_SERVICE_TIER::START)
        {
          // This is a built-in method from Individual Progression module that clears out all quests then sets the progression to newTier
          sIndividualProgression->ForceUpdateProgressionState(player, static_cast<ProgressionState>(newTier));
          sIndividualProgression->checkIPPhasing(player, player->GetAreaId());

          ChatHandler(player->GetSession()).PSendSysMessage("You have successfully progressed to {}.", GetTierName(newTier));
        }
        // Handles the full reset to 0 case
        else
        {
          // Progression is determined by quests, so we remove the corresponding progression quests
          for(uint32 i=static_cast<uint32>(PROGRESSION_SERVICE_TIER::MOLTEN_CORE); i <= static_cast<uint32>(PROGRESSION_SERVICE_TIER::WOTLK_TIER_5);++i)
          {
            uint32 progressionQuest = 66000 + i;

            if(player->GetQuestStatus(progressionQuest) == QUEST_STATUS_REWARDED)
              player->RemoveRewardedQuest(progressionQuest);
          }

          // Reset their phase
          sIndividualProgression->checkIPPhasing(player, player->GetAreaId());
          
          ChatHandler(player->GetSession()).PSendSysMessage("Your progression has been reset to the start.");
        }

    }

    std::string GetTierIcon(PROGRESSION_SERVICE_TIER tier)
    {
      switch (tier)
      {
        case PROGRESSION_SERVICE_TIER::START:
          return "Interface/Icons/inv_misc_map02";
        case PROGRESSION_SERVICE_TIER::MOLTEN_CORE:
          return "Interface/Icons/achievement_boss_ragnaros";
        case PROGRESSION_SERVICE_TIER::ONYXIA:
          return "Interface/Icons/achievement_boss_onyxia";
        case PROGRESSION_SERVICE_TIER::BLACKWING_LAIR:
          return "Interface/Icons/achievement_boss_nefarion";
        case PROGRESSION_SERVICE_TIER::PRE_AQ:
          return "Interface/Icons/inv_misc_qirajicrystal_05";
        case PROGRESSION_SERVICE_TIER::AQ_WAR:
          return "Interface/Icons/inv_misc_qirajicrystal_01";
        case PROGRESSION_SERVICE_TIER::AQ:
          return "Interface/Icons/achievement_boss_cthun";
        case PROGRESSION_SERVICE_TIER::NAXX40:
          return "Interface/Icons/achievement_boss_kelthuzad_01";
        case PROGRESSION_SERVICE_TIER::PRE_TBC:                 
          return "Interface/Icons/Achievement_Dungeon_Outland_DungeonMaster";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_1:
          return "Interface/Icons/achievement_boss_princemalchezaar_02";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_2:
          return "Interface/Icons/achievement_boss_kael'thassunstrider_01";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_3:   
          return "Interface/Icons/Achievement_Boss_Zuljin";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_4:
          return "Interface/Icons/achievement_boss_illidan";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_5:                
          return "Interface/Icons/ACHIEVEMENT_BOSS_KILJAEDAN";    
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_1:
          return "Interface/Icons/achievement_boss_kelthuzad_01";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_2:
          return "Interface/Icons/achievement_boss_yoggsaron_01";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_3:                 
          return "Interface/Icons/Achievement_Boss_Anubarak";       
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_4:
          return "Interface/Icons/achievement_boss_lichking";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_5:                 
          return "Interface/Icons/Achievement_Boss_ValithraDreamwalker";     
        default:
          return "Interface/Icons/inv_misc_questionmark";
      }
    }

    std::string GetTierName(PROGRESSION_SERVICE_TIER tier)
    {
      switch (tier)
      {
        case PROGRESSION_SERVICE_TIER::START:
          return "Start";
        case PROGRESSION_SERVICE_TIER::MOLTEN_CORE:
          return "Molten Core";
        case PROGRESSION_SERVICE_TIER::ONYXIA:
          return "Onyxia's Lair";
        case PROGRESSION_SERVICE_TIER::BLACKWING_LAIR:
          return "Blackwing Lair";
        case PROGRESSION_SERVICE_TIER::PRE_AQ:
          return "Pre-AQ";
        case PROGRESSION_SERVICE_TIER::AQ_WAR:
          return "AQ War Effort";
        case PROGRESSION_SERVICE_TIER::AQ:
          return "Ahn'Qiraj";
        case PROGRESSION_SERVICE_TIER::NAXX40:
          return "Naxxramas (Vanilla)";
        case PROGRESSION_SERVICE_TIER::PRE_TBC:
          return "Pre-TBC";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_1:
          return "TBC Tier 1";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_2:
          return "TBC Tier 2";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_3: 
          return "TBC Tier 3";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_4:
          return "TBC Tier 4";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_5:
          return "TBC Tier 5";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_1:
          return "WotLK Tier 1";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_2:
          return "WotLK Tier 2";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_3:
          return "WotLK Tier 3";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_4:
          return "WotLK Tier 4";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_5:
          return "WOTLK Tier 5";
        default:
          return "Unknown Tier";
      }
    }

};

// Add all scripts in one
void AddIndividualProgressionServiceScripts()
{
    new IndividualProgressionService();
}
