# Phase 3: 多人协作系统实现指南
# Phase 3: Multiplayer Collaboration Implementation Guide

## 目录 Table of Contents
1. Multiplayer Overview 多人系统概述
2. Network Architecture 网络架构
3. Session Management 会话管理
4. Synchronization Systems 同步系统
5. Collaborative Assembly 协作装配
6. Voice Communication 语音通信
7. Player Roles 玩家角色
8. Testing and Optimization 测试与优化

---

## 1. Multiplayer Overview 多人系统概述

### Purpose 目的
Enable 2-4 players to collaborate on rocket assembly in VR, promoting:
- Teamwork and communication 团队合作与沟通
- Division of labor 分工协作
- Educational group learning 教育小组学习
- Social VR experience 社交VR体验

### Features 功能
- **Co-op Assembly**: Multiple players assemble together
- **Role Assignment**: Teacher, Student, Observer roles
- **Voice Chat**: Real-time communication
- **Synchronized State**: All players see same rocket state
- **Shared Progress**: Collaborative achievements

### Technical Stack 技术栈
- **VRExpansionPlugin**: VR multiplayer support
- **AdvancedSessions Plugin**: Session management (already in project)
- **UE5 Replication**: Actor and variable replication
- **Steam/EOS**: Online subsystem (PC)
- **Oculus Platform**: Quest networking (optional)

---

## 2. Network Architecture 网络架构

### Client-Server Model 客户端-服务器模型

```
Server (Host Player) 服务器(主机玩家)
├─ Authoritative 权威
├─ Manages game state 管理游戏状态
├─ Validates all actions 验证所有操作
└─ Replicates to clients 复制到客户端

Clients (Join Players) 客户端(加入玩家)
├─ Send inputs to server 发送输入到服务器
├─ Receive replicated state 接收复制状态
├─ Predict local actions 预测本地操作
└─ Smooth corrections 平滑修正
```

### Replication Groups 复制组

**High Priority 高优先级**:
- Player transforms (position, rotation)
- Held parts (which player holding which part)
- Assembly state (parts assembled/disassembled)

**Medium Priority 中优先级**:
- Part positions (when not held)
- UI state (current tutorial step)
- Audio triggers

**Low Priority 低优先级**:
- Particle effects (spawn locations)
- Non-critical UI updates

---

## 3. Session Management 会话管理

### BP_MultiplayerSessionManager

**File**: `Content/RocketAssembly/Blueprints/Multiplayer/BP_MultiplayerSessionManager.uasset`

#### Variables 变量
```
- SessionName: FName = "RocketAssemblySession"
- MaxPlayers: int32 = 4
- bIsLAN: bool = false (use for local testing)
- CurrentSessionSettings: FBlueprintSessionResult
- PlayerList: Array<APlayerState*>
```

#### Functions 函数

##### HostSession(PlayerName, MaxPlayers)
```
使用 AdvancedSessions 插件:
1. Call CreateAdvancedSession node
2. Settings:
   - SessionName
   - MaxPlayers (2-4)
   - bUsesPresence = true
   - bAllowJoinInProgress = true
   - bIsLANMatch = bIsLAN
3. On success:
   - Open level: L_AssemblyWorkshop_MP?listen
   - Set player as host
   - Enable server-authoritative mode
4. On failure:
   - Show error message
```

##### FindSessions()
```
1. Call FindSessionsAdvanced node
2. Search parameters:
   - MaxResults = 20
   - SearchLAN = bIsLAN
3. On success:
   - Display list of available sessions
   - Show: Host name, players, ping
4. On failure:
   - Show "No sessions found"
```

##### JoinSession(SessionResult)
```
1. Call JoinSession node with SessionResult
2. On success:
   - Travel to server map
   - Set player as client
3. On failure:
   - Show error: "Failed to join"
```

##### LeaveSession()
```
1. Call DestroySession
2. Return to main menu
3. Clean up player references
```

---

## 4. Synchronization Systems 同步系统

### Rocket Part Replication 火箭零件复制

**In AInteractiveRocketPart**:

