# Phase 2 - Data Tables Complete Content Guide
# 火箭装配系统 Phase 2 数据表完整内容指南

## 📋 Overview 概述

This guide provides complete content for expanding data tables with 15+ rocket parts, complete assembly steps, knowledge base entries, and achievement definitions.

本指南提供完整内容以扩展数据表，包括 15+ 火箭零件、完整装配步骤、知识库条目和成就定义。

---

## 1. DT_RocketParts - Expanded Part Data 扩展零件数据

### Data Table Structure 数据表结构

Based on `FRocketPartData` structure from Phase 1.

### Complete Parts List (15 Parts) 完整零件列表（15个零件）

#### Part 1: First Stage Rocket Body 一级火箭筒体
```csv
PartID,PartNameCN,PartNameEN,PartType,AssemblyOrder,DimensionX,DimensionY,DimensionZ,Mass,MaterialType,AcceptedSocketIDs,DependentPartIDs,EducationalContentCN,EducationalContentEN,TechnicalDetails
Part_001,一级火箭筒体,First Stage Rocket Body,FirstStageBody,1,3.0,3.0,8.0,5000.0,Aluminum Alloy,Socket_Base,,一级火箭是整个火箭的基础结构，承载着主发动机和大部分燃料。它必须足够坚固以承受发射时的巨大推力。,The first stage is the foundational structure of the rocket carrying the main engines and most of the fuel. It must be strong enough to withstand the tremendous thrust during launch.,采用铝锂合金制造，具有高强度和轻质特性。内部有加强筋结构。
```

#### Part 2: Main Engine Nozzle 主发动机喷嘴
```csv
Part_002,主发动机喷嘴,Main Engine Nozzle,MainEngineNozzle,2,2.5,2.5,4.0,1200.0,Titanium Alloy,Socket_Engine1,Part_001,主发动机喷嘴将高温高压气体加速喷出，产生巨大的推力。喷嘴的形状经过精心设计，以获得最大推力效率。,The main engine nozzle accelerates high-temperature high-pressure gases to produce tremendous thrust. The nozzle shape is carefully designed for maximum thrust efficiency.,推力：450kN，燃烧室温度可达3300°C，喷嘴采用再生冷却技术。
```

#### Part 3: Turbo Pump 涡轮泵
```csv
Part_003,涡轮泵,Turbo Pump,TurboPump,3,0.8,0.8,1.2,250.0,Steel Alloy,Socket_Pump1,Part_002,涡轮泵负责将燃料和氧化剂以高压输送到燃烧室。它的转速可达数万转每分钟，是火箭发动机的心脏。,The turbo pump delivers fuel and oxidizer to the combustion chamber at high pressure. It can spin at tens of thousands of RPM and is the heart of the rocket engine.,转速：30000 RPM，流量：500 L/s，功率：40 MW
```

#### Part 4: LOX Tank 液氧舱
```csv
Part_004,液氧燃料舱,Liquid Oxygen Tank,LOXTank,4,2.8,2.8,5.0,800.0,Aluminum Alloy,Socket_LOXTank,Part_001,液氧舱储存零下183°C的液态氧，作为火箭的氧化剂。它必须具有良好的隔热性能以保持液氧的低温状态。,The LOX tank stores liquid oxygen at -183°C as the rocket's oxidizer. It must have excellent insulation to maintain the cryogenic temperature.,容量：95000升，保温层厚度：10cm，采用多层绝缘技术
```

#### Part 5: Fuel Tank 燃料舱
```csv
Part_005,燃料舱（液氢/煤油）,Fuel Tank (LH2/Kerosene),FuelTank,5,2.8,2.8,6.0,750.0,Aluminum Alloy,Socket_FuelTank,Part_001,燃料舱储存火箭燃料，可能是液氢（-253°C）或煤油（RP-1）。液氢密度低但能量高，煤油密度高但技术要求相对简单。,The fuel tank stores rocket propellant which may be liquid hydrogen (-253°C) or kerosene (RP-1). Liquid hydrogen has low density but high energy; kerosene has high density but simpler technology.,液氢容量：225000升，煤油容量：85000升
```

#### Part 6: Fuel Pipeline 燃料管路
```csv
Part_006,燃料管路,Fuel Pipeline,FuelPipeline,6,0.3,0.3,4.0,120.0,Stainless Steel,Socket_Pipeline1,"Part_004,Part_005",燃料管路连接燃料舱、氧化剂舱和发动机，确保燃料和氧化剂能够以正确的比例混合并输送到燃烧室。,The fuel pipeline connects the fuel tank oxidizer tank and engine ensuring fuel and oxidizer are mixed in the correct ratio and delivered to the combustion chamber.,耐压：40 MPa，管道内径：15cm，材料耐低温-253°C
```

