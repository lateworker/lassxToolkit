# RealESR Tools

## *Version 2.7.5 Release*

### AI图片放大工具, 支持自动遍历文件夹, 支持批量处理.

### **注意:RealESR Tools只适用于Windows10和Windows11**

### 特点

- 可以批量放大图片.
- 可以指定任意文件夹.
- 可以自动创建文件夹.
- 可以还原图片放大前的文件夹结构.

### 解压&部署

#### 第一步: 下载文件

![第一步](https://github.com/Tptr-lateworker/RealESR_Tools/blob/main/output/1.jpg)

#### 第二步: 解压文件

![第二步](.\output\2.jpg)

![第二步](.\output\3.jpg)

#### 第三步: 解压RealESR模型

![第三步](.\output\4.jpg)

![第三步](.\output\5.jpg)

![第三步](.\output\6.jpg)

![第三步](.\output\7.jpg)

![第三步](.\output\8.jpg)

![第三步](.\output\9.jpg)

#### 第四步: 检查目录结构

![第四步](.\output\11.jpg)

#### 第五步: 运行根目录下RealESR_Tools程序

![第五步](.\output\10.jpg)

### 使用方法

- #### 主程序: RealESR_Tools.exe

  ```c++
  Version: 2.7.5 Release					// 版本号
  Process									// 开始处理图片
  Exit									// 退出程序
  
  Settings								// 设置板块
  - RealESR Mode:	[Your RealESR Mode]		// 设置RealESR模型
  - Input Path:	[Your Input Path]		// 设置输入文件或文件夹路径
  - Output Path:	[Your Output Path]		// 设置输出文件夹路径
  - Selector:		[Your Selector]			// 设置筛选器
  Save									// 保存设置
  Reset									// 将设置恢复为默认值
  ```

- #### 设置板块详解

  以下内容中 "输入文本" 代表更改配置时, 使用者用键盘输入的内容, 与输入输出文件夹此类名称无关.

  - #### RealESR模型

    - `realesrgan-x4plus`
    
    - `realesrgan-x4plus-anime`
    - `realesrnet-x4plus`

    - realESR使用显卡放大图片, 处理速度取决于显卡算力. 

    - 速度上，`realesrgan-x4plus-anime > realesrnet-x4plus > realesrgan-x4plus`.
    - 真实性上，`realesrgan-x4plus > realesrnet-x4plus > realesrgan-x4plus-anime`.
    
    - 若没有独显或独显很弱, 建议使用`realesr-x4plus-anime`模型.

  - #### 输入文件或文件夹路径

    - 输入文件夹路径就是待处理图片的存放路径.

    - 程序会读取输入文本中的空格.

    - 程序会自动忽略输入文本开头的双引号.

    - 程序会自动忽略输入文本末尾的双引号和右斜杠.

    - 如果输入文本是一个文件路径, 则程序会单独处理此文件而不是整个文件夹.

    - **如果输入文本是一个文件路径, 则程序将会在**`[Your Input Path]`**中显示文件所在文件夹的路径.**
    
    - **输入文件或文件夹必须已经存在.**
    
    - **请勿使用左斜杠表示路径**.
    
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
      
      - **请勿使用左斜杠表示路径**.
      
      - **输入文件夹原则上不能与输出文件夹在同一路径**.
      
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
    - RealESR Mode:	realsrgan-x4plus
      - Input Path:	.\input
      - Output Path:	.\output
      - Selector:		*
    ```


### Tips

- 如果不想手动输入文件夹路径, 可以右键文件或文件夹, 点击 "复制文件地址(A)" 按钮, 直接粘贴进程序输入框. 或者可以直接将文件或文件夹拖入程序输入框.
- 与程序同级的`input`和"`output`"文件夹为示例文件夹, 若图片存放于其它文件夹, 就可以将其删除.
- Tptr在程序里埋藏了7个彩蛋, 若全部找出可以B站[@Tptr-lateworker](https://space.bilibili.com/454920362)交作业哦.

