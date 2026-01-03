# GitHub Copilot Instructions for VRExpPluginExample

## Project Overview

This is a VR educational project built on **Unreal Engine 5.3** featuring a complete **Rocket Assembly VR System**. The project combines C++ core systems with Blueprint visual scripting to create an immersive educational experience for teaching rocket assembly through virtual reality.

### Key Features
- Interactive rocket part assembly with VR controllers
- Educational content system with bilingual support (Chinese/English)
- Physics-based assembly validation with precision scoring
- Snap-to-socket system with magnetic alignment
- Multi-state highlighting and visual feedback

## Tech Stack

- **Engine**: Unreal Engine 5.3
- **Languages**: C++ (core systems) + Blueprint (game logic)
- **VR Framework**: VRExpansionPlugin + OpenXR
- **Plugins**: 
  - VRExpansionPlugin (VR interaction core)
  - OpenXRExpansionPlugin (OpenXR extensions)
  - OpenXRHandTracking (hand tracking support)
  - AdvancedSessions (multiplayer support)
- **Platforms**: Windows PC VR, Meta Quest (Android), HoloLens, Linux VR
- **IDE**: Visual Studio 2022

## Build Instructions

### Prerequisites
- Unreal Engine 5.3 installed
- Visual Studio 2022 with C++ development tools
- VR headset (optional for development)

### Building the Project
1. Right-click `VRExpPluginExample.uproject` → "Generate Visual Studio project files"
2. Open `VRExpPluginExample.sln` in Visual Studio 2022
3. Select "Development Editor" configuration
4. Build the project (F7 or Build → Build Solution)
5. Run the editor (F5 or double-click `.uproject` file)

### Build Configuration
- **Development Editor**: For development in Unreal Editor
- **Development**: For packaged builds with debugging
- **Shipping**: For final release builds

## Code Structure

### Directory Layout
```
VRExpPluginExample_AI/
├── Config/                           # Project configuration files
├── Content/                          # Content assets (models, materials, blueprints)
│   ├── RocketAssembly/              # Rocket assembly system assets
│   │   ├── Blueprints/              # Blueprint classes
│   │   ├── UI/                      # Widget blueprints
│   │   ├── Data/                    # Data tables (CSV templates)
│   │   ├── Materials/               # Materials and material instances
│   │   ├── Meshes/                  # 3D models
│   │   ├── Audio/                   # Sound effects and music
│   │   ├── VFX/                     # Visual effects
│   │   └── Maps/                    # Game levels
│   └── VRE/                         # VR base assets
├── Plugins/                          # Third-party plugins
│   ├── VRExpansionPlugin/           # Main VR plugin
│   └── AdvancedSessions/            # Session management
└── Source/                           # C++ source code
    └── VRExpPluginExample/
        └── RocketAssembly/          # Rocket assembly C++ classes
```

### Core C++ Classes

#### 1. RocketPartData.h (Data Layer)
- **Location**: `Source/VRExpPluginExample/RocketAssembly/RocketPartData.h`
- **Purpose**: Defines all data structures for the rocket assembly system
- **Key Types**:
  - `ERocketPartType`: 20+ rocket part types (engines, fuel tanks, body sections, etc.)
  - `EAssemblyState`: Assembly state enumeration
  - `FRocketPartData`: Complete part data structure (for Data Tables)
  - `FAssemblyStepData`: Assembly step definition
  - `FRocketKnowledgeData`: Educational knowledge structure

#### 2. InteractiveRocketPart (Game Logic Layer)
- **Files**: `InteractiveRocketPart.h/cpp`
- **Inheritance**: `AGrippableStaticMeshActor` → `AInteractiveRocketPart`
- **Purpose**: VR-grippable rocket part with assembly logic
- **Key Features**:
  - VR grab/release integration
  - Multi-state highlighting (idle, hover, can-assemble, error)
  - Assembly state management
  - Part locking/unlocking
  - Dependency checking
  - Event delegates (OnPartAssembled, OnAssemblyFailed, etc.)