#### Part 7: Interstage Section 级间段
```csv
Part_007,级间段,Interstage Section,InterstageSection,7,2.6,2.6,2.0,400.0,Aluminum Alloy,Socket_Interstage,Part_001,级间段连接火箭的不同级，并在级间分离时提供分离机制。它包含分离爆炸螺栓和推进系统。,The interstage section connects different stages of the rocket and provides separation mechanism during stage separation. It contains separation pyro bolts and propulsion systems.,分离时间：0.5秒，分离速度差：2 m/s
```

#### Part 8: Second Stage Body 二级火箭筒体
```csv
Part_008,二级火箭筒体,Second Stage Rocket Body,SecondStageBody,8,2.4,2.4,6.0,2000.0,Aluminum Alloy,Socket_Stage2,Part_007,二级火箭在一级分离后继续提供推力，将载荷送入更高轨道。它体积较小但效率更高，通常使用更高效的发动机。,The second stage provides continued thrust after first stage separation sending the payload into higher orbit. It is smaller but more efficient usually using more efficient engines.,推重比：1.2，比冲：350秒
```

#### Part 9: Second Stage Engine 二级发动机
```csv
Part_009,二级发动机,Second Stage Engine,MainEngineNozzle,9,1.5,1.5,2.5,400.0,Titanium Alloy,Socket_Engine2,Part_008,二级发动机专为真空环境设计，具有更大的喷嘴扩张比以提高真空比冲。它通常使用可重启技术。,The second stage engine is designed for vacuum environment with a larger nozzle expansion ratio to increase vacuum specific impulse. It typically uses restart capability.,推力：100kN（真空），可重启次数：3次，喷嘴扩张比：80:1
```

#### Part 10: Attitude Control Engine 姿态控制发动机
```csv
Part_010,姿态控制发动机,Attitude Control Engine,AttitudeControlEngine,10,0.3,0.3,0.5,25.0,Titanium Alloy,Socket_RCS1,Part_008,姿态控制发动机（RCS）用于调整火箭的姿态和轨道微调。它们可以提供精确的小推力，实现多方向控制。,Reaction Control System (RCS) engines adjust the rocket's attitude and fine-tune orbit. They provide precise small thrusts for multi-directional control.,推力：25N × 16个喷嘴，响应时间：0.01秒
```

#### Part 11: Gyroscope Unit 陀螺仪组件
```csv
Part_011,陀螺仪组件,Gyroscope Unit,GyroscopeUnit,11,0.5,0.5,0.4,15.0,Composite Material,Socket_Gyro,Part_008,陀螺仪测量火箭的角速度和姿态，为制导系统提供精确的定位信息。现代火箭使用激光陀螺仪或光纤陀螺仪。,The gyroscope measures the rocket's angular velocity and attitude providing precise positioning information for the guidance system. Modern rockets use laser gyroscopes or fiber optic gyroscopes.,精度：0.001°/h，更新频率：1000 Hz
```

#### Part 12: Control Computer 控制计算机
```csv
Part_012,控制计算机,Control Computer,ControlComputer,12,0.6,0.4,0.3,20.0,Electronics,Socket_Computer,Part_008,控制计算机是火箭的"大脑"，处理导航数据，计算轨道，控制发动机和姿态调整。它必须具有极高的可靠性和冗余设计。,The control computer is the rocket's brain processing navigation data calculating orbits controlling engines and attitude adjustments. It must have extremely high reliability and redundant design.,处理器：四核1.5 GHz，内存：8GB，操作系统：实时系统
```

#### Part 13: Fairing Left 整流罩（左）
```csv
Part_013,整流罩（左半）,Fairing (Left),FairingLeft,13,2.0,1.5,4.0,180.0,Carbon Fiber,Socket_FairingL,Part_008,整流罩保护有效载荷在大气层内飞行时免受气动加热和压力影响。到达太空后整流罩会分离，减轻火箭重量。,The fairing protects the payload from aerodynamic heating and pressure during atmospheric flight. After reaching space the fairing separates to reduce rocket weight.,分离高度：115km，分离速度：火箭速度 + 1.5 m/s
```

