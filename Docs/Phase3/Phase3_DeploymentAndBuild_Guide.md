# Phase 3: 部署与构建指南
# Phase 3: Deployment and Build Guide

## Build Configuration 构建配置

### Project Settings 项目设置

**Engine Settings**:
```
Project Settings → Project → Description:
- Project Name: VR Rocket Assembly Educational System
- Company Name: [Your Company]
- Homepage: [Your URL]
- Copyright Notice: Copyright 2026

Project → Packaging:
- Build Configuration: Shipping
- Full Rebuild: ✓ (for final build)
- Create compressed cooked packages: ✓
- Use Pak File: ✓
- Generate Chunks: ✗ (unless using DLC)
```

**Platform-Specific**:

PC VR:
```
Platforms → Windows:
- Target Platform: Windows 64-bit
- Default RHI: DirectX 12 (or Vulkan for compatibility)
- Targeted RHIs: DX12, DX11, Vulkan
```

Quest:
```
Platforms → Android:
- Minimum SDK Version: 26
- Target SDK Version: 32
- Package for Oculus Mobile: ✓
- Configure for Quest 2/3
```

---

## Content Cooking 内容烘焙

### Asset Cooking Settings 资产烘焙设置

```
Project Settings → Packaging → Advanced:
- Cook everything in project: ✗ (select specific content)
- Directories to Always Cook:
  - /Game/RocketAssembly
  - /Game/UI
  - /Game/Maps
- Exclude editor content: ✓
- Exclude developer content: ✓
```

### Compression 压缩

```
Packaging → Packaging:
- Create compressed cooked packages: ✓

Per-asset compression:
- Textures: BC7 (PC), ASTC (Quest)
- Audio: Vorbis/Opus
- Enable streaming for large assets
```

---

## Platform Builds 平台构建

### PC VR Build (Windows) PC VR构建

**Prerequisites**:
- Visual Studio 2022
- Windows SDK 10
- SteamVR or Oculus PC software

**Build Steps**:
```
1. File → Package Project → Windows → Windows (64-bit)
2. Select output directory: Builds/Windows/
3. Wait for packaging (20-60 minutes for full build)
4. Verify build in output directory:
   - [ProjectName].exe
   - [ProjectName] folder (content)
5. Test .exe launch and basic functionality
```

**Post-Build**:
```
- Check build size (should be < 2GB for distribution)
- Test on clean machine (no development tools)
- Create installer (optional, use Inno Setup or similar)
```

---

### Quest Build (Android APK) Quest构建

**Prerequisites**:
- Android SDK/NDK (via UE5)
- Oculus Developer Account
- Quest device in Developer Mode

**Build Steps**:
```
1. File → Package Project → Android → Android (ASTC)
2. Select output directory: Builds/Quest/
3. Wait for packaging (30-90 minutes)
4. Locate APK: Builds/Quest/[ProjectName]-arm64.apk
```

**Install on Quest**:
```
Connect Quest via USB
Enable ADB:
  adb devices  (verify device connected)
  adb install [ProjectName]-arm64.apk

Or use SideQuest tool for easier installation
```

**Testing**:
```
1. Launch app on Quest
2. Check performance (72 FPS)
3. Test all features
4. Monitor for crashes (adb logcat)
```

---

## Distribution 分发

### Steam Distribution Steam分发

**Setup**:
```
1. Create Steamworks account
2. Create app entry for game
3. Set up store page (description, screenshots, videos)
4. Configure depots (Windows build)
5. Upload build via SteamPipe
```

**Steam VR Configuration**:
```
steamapps/common/[YourGame]/
├─ [YourGame].exe
├─ steam_appid.txt (your App ID)
└─ Content/

Launch options: Add VR support tags
```

---

### Oculus Store (Quest) Oculus商店

**Submission Process**:
```
1. Create Oculus Developer Organization
2. Create new app in dashboard
3. Fill out store metadata:
   - Title, description (EN + localized)
   - Screenshots (at least 4)
   - Trailer video (recommended)
   - Age rating
   - Privacy policy
4. Upload APK
5. Submit for review (2-4 weeks)
```

**Requirements**:
- Performance: 72 FPS minimum
- No crashes during 30-min session
- Comfortable VR experience
- Complete functionality
- Proper Oculus integration

---

### SideQuest / App Lab 侧载

**App Lab** (easier than full Oculus Store):
```
1. Create App Lab listing (less strict review)
2. Upload APK
3. Generate shareable link
4. Users can install via Oculus app
```

