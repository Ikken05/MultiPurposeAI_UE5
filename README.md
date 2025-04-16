# 🧠 Modular Enemy Spawner System – Unreal Engine 5

The **Enemy Spawner System** is a flexible, data-driven solution for dynamically spawning enemies in Unreal Engine 5. It supports behavior tree assignment, AI perception, custom components, and visual debug utilities—all driven by data assets.

---

## 🧑‍💻 Author

👤 **[Mahdi Ikken]**  
🎮 Gameplay Programmer | AI Systems | Unreal Engine 5  
⏺️ Youtube Video will soon be uploaded on my [Youtube Channel](https://www.youtube.com/@Hissatsui)  
📫 Reach me on [LinkedIn](https://www.linkedin.com/in/ikken-mahdi/)  
🔗 My Links [LinkTree](https://linktr.ee/Ikken_)  

## 🚀 Features

- ✅ Modular enemy spawning via `UCharacterDataAsset`
- 🧩 Supports any `ACharacter`-derived class
- 🎮 Assigns AI Controller, Behavior Trees, and Subtrees dynamically
- 👁️ Dynamic AI Perception with multiple senses
- ⚙️ Component injection (health, damageable, etc.)
- 🔧 Visual debug rendering in-editor
- 🧠 Blackboard setup per AI state
- 🔁 Easily expandable and designer-friendly

---

## 🧱 Core Class: `AEnemySpawner`

### Key Responsibilities:
- Spawning enemies at specified transforms
- Initializing their meshes, animation blueprints, behavior trees, and components
- Assigning perception systems and binding delegates
- Debug visualization (spheres and arrows)

### Public Properties

| Property              | Description                              |
|-----------------------|------------------------------------------|
| `EnemiesToSpawn`      | Array of spawn entries                   |
| `bShowSpawnPoints`    | Renders visual indicators                |
| `DefaultSkeletalMesh` | Used if mesh not set in data asset       |
| `DefaultAnimBlueprint`| Used if AnimBP not set in data asset     |

---

## 🧠 `UCharacterDataAsset` Breakdown

This data asset powers the entire setup:

| Field                 | Description                                  |
|-----------------------|----------------------------------------------|
| `CharacterClass`      | Enemy type to spawn                          |
| `CharacterMesh`       | Optional skeletal mesh override              |
| `AnimationBlueprint`  | Optional animation override                  |
| `MainBT`              | Main behavior tree                           |
| `Subtrees`            | Map of AI states to behavior trees           |
| `DefaultStartState`   | AI state on spawn                            |
| `ComponentsToAdd`     | List of extra components                     |
| `bEnableComponents`   | Whether to add components                    |
| `MaxHealth`           | Health initialized on spawn                 |
| `SensesConfig`        | AI perception senses                         |
| `DominantSense`       | Main sense used                              |

---

## ⚙️ System Flow

```plaintext
graph TD;
    A[BeginPlay] --> B[Loop Through EnemiesToSpawn]
    B --> C[Spawn Character]
    C --> D[Assign Mesh / AnimBP]
    D --> E[Run Behavior Tree]
    E --> F[Initialize Blackboard with Subtrees]
    F --> G[Add Components]
    G --> H[Setup Perception]
```
## 🔬 Perception System

- Uses `UAIPerceptionComponent`
- Senses configured dynamically from the data asset
- Binds `OnPerceptionUpdated` for AI reactions

### You can use this to:
- Trigger combat mode
- Alert nearby enemies
- Change AI states

---

## 🎨 Debug

Set `bShowSpawnPoints = true` to:
- Draw spheres at spawn positions
- Draw arrows for forward direction
- Helps visualize horde formation or ambush layouts

---

## ✅ Example Usage (in Editor)

1. Drag an `EnemySpawner` into your level.
2. Add entries to `EnemiesToSpawn` via the details panel.
3. Set each enemy’s data asset and transform.
4. Enable `bShowSpawnPoints` to debug positions.
5. Hit Play – enemies spawn and behave according to their data assets.

---

## 📌 Recommended Extensions

- Health/Death Component integration
- Staggered wave spawning (timeline-based)
- Global Horde Manager with group alerts
- Blueprint editor utility widget for managing spawn points

---

## 📁 Folder Structure Recommendation
```plaintext
Source/
├── EnemySpawner/
│   ├── EnemySpawner.h / .cpp
│   ├── CharacterDataAsset.h / .cpp
│   └── ...
Content/
├── DataAssets/
│   ├── Goblin.asset
│   ├── Brute.asset
├── BehaviorTrees/
├── Components/
```
## 🛠️ Requirements:

* Unreal Engine 5+

* Project should support AI Module (AIPerception, BehaviorTree, etc.)

* Character class must support custom components (e.g., health)