#### Part 14: Fairing Right 整流罩（右）
```csv
Part_014,整流罩（右半）,Fairing (Right),FairingRight,14,2.0,1.5,4.0,180.0,Carbon Fiber,Socket_FairingR,Part_008,整流罩的右半部分，与左半部分一起形成完整的保护罩。整流罩通常采用复合材料制造，轻质且坚固。,The right half of the fairing which forms a complete protective cover with the left half. Fairings are typically made of composite materials that are lightweight yet strong.,厚度：15mm，隔热层：5层
```

#### Part 15: Payload Adapter 载荷适配器
```csv
Part_015,卫星适配器,Satellite Adapter,SatelliteAdapter,15,1.8,1.8,1.0,150.0,Aluminum Alloy,Socket_Payload,Part_008,载荷适配器连接火箭和卫星或其他有效载荷。它提供机械连接、电气接口和分离机构，确保载荷能够安全释放到预定轨道。,The payload adapter connects the rocket to the satellite or other payload. It provides mechanical connection electrical interface and separation mechanism ensuring the payload can be safely released into the intended orbit.,承载能力：10000 kg，分离机制：爆炸螺栓，电气接口：100针连接器
```

---

## 2. DT_AssemblySteps - Complete Assembly Steps 完整装配步骤

### 10-Step Assembly Process 10步装配流程

```csv
StepNumber,StepNameCN,StepNameEN,InstructionTextCN,InstructionTextEN,PartIDToAssemble,TargetSocketID,DependentStepIndices,EstimatedTime,DifficultyLevel,VoiceFileCN,VoiceFileEN,ARGuideType
1,安装一级火箭基座,Install First Stage Base,找到一级火箭筒体，将其垂直放置在中央装配架的基座上。确保底部完全对齐。,Find the first stage rocket body and place it vertically on the base of the central assembly frame. Ensure the bottom is fully aligned.,Part_001,Socket_Base,,60,Easy,Voice/CN/Step_01.wav,Voice/EN/Step_01.wav,Arrow
2,安装主发动机,Install Main Engine,拿起主发动机喷嘴，将其对准一级火箭底部的发动机装配点。听到"咔嗒"声表示安装成功。,Pick up the main engine nozzle and align it with the engine mount at the bottom of the first stage. A click sound indicates successful installation.,Part_002,Socket_Engine1,1,90,Medium,Voice/CN/Step_02.wav,Voice/EN/Step_02.wav,Arrow+Preview
3,连接涡轮泵,Connect Turbo Pump,将涡轮泵安装到主发动机侧面。涡轮泵负责输送燃料，必须牢固安装。,Install the turbo pump on the side of the main engine. The turbo pump delivers fuel and must be securely mounted.,Part_003,Socket_Pump1,2,75,Medium,Voice/CN/Step_03.wav,Voice/EN/Step_03.wav,Arrow
4,安装液氧舱,Install LOX Tank,将液氧舱放置在一级火箭筒体内部的指定位置。注意液氧舱顶部有连接口朝上。,Place the LOX tank at the designated position inside the first stage rocket body. Note the connection port at the top of the LOX tank should face upward.,Part_004,Socket_LOXTank,1,80,Medium,Voice/CN/Step_04.wav,Voice/EN/Step_04.wav,Arrow+Preview
5,安装燃料舱,Install Fuel Tank,在液氧舱上方安装燃料舱。确保两个舱体之间有适当的间隔和连接。,Install the fuel tank above the LOX tank. Ensure proper spacing and connection between the two tanks.,Part_005,Socket_FuelTank,4,80,Medium,Voice/CN/Step_05.wav,Voice/EN/Step_05.wav,Arrow+Preview
6,连接燃料管路,Connect Fuel Pipeline,安装燃料管路，将燃料舱、液氧舱和发动机连接起来。管路必须密封良好。,Install the fuel pipeline connecting the fuel tank LOX tank and engine. The pipeline must be well sealed.,Part_006,Socket_Pipeline1,"4,5",120,Hard,Voice/CN/Step_06.wav,Voice/EN/Step_06.wav,Arrow
7,安装级间段,Install Interstage Section,将级间段放置在一级火箭顶部。级间段将连接一级和二级火箭。,Place the interstage section on top of the first stage. The interstage will connect the first and second stages.,Part_007,Socket_Interstage,1,70,Easy,Voice/CN/Step_07.wav,Voice/EN/Step_07.wav,Arrow+Preview
8,安装二级火箭,Install Second Stage,将二级火箭筒体放置在级间段上方。二级火箭体积较小但很重要。,Place the second stage rocket body on top of the interstage section. The second stage is smaller but crucial.,Part_008,Socket_Stage2,7,90,Medium,Voice/CN/Step_08.wav,Voice/EN/Step_08.wav,Arrow+Preview
9,安装二级发动机和控制系统,Install Second Stage Engine and Control Systems,在二级火箭底部安装二级发动机，然后依次安装姿态控制发动机、陀螺仪和控制计算机。,Install the second stage engine at the bottom of the second stage then install the attitude control engine gyroscope and control computer.,Part_009,Socket_Engine2,8,150,Hard,Voice/CN/Step_09.wav,Voice/EN/Step_09.wav,Arrow
10,安装整流罩,Install Fairing,最后，安装整流罩的左右两部分，形成完整的保护罩。整流罩必须对称安装。,Finally install both left and right parts of the fairing to form a complete protective cover. The fairing must be installed symmetrically.,Part_013,Socket_FairingL,8,100,Medium,Voice/CN/Step_10.wav,Voice/EN/Step_10.wav,Arrow+Preview
```