#### 3. AssemblySocket (Component)
- **Files**: `AssemblySocket.h/cpp`
- **Inheritance**: `USceneComponent` → `UAssemblySocket`
- **Purpose**: Attachment point for part assembly
- **Key Features**:
  - Part type and ID validation
  - Alignment detection (position and rotation with configurable tolerance)
  - Magnetic snap effect with smooth interpolation
  - Visual indicator system (color-coded states)
  - Nearby part detection
  - Event system (OnPartEnterRange, OnPartAssembled, etc.)

#### 4. AssemblyValidator (Validation System)
- **Files**: `AssemblyValidator.h/cpp`
- **Inheritance**: `UObject` → `UAssemblyValidator`
- **Purpose**: Validates assembly correctness and calculates precision scores
- **Key Features**:
  - Position and rotation validation
  - Assembly sequence validation
  - Dependency relationship checking
  - Collision detection
  - Precision scoring (0-100 scale: position 0-50, rotation 0-50)
  - Detailed validation results with error messages

## Coding Conventions

### C++ Style Guidelines

1. **Naming Conventions**:
   - Classes: PascalCase with prefix (e.g., `AInteractiveRocketPart`, `UAssemblySocket`)
   - Enums: PascalCase with `E` prefix (e.g., `ERocketPartType`)
   - Structs: PascalCase with `F` prefix (e.g., `FRocketPartData`)
   - Member variables: PascalCase (e.g., `PartType`, `bIsAssembled`)
   - Functions: PascalCase (e.g., `ValidateAssembly`, `GetPartData`)
   - Constants: PascalCase with appropriate prefix

2. **Comments**:
   - Use bilingual comments (Chinese/English) for major sections
   - Document all public APIs with Doxygen-style comments
   - Example:
     ```cpp
     /**
      * 验证零件装配是否有效
      * Validates if part assembly is valid
      */
     bool ValidateAssembly();
     ```

3. **UPROPERTY Specifiers**:
   - Use `EditAnywhere, BlueprintReadWrite` for properties that should be editable in editor and accessible in Blueprint
   - Use `BlueprintAssignable` for event delegates
   - Always specify `Category` for organization in editor
   - Example:
     ```cpp
     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assembly")
     float PositionTolerance = 5.0f;
     ```

4. **Header Organization**:
   - Copyright notice at top
   - `#pragma once` for header guards
   - Include CoreMinimal.h first
   - Use forward declarations when possible
   - End with `.generated.h` include for UCLASS/USTRUCT

5. **Blueprint Integration**:
   - Mark functions with `BlueprintCallable` when they should be accessible from Blueprint
   - Use `BlueprintPure` for const getter functions
   - Provide meaningful `DisplayName` and `ToolTip` metadata

### Blueprint Naming
- Prefix with `BP_` (e.g., `BP_RocketPart_Base`)
- Use descriptive names (e.g., `BP_Part_MainEngine`, `BP_AssemblySocket`)
- Organize in appropriate folders under `Content/RocketAssembly/Blueprints/`

### Data Table Naming
- Prefix with `DT_` (e.g., `DT_RocketParts`, `DT_AssemblySteps`)
- Store in `Content/RocketAssembly/Data/`
- Provide CSV templates for easy editing

## Important Files

### Documentation
- **README.md**: Project overview and quick start
- **DEVELOPER_GUIDE.md**: Comprehensive API reference and development workflows
- **IMPLEMENTATION_GUIDE.md**: Step-by-step implementation guide
- **PROJECT_SUMMARY.md**: Complete project summary with all implemented features

### Configuration
- **VRExpPluginExample.uproject**: Project definition with plugin list
- **Config/DefaultEngine.ini**: Engine configuration
- **Config/DefaultInput.ini**: Input bindings

