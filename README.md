# Modular 3D Platformer Template (UE5)

## Overview
A fully modular single-player 3D platformer framework inspired by classic games.  
Includes systems for player movement, combo attacks, interaction, saving/loading, AI, and event observation. Implemented in Unreal Engine 5 Blueprints and C++.

// TODO
## Features
- Modular character controller with platforming mechanics  
- Combo attack and interaction system  
- Saving/loading and observer event systems  
- UI framework with controller and keyboard icons  
- Includes original 3D models, textures, and a custom font  
- Clean folder structure and documented Blueprints  

## Project Structure

### Content folder structer
```
Content/
|- Blueprints/
|  |- AI/
|  |  |- BehaviorTrees/                     # Enemy AI behavior trees
|  |  |- Blackboards/                       # Blackboards
|  |  |- Controllers/                       # AI controllers
|  |  |- Decorators/                        # Behavior tree decorators
|  |  |- EQS/                               
|  |  |- Services/                          # Behavior tree services
|  |  |- Tasks/                             # Behavior tree tasks
|  |- AttackTypes/                          # Attack types for combat
|  |- Characters/
|  |  |- Enemies/                           # AI controlled enemies 
|  |  |- Player/                            # Player controlled enemies
|  |- Effects/                              # Effects that can be applied on actors
|  |- Environment/
|  |  |- Checkpoints/                       # Checkpoint actors used to save current state
|  |  |- Collectable/                       # Collectable actors
|  |  |- Damage/                            # Actors that deal damage
|  |  |- Damageable/                        # Actors that can be damaged
|  |  |- Doors/                             # Different types of doors
|  |  |- Interactable/                      # Interactable actors
|  |  |- Mushrooms/                         # Mushroom that can launch player
|  |  |- Observing/
|  |  |  |- Observable/                     # Actors that have ObservableComponent
|  |  |  |- Observer/                       # Actors that have ObserverComponent
|  |  |- OneWayLeaf/                        # Disable its surface and can re-enable after a specifed amount of time
|  |  |- Platforms/                         # Different platform types 
|  |  |- Surfaces/                          # Actors with decal and collision components
|  |- GameModes/                            # Game modes used in this project
|  |- Input/                                # Input actions and input mapping contexts used in this project
|  |- PlayerControllers/                    # Base player controller 
|  |- Projectiles/                          # Projectiles that can be launched at a specified direction 
|  |- Test                                  # Actors used for testing during development
|  |- UI                                    # UI widgets
|- CommonUI/                                # Contains data objects and textures used for CommonUI plugin
|- Fonts/                                   # Fonts used in this project
|- Maps/
|- Materials/
|  |- Decals/                               # Decal materials
|  |- Demonstration/                        # Materials used to visualize changes during tests
|  |- Environment/                          # Materials used for walls, ground
|  |- MaterialFunctions/                    # Material functions used in different materials
|  |- MaterialMasters/                      # Common master materials
|  |- UI/                                   # UI materials
|- Mesh/
|  |- Blocks/                               # Meshes used for building levels (blocks, stairs, planes, etc.)
|  |- Characters/
|  |  |- Enemies/                           # All enemy skeletal meshes
|  |  |- Seed/                              # Player character skeletal mesh
|  |- Checkpoints/                          # Checkpoint mesh
|  |- Destructible/                         # Meshes of destructible actors
|  |- Doors/                                # Door frame and panel meshes
|  |- Foliage/                              # Foliage meshes (flowers, grass, rocks)
|  |- GrassSpring/                          # Grass spring meshes
|  |- Interactable/                         # Meshes of interactable actors
|  |- Mushrooms/                            # Launching mushroom meshes
|  |- OneWayLeaf/                           # One way leaf mesh
|  |- Pickups/                              # Meshes of pickup actors
|  |- Platforms/                            # Meshes of Different platform types 
|  |- Projectile/                           # Projectile mesh
|  |- ThornStick/                           # Thorn stick mesh
|  |- WoodenPressurePlane/                  # Wooden pressure plane mesh
|- VFX/                                     # VFX used for different actors 
```