---

## 3. DT_RocketKnowledge - Knowledge Base 知识库

### 10 Knowledge Entries 10个知识条目

```csv
KnowledgeID,TitleCN,TitleEN,Category,ContentCN,ContentEN,RelatedPartIDs,MediaAssets,Difficulty
Know_001,火箭发动机工作原理,Rocket Engine Working Principle,Propulsion,火箭发动机通过燃烧燃料和氧化剂产生高温高压气体，这些气体从喷嘴高速喷出，根据牛顿第三定律产生反作用力推动火箭前进。液体火箭发动机使用涡轮泵将燃料和氧化剂输送到燃烧室，在那里它们混合并燃烧。燃烧产生的气体温度可达3000-3500°C，压力达20-30 MPa。喷嘴的作用是将这些高温高压气体加速到超音速（2-4 km/s），从而产生巨大的推力。,Rocket engines generate high-temperature high-pressure gases by burning fuel and oxidizer. These gases are expelled at high speed through a nozzle creating thrust according to Newton's third law. Liquid rocket engines use turbo pumps to deliver fuel and oxidizer to the combustion chamber where they mix and combust. The resulting gases reach temperatures of 3000-3500°C and pressures of 20-30 MPa. The nozzle accelerates these gases to supersonic speeds (2-4 km/s) generating tremendous thrust.,"Part_002,Part_003",Video_EngineOperation.mp4,Intermediate
Know_002,液氧液氢燃料系统,LOX-LH2 Propellant System,Fuel Systems,液氧（LOX）和液氢（LH2）是一对高性能火箭推进剂。液氧在-183°C下储存，作为氧化剂。液氢在-253°C下储存，作为燃料。这对组合的比冲（specific impulse）可达450秒，远高于煤油-液氧组合的350秒。然而，液氢密度很低（70 kg/m³），需要巨大的储罐。液氧液氢的燃烧产物主要是水蒸气，相对环保。这种组合被用于阿波罗计划的土星五号火箭上级和航天飞机主发动机。,Liquid Oxygen (LOX) and Liquid Hydrogen (LH2) are a high-performance rocket propellant pair. LOX is stored at -183°C as the oxidizer. LH2 is stored at -253°C as the fuel. This combination achieves a specific impulse of 450 seconds far exceeding the 350 seconds of kerosene-LOX. However liquid hydrogen has very low density (70 kg/m³) requiring enormous tanks. LOX-LH2 combustion produces mainly water vapor making it relatively environmentally friendly. This combination was used in the upper stages of the Saturn V and the Space Shuttle Main Engines.,"Part_004,Part_005",Diagram_CryogenicStorage.png,Advanced
Know_003,多级火箭原理,Multi-Stage Rocket Principle,Rocket Design,多级火箭是解决火箭到达高速度和远距离的关键技术。随着火箭飞行，燃料被消耗，但空燃料舱和发动机的重量仍然拖累火箭。多级设计允许火箭在燃料耗尽时抛弃下一级的舱体和发动机，大大减轻重量。典型的多级火箭有2-3级。齐奥尔科夫斯基火箭方程表明，火箭的最终速度与质量比（初始质量/最终质量）的对数成正比。通过级间分离，每一级都可以达到更高的质量比，从而实现更高的总速度。例如，土星五号火箭有三级，能够将阿波罗飞船送往月球。,Multi-stage rockets are the key technology for achieving high velocities and long distances. As the rocket flies fuel is consumed but the weight of empty tanks and engines continues to burden the rocket. Multi-stage design allows the rocket to jettison lower stage bodies and engines when fuel is depleted greatly reducing weight. Typical multi-stage rockets have 2-3 stages. The Tsiolkovsky rocket equation shows that a rocket's final velocity is proportional to the logarithm of the mass ratio (initial mass / final mass). Through stage separation each stage can achieve a higher mass ratio thus achieving higher total velocity. For example the Saturn V had three stages capable of sending Apollo spacecraft to the Moon.,"Part_001,Part_007,Part_008",Animation_StageSepar ation.mp4,Intermediate
Know_004,齐奥尔科夫斯基火箭方程,Tsiolkovsky Rocket Equation,Physics,俄罗斯科学家康斯坦丁·齐奥尔科夫斯基在1903年推导出了著名的火箭方程：Δv = ve * ln(m0/mf)，其中Δv是速度变化，ve是喷气速度（排气速度），m0是初始质量，mf是最终质量。这个方程揭示了火箭性能的基本原理：1) 速度变化与质量比的对数成正比，2) 速度变化与喷气速度成正比。这意味着要达到轨道速度（约7.8 km/s），火箭必须具有很高的质量比（通常90-95%的初始质量是推进剂）或很高的喷气速度（高比冲发动机）。该方程是航天工程的基础，指导着火箭设计的各个方面。,Russian scientist Konstantin Tsiolkovsky derived the famous rocket equation in 1903: Δv = ve * ln(m0/mf) where Δv is velocity change ve is exhaust velocity m0 is initial mass and mf is final mass. This equation reveals fundamental principles of rocket performance: 1) velocity change is proportional to the logarithm of mass ratio 2) velocity change is proportional to exhaust velocity. This means to reach orbital velocity (~7.8 km/s) rockets must have a very high mass ratio (typically 90-95% of initial mass is propellant) or very high exhaust velocity (high specific impulse engines). This equation is fundamental to space engineering guiding all aspects of rocket design.,All Parts,Formula_TsiolkovskyEquation.png,Advanced
Know_005,火箭姿态控制,Rocket Attitude Control,Control Systems,火箭在飞行过程中需要精确控制其姿态（方向）。姿态控制系统（ACS）通过多种方式实现：1) 主发动机推力矢量控制（TVC）：通过万向节或流体注入改变喷嘴方向；2) 反应控制系统（RCS）：使用小型推力器进行精确调整；3) 控制表面：在大气层内使用舵面或栅格舵。姿态由陀螺仪和加速度计测量，控制计算机根据预定轨道计算所需姿态，然后发出指令调整。现代火箭使用惯性导航系统（INS）配合GPS进行精确导航。SpaceX的猎鹰9号使用栅格舵和推力矢量控制实现了火箭回收着陆。,Rockets need to precisely control their attitude (orientation) during flight. Attitude Control Systems (ACS) achieve this through various means: 1) Main engine Thrust Vector Control (TVC): changing nozzle direction via gimbals or fluid injection; 2) Reaction Control System (RCS): using small thrusters for precise adjustment; 3) Control surfaces: using fins or grid fins in the atmosphere. Attitude is measured by gyroscopes and accelerometers; the control computer calculates required attitude based on planned trajectory then issues adjustment commands. Modern rockets use Inertial Navigation Systems (INS) combined with GPS for precise navigation. SpaceX's Falcon 9 uses grid fins and thrust vector control for rocket landing recovery.,"Part_010,Part_011,Part_012",Video_AttitudeControl.mp4,Intermediate
Know_006,整流罩的作用,Purpose of Payload Fairing,Structures,整流罩是保护有效载荷的流线型外壳。在火箭穿越大气层时，整流罩有三个关键作用：1) 气动保护：减小空气阻力，使火箭能够高效穿越大气层；2) 热防护：保护载荷免受气动加热（温度可达1000°C以上）；3) 声学保护：减轻发动机噪音对敏感载荷（如卫星）的影响。整流罩通常由轻质高强度的碳纤维复合材料制成。当火箭到达100-120公里高度，大气密度极低时，整流罩会沿中线分离成两半并抛弃，减轻火箭重量。SpaceX甚至尝试回收整流罩，因为它们价值数百万美元。,The payload fairing is a streamlined shell protecting the payload. During atmospheric transit the fairing serves three key purposes: 1) Aerodynamic protection: reducing air resistance allowing efficient atmospheric transit; 2) Thermal protection: shielding payload from aerodynamic heating (temperatures can exceed 1000°C); 3) Acoustic protection: reducing engine noise impact on sensitive payloads like satellites. Fairings are typically made of lightweight high-strength carbon fiber composite. When the rocket reaches 100-120 km altitude where atmospheric density is extremely low the fairing separates along its centerline into two halves and is jettisoned reducing rocket weight. SpaceX even attempts to recover fairings as they cost millions of dollars.,"Part_013,Part_014",Diagram_FairingSepar ation.png,Beginner
Know_007,火箭发射流程,Rocket Launch Procedures,Operations,火箭发射是一个复杂的多步骤流程。典型流程包括：T-24小时：最终检查和燃料准备；T-4小时：开始加注液氧和液氢（必须在发射前不久进行，因为低温燃料会蒸发）；T-1小时：最终系统检查，清空发射台区域；T-10分钟：进入自动发射程序；T-10秒：主发动机点火程序开始；T-0：所有发动机达到100%推力，释放火箭；T+2分钟：一级燃料耗尽，级间分离；T+8分钟：二级关机，载荷进入轨道或继续爬升。整个过程由任务控制中心严密监控，任何异常都可能导致中止发射。,Rocket launch is a complex multi-step process. Typical procedure: T-24 hours: Final checks and propellant preparation; T-4 hours: Begin loading LOX and LH2 (must be done shortly before launch as cryogenic propellants evaporate); T-1 hour: Final system checks clear launch pad area; T-10 minutes: Enter automatic launch sequence; T-10 seconds: Main engine ignition sequence begins; T-0: All engines reach 100% thrust rocket released; T+2 minutes: First stage propellant depleted stage separation; T+8 minutes: Second stage shutdown payload enters orbit or continues ascent. The entire process is closely monitored by mission control; any anomaly may trigger launch abort.,All Parts,Timeline_LaunchSequence.png,Beginner
Know_008,火箭回收技术,Rocket Recovery Technology,Innovation,传统火箭在使用后被丢弃，造成巨大成本。SpaceX开创的火箭回收技术彻底改变了这一状况。回收过程：1) 一级分离后，火箭继续向上飞行一段距离；2) 使用RCS翻转火箭，使发动机朝下；3) 重启发动机进行"再入烧"，减速并保护火箭免受再入大气时的热量；4) 展开栅格舵进行气动控制；5) 在接近地面时，发动机再次点火进行"着陆烧"，精确控制下降速度；6) 在陆地着陆平台或海上驳船上垂直着陆。这项技术使火箭能够重复使用，大大降低了发射成本。猎鹰9号的一级火箭已经实现了10次以上的重复使用。,Traditional rockets are discarded after use creating enormous costs. SpaceX's pioneering rocket recovery technology has revolutionized this. Recovery process: 1) After first stage separation the rocket continues upward briefly; 2) Uses RCS to flip the rocket engine-down; 3) Restarts engine for reentry burn slowing down and protecting from reentry heat; 4) Deploys grid fins for aerodynamic control; 5) Near ground engine reignites for landing burn precisely controlling descent speed; 6) Vertically lands on ground pad or ocean barge. This technology enables rocket reusability dramatically reducing launch costs. Falcon 9 first stages have achieved over 10 reuses.,"Part_002,Part_010",Video_RocketLanding.mp4,Advanced
Know_009,逃逸塔系统,Launch Escape System,Safety,逃逸塔（LES）是载人火箭的重要安全系统。如果火箭在发射或上升过程中发生故障，逃逸塔能够迅速将载人舱拖离危险区域。逃逸塔通常位于载人舱顶部，包含多个固体火箭发动机。这些发动机能在数秒内点火，产生比主发动机更大的加速度（15-20g），将载人舱弹射到安全距离。在阿波罗计划中，逃逸塔从未被使用，但在苏联的联盟T-10-1任务中，逃逸塔成功救了两名宇航员。现代的龙飞船采用集成式逃逸系统，发动机内置在飞船中，可以在整个上升阶段使用，更加可靠。,The Launch Escape System (LES) is a critical safety system for crewed rockets. If the rocket experiences a failure during launch or ascent the LES can quickly pull the crew capsule away from danger. The escape tower typically sits atop the crew capsule and contains multiple solid rocket motors. These motors can ignite within seconds generating greater acceleration than the main engines (15-20g) ejecting the capsule to a safe distance. In the Apollo program the LES was never used but in the Soviet Soyuz T-10-1 mission the LES successfully saved two cosmonauts. Modern Dragon spacecraft use an integrated escape system with engines built into the capsule usable throughout ascent making it more reliable.,Part_015,Animation_EscapeTower.mp4,Intermediate
Know_010,著名火箭型号历史,Famous Rocket Models History,History,人类火箭技术发展史上有许多里程碑式的型号：1) V-2火箭（1942）：世界第一枚弹道导弹，也是第一个到达太空边缘的人造物体；2) 土星五号（1967-1973）：至今最强大的运载火箭，将人类送上月球，高111米，起飞推力34500 kN；3) 航天飞机（1981-2011）：第一个可重复使用的轨道飞行器，完成135次任务；4) 长征系列：中国主力运载火箭，可靠性高；5) 猎鹰9号（2010-）：首个实现一级回收的轨道火箭，大幅降低成本；6) 猎鹰重型（2018-）：现役最强大火箭，近地轨道运载能力63.8吨；7) 星舰（研发中）：SpaceX的超重型完全可重复使用火箭，设计运载能力超过100吨。,Milestones in human rocket technology: 1) V-2 Rocket (1942): World's first ballistic missile and first man-made object to reach the edge of space; 2) Saturn V (1967-1973): Most powerful launch vehicle to date sent humans to the Moon 111m tall 34500 kN thrust; 3) Space Shuttle (1981-2011): First reusable orbital vehicle completed 135 missions; 4) Long March series: China's workhorse rockets with high reliability; 5) Falcon 9 (2010-): First orbital rocket with first-stage recovery dramatically reducing costs; 6) Falcon Heavy (2018-): Most powerful operational rocket 63.8-ton LEO capacity; 7) Starship (in development): SpaceX super-heavy fully reusable rocket designed for 100+ ton capacity.,All Parts,Timeline_RocketHistory.png,Beginner
```

