# rage-exec

JavaScript executor for RAGE Multiplayer. This source was owned by **soulcheats** and **Savage** in the past.

## Compile

Clone this repository using `git clone --recurse-submodules -j8 https://github.com/Keiichi32/rage-exec.git`

Navigate to the cloned folder and open `rage-exec.sln` with Visual Studio, and build it in Release/x64.

## Requirements

1) RAGE Multiplayer installed at **C:/RAGEMP**
2) Older **[multiplayerL.dll](https://github.com/Keiichi32/rage-exec/releases/download/v1.0/multiplayerL.dll)** in the same directory as **rage-inj.exe**
3) File(s) to execute at **C:/rage-exec**

## Usage

1) Start RAGE Multiplayer Launcher
2) Start rage-inj.exe
3) Connect to a server (should be **0.3.7** or else it won't work)
4) Wait for GTA5 to pop up and start loading
5) Type in the file to execute into the **rage-inj.exe** console
6) Wait for RAGEMP to load and it will be executed

## Troubleshooting

If it doesn't work on a specific server then change [this line](https://github.com/Keiichi32/rage-exec/blob/9c2e0410f7449d70d9524c9e37f5126fe9e34e34/rage-exec/main.cpp#L22) to:
```cpp
int max_count = 4;
```
then compile again.