### C++ Source folder structer
```
Source/
|- ForestPlatformer/
|  |- ActorComponents/
|  |  |- AIComponents/                      # Components only used by AI controlled pawns
|  |  |- CharacterMovementComponents/       # Custom character movement component with movement states and floating function
|  |  |- CollisionComponents/
|  |  |  |- StompCollisions/                # Collisions that detects stomping
|  |  |- EffectComponent/                   # Base effect component for using effects
|  |  |- HealthComponent/                   # Component that handles health changes
|  |  |- ObserverComponents/                # Observer and obsevable components. The observer component observes changes in the observable component 
|  |  |- SpringArmComponents/               # Custom spring arm component with zooming and rotation alignment functions 
|  |- AI/
|  |  |- Tasks/                             # Custom tasks for behavior tree
|  |- Animations
|  |  |- AnimInstances/                     # Custom AnimInstance with common locomotion fields
|  |- AsyncActions/                         # Async actions with exposed pins for blueprints
|  |- Characters/                           # Base character classes
|  |- Controllers/
|  |  |- AIControllers/                     # Base AI controller 
|  |  |- PlayerControllers/                 # Base player controller
|  |- CoreTypes/
|  |  |- AttackTypes/                       # Attack types that can be performed with CombatComponent
|  |  |- Effects/                           # Effects that can be applied with EffectComponent
|  |  |- ObservingTypes/
|  |  |  |- ObservableDataTypes/            # Contains wrapping structs to store ObservableState values
|  |  |  |- ObservableStates/               # States of ObservableComponent
|  |  |  |- ObserverConditions/             # Condition checkers to validate if ObserverComponent state is set to specified value
|  |  |- PatrolTypes/                       # Patrol types used to get next patrol locations
|  |- EnvironmentActors/
|  |  |- Checkpoints/                       # Checkpoint that saves current progress
|  |  |- Collectables/                      # Actors that can be collected
|  |  |- DecalVolume/                       # Actor with collision and decal component sized to match the collision 
|  |  |- InteractableActors/                # Base interactable actor for faster interactables creation
|  |  |- Platforms/                         # Platforms that can be placed in level
|  |- FunctionLibrary/                      # Blueprint function library used in this project
|  |- GameModes/                            # Base game mode class that handles saving and loading
|  |- Interfaces/                           # Interfaces used in this project
|  |- ObjectPool/                           # Object pool UObject class used for efficient spawning and using actors
|  |- Projectiles/                          # Projectile actors that can be launched at the specified direction
|  |- SaveGame/                             # Save game class that stores saved data
|  |- Subsystems/                           # Subsystems used in this project
```

Source/
 |- AI/
 |  |- Tasks/
 |- ActorComponents/
 |  |- AIComponents/
 |  |- CharacterMovementComponents/
 |  |- CollisionComponents/
 |  |   |- StompCollisions/
 |  |- CombatComponents/
 |  |- EffectComponent/
 |  |- HealthComponent/
 |  |- InteractableComponents/
 |  |- InventoryComponent/
 |  |- ObserverComponents/
 |  |- SpringArmComponents/
 |  |- WalletComponent/
 |- Animations/
 |  |- AnimInstances/
 |- AsyncActions/
 |- Characters/
 |- Controllers/
 |  |- AIControllers/
 |  |- PlayerControllers/
 |- CoreTypes/
 |  |- AttackTypes/
 |  |- Effects/
 |  |- InventoryTypes/
 |  |- ObservingTypes/
 |  |   |- ObservableDataTypes/
 |  |   |- ObservableStates/
 |  |   |- ObserverConditions/
 |  |- PatrolTypes/
 |- EnvironmentActors/
 |  |- Checkpoints/
 |  |- Collectables/
 |  |- DecalVolume/
 |  |- InteractableActors/
 |  |- Platforms/
 |- FunctionLibrary/
 |- GameModes/
 |- Interfaces/
 |- ObjectPool/
 |- Projectiles/
 |- SaveGame/
 |- Subsystems/



## Getting Started
1. Extract this folder into your Unreal Projects directory.  
2. Open the project with Unreal Engine 5.3+  
3. Load the map `ExampleLevel` to start exploring the systems.  
4. Use **Play in Editor (PIE)** to test mechanics.  

## Using the Framework
- Extend or modify systems located under `/Blueprints/Core`.  
- Replace placeholder assets with your own.  
- Refer to **Manual.pdf** for in-depth explanations and system flowcharts.  

## License
This product is distributed under the **Epic Games Fab Marketplace EULA**.  
© [Year] [Your Name or Studio]. All rights reserved.  

## Related Products
- [3D Platformer Asset Pack (cross-engine)](your-link-here)

## Support
For feedback or questions, contact:  
[Your Email or Website]