---

## 4. DT_Achievements - Achievement Definitions 成就定义

```csv
AchievementID,NameCN,NameEN,DescriptionCN,DescriptionEN,IconPath,UnlockCondition,RewardPoints,Difficulty
Achieve_001,首次起飞,First Launch,完成第一次火箭装配,Complete your first rocket assembly,Icon_FirstLaunch.png,CompleteAssembly == 1,100,Bronze
Achieve_002,完美工程师,Perfect Engineer,零错误完成装配,Complete assembly with zero errors,Icon_Perfect.png,ErrorCount == 0 AND CompleteAssembly >= 1,500,Gold
Achieve_003,速度之王,Speed Demon,10分钟内完成装配,Complete assembly in under 10 minutes,Icon_Speed.png,CompletionTime < 600,300,Silver
Achieve_004,知识达人,Knowledge Seeker,查看所有知识点,View all knowledge entries,Icon_Knowledge.png,KnowledgeViewCount >= 10,200,Silver
Achieve_005,百折不挠,Persistent,失败10次后成功完成,Complete assembly after 10 failures,Icon_Persistent.png,RetryCount >= 10 AND CompleteAssembly >= 1,250,Silver
Achieve_006,专业装配工,Professional Assembler,在专业难度完成装配,Complete assembly on Professional difficulty,Icon_Professional.png,Difficulty == Professional AND CompleteAssembly >= 1,400,Gold
Achieve_007,连续成功,Streak Master,连续完成5次装配无错误,Complete 5 assemblies in a row without errors,Icon_Streak.png,CompletionCount >= 5 AND ErrorCount == 0 in last 5,350,Gold
Achieve_008,教学大师,Tutorial Master,完成所有教学步骤,Complete all tutorial steps,Icon_Tutorial.png,TutorialStepsCompleted >= TotalSteps,150,Bronze
Achieve_009,收藏家,Collector,查看所有零件的详细信息,View detailed info for all parts,Icon_Collector.png,PartsViewedCount >= TotalParts,200,Silver
Achieve_010,分享者,Sharer,分享成绩报告,Share score report,Icon_Share.png,ShareCount >= 1,50,Bronze
```

