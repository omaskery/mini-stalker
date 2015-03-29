# To-Do list #

# Map Editor - Complete #

# Main Game #

  1. Represent the player in the game world in some asthetically pleasing manner (aka: finish hooking up the players drawing to animations that correspond to his actions)
  1. Make it possible to view the players inventory
  1. Put stashes on the map to use with testing
  1. Develop suitable interface for allowing items to be transfered to and from the stash from the players inventory
  1. Add static NPC's to the game (e.g traders like Siddrovich from the real game), make it possible to trade with them using a similar interface to the stash interface, tweaked to charge/give you money
  1. Add moving NPC's to the game such as mutants and various characters, NPC's will be given factions (Loners, Freedom, Duty, Bandits, Mutants, etc...),
  1. Implement attacking in the player and NPC's, all factions will have some kind of virtual "slider" dictating their attitude towards other factions - this will be used to decide how they will react to that faction. This slider will be used for everybody inside that faction, if the player is a Freedom member and shoots a duty member, Duty will start to dislike Freedom - regardless of who it is. If the Duty shoots a Freedom member as revenge, it doesn't have to be the player, then Duty will start to dislike Freedom - creating a loop.
  1. Implement some kind of missions system, these would be given by static NPC's who act as traders and mission providers. They would most suitably be placed deep within a town/compound so they don't get in situations where they have to interact with enemies - since they won't be programmed to interact with enemies.
  1. And more when these tasks are done...

# Notes #

  * On the topic of world being populated with NPC's, it would be best if special entities were placed on the map that indicated a "spawn zone" or "control point". These places would be allocated to a specific faction, and would allow for NPC's of that faction to spawn there. This would then allow for enemy factions to attack a control/spawn point and if they kill all of the enemies there, it will become theirs. This of course brings about the issue of one faction dominating, with certain factions unable to spawn. In these times those factions will spawn on the edges of maps in groups - acting as "raiding parties" possibly. This could be implemented by adding another special entity to maps dictating where such "neutral" spawn points could be placed. Another important note is that Mutants will not use this system at all and will have their own spawn indicators.
  * Other notes may follow...