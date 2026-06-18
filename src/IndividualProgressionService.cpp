/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "IndividualProgression.h"


enum class PROGRESSION_STATE{
    START,
    MOLTEN_CORE,
    ONYXIA
}

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
        AddGossipItemFor(
            player,
            GOSSIP_ICON_INTERACT_1,
            "|T" + getTierIcon(PROGRESSION_STATE::START) + ":50:50|tProgress to " + getTierName(PROGRESSION_STATE::START),
            GOSSIP_SENDER_MAIN,
            PROGRESSION_STATE::START)

        return true;
    }

    bool OnGossipSelect( Player* player, Creature* creature, uint32 sender, uint32 action)
    {

        return true;
    }

};

    std::string GetTierIcon(uint8 tier)
    {
      switch (tier)
      {
        case PROGRESSION_START:
          return "Interface/Icons/inv_misc_map02";
        case PROGRESSION_MOLTEN_CORE:
          return "Interface/Icons/achievement_boss_ragnaros";
        case PROGRESSION_ONYXIA:
          return "Interface/Icons/achievement_boss_onyxia";
        case PROGRESSION_BLACKWING_LAIR:
          return "Interface/Icons/achievement_boss_nefarion";
        case PROGRESSION_PRE_AQ:
          return "Interface/Icons/inv_misc_qirajicrystal_05";
        case PROGRESSION_AQ_WAR:
          return "Interface/Icons/inv_misc_qirajicrystal_01";
        case PROGRESSION_AQ:
          return "Interface/Icons/achievement_boss_cthun";
        case PROGRESSION_NAXX40:
          return "Interface/Icons/achievement_boss_kelthuzad_01";
        case PROGRESSION_PRE_TBC:
          return "Interface/Icons/achievement_dungeon_karazhan";
        case PROGRESSION_TBC_TIER_1:
          return "Interface/Icons/achievement_boss_princemalchezaar_02";
        case PROGRESSION_TBC_TIER_2:
          return "Interface/Icons/achievement_boss_kael'thassunstrider_01";
        case PROGRESSION_TBC_TIER_4:
          return "Interface/Icons/achievement_boss_illidan";
        case PROGRESSION_TBC_TIER_5:
          return "Interface/Icons/achievement_boss_kiljaeden";
        case PROGRESSION_WOTLK_TIER_1:
          return "Interface/Icons/achievement_boss_kelthuzad_01";
        case PROGRESSION_WOTLK_TIER_2:
          return "Interface/Icons/achievement_boss_yoggsaron_01";
        case PROGRESSION_WOTLK_TIER_3:
          return "Interface/Icons/achievement_boss_anubarak_01";
        case PROGRESSION_WOTLK_TIER_4:
          return "Interface/Icons/achievement_boss_lichking";
        case PROGRESSION_WOTLK_TIER_5:
          return "Interface/Icons/achievement_boss_halion";
        default:
          return "Interface/Icons/inv_misc_questionmark";
      }
    }

    std::string GetProgressionTierName(uint8 tier)
    {
      switch (tier)
      {
        case PROGRESSION_START:
          return "Start";
        case PROGRESSION_MOLTEN_CORE:
          return "Molten Core";
        case PROGRESSION_ONYXIA:
          return "Onyxia's Lair";
        case PROGRESSION_BLACKWING_LAIR:
          return "Blackwing Lair";
        case PROGRESSION_PRE_AQ:
          return "Pre-AQ";
        case PROGRESSION_AQ_WAR:
          return "AQ War Effort";
        case PROGRESSION_AQ:
          return "Ahn'Qiraj";
        case PROGRESSION_NAXX40:
          return "Naxxramas (Vanilla)";
        case PROGRESSION_PRE_TBC:
          return "Pre-TBC";
        case PROGRESSION_TBC_TIER_1:
          return "TBC Tier 1";
        case PROGRESSION_TBC_TIER_2:
          return "TBC Tier 2";
        case PROGRESSION_TBC_TIER_4:
          return "TBC Tier 4";
        case PROGRESSION_TBC_TIER_5:
          return "TBC Tier 5";
        case PROGRESSION_WOTLK_TIER_1:
          return "WotLK Tier 1";
        case PROGRESSION_WOTLK_TIER_2:
          return "WotLK Tier 2";
        case PROGRESSION_WOTLK_TIER_3:
          return "WotLK Tier 3";
        case PROGRESSION_WOTLK_TIER_4:
          return "WotLK Tier 4";
        case PROGRESSION_WOTLK_TIER_5:
          return "WotLK Tier 5";
        default:
          return "Unknown Tier";
      }
    }


// Add all scripts in one
void AddIndividualProgressionServiceScripts()
{
    new MyPlayer();
    new MyWorldScript();
}
