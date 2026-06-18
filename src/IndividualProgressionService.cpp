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
    START = 0,           // (Starting state - nothing to purchase)
    MOLTEN_CORE = 1,     // (Molten Core)
    ONYXIA = 2,          // (Onyxia's Lair)
    BLACKWING_LAIR = 3,  // (Blackwing Lair)
    PRE_AQ = 4,          // (Pre-Ahn'Qiraj / AQ gates quest line)
    AQ_WAR = 5,          // (AQ War Effort)
    AQ = 6,              // (Ahn'Qiraj)
    NAXX40 = 7,          // (Naxxramas - Vanilla)
    PRE_TBC = 8,         // (Pre-TBC / Dark Portal - unlocks Karazhan, Gruul, Magtheridon)
    TBC_TIER_1 = 9,      // (Karazhan, Gruul's Lair, Magtheridon's Lair)
    TBC_TIER_2 = 10,     // (Serpentshrine Cavern, Tempest Keep)
    TBC_TIER_3 = 11,     // (Zul'Aman) - DISABLED in IP: no cost entry, cannot be purchased
    TBC_TIER_4 = 12,     // (Hyjal Summit, Black Temple)
    TBC_TIER_5 = 13,     // (Sunwell Plateau - unlocks Northrend / WotLK leveling)
    WOTLK_TIER_1 = 14,   // (Naxxramas, Eye of Eternity, Obsidian Sanctum)
    WOTLK_TIER_2 = 15,   // (Ulduar)
    WOTLK_TIER_3 = 16,   // (Trial of the Crusader)
    WOTLK_TIER_4 = 17,   // (Icecrown Citadel)
    WOTLK_TIER_5 = 18,   // (Ruby Sanctum)
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
        // TODO: Add top message to greet the player and acknowledge which progression they're currently on

        // Creating Menu Items
        AddGossipItemFor(
            player,
            GOSSIP_ICON_INTERACT_1,
            "|T" + GetTierIcon(PROGRESSION_SERVICE_TIER::START) + ":50:50|tProgress to " + GetTierName(PROGRESSION_SERVICE_TIER::START),
            GOSSIP_SENDER_MAIN,
            static_cast<uint32>(PROGRESSION_SERVICE_TIER::START)
        );

        AddGossipItemFor(
            player,
            GOSSIP_ICON_INTERACT_1,
            "|T" + GetTierIcon(PROGRESSION_SERVICE_TIER::MOLTEN_CORE) + ":50:50|tProgress to " + GetTierName(PROGRESSION_SERVICE_TIER::MOLTEN_CORE),
            GOSSIP_SENDER_MAIN,
            static_cast<uint32>(PROGRESSION_SERVICE_TIER::MOLTEN_CORE)
        );

        // TODO: Add rest of Gossip Menus once we prove this works

        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

        return true;
    }

    bool OnGossipSelect( Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        // TODO: Here we need to check if the player can purchase it, here is where we do the purchase
        ApplyProgressionTierPurchase(player, action);

        // TODO: Make it a purchase and check config to see if it's free or not

        CloseGossipMenuFor(player);
        return true;
    }

    void ApplyProgressionTierPurchase(Player* player, uint32 newTier)
    {
        sIndividualProgression->ForceUpdateProgressionState(player, static_cast<ProgressionState>(newTier));
        sIndividualProgression->checkIPPhasing(player, player->GetAreaId());

        ChatHandler(player->GetSession()).PSendSysMessage("You have successfully progressed to {}.", GetTierName(static_cast<PROGRESSION_SERVICE_TIER>(newTier)));
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
          return "Interface/Icons/achievement_dungeon_karazhan";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_1:
          return "Interface/Icons/achievement_boss_princemalchezaar_02";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_2:
          return "Interface/Icons/achievement_boss_kael'thassunstrider_01";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_3:   // Note Zul Aman is not in the Individual Progression mod, so this should never be called
          return "Interface/Icons/inv_misc_questionmark";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_4:
          return "Interface/Icons/achievement_boss_illidan";
        case PROGRESSION_SERVICE_TIER::TBC_TIER_5:
          return "Interface/Icons/achievement_boss_kiljaeden";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_1:
          return "Interface/Icons/achievement_boss_kelthuzad_01";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_2:
          return "Interface/Icons/achievement_boss_yoggsaron_01";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_3:
          return "Interface/Icons/achievement_boss_anubarak_01";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_4:
          return "Interface/Icons/achievement_boss_lichking";
        case PROGRESSION_SERVICE_TIER::WOTLK_TIER_5:
          return "Interface/Icons/achievement_boss_halion";
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
        case PROGRESSION_SERVICE_TIER::TBC_TIER_3: // Note Zul Aman is not in the Individual Progression mod, so this should never be called
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
