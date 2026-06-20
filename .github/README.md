# mod-individual-progresssion-service

## About
- This mod provides a spawnable NPC that allows the user to set their progression tier in the [Individual Progression](https://github.com/ZhengPeiRu21/mod-individual-progression) mod by ZhengPeiRu21 quickly and easily. The intent is to provide a self-service and in-game solution to jumping forward (or going backwards) in the progression system. **Note: This does NOT handle attunement/granting items typically required for Vanilla & TBC raids** (ex: Needing the Drakefire Amulet in your inventory for Vanilla Onyxia). If there is a desire for that, I will happily explore it in the future, so please feel free to provide any feedback you have on that or anything else!

## Features
- Spawnable progression tier selection npc
- Supports all Individual Progression tiers (including resetting to start)
- Currently all tiers are free to purchase with no requirements (to be configurably priced in later versions)

## Installation
- No special tricks! Simply place the mod-individual-progression-service folder into the /azerothcore-wotlk-repo/modules/ folder, then re-build and re-compile
- After compiling, clone the individual-progression-service.conf.dist file under /configs/modules into the same folder and edit the name to be individual-progression-service.conf (not ".dist"). Make an configuration changes in the new file
- Add the related NPC to the database with the included .sql script
- They can be added into the game with the command ".npc add 371001" once in the world database
- Enjoy!

## Notes
- The list of options in the NPCs dialog are presented in a default blizz-like succession, but [Individual Progression](https://github.com/ZhengPeiRu21/mod-individual-progression) provides configuration that can modify properties such as required tier for Zul'Aman and Death Knight.