Add replication:
```cpp
AInteractiveRocketPart::AInteractiveRocketPart()
{
    bReplicates = true;
    bNetLoadOnClient = true;
    SetReplicateMovement(true);
}

// Replicated variables
UPROPERTY(Replicated)
bool bIsAssembled;

UPROPERTY(Replicated)
APlayerController* HoldingPlayer;

UPROPERTY(ReplicatedUsing=OnRep_AssemblyState)
EAssemblyState CurrentState;
```

#### OnGripped Event (Client → Server)
```
Client calls:
  ServerGripPart(PartActor)

Server RPC:
  UFUNCTION(Server, Reliable, WithValidation)
  void ServerGripPart(AActor* Part)
  {
    Validation: Check if part is grippable
    
    Execution:
    1. Set HoldingPlayer = RequestingPlayer
    2. Replicate to all clients
    3. Play grip SFX on all clients (multicast RPC)
  }
```

#### OnAssembled Event (Client → Server → All)
```
Client: Attempts assembly (prediction)
Client calls: ServerAttemptAssembly(Part, Socket)

Server RPC:
  UFUNCTION(Server, Reliable)
  void ServerAttemptAssembly(APart* Part, USocket* Socket)
  {
    1. Validate assembly (position, sequence)
    2. If valid:
       - Set Part.bIsAssembled = true (replicated)
       - Multicast success effects
    3. If invalid:
       - Client corrects position (undo prediction)
       - Multicast failure effects
  }

Multicast RPC:
  UFUNCTION(NetMulticast, Reliable)
  void MulticastAssemblySuccess(APart* Part)
  {
    Play VFX, SFX on all clients
  }
```

---

### Player State Synchronization 玩家状态同步

**Custom APlayerState**:

```cpp
UCLASS()
class ARocketAssemblyPlayerState : public APlayerState
{
    UPROPERTY(Replicated)
    EPlayerRole AssignedRole;  // Teacher, Student, Observer

    UPROPERTY(Replicated)
    FLinearColor PlayerColor;  // Unique color per player

    UPROPERTY(Replicated)
    int32 ContributionScore;  // Parts assembled by this player
};
```

---

## 5. Collaborative Assembly 协作装配

### Division of Labor 分工协作

#### Role System 角色系统

**Teacher Role 教师角色**:
- Can assign tasks to students
- Controls tutorial progression
- Has authority to approve/reject assemblies
- Can reset parts

**Student Role 学生角色**:
- Follows teacher's instructions
- Can assemble assigned parts
- Earns points for correct assembly

**Observer Role 观察者角色**:
- Cannot interact with parts (read-only)
- Can view and learn
- Useful for demonstrations

#### Task Assignment 任务分配

**BP_TaskAssignmentSystem**:

```
Variables:
- AssignedTasks: Map<APlayerState*, Array<FName>> (Player → Part IDs)

Function: AssignTask(PlayerState, PartID)
  1. Add PartID to player's task list
  2. Replicate to all clients
  3. Highlight assigned part for that player (color-coded)
  4. Display notification: "Assemble [Part Name]"

Function: CompleteTask(PlayerState, PartID)
  1. Verify part assembled correctly
  2. Remove from task list
  3. Award points to player
  4. Unlock next task
```

---

### Conflict Resolution 冲突解决

**Problem**: Two players try to grab same part simultaneously

**Solution**: Server-authoritative ownership

```
When PlayerA grips part:
  Client A: Predict grip (immediate feedback)
  Client A → Server: ServerGripPart()
  Server: Validate (first request wins)
  Server → All Clients: Replicate ownership to A

If PlayerB also tries:
  Client B: Predict grip
  Client B → Server: ServerGripPart()
  Server: Reject (already owned by A)
  Server → Client B: Correction (release part)
  Client B: Undo prediction, show message "Part in use"
```

---

## 6. Voice Communication 语音通信

### Built-in Voice Chat VR语音聊天

**Using UE5 Voice Chat**:

```cpp
In BP_MultiplayerGameMode:
  Enable voice chat in session settings
  AllowVoiceChat = true
```

**Configuration**:
- Voice codec: Opus
- Sample rate: 16kHz (sufficient for voice)
- Push-to-talk: Optional (hold button to talk)
- Always-on: Recommended for VR (hands-free)

### Spatial Voice 3D语音