---

## 5. DT_StringTable_UI - Localization String Table UI 本地化字符串表

### Key UI Strings 关键 UI 字符串

```csv
StringKey,SourceString_CN,SourceString_EN,Context
UI_MainMenu_Title,火箭装配 VR 科普教育,Rocket Assembly VR Educational System,Main menu title
UI_MainMenu_Subtitle,沉浸式学习火箭结构与装配,Immersive Learning of Rocket Structure and Assembly,Main menu subtitle
UI_Mode_Tutorial,教学模式,Tutorial Mode,Game mode
UI_Mode_Free,自由模式,Free Mode,Game mode
UI_Mode_Assessment,考核模式,Assessment Mode,Game mode
UI_Mode_Knowledge,知识浏览,Knowledge Browser,Game mode
UI_Difficulty_Simple,简化版 (5个零件),Simple (5 parts),Difficulty level
UI_Difficulty_Standard,标准版 (10个零件),Standard (10 parts),Difficulty level
UI_Difficulty_Professional,专业版 (20+个零件),Professional (20+ parts),Difficulty level
UI_Button_Start,开始游戏,Start Game,Button text
UI_Button_Continue,继续进度,Continue Progress,Button text
UI_Button_NewGame,新建进度,New Game,Button text
UI_Button_Settings,设置,Settings,Button text
UI_Button_Exit,退出,Exit,Button text
UI_HUD_Step,第 {0}/{1} 步,Step {0}/{1},HUD step counter
UI_HUD_Score,分数,Score,HUD label
UI_HUD_Errors,错误,Errors,HUD label
UI_HUD_Time,时间,Time,HUD label
UI_Error_WrongPart,错误：零件类型不正确,Error: Wrong part type,Error message
UI_Error_WrongSocket,错误：装配点不正确,Error: Wrong socket,Error message
UI_Success_StepComplete,步骤完成！,Step Complete!,Success message
UI_Success_AllComplete,恭喜！装配完成！,Congratulations! Assembly Complete!,Success message
UI_ScoreReport_Title,成绩报告,Score Report,Score report title
UI_ScoreReport_Accuracy,装配正确率,Assembly Accuracy,Score category
UI_ScoreReport_Sequence,装配顺序,Assembly Sequence,Score category
UI_ScoreReport_Time,完成时间,Completion Time,Score category
UI_ScoreReport_ErrorPenalty,失误次数,Error Penalty,Score category
UI_Grade_Excellent,优秀,Excellent,Grade
UI_Grade_Good,良好,Good,Grade
UI_Grade_Pass,及格,Pass,Grade
UI_Grade_Fail,不及格,Fail,Grade
UI_Settings_Language,语言,Language,Settings label
UI_Settings_Volume,音量,Volume,Settings label
UI_Settings_Graphics,图形质量,Graphics Quality,Settings label
UI_Settings_Controls,控制方式,Controls,Settings label
```

