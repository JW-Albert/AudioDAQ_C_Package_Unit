# AudioDAQ C Package Unit

AudioDAQ C Package Unit 是一個用於音訊數據採集與處理的 C 語言工具包。該專案提供了輕量、高效的函式庫，方便開發者在嵌入式系統和應用程式中進行音訊數據的處理。
系統環境： Debian 12

## 功能特點

- 支援多種音訊格式的讀取與處理
- 提供靈活的 API 以實現定制化功能
- 高效的數據採集與處理流程
- 可擴展性強，適用於多種應用場景

## 安裝方式

要使用此工具包，請按照以下步驟進行安裝：

1. **克隆此專案到本地目錄：**
   ```bash
   git clone https://github.com/JW-Albert/AudioDAQ_C_Package_Unit.git
   ```

2. **進入專案目錄：**
   ```bash
   cd AudioDAQ_C_Package_Unit
   ```

3. **編譯專案：**
   使用 `make` 進行自動化編譯：
   ```bash
   make
   ```

4. **運行程式：**
   編譯完成後，您可以直接運行可執行檔案。
   ```bash
   ./nidaq
   ```

## 文件結構

專案包含以下主要目錄與檔案：

- `main.cpp`：包含工具包的實作檔案。
- `Makefile`：G++ 編譯文件。
- `include/`：包含所有公共標頭檔案。
- `output/`：請確保這個資料夾是存在的，否則資料會無法存儲。
- `API/`：ini 組態檔存放位置。
- `Makeconfig/`：ini 組態檔製作器(需要用 make 編譯)。

## ALSA 設置指南

### 1. 安裝 ALSA 開發套件

在 Linux 系統中，使用 ALSA 來開發音訊應用程式之前，首先需要安裝 ALSA 開發相關的套件。這些套件包括 `libasound2-dev`，它包含了 ALSA 的開發庫和標頭檔案。

#### 安裝指令
```bash
sudo apt update
sudo apt install libasound2-dev
```
- `libasound2-dev`：這是 ALSA 的開發庫，包含了你需要的標頭檔案和靜態庫/動態庫。

### 2. 設定音訊設備權限

在 Linux 系統上，通常需要對音訊設備擁有適當的權限才能進行錄音或播放。這通常涉及到將用戶添加到 `audio` 群組中，這樣用戶才能訪問音訊設備。

#### 將用戶添加到 `audio` 群組

1. 確認 `audio` 群組是否存在：
   ```bash
   grep audio /etc/group
   ```

2. 將當前用戶添加到 `audio` 群組：
   ```bash
   sudo usermod -aG audio $USER
   ```

3. 重新連接 SSH
   ```bash
   ssh username@hostname
   ```

4. 列出音訊設備
   ```bash
   aplay -l
   ```

此步驟會確保你的用戶有權限訪問音訊設備，例如 `/dev/snd/` 下的音訊設備。

### 3. 編譯程式

使用 `gcc` 編譯這個程式，你需要告訴編譯器鏈接 ALSA 庫。你可以使用以下命令：

#### 編譯指令
```bash
g++ -o main main.cpp -lasound
```
- `main.cpp` 是你的 C++ 程式文件。
- `-o main` 指定編譯後的輸出檔案名稱。
- `-lasound` 是鏈接 ALSA 庫的參數，告訴編譯器在編譯過程中鏈接 `libasound.so`。

### 4. 故障排除

- **音訊設備權限問題**：
  如果你無法訪問音訊設備，請確保你的用戶已經被添加到 `audio` 群組中。
  
  使用以下命令檢查你的用戶是否在 `audio` 群組中：
  ```bash
  groups $USER
  ```
  如果沒有 `audio` 群組，可以使用以下指令將用戶添加到 `audio` 群組：
  ```bash
  sudo usermod -aG audio $USER
  ```

- **找不到音訊設備**：
  請檢查系統中是否有音訊設備，並確保 ALSA 配置正確。

### 5. 參考資料

- [ALSA 官方文檔](https://www.alsa-project.org/)
- [ALSA 開發者文檔](https://www.alsa-project.org/wiki/Docs)

## 貢獻

我們歡迎所有對音訊處理有興趣的開發者參與貢獻此專案。若有改進建議或問題，請提交 issue 或 pull request。