```
VoiceComponent settings:
- Enable 3D spatialization
- Attenuation:
  - Inner Radius: 500 cm (5m - clear)
  - Outer Radius: 2000 cm (20m - fades out)
- Falloff: Natural

Implementation:
  Each player has AudioComponent for voice
  Voice output spatialized at player head position
  Distant players sound quieter (realistic)
```

---

## 7. Player Roles 玩家角色

### Visual Identification 视觉识别

**Player Name Tags 玩家姓名标签**:

```
BP_PlayerNameTag (Widget Component on VR pawn):
  Displays above player's head:
  - Player name
  - Role icon (teacher/student/observer)
  - Player color border
  - Voice indicator (animates when talking)
```

**Player Color System 玩家颜色系统**:
```
Assign colors on join:
- Player 1 (Host): Blue
- Player 2: Green
- Player 3: Red
- Player 4: Yellow

Used for:
- Name tag borders
- Highlight color when holding parts
- Task assignment indicators
```

---

### Permission System 权限系统

**In BP_MultiplayerGameMode**:

```
Function: CanPlayerInteractWithPart(PlayerState, Part)
  Returns bool

  Logic:
  If PlayerState.Role == Teacher:
    Return true (full access)
  
  If PlayerState.Role == Student:
    If Part in AssignedTasks[PlayerState]:
      Return true (can interact with assigned parts)
    Else:
      Return false
  
  If PlayerState.Role == Observer:
    Return false (no interaction)
```

---

## 8. Testing and Optimization 测试与优化

### Local Testing 本地测试

**Setup**:
```
1. Enable "Run Dedicated Server" in project settings
2. Launch with command line:
   UE5Editor.exe [Project].uproject -game -server -log

3. Launch clients:
   Client 1: UE5Editor.exe [Project].uproject -game -windowed -ResX=1280 -ResY=720
   Client 2: Same with different window position

4. Test LAN multiplayer
```

### Network Profiling 网络分析

```
Console commands:
- stat net: Show network stats
- net.PackageMap.DebugObject: Debug object replication
- net.Emulation.Latency X: Simulate lag (X ms)
- net.Emulation.PacketLoss X: Simulate packet loss (X%)
```

### Optimization Tips 优化建议

1. **Reduce Replication Frequency**: Parts only replicate when moving
2. **Relevancy**: Far parts don't replicate to distant players
3. **Dormancy**: Assembled parts become dormant (no updates)
4. **Compress**: Use quantized rotation/position for parts
5. **LOD**: Lower quality meshes for distant players' views

---

## Implementation Timeline 实施时间线

### Week 1: Foundation
- Day 1-2: Setup multiplayer project settings
- Day 3: Implement session management (host/find/join)
- Day 4: Test basic connection and player spawning
- Day 5: Implement player state replication

### Week 2: Synchronization
- Day 1-2: Add replication to rocket parts
- Day 3: Implement grip/assembly RPCs
- Day 4: Test collaborative assembly
- Day 5: Add conflict resolution

### Week 3: Features
- Day 1: Implement role system
- Day 2: Add task assignment
- Day 3: Setup voice chat
- Day 4: Create player name tags
- Day 5: Test all multiplayer features

### Week 4: Testing & Polish
- Day 1-2: Network testing (LAN and online)
- Day 3: Performance optimization
- Day 4: Bug fixes
- Day 5: Final validation

---

## Deliverables 交付清单

**Blueprints**:
- [ ] BP_MultiplayerSessionManager
- [ ] BP_TaskAssignmentSystem
- [ ] BP_PlayerNameTag
- [ ] ARocketAssemblyPlayerState (C++)

**Maps**:
- [ ] L_AssemblyWorkshop_MP (multiplayer version)

**UI**:
- [ ] WBP_MultiplayerLobby
- [ ] WBP_SessionBrowser
- [ ] WBP_PlayerList

---

**Note**: Multiplayer is marked as **OPTIONAL** in Phase 3.  
Priority: Implement if time allows after core features complete.

**Status**: Complete Specification ✅  
**Version**: 1.0.0  
**Date**: 2026-01-03

**Phase 3 Multiplayer Guide Complete! 👥🚀**