### Build Files
- **Source/VRExpPluginExample/VRExpPluginExample.Build.cs**: Build configuration
  - Dependencies: Core, CoreUObject, Engine, InputCore, AdvancedSessions, VRExpansionPlugin
  - Private dependencies: Slate, SlateCore, UMG

## Development Workflows

### Adding a New Rocket Part Type

1. Add enum value to `ERocketPartType` in `RocketPartData.h`
2. Create Blueprint child of `BP_RocketPart_Base` (when implemented)
3. Add row to `DT_RocketParts` data table
4. Configure 3D mesh, materials, and physics properties
5. Set up assembly socket references and dependencies

### Creating an Assembly Sequence

1. Define steps in `DT_AssemblySteps` data table
2. Reference part IDs and socket IDs
3. Add educational content (text, audio, video references)
4. Configure AR guidance settings
5. Test in-game with validation system

### Debugging VR Interaction

1. Enable VR Preview in editor (PIE → VR Preview)
2. Use `DrawDebugString` and `DrawDebugSphere` for visual debugging
3. Check Output Log for validation messages
4. Use Blueprint debugging with breakpoints
5. Test with motion controllers or simulate in editor

## Testing

### Current Status
- Phase 1 (Core C++ System) is complete and compiles
- Phases 2-6 (Blueprint implementation, UI, content) are planned

### Testing Guidelines
- **Compilation**: Ensure C++ code compiles without errors before committing
- **Blueprint Compilation**: Check for Blueprint compilation errors in editor
- **VR Testing**: Test grab/release, assembly, and validation in VR when available
- **Data Validation**: Verify data table entries are complete and correctly formatted

## Common Tasks

### Working with Data Tables
- Data tables use CSV format for easy external editing
- Structure types defined in `RocketPartData.h`
- Import CSV via: Right-click Data Table → Reimport
- Export changes: Right-click Data Table → Export as CSV

### Extending the System
- New part types: Add to `ERocketPartType` enum
- New validation rules: Extend `UAssemblyValidator`
- New socket behaviors: Extend `UAssemblySocket`
- New part features: Override in `AInteractiveRocketPart` child classes

### Localization
- Use `FText` for all user-facing strings
- Provide both Chinese (CN) and English (EN) versions
- Access via `GetLocalizedPartName()` and `GetLocalizedPartDescription()`
- Store in data tables for easy translation

## Plugin Dependencies

### VRExpansionPlugin
- Provides core VR interaction components
- Base class: `AGrippableStaticMeshActor` (used by `AInteractiveRocketPart`)
- Handle grab/release events in derived classes
- Documentation: https://vreue4.com/

### OpenXR
- Cross-platform VR API
- Configured in project settings
- Supports multiple VR headsets (Quest, Vive, Index, etc.)

## Performance Considerations

- Use object pooling for frequently spawned/destroyed parts
- Optimize collision meshes (use simple collision)
- Limit number of concurrent `DrawDebug` calls
- Use LODs for complex rocket part meshes
- Batch assembly validation checks when possible

## Common Pitfalls

1. **Forgetting to regenerate project files** after adding new C++ classes
2. **Not marking UCLASS/USTRUCT/UENUM types** with appropriate macros
3. **Missing `.generated.h` include** in header files
4. **Incorrect inheritance** - ensure proper Unreal base classes
5. **Blueprint compilation errors** after C++ changes - may need to recompile Blueprints
6. **VR controller setup** - ensure input bindings are configured in project settings

## Additional Resources

- [Unreal Engine Documentation](https://docs.unrealengine.com/)
- [VRExpansionPlugin Documentation](https://vreue4.com/)
- [OpenXR Documentation](https://www.khronos.org/openxr/)
- Project documentation in root directory (DEVELOPER_GUIDE.md, etc.)

---

**Last Updated**: 2026-01-03
**Project Version**: 1.0.1 - Foundation Complete
