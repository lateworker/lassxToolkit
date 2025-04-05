# 注意！本工具箱的后续开发将转至另一仓库，本仓库内的所有内容将作为历史版本归档。

# lassxToolkit

AI图片放大工具，整合了多种模型，支持自动遍历文件夹，支持批量处理。

## Version 4.1.a Beta

2024.2.19 update（标粗为版本升级带来的变动）：

- **修改了输入路径的判断方法（判断 文件 / 文件夹）**

- 修正了 xcopy 的 /-i 开关在旧版Windows系统中无法使用的问题

- 修正了程序在 pos_slash 变量为 infinity 时（输入路径中没有 ”\\“ ）的异常行为。

### **注意！RealESR_Tools 已正式更名为 lassxToolkit！早于 4.0.0 的版本暂时不受影响！可自行去 Release 中下载**

Version 4.1.a Beta 为内测版本，没有UI界面（需要手动修改配置文件）。有任何 ‘使用方面的问题 / BUG’ 应在 ‘[Github](https://github.com/lateworker/lassxToolkit) / QQ群（935718273）/ ~~洛谷~~ ’ 提出。

## 使用方法

**从此版本起，使用时应将配置文件拖入主程序，而不是直接打开主程序！**

也就是说，程序不再局限于单个配置文件。

## 配置文件介绍

配置文件为严格的 .json 文件。任何格式错误都会导致 程序在控制台输出 "ERROR" 并跳过此配置。

接下来会介绍配置文件的内容、默认值，以及与之相关的必要的知识。

- 每个配置前的括号内标明了该配置的属性。

- **带 \*号的内容会在后文中详细解释。**

- 加粗的字符串为对应其功能的键名（key name）。

如有不清晰，可参见文末的测试用配置文件。

### 以下是配置文件的内容：

- （必填，String）**input_path**：输入 文件 / 文件夹 的路径

- （必填，String）**output_path**：输出 文件 / 文件夹 的路径

- \*（选填，String）**error_path**：错误文件夹的路径

- *（选填，String）**selector**：选择器, 用来匹配文件
  
  - 文件夹遍历时会用到选择器。

- *（必填，String）**model**：选用的AI模型

- （必填，String）**scale**：模型的放大倍数

- （选填，String）**denoise**：模型的降噪等级
  
  - realesr 系列模型不支持降噪等级，这时此项无意义。

- （选填，String）**syncgap**：图片分块等级
  
  - 我也不知道有什么效果（也许可以加速处理？），但有的模型需要这个参数。
  
  - 只有 realcugan 模型支持图片分块等级，其它情况下此项无意义。

- *（必填，Boolean）**tree_restore**：是否还原目录结构

- （必填，Boolean）**subdir_find**：是否递归处理子目录

- （选填，Boolean）**emptydir_find**：是否处理 不包含目标文件 的目录
  
  - 如果将此项设为 true，程序就会根据输入文件夹的结构，生成完全相同的输出文件夹（可能有些输出文件夹为空）。某种意义上，程序处理了空的文件夹（相比于那些包含了待处理文件的），因此将此项命名为 emptydir_find（空目录寻找）。
  
  - 可以帮你还原整个目录结构，即使不处理全部文件。

- *（选填，Boolean）**file_error**：是否处理文件错误

- *（选填，Boolean）**dir_error**：是否处理文件夹错误
  
  - 一般来说，dir_error 完全包含 file_error。

- *（选填，Array）**match**：正则表达式形式的黑名单
  
  - 可以筛选文件的名称（"name"）和目录（"path"）。
  
  - 现有完全匹配（"match"），部分匹配（"search"）两种模式。

- *（选填，Object）**addons**：插件模块
  
  - 自己写的插件要放到 项目根目录下 addons 文件夹里，填入文件名即可。

### 选填项默认值

```json
"error_path": "" // 空字符串
"selector": "\*" // 用于筛选所有文件
"denoise": "0"
"syncgap": "0"
"emptydir_find": false // 这一项一般用不到
"file_error": true
"dir_error": false
"match": null
"addons": null
```

### 模型介绍

以下是模型列表和一些注意事项（加粗为模型的默认值，仅为参考）：

- DF2K / DF2K-JPEG（realesr一代模型）
  
  - 放大倍数：2 / **4**

- realesrgan / realesrnet（realesr二代模型）
  
  - 放大倍数：2 / **4**

- realesrgan-anime（realesr三代模型，使用内存）
  
  - 放大倍数：2 / 3 / **4**

- realcugan
  
  - 放大倍数：1 / **2** / 3 / 4
  
  - 降噪等级：**-1** / 0 / 1 / 2 / 3
  
  - 分块等级：0 / 1 / 2 / **3**

- waifu2x-anime / waifu2x-photo（waifu2x模型）
  
  - 放大倍数：1 / **2** / 4 / 8 / 16 / 32
  
  - 降噪等级：-1 / **0** / 1 / 2 / 3

- realesr 系列（realesrgan / realesrnet / realesrgan-anime / DF2K / DF2K-JPEG）模型不支持 denoise（降噪等级），这时此项无意义。

- 只有 realcugan 模型支持 syncgap（图片分块等级），其它情况下此项无意义。

虽然模型有默认值，但配置文件内还是要手动设置。

**注意事项：**

- AI 模型使用显卡放大图片，处理速度取决于显卡算力和图片分辨率。

- 模型之间有一定的差异，具体可见 testimagine.7z 压缩包。

- realesrgan-anime 模型适合超分动漫图片。

- realesrnet & waifu2x-photo 模型适合超分真实图片。

- 若没有独显或独显很弱，建议使用 realesrgan-anime 模型。此模型处理速度最快。

- realesrgan-anime 模型为Ram版本（内存版本），没有独显也可以超分大图片。独显性能不受影响。

- 除 realesrgan-anime 以外所有模型都是非Ram版本，显存和内存不足8G可能会导致崩溃。通常在处理 ’$30MB\ /\ 10^8$（一亿）像素‘ 以上的图片时会崩溃。

- 显卡测试1: RX588 ARCAEA-8K-HKT.png 16MB 7680*4320 
  
  - realesrgan模型-用时30min 
  
  - realesrgan-anime模型-用时14min.
 
    感谢 LJX 提供性能数据。

- 注意：关于显存占用问题，
  
  - 如果使用独显, 在任务管理器中看不到显卡占用；在NA软件中显示占用满。
  
  - 集显可以直接在任务管理器中看到。

- **对真实图片进行超分不能使用anime模型！** 

### 选择器（selector）介绍

选择器决定了程序在遍历文件夹、搜索输入文件时得到的结果。它是一个由Windows通配符和普通字符组成的字符串，在使用上和Windows资源管理器（explorer.exe）的搜索框相同。

简单说，"?"（英文问号）可以代替文件路径中任意的**一个**字符，"\*"（乘号 / 星号）可以代替路径中任意的**一个或多个**字符。比如：

- "\*"：所有文件 / 文件夹

- "\*.jpg"：所有 .jpg 格式的文件

- "a*.jpg"：所有以 'a' 开头， .jpg 格式的文件

更多细节可以参考微软官方文档：[通配符简介 - PowerShell | Microsoft Learn](https://learn.microsoft.com/zh-cn/powershell/module/microsoft.powershell.core/about/about_wildcards?view=powershell-7.4)

### tree_restore（还原目录结构）参数介绍

**tree_restore & subdir_find & emptydir_find** 三个参数功能相似，这里只对 tree_restore 参数展开描述。

tree_restore 参数功能很简单，当它为 true 时，程序会自动在输出文件夹内建立子文件夹，并将文件依次丢回原来的子文件夹里。举个例子：

- 假如输入文件夹内文件结构如下：

```
  Input/
   ├── dir1/
   │    ├── A.jpg
   │    └── B.png
   ├── dir2/
   │   ├── dir3/
   │   │    └── C.jpeg
   │   └── D.txt
   └── E.webp
```

- 那么，如果 tree_restore = true 并且只处理图片的话，输出文件夹内是这样的：

```
  Input/
   ├── dir1/
   │    ├── A.jpg
   │    └── B.png
   ├── dir2/
   │    └── dir3/
   │         └── C.jpeg
   └── E.webp
```

- 反之，如果 tree_restore = false，其他条件不变，输出文件夹内是这样的：

```
  Input/
   ├── A.jpg
   ├── B.png
   ├── C.jpeg
   └── E.webp
```

很直观，此参数会直接影响输出文件夹结构。另外两个参数就是字面意思，这里不做解释。

### 错误处理模块介绍（包括 error_path，file_error，dir_error）

一般来说，程序的错误分为以下四种：

- 当 ‘输出文件夹和输入文件夹间 存在名称相同完全的文件’ 时，构成 ‘标准错误’ 。

- 当 ‘配置文件不存在 / json 格式错误 / 缺少必填配置’ 时，构成‘ 配置文件错误’ 。

- 当 ‘模型崩溃 / 程序卡死 / 文件、文件夹输出有误’ 时，构成 ‘程序运行错误’ 。

- 当 ’插件无法调用 / 插件失效 / 文件路径表示出错‘ 时，构成 ’其它功能错误‘ 。

程序内置了错误处理模块，以尽可能避免错误。由于第二至四项错误都 极少见 或由 ‘使用者操作不当 / 代码功能不完善’ 导致，无法预测，因此程序只提供了标准错误的解决方案和配置空间。

#### error_path

一个文件夹路径，该文件夹用于保存输出文件夹内的错误文件，相当于一个备份文件夹。

该文件夹一般位于输入或输出文件夹下，名称随意。比如：

```json
"input_path": "D:\input"
"output_path": "D:\output"
"error_path": "D:\output\ERROR" // 在输出文件夹下
```

当然，你也可以让 该错误文件夹 设为 输出文件夹。这样做，输出文件夹内的错误文件就会被直接覆盖。

**当 file_error 或 dir_error 其中一项为 true 时，error_path 便不能为空！否则后果自负！**

~~没测过，不知道会发生什么。。可能是文件泄露？~~

#### file_error

是否备份错误的文件。推荐设为 true。

#### dir_error

是否备份错误的文件夹，默认为 false。

此选项在最终效果上完全覆盖 file_error 选项。也就是说，

```json
"file_error": false,
"dir_error": true
```

完全等同于

```json
"file_error": true,
"dir_error": true
```

### 正则表达式匹配介绍

lassxToolkit 于 Version 4.0.0.b Beta 版本加入正则表达式功能。此功能在效果上与普通正则表达式无异，是一个**黑名单**，可以借此筛掉一些不需要处理的文件。

正则表达式的详细介绍可以参见菜鸟教程：[正则表达式教程 | 菜鸟教程](https://www.runoob.com/regexp)

如果想检查正则表达式的语法，可以去这个网站（英文）：[正则表达式测试 | Regex101](https://regex101.com/)

接下来介绍 match 键配置内的格式要求：

```json
"match": [ // "match" 为正则表达式匹配模块的键名
    {
        "name": "RegexRule1", // "name"键用来筛选文件名
        "path": "RegexRule2", // "path"键用来筛选路径（不包括文件名）
        "name_mode": "MatchMode1",
        "path_mode": "MatchMode2"
    }, // RegexRule1 第一个正则表达式配置
    {...}, // RegexRule2 注意Object外的逗号！
    .
    .
    .
]
```

如上文所示，match 键内含若干个Object类型的子类，子类中接收 "name" & "path" & "name_mode" & "path_mode" 四个键值对。

MatchMode（匹配模式）现有 "search"（搜索）、"match"（完全匹配）两种。

- "match" 模式要求，字符串（文件名 / 路径）必须与正则表达式完全匹配。

- "search" 模式要求，只要字符串中的某一子串与正则表达式匹配就视为匹配成功。

各个子类内部执行 and（与）逻辑，子类之间执行 or（或）逻辑。

### 插件模块介绍

lassxToolkit 于 Version 4.0.0.b Beta 版本同时引入了插件功能。

**所有自己写的插件都要放到项目根目录下 addons 文件夹里！**

可运行的程序都可以作为插件被主程序调用。

"addons"（插件）模块细分为三种键名，分别为：

- "first"：这组插件在 ‘程序开始时 & 所有图片未做任何处理时’ 被调用。
  
  - 插件需接收命令行传入的一个参数。
  
  - 此参数为一个 Json 格式的字符串，内含配置文件中的所有内容。

- "second"：**每处理**一个 ‘文件 / 文件夹’，这组插件就会被调用一次。
  
  - 插件需接收命令行传入的**两个**参数。
  
  - 第一个参数的形式和意义与 "first" 插件相同。
  
  - 第二个参数为 **刚刚**处理完成的 ‘文件 / 文件夹’ 路径。

- "third"：这组插件在所有图片处理完成后被调用。
  
  - 插件需接收命令行传入的**两个**参数。
  
  - 第一个参数的形式和意义与 "first" 插件相同。
  
  - 第二个参数为一个 Json 格式的字符串，内含本次处理中所有 ‘文件 / 文件夹’ 路径。

以上三种键名都对应着一个 Array（数组）作为值。

插件模块格式如下：

```json
"addons": { // "addons" 为插件模块的键名
    "first": [
        "a.exe", "b.exe" ... // first组的插件列表，填入文件名即可
    ],
    "second": [ ... ], // second组的插件列表
    "third": [ ... ] // third组的插件列表
}
```

软件自带 "print.exe" 插件，此插件可以输出命令行传入的字符串。可以借此自行测试并理解插件调用机制。

## 测试用配置文件

```json
{
    "input_path": "D:\\input",
    "output_path": "D:\\output",
    "error_path": "D:\\error", // error 文件夹也可以放在其他地方
    "selector": "*", // 处理所有文件
    "model":"realesrgan-anime", // anime模型，处理动漫图片
    "scale":"4", // 放大倍数
    "denoise":"0", // 降噪等级
    "syncgap":"0", // 分块等级
    "tree_restore": true, // 还原目录结构
    "subdir_find": true, // 处理子文件夹
    "emptydir_find": false, // 不处理空文件夹
    "file_error": true, // 处理文件标准错误
    "dir_error": false, // 不处理文件夹标准错误
    "match": [
        // 所有含有字母D的文件都不会被处理
        { "name": "D", "name_mode": "search" }
    ],
    "addons": {
        "first": [],
        "second": [],
        "third": [
            "print.exe" // 调用 print.exe 作为第三组插件，查看参数传递情况
        ]
    }
}
```
