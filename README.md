# Modular 3D Platformer Template (UE5)

## Overview
A single-player 3D platformer framework inspired by classic games.  
Includes modualr and scalable systems. Implemented in Unreal Engine 5.3 Blueprints and C++.

## Features
- Combat system
- Interaction system  
- Saving/loading system  
- Observer system  
- Invenotry system
- Enhanced spring arm and character movement components
- AI-cotnrolled enemies
- UI assets with controller and keyboard icons  
- Includes original 3D models, textures, and a custom font    

## Project Structure

### Content folder structer
```
Content/
 |- Blueprints/
 |  |- AI/                                  # Contains all AI related BPs (Blackboard, BehaviourTrees, Tasks, and e.t.c.)
 |  |- AttackTypes/                         # Contains all attack types
 |  |- Characters/                          # Contains character BPs (Player, enemies)
 |  |- Effects/                             # Contains all effect types
 |  |- Environment/                         # Contains BPs that can be placed in world (Doors, destructible actors, interacable actors, and e.t.c.)
 |  |- GameModes/                           # Contains Game Modes
 |  |- Input/                               # Contains input data (input actions, mapping contexts)
 |  |- PlayerControllers/                   # Contains Player Controllers
 |  |- Projectiles/                         # Contains Projectiles
 |  |- UI/                                  # Contains widgets
 |- CommonUI/                               # Contains CommonUI data and textures for different devices
 |- Fonts/                                  # Contains fonts
 |- Maps/                                   # Contains maps
 |- Materials/                              # Contains materials
 |- Mesh/                                   # Contains all static and skeletal meshes
 |- Miscellaneous/                          # Contains all data objects (data assets, data tables)
 |- VFX/                                    # Contains VFX
```

### C++ Source folder structer
```
Source/
 |- ActorComponents/
 |  |- AIComponents/                        # Components only used by AI controlled pawns
 |  |- CharacterMovementComponents/         # Custom character movement component with movement states and floating function
 |  |- CollisionComponents/
 |  |   |- StompCollisions/                 # Collisions that detects stomping
 |  |- CombatComponents/                    # Component that handles combat logic
 |  |- EffectComponent/                     # Base effect component for using effects
 |  |- HealthComponent/                     # Component that handles health changes
 |  |- InteractableComponents/              # Components for interactable and interacting actors
 |  |- InventoryComponent/                  # Component that manages inventory
 |  |- ObserverComponents/                  # Observer and observable components. The observer component observes changes in the observable component
 |  |- SpringArmComponents/                 # Custom spring arm component with zooming and rotation alignment functions      
 |  |- WalletComponent/                     # Component that handles currency
 |- Animations/
 |  |- AnimInstances/                       # Custom AnimInstance with common locomotion fields
 |- AsyncActions/                           # Async actions with exposed pins for blueprints
 |- Characters/                             # Base character classes
 |- Controllers/
 |  |- AIControllers/                       # Base AI controller
 |  |- PlayerControllers/                   # Base player controller
 |- CoreTypes/
 |  |- AttackTypes/                         # Attack types that can be performed with CombatComponent
 |  |- Effects/                             # Effects that can be applied with EffectComponent
 |  |- InventoryTypes/                      # Structs that are used in inventory system
 |  |- ObservingTypes/
 |  |   |- ObservableDataTypes/             # Contains wrapping structs to store ObservableState values
 |  |   |- ObservableStates/                # States of ObservableComponent
 |  |   |- ObserverConditions/              # Condition checkers to validate if ObserverComponent state is set to specified value
 |  |- PatrolTypes/                         # Patrol types used to get next patrol locations
 |- EnvironmentActors/
 |  |- Checkpoints/                         # Checkpoint that saves current progress
 |  |- Collectables/                        # Actors that can be collected
 |  |- DecalVolume/                         # Actor with collision and decal component sized to match the collision 
 |  |- InteractableActors/                  # Base interactable actor for faster interactables creation
 |  |- Platforms/                           # Platforms that can be placed in level
 |- FunctionLibrary/                        # Blueprint function library used in this project
 |- GameModes/                              # Base game mode class that handles saving and loading
 |- Interfaces/                             # Interfaces used in this project
 |- ObjectPool/                             # Object pool UObject class used for efficient spawning and using actors
 |- Projectiles/                            # Projectile actors that can be launched at the specified direction
 |- SaveGame/                               # Save game class that stores saved data
 |- Subsystems/                             # Subsystems used in this project
```

## Getting Started
1. Extract this folder into your Unreal Projects directory.  
2. Open the project with Unreal Engine 5.3+  
3. Load the map `DemoMap` to start exploring the systems.  
4. Use **Play in Editor (PIE)** to test mechanics.  