**SideQuest** (for early access/testing):
```
1. Upload APK to SideQuest platform
2. Free and open distribution
3. Users download via SideQuest app
```

---

## Build Automation 构建自动化

### Unreal Automation Tool (UAT) 虚幻自动化工具

**Command Line Build**:

Windows:
```batch
RunUAT.bat BuildCookRun ^
  -project="D:/Projects/RocketAssembly/RocketAssembly.uproject" ^
  -platform=Win64 ^
  -clientconfig=Shipping ^
  -serverconfig=Shipping ^
  -cook ^
  -allmaps ^
  -build ^
  -stage ^
  -package ^
  -pak ^
  -archive ^
  -archivedirectory="D:/Builds/Windows"
```

Quest:
```batch
RunUAT.bat BuildCookRun ^
  -project="D:/Projects/RocketAssembly/RocketAssembly.uproject" ^
  -platform=Android_ASTC ^
  -clientconfig=Shipping ^
  -cook ^
  -build ^
  -stage ^
  -package ^
  -pak ^
  -archive ^
  -archivedirectory="D:/Builds/Quest"
```

---

### CI/CD Integration 持续集成

**Using Jenkins/GitHub Actions**:

Example GitHub Action:
```yaml
name: Build VR Project

on:
  push:
    branches: [ main ]

jobs:
  build-windows:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v2
      - name: Build Windows
        run: |
          RunUAT.bat BuildCookRun [parameters]
      - name: Upload Artifact
        uses: actions/upload-artifact@v2
        with:
          name: WindowsBuild
          path: Builds/Windows/
```

---

## Version Management 版本管理

### Versioning Scheme 版本方案

```
Format: Major.Minor.Patch (e.g., 1.0.0)

Major: Breaking changes, major features
Minor: New features, backward compatible
Patch: Bug fixes, small improvements

Example:
- 1.0.0: Initial release
- 1.1.0: Added multiplayer
- 1.1.1: Bug fixes
- 2.0.0: Major content update
```

**In Project Settings**:
```
Project → Description → Version:
  Project Version: 1.0.0
  
Update for each release
```

---

## Release Checklist 发布检查清单

### Pre-Release 发布前
- [ ] All features implemented and tested
- [ ] Performance targets met (90 FPS PC, 72 FPS Quest)
- [ ] No critical/high bugs remaining
- [ ] User acceptance testing passed
- [ ] Localization complete (CN + EN)
- [ ] Legal compliance (GDPR, age rating, etc.)
- [ ] Store pages prepared (screenshots, description, trailer)

### Build Quality 构建质量
- [ ] Clean build from source control (no local dependencies)
- [ ] Build size reasonable (< 2GB PC, < 1GB Quest)
- [ ] All assets included (no missing textures/sounds)
- [ ] Launch tested on clean machine
- [ ] No development/debug content included

### Documentation 文档
- [ ] User manual or in-game tutorial complete
- [ ] README with system requirements
- [ ] Known issues documented
- [ ] Support contact information provided

### Post-Release 发布后
- [ ] Monitor user feedback
- [ ] Track crash reports
- [ ] Collect performance metrics
- [ ] Plan patch releases for critical issues
- [ ] Prepare for future updates

---

## Troubleshooting 故障排除

### Common Build Issues 常见构建问题

**Issue**: Build fails with missing dependencies
```
Solution:
- Verify all plugins enabled and compatible
- Check for compile errors in C++ code
- Rebuild project (Clean, Rebuild)
```

**Issue**: APK too large (> 1GB)
```
Solution:
- Remove unused assets
- Compress textures more aggressively
- Enable Pak file compression
- Check for duplicate assets
```

**Issue**: App crashes on launch
```
Solution:
- Check logs in [ProjectName]/Saved/Logs/
- Verify all required libs included
- Test on multiple devices
- Check for platform-specific issues (Android permissions, etc.)
```

---

## Deliverables 交付清单

**Builds**:
- [ ] PC VR build (Windows .exe + content)
- [ ] Quest build (Android APK)
- [ ] (Optional) Linux build
- [ ] (Optional) Mac build (if supported)

**Documentation**:
- [ ] Release notes
- [ ] Installation instructions
- [ ] System requirements
- [ ] Known issues list

**Distribution**:
- [ ] Steam store page live (if using Steam)
- [ ] Oculus Store submission complete (if using Oculus Store)
- [ ] Download links ready (if self-hosting)
- [ ] Marketing materials (trailer, screenshots)

---

**Version**: 1.0.0  
**Status**: Complete Deployment Guide ✅
