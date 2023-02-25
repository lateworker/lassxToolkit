# RealESR Tools

## *Version 3.0.1 Release*

### AI图片放大工具, 支持自动遍历文件夹, 支持批量处理.

### **注意:RealESR Tools只适用于Windows10和Windows11**

### 更新内容

- 支持自定义语言包.
- 加入了更多模型.
- 将`realesrgan-anime`模型替换为Ram版本
- 修复了配置文件不存在导致的错误

### 特点

- 可以批量放大图片.
- 可以指定任意文件夹.
- 可以自动创建文件夹.
- 原生支持保留文件夹结构.
- 支持自定义语言包.
- 整合了所有主流的图片超分辨率模型

### 解压&部署

从https://github.com/Tptr-lateworker/RealESR_Tools/releases 下载最新版RealESR.Tools.7z并解压

进入目录



### 使用方法(英文)

- #### 主程序: RealESR_Tools.exe

  ```c++
  Version: 2.7.5 Release			      		// 版本号
  Process								        // 开始处理图片
  Exit								       	// 退出程序
  
  Settings						            // 设置板块
  - AI Mode:	[Your RealESR Mode]	         	// 设置AI模型
  - Upscale Ratio:[Your Upscale Ratio]		// 设置放大倍数
  - Input Path:	[Your Input Path]	    	// 设置输入文件或文件夹路径
  - Output Path:	[Your Output Path]	    	// 设置输出文件夹路径
  - Selector:		[Your Selector]		     	// 设置筛选器
  - Denoise Level:[Your Denoise Level]		// 设置降噪等级(有些模型没有这个选项)
  Save								      	// 保存设置
  Reset							       		// 将设置恢复为默认值
  ```

- #### 设置板块详解

  以下内容中 "输入文本" 代表更改配置时, 使用者用键盘输入的内容, 与输入输出文件夹此类名称无关.

  - #### AI 模型

    - `realesrgan`

    - `realesrnet`

    - `realesrgan-anime`

    - `realcugan`

    - `waifu2x-anime`

    - `waifu2x-photo`

    - `DF2K`

    - `DF2K-JPEG`

    - AI 模型使用显卡放大图片, 处理速度取决于显卡算力和图片分辨率. 

    - 模型之间有一定的差异, 具体可见`testimagine.7z`.

    - `realesrgan-anime`模型适合超分动漫图片.

    - `realesrnet`&`waifu2x-photo`模型适合超分真实图片.

    - 若没有独显或独显很弱, 建议使用`realesrgan-anime`模型. 此模型处理速度最快.
    
    - `realesrgan-anime`模型为Ram版本, 没有独显也可以超分大图片.
    
    - 除`realesrgan-anime`以外所有模型都是非Ram版本, 显存和内存不足8G可能会导致崩溃, 通常在处理30MB以上或者10^8(一亿) 像素以上会崩溃.
    
    - 显卡测试1: RX588 ARCAEA-8K-HKT.png 16MB 7680*4320 x4plus模型-用时30min Anime模型-用时14min. ~~这是Tptr的朋友提供的~~
    
    - 注意: 如果使用独显, 在任务管理器中看不到显卡占用/NA软件中占用满/集显可以直接看到.
    
    - **对真实图片进行超分不能使用anime模型 !**
    
      
    
    - #### 输入文件或文件夹路径
  
      - 输入文件夹路径就是待处理图片的存放路径.
  
      - 程序会读取输入文本中的空格.
  
      - 程序会自动忽略输入文本开头的双引号.
  
      - 程序会自动忽略输入文本末尾的双引号和右斜杠.
  
      - 如果输入文本是一个文件路径, 则程序会单独处理此文件而不是整个文件夹.
  
      - **如果输入文本是一个文件路径, 则程序将会在**`[Your Input Path]`**中显示文件所在文件夹的路径.**
  
      - **输入文件或文件夹必须已经存在.**
  
      - **输入文件夹原则上不能与输出文件夹在同一路径**.
  
      - 合法的输入文本: 
  
        ```
        .\input
        .\input\"
        "C:\
        ".\input"
        "C:\a.jpg"
        ```
  
    - #### 输出文件夹路径
  
        - 输出文件夹路径就是存放处理完成的图片的文件夹路径. 
  
        - 程序会读取输入文本中的空格.
  
        - 程序会自动忽略输入文本开头的双引号.
  
        - 程序会自动忽略输入文本末尾的双引号和右斜杠.
  
        - 输出文件夹可以不存在.
  
        - **输入文件夹原则上不能与输出文件夹在同一路径. 否则程序会直接覆盖旧文件.**
  
        - 合法的输入文本:
  
          ```
          .\output
          "C:\
          ```
  
    - #### 筛选器
  
      - 筛选器可以实现只处理输入文件夹中某一种格式的文件.
  
      - 暂时不支持同时筛选多种格式处理, 但可以全部处理(即不筛选, 用*实现).
  
      - **只需要输入文件格式即可. 请勿填入任何多余的符号**.
  
      - **如果在输入文件或文件夹路径中填入单一文件路径, 则程序会在**`[Your Selector]`**中显示文件名**.
  
      - 合法的输入文本:
  
          ```
          *		// 填*代表不筛选, 处理全部文件
          jpg
          png
          webp
          ```
  

  

  - #### 默认值

      ```
    - RealESR Mode:		realesrgan-anime
    - Upscale Ratio:	4x
    - Input Path:		.
    - Output Path:		.
    - Selector:			*
    ```



### Tips

- 如果不想手动输入文件夹路径, 可以右键文件或文件夹, 点击 "复制文件地址(A)" 按钮, 直接粘贴进程序输入框. 或者可以直接将文件或文件夹拖入程序输入框.
- **Win10系统"复制文件路径"按钮在文件管理器上方标题栏中, 不在右键里!!**