---

## Implementation Steps 实施步骤

### For DT_RocketParts (in Unreal Editor) 在虚幻编辑器中实施

1. **Create Data Table**
   ```
   Right-click in Content Browser → Miscellaneous → Data Table
   Row Structure: FRocketPartData
   Name: DT_RocketParts
   ```

2. **Import from CSV**
   ```
   Open DT_RocketParts
   Click "Import from CSV" or "Reimport from CSV"
   Select prepared CSV file
   Verify data imported correctly
   ```

3. **Manual Entry Alternative**
   ```
   Click "Add Row"
   Enter PartID (e.g., Part_001)
   Fill in all fields manually
   Repeat for all 15+ parts
   ```

4. **Validate Data**
   ```
   Check all IDs are unique
   Verify socket IDs match actual sockets
   Ensure dependencies are correct
   Test localization (CN and EN text)
   ```

### For DT_AssemblySteps

Similar process as DT_RocketParts, using FAssemblyStepData structure.

### For DT_RocketKnowledge

Similar process, using FRocketKnowledgeData structure.

### For DT_Achievements

Create new structure FAchievementData if not exists, then create data table.

---

## Testing Checklist 测试清单

### Data Integrity 数据完整性
- [ ] All PartIDs are unique
- [ ] All socket references are valid
- [ ] All dependencies are resolvable
- [ ] Localization complete (CN + EN)
- [ ] All numeric values reasonable

### Content Quality 内容质量
- [ ] Educational content accurate
- [ ] Technical details correct
- [ ] Instructions clear and concise
- [ ] Knowledge base informative
- [ ] Achievement descriptions motivating

### Integration 集成
- [ ] Parts spawn correctly from data table
- [ ] Tutorial steps load sequentially
- [ ] Knowledge entries display properly
- [ ] Achievements trigger correctly
- [ ] String table localization works

---

## Content Expansion 内容扩展

### Adding More Parts 添加更多零件

To expand to 20+ parts, consider adding:
- Advanced avionics systems
- Additional RCS thrusters
- Heat shield components
- Solar panel arrays
- Communication antennas
- Backup systems

### Adding More Knowledge 添加更多知识

Additional topics:
- Orbital mechanics
- Rocket propellant chemistry
- Materials science
- Computational fluid dynamics
- Mission planning
- Space law and regulations

---

**Content Creation Complete! Ready for Implementation in Unreal Editor! 内容创建完成！准备在虚幻编辑器中实施！📊